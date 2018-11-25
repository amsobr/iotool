#if !defined( RUNNABLE_HPP )
#define RUNNABLE_HPP

#include "nocopy.hpp"

namespace cbsl {
    
class Runnable 
{
protected:
    virtual ~Runnable()
    {
    }

public:    
    virtual void run() = 0;
};

}; /* namespace cbsl */


#endif /* !defined(RUNNABLE_HPP) */