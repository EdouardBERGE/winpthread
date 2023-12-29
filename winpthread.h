#include<windows.h>
#define sem_t           HANDLE
#define pthread_t       HANDLE
#define pthread_mutex_t HANDLE

#include<process.h>

enum e_detached_state {
PTHREAD_CREATE_DETACHED,
PTHREAD_CREATE_JOINABLE
};

struct pthread_attr_t {
	int	stack_size;
	enum e_detached_state detached_state;
};
typedef struct pthread_attr_t pthread_attr_t;

int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_post(sem_t *sem);
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_destroy(sem_t *sem);


int pthread_mutex_init(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
int pthread_attr_getstacksize(pthread_attr_t *attr, size_t *stacksize);
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int pthread_attr_getdetachstate(pthread_attr_t *attr, int *detachstate);
int pthread_create(pthread_t *thread, pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
int pthread_join(pthread_t thread, void **retval);
void pthread_exit(void *retval);

/*EOF*/
