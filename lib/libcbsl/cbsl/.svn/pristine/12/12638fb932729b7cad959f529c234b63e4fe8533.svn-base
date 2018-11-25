
#include <list>
#include <set>
#include <stdio.h>

#include "mutex.hpp"
#include "threadpoolworker.h"
#include "threadpool.h"
#include "threadpooltask.h"

namespace cbsl {
    
class ThreadPoolTask;

typedef std::list<ThreadPoolWorker*> WorkerQueue;
typedef std::set<ThreadPoolWorker*> WorkerSet;
typedef std::list<ThreadPoolTask*> TaskQueue;

struct ThreadPool::impl {
    unsigned int nworkers;
    bool        running;
    Mutex      *mutex;
    void (*onTaskComplete)(ThreadPoolTask*);
    WorkerQueue worker;
    WorkerQueue idleWorkers;
    WorkerSet   busyWorkers;
    TaskQueue   readyTasks;
    
    impl() :
    nworkers(0)         ,
    running(false)      ,
    mutex(0)            ,
    onTaskComplete(0)   ,
    worker()            ,
    idleWorkers()       ,
    busyWorkers()       ,
    readyTasks()        
    {
    }
    
    ~impl()
    {
    }
    
private:
    impl( impl const &);
    impl operator=(const impl&);
};


ThreadPool::ThreadPool() :
pimpl( new struct impl )
{
    try {
        pimpl->nworkers = 1;
        pimpl->running  = false;
        pimpl->mutex    = new Mutex;
        pimpl->onTaskComplete   = 0;
    }
    catch (...) {
        
    }
}


ThreadPool::~ThreadPool()
{
    delete pimpl->mutex;
    pimpl->mutex    = 0;
    for ( WorkerQueue::iterator it=pimpl->worker.begin(); it!=pimpl->worker.end() ; ++it ) {
        delete (*it);
    }
    pimpl->worker.clear();
    pimpl->idleWorkers.clear();
    delete pimpl;
    pimpl   = 0;
}



int ThreadPool::setNumWorkers( int const nworkers )
{
    if ( nworkers>0 ) {
        pimpl->nworkers = nworkers;
        return Ok;
    }
    return ErrorBadParameter;
}


/* this whole method must be protected by a mutex. */
/* Create an index of pointers to worker threads
 * Allocate new workers and populate the index
 * Fire up worker threads (Right after start they'll enter an idle state)
 * Try to assign as many queued tasks as possible to idle workers.
 */
int ThreadPool::launchWorkers()
{
    MutexLocker locker( pimpl->mutex );
    if ( pimpl->running ) {
        return ErrorAlreadyRunning;
    }
    for ( unsigned int i=0 ; i<pimpl->nworkers ; ++i ) {
        ThreadPoolWorker *p = new ThreadPoolWorker(this);
        pimpl->worker.push_back(p);
        p->start();
        pimpl->idleWorkers.push_back(p);
    }
    TaskQueue::iterator it;
    for ( it=pimpl->readyTasks.begin(); it!=pimpl->readyTasks.end() ; ++it ) {
        if ( pimpl->idleWorkers.empty() ) {
            return Ok;
        }
        else if ( (*it)->getWorker()==0 ) {
            ThreadPoolTask *task    = (*it);
            pimpl->readyTasks.erase(it);
            ThreadPoolWorker *w = pimpl->idleWorkers.front();
            pimpl->idleWorkers.pop_front();
            w->loadTask(task);
            pimpl->busyWorkers.insert(w);
        }
    }
    if ( (pimpl->busyWorkers.size()+pimpl->idleWorkers.size()) !=
            (unsigned)pimpl->nworkers ) {
        fprintf(stderr, "THREAD POOL on launch: nworkers=%u , idle=%zu, "
                "busy=%zu.\n", pimpl->nworkers, pimpl->idleWorkers.size(),
                pimpl->busyWorkers.size() );
        *(int*)0    = 0;
    }
    return Ok;
}


/* Add a task to the thread pool. */
int ThreadPool::addTask( ThreadPoolTask *task )
{
    cbsl::MutexLocker locker(pimpl->mutex);
    task->myPool    = this;
    task->setWorker(0);
    return Ok;
}


int ThreadPool::enqueueTask(ThreadPoolTask* task)
{
    pimpl->mutex->lock();
    if ( (!pimpl->idleWorkers.empty()) && (task->getWorker()==0) ) {
        ThreadPoolWorker *w     = pimpl->idleWorkers.front();
        pimpl->idleWorkers.pop_front();
        w->loadTask(task);
        pimpl->busyWorkers.insert(w);
        if ( (pimpl->busyWorkers.size()+pimpl->idleWorkers.size()) != pimpl->nworkers ) {
            fprintf(stderr, "THREAD POOL on launch: nworkers=%u, idle=%zu, "
                    "busy=%zu.\n", pimpl->nworkers, pimpl->idleWorkers.size(),
                    pimpl->busyWorkers.size() );
            *(int*)0    = 0;
        }
    }
    else {
        pimpl->readyTasks.push_back(task);
        if ( (pimpl->busyWorkers.size()+pimpl->idleWorkers.size()) != pimpl->nworkers ) {
            fprintf(stderr, "THREAD POOL on launch: nworkers=%u, idle=%zu,"
                    " busy=%zu.\n", pimpl->nworkers, pimpl->idleWorkers.size(),
                    pimpl->busyWorkers.size() );
            *(int*)0    = 0;
        }
    }
    pimpl->mutex->unlock();
    return Ok;
}

/** \brief Event handler: a task completed one action */
void ThreadPool::onTaskReturn(ThreadPoolWorker* worker, ThreadPoolTask *task)
{
    cbsl::MutexLocker locker( pimpl->mutex);
    WorkerSet::iterator bIt = pimpl->busyWorkers.find(worker);
    if ( bIt==pimpl->busyWorkers.end() ) {
        fprintf( stderr , "THREAD POOL [on task return] , worker %p not found in set.\n" , worker );
        *(int*)0    = 0;
    }
    task->setWorker(0);
    TaskQueue::iterator it;
    for ( it=pimpl->readyTasks.begin() ; it!=pimpl->readyTasks.end() ; ++it ) {
        if ( (*it)->getWorker()==0 ) {
            ThreadPoolTask *t   = *it;
            pimpl->readyTasks.erase(it);
            worker->loadTask( t );
            if ( pimpl->onTaskComplete ) {
                pimpl->onTaskComplete(task);
            }
            return;
        }
    }
    pimpl->busyWorkers.erase(bIt);
    pimpl->idleWorkers.push_back(worker);
    if ( (pimpl->idleWorkers.size()+pimpl->busyWorkers.size())!=pimpl->nworkers ) {
        fprintf(stderr, "THREAD POOL [ON task return] total=%u, idle=%zu "
                "busy=%zu\n", pimpl->nworkers, pimpl->idleWorkers.size(),
                pimpl->busyWorkers.size() );
        *(int*)0    = 0;
    }
    if ( pimpl->onTaskComplete ) {
        pimpl->onTaskComplete(task);
    }
}


int ThreadPool::stopWorkers()
{
    for ( WorkerQueue::iterator it=pimpl->worker.begin(); it!=pimpl->worker.end() ; ++it ) {
        (*it)->stop();
    }
    return Ok;
}


void ThreadPool::setCbTaskComplete( void(*fnptr)(ThreadPoolTask *) )
{
    pimpl->mutex->lock();
    pimpl->onTaskComplete   = fnptr;
    pimpl->mutex->unlock();
}



}; /* namespace Wcommon */

