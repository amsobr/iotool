#ifndef ACCUMULATOR_BUCKET_HPP
#define ACCUMULATOR_BUCKET_HPP


#include <Poco/Logger.h>


#include <common/data_bucket.hpp>
#include <common/data_bucket_consumer.hpp>
#include <common/cmd_arguments.hpp>
#include <common/result.hpp>

class AccumulatorBucket
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
    Result reset(std::string label="");
    Result flush();
    Result init(CmdArguments &args);

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
}; /* AccumulatorBucket */


#endif /* !defined ACCUMULATOR_BUCKET_HPP */
