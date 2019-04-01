
#include <string>

#include <Poco/Format.h>
#include <Poco/Logger.h>

#include <common/result.hpp>

#include "accumulator_bucket.hpp"


using namespace std;

Result AccumulatorBucket::flush()
{
    if ( myBucketConsumer!=nullptr ) {
        logger.debug( Poco::format("Flushing bucket (label=%s #dataPoints=%u) through %p" ,
                                    myAccumulator->name ,
                                    myAccumulator->dataPoints.size() ,
                                     myBucketConsumer)
                        );
        myBucketConsumer->incomingBucket(myAccumulator);
    }
    init();
    return Result::OK;
}

Result AccumulatorBucket::init( string const &name )
{
    logger.debug( Poco::format( "Init bucket. name='%s'",name) );
    // FIXME: instead of creating new buckets all over again. Implement Bucket::init(time,name)...
    myAccumulator.reset( new DataBucket(name) );
    return Result::OK;
}


void AccumulatorBucket::append( DataBucket const &db )
{
    myAccumulator->dataPoints.insert(
        myAccumulator->dataPoints.end() ,
        db.dataPoints.begin() ,
        db.dataPoints.end()
        );
}

vector<string> AccumulatorBucket::getPrefixes() const
{
    return vector<string>({"bucket"});
}

Result AccumulatorBucket::runCommand(std::string const &prefix, CmdArguments &args, DataBucket &)
{
    if ( prefix!="bucket") {
        logger.error(Poco::format("The impossible happened: bucket command called but prefix='%s'", prefix));
        return Result::E_NOT_SUPPORTED;
    }

    logger.trace( Poco::format("Accumulator num_args=%u",args.size()) );
    if ( args.size()<1 ) {
        return Result::E_BAD_ARGS;
    }

    Argument arg = args.shift();
    if ( !arg.isToken() ) {
        return Result::E_BAD_ARGS;
    }
    string cmdName(arg.token());

    if ( cmdName=="flush" ) {
        if ( args.size()!=0 ) {
            return Result::E_BAD_ARGS;
        }
        return flush();
    }
    else if ( cmdName=="init" ) {
        if ( args.size()<1 ) {
            return Result::E_BAD_ARGS;
        }
        Argument nameArg = args.shift();
        if ( !nameArg.isToken() ) {
            return Result::E_BAD_ARGS;
        }
        string name = nameArg.token();
        return init(name);
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
}
