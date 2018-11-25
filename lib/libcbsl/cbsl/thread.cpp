
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include "thread.hpp"


namespace cbsl {

void *Thread::runWrapper( void *param )
{
	Thread *self		= (Thread*)(param);
	self->m_isRunning	= true;
    if ( self->m_cancelable ) {
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,0);
        if ( self->m_asyncCancel ) {
            pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS , 0 );
        }
        else {
            pthread_setcanceltype( PTHREAD_CANCEL_DEFERRED , 0 );
        }
    }
    else {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,0);
    }
	self->run();
	self->m_isRunning	= false;
	self->m_isFinished	= true;
	pthread_exit(0);
}


void Thread::quit( int retCode )
{
	pthread_exit((void*)&retCode); // this call does not return!!
}


Thread::Thread()
: m_isRunning(false) ,
m_isFinished(false)  ,
m_attributes()       ,
m_mythread()         ,
m_retval()           ,
m_cancelable(true)   ,
m_asyncCancel(false)
{
	pthread_attr_init( &m_attributes );
    pthread_attr_setdetachstate( &m_attributes , PTHREAD_CREATE_JOINABLE);

}

// virtual destructor
Thread::~Thread()
{
	pthread_attr_destroy( &m_attributes );
}
		
int Thread::start()
{
	int ret = pthread_create( &m_mythread , &m_attributes , &runWrapper , this );
    return ret;
}
		
unsigned long int Thread::getStackSize()
{
	size_t stack_size;
	pthread_attr_getstacksize( &m_attributes , &stack_size );
	return (unsigned long int) stack_size;
}

int Thread::setStackSize(unsigned long int newStackSize)
{
	int ret = pthread_attr_setstacksize( &m_attributes , newStackSize );
    return ret;
}


bool Thread::wait(unsigned long int)
{
	pthread_join( m_mythread , (void**)&m_retval );
	return true;
}

int Thread::deliverSignal( int const signo )
{
    return pthread_kill( m_mythread , signo );
}


// int Thread::cancel()
// {
//     int res = pthread_cancel( m_mythread );
//     if ( 0 != res ) {
//         int code = errno;
//         return code;
//     }
//     
//     res = pthread_join( m_mythread , 0 );
//     if ( 0 != res ) {
//         int code = errno;
//         return code;
//     }
//     return 0;
// }

void Thread::join()
{
    pthread_join(m_mythread,0);
}

pid_t Thread::getTid()
{
	return syscall(SYS_gettid);
}

void Thread::msSleep( unsigned int ms )
{
	usleep( ms*1000 );
}

void usSleep( unsigned int us )
{
	usleep(us);
}


}; /* namespace Wcommon */
