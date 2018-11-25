#if !defined(STATIC_FIFO_HPP)
#define STATIC_FIFO_HPP

#include <stddef.h>

#include "nocopy.hpp"


namespace cbsl {

template< typename T , size_t FIFO_LEN >
class StaticFifo {
private:
    T       myData[FIFO_LEN+1];
    
    unsigned int myInPos;
    unsigned int myOutPos;
    
    WC_DISABLE_COPY(StaticFifo);
    
public: 
    
    StaticFifo() :
    myData()        ,
    myInPos(0)      ,
    myOutPos(0)     
    {
    }
    
    ~StaticFifo()
    {
    }
    
    /** \brief Check if the FIFO is empty.
     */
    inline bool empty() const {
        return myInPos==myOutPos;
    }
    
    /** \brief Check if the FIFO is full */
    inline bool full() const {
        return (myOutPos-myInPos==1) || (myInPos-myOutPos==FIFO_LEN) ? true : false;
    }
    
    /** \brief Check the current size of the FIFO's queue. */
    size_t size() const {
        return (myInPos+FIFO_LEN+1-myOutPos)%(FIFO_LEN+1);
    }
    
    /** \brief Enqueue an entry in the FIFO.
     * \return Returns true on success or false on error.
     * The call will fail if there is no room in the FIFO to store the new
     * element.
     */
    bool enqueue( T const &entry ) {
        if ( full() ) {
            return false;
        }
        
        myData[myInPos]     = entry;
        myInPos             = (myInPos+1)%(FIFO_LEN+1);
        return true;
    }
    
    
    /** \brief Retrieve the next element in the FIFO
     * \return Returns true on success, otherwise returns false
     */
    bool dequeue( T *entry ) {
        if ( empty() ) {
            return false;
        }
        
        *entry      = myData[myOutPos];
        myOutPos    = (myOutPos+1)%(FIFO_LEN+1);
        return true;
    }
}; /* class StaticFifo */

}; /* namespace Wcommon */

#endif /* ! defined( STATIC_FIFO_HPP) */

    
