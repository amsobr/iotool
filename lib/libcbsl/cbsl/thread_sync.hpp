/* 
 * File:   thread_sync.hpp
 * Author: to
 *
 * Created on 04 April 2016, 01:33
 */

#ifndef THREAD_SYNC_HPP
#define THREAD_SYNC_HPP

#include <semaphore.h>
#include <errno.h>
#include "mutex.hpp"
#include "waitcondition.hpp"

namespace cbsl {
    
class ThreadSync 
{
    
private:
    bool mySignaled;
    Mutex myMutex;
    WaitCondition myCond;
    //sem_t mySem;
		
public:
    ThreadSync() :
    mySignaled(false) ,
    myMutex(false) ,
    myCond()
    //mySem()
    {
        //sem_init( &mySem , 0 , 0 );
    }

    ~ThreadSync()
    {
    //    sem_destroy( &mySem );
    }
		
    void waitForOther() {
        myMutex.lock();
        if ( !mySignaled ) {
            myCond.wait(&myMutex);
        }
        myMutex.unlock();
//        int ret;
//        do {
//            int r=sem_wait(&mySem);
//            if ( r!=0 ) {
//                ret = errno;
//            }
//            else {
//                ret = 0;
//            }
//        } while ( ret==EINTR );
    }
		
    void wakeOther() {
        myMutex.lock();
        mySignaled  = true;
        myCond.signalOne();
        myMutex.unlock();       
//        sem_post( &mySem );
    }
}; /* class ThreadSync */

}; /* namespace cbsl */


#endif /* THREAD_SYNC_HPP */

