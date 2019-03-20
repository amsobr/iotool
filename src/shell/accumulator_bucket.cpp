
#include <string>

#include <Poco/Format.h>
#include <Poco/Logger.h>

#include <common/result.hpp>

#include "accumulator_bucket.hpp"


using namespace std;

Result AccumulatorBucket::reset( string name)
{
    logger.debug( Poco::format( "Resetting bucket. label=\"%s\"",name) );
    myAccumulator.dataPoints.clear();
    myAccumulator.timestamp.update();
    myAccumulator.name=name;
    return Result::OK;
}

Result AccumulatorBucket::flush()
{
    if ( myBucketConsumer!=nullptr ) {
        logger.debug( Poco::format("Flushing bucket (label=%s #dataPoints=%u) through %p" ,
                                    myAccumulator.name ,
                                    myAccumulator.dataPoints.size() ,
                                     myBucketConsumer)
                        );
        myBucketConsumer->processBucket(myAccumulator);
    }
    reset();
    return Result::OK;
}

Result AccumulatorBucket::init( CmdArguments &args )
{
    if ( args.size()!=1 ) return Result::E_BAD_ARGS;
    if ( !args.hasArg("name") ) return Result::E_BAD_ARGS;

    string label = args.getValue("name");
    reset(label);
    return Result::OK;
}


Result AccumulatorBucket::command( CmdArguments &args )
{
    logger.trace( Poco::format("Accumulator num_args=%u",args.size()) );
    if ( args.size()<1 ) return Result::E_BAD_ARGS;

    Argument arg = args.shift();
    if ( !arg.isToken() ) return Result::E_BAD_ARGS;
    string cmdName(arg.token());

    if ( cmdName=="reset" ) {
        if ( args.size()!=0 ) return Result::E_BAD_ARGS;
        return reset();
    }
    if ( cmdName=="flush" ) {
        if ( args.size()!=0 ) return Result::E_BAD_ARGS;
        return flush();
    }
    if ( cmdName=="init" ) {
        return init(args);
    }
    else {
        return Result::E_BAD_ARGS;
    }
}


void AccumulatorBucket::append( DataBucket const &db )
{
    myAccumulator.dataPoints.insert(
        myAccumulator.dataPoints.end() ,
        db.dataPoints.begin() ,
        db.dataPoints.end() );
}
