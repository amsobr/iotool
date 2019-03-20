//
// Created by to on 20/03/19.
//

#ifndef IOTOOL_JOB_HPP
#define IOTOOL_JOB_HPP


#include <string>
#include <rpn-processor/script.hpp>
#include <common/data_bucket.hpp>

class Job
{
private:
    std::string myName;
    Rpn::ScriptPtr myScript;

    struct ValueMapping {
        std::string name;
        int stackPos;

        ValueMapping(std::string n, int p) : name(n) , stackPos(p) {}
    };

    std::vector<ValueMapping> myOutputMappings;

public:
    Job( std::string const &path );
    virtual ~Job() = default;

    DataBucketPtr processBucket( DataBucket input);

};

typedef std::shared_ptr<Job> JobPtr;


#endif //IOTOOL_JOB_HPP
