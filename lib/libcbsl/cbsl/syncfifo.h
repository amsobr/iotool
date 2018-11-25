#ifndef SYNCFIFO_H
#define SYNCFIFO_H

#include <list>
#include <pti/mutex.h>

template< class T >
class SyncFifo {
	private:
		Pti::Mutex	*pMyMutex;
		std::list<T> myFifo;
		
	public:
		SyncFifo() {
			pMyMutex	= new Pti::Mutex;
		}
		
		~SyncFifo() {
			delete pMyMutex;
		}
		
		inline void enqueue(T const &data) {
			pMyMutex->lock();
			myFifo.push_tail( data );
			pMyMutex->unlock();
		}
		
		inline T dequeue() {
			T const data;
			pMyMutex->lock();
			data	= myFifo.head();
			myFifo.pop_head();
			pMyMutex->unlock();
			return data;
		}
		
		inline bool isEmpty() {
			bool empty;
			pMyMutex->lock();
			empty	= myFifo.empty();
			pMyMutex->unlock();
			return empty;
		}
		
		inline int size() {
			int n;
			pMyMutex->lock();
			n	= myFifo.size();
			pMyMutex->unlock();
			return n;
		}
}; /* class SyncFifo */


#endif /* SYNCFIFO_H */
