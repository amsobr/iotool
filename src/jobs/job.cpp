//
// Created by to on 20/03/19.
//

#include <string>
#include <vector>
#include <fstream>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <rpn-processor/script.hpp>
#include <rpn-processor/rpn_lib.hpp>


#include "job.hpp"

using namespace Poco::JSON;
using Poco::Dynamic::Var;
using std::string;
using std::vector;

Job::Job(std::string const &path , Rpn::RpnLib &rpnLib ) :
myRpnLib(rpnLib)
{
    std::ifstream fileStream(path);

    Parser parser;
    Var parsed = parser.parse(fileStream);


    Object::Ptr root    = parsed.extract<Object::Ptr>();
    myName              = root->getValue<std::string>("jobName");
    Array::Ptr jScript  = root->getArray("script");

    vector<string> script;

    for ( auto it=jScript->begin() ; it!=jScript->end() ; it++ ) {
        script.push_back((*it).convert<string>());
    }
    myScript    = rpnLib.compile(script);
    if ( myScript==nullptr ) {
        throw std::runtime_error("Invalid job descriptor at " + path );
    }

    Object::Ptr jOutputs   = root->getObject("outputs");
    for ( auto it=jOutputs->begin() ; it!=jOutputs->end() ; it++ ) {
        auto const &entry = *it;
        myOutputMappings.push_back( ValueMapping(entry.first,entry.second.convert<int>()) );
    }
}


DataBucketPtr Job::processBucket( DataBucketPtr input)
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




