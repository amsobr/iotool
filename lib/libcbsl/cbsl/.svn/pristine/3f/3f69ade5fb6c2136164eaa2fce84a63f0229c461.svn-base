#ifndef THREADPOOLWORKER_H
#define THREADPOOLWORKER_H

#include "thread.hpp"

namespace cbsl {
    
class ThreadPool;
class ThreadPoolTask;

class ThreadPoolWorker : public Thread {
public:
    typedef enum {
        Ok              = 0 ,
        ErrorUnknown    = 1 ,
        ErrorBusy       = 2 ,
    } ResultCode;
private:
    
    ThreadPoolWorker( ThreadPoolWorker const &other );
    ThreadPoolWorker& operator=( ThreadPoolWorker const &other );
    
    struct impl;
    impl *pimpl;
    
    virtual void run();
    
    friend class ThreadPool;
    
public:
    ThreadPoolWorker( ThreadPool *pool );
    virtual ~ThreadPoolWorker();
    
    int loadTask( ThreadPoolTask *task );
    
}; /* class ThreadPoolWorker */


}; /* namespace Wcommon */

#endif /* !defined( THREADPOOLWORKER_H ) */
