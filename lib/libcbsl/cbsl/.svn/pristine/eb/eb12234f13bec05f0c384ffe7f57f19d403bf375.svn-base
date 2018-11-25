#ifndef THREADPOOL_H
#define THREADPOOL_H


namespace cbsl {
    
class ThreadPoolWorker;
class ThreadPoolTask;

class ThreadPool {
public:
    typedef enum {
        Ok                  =  0 ,
        ErrorUnknown        =  1 ,
        ErrorBadParameter   =  2 ,
        ErrorAlreadyRunning =  3 ,
    } ResultCode;
    
private:
    struct impl;
    struct impl *pimpl;
    
    ThreadPool( ThreadPool const &other );
    ThreadPool& operator=( ThreadPool const &other );
    
    

public:
    ThreadPool();
    ~ThreadPool();
    
    int setNumWorkers( int const nworkers );
    
    int launchWorkers();
    int stopWorkers();
    
    /** \brief Add a task to the thread pool.
     * \details Tasks must be added to the thread pool before being enqueued.
     * Failure to do so will result in unexpected behaviour.
     */
    int addTask( ThreadPoolTask *task );
    /** \brief Mark \a task for execution */
    int enqueueTask( ThreadPoolTask *task );
    /** \brief signal handler for when task completes an action
     * \details This handler should be called by a worker when its task
     * completed execution of an action. When that happens, the thread pool
     * checks its queue and loads a new task into an idlde worker if needed.
     */
    void onTaskReturn( ThreadPoolWorker *worker , ThreadPoolTask *task );
    
    /** \brief Set callback for to handle completion  of tasks.
     * \details The application may set this callback to handle the completion
     * of tasks. By default this callback is not defined and nothing is done
     * on this event.
     * The callback will take parameter with the address of the task that
     * completed execution.
     */
    void setCbTaskComplete( void(*fnptr)(ThreadPoolTask *) );
}; /* class ThreadPool */

}; /* namespace Wcommon */



#endif /* !defined( THREAD_POOL_H ) */
