
#include <vector>

#include <stdio.h>

#include "waitcondition.hpp"
#include "mutex.hpp"

#include "runnable.hpp"

#include "runner_pool.hpp"
#include "thread.hpp"

namespace cbsl {

class RunnerThread;
    
class RunnerThreadListener
{
public:
    /** \brief The worker \a runner completed execution of \a runnable and is
     * ready to accept new runnables.
     */
    virtual void runnerReturned( RunnerThread *runner , Runnable *runnable )  = 0;
    
    virtual void runnerStopped( RunnerThread *runner )  = 0;
    
}; /* class RunnerThreadListener */


class RunnerThread : public Thread {
private:
    RunnerThreadListener *myListener;
    WaitCondition   myCond;
    Mutex           myMutex;
    Runnable       *myRunnable;
    bool            myStop;
    bool            myWaiting;
public:
    
    /* Runner Thread starts waiting.
     */
    RunnerThread( RunnerThreadListener *listener )
    : Thread()      ,
    myListener(listener) ,
    myCond()        ,
    myMutex()       ,
    myRunnable(0)   ,
    myStop(false)   ,
    myWaiting(true)
    {

    }
    
    ~RunnerThread()
    {

    }

    /* Consumer:
     * Run myRunnable if it is defined. Set myRunnable to 0 when done with it.
     * Producer:
     * Set myRunnable strictly IF its value is 0.
     * We ensure that each set runnable gets executed.
     */
    virtual void run()
    {
        while ( !myStop ) {
            Runnable *r  = 0;
            myMutex.lock();
            if ( myRunnable==0 ) {
                myWaiting   = true;
                myCond.wait(&myMutex);
                myWaiting   = false;
            }
            /* NB: this is NOT an else! We need to re-test myRunnable after
             * waking up from a wait().
             */
            r       = myRunnable;
            myMutex.unlock();
            if ( r!=0 ) {
                r->run();
                
                /* we know for sure myRunnable was != 0, so it couldn't have
                 * been touched anywhere else. We can now set it to 0 and 
                 * make this RunnerThread available again.
                 */
                myRunnable  = 0;
                /* callback only when ready for incoming runnables
                 * Parent can insert a new runnable right from the callback if
                 * it wants to.
                 */
                myListener->runnerReturned(this,r);
            }
        }
        myListener->runnerStopped(this);
    }
    
    void stop()
    {
        myMutex.lock();
        myStop  = true;
        if ( myWaiting ) {
            myCond.signalAll();
        }
        myMutex.unlock();
    }
    
    bool assign( Runnable *r )
    {
        cbsl::MutexLocker locker(&myMutex);
        if ( myRunnable==0 ) {
            myRunnable  = r;
            if ( myWaiting ) {
                myCond.signalAll();
            }
            return true;
        }
        else {
            fprintf( stderr , "ASSIGNING RUNNABLE TO NON-IDLE WORKER!!!!!\n" );
            return false;
        }
    }
    

    
}; /* class RunnerThread */
    
    
struct RunnerPool::Impl : public RunnerThreadListener {
private:
    cbsl::Mutex myMutex;
    std::vector<Runnable*> myQueuedRunnables;
    std::vector<RunnerThread*> myRunnerThreads;
    std::vector<RunnerThread*> myReadyRunners;
    std::vector<Runnable*> myActiveRunnables;
    size_t myNumWorkers;
    
    
    
public:
    
    Impl( unsigned int numWorkers ) :
    myNumWorkers( numWorkers )
    {
        cbsl::MutexLocker locker(&myMutex);
        for ( unsigned int i=0 ; i<numWorkers ; ++i ) {
            RunnerThread *runner    = new RunnerThread(this);
            myRunnerThreads.push_back(runner);
        }
    }
    
    virtual ~Impl()
    {
    }

    Result start()
    {
        for ( size_t i=0 ; i<myNumWorkers ; i++ ) {
            RunnerThread *runner    = myRunnerThreads[i];
            runner->start();
            myReadyRunners.push_back(runner);
        }
        return cbsl::OK;
    }
            
    Result shutdown()
    {
        for ( size_t i=0 ; i<myNumWorkers ; i++ ) {
            RunnerThread *runner    = myRunnerThreads[i];
            runner->stop();
        }
        
        for ( size_t i=0 ; i<myNumWorkers ; i++ ) {
            RunnerThread *runner    = myRunnerThreads[i];
            runner->join();
        }
        return cbsl::OK;

    }

    Result enqueue( Runnable *runnable )
    {
        RunnerThread *runner    = 0;
        int numRdyRunners;
        int numQueuedTasks;
        myMutex.lock();
        /* If there is one available worker, skip the queue and enqueue the
         * runnable right away. Otherwise, place it on the queue for later.
         */
        if ( !myReadyRunners.empty() ) {
            runner  = myReadyRunners.back();
            myReadyRunners.pop_back();
            myActiveRunnables.push_back(runnable);
        }
        else {
            myQueuedRunnables.push_back(runnable);
        }
        numRdyRunners   = myReadyRunners.size();
        numQueuedTasks  = myQueuedRunnables.size();
        myMutex.unlock();

        if ( runner!=0 ) {
            runner->assign(runnable);
        }
        //log_always( logctx.mgmt , "Enqueued Task. queued_tasks=%d , ready_runners=%d\n" , numQueuedTasks , numRdyRunners );
        return cbsl::OK;
    }
    
    
    void runnerReturned( RunnerThread *runner , Runnable *runnable )
    {
        Runnable *r = 0;
        myMutex.lock();
        /* delete all occurences of runnable, although only one should exist.
         * since active runnables is a small vector, this doesn't consume much
         * cpu.
         */
        std::vector<Runnable *>::iterator it = myActiveRunnables.begin();
        while( it!=myActiveRunnables.end() )  {
            if ( *it == runnable ) {
                it  = myActiveRunnables.erase(it);
            } 
            else {
                it++;
            }
        }
        
        if ( myQueuedRunnables.empty() ) {
            myReadyRunners.push_back(runner);
        }
        else {
            r   = *(myQueuedRunnables.begin());
            myQueuedRunnables.erase(myQueuedRunnables.begin());
            myActiveRunnables.push_back(runnable);
        }
        myMutex.unlock();
        if ( r!=0 ) {
            runner->assign(r);
        }
        return;
    }
    
    void runnerStopped( RunnerThread * /*runner*/)
    {
        /* nothing to do here for now.... */
    }
    
    Result cancelRunnable( Runnable *runnable )
    {
        cbsl::MutexLocker locker(&myMutex);
        std::vector<Runnable *>::iterator it;
        for ( it=myActiveRunnables.begin() ; it!=myActiveRunnables.end() ; it++ ) {
            if ( *it == runnable ) {
                return ERR_BUSY;
            }
        }
        
        bool found  = false;
        for ( it=myQueuedRunnables.begin() ; it!=myQueuedRunnables.end() ; it++ ) {
            if ( *it == runnable ) {
                it      = myQueuedRunnables.erase(it);
                found   = true;
                continue;
            }
        }
        if ( found ) {
            return OK;
        }
        else {
            return ERR_NOT_FOUND;
        }
    } /* cancelRunnable() */

}; /* struct RunnerPool::Impl */
    

RunnerPool::RunnerPool( size_t numWorkers )
: pimpl( new Impl(numWorkers) )
{
    
}

RunnerPool::~RunnerPool()
{
    delete pimpl;
}


Result RunnerPool::start()
{
    return pimpl->start();
}


Result RunnerPool::stop()
{
    return pimpl->shutdown();
}


Result RunnerPool::enqueue( Runnable *runnable )
{
    return pimpl->enqueue(runnable);
}

Result RunnerPool::cancelRunnable(Runnable* runnable)
{
    return pimpl->cancelRunnable(runnable);
}



}; /* namespace cbsl */
