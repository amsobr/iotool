#ifndef DATA_BUCKET_CONSUMER_HPP
#define DATA_BUCKET_CONSUMER_HPP

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
    virtual void incomingBucket(DataBucket &db) = 0;


}; /* DataBucketConsumer */



#endif /* DATA_BUCKET_CONSUMER_HPP */
