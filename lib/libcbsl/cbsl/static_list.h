/**
 * \file static_list.hpp
 * \brief This module implements a pool of ID of a given integer type
 * \author António Oliveira
 */
#ifndef STATIC_LIST_H
#define STATIC_LIST_H

#include <stdio.h> // for fprintf, printf and FILE *
#include <stdint.h>
#include <assert.h>
#include <vector>
#include "logger.h

/**
 * \class StaticList
 * \brief High performance, fully static list of integers
 * \details This class implements a bidirectional linked list of integers, 
 *          that does not require the usage of dynamic memory, other than the
 *          initial allocation for its internal data, used by the member
 *           std::vectors \n
 *          This list takes the followind assumptions:
 *          * members are integers.
 *          * the rance of integers allowed on this list is continuous
 *          * each member may belong to the list just once. Re-adding the same
 *            member to the list either leaves the list as is.
 *          * All operations execute in constant time (add,remove,pop_head,
 *            pop_tail,head,tail,size)
 *          * the list is not thread-safe. It is up to the caller to ensure
 *            thread safety
 *          
 *          The maximum allowed size of the list is 65535 elements. The range
 *          of the elements may be any integer value.
 * \par Implementation
 *      Elements are identified by position, thus not requiring any storage at
 *      all. The linking elements are made of two arrays of indexes that
 *      link each list element to their neighbours (nxt and prv).
 */
         

namespace cbsl {

class StaticList {
    private:
        StaticList( StaticList const&);
        StaticList &operator=(StaticList const &);
        
        int const               myFirstId;
        int const               myLastId;
        size_t                  myNelem;
        size_t                  mySize;
        uint16_t                myHead;
        uint16_t                myTail;
        
        /* next or prev point to next or previous elements. A pointer to 
         * BlankPos is a null pointer (element not in list)
         */
        std::vector<uint16_t>   myNext;
        std::vector<uint16_t>   myPrev;
        
        enum {
            BlankPos    = 0xffff        ,
        };
        
    public:
        /** 
         * \fn StaticList::StaticList()
         * \brief Create a StaticList with given range of possible members
         * \details Creates the list object. The range of elements allowed in
         * the list is defined by \a ini and \a end, inclusive.
         * The initial list is empty.
         */
        StaticList( int const ini , int const end ) :
        myFirstId(ini)        ,
        myLastId(end)         ,
        myNelem(end-ini+1)    ,
        mySize(0)             ,
        myHead(BlankPos)      ,
        myTail(BlankPos)      ,
        myNext()              ,
        myPrev()              
        {
            myNext.resize(myNelem);
            myPrev.resize(myNelem);
            for ( unsigned int i(0) ; i<myNelem ; ++i ) {
                myNext[i]     = BlankPos;
                myPrev[i]     = BlankPos;
            }            
        }
        
        ~StaticList()
        {
        }
        
        inline void dump( FILE *s ) const {
            fprintf( s , "Dump of StaticList\n" );
            fprintf( s , "firstId:     %d\n"
                    "lastId:      %d\n"
                    "nelem:       %d\n"
                    "curSize      %d\n"
                    "head:        %d (id=%d)\n"
                    "tail:        %d (id=%d)\n"
                    ,myFirstId , myLastId , myNelem , mySize , myHead , myHead+myFirstId
                    , myTail , myTail+myFirstId
            );
            fprintf( s , "List:\n" );
            fprintf( s , "HEAD -> " );
            unsigned int cnt    = 0;            
            unsigned int useCnt = 0;
            if ( mySize>0 ) {
                for ( unsigned int pos=myHead ;  ; pos=myNext[pos] ) {
                    ++cnt;
                    if ( pos==myNext[pos] && myTail!=pos ) {
                        fprintf( s , "\nTAIL link broken at element %d (id=%d)\n" , pos , pos+myFirstId );
                        goto raw_dump;
                    }
                                
                    if ( pos==myHead && myPrev[pos]!=pos ) {
                        fprintf( s , "\nHEAD link broken at element %d (id=%d)\n" , pos , pos+myFirstId ); 
                        goto raw_dump;
                    }
                    
                    if ( myNext[pos]==BlankPos || myPrev[pos]==BlankPos ) {
                        fprintf( s  , "\nCORRUPT element %d (id=%d) prev=%d next=%d\n" , pos , pos+myFirstId , myPrev[pos] , myNext[pos] );
                        goto raw_dump;
                    }
                    
                    if ( pos!=myHead ) {
                        if ( pos!=myNext[myPrev[pos]] ) {
                            fprintf( s , "\nbroken prev link between pos %d (id=%d) and %d (id=%d)\n", pos , pos+myFirstId , myPrev[pos] , myPrev[pos]+myFirstId );
                            goto raw_dump;
                        }
                    }

                    if ( pos!=myTail && pos!=myPrev[myNext[pos]] ) {
                        fprintf( s , "\nbroken next link between pos %d (id=%d) and %d (id=%d)\n", pos , pos+myFirstId , myNext[pos] , myNext[pos]+myFirstId );
                        goto raw_dump;
                    }
                    
                    fprintf( s , "%d(id=%d) -> " , pos , myFirstId+pos );
                    if ( pos==myTail ) {
                        break;
                    }
                        
                }
            }
            fprintf( s , " {END|TAIL}\n" );
            
            if ( cnt!=mySize ) {
                fprintf( s , "\nElements found in list (%d) and mySize (%d) mismatch!\n" , cnt , mySize );
                goto raw_dump;
            }

            for ( unsigned int i=0 ; i<myNelem ; ++i ) {
                if ( myNext[i]!=BlankPos && myPrev[i]!=BlankPos ) {
                    ++useCnt;
                }
            }
            if ( useCnt!=mySize ) {
                fprintf( s , "\nElement count is wrong. nonfree=%d mySize=%d\n" , useCnt , mySize );
                goto raw_dump;
            }            
                        
            return;
            
            raw_dump:
                fprintf( s , "RAW CONTENTS OF STATIC_LIST:\n" );
                fprintf( s ,    " pos | id | prev (id) | next (id) \n" );
                for ( unsigned int i(0) ; i<myNelem ; ++i ) {
                    fprintf( s , "  %02d | %02d |  %02d  (%02d) |  %02d  (%02d)\n",
                           i , myFirstId+i , myPrev[i] , myFirstId+myPrev[i] , myNext[i] , myFirstId+myNext[i] );
                }
        }
        
        /** \brief Get the current size of the list.
         */
        inline size_t size() const {
            return mySize;
        }
        
        /** \brief Check if the list is empty.
         * \return Returns true if the list is empty.
         * \sa size
         */
        inline bool empty() const {
            return mySize==0;
        }
        
        
        /** \brief Returns the element at HEAD position in the list.
         * \note When the list is empty, the result is undefined.
         * \sa size , empty , myTail
         */
        inline int head() const {
            return myHead+myFirstId;
        }
        
        
        /** \brief Pops the element at HEAD position in the list.
         * \note When the list is empty, the result is undefined.
         * \sa head, size, empty
         */
        inline int pop_head() {
            if ( mySize==0 ) {
                LOG_PRINT( verb.main , "##### pop head withoud data!!!\n" );
                return 0;
            }
            
            int pos = myHead;
            if ( mySize==1 ) {
                myHead          = BlankPos;
                myTail          = BlankPos;
            }
            else {
                myHead          = myNext[pos];
                myPrev[myHead]  = myHead;
            }
            myPrev[pos]         = BlankPos;
            myNext[pos]         = BlankPos;
            --mySize;
//             LOG_PRINT( verb.main , "POP_HEAD (id=%d)\n" , pos+myFirstId );
//             dump(logger_get_stream(verb.main));
            return pos+myFirstId;
        } /* pop_head() */
        
        
        
        /** \brief Push an element into the HEAD of the list.
         * \details Pushes an integer into the head of the list. The integer
         * \em must be within the range assigned to the list, otherwise the
         * operation is undefined.\n
         * If the element was already in the list, nothing is done on the list.
         * \return Returns true only when the command is executed.
         */
        inline bool push_head( int id ) {
            if ( id<myFirstId || id>myLastId ) {
                fprintf( stderr , "StaticList: ini=%d end=%d , attempt to push_head %d\n" , myFirstId , myLastId , id );
                return false;
            }
            
            int pos = id-myFirstId;
            
            if ( myNext[pos]!=BlankPos ) {
//                 LOG_PRINT( verb.main , "PUSH_HEAD (id=%d)\n" , id );
//                 dump(logger_get_stream(verb.main));
                return true;
            }
            
            
            if ( myHead==BlankPos ) {                
                myNext[pos]     = pos;
                myTail          = pos;
            }
            else {
                myNext[pos]     = myHead;
                myPrev[myHead]  = pos;
            }
            myHead              = pos;
            myPrev[pos]         = pos;
            ++mySize;
//             LOG_PRINT( verb.main , "PUSH_HEAD (id=%d)\n" , id );
//             dump(logger_get_stream(verb.main));
            return true;
        } /* push_head */
        
        
        /** \brief Returns the element at TAIL of the list.
         * \note When the list is empty, the result is undefined.
         * \sa size, empty, head
         */
        inline int tail() const 
        {
            return myTail+myFirstId;
        }
        
        
        /** \brief Pop the element at TAIL and return it's value
         * \note When the list is empty, the result is undefined
         */
        inline int pop_tail()
        {
            if ( mySize==0 ) {
                LOG_PRINT( verb.main , "###### POP TAIL With no data.!!!\n" );
                return 0;
            }
            
            int pos             = myTail;
            if ( mySize==1 ) {
                myTail          = BlankPos;
                myHead          = BlankPos;
            }
            else {
                myTail          = myPrev[pos];
                myNext[myTail]  = myTail;
            }
            
            myNext[pos]         = BlankPos;
            myPrev[pos]         = BlankPos;
            --mySize;
//             LOG_PRINT( verb.main , "POP_TAIL (id=%d)\n" , pos+myFirstId );
//             dump(logger_get_stream(verb.main));
            return pos+myFirstId;                
        } /* pop_tail() */
        
        
        /** \brief Push an element into the TAIL of the list.
         * \details Pushes an integer into the tail of the list. The integer
         * \em must be within the range assigned to the list, otherwise the
         * operation is undefined.\n
         * If the element was already in the list, nothing is done on the list.
         * \return Returns true only if the command executed.
         */
        inline bool push_tail(int id)
        {
            if ( id<myFirstId || id>myLastId ) {
                fprintf( stderr , "StaticList: ini=%d end=%d , attempt to push_tail %d\n" , myFirstId , myLastId , id );
                return false;
            }

            int pos = id-myFirstId;            
            if ( myNext[pos]!=BlankPos ) {
                /* element is already in list. Accept silently. */
//                 LOG_PRINT( verb.main , "PUSH_TAIL (id=%d)\n" , id );
//                 dump(logger_get_stream(verb.main));
                return true;                
            }
            
            if ( myTail!=BlankPos ) {
                myNext[myTail]  = pos;
                myPrev[pos]     = myTail;
            }
            else {
                myHead          = pos;
                myPrev[pos]     = pos;
            }
            myNext[pos]         = pos;
            myTail              = pos;
                
            ++mySize;
//             LOG_PRINT( verb.main , "PUSH_TAIL (id=%d)\n" , id );
//             dump(logger_get_stream(verb.main));
            return true;
        } /* push_tail() */
        
        
        
        /** \brief Remove an element from the list.
         * \details Tries to remove an element from the list. If the element
         *  is not on the list, the call is ignored.
         * If the element is outside the supported range, the call is also
         * ignored
         */
        inline void remove( int id )
        {
            if ( id<myFirstId || id>myLastId ) {
                fprintf( stderr , "StaticList: ini=%d end=%d , attempt to remove %d\n" , myFirstId , myLastId , id );
                return;
            }
            int pos = id-myFirstId;
            if ( myNext[pos]==BlankPos && myPrev[pos]==BlankPos ) {
                /* wasn't in the list at all */
//                 LOG_PRINT( verb.main , "REMOVE_ELEMENT (id=%d)\n" , id );
//                 dump(logger_get_stream(verb.main));
                return;
            }
            
            if ( pos!=myHead ) {
                myNext[myPrev[pos]] = myNext[pos];
            }
            else {
                myHead              = myNext[pos];
            }
            
            if ( pos!=myTail ) {
                myPrev[myNext[pos]] = myPrev[pos];
            }
            else {
                myTail              = myPrev[pos];
            }
            myNext[pos]             = BlankPos;
            myPrev[pos]             = BlankPos;
            --mySize;
//             LOG_PRINT( verb.main , "REMOVE_ELEMENT (id=%d)\n" , id );
//             dump(logger_get_stream(verb.main));
        } /* remove() */
        
        
        /** \brief Clear the list. After this call the list becomes empty
         */
        inline void clear()
        {
            for ( unsigned int i=0 ; i<myNelem ; ++i ) {
                myNext[i] = BlankPos;
                myPrev[i] = BlankPos;
            }
            mySize      = 0;
            myHead        = BlankPos;
            myTail        = BlankPos;
        }
}; /* class StaticList */


}; /* namespace Wcommon */

#endif /* ! defined( STATIC_LIST_H ) */

