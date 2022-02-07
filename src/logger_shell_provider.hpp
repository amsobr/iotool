//
// Created by to on 02/05/19.
//

#ifndef IOTOOL_LOGGER_SHELL_PROVIDER_HPP
#define IOTOOL_LOGGER_SHELL_PROVIDER_HPP

#include <Poco/Logger.h>

#include <shell/shell_provider.hpp>
#include <shell/command_help.hpp>

class LoggerShellProvider : public ShellProvider
{
private:
    Poco::Logger &myLogger;

    std::vector<CommandHelpPtr> myCommandHelps;

    Result runLoggerStatus(DataBucket &bucket);

    Result runLoggerConfig(CmdArguments &args, DataBucket &bucket);

public:
    LoggerShellProvider();

    std::vector<CommandHelpPtr> getCommandHelp(std::vector<CommandHelpPtr> *helpVec) override;

    ~LoggerShellProvider() = default;

    Result runCommand(std::string const &prefix, CmdArguments &args, DataBucket &accumulator) override;

    std::string helpBrief() override;

    std::string helpFamily(std::string const &prefix) override;

    std::string helpCommand(std::string const &prefix, std::string const &cmd) override;

};


#endif //IOTOOL_LOGGER_SHELL_PROVIDER_HPP
