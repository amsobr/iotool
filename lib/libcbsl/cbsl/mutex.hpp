/** \file	mutex.hpp
 * \brief	Class for handling of mutexes.
 * \details	This module implements the handling of mutexes using a simple
 *			C++ interface.
 *			Two classes are provided: Mutex and MutexLocker.
 *			Mutex is a simple mutex class whereas ScopeMutex is a
 *			convenience class that allows a subroutine to hold a mutex
 *			locked while execution is within its scope, releasing the
 *			mutex as soon as its scope ends.
 */

#ifndef WCOMMON_MUTEX_HPP
#define WCOMMON_MUTEX_HPP

#include <pthread.h>
#include <errno.h>
namespace cbsl {
	
/** \class 	Mutex
 * \brief	Mutual exclusion mutex.
 * \details	This is a simple mutex class.
 */
class Mutex {
	private:
		pthread_mutex_t	m_mymutex;
		pthread_mutexattr_t m_myattr;
		
	public:
        Mutex( bool recursive=false ) :
        m_mymutex() ,
        m_myattr() 
        {
            pthread_mutexattr_init( &m_myattr );
            int mutexType   = recursive ? PTHREAD_MUTEX_RECURSIVE : PTHREAD_MUTEX_NORMAL;
            pthread_mutexattr_settype( &m_myattr , mutexType );
            pthread_mutex_init( &m_mymutex , &m_myattr );
        }

		/** \brief	Mutex object destructor.
		 * \details	Destroys the Mutex object. Note that the mutex must
		 *			unlocked prior to its deletion, otherwise the
		 *			behavior is not defined.
		 */
		~Mutex() {
			pthread_mutex_destroy( &m_mymutex );
			pthread_mutexattr_destroy( &m_myattr );
		}
		
		/** \brief	Lock the mutex.
		 * \details	Locks the mutex. If the mutex is currently unlocked,
		 *			the calling thread takes ownership of the mutex and
		 *			locks it.
		 *			Please note that mutexes should be kent in locked
		 *			state for as little time as possible.
		 *			If the mutex was already locked, the calling thread
		 *			will block until the mutex is unlocked by its current
		 *			owner and the blocked thread is eventually woken up
		 *			by the OS scheduler to resume operation.
		 * \return	When lock returns, the mutex is guaranteed locked and
		 *			the calling thread took ownership of the mutex.
		 * \warning The current implementation of Pti::Mutex is not
		 *			recursive, so it is absolutely prohibited to call
		 *			lock() more than once on the same mutex consecutively.
		 */
		void lock()
		{
			pthread_mutex_lock(&m_mymutex);
		}

		/** \brief	Unlock the mutex.
		 * \details	Unlock the mutex, leaving it free to be acquired by
		 *			other concurrent threads.
		 *			If the mutex is already unlocked, this has no effect.
		 */
		void unlock()
		{
			pthread_mutex_unlock(&m_mymutex);
		}
		
		/** \brief	Try to lock the mutex without blocking
		 * \details	This method behaves like lock() but does not block
		 *			the calling thread if the mutex was already locked.
		 * \return	If the lock was successful, returns true. If the
		 *			mutex was already locked, returns false and the mutex
		 *			remains unchanged.
		 */
		bool tryLock()
		{
			return ( EBUSY!=pthread_mutex_trylock(&m_mymutex) );
		}
		
		friend class WaitCondition;
}; // end class Mutex


/** \class 	Pti::MutexLocker
 * \brief	Scoped mutex - automatically unlocking on scope exit.
 * \details	This class implements a container that automatically holds a
 *			locked mutex for its lifetime, unless the caller explicitly
 *			requests a mutex unlock.
 *			ScopeMutex is usefull on routines with several exit points,
 *			where there is a high risk that the owner forgets to unlock
 *			a mutex on exit. At the same time, for smaller routines, this
 *			class helps making code more compact, by eliminating the need
 *			to call lock() and unlock() explicitly.
 */
class MutexLocker {
	private:
		/** \brief reference to the scoped mutex. */
		Mutex	*m_pMutex;
		/** Constructor made private to enforce instances with a mutex
		 * given as parameter
		 */
		MutexLocker()
        : m_pMutex(0)
        {            
        };
        
        MutexLocker( MutexLocker const &other );
        MutexLocker& operator=( MutexLocker const &other );
		
	public:
		MutexLocker( cbsl::Mutex *mut ) :
        m_pMutex(mut)
		{
            m_pMutex->lock();
		};
		
		~MutexLocker()
		{
            m_pMutex->unlock();
		};
		
		/** \brief Manually lock the mutex.
		 * \details Behaves simillarly to Pti::Mutex::lock()
		 */
		void lock() { m_pMutex->lock(); };
		
		/** \brief Manually unlock the mutex.
		 * \details Behaves simillarly to Pti:Mutex::unlock();
		 */
		void unlock() { m_pMutex->unlock(); };
		
		/** \brief Try to lock the mutex
		 * \details Behaves simillarly to Pti::Mutex::tryLock()
		 */
		bool tryLock()
		{
			return m_pMutex->tryLock();
		}
};

}; // end namespace Wcommon

#endif // WCOMMON_MUTEX_HPP

