//
// Created by to on 20/04/19.
//

#ifndef IOTOOL_OUTPUT_CHANNEL_HOLDER_HPP
#define IOTOOL_OUTPUT_CHANNEL_HOLDER_HPP

#include <common/output_channel.hpp>

class OutputChannelHolder
{
private:
    OutputChannelPtr myChannel;
    std::string myChannelName;
    std::string myJobName;

public:
    OutputChannelHolder( std::string const &channelName, std::string const &jobName , OutputChannelPtr channel ) :
    myChannel(channel),
    myChannelName(channelName),
    myJobName(jobName)
    {}

    virtual ~OutputChannelHolder()
    {

    }

    OutputChannelHolder( OutputChannelHolder const &other ) :
    myChannel(other.myChannel) ,
    myChannelName(other.myChannelName) ,
    myJobName(other.myJobName)
    {

    }

    OutputChannelHolder operator=( OutputChannelHolder const &other )
    {
        myChannel       = other.myChannel;
        myChannelName   = other.myChannelName;
        myJobName       = other.myJobName;
        return *this;
    }


    const OutputChannelPtr channel() const
    {
        return myChannel;
    }

    const std::string channelName() const
    {
        return myChannelName;
    }

    const std::string jobName() const
    {
        return myJobName;
    }

    bool jobIsCalled( std::string const &name ) const
    {
        return myJobName==name;
    }
};


#endif //IOTOOL_OUTPUT_CHANNEL_HOLDER_HPP
