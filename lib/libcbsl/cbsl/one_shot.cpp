
#include <vector>

#include <semaphore.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "nocopy.hpp"
#include "mutex.hpp"
#include "threadpool.h"
#include "threadpooltask.h"
#include "one_shot.h"
#include "timespec.h"

class Dispatcher__ : public cbsl::ThreadPoolTask {
private:
    WC_DISABLE_COPY(Dispatcher__);
    
    void (*myFn)(void*,void*);
    void *p1;
    void *p2;
    
public:
    Dispatcher__( void(*fn__)(void*,void*) , void *p1__ , void *p2__ ) :
    cbsl::ThreadPoolTask()   ,
    myFn(fn__)  ,
    p1(p1__)    ,
    p2(p2__)
    {
    }
    
    ~Dispatcher__()
    {
    }
    
    virtual void execute()
    {
        (*myFn)(p1,p2);
    }
}; /* class Dispatcher__ */


namespace cbsl {


struct Episode {
    TimeSpec tFire;
    std::vector<Dispatcher__*> handlers;
    
    Episode( TimeSpec ts ) :
    tFire(ts)    ,
    handlers()
    {
    }
    
    Episode const &operator=( Episode const &other )
    {
        tFire       = other.tFire;
        handlers    = other.handlers;
        return *this;
    }
};


OneShot::OneShot(logctl_t *l__) :
Thread()            ,
myStop(false)       ,
myTmrEvt(false)     ,
myMutex(new Mutex)  ,
mySem()             ,
myNewEvts()         ,
logger(l__)
{
    sem_init( &mySem , 0 , 0 );
//     LOG_PRINT( logger , "ONE_SHOT starting thread.\n" );
    start();
}

OneShot::~OneShot()
{
    myStop      = true;
    delete myMutex;
    sem_destroy( &mySem );
}


long int OneShot::add( void(*fnptr)(void*,void*), void* key0, void* key1 , unsigned int msdelay )
{
    myMutex->lock();
    myNewEvts.push_back( NewEntry(fnptr,key0,key1,msdelay) );
    myMutex->unlock();
    sem_post( &mySem );
    return 0;
} /* OneShot::add() */


void evtTmrWrapper( union sigval v );

void OneShot::run()
{
//     pid_t tid   = Thread::getTid();
    ThreadPool pool;
    pool.setNumWorkers(4);
    pool.launchWorkers();
    std::list<Episode> episodes;
    timer_t evtTmr;
    struct sigevent tmrHnd;
    memset( &tmrHnd , 0 , sizeof(struct sigevent) );
    tmrHnd.sigev_notify     = SIGEV_THREAD;
    tmrHnd.sigev_notify_function    = evtTmrWrapper;
    tmrHnd.sigev_value.sival_ptr    = this;
    timer_create( CLOCK_MONOTONIC , &tmrHnd , &evtTmr );
    
    do {
        int sr(0);
//         LOG_PRINT( logger , "ONE_SHOT worker --> sleep\n" );
        do {
            if ( sem_wait(&mySem)!=0 ) {
                sr  = errno;
            }
        } while( sr==EINTR );
        
        if ( myStop ) {            
            break;
        }
        
        myMutex->lock();        
        if ( !myNewEvts.empty() ) {
            NewEntry newEntry(myNewEvts.front());
            myNewEvts.pop_front();
//             LOG_PRINT( logger , "ONE_SHOT: incoming event registry: when=%.3f\n" , newEntry.tFire.asDouble() );
            std::list<Episode>::iterator it;
            if ( episodes.empty() ) {
//                 LOG_PRINT( logger , "ONE_SHOT: program is empty. Adding first element.\n" );
                episodes.push_back( Episode(newEntry.tFire) );
                it  = episodes.begin();
            }
            else {
                for ( it=episodes.begin() ; it!=episodes.end() ; ++it ) {
                    if ( (*it).tFire==newEntry.tFire ) {
//                         LOG_PRINT( logger , "ONE_SHOT: will append to episode @when=%.3f\n" , (*it).tFire.asDouble() );
                        /* exact episode exists. We may simply add another event to it */
                        break;
                    }
                    else if ( ((*it).tFire)>(newEntry.tFire) ) {
//                         LOG_PRINT( logger , "ONE_SHOT: inserting episode before @when=%.3f\n" , (*it).tFire.asDouble() );
                        it  = episodes.insert( it , Episode(newEntry.tFire) );
                        break;
                    }
                }
                if ( it==episodes.end() ) {
//                     LOG_PRINT( logger , "ONE_SHOT: insert episode at program end.\n" );
                    it  = episodes.insert( it , Episode(newEntry.tFire) );
                }
            }
            (*it).handlers.push_back( new Dispatcher__(newEntry.fn,newEntry.p1,newEntry.p2) );
            if ( it==episodes.begin() ) {
//                 LOG_PRINT( logger , "ONE_SHOT: episode was inserted at program head. Re-scheduling.\n" );
                struct itimerspec t;
                memset( &t , 0 , sizeof(struct itimerspec) );
                newEntry.tFire.getTs( &(t.it_value) );
                timer_settime( evtTmr , TIMER_ABSTIME , &t , 0 );                
            }            
        }
        else if ( myTmrEvt ) {
//             LOG_PRINT( logger , "ONE_SHOT: timer expired => executing events.\n" );
            myTmrEvt    = false;
            struct timespec now_;
            clock_gettime( CLOCK_MONOTONIC , &now_ );
            TimeSpec now(now_);
            while ( !episodes.empty() && !(now<episodes.front().tFire) ) {
                Episode e( episodes.front() );
                episodes.pop_front();
//                 LOG_PRINT( logger , "ONE_SHOT: firing episode @t=%.3f with %d events\n" , e.tFire.asDouble() , e.handlers.size() );
                for ( size_t i=0 ; i<e.handlers.size() ; ++i ) {
                    Dispatcher__ *d = e.handlers[i];
                    pool.addTask(d);
                    d->requestWork();
                }
            }
            if ( !episodes.empty() ) {
                /* schedule next Episode execution */
                struct itimerspec t;
                memset( &t , 0 , sizeof(struct itimerspec) );
                episodes.front().tFire.getTs( &(t.it_value) );                
                timer_settime( evtTmr , TIMER_ABSTIME , &t , 0 );
//                 LOG_PRINT( logger , "ONE_SHOT: rescheduled timer to t=%.3f\n" , episodes.front().tFire.asDouble() );                
            }
            else {
//                 LOG_PRINT( logger , "ONE_SHOT: program is empty. do not rearm timer\n" );
            }
            
//             std::list<Episode>::iterator it = episodes.begin();
//             for ( int i=0 ; i<10 ; ++i ) {
//                 if ( it==episodes.end() ) break;
//                 LOG_PRINT( logger , "ONE_SHOT: episode[%d] @t=%.3f e<now=%d e>now=%d e==now=%d e!=now=%d\n" , i , (*it).tFire.asDouble()
//                     , (*it).tFire<now?1:0 ,
//                            (*it).tFire>now?1:0 ,
//                            (*it).tFire==now?1:0,
//                            (*it).tFire!=now?1:0                           
//                 );
//                 ++it;
//             }
        }
        else {
            fprintf( stderr ,  "ONE_SHOT: ##### Abnormal iteration, no actions found.\n" );
            /* abnormal wake??? */
        }
        myMutex->unlock();
        
    } while( !myStop );
    
    fprintf( stderr , "ONE_SHOT: worker stopping.\n" );
    
    struct itimerspec it;
    memset( &it , 0 , sizeof(it) );
    timer_settime( &evtTmr , 0 , &it, 0 );
    timer_delete( &evtTmr );
    
    while ( ! episodes.empty() ) {
        Episode e   = episodes.front();
        for ( size_t i=0 ; i<e.handlers.size() ; ++i ) {
            delete e.handlers[i];
        }
    }
}





void OneShot::cb_onTmrEvt( void *p__ )
{
    OneShot *self   = (OneShot*)p__;
//     LOG_PRINT( self->logger , "ONE_SHOT: callback executing...\n" );
    self->myMutex->lock();
    self->myTmrEvt  = true;
    self->myMutex->unlock();
    sem_post( &(self->mySem) );    
}

void evtTmrWrapper( union sigval v ) {
    OneShot::cb_onTmrEvt(v.sival_ptr);
}




}; /* namespace Wcommon */
