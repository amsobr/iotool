#ifndef ACCUMULATOR_BUCKET_HPP
#define ACCUMULATOR_BUCKET_HPP


#include <Poco/Logger.h>


#include <common/data_bucket.hpp>
#include <common/data_bucket_consumer.hpp>
#include <common/cmd_arguments.hpp>
#include <common/result.hpp>
#include "shell_provider.hpp"

class AccumulatorBucket : public ShellProvider
{
private:
    DataBucketPtr myAccumulator;
    DataBucketConsumer *myBucketConsumer;
    Poco::Logger &logger;

    /**
     * @brief Reset bucket, optionally provide a new label.
     * 
     * The bucket is initialized, and the timestamp is set to NOW.
     * Use the new optional label.
     * 
     * @param label New label to use (optional). Default is empty.
     * @return Result::OK
     */
    Result flush();
    Result init(std::string const &name = "" );

    AccumulatorBucket() = delete;
public:
    AccumulatorBucket(DataBucketConsumer *consumer ) :
    myAccumulator( new DataBucket() ) ,
    myBucketConsumer(consumer) ,
    logger( Poco::Logger::get("accumulator") )
    {
    }

    ~AccumulatorBucket()
    {
    }

    Result command( CmdArguments &args );

    void append( DataBucket const &db );

    std::vector<std::string> getPrefixes() const override;

    Result runCommand(std::string const &prefix, CmdArguments &args, DataBucket &) override;

    std::string helpBrief() override;

    std::string helpFamily(std::string const &prefix) override;

    std::string helpCommand(std::string const &prefix , std::string const &cmd ) override;
}; /* AccumulatorBucket */


#endif /* !defined ACCUMULATOR_BUCKET_HPP */
