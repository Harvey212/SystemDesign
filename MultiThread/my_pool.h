#include <pthread.h>
#ifndef __MY_THREAD_POOL_H
#define __MY_THREAD_POOL_H

typedef void *(*fun)(void *);
//////////////////////////////////////////////////////
typedef struct job
{
    struct job *next;
    fun   myfunc;
    void    *arg;
} job;

///////////////////////////////////////////////////////////

typedef struct tpool
{
    // TODO: define your structure
    //////////////////////////////////////////
    int jc;
    int tc;
    int shutdown;
    pthread_cond_t  Acond;
    pthread_cond_t  Bcond;
    pthread_cond_t  Ccond;
    pthread_mutex_t mylock;
    job *head;
    job *tail;

} tpool;


////////////////////////////////////////////////////

void wait2(tpool *);

tpool *tpool_init(int n_threads);
void tpool_add(tpool *, void *(*func)(void *), void *);
void tpool_wait(tpool *);
void tpool_destroy(tpool *);

#endif
