/** \file	rwlock.h
 * \brief	This module implements Read-Write Locks
 * \details	Read Write Locks allow multiple access to shared data for reading
 *			while granting exclusive access for writing.
 *			When a consumer tries to aquire a read lock on a resource, if
 *			the resource is not locked for writing, the lock is granted and
 *			the consumer may proceed. If more than one consumer try to acquire
 *			the read lock without any active read lock, all consumers are
 *			granted the lock and proceed normally. This hapens so because
 *			read operations do not modify the data and, as so, multiple
 *			processes or threads can be given access to the same data without
 *			the risk of contention.
 *			In the simplest implementation, writers, will be held of for as
 *			long as there are read locks active. As new readers are not held
 *			off, the writer may hang forever waiting for a lock.
 *			The \a RWLock class behaves in a manner that read locks are not
 *			granted if there are threads waiting for a write lock, so that
 *			writers have access to the data to be written without starvation.
 * \author	António Oliveira - Withus, Lda ( antonio.oliveira@withus.pt )
 * \ingroup Synchronization
 */

#ifndef RWLOCK_H
#define RWLOCK_H

#include <pthread.h>

namespace Pti {
	
/** \class 	RWLock
 * \brief	This class provides a Read-Write Lock
 * \details	The Read-Write Lock allows multiple threads to access a shared
 *			resource for reading, while at the same time, granting
 *			exclusive access for writing. Readers can access the same
 *			resource simultaneously as long as there are no writers
 *			waiting for a write-lock.
 *			This class effectively reduces the cost of locks on resources
 *			which are mainly read or written.
 * \sa Mutex WaitCondition
 * \ingroup Synchronization
 */
class RWLock {
	private:
		pthread_rwlock_t myLock;
		pthread_rwlockattr_t myLockAttr;
		
	public:
		RWLock() :
		myLock()    ,
		myLockAttr()
        {
            pthread_rwlockattr_init( &myLockAttr );
            pthread_rwlockattr_setpshared( &myLockAttr , PTHREAD_PROCESS_PRIVATE );
            pthread_rwlock_init( &myLock , &myLockAttr );
        }
        
		virtual ~RWLock()
        {
            pthread_rwlock_destroy( &myLock );
            pthread_rwlockattr_destroy( &myLockAttr );
        }
		
		/** \brief Lock for reading.
		 * \details	Locks the lock for reading. If there are no writers
		 *			waiting for a write lock, the lock is granted, otherwise
		 *			the caller is blocked until there are no write locks
		 *			active.
		 * \return	Returns true on success, or false on failure.
		 * \sa rdunlock tryRdlock
		 */
		bool rdlock() {
            return (pthread_rwlock_rdlock(&myLock) == 0);
        }
		
		/** \brief	Attempt to lock for reading without blocking the caller
		 * \details	rdtrylock() tries to lock the lock for reading but
		 *			but instead of blocking the thread like \a rdlock would
		 *			do, returns and lets the caller decide what to do next.
		 * \return	If rdtrylock acquired a lock, returns true. The lock
		 *			must be unlocked with \a rdunlock() later. If the lock
		 *			was not acquired, returns false immediately.
		 * \sa rdlock rdunlock
		 */
		bool rdtrylock()
        {
            if ( pthread_rwlock_tryrdlock( &myLock ) == 0 ) {
                /* aquired lock for read */
                return true;
            }
            /* lock not acquired */
            return false;
        }

		
		/** \brief 	Unlock a read lock
		 * \details	Unlocks a read lock which was acquired previously by
		 * 			\a rdlock or \a tryRdlock. Calling rdunlock on an 
		 *			read-unlocked lock results undefined behaviour.
		 * \return	Returns true in case of success, otherwise returns false.
		 * \sa rdlock rdtrylock
		 */
		bool rdunlock()
        {
            return ( pthread_rwlock_unlock(&myLock) == 0 );
        }

		/** \brief Lock for writing
		 * \details	Locks the lock for writing. If there are no active locks
		 *			the lock is acquired and returns immediatelly. If there
		 *			are other locks active, the caller is blocked until it
		 *			can proceed.
		 * \return Returns true on success or false on failure.
		 * \sa wrtrylock wrunlock
		 */
		bool wrlock()
        {
            return (pthread_rwlock_wrlock(&myLock) == 0 );
        }

		
		/** \brief	Try to lock for writine without blocking the caller.
		 * \details	Try to lock for writing. Behaves like \a wrlock, except
		 *			that if the lock was not acquired, instead of blocking 
		 *			the caller, returns immediately and lets the caller
		 *			decide what to do next.
		 * \return	If a write lock is acquires this way, returns true. The
		 *			thread must call \a wrunlock at some time later.\n
		 *			If a lock was not acquired, returns false and the lock is
		 *			left untouched.
		 * \sa wrlock wrunlock
		 */
		bool wrtrylock()
        {
            if (pthread_rwlock_trywrlock(&myLock) == 0 ) {
                return true;
            }
            return false;
        }

		
		/** \brief 	Release a write-lock
		 * \details	Release a write lock. If the lock wasn't previously
		 *			acquired, the result is undefined.
		 * \warning	This must be called from the same thread that acquired
		 *			the lock, or the result will be undefined.
		 * \return	Returns true on success, otherwise returns false
		 * \sa wrlock wrtrylock
		 */
		bool wrunlock()
        {
            return (pthread_rwlock_unlock(&myLock) == 0);
        }
}; /* class RWLock */
	
}; /* namespace Pti */

#endif /* RWLOCK_H */
