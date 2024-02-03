#ifndef THREADTOOL
#define THREADTOOL
#include <setjmp.h>
#include <sys/signal.h>
#include "bank.h"


#define THREAD_MAX 16  // maximum number of threads created
#define BUF_SIZE 512

///////////////////////////////////////////////////////////////////////////////
//The thread control block (TCB) serves as the storage of per-thread metadata and variables.
//You are allowed to add more variables if needed.
struct tcb
{
    int id;  // the thread id
    jmp_buf environment;  // where the scheduler should jump to
    int arg;  // argument to the function
    int i, x, y;  // declare the variables you wish to keep between switches
    int i2,z;
    /////////////////////////////////////////////////
};
///////////////////////////////////////////////////////////////////////////////////


extern int timeslice;
extern jmp_buf sched_buf;
extern struct tcb *ready_queue[THREAD_MAX], *waiting_queue[THREAD_MAX];
extern struct Bank bank;
/*
 * rq_size: size of the ready queue
 * rq_current: current thread in the ready queue
 * wq_size: size of the waiting queue
 */
extern int rq_size, rq_current, wq_size;
/*
* base_mask: blocks both SIGTSTP and SIGALRM
* tstp_mask: blocks only SIGTSTP
* alrm_mask: blocks only SIGALRM
*/
extern sigset_t base_mask, tstp_mask, alrm_mask;
/*
 * Use this to access the running thread.
 */
#define RUNNING (ready_queue[rq_current])

void sighandler(int signo);
void scheduler();

///////////////////////////////////////
//threads.c defines the threads that are going to run in parallel.
//The lifecycle of a thread should be:
//Created by calling thread_create in main.c.
//Calls thread_setup to initialize itself.
//When a small portion of computation is done, call thread_yield to check if a context switch is needed.
//Call lock() to acquire a lock
//Call unlock() to realease a lock
//After all computations are done, call thread_exit to clean up.
///////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// TODO
#define thread_create(func, id, arg) \
{                 \
    /*
    //Call the function func and pass in the arguments id and arg.
    //We guarantee that the maximum number of threads created is 16, and no two threads share the same ID.
    //This macro also print a line to the standard output:
    //[thread id] [function name]
    */ \
    func(id, arg);\
}
///////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////
#define thread_setup(id, arg) \
{         \
    /*
    //Initialize the thread control block and append it to the ready queue.
    //This macro should also call setjmp so the scheduler knows where to longjump when it decides to run the thread.
    //Afterwards, it should return the control to main.c.

    //Upon the initialization of new threads, a TCB structure should be allocated and appended to the ready queue.
    */ \
    printf("%d %s\n",id,__func__); \
    do \
    { \
        struct tcb *new_thread = (struct tcb *)malloc(sizeof(struct tcb)); \
        new_thread->id = id; \
        new_thread->arg = arg; \
        new_thread->i = 1; \
        new_thread->i2 = 1; \
        new_thread->x = 1; \
        new_thread->y = 1; \
        new_thread->z = 1; \
        ready_queue[rq_size] = new_thread;\
        if (sigsetjmp(ready_queue[rq_size]->environment,1) == 0) \
        {\
            rq_size+=1;\
            return;\
        }\
    }while (0);\
}
///////////////////////////////////////////

//////////////////////////////////////////

#define thread_exit() \
{\
    /*
    //Jump to the scheduler with longjmp(sched_buf, 3).
    */ \
    siglongjmp(sched_buf, 3);\
}
////////////////////////////////////////

#define thread_yield() \
{\
    /*
    ////////////////////////////////////////////////
    //Most computational problem takes several iterations to finish (except Bank Operation has got only 3 steps).
    //After each iteration (step), a thread should use this macro to check if there¡¦s a need to let another thread execute.
    //This macro should save the execution context, then, sequentially unblock SIGTSTP and SIGALRM.
    //
    //If any of the two signals are pending, the signal handler will take over and run the scheduler.
    //If there aren't any signals pending, this macro should block the signals again,
    //then continue executing the current thread.
    /////////////////////////////////////////////////
    */ \
    do \
    { \
        if (sigsetjmp(RUNNING->environment,1) == 0) \
        {\
            sigset_t waiting_mask; \
            sigpending (&waiting_mask); \
            if((sigismember (&waiting_mask, SIGALRM)) && (sigismember (&waiting_mask, SIGTSTP))) \
            { \
                sigprocmask(SIG_UNBLOCK, &tstp_mask, NULL); \
            }else \
            { \
                if (sigismember (&waiting_mask, SIGALRM)) \
                { \
                    sigprocmask(SIG_UNBLOCK, &alrm_mask, NULL); \
                }else if (sigismember (&waiting_mask, SIGTSTP)) \
                { \
                    sigprocmask(SIG_UNBLOCK, &tstp_mask, NULL); \
                }else \
                { \
                    sigprocmask(SIG_SETMASK, &base_mask, NULL); \
                } \
            } \
        } \
    }while (0);\
}

//////////////////////////////////////////////////

////////////////////////////////////////////////


#define lock() \
{                \
    /*
    ////////////////////////////////////////////////
    //Check if the lock is available, if so, acquire the lock and continue.
    //Otherwise, if the lock is held by another thread, save the execution context,
    //then jump to the scheduler with longjmp(sched_buf, 2).
    //After that, this thread should be put to waiting queue by the scheduler.
    //When the lock is available,it will automatically acquire the lock and continue its execution.
    //This is done by the scheduler,
    /////////////////////////////////////////////////
    */ \
    do \
    { \
        if (sigsetjmp(RUNNING->environment,1) == 0) \
        { \
            siglongjmp(sched_buf, 2); \
        } \
    }while (0);\
}
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////

#define unlock()    \
{                   \
    do \
    { \
        /*
        ////////////////////////////////////////////////
        //Release the lock, only the owner of the lock can release it.
        /////////////////////////////////////////////////
        */ \
        if(RUNNING->id==bank.lock_owner) \
        { \
            bank.lock_owner = -1; \
        } \
    }while (0);\
}
//////////////////////////////////////////////////////////


#endif // THREADTOOL
////////////////////////////////////////////////
