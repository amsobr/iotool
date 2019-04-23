//
// Created by to on 20/03/19.
//

#ifndef IOTOOL_TRANSFORM_JOB_HPP
#define IOTOOL_TRANSFORM_JOB_HPP


#include <string>
#include <memory>

#include <Poco/JSON/Object.h>

#include <rpn-processor/rpn_lib.hpp>
#include <rpn-processor/script.hpp>
#include <common/data_bucket.hpp>


class TransformJob
{
private:
    std::string myName;
    Rpn::ScriptPtr myScript;
    Rpn::RpnLib myRpnLib;

    struct ValueMapping {
        std::string name;
        int stackPos;

        ValueMapping(std::string n, int p) : name(n) , stackPos(p) {}
    };

    std::vector<ValueMapping> myOutputMappings;

public:
    TransformJob(std::string const &path , Rpn::RpnLib &rpnLib );

    virtual ~TransformJob() = default;

    DataBucketPtr processBucket( DataBucketPtr input);

    std::string name() const { return myName; }

    bool isCalled( std::string const &s ) const
    {
        return myName==s;
    }

};

typedef std::shared_ptr<TransformJob> TransformJobPtr;


#endif //IOTOOL_TRANSFORM_JOB_HPP
