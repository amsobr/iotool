#ifndef ONE_SHOT_H
#define ONE_SHOT_H

#include <semaphore.h>
#include <time.h>

#include <list>

#include "nocopy.hpp"
#include "thread.hpp"
#include "timespec.h"
#include "logger.h"

namespace cbsl {
      
struct NewEntry {
    void (*fn)(void*,void*);
    void *p1;
    void *p2;
    TimeSpec tFire;
    
    NewEntry( void(*fn__)(void*,void*) , void *p1__ , void *p2__ , unsigned int d__  ) :
    fn(fn__)        ,
    p1(p1__)        ,
    p2(p2__)        ,
    tFire()
    {
        struct timespec ts;
        clock_gettime( CLOCK_MONOTONIC , &ts );
        tFire       = ts;
        ts.tv_sec   = d__/1000;
        ts.tv_nsec  = ((d__%1000)/10)*10*1000000; /* use 10ms resolution */
        tFire      += TimeSpec(ts);
    }
    
    NewEntry( NewEntry const &o ) :
    fn( o.fn )      ,
    p1( o.p1 )      ,
    p2( o.p2 )      ,
    tFire( o.tFire )
    {
    }
    
    NewEntry const &operator=( NewEntry const &o )
    {
        fn      = o.fn;
        p1      = o.p1;
        p2      = o.p2;
        tFire   = o.tFire;
        return *this;
    }
    
    ~NewEntry()
    {
    }
}; /* class NewEntry */


    
class Mutex;
    
/** \class OneShot
 * \brief Schedule function calls for a specified time
 * \details This class provides a mechanism for users to schedule routines to
 * be called sometime later without any special needs to perform housekeeping
 * with timers and sorts.
 * 
 * An application may instantiate any number of instances of OneShot, however
 * one single global instance internally scales to the needs of an entire
 * application and handles all timed funcion calls requested.
 * 
 * \par Usage
 * To schedule a function call to some later time, the client uses \a add() to
 * define the function, the value passed as a parameter and the delay.
 * 
 * The delay to the function call is guaranteed to be always no less than the
 * given delay. The call may, however, take more than the specified delay to
 * execute, depending on the precision of the system's timers.
 * 
 * Single Shot attempts to reduce coupling of the timings of different
 * functions to a minimum. There are no guarantees regarding which thread will
 * be used to call the scheduled function.
 */
class OneShot : private Thread {
    
private:
       
    WC_DISABLE_COPY(OneShot);
    
    bool        myStop;
    bool        myTmrEvt;
    Mutex      *myMutex;
    sem_t       mySem;    
    std::list<NewEntry> myNewEvts;
    logctl_t   *logger;
    
    virtual void run();    
    
public:
    OneShot(logctl_t *l__);
    ~OneShot();
    
    /** Schedule a function call for some time later.
     * \details Schedules a function call with given parameters to happen
     * the specified time after this method call.
     * \param fnptr Pointer to a function returning void and taking a void* as
     *  input parameter. This is the function that will be called when the 
     *  delay expires.
     * \param key0 Value to be passed as first parameter to the function call
     * \param key1 Value to be passed as second parameter to the function call
     * \param msdelay Delay, in ms, until the function call.
     * \return The return value is 0 on the initial version. Future versions
     * may return a random value that serves as a handle to further control
     * of the scheduled events.
     */
    long int add( void(*fnptr)(void*,void*) , void *key0 , void *key1 , unsigned int msdelay );
    
    static void cb_onTmrEvt( void *p__ );
    
}; /* class OneShot */
    
    
}; /* namespace Wcommon */

#endif /* !defined(ONE_SHOT_H) */
