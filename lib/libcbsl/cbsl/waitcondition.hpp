#ifndef WAITCONDITION_H
#define WAITCONDITION_H
/** \file	waitcondition.h
 * \brief	This module implements thread synchronization conditions.
 * \author	António Oliveira - Withus, Lda (antonio.oliveira@withus.pt)
 */

#include <pthread.h>
#include <time.h>
#include <assert.h>

namespace cbsl {
	class Mutex;
	
/** \class 	WaitCondition
 * \brief 	This class implements a wait condition.
 * \details	Wait conditions are a thread synchronization that enable a thread
 *			to wait until a given condition is met. Without wait conditions,
 *			the programmer would, most likely, have to use busy waiting or
 *			other complex forms of thread synchronization.
 *			If one thread needs to wait for some condition to be met, it
 *			calls wait(), and is put to sleep. Some other thread reaches the
 *			desired condition and calls signal() or signalAll(), which leads
 *			to one or all the wait()int threads to be woke up and continue.
 *			The wait() call needs a mutex and must be performed atomically
 *			to ensure that a spurious signal() is not called before the thread
 *			being suspended (such cases might lead to a thread permanently
 *			suspended).
 */
class WaitCondition {
	private:
		pthread_cond_t	myCond;
		
	public:
		/** \brief	Default Constructor
		 * \details	Creates a WaitCondition object
		 */
		WaitCondition() :
		myCond()
		{
            pthread_condattr_t attrs;
            pthread_condattr_init(&attrs);
            int res = pthread_condattr_setclock(&attrs,CLOCK_MONOTONIC);
            assert(res==0);

			res     = pthread_cond_init( &myCond , &attrs );
            assert(res==0);
            
            pthread_condattr_destroy(&attrs);
		}
		
		/** \brief Destructor */
		virtual ~WaitCondition()
		{
			pthread_cond_destroy( &myCond );

		}
		
		/** \brief 	Wait on condition.
		 * \details	Wait until the condition is met. The calling thread is
		 *			suspended until another thread calls signalOne() or
		 *			signalAll().
		 *			The mutex \em must be locked when calling wait().
		 *			When the thread is suspended, the mutex is automatically
		 *			unlocked. This operation is guaranteed to be atomic.
		 *			Calling wait() with an unlocked mutex is a logical error
		 *			and may lead to the condition receiving a signal before
		 *			the wait(), which may leave the thread asleep forever.
		 *			When wait() returns, the mutex is locked again for use
		 *			by the calling thread.
		 * \param pMutex	Pointer to a locked mutex. The mutex must be
		 *					locked when calling wait(). If *pMutex is
		 *					unlocked, wait() returns immediately and the
		 *					thread is not suspended at all.
		 * \return			If the call was successful, unlocks pMutex and 
		 *					returns true. If pMutex was unlocked, it remains
		 *					unlocked, the thread does not block, and returns
		 *					false.
		 * \note	Calling wait() on an unlocked mutex is a serious error
		 *			and could lead the thread to hang forever if the
		 *			signalOne() or signalAll() were called before wait().
		 *			To prevent this behaviour, pMutex must be locked when
		 *			calling wait() (as wall as signalOne() / signalAll() )
		 *			to ensure proper operation.
		 */
		
		bool wait( Mutex *pMutex );
        
        /** \brief  Wait on condition, for atmost some specified time
         * \details Wait until the condition is met. The calling thread is
         *          suspended until another thread calls signalOne() or
         *          signalAll(), or the specified time is exceeded.
         *          The mutex \em must be locked when calling wait().
         *          When the thread is suspended, the mutex is automatically
         *          unlocked. This operation is guaranteed to be atomic.
         *          Calling wait() with an unlocked mutex is a logical error
         *          and may lead to the condition receiving a signal before
         *          the wait(), which may leave the thread asleep forever.
         *          When wait() returns, the mutex is locked again for use
         *          by the calling thread.
         * \param pMutex    Pointer to a locked mutex. The mutex must be
         *                  locked when calling wait(). If *pMutex is
         *                  unlocked, wait() returns immediately and the
         *                  thread is not suspended at all.
         * \param msTimeout Maximum wait time, in milliseconds. If this time
         *                  is exceeded during wait, the call returns with
         *                  false.
         * \return          True when timedWait() returned due to another thread
         *                  calling signalOne() or signalAll(). When a timeout
         *                  occurs, returns false.
         * \note    Calling wait() on an unlocked mutex is a serious error
         *          and could lead the thread to hang forever if the
         *          signalOne() or signalAll() were called before wait().
         *          To prevent this behaviour, pMutex must be locked when
         *          calling wait() (as wall as signalOne() / signalAll() )
         *          to ensure proper operation.
         */        
        bool timedWait( Mutex *pMutex , unsigned int msTimeout );
                
        
		
		/** \brief	Wake one blocked thread waiting on the condition
		* \details	Waks one thread that is blocked, waiting for the
		*			condition to be met. If more than one thread are waiting
		*			on the condition, it is up to the OS scheduler to choose
		*			one one to wake up. The class cannot determine which
		*			thread will be woken up.
		* \return	If the signal operation is successful, returns true,
		*			otherwise returns false. Error codes or exceptions are
		*			not implemented yet.
		* \warning	Calling signalOne() before wait() is a logical error and
		*			may lead to the waiting thread remaining blocked forever.
		*/
		bool signalOne();
		
		/** \brief 	Wake all blocked threads waiting on the condition
		 * \details	Wakes all threads that are blocked waining on the
		 *			condition. If more than one thread are waiting on the
		 *			condition, the OS scheculed determines the order by which
		 *			they are woken up. The class doesn't know by which order
		 *			threads will be woken up.
		 * \return	signalAll() has the same return value as signalOne().
		 * \warning	Calling signalAll() before wait() is a logical error and
		 *			may lead to the waiting thread to remain blocked forever.
		 */
		bool signalAll();
}; /* class WaitCondition */

}; /* namespace Wcommon */


#endif /* WAITCONDITION_H */
