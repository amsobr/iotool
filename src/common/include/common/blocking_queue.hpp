//
// Created by to on 20/03/19.
//

#ifndef IOTOOL_BLOCKING_QUEUE_HPP
#define IOTOOL_BLOCKING_QUEUE_HPP

#include <list>
#include <mutex>
#include <condition_variable>

template<typename T>
class BlockingQueue
{
private:
    std::list<T> myQueue;
    std::mutex myMutex;
    bool myConsumerIsBlocked;
    bool myCancelled;
    std::condition_variable myCondition;

public:
    BlockingQueue() :
        myQueue(),
        myMutex(),
        myConsumerIsBlocked(false),
        myCancelled(false) ,
        myCondition()
    {}

    virtual ~BlockingQueue() {}

    void shutdown()
    {
        std::lock_guard<std::mutex> lock(myMutex);
        myCancelled = true;
        if ( myConsumerIsBlocked ) {
            myConsumerIsBlocked = false;
            myCondition.notify_all();
        }
    }

    void enqueue( T elem )
    {
        std::lock_guard<std::mutex> lock(myMutex);
        myQueue.push_back(elem);
        if ( myConsumerIsBlocked ) {
            myCondition.notify_one();
            myConsumerIsBlocked = false;
        }
    }

    T getNext()
    {
        std::unique_lock<std::mutex> lock(myMutex);
        if ( myQueue.empty() ) {
            myConsumerIsBlocked = true;
            while ( myConsumerIsBlocked ) {
                myCondition.wait(lock);
            }
        }
        if ( myCancelled ) {
            throw std::runtime_error("Interrupted: blocking queue was cancelled");
        }
        T ret(myQueue.front());
        myQueue.pop_front();
        return ret;
    }


};


#endif //IOTOOL_BLOCKING_QUEUE_HPP
