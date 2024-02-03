#include "threadtools.h"
#include <sys/signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void fibonacci(int id, int arg)
{
    //////////////////////////////////////////////
    //Where id is the thread ID, and arg is the argument passed in.
    //A thread function should always call thread_setup(id, arg) in the first line.
    ////////////////////////////////////
    thread_setup(id, arg);
    ///////////////////////////////////////////
    //////////////////////////////////////////////

    //All tasks should be implemented iteratively instead of recursively.
    for (RUNNING->i = 1; ; RUNNING->i++)
    {
        ////////////////////////////////////////////
        //The variables you declare in the stack will be overwritten after context switching.
        //If you wish to keep them, store them in the TCB.
        /////////////////////////////////////////
        if (RUNNING->i <= 2)
            RUNNING->x = RUNNING->y = 1;
        else
        {
            /* We don't need to save tmp, so it's safe to declare it here. */
            int tmp = RUNNING->y;
            RUNNING->y = RUNNING->x + RUNNING->y;
            RUNNING->x = tmp;
        }
        //////////////////////////
        //For the ith iteration, you should print out a line to the standard output:
        //[thread id] [FIB(i)]
        /////////////////////////////
        printf("%d %d\n", RUNNING->id, RUNNING->y);

        /////////////////////////////////////////////
        //Each iteration should take a bit more than 1 second.
        //To guarantee this, call sleep(1) after you¡¦ve done every iteration (step).
        //////////////////////////////////////////////
        sleep(1);

        if (RUNNING->i == RUNNING->arg)
        {
            thread_exit();
        }
        else
        {
            ////////////////////////////////
            //Fibonacci number and Factorial should call thread_yield after each iteration
            ///////////////////////////////////////////
            thread_yield();
        }
    }
}

void factorial(int id, int arg)
{
    // TODO
    /////////////////////////////////////////////////////////////
    thread_setup(id, arg);
    RUNNING->z = 1;
    //Note that you are asked to compute the answer in exact N iterations
    for (RUNNING->i2 = 1; ; RUNNING->i2++)
    {
        RUNNING->z *= RUNNING->i2;

        /////////////////////////////
        //For the ith iteration, you should print out a line to the standard output:
        //[thread id] [FACTORIAL(i)]
        //////////////////////////////////////////
        printf("%d %d\n", RUNNING->id, RUNNING->z);
        sleep(1);

        if (RUNNING->i2 == RUNNING->arg)
        {
            thread_exit();
        }
        else
        {
            thread_yield();
        }
    }
}


void bank_operation(int id, int arg)
{

    //////////////////////////////////////////////////////////////////////////////
    // TODO
    //The argument N indicate a operation of a user, abs(N) > 0
    //You need to perform three steps in order, and you need to call the thread_yield at the end of each step.

    thread_setup(id, arg);

    /////////////////////////////////////////////
    //(Step 1) Acquire lock and print the current balance
    //At this step, you call lock() to acquire the lock.
    //After the lock is acquired, print the following line to the standard output:
    //[thread id] acquired the lock
    lock();
    printf("%d acquired the lock\n", RUNNING->id);
    sleep(1);
    thread_yield();
    /////////////////////////////////////////////////

    //(Step 2) Check the balance and update the balance
    //You can only modify the balance at this step, after aquiring the lock.

    int tmp = bank.balance;
    if (RUNNING->arg > 0)
    {
        //If N is positive, deposit N.
        bank.balance += RUNNING->arg;

        printf("%d %d %d\n",RUNNING->id ,tmp, bank.balance);
    }
    else
    {
        /////////////////////////
        //If N is negative and balance >= abs(N) , withdraw abs(N)
        //
        //After that, print a line to the standard output:
        //[thread id] [balance] [updated balance]
        //////////////////////////////////////
        if(bank.balance >= abs(RUNNING->arg))
        {
            bank.balance -= abs(RUNNING->arg);
            printf("%d %d %d\n",RUNNING->id,tmp, bank.balance);
        }else
        {
            printf("%d %d %d\n",RUNNING->id,tmp, tmp);
        }
    }
    sleep(1);
    thread_yield();

    /////////////////////////////////////////////////////////

    //(Step 3) Release the lock
    //Call unlock() to release the lock.And print the following line to the standard output:
    //[thread id] released the lock
    unlock();
    printf("%d released the lock\n", RUNNING->id);
    sleep(1);
    thread_exit();
    ////////////////////////////////
}
