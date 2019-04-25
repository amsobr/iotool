//
// Created by to on 20/03/19.
//

#include <string>
#include <vector>
#include <fstream>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Logger.h>
#include <Poco/Format.h>
#include <Poco/JSON/JSON.h>

#include <rpn-processor/script.hpp>
#include <rpn-processor/rpn_lib.hpp>


#include "transform_job.hpp"
#include "../periodic_scheduler.hpp"

using namespace Poco::JSON;
using Poco::Dynamic::Var;
using Poco::Logger;
using Poco::format;
using namespace std;
/**
 * \brief
 * \details Structure of the JSON file:
 * {
 *   "jobName" : "ARG" ,
 *     "script" : [
 *         "instr1" ,
 *         "instr2" ,
 *         "instr3"
 *     ] ,
 *     "outputs" : {
 *         "output_name1" : 0 ,
 *         "output_name2" : 1 ,
 *         "output_namex" : 2
 *     }
 * }
 *
 * \param path
 * \param rpnLib
 */
TransformJob::TransformJob(std::string const &path , Rpn::RpnLib &rpnLib ) :
myRpnLib(rpnLib)
{
    Logger &logger  = Logger::get("iotool");

    std::ifstream fileStream(path);

    logger.information(format("TransformJob: loading JSON tree from '%s'...",path));

    Parser parser;
    Var parsed = parser.parse(fileStream);

    Object::Ptr root    = parsed.extract<Object::Ptr>();
    myName              = root->getValue<std::string>("jobName");
    logger.information(format("TransformJob: found name='%s'",myName));

    logger.information(format("TransformJob(%s): loading script...",myName));
    Array::Ptr jScript  = root->getArray("script");
    vector<string> script;
    for ( auto it=jScript->begin() ; it!=jScript->end() ; it++ ) {
        script.push_back((*it).convert<string>());
    }
    logger.information(format("TransformJob(%s): compiling RPN script...",myName));
    myScript    = rpnLib.compile(script);
    if ( myScript==nullptr ) {
        logger.error(format("TransformJob(%s): RPN script compilation failed.",myName));
        throw std::runtime_error("Invalid job descriptor at " + path );
    }

    logger.information(format("TransformJob(%s): loading outputs mappings...",myName));
    Object::Ptr jOutputs   = root->getObject("outputs");
    for ( auto it=jOutputs->begin() ; it!=jOutputs->end() ; it++ ) {
        auto const &entry = *it;
        myOutputMappings.push_back( ValueMapping(entry.first,entry.second.convert<int>()) );
    }
    logger.information(format("TransformJob(%s): looking good. Job loaded.",myName));
}


DataBucketPtr TransformJob::processBucket( DataBucketPtr input)
{
    Rpn::ContextPtr ctx = myRpnLib.createContext();

    for ( auto const &point : input->dataPoints ) {
        ctx->assignConstant(point.label(),Poco::NumberParser::parseFloat(point.value()));
    }

    myScript->execute(ctx);

    DataBucket *bucket  = new DataBucket(input->name);
    bucket->timestamp   = input->timestamp;
    for ( auto const &out : myOutputMappings ) {
        double v        = ctx->valueAt(out.stackPos);
        bucket->addDataPoint(out.name,v);
    }
    return DataBucketPtr(bucket);
}






