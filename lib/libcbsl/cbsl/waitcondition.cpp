
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include "mutex.hpp"
#include "waitcondition.hpp"

namespace cbsl {

bool WaitCondition::wait( Mutex *pMutex )
{
    /* Cannot execute if the mutex was unlocked because it may not guarantee
     * the atomic blocking behaviour.
     */
//  if( pMutex->tryLock() ) {
//      pMutex->unlock();
//      fprintf( stderr , "Cannot call wait() on an unlocked mutex!\n" );
//      return false;
//  }
    /* mutex will be atomically unlocked */
    int ret = pthread_cond_wait( &myCond , &pMutex->m_mymutex );
    /* mutex is locked again */
    return ( 0==ret );
}
        
bool WaitCondition::timedWait( Mutex *pMutex , unsigned int msTimeout )
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC,&ts);    
    ts.tv_sec          += msTimeout/1000;
    unsigned int nsec   = (msTimeout%1000)*1000000;
    ts.tv_nsec         += nsec;
    while( ts.tv_nsec>=1000000000 ) {
        ts.tv_sec++;
        ts.tv_nsec -=1000000000;
    }
    /* mutex will be atomically unlocked */
    int ret = pthread_cond_timedwait( &myCond , &pMutex->m_mymutex , &ts );
    /* mutex is locked again */
    return ( 0==ret );
}
        

        bool WaitCondition::signalOne()
{
	int ret	= pthread_cond_signal( &myCond );
	return (0==ret);
}

bool WaitCondition::signalAll()
{
	int ret	= pthread_cond_broadcast( &myCond );
	return (0==ret);
}

}; /* namespace Wcommon */
