//
// Created by to on 20/03/19.
//

#ifndef IOTOOL_OUTPUT_CHANNEL_HPP
#define IOTOOL_OUTPUT_CHANNEL_HPP


#include "data_bucket_consumer.hpp"

class OutputChannel : public DataBucketConsumer
{

public:
    OutputChannel()
    {}

    virtual ~OutputChannel()
    {
    }

    virtual void incomingBucket(DataBucketPtr db) = 0;
};

typedef std::shared_ptr<OutputChannel> OutputChannelPtr;


#endif //IOTOOL_OUTPUT_CHANNEL_HPP
