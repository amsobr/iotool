//
// Created by to on 19/04/19.
//

#ifndef IOTOOL_IO_CORE_HPP
#define IOTOOL_IO_CORE_HPP


#include <Poco/Logger.h>

#include <common/acquisition_scheduler.hpp>

#include "shell/ShellFrontend.hpp"


class IoCore
{
private:
    Poco::Logger &logger;
    std::string myConfigDirPath;

public:
    explicit IoCore(std::string configDirPath);

    virtual ~IoCore();


    void shutdown();

    void start();
};


#endif //IOTOOL_IO_CORE_HPP
