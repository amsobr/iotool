#if !defined(RUNNER_HPP)
#define RUNNER_HPP

#include "result.hpp"

namespace cbsl {

class Runnable;

class Runner
{
public:
    
    /** \brief Enqueue a runnable for execution
     * \details This call shall perform so that the Runnable's run() method
     * is executed later on. As a general rule, the runnable should be ran
     * as soon as possible.
     * Nevertheless, this interface doesn't provide any warranties regarding
     * the timing of the execution.
     * \warning Memory Management is up to the caller. The caller may choose to
     * enqueue stack or heap allocated objects freely. The release of the memory
     * used by the runnables is handled by their respective owners.
     */

    virtual Result enqueue( Runnable *r ) = 0;
};
    
}; /* namespace cbsl */



#endif
