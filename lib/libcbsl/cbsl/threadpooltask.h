#ifndef THREADPOOLTASK_H
#define THREADPOOLTASK_H

namespace cbsl {

class ThreadPool;
class ThreadPoolWorker;
class ThreadPool;

class ThreadPoolTask {
private:    
    ThreadPoolTask( ThreadPoolTask const & );
    ThreadPoolTask& operator=(ThreadPoolTask const &);
    
protected:
    ThreadPoolWorker *myWorker;
    ThreadPool       *myPool;
    
    friend class ThreadPool;
    
public:
    ThreadPoolTask() :
    myWorker(0)   ,
    myPool(0)
    {
    }
    
    virtual ~ThreadPoolTask()
    {
    }
    
    /** \brief execute the actions for the task.
     * \details This virtual method should be reimplemented on derived
     * classes to execute the operations of the task itself.
     * The task executes for as long as it needs processing time. The worker
     * is freef only when \a ececute() returns.
     */
    virtual void execute() = 0;
    void setWorker( ThreadPoolWorker *ptr=0 ) { myWorker = ptr; }
    inline ThreadPoolWorker *getWorker() const { return myWorker; }
    
    /** \brief The task needs to perform work.
     * \details this method instructs the threadpool to assign a worker to
     * the task as soon as possible so that it can perform work.
     * \note If this method isn't called when the task needs processing time,
     * the task will become stall.
     */
    void requestWork();
}; /* class ThreadPoolTask */
    
}; /* namespace Wcommon */

#endif /* !defined( THREADPOOLTASK_H ) */
