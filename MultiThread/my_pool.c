#include "my_pool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>


void *eachthread(void *p)
{
    tpool *pool=p;
    job *job1;

    bool con1;
    bool con2;

    /////////////////////////////////////////////////////////////////////
    while (true)
    {


        pthread_mutex_lock(&(pool->mylock));

        //
        while ( (pool->shutdown==0) && (pool->head == NULL))
        {
            pthread_cond_wait(&(pool->Acond), &(pool->mylock));
        }

        if (pool->shutdown==1)
        {
            break;
        }

        job1 = pool->head;
        if (job1 != NULL)
        {
            if (job1->next == NULL)
            {
                pool->head = NULL;
                pool->tail  = NULL;
            }else
            {
                pool->head = job1->next;
            }
            pool->jc++;
        }
        ///////////////////////////////////////////////////////
        if (job1!= NULL)
        {
            job1->myfunc(job1->arg);

        }



        pthread_mutex_unlock(&(pool->mylock));



        pthread_mutex_lock(&(pool->mylock));
        if(job1!=NULL)
        {
            free(job1);
            pool->jc--;
        }




        if(pool->jc== 0)
        {
            con1=true;
        }else
        {
            con1=false;
        }

        if(pool->head == NULL)
        {
            con2=true;
        }else
        {
            con2=false;
        }



        //////////////////////////////////////////////////////


        if (con1 && con2)
        {
            pthread_cond_signal(&(pool->Bcond));
        }
        pthread_mutex_unlock(&(pool->mylock));
    }
    //////////////////////////////////////////////////////////////


    pool->tc--;
    pthread_cond_signal(&(pool->Ccond));
    pthread_mutex_unlock(&(pool->mylock));
}
























/////////////////////////////////////////////////////////////////////////////
void tpool_add(tpool *pool, void *(*func)(void *), void *arg)
{
    // TODO
    job *job1;

    job1    = malloc(sizeof(*job1));
    job1->myfunc = func;
    job1->arg  = arg;
    job1->next = NULL;

    pthread_mutex_lock(&(pool->mylock));
    if (pool->head != NULL)
    {
        pool->tail->next=job1;
        pool->tail=job1;
    } else
    {
        pool->head = job1;
        pool->tail = job1;
    }

    pthread_cond_broadcast(&(pool->Acond));
    pthread_mutex_unlock(&(pool->mylock));

}


void tpool_wait(tpool *pool)
{

    pthread_mutex_lock(&(pool->mylock));
    bool con3;
    bool con4;



    while (true)
    {
        if(pool->jc==0)
        {
            con3=true;
        }else
        {
            con3=false;
        }

        if(pool->head==NULL)
        {
            con4=true;
        }else
        {
            con4=false;
        }






        if( (!con4) || (!con3) )
        {
            pthread_cond_wait(&(pool->Bcond), &(pool->mylock));
        } else
        {
            break;
        }
    }


    ////////////////////////////////


    /////////////////////////////////

    pthread_mutex_unlock(&(pool->mylock));
}


void wait2(tpool *pool)
{
    pthread_mutex_lock(&(pool->mylock));

    pool->shutdown = 1;
    pthread_cond_broadcast(&(pool->Acond));


    while(true)
    {
        if(pool->tc != 0)
        {
            pthread_cond_wait(&(pool->Ccond), &(pool->mylock));
        }else
        {
            break;
        }
    }

    pthread_mutex_unlock(&(pool->mylock));
}



void tpool_destroy(tpool *pool)
{






    wait2(pool);

    pthread_cond_destroy(&(pool->Acond));
    pthread_cond_destroy(&(pool->Bcond));
    pthread_cond_destroy(&(pool->Ccond));
    pthread_mutex_destroy(&(pool->mylock));

    free(pool);
}





tpool *tpool_init(int n_threads)
{
    // TODO
    tpool   *pool;

    pool = malloc(sizeof(*pool));

    pool->tc = n_threads;
    pool->jc = 0;
    pool->shutdown = 0;


    pthread_cond_init(&(pool->Acond), NULL);
    pthread_cond_init(&(pool->Bcond), NULL);
    pthread_cond_init(&(pool->Ccond), NULL);
    pthread_mutex_init(&(pool->mylock), NULL);

    pool->head = NULL;
    pool->tail  = NULL;

    pthread_t  tid;
    for (int i=0; i<n_threads; i++)
    {
        pthread_create(&tid, NULL, eachthread, pool);
        pthread_detach(tid);
    }
    return pool;
}
