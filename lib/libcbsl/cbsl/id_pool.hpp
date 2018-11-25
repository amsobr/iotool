
#ifndef ID_POOL_HPP
#define ID_POOL_HPP

//#define IDPOOL_BIDIRECTIONAL

#include <stdio.h> // for fprintf, printf and FILE *
#include <stdint.h>
#include <assert.h>
#include <vector>

namespace cbsl {


/** \brief ID pool that executes in constant time - O(1)
 */
class IdPool {
    private:
        IdPool( IdPool const&);
        IdPool &operator=(IdPool const &);
        
        uint16_t const          firstId;
        uint16_t const          lastId;
        unsigned int const      nelem;
        uint16_t                head;
        uint16_t                tail;
        
        std::vector<uint16_t>   next;
#ifdef IDPOOL_BIDIRECTIONAL        
        std::vector<uint16_t>   prev;
#endif
        
        enum {
            BlankPos    = 0xffff        ,
        };
        
    public:
        IdPool( unsigned int ini , unsigned int end )
        : firstId(ini)      ,
        lastId(end)         ,
        nelem(end-ini+1)    ,
        head(0)             ,
        tail(end-ini)       ,
        next()              
#ifdef IDPOOL_BIDIRECTIONAL        
        ,prev()              
#endif
        {
            next.resize(nelem);
            head        = 0;
            tail        = end-ini;
            for ( unsigned int i(0) ; i<nelem ; ++i ) {
                next[i]     = i+1;
            }
            next[nelem-1]   = BlankPos;
            
#ifdef IDPOOL_BIDIRECTIONAL
            prev.resize(nelem);
            for ( unsigned int i(1) ; i<nelem ; ++i ) {
                prev[i]     = i-1;
            }
            prev[0]         = BlankPos;
#endif
        }
        
        ~IdPool()
        {
        }
        
        void dump( FILE *s ) {
            fprintf( s , "Dump of ID_POOL_V2\n" );
            fprintf( s , "firstId:     %d\n"
                    "lastId:      %d\n"
                    "nelem:       %d\n"
                    "head:        %d (id=%d)\n"
                    "tail:        %d (id=%d)\n"
                    ,firstId , lastId , nelem , head , head+firstId
                    , tail , tail+firstId
            );
            fprintf( s , "FREE List:\n" );
            fprintf( s , "HEAD -> " );
            for ( unsigned int pos=head ; pos!=BlankPos ; pos=next[pos] ) {
                
                /* detect not-free elements in the free list. */
                if ( pos==next[pos] ) {
                    fprintf( s , "\nelement %d (id=%d) is in use but apears in free list.\n" , pos , pos+firstId );
                    goto raw_dump;
                }
                               
#ifdef IDPOOL_BIDIRECTIONAL                               
                if ( pos==head && prev[head]!=BlankPos ) {
                    fprintf( s , "\nID %d, pointed to by HEAD, doesn't point back to HEAD\n" , pos+firstId ); 
                    goto raw_dump;
                }
#endif
                
                if ( pos==tail && next[pos]!=BlankPos ) {
                    fprintf( s , "\nID %d, pointed to by TAIL, doesn't point to Blank\n" , pos+firstId );
                    goto raw_dump;
                }
                
                if ( BlankPos==next[pos] && pos!=tail ) {
                    fprintf( s , "\nID %d points next to Blank but isn't TAIL.\n" , pos+firstId );
                    goto raw_dump;
                }
                
#ifdef IDPOOL_BIDIRECTIONAL
                if ( prev[pos]!=BlankPos && pos!=next[prev[pos]] ) {
                    fprintf( s , "\nbroken prev link between pos %d and %d\n", pos , prev[pos] );
                    goto raw_dump;
                }

                if ( next[pos]!=BlankPos && pos!=prev[next[pos]] ) {
                    fprintf( s , "\nbroken next link between pos %d and %d\n", pos , next[pos] );
                    goto raw_dump;
                }
#endif
                
                fprintf( s , "%d(id=%d) -> " , pos , firstId+pos );
            }
            fprintf( s , "\n" );
            
            fprintf( s , "IDS IN USE:\n" );
            for ( unsigned int i(0) ; i<nelem ; ++i ) {
#ifdef IDPOOL_BIDIRECTIONAL
                if ( next[i]==i && prev[i]==i ) {
#else
                if( next[i]==i ) {
#endif
                    fprintf( s , "%d(id=%d) -> " , i , firstId+i );
                }
            }
            fprintf( s , "\n" );
            return;
            
            raw_dump:
                fprintf( s , "RAW CONTENTS OF ID_MAP:\n" );
#ifdef IDPOOL_BIDIRECTIONAL                
                fprintf( s ,    " pos | id | prev (id) | next (id) \n" );
#else
                fprintf( s ,    " pos | id | next (id) \n" );
#endif
                for ( unsigned int i(0) ; i<nelem ; ++i ) {
#ifdef IDPOOL_BIDIRECTIONAL                    
                    fprintf( s , "  %02d | %02d |  %02d  (%02d) |  %02d  (%02d)\n",
                           i , firstId+i , prev[i] , firstId+prev[i] , next[i] , firstId+next[i] );
#else
                    fprintf( s , "  %02d | %02d |  %02d  (%02d)\n",
                           i , firstId+i , next[i] , firstId+next[i] );
#endif
                }
        }
        
        /** \brief Get the number of IDs in use in the pool */
        unsigned int countUsed() const {
            unsigned int cnt(0);
            for ( unsigned int i(0) ; i<nelem ; ++i ) {
                if ( next[i]==i ) {
                    ++cnt;
                }
            }
            return cnt;
        }
        
        /** \brief Get the total number of IDs in the pool */
        unsigned int size() const {
            return nelem;
        }
        
        
        /** \brief Aqcuire an ID and save it into \a id
         * \return Returns true on success or false on error
         */
        bool acquire( unsigned int *id ) {
            if ( BlankPos==head ) {
                printf( "Id Pool is exausted.\n" );
                return false;
            }
            
            *id                 = firstId+head;
            unsigned int pos    = head;
            head                = next[head];
#ifdef IDPOOL_BIDIRECTIONAL            
            prev[head]          = BlankPos;
            prev[pos]           = pos;
#endif            
            next[pos]           = pos;
            return true;
        }
            
#ifdef IDPOOL_BIDIRECTIONAL            
        bool acquireHint( unsigned int *id , unsigned int const hint ) {
            if ( hint<firstId || hint>lastId ) {
                return false;
            }
            
            int pos     = hint-firstId;
            
            assert( pos!=prev[pos] || pos!=next[pos] );
            if ( pos==prev[pos] && pos==next[pos] ) {
                printf("attempt to acquire an in-use ID: %d\n", hint );
                return false;
            }
            
            *id             = hint;
            int left        = prev[pos];
            int right       = next[pos];
            if ( BlankPos==left ) {
                head        = next[pos];
            }
            else {
                next[left]      = next[pos];
            }
            
            if ( BlankPos==right ) {
                tail            = prev[pos];
            }
            else {
                prev[right]     = prev[pos];
            }            
            prev[pos]           = pos;
            next[pos]           = pos;
            return true;
        }
#endif
        
        void release( unsigned int const id ) {
            assert( id>=firstId && id<=lastId );
            if ( id<firstId || id>lastId ) {
                printf( "Id out of range: %d (first=%d , last=%d)\n" , id , firstId , lastId );
                return;
            }

            int pos = id-firstId;
            
#ifdef IDPOOL_BIDIRECTIONAL
            assert( pos==next[pos] && pos==prev[pos] );
            if ( pos!=next[pos] || pos!=prev[pos] ) {
                printf( "Attempt to release an unused id: %d\n", id );
                return;
            }
#else
            assert( pos==next[pos] );
            if ( pos!=next[pos] ) {
                printf( "Attempt to release an unused id: %d\n", id );
                return;
            }
#endif
            
            next[pos]   = BlankPos;
            if ( BlankPos==head ) {
                head        = pos;
#ifdef IDPOOL_BIDIRECTIONAL
                prev[pos]   = BlankPos;
#endif
            }
            else {
                next[tail]  = pos;
#ifdef IDPOOL_BIDIRECTIONAL
                prev[pos]   = tail;
#endif
            }
            tail            = pos;
        }
        
        void releaseAll() {
            head        = 0;
            tail        = nelem-1;
            for ( unsigned int i(0) ; i<nelem ; ++i ) {
                next[i]     = i+1;
            }
            next[nelem-1]   = BlankPos;
            
#ifdef IDPOOL_BIDIRECTIONAL
            for ( unsigned int i(1) ; i<nelem ; ++i ) {
                prev[i]     = i-1;
            }
            prev[0]         = BlankPos;
#endif
        }
}; /* class IdPool */

/**
\file id_pool.hpp
\brief This module implements a pool of ID of a given integer type
\author Antonio Oliveira (antonio.oliveira@withus.pt)
\date   Jannuary 2014

\class IdPool
\brief Pool of integer values to acquire and release in an unique fashion
\details This class implements a pool of integers that may be acquired and
         released by an application, ensuring that que aquired IDs are always
         unique within their range.
         An ID may be reused if it is released at some point.
         
\fn IdPool::assignRange()
\brief Assign one range of integers to the pool of free ids.
\details Assigns the range given by \a ini to \a end, inclusive to the pool
         of free IDs. These values become imediately available for subsequent
         calls to \a aquire.
\param ini Initial value of the range. \a ini is included in the range
\param end Final value of the range. \a end is included in the range.
\note if \a ini >= \a end, no range will be assigned.
\sa IdPool::addId()

\fn IdPool::addId()
\brief Add a single value into the range of free IDs
\param id Value to add to the pool of free IDs. If ID already exists on the
          range of IDs, regardless of being free or used, no id will be added.
\sa IdPool::assignRange();
          
\fn IdPool::acquire()
\brief Get one unique free ID
\details Gets one free ID from the pool and writes it in \a id.  This ID
         is marked as in use and will not be available until it is released
         by calling \a IdPool::release().
\return Returns 0 on sucess or -1 in case the pool of free IDs is exausted.
\sa IdPool::release()

\fn IdPool::release()
\brief Release an used ID and mark it as free.
\details Releases an ID and marks it as free. If \a id is not found on the
         list of used IDs, nothing will be done. Once released, this ID
         becomes available on future calls of \a IdPool::acquire()
\param id value of the ID to be released.

\fn IdPool::acquireHint()
\brief Get one ID with some desired value.
\details Tries to acquire an ID with value \a hint and store it in \a id.
        If there is no ID available with the requested value, the call
        doesn't provida any ID and returns -1. If the request ID is available
        it is marked as in-use, stored in \a id and the call returns 0.

\fn IdPool::releaseAll()
\brief Mark all IDs on the managed range as free.
*/


}; /* namespace Wcommon */

#endif /* ! defined( ID_POOL_HPP ) */
