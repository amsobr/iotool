
#include <string>

#include <Poco/Format.h>
#include <Poco/Logger.h>

#include <common/result.hpp>

#include "accumulator_bucket.hpp"


using namespace std;
using Poco::format;

Result AccumulatorBucket::flushBucket(DataBucket &bucket)
{
    if ( myBucketConsumer!=nullptr ) {
        logger.debug( Poco::format("Flushing bucket (label=%s #dataPoints=%u) through %p" ,
                                    bucket.name ,
                                    bucket.dataPoints.size() ,
                                     myBucketConsumer)
                        );
        myBucketConsumer->incomingBucket( DataBucketPtr(new DataBucket(bucket)) );
    }
    init(bucket);
    return Result::OK;
}

Result AccumulatorBucket::init(DataBucket &bucket, const std::string &name)
{
    logger.debug( Poco::format( "Init bucket. name='%s'",name) );
    // FIXME: instead of creating new buckets all over again. Implement Bucket::init(time,name)...
    bucket.name = name;
    bucket.timestamp.update();
    bucket.dataPoints.clear();
    return Result::OK;
}




Result AccumulatorBucket::runCommand(std::string const &prefix, CmdArguments &args, DataBucket &accumulator)
{
    if ( prefix!="bucket") {
        logger.error(Poco::format("The impossible happened: bucket command called but prefix='%s'", prefix));
        return Result::E_NOT_SUPPORTED;
    }

    logger.trace( Poco::format("Accumulator num_args=%u",args.size()) );
    if ( args.size()<1 ) {
        return Result::E_BAD_ARGS;
    }

    string cmdName;
    if ( !args.shiftToken(&cmdName) ) {
        logger.error( format("bucket: invalid command - '%s'",args.shift().toString()) );
        return Result::E_INVALID_SYNTAX;
    }

    if ( cmdName=="flush" ) {
        if ( args.size()!=0 ) {
            return Result::E_BAD_ARGS;
        }
        return flushBucket(accumulator);
    }
    else if ( cmdName=="init" ) {
        string name;
        if ( !args.shiftToken(&name)) {
            logger.error( format("bucket init: invalid name - '%s'",args.shift().toString()) );
            return Result::E_INVALID_SYNTAX;
        }
        return init(accumulator,name);
    }
    else {
        return Result::E_BAD_ARGS;
    }
}

string AccumulatorBucket::helpBrief()
{
    return
    "bucket:\n"
    "  Manage the session data bucket accumulator.\n"
    ;
}

string AccumulatorBucket::helpFamily( string const &prefix)
{
    if ( prefix!="bucket" ) {
        return "AccumulatorBucker: INVALID REQUEST";
    }

    return
    "bucket - command interface data bucket management\n"
    "Available commands:\n"
    "  bucket init  - initialize bucket.\n"
    "  bucket flush - dispatch contents of bucket to consumers.\n"
    ;
}

string AccumulatorBucket::helpCommand( string const &prefix , string const &cmd)
{
    if ( prefix!="bucket" ) {
        return "AccumulatorBucker: INVALID REQUEST";
    }

    if ( cmd=="init" ) {
        return
        "bucket init - initialize the accumulator\n"
        "              The timestamp of the bucket is set to current time in the given mode.\n"
        "Usage:\n"
        "  bucket init [name=ARG]\n"
        "Arguments:\n"
        "  name      The name of the bucket. If none is provided, default is blank.\n"
        ;
    }
    else if ( cmd=="flush" ) {
        return
            "bucket flush - dispatch the accumulator to registered consumers\n"
            "               The bucket becomes empty after execution.\n"
            "Usage:\n"
            "  bucket flush\n"
            ;
    }
    else {
        return "bucket " + cmd + ": invalid command.\n";
    }
}
