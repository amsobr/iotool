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
