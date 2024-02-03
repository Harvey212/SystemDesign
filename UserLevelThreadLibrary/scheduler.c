#include "threadtools.h"
#include <sys/signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


///////////////////////////////////////////////////////////
/*
 * Print out the signal you received.
 * If SIGALRM is received, reset the alarm here.
 * This function should not return. Instead, call siglongjmp(sched_buf, 1).
*/
////////////////////////////////////////////////////////////
void sighandler(int signo)
{
    // TODO
    /////////////////////////////////////////////////////
    //In this assignment, we repurpose two signals to help us perform context-switching.
    //SIGTSTP can be triggered by pressing Ctrl+Z on your keyboard,
    //and SIGALRM is triggered by the alarm syscall.

    //This function should only be triggered as a result of thread_yield.
    //Under no circumstances should you call this function explicitly in your code.
    //
    //Upon executing this function, it should print one of the following lines to the standard output:
    //caught SIGTSTP
    //caught SIGALRM
    //
    //If the signal caught is SIGALRM, you should reset the alarm here.
    //Then, you should jump to the scheduler with longjmp(sched_buf, 1).
    //
    //////////////////////////////////////////////////////
    //Both SIGTSTP and SIGALRM should be blocked when the threads are executing.

    if (signo == SIGTSTP)
    {
        printf("caught SIGTSTP\n");
        sigprocmask(SIG_BLOCK, &tstp_mask, NULL);

    }else if (signo == SIGALRM)
    {
        printf("caught SIGALRM\n");
        sigprocmask(SIG_BLOCK, &alrm_mask, NULL);
        alarm(timeslice);
    }
    siglongjmp(sched_buf, 1);
}



/////////////////////////////////////////////////////////////
void myclear(int idx, int which)
{

    if(which==0)//ready queue
    {
        ready_queue[idx] = NULL;
    }else//waiting queue
    {
        waiting_queue[idx] = NULL;
    }
}
///////////////////////////////////////////////////////////







//////////////////////////////////////////////////////////////////////////////
/*
 Prior to calling this function, both SIGTSTP and SIGALRM should be blocked.
 This file contains the functions that determine the order of execution.

 The scheduler maintains the waiting queue and the ready queue.

 Each time the scheduler is triggered, it decides which thread to run next,
 and brings an available thread from the waiting queue to the ready queue.

 The ready queue and the waiting queue are defined as arrays for simplicity.

 */
 ////////////////////////////////////////////////////////////////////////////
void scheduler()
{
    // TODO
    int see=sigsetjmp(sched_buf,1);
    struct tcb *next_thread;
    struct tcb *last_thread;
    struct tcb *nolock_thread;


    if(see==0)// Called by main.c
    {
        ////////////////////////////////////
        //For the first case, this function should execute the earliest created thread.
        //
        //////////////////////////////////////
        rq_current = 0;
    }else//Otherwise, it should perform the following tasks in order:
    {
    ////////////////////////////////////////////////////////////////////////
        if(wq_size>0)//, but the waiting queue is not empty
        {
            if(bank.lock_owner == -1)//If the lock is available
            {

                //////////////////////////////////////
                //bring the first thread in the waiting queue to the ready queue.
                //And that thread should acquire the lock.
                //
                ////////////////////////////////////////
                next_thread = waiting_queue[0];

                ready_queue[rq_size] = next_thread;
                rq_size++;
                //Then, the holes left in the waiting queue should be filled, while keeping the original relative order.

                for(int i=0;i<wq_size;i++)
                {
                    waiting_queue[i] = waiting_queue[i+1];
                }


                //waiting_queue[wq_size] =NULL;
                myclear(wq_size, 1);
                wq_size--;

                /////////////////////////////////////////////////
                bank.lock_owner = ready_queue[rq_size-1]->id;
                ////////////////////////////////////////////////



                ///////////////////////////////////////////
                ///////////////////////////////////////////////
                //Remove the current thread from the ready queue if needed. There are two cases:
                //For lock, move the thread to the end of the waiting queue.
                //For thread_exit, clean up the data structures you allocated for this thread, then remove it from the ready queue.
                //
                /////////////////////////////////////////////////////////
                //If you have removed a thread in the previous step,
                //take the thread from the end of the ready queue to fill up the hole.
                //For lock and thread_exit, you should execute the thread you used to fill up the hole.
                ////////////////////////////////////////////////////////////
                if(see==3)
                {

                    //ready_queue[rq_current] = NULL;
                    myclear(rq_current, 0);

                    last_thread = ready_queue[rq_size-1];
                    ready_queue[rq_current] = last_thread;

                    //ready_queue[rq_size-1] = NULL;
                    myclear(rq_size-1, 0);
                    rq_size--;
                }


                if(see==2)
                {
                    nolock_thread = ready_queue[rq_current];
                    waiting_queue[wq_size] = nolock_thread;
                    wq_size++;

                    //ready_queue[rq_current] = NULL;
                    myclear(rq_current, 0);
                    ////////////////////////////////////////////////////

                    last_thread = ready_queue[rq_size-1];
                    ready_queue[rq_current] = last_thread;

                    //ready_queue[rq_size-1] = NULL;
                    myclear(rq_size-1, 0);
                    rq_size--;

                }
                //////////////////////////////////////////////////////////////

                if(see==1)
                {
                    ///////////////////////////////////////////
                    //Because
                    //1.priority 1
                    //If the lock is available, but the waiting queue is not empty,
                    //bring the first thread in the waiting queue to the ready queue.
                    //And that thread should acquire the lock.
                    //
                    //2.priority 2
                    //For thread_yield, you should execute the next thread in the queue.
                    //If the thread calling thread_yield, thread_exit or lock is the last thread in queue, execute the first thread.
                    //
                    //
                    ///////////////////////////////////////////////

                    rq_current = (rq_current + 1) % rq_size;
                }
                ////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
            }else//skip priority 1, priority 2
            {



                ///////////////////////////////////////////////////////////////////////////////////
                //////////////////////////////////////////
                //Remove the current thread from the ready queue if needed. There are two cases:
                //For lock, move the thread to the end of the waiting queue.
                //For thread_exit, clean up the data structures you allocated for this thread, then remove it from the ready queue.
                ///////////////////////////////////////////////////

                ///////////////////////////////////
                //If you have removed a thread in the previous step,
                //take the thread from the end of the ready queue to fill up the hole.
                //////////////////////////////////////////

                ////////////////////////////////////////
                //For lock and thread_exit, you should execute the thread you used to fill up the hole.
                //If the thread calling thread_yield, thread_exit or lock is the last thread in queue, execute the first thread.
                //
                ////////////////////////////////////////////




                ///////////////////////////////////////////////////////
                if(see==3)
                {
                    ///////////////////////////////////////
                    //you don't have to check if your id is the same as bank id
                    //because if == , it will start from macro !=0 and continue
                    //if !=, it will start from the beginning, call lock, and go to waiting queue
                    /////////////////////////////////////////////

                    if(rq_current!=(rq_size-1))
                    {
                        //ready_queue[rq_current] = NULL;
                        myclear(rq_current, 0);

                        last_thread = ready_queue[rq_size-1];
                        ready_queue[rq_current] = last_thread;

                        //ready_queue[rq_size-1] = NULL;
                        myclear(rq_size-1, 0);
                        rq_size--;
                    }else
                    {
                        //ready_queue[rq_current] = NULL;
                        myclear(rq_current, 0);
                        rq_size--;
                        rq_current = 0;
                    }

                }

                if(see==2)
                {
                    nolock_thread = ready_queue[rq_current];
                    waiting_queue[wq_size] = nolock_thread;
                    wq_size++;

                    if(rq_current!=(rq_size-1))
                    {
                        //ready_queue[rq_current] = NULL;
                        myclear(rq_current, 0);

                        last_thread = ready_queue[rq_size-1];
                        ready_queue[rq_current] = last_thread;

                        //ready_queue[rq_size-1] = NULL;
                        myclear(rq_size-1, 0);

                        rq_size--;
                    }else
                    {
                        //ready_queue[rq_current] = NULL;
                        myclear(rq_current, 0);
                        rq_size--;
                        rq_current = 0;
                    }

                }

                /////////////////////////////////////////////////////////


                ///////////////////////////////////////////
                //For thread_yield, you should execute the next thread in the queue.
                //If the thread calling thread_yield, thread_exit or lock is the last thread in queue, execute the first thread.
                //////////////////////////////////////////////
                if(see==1)
                {
                    rq_current = (rq_current + 1) % rq_size;
                }



            }
////////////////////////////////////////////////////////////////////////////////////////////////
        }else
        {

            if(see==3)
            {
                if(rq_current!=(rq_size-1))
                {
                    //ready_queue[rq_current] = NULL;
                    myclear(rq_current, 0);

                    last_thread = ready_queue[rq_size-1];
                    ready_queue[rq_current] = last_thread;

                    //ready_queue[rq_size-1] = NULL;
                    myclear(rq_size-1, 0);
                    rq_size--;
                }else
                {
                    /////////////////////////////////////
                    //If the thread calling thread_yield, thread_exit or lock is the last thread in queue, execute the first thread.
                    //////////////////////////////////
                    //ready_queue[rq_current] = NULL;
                    myclear(rq_current, 0);
                    rq_size--;
                    rq_current = 0;
                }
            }

            if(see==2)
            {
                if(bank.lock_owner == -1)
                {
                    rq_current = rq_current;
                    bank.lock_owner = ready_queue[rq_current]->id;
                }else
                {
                    nolock_thread = ready_queue[rq_current];
                    waiting_queue[wq_size] = nolock_thread;
                    wq_size++;

                    if(rq_current!=(rq_size-1))
                    {
                        //ready_queue[rq_current] = NULL;
                        myclear(rq_current, 0);

                        last_thread = ready_queue[rq_size-1];
                        ready_queue[rq_current] = last_thread;

                        //ready_queue[rq_size-1] = NULL;
                        myclear(rq_size-1, 0);

                        rq_size--;
                    }else
                    {
                        /////////////////////////////////////
                        //If the thread calling thread_yield, thread_exit or lock is the last thread in queue, execute the first thread.
                        //////////////////////////////////
                        //ready_queue[rq_current] = NULL;
                        myclear(rq_current, 0);

                        rq_size--;
                        rq_current = 0;
                    }
                }

            }

            if(see==1)
            {
                rq_current = (rq_current + 1) % rq_size;
            }
        }
    }
    ///////////////////////////////////////////////////

    ////////////////////////////////////////
    //When both the ready queue and the waiting queue are empty, return to main.
    //////////////////////////////////////
    if (rq_size == 0 && wq_size == 0)
    {
        return;
    }else
    {
        siglongjmp(RUNNING->environment, 1);
    }
}

