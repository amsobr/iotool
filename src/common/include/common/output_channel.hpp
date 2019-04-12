//
// Created by to on 20/03/19.
//

#ifndef IOTOOL_OUTPUT_CHANNEL_HPP
#define IOTOOL_OUTPUT_CHANNEL_HPP


#include "data_bucket_consumer.hpp"

class OutputChannel : public DataBucketConsumer
{
private:
    std::string myName;

public:
    OutputChannel(std::string const &name) : myName(name)
    {}

    virtual ~OutputChannel()
    {

    }

    const std::string &getName() const
    {
        return myName;
    }

    virtual void incomingBucket(DataBucket &db) = 0;
};

typedef std::shared_ptr<OutputChannel> OutputChannelPtr;


#endif //IOTOOL_OUTPUT_CHANNEL_HPP
