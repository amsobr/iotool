//
// Created by to on 19/04/19.
//

#ifndef IOTOOL_ACQUISITION_SCHEDULER_FACTORY_HPP
#define IOTOOL_ACQUISITION_SCHEDULER_FACTORY_HPP


#include <vector>
#include <shell/shell_backend.hpp>
#include "common/acquisition_scheduler.hpp"

class AcquisitionSchedulerFactory
{
public:
    static std::vector<AcquisitionScheduler::Ptr> loadFromPath(std::string const &path,ShellBackendPtr shellBackend);


};


#endif //IOTOOL_ACQUISITION_SCHEDULER_FACTORY_HPP
