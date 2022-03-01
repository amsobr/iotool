//
// Created by to on 20/02/22.
//

#include "CmdLogger.hpp"

#if 0
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







//
// Created by to on 02/05/19.
//

#include <Poco/Logger.h>

#include "logger_shell_provider.hpp"


using namespace Poco;
using namespace std;

static vector<string> levelNames = {
    "OFF" , // 0
    "fatal" ,
    "critical" ,
    "error" ,
    "warning" ,
    "notice" , // 5
    "information" ,
    "debug" ,
    "trace" // 8
};

static inline string getLevelName(int lv)
{
    if ( lv<0 || lv>=levelNames.size() ) {
        return "UNKNOWN";
    }
    return levelNames[lv];
}

LoggerShellProvider::LoggerShellProvider() :
ShellProvider({"logger"}) ,
myLogger( Logger::get("logger"))
{
    myCommandHelps.push_back( CommandHelpPtr( new CommandHelp("logger status","Get current state of all loggers")));
    myCommandHelps.push_back( CommandHelpPtr( new CommandHelp("logger status","Configure verbosity of loggers")));
}


Result LoggerShellProvider::runCommand(std::string const &prefix, CmdArguments &args, DataBucket &accumulator)
{
    if ( prefix!="logger" ) {
        myLogger.error(format("LoggerShellProvider: invalid prefix - '%s'",prefix));
        return Result::E_NOT_SUPPORTED;
    }
    string cmd;
    if ( !args.shiftToken(&cmd) ) {
        myLogger.error(format("LoggerShellProvider: invalid command - '%s'",args.shift().toString()));
        return Result::E_INVALID_SYNTAX;
    }

    if ( cmd=="status" ) {
        return runLoggerStatus(accumulator);
    }
    else if ( cmd=="config" ) {
        return runLoggerConfig(args, accumulator);
    }
    else {
        myLogger.error(format("LoggerShellProvider: unknown logger command - '%s'",cmd));
        return Result::E_NOT_SUPPORTED;
    }
}

std::string LoggerShellProvider::helpBrief()
{
    return
    "logger commands:\n"
    "    logger status  - get current status of loggers\n"
    "    logger config  - configure loggers\n"
    ;
}

std::string LoggerShellProvider::helpFamily(std::string const &prefix)
{
    return
        "logger commands:\n"
        "    logger status  - get current status of loggers\n"
        "    logger config  - configure loggers\n"
        ;
}

std::string LoggerShellProvider::helpCommand(std::string const &prefix, std::string const &cmd)
{
    return
        "logger commands:\n"
        "    logger status  - get current status of loggers\n"
        "    logger config  - configure loggers\n"
        ;
}

Result LoggerShellProvider::runLoggerStatus(DataBucket &bucket)
{
    /* Note to unresting minds...
     * This code is racy: new loggers may be brought to life after the call Logger::names()
     * This is just how the Logger API works and we can't do much about it.
     * FWIW, we can live with this and just call `logger status' in case we don't trust the command's output.
     * The result of this command is informational and as such we can tolerate this kind of weakness....
     */
    myLogger.trace("LoggerShellProvider: getting current logger statuses...");
    vector<string> loggerNames;
    Logger::names(loggerNames);
    for ( auto loggerName : loggerNames ) {
        Logger &logger(Logger::get(loggerName));
        if ( loggerName=="" ) {
            loggerName="ROOT";
        }
        bucket.addDataPoint(loggerName,getLevelName(logger.getLevel()));
    }
    return Result::OK;
}

Result LoggerShellProvider::runLoggerConfig(CmdArguments &args, DataBucket &bucket)
{
    myLogger.trace("LoggerShellProvider: configuring logger levels...");
    while ( ! args.empty() ) {
        Argument arg    = args.shift();
        if ( arg.isToken() ) {
            myLogger.warning(format("LoggerShellProvider: ignoring invalid argument - '%s'",arg.toString()));
            continue;
        }
        string n(arg.name());
        string l(arg.value());
        if ( n!="logger" ) {
            myLogger.notice(format("LoggerShellProvider: setting logger '%s' severity to '%s'", n, l));
            if ( n=="." || n=="ROOT" ) {
                n="";
            }
            Logger::setProperty(n, "level", l);
        }
        else {
            myLogger.warning("LoggerShellProvideR: ignoring configuration for 'logger' logger. Always set to 'information'");
        }
    }
    return Result::OK;
}

vector<CommandHelpPtr> LoggerShellProvider::getCommandHelp(vector<CommandHelpPtr> *helpVec)
{
    if ( helpVec!= nullptr ) {
        helpVec->insert(helpVec->end(),myCommandHelps.begin(),myCommandHelps.end());
        return *helpVec;
    }
    else {
        return myCommandHelps;
    }
}




#endif
