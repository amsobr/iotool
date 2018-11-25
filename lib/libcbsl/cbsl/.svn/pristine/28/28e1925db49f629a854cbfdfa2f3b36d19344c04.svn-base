#ifndef CALL_FIFO_H
#define CALL_FIFO_H

#include <pti/nocopy.h>
#include <sys/types.h>
#include <pti/logger.h>
#include <common/logctl.h>


#define FIFO_SIZE (1<<POW__)


/** This class implements a call FIFO that may be used by threaded objects
 * that execute methods for their clients.
 * This class avoids using memory allocations and relies on copy semantics.
 */
template<typename CALL_TYPE__,int POW__>
class CallFifo {   
private:
    DISABLE_COPY(CallFifo);
    
    unsigned int myInPos;
    unsigned int myOutPos;
    
    CALL_TYPE__    myEntry[FIFO_SIZE];

public:
    
    CallFifo() :
    myInPos(0)      ,
    myOutPos(0)     ,
    myEntry()
    {
    }
    
    ~CallFifo()
    {
    }
    

    inline size_t size() const {
        return (myInPos+FIFO_SIZE-myOutPos)&(FIFO_SIZE-1);
    }

    
    bool isEmpty() const {
        return size()==0;
    }
    
        
    /** enqueue a call in the FIFO. Returns true on success and false on
     * failure (FIFO full)
     */
    
    bool enqueue( CALL_TYPE__ const &call__ )
    {
        if ( ((myOutPos+FIFO_SIZE-myInPos)&(FIFO_SIZE-1))==1 ) {
            /* queue is full */
            return false;
        }
        myEntry[myInPos]    = call__;
        myInPos             = (myInPos+1)&(FIFO_SIZE-1);
        return true;
    }
    
    
    /** Get pointer to next element to retrieve but don't remove it from
     * the queue.
     * If the queue is empty, returns a 0 pointer.
     */
    CALL_TYPE__ const *getNext()
    {
        if ( isEmpty() ) {
            LOG_PRINT( verb.pon , "QUEUE IS EMPTY!!!\n" );
            return 0;
        }
        return &myEntry[myOutPos];
    }
    
    
    /** dequeue next call in the FIFO.
     * Returns a reference to the call, which may be used by a copy
     * constructor or assignment operator
     * \warning This method may not be called when the queue is empty. The
     * behaviour is undefined.
     */
    CALL_TYPE__ const &dequeue()
    {
        CALL_TYPE__& t  = myEntry[myOutPos];
        if ( isEmpty() ) {
            LOG_PRINT( verb.pon , "QUEUE IS EMPTY!!!\n" );
            /* by nulling the method ptr, we avoid the caller making anything
             * stupid and even more dangerous.
             */
            t.method    = 0;
        }
        else {
            myOutPos        = (myOutPos+1)&(FIFO_SIZE-1);
        }
        return t;
    }

    
}; /* class CallFifo */



#endif /* !defined(CALL_FIFO_H) */

