#ifndef DATA_BUCKET_CONSUMER_HPP
#define DATA_BUCKET_CONSUMER_HPP

#include <memory>

#include "data_bucket.hpp"

class DataBucketConsumer
{
public:
    virtual ~DataBucketConsumer() {}
    
    /**
     * @brief process one data bucket.
     * 
     * @param db The DataBucket to be processed
     */
    virtual void incomingBucket(DataBucketPtr db) = 0;


}; /* DataBucketConsumer */

typedef std::shared_ptr<DataBucketConsumer> DataBucketConsumerPtr;



#endif /* DATA_BUCKET_CONSUMER_HPP */
