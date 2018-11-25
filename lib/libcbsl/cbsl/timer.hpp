#ifndef TIMER_HPP
#define TIMER_HPP

#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


/** \file timer.h
 * \brief   This module implements a new generation of timers.
 * \author  António Oliveira - Withus, Lda (antonio.oliveira@withus.pt)
 */
    
namespace cbsl {
/** \class Timer
 * \brief   This class implements a timer with an event mechanism
 * \details This class implements a timer, able to run for a specified period
 *          of time, either once or ciclicly, and call an event handler each
 *          time it expires.
 *          More details to be added later.
 */
class Timer {
    private:
        timer_t myTimerId;
        unsigned long int myMsInterval;
        bool    myOneShot;
        /** Interval in posix timer format */
        struct itimerspec myInterval;
        void (*myHandler)(void*);
        void *myKey;
        
        static void timeoutWrapper( union sigval s ) {
            Timer *t  = (cbsl::Timer*)s.sival_ptr;
            (*(t->myHandler))(t->myKey);
        }
        
        Timer( Timer const &);
        Timer& operator=(Timer const &);
        
    public:
        typedef enum return_code {
            Ok              = 0     ,
            TimerError              ,
            TimerIsRunning          ,
            TimerIsStopped          ,
            IntervalNotSet          ,
            InvalidThread           ,
        } ReturnCode;
        
        /** \brief  Default Constructor.
         * \details Newly created timers are set as one-shot with a timeout
         *          of 0 milliseconds.
         * \param   fnpr    Pointer to a static function which will be called
         *                  to handle the timeout event.
         * \param   key     Value that will be passed as parameter to the
         * 
         *                  the function on timeout event.
         */
        Timer( void (*fnptr)(void*) , void *key ) :
        myTimerId(0)            ,
        myMsInterval(0)         ,
        myOneShot(true)         ,
        myInterval()            ,
        myHandler(fnptr)        ,
        myKey(key)
        {
            myInterval.it_interval.tv_sec       = 0;
            myInterval.it_interval.tv_nsec      = 0;
            myInterval.it_value.tv_sec          = 0;
            myInterval.it_value.tv_nsec         = 0;

            struct sigevent tmrSig;
            memset(&tmrSig,0,sizeof(struct sigevent));
            tmrSig.sigev_notify                 = SIGEV_THREAD;
            tmrSig.sigev_signo                  = SIGALRM;
            tmrSig.sigev_value.sival_ptr        = this;
            tmrSig.sigev_notify_function        = &Timer::timeoutWrapper;
            tmrSig.sigev_notify_attributes      = NULL;
    
            int res = timer_create( CLOCK_MONOTONIC , &tmrSig , &myTimerId );
            if ( res != 0 ) {
                int errCode = errno;
                fprintf( stderr , "timer: Unable to create timer. errno=%d\n" , errCode );
                abort();
            }
        }
        
        /** \brief  Destructor */
        ~Timer()
        {
            timer_delete( myTimerId );
        }


        /** \brief  Set the timer interval
         * \details Sets the interval of the timer. This can only be done
         *          if the timer hasn't been yet fired up. Once the timer is
         *          launched, calls to setInterval() will fail.
         * \param msInterval    Interval to set for the timer. The interval
         *                      is given in milliseconds.
         * \return  Returns Timer::Ok on success. If the timer is already
         *          running returs Timer::TimerIsRunning and the interval
         *          is not set.
         */
        ReturnCode setInterval( unsigned long int msInterval )       
        {
            if ( isRunning() ) {
                fprintf( stderr , "timer: Can not change interval of a running timer.\n" );
                return TimerIsRunning;
            }
            unsigned long int secs  = msInterval/1000;
            unsigned long int nsecs = (msInterval%1000)*1000*1000;
            myInterval.it_value.tv_sec          = secs;
            myInterval.it_value.tv_nsec         = nsecs;
            if ( myOneShot ) {
                myInterval.it_interval.tv_sec   = 0;
                myInterval.it_interval.tv_nsec  = 0;
            }
            else {
                myInterval.it_interval.tv_sec   = myInterval.it_value.tv_sec;
                myInterval.it_interval.tv_nsec  = myInterval.it_value.tv_nsec;
            }
            return Ok;
        }
        
        
        /** \brief  Get the current timer interval
         * \details Reads the interval that is currently set on the timer.
         * \return  Returns the current timer value, in miliseconds. This
         *          method never fails.
         */
        unsigned long int getInterval() const { return myMsInterval; };
        
        /** \brief  Select one-shot or continuous operation
         * \details Puts the timer in one-shot, or continuous operation, 
         *          as set by the oneShot flag. This method \em must be
         *          called before start().
         * \param oneShot   If true, selects one-shot operation. If false, 
         *                  selects continuous operation.
         * \return  Returns Timer::Ok if successful. If the timer is already
         *          running, the mode is not set, and returns an error.
         *          Check Timer::ReturnCode for a list of error codes.
         * \sa  Timer::ReturnCode
         */
        ReturnCode setOneShot( bool oneshot )
        {
            if ( isRunning() ) {
                fprintf( stderr , "timer: Cannot change oneshot of a running timer.\n" );
                return TimerIsRunning;
            }
            else {
                myOneShot   = oneshot;
            }
            if ( oneshot ) {
                myInterval.it_interval.tv_sec   = 0;
                myInterval.it_interval.tv_nsec  = 0;
            }
            else {
                myInterval.it_interval.tv_sec   = myInterval.it_value.tv_sec;
                myInterval.it_interval.tv_nsec  = myInterval.it_value.tv_nsec;
            }
            return Ok;
        }

        
        /** \brief Get the timer mode of operation of the timer
         * \return  Returns the one-shot mode of operation. If the timer is
         *          set to one-shot mode, returns true, otherwise returns
         *          false.
         */
        bool getOneShot() const { return myOneShot; };


        /** \brief  Start the timer
         * \details Starts the timer. The timer must have been previously
         *          configured with setTimeout(), setOneShot() and connect().
         *          When calling start() on a timer that is not running, the
         *          timer starts with the interval set with setInterval().
         * \return  If the timer is started, returns Ok. Otherwise, the
         *          timer does not start, and returns an error code.
         * \sa Timer::ReturnCode
         */
        ReturnCode start()
        {
            int res = timer_settime( myTimerId , 0 , &myInterval , 0 );
            if ( res != 0 ) {
                int errCode = errno;
                fprintf( stderr , "timer: start failed - errno=%d\n", errCode );
                return TimerError;
            }
            return Ok;
        }


        /** \brief  Check if the timer is running
         * \return  Returns true if the timer is running, otherwise returns
         *          false.
         * \note    If the timer has not yet started, or if the timer is set
         *          as single-shot and has already timed out, this call will
         *          return false.
         */
        bool isRunning()
        {
            struct itimerspec cur;
            int res = timer_gettime( myTimerId ,  &cur );
            if ( 0 != res ) {
                int code    = errno;
                fprintf( stderr , "timer  current time read failed. errno=%d\n", code );
                return false; // FIXME: This can foul the caller to a false negative
            }
            
            if ( 0==cur.it_value.tv_sec && 0==cur.it_value.tv_nsec ) {
                return false;
            }
            else {
                return true;
            }
        }
        
        
        /** \brief  Stop the timer
         * \details The timer count is suspended until a call to start() or
         *          resume().
         * \return  If the timer was suspended this way, returns Ok.
         *          If some error occurs, returns the apropriate error code.
         * \sa Timer::ReturnCode
         */
        ReturnCode stop()
        {
            struct itimerspec halt;
            halt.it_value.tv_sec    = 0;
            halt.it_value.tv_nsec   = 0;
            halt.it_interval.tv_sec = 0;
            halt.it_interval.tv_nsec= 0;
            
            int res = timer_settime( myTimerId , 0 , &halt , 0 );
            if ( res != 0 ) {
                int code = errno;
                fprintf( stderr , "timer: stop failed; errno=%d.\n" , code );
                return TimerError;
            }
            return Ok;
        }
        
}; /* class Timer */

}; /* namespace Wcommon */


#endif /* TIMER_HPP */
