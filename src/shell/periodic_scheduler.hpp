//
// Created by to on 28/03/19.
//

#ifndef IOTOOL_PERIODIC_SCHEDULER_HPP
#define IOTOOL_PERIODIC_SCHEDULER_HPP

#include <string>
#include <vector>

#include <Poco/Util/Timer.h>


class PeriodicScheduler
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





};


#endif //IOTOOL_PERIODIC_SCHEDULER_HPP
