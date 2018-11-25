/** \file	Thread.hpp
 * \brief	Module to manage threads in PTi software
 * \details	This module implements several routines and data structures
 *			to enable thread management and synchronization in PTi
 *			software projects.
 *			If does not intend to be cross-platform at all and only
 *			unix pthreads are supported so far.
 * \author	António Oliveira - Withus Lda (antonio.oliveira@withus.pt)
 */

#ifndef WC_THREAD_H
#define WC_THREAD_H

#include <sys/types.h>

namespace cbsl {

class Thread  {
	private:
		/** \brief Flag to hold running state. Initially false */
		bool	m_isRunning;
		
		/** \brief Flag to hold termination state. Initially false */
		bool	m_isFinished;
		
		/** \brief Internal thread attributes */
		pthread_attr_t	m_attributes;
		
		/** \brief Internal pthread object */
		pthread_t	m_mythread;
		
		/** \brief	Return value from the thread, if any */
		int		m_retval;
        
        bool    m_cancelable;
        bool    m_asyncCancel;
		
		/** \brief Thread termination handler.
		 * \details This handler is called when a thread bails out,
		 *			wether by pthread_exit() or pthread_cancel().
		 *			pFlag must point to m_isFinished, so that the flag
		 *			is set to true when the thread terminates.
		 */
		static void thread_exit_handler( void *pFlag )
		{
			*(bool*)pFlag	= true;
		};
		
		static void *runWrapper( void *param );
		
	protected:
		/** \brief	Thread worker routine.
		 * \details	When start() is called, this method will be run in
		 *			the newly created thread. Derived classes must
		 *			implement it to create the desired functionality.
		 *			The thread finishes when either run() returns or
		 *			calls quit().
		 */
		virtual void run()	= 0;
		
		/** \brief	Return from the thread
		 * \details	quits the thread execution with return code retCode.
		 *			This method can be called from run() to finish its
		 *			execution with an exit code.
		 */
		void quit( int retCode );
		
	public:
		Thread();
		virtual ~Thread();
		
		/** \brief 	Start the execution of the thread worker.
		 * \details	Starts the execution of the worker thread.
         * \return Returns 0 on success or an error code on failure. The
         * error code is the same as errno returned by pthread_create()
		 * \sa run(), stop()
		 */
		int start();

		/** \brief Check if the thread finished execution
		 * \details	Checks if the thread finished its execution, after
		 *			run() started.
		 * \return	Returns true of the thread finished its execution.
		 *			If the run() is running or not started yet, returns
		 *			false.
		 */
		bool isFinished() const { return m_isFinished; };
		
		/** \brief 	Get the current size of the thread's stack.
		 * \details	Gets the size, in bytes, of the stack reserved for
		 *			the thread.
		 *			The size of the thread's stack can not be modified
		 *			after run() started running.
		 * \return	Returns the size, in bytes, of the stack for the
		 *			thread.
		 */
		unsigned long int getStackSize();
		
		/** \brief 	Set the size of the thread's stack
		 * \details	Set the new size of the stack for this thread.
		 * \note	This method can only be called if start() hasn't
		 *			been called yet. Once start() is called and run()
		 *			started execution, calls to setStackSize() will fail.
		 * \param newStackSize	New size, in bytes, of the stack for this
		 *						thread. If newStackSize is 0, then the
		 *						Thread object will use the operating
		 *						system's defaults.
         * \return Returns 0 on sucess or an error code on failure. The error
         * code is the result of a call to pthread_setstaksize().
		 * \note	Be aware that not all stack sizes might be supported
		 *			by all OSes. If setStackSize() is called with an
		 *			invalid value, the new thread may not start at all.
		 */
		int setStackSize(unsigned long int newStackSize);
		
		/** \brief	Wait for the thread to finish execution.
		 * \details	Waits for the thread to terminate for, atmost, usec
		 *			microseconds.
		 * \param usecs	Time, in microseconds to wait for the thread's
		 *				termination. If usecs=ULONG_MAX, then wait() will
		 *				wait forever until the thread finishes and never
		 *				times out. Otherwise it will timeout after usecs
		 *				microseconds and return even if the thread didn't
		 *				finish.
		 * \return	If the thread stopped execution returns true. Also
		 *			returns true if the thread never started at all.
		 *			If the call timedout withou the thread quitting,
		 *			returns false.
		 */
		bool wait(unsigned long int);
        
        
        /** \brief Send a signal to the thread.
         * \details Sends signal number \a signo to the Thread instance.
         * Returns 0 on success or a non-zero value on error.
         * \note \a signo must be a valid signal number as in header file
         * signal.h
         */
        int deliverSignal( int const signo );
        
        
        /** \brief Set the cancelability state of the thread.
         * \details Sets the cancelability state of the thread. This must be
         * done before launching the thread with \a start()
         */
        void setCancelable( bool cancelable ) { m_cancelable=cancelable; }
        
        /** \brief Set if the thread can be canceled assynchonously.
         */
        void setCancelAsync( bool async ) { m_asyncCancel=async; }

        /** \brief Wait until the thread exits.
         */
        void join();
        
		/** \brief Get the ID of the current thread.
		 * \return returns the ID of the calling thread.
		 */
		static pid_t getTid();
		
        /** \fn Thread::msSleep
         * \brief Put the current thread to sleep for, at least \a ms milliseconds.
         * \details    msSleep makes the calling thread sleep for at least the specified
                        time. However, if a signal is caught by the calling thread, the
                        call to msSleep may return prematurely.
         *\sa Thread::usSleep
         */
        static void msSleep( unsigned int ms );
		
        /* \fn Thread::usSleep
         *\brief Put the calling thread to sleep for at lease \a us microseconds.
         * \details    usSleep makes the calling thread sleep for at least the specified
         * time. However, if a signal is caught by the calling thread, the
         *call to usSleep may return prematurely.
         * \sa Threas::msSleep
         */
        static void usSleep( unsigned int us );		
}; // class Thread


}; // namespace Wcommon


#endif /* WC_THREAD_H */
