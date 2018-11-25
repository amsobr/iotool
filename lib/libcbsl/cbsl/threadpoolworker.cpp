#include <stdio.h>
#include <semaphore.h>
#include <errno.h>

#include "logger.h"
#include "thread.hpp"
#include "threadpool.h"
#include "threadpooltask.h"
#include "threadpoolworker.h"

namespace cbsl {


struct ThreadPoolWorker::impl {
    ThreadPool     *myPool;
    ThreadPoolTask *task;
    bool            stop;
    sem_t           sem;
};

ThreadPoolWorker::ThreadPoolWorker( ThreadPool *pool ) :
Thread()   ,
pimpl(new impl)
{
    pimpl->myPool       = pool;
    pimpl->task         = 0;
    pimpl->stop         = false;
    sem_init( &pimpl->sem , 0 , 0 );
    //setStackSize(16384);
}

ThreadPoolWorker::~ThreadPoolWorker()
{
    sem_destroy( &pimpl->sem );
    delete pimpl;
}


void ThreadPoolWorker::run()
{
    pid_t tid   = getTid();
    fprintf( stderr , "THREAD POOL WORKER STARTED (tid=%d)\n", tid );
    int ntasks  = 0;
    while(1) {
        int code;
        do {
            int r   = sem_wait( &pimpl->sem );
            if ( 0!=r ) {
                code    = errno;
            }
            else {
                code    = 0;
            }
        } while( EINTR==code );
        
        if ( pimpl->stop ) {
            return;
        }
        if ( 0 == pimpl->task ) {
            continue;
        }
        
        ++ntasks;
        ThreadPoolTask *task(pimpl->task);
        pimpl->task->execute();
        pimpl->task = 0;
        pimpl->myPool->onTaskReturn(this,task);
    }
    fprintf( stderr , "Worker stopped. Executed a total of %d tasks.\n", ntasks );
}


int ThreadPoolWorker::loadTask(ThreadPoolTask* task)
{
    if ( 0 != pimpl->task ) {
        fprintf( stderr , "Cannot load a task on a busy worker.\n" );
        return ErrorBusy;
    }
    pimpl->task = task;
    task->setWorker(this);
    sem_post( &pimpl->sem );
    return Ok;
}

}; /* namespace cbsl */
