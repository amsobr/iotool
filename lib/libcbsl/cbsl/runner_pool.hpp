#if !defined( RUNNER_POOL_HPP )
#define RUNNER_POOL_HPP

#include <stddef.h>
#include <stdlib.h>

#include "runner.hpp"
#include "nocopy.hpp"
#include "result.hpp"
#include "runnable.hpp"

namespace cbsl {
    
class RunnerPool : public Runner {
    
public:
    
private:
    CBSL_NO_COPY(RunnerPool);
    
    struct Impl;
    struct Impl *pimpl;
    
public:
    RunnerPool( size_t numThreads );
    ~RunnerPool();
    
    Result start();
    Result stop();
        
    Result enqueue( Runnable *runnable );
    
    /** Cancel an enqueued Runnable.
     * Cancels the runnable so that is does not get executed. If the runnable
     * is not found or is already executing, this action takes no effect.
     * \return
     *  OK - The runnable was found and cancelled
     *  ERR_NOT_FOUND - The runnable wasn't found
     *  ERR_BUSY - The runner was already running.
     */
    Result cancelRunnable( Runnable *runnable );
    
}; /* class RunnerPool */

}; /* namespace cbsl */



#endif /* !defined( RUNNER_POOL_H ) */
