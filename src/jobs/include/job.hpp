//
// Created by to on 20/03/19.
//

#ifndef IOTOOL_JOB_HPP
#define IOTOOL_JOB_HPP


#include <string>
#include <memory>

#include <rpn-processor/rpn_lib.hpp>
#include <rpn-processor/script.hpp>
#include <common/data_bucket.hpp>


class Job
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
    Job(std::string const &path , Rpn::RpnLib &rpnLib );

    virtual ~Job() = default;

    DataBucketPtr processBucket( DataBucketPtr input);

    std::string name() const { return myName; }

    bool isCalled( std::string const &s ) const
    {
        return myName==s;
    }

};

typedef std::shared_ptr<Job> JobPtr;


#endif //IOTOOL_JOB_HPP
