//
// Created by to on 02/05/19.
//

#ifndef IOTOOL_LOGGER_SHELL_PROVIDER_HPP
#define IOTOOL_LOGGER_SHELL_PROVIDER_HPP

#include <Poco/Logger.h>

#include <shell/shell_provider.hpp>

class LoggerShellProvider : public ShellProvider
{
private:
    Poco::Logger &myLogger;

    Result runLoggerStatus(DataBucket &bucket);

    Result runLoggerConfig(CmdArguments &args, DataBucket &bucket);

public:
    LoggerShellProvider();

    ~LoggerShellProvider() = default;

//    std::vector<std::string> getPrefixes() const override;

//    bool provides(std::string const &prefix) const override;

    Result runCommand(std::string const &prefix, CmdArguments &args, DataBucket &accumulator) override;

    std::string helpBrief() override;

    std::string helpFamily(std::string const &prefix) override;

    std::string helpCommand(std::string const &prefix, std::string const &cmd) override;

};


#endif //IOTOOL_LOGGER_SHELL_PROVIDER_HPP
