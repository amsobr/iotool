#ifndef WC_TIMESPEC_H
#define WC_TIMESPEC_H

#include <time.h>

namespace cbsl {

class TimeSpec {

private:
    struct timespec myTs;

public:
    TimeSpec() :
    myTs()
    {
    }
    
    TimeSpec( time_t sec_ , long nsec_ ) :
    myTs()
    {
        myTs.tv_sec     = sec_;
        myTs.tv_nsec    = nsec_;
        while ( myTs.tv_nsec>999999999 ) {
            myTs.tv_nsec    -= 1000000000;
            ++myTs.tv_sec;
        }
    }
    
    TimeSpec( TimeSpec const &other ) :
    myTs(other.myTs)
    {
    }
    
    TimeSpec( struct timespec ts__ ) :
    myTs(ts__)
    {
        while ( myTs.tv_nsec>999999999 ) {
            myTs.tv_nsec    -= 1000000000;
            ++myTs.tv_sec;
        }

    }
    
    ~TimeSpec()
    {
    }
    
    
    TimeSpec const &operator=( struct timespec ts )
    {
        myTs.tv_sec = ts.tv_sec;
        myTs.tv_nsec= ts.tv_nsec;
        while ( myTs.tv_nsec>999999999 ) {
            myTs.tv_nsec    -= 1000000000;
            ++myTs.tv_sec;
        }

        return *this;
    }
    
    
    TimeSpec const &operator=( TimeSpec const &other )
    {
        myTs.tv_sec = other.myTs.tv_sec;
        myTs.tv_nsec= other.myTs.tv_nsec;
        return *this;
    }
    
    
    TimeSpec const operator+( TimeSpec const &other )
    {
        struct timespec t;
        t.tv_sec    = myTs.tv_sec + other.myTs.tv_sec;
        t.tv_nsec   = myTs.tv_nsec+ other.myTs.tv_nsec;
        while ( myTs.tv_nsec>999999999 ) {
            myTs.tv_nsec    -= 1000000000;
            ++myTs.tv_sec;
        }
        return TimeSpec(t);
    }
    
    TimeSpec const &operator+=( TimeSpec const &other )
    {
        myTs.tv_sec += other.myTs.tv_sec;
        myTs.tv_nsec+= other.myTs.tv_nsec;
        while ( myTs.tv_nsec>999999999 ) {
            myTs.tv_nsec    -= 1000000000;
            ++myTs.tv_sec;
        }
        return *this;
    }
    
    bool operator<( TimeSpec const &other ) const
    {
        if ( myTs.tv_sec < other.myTs.tv_sec ) {
            return true;
        }
        
        if ( myTs.tv_sec==other.myTs.tv_sec && myTs.tv_nsec<other.myTs.tv_nsec ) {
            return true;
        }
        
        return false;
    }
    
    bool operator>( TimeSpec const &other ) const
    {
        if ( myTs.tv_sec>other.myTs.tv_sec ) {
            return true;
        }
        
        if ( myTs.tv_sec==other.myTs.tv_sec && myTs.tv_nsec>other.myTs.tv_nsec ) {
            return true;
        }
        
        return false;        
    }
    
    bool operator==( TimeSpec const &other ) const
    {
        if ( myTs.tv_sec!=other.myTs.tv_sec || myTs.tv_nsec!=other.myTs.tv_nsec ) {
            return false;
        }
        return true;
    }
    
    bool operator!=( TimeSpec const &other ) const
    {
        if ( myTs.tv_sec!=other.myTs.tv_sec || myTs.tv_nsec!=other.myTs.tv_nsec ) {
            return true;
        }
        return false;
    }
    
    
    void getTs( struct timespec *ts ) const
    {
        ts->tv_sec  = myTs.tv_sec;
        ts->tv_nsec = myTs.tv_nsec;
    }
    
    double asDouble() const
    {
        return myTs.tv_sec*1.0 + myTs.tv_nsec*1e-9;
    }
    
    static inline TimeSpec now( clockid_t clkid ) {
        struct timespec ts;
        clock_gettime( clkid , &ts );
        return TimeSpec(ts);
    }
    
    static unsigned long long timestampInMillis() {
        struct timespec ts;
        clock_gettime( CLOCK_MONOTONIC , &ts );
        return ts.tv_sec*1000 + ts.tv_nsec/1000000;
    }
    
}; /* class TimeSpec */

}; /* namespact cbsl */



#endif /* ! defined(WC_TIMESPEC_H) */
