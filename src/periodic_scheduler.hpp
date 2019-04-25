//
// Created by to on 28/03/19.
//

#ifndef IOTOOL_PERIODIC_SCHEDULER_HPP
#define IOTOOL_PERIODIC_SCHEDULER_HPP

#include <string>
#include <vector>
#include <memory>

#include <Poco/Logger.h>
#include <Poco/Util/Timer.h>
#include <Poco/Util/TimerTask.h>
#include <Poco/JSON/Object.h>

#include <common/acquisition_scheduler.hpp>
#include <shell/shell_backend.hpp>


class PeriodicScheduler : public AcquisitionScheduler
{
private:
    Poco::Logger &logger;
    Poco::Util::Timer myTimer;
    Poco::Util::TimerTask::Ptr myTimerTask;

    std::vector<std::string> mySetupScript;
    std::vector<std::string> myExecutionScript;
    std::vector<std::string> myTeardownScript;
    ShellBackendPtr myShellBackend;

    unsigned long int myExecutionInterval; /*< given in nanoseconds */

    void onTimer(Poco::Util::TimerTask&);

public:
    typedef std::shared_ptr<PeriodicScheduler> Ptr;

    PeriodicScheduler(ShellBackendPtr shellBackend);
    virtual ~PeriodicScheduler();

    static PeriodicScheduler::Ptr loadFromJSON(Poco::JSON::Object::Ptr config, ShellBackendPtr shellBackend);

    void start() override;
    void stop() override;

    friend class PeriodicSchedulerTimerTask;

};


#endif //IOTOOL_PERIODIC_SCHEDULER_HPP
