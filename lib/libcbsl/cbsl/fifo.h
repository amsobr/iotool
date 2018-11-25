#ifndef FIFO_H
#define FIFO_H

#include <list>
#include <pti/mutex.h>

class DummyMutex {
public:
    DummyMutex()
    {
    }
    
    ~DummyMutex()
    {
    }
    
    void lock() {}
    
    void unlock() {}
};

namespace Pti {

template< class T , class MutexT=DummyMutex >
class Fifo {
	private:
		MutexT      *pMyMutex;
		std::list<T> myFifo;
        
        Fifo( Fifo const & );
        Fifo & operator=( Fifo const & );
		
	public:
		Fifo() :
		pMyMutex(new MutexT)    ,
		myFifo()
		{
		}
		
		~Fifo() {
			delete pMyMutex;
		}
		
		void enqueue(T const &data) {
			pMyMutex->lock();
			myFifo.push_back( data );
			pMyMutex->unlock();
		}
		
		
		/** \brief Pull the next element from the FIFO and return it.
         */
		T dequeue() {
			T data;
			pMyMutex->lock();
			data	= myFifo.front();
			myFifo.pop_front();
			pMyMutex->unlock();
			return data;
		}
		
		bool isEmpty() {
			bool empty;
			pMyMutex->lock();
			empty	= myFifo.empty();
			pMyMutex->unlock();
			return empty;
		}
		
		int size() {
			int n;
			pMyMutex->lock();
			n	= myFifo.size();
			pMyMutex->unlock();
			return n;
		}
		
		void clear() {
            pMyMutex->lock();
            myFifo.clear();
            pMyMutex->unlock();
        }
        
        void remove( T const &elem ) {
            pMyMutex->lock();
            typename std::list<T>::iterator it;
            for ( it=myFifo.begin() ; it!=myFifo.end() ; ++it ) {
                if ( (*it)==elem ) {
                    it  = myFifo.erase(it);
                }
            }
            pMyMutex->unlock();
        }
}; /* class Fifo */

}; /* namespace Pti */


#endif /* FIFO_H */
