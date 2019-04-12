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
    Result flushBucket(DataBucket &bucket);
    Result init(DataBucket &bucket, const std::string &name = "");

    AccumulatorBucket() = delete;
public:
    AccumulatorBucket(DataBucketConsumer *consumer ) :
    ShellProvider( {"bucket"} ) ,
    myBucketConsumer(consumer) ,
    logger( Poco::Logger::get("accumulator") )
    {
    }

    ~AccumulatorBucket()
    {
    }

    Result command( CmdArguments &args );

    void append( DataBucket const &db );

    Result runCommand(std::string const &prefix, CmdArguments &args, DataBucket &accumulator) override;

    std::string helpBrief() override;

    std::string helpFamily(std::string const &prefix) override;

    std::string helpCommand(std::string const &prefix , std::string const &cmd ) override;
}; /* AccumulatorBucket */


#endif /* !defined ACCUMULATOR_BUCKET_HPP */
