//
// Created by to on 28/03/19.
//

#ifndef IOTOOL_PERIODIC_SCHEDULER_HPP
#define IOTOOL_PERIODIC_SCHEDULER_HPP

#include <string>
#include <vector>
#include <memory>

#include <Poco/Util/Timer.h>
#include <Poco/JSON/Object.h>

#include <common/acquisition_scheduler.hpp>


class PeriodicScheduler : public AcquisitionScheduler
{
private:
    Poco::Util::Timer myTimer;

    std::vector<std::string> mySetupScript;
    std::vector<std::string> myExecutionScript;
    std::vector<std::string> myTeardownScript;

    unsigned long int myExecutionInterval; /*< given in nanoseconds */


public:
    PeriodicScheduler();
    virtual ~PeriodicScheduler();

    typedef std::shared_ptr<PeriodicScheduler> Ptr;

    static Ptr loadFromJSON(Poco::JSON::Object::Ptr config);

};


#endif //IOTOOL_PERIODIC_SCHEDULER_HPP
