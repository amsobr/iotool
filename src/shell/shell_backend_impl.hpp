#ifndef SHELL_BACKEND_IMPL_HPP
#define SHELL_BACKEND_IMPL_HPP

#include <iostream>
#include <mutex>

#include <Poco/Logger.h>
#include <Poco/Format.h>

#include <shell_backend.hpp>
#include <shell_provider.hpp>
#include <common/string_utils.hpp>
#include <shell_backend.hpp>

#include "shell_backend_core.hpp"

using namespace std;
using namespace Poco;

class ShellBackendImpl
{
private:
    std::recursive_mutex myMutex;

    Poco::Logger        &logger;
    DataBucket          myAccumulator;
    ShellBackendCore   *myCore;
    std::vector<DataBucketConsumer*> myDataConsumers;

public:

    ShellBackendImpl(ShellBackendCore *core) :
        myMutex() ,
        logger(Logger::get("iotool")) ,
        myAccumulator() ,
        myCore(core)
    {
    }

    ~ShellBackendImpl()
    {
    }

    void flushBucket()
    {
        logger.information( Poco::format("Flushing bucket (label=%s #dataPoints=%z)",myAccumulator.name ,myAccumulator.dataPoints.size()) );

        for ( auto consumer : myDataConsumers ) {
            /* each consumer nreceives a fresh copy of the bucket to ensure hassle-free threading support.
             */
            consumer->incomingBucket( DataBucketPtr(new DataBucket(myAccumulator)) );
        }
        initBucket("");
    }

    void initBucket(const std::string &name)
    {
        logger.debug( Poco::format( "Init bucket. name='%s'",name) );
        myAccumulator.name      = name;
        myAccumulator.timestamp.update();
        myAccumulator.dataPoints.clear();
    }


    Result bucketCommand(CmdArguments &args)
    {
        logger.information( Poco::format("Accumulator num_args=%z",args.size()) );
        if ( args.size()<1 ) {
            return Result::E_BAD_ARGS;
        }

        string cmdName;
        if ( !args.shiftToken(&cmdName) ) {
            logger.error( format("bucket: invalid command - '%s'",args.shift().toString()) );
            return Result::E_INVALID_SYNTAX;
        }

        if ( cmdName=="flush" ) {
            if ( args.size()!=0 ) {
                return Result::E_BAD_ARGS;
            }
            flushBucket();
            return Result::OK;
        }
        else if ( cmdName=="init" ) {
            if ( args.size()!=1 ) {
                logger.error(format("bucket init: invalid number of arguments. have %z, expected 1",args.size()));
                return Result::E_BAD_ARGS;
            }
            Argument arg( args.shift() );
            if ( arg.isToken() ) {
                logger.error("bucket init: invalid argument type. needed name=ARG");
                return Result::E_INVALID_SYNTAX;
            }
            if ( arg.name()!="name" ) {
                logger.error("bucket init: invalid argument name. needed name=ARG");
                return Result::E_BAD_ARGS;
            }
            string name = arg.value();
            initBucket(name);
            return Result::OK;
        }
        else {
            logger.error(format("ShellBackend: unrecognized bucket command - '%s'",cmdName));
            return Result::E_BAD_ARGS;
        }
    }

/*

string ShellBackend::bucketHelp( string const &prefix , string const &cmd)
{
    if ( prefix!="bucket" ) {
        return "AccumulatorBucker: INVALID REQUEST";
    }

    if ( cmd=="init" ) {
        return
            "bucket init - initialize the accumulator\n"
            "              The timestamp of the bucket is set to current time in the given mode.\n"
            "Usage:\n"
            "  bucket init [name=ARG]\n"
            "Arguments:\n"
            "  name      The name of the bucket. If none is provided, default is blank.\n"
            ;
    }
    else if ( cmd=="flush" ) {
        return
            "bucket flush - dispatch the accumulator to registered consumers\n"
            "               The bucket becomes empty after execution.\n"
            "Usage:\n"
            "  bucket flush\n"
            ;
    }
    else {
        return "bucket " + cmd + ": invalid command.\n";
    }
}
 */

#if 0
    string ShellBackend::help( string const &devType , string const &cmdName )
{
    lock_guard<recursive_mutex> locker(myMutex);

    Logger &logger  = Logger::get("iotool");

    if ( devType=="" ) {
        logger.information( "No devType given. Generating full help" );
        string msg;
        for ( auto node : myAppletsByType ) {
            msg += generateFamilyHelp( node.first.toString() , node.second );
        }
        msg += generateSystemHelp( mySysApplets );
        return msg;
    }
    else {
        for ( auto node : myAppletsByType ) {
            if ( node.first.toString() == devType ) {
                if ( cmdName=="" ) {
                    return generateFamilyHelp(devType,node.second);
                }
                else {
                    for ( AppletPtr a : node.second ) {
                        if ( a->getName()==cmdName ) {
                            return a->help();
                        }
                    }
                    return "Command " + cmdName + "not found in group " + devType;
                }
            }
        }
        return "Unknown command group: " + devType;
    }
}
#endif

    Result runCommand(string const &cmdLine, DataBucket *outcome)
    {
        Logger &logger  = Logger::get("iotool");
        logger.debug( "ShellBackend: parsing command..." );

        CmdArguments args;
        try {
            args.loadFrom(cmdLine);
        }
        catch (std::exception ex) {
            logger.error("Error parsing command line:" + cmdLine);
            return Result::E_INVALID_SYNTAX;
        }

        if (args.size() == 0) {
            return Result::OK;
        }

        string prefix;
        if ( !args.shiftToken(&prefix) ) {
            logger.error( Poco::format("First argument '%s' is an invalid prefix", args.shift().toString() ) );
            return Result::E_NOT_SUPPORTED;
        }

        logger.debug( format("ShellBackend: command prefix is '%s'",prefix));

        if ( prefix=="bucket" ) {
            logger.information("ShellBackend: handling bucket command...");
            return bucketCommand(args);
        }
        else if ( prefix.find("help")==0 ) {
            return runHelp(prefix, args);
            // TODO: print output of HELP here!!!
        }
        else {
            /* <prefix> args
             * dispatch the call to the appropriate provider.
             */
            ShellProviderPtr provider = myCore->getProvider(prefix);
            if (provider == nullptr) {
                logger.error(Poco::format("ShellBackend: no provider found for prefix '%s'", prefix));
                return Result::E_NOT_SUPPORTED;
            }

            logger.information("ShellBackend: delegating command execution to provider...");
            DataBucket cmdOutcome;
            Result res = provider->runCommand(prefix, args, cmdOutcome);
            if ( res.isSuccess() ) {
                myAccumulator.dataPoints.insert(
                    myAccumulator.dataPoints.end(),
                    cmdOutcome.dataPoints.begin(),
                    cmdOutcome.dataPoints.end()
                    );
                if ( outcome!= nullptr ) {
                    outcome->dataPoints = cmdOutcome.dataPoints;
                }
            }
            return res;
        }
    }

    Result runHelp(std::string const &helpCmd, CmdArguments &args)
    {
        /* Possibilities are:
         * help                --> brief help, list prefixes, short list of commands and help options
         * help <variant>      --> other kinds of help, shell-backend defined
         * help-<prefix>       --> list commands and short description for each command and additional help options
         * help-<prefix> @args --> delegate help into provider
         */
        logger.debug( "Checking help modes...");

        if ( helpCmd=="help") {
            // help
            // help <args>
            return simpleHelp(args);
        }

        // accept only in the form of:
        // help-<prefix> <args>
        vector<string> tokens;
        size_t cnt = StringUtils::split(helpCmd, "-", tokens, false);
        if (cnt != 2) {
            return Result::E_NOT_SUPPORTED;
        }
        return helpPrefix(tokens[1], args);
    }

    Result simpleHelp(CmdArguments arguments)
    {
        /* help
         * help <args>
         */
        return Result(0,"Not Implemented Yet...");
    }

    Result helpPrefix(std::string const &prefix, CmdArguments &arguments)
    {
        /* help-<prefix>
         * help-<prefix> args
         */
        return Result(0,"Not Implemented yet...");
    }

    void addConsumer(DataBucketConsumer *consumer)
    {
        myDataConsumers.push_back(consumer);

    }
}; /* class ShellBackend::Impl */

#endif
