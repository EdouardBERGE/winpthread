#include "winpthread.h"

// very simple wrapper intended to be used as flip/flop
// with sem_init, sem_wait, sem_post, sem_destroy usage only
int sem_init(sem_t *sem, int pshared, unsigned int value) {
	*sem=CreateSemaphore(NULL,value,1,NULL); // attr, initial count, maximum count, unnamed
	if (!sem) return 1; else return 0;
}
int sem_post(sem_t *sem) {
	// do not care about previous count
	if (!ReleaseSemaphore(*sem,1,NULL)) return 1; else return 0;
}
int sem_wait(sem_t *sem) {
	int dwWaitResult;
	dwWaitResult=WaitForSingleObject(*sem,INFINITE);
	if (dwWaitResult==WAIT_OBJECT_0) return 0;
	return 1;
}
int sem_trywait(sem_t *sem) {
	int dwWaitResult;
	dwWaitResult=WaitForSingleObject(*sem,0);
	if (dwWaitResult==WAIT_OBJECT_0) return 0;
	return 1;
}
int sem_destroy(sem_t *sem) {
	if (!CloseHandle(*sem)) return 1; else return 0;
}

int pthread_mutex_init(pthread_mutex_t *mutex)
{
	HANDLE createdmutex;
	
	/* using hex address of mutex as string id */

printf("creating mutex\n");
	createdmutex=CreateMutex(NULL,FALSE,"Grouik");
	*mutex=createdmutex;
	if (*mutex==NULL) {
		printf("impossible d'initialiser le mutex\n");
		return 1;
	} else return 0;
}
int pthread_mutex_lock(pthread_mutex_t *mutex)
{
	pthread_t zehand;
	while ((zehand=OpenMutex(MUTEX_ALL_ACCESS,TRUE,"Grouik"))==NULL) {
		WaitForSingleObject(*mutex,INFINITE);
	}
	return 0;
}
int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
	pthread_mutex_t local_mutex;
	local_mutex=OpenMutex(MUTEX_ALL_ACCESS,FALSE,"Grouik");
	if (local_mutex==NULL) return 1; else return 0;
}
int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	ReleaseMutex(*mutex);
	return 0;
}


int pthread_attr_init(pthread_attr_t *attr)
{
	attr->stack_size=1024*1024;
	attr->detached_state=PTHREAD_CREATE_JOINABLE;
	return 0;
}
int pthread_attr_destroy(pthread_attr_t *attr)
{
	return 0;
}
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
	attr->stack_size=stacksize;
	return 0;
}
int pthread_attr_getstacksize(pthread_attr_t *attr, size_t *stacksize)
{
	*stacksize=attr->stack_size;
	return 0;
}
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
{
	if (detachstate!=PTHREAD_CREATE_DETACHED && detachstate!=PTHREAD_CREATE_JOINABLE) return 1;
	attr->detached_state=detachstate;
	return 0;
}
int pthread_attr_getdetachstate(pthread_attr_t *attr, int *detachstate)
{
	*detachstate=attr->detached_state;
	return 0;
}
int pthread_create(pthread_t *thread, pthread_attr_t *attr,void *(*start_routine) (void *), void *arg)
{
	unsigned int stack_size=1024*1024;
	int detached_state=PTHREAD_CREATE_JOINABLE;
	void (*bounce)(void *);
	
	if (attr) {
		stack_size=attr->stack_size;
		detached_state=attr->detached_state;
	}
	
	bounce=(void (__cdecl *)(void *))start_routine;
	*thread=(pthread_t)_beginthread(bounce,stack_size,arg);

	if (detached_state==PTHREAD_CREATE_DETACHED) {
		CloseHandle(*thread);
		_ReadWriteBarrier();
		*thread=0;
	}
	
	return 0;
}

int pthread_join(pthread_t thread, void **retval)
{
	DWORD exitcode;
	DWORD *retptr;
	WaitForSingleObject(thread,INFINITE);
	GetExitCodeThread(thread,&exitcode);
	CloseHandle(thread);
	retptr=(DWORD *)*retval;
	if (retptr) *retptr=exitcode;
	return 0;
}

void pthread_exit(void *retval)
{
	DWORD *retptr;
	retptr=(DWORD *)retval;
	if (retptr) ExitThread(*retptr); else ExitThread(0);
}

/*EOF*/
