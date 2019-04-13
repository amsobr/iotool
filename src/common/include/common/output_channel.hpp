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

    std::string getName() const
    {
        return myName;
    }

    bool isCalled( std::string const &name ) const
    {
        return myName==name;
    }

    virtual void incomingBucket(DataBucketPtr db) = 0;
};

typedef std::shared_ptr<OutputChannel> OutputChannelPtr;


#endif //IOTOOL_OUTPUT_CHANNEL_HPP
