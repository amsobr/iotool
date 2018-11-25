#ifndef THREAD_SYNC_H
#define THREAD_SYNC_H

/* THREADSYNC_SEM IS NOT VALIDATED YET. USE THE CLASSIC IMPLEMENTATION */
#define THREADSYNC_SEM

#ifdef THREADSYNC_SEM
#include <semaphore.h>
#else
#include <pti/waitcondition.h>
#include <pti/mutex.h>
#endif

class ThreadSync {
	private:
#ifdef THREADSYNC_SEM        
        sem_t mySem;
#else
        Pti::Mutex          myMutex;
        Pti::WaitCondition  myCond;
        bool            myIsWaiting;
        bool            myIsDone;
#endif
		
	public:
#ifdef THREADSYNC_SEM    
        ThreadSync() :
        mySem()
        {
            sem_init( &mySem , 0 , 0 );
        }
        ~ThreadSync()
        {
            sem_destroy( &mySem );
        }
#else
        ThreadSync(): myIsWaiting(false) , myIsDone(false) {};
        ~ThreadSync(){};
#endif
		
		void waitForOther() {
#ifdef THREADSYNC_SEM
            int ret;
            do {
                int r=sem_wait(&mySem);
                if ( r!=0 ) {
                    ret = errno;
                }
                else {
                    ret = 0;
                }
            } while ( ret==EINTR );
#else
			myMutex.lock();
			if ( myIsDone ) {
				myMutex.unlock();
				return;
			}
			else {
				myIsWaiting	= true;
				myCond.wait( &myMutex );
				myMutex.unlock();
				return;
			}
#endif
		}
		
		void wakeOther() {
#ifdef THREADSYNC_SEM
            sem_post( &mySem );
#else
			myMutex.lock();
			if ( myIsWaiting ) {
				myCond.signalOne();
				myMutex.unlock();
				return;
			}
			else {
				myIsDone	= true;
				myMutex.unlock();
				return;
			}
#endif
		}
}; /* class ThreadSync */

/** \class ThreadSync
\brief	This class allows one thread to wait for some event on another thread.
\details	This class implements a mechanism that enables one thread to
			wait for some event to happen on another thread.
			When thread A wants to wait for event X to happen in thread B
			before continuing execution, both threads must share a \a
			ThreadSync object. Thread A calls \a waitForOther and if X wasn't
			reached, thread A will be put to sleep. When thread B reaches X
			if calls \a wakeOther. Thread A now resumes its operation. If
			thread B reached X before A being put to sleep, when thread A
			tries to call \a waitForOther, it will simply continue running
			as the "event reached" condition was detected.\n
*/
#endif /* ThreadSync */
