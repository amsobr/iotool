#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <sstream>

#include <cbsl/keyval.hpp>

#include <Poco/Logger.h>
#include <Poco/Format.h>

#include <common/cmd_arguments.hpp>
#include <common/data_bucket.hpp>
#include <common/data_bucket_consumer.hpp>

#include "shell_frontend.hpp"
#include "shell_backend.hpp"

using namespace std;
using namespace Poco;

static void printBucket( StreamAdapter *s , DataBucket const &db )
{
    for ( auto entry : db.dataPoints ) {
        s->writeLine( Poco::format("%s=%s",entry.label(),entry.value()) );
    }
}

ShellFrontend::ShellFrontend( StreamAdapter *ioStream, ShellBackendPtr engine , DataBucketConsumer *bucketConsumer ) :
myStream(ioStream) ,
myEngine(engine) ,
myAccumulator( new AccumulatorBucket(bucketConsumer) )
{

}

ShellFrontend::~ShellFrontend()
{
    delete myAccumulator;
}


string usage()
{
    return 
    "Shell Usage: \n"
    "DEV CMD ARGS...     Execute command CMD on device DEV with arguments\n"
    "!SYSCMD ARGS...     Execute system command SYSCMD.\n"
    "                    All system commands start with '!'\n"
    "                    except 'help' , '?' and 'h'\n"
    ""
    "\n"
    "Some useful commands:\n"
    "help, h or ?        Show this help message, and a list of\n"
    "                    all available devices and commands.\n"
    "help GROUP          List all commands available for GROUP\n"
    "help GROUP CMD      Show detailed help for command CMD of\n"
    "                    group GROUP\n"
    "!usage              Show this help message\n"
    ;
}

void ShellFrontend::run()
{
    Logger &logger  = Logger::get("iotool");

    myStream->writeLine("Welcome to HW manager shell.");
    myStream->writeLine( usage() );

    DataBucket lastDataBucket;
    
    while( true ) {
        myStream->putc('>');
        string cmdLine;
        cmdLine = myStream->readLine();

        CmdArguments args;
        try {
            args.loadFrom(cmdLine);
        }
        catch ( std::exception ex ) {
            logger.error( "Error parsing command line:" + cmdLine );
            continue;
        }

        if ( args.size()==0 ) {
            continue;
        }

        Argument arg0 = args.shift();
        if ( !arg0.isToken() ) {
            myStream->writeLine("Invalid syntax.");
            continue;
        }
        string first  = arg0.token();
        logger.debug( format("First/Command is '%s' numTokens=%u",first,args.size()) );

        if ( first=="help" || first=="h" || first=="?" ) {
            logger.debug( "Checking help modes...");
            if ( first=="help" || first=="h" || first=="?" ) {
                /* Possibilities are:
                    help         --> list commands
                    help devType --> list commands for devType
                    help devType cmd --> detailed help for command of devType
                */
                if ( args.size()==2 ) {
                    string helpClass(args.shift().token());
                    string  helpCmd(args.shift().token());

                    logger.debug( format("Showing help for class=%s cmd=%s",helpClass,helpCmd));
                    myStream->writeLine(  myEngine->help(helpClass,helpCmd) );
                }
                else if ( args.size()==1 ) {
                    string helpClass(args.shift().token());
                    logger.debug( format("Showing help for class=%s",helpClass));
                    myStream->writeLine( myEngine->help(helpClass) );
                }
                else {
                    /* show full help, even if cmdLine is plain wrong... */
                    logger.debug( "Showing full help" );
                    myStream->writeLine( usage() );
                    myStream->writeLine( myEngine->help() );
                }
            }
        }
        else if ( first[0]=='!' ) {
            string cmdName  = first.substr(1);
            if ( cmdName=="usage" ) {
                myStream->writeLine( usage() );
            }
            else if ( cmdName=="bucket" ) {
                myAccumulator->command(args);
            }
            else {
                myStream->writeLine( cmdName + ": Command not found" );
            }
        }
        else if ( first=="exit" ) {
            myStream->writeLine("Leaving shell...");
            return;
            logger.information( "received 'exit'. Leaving shell event loop.");
        }
        else {
            string deviceId(first);
            if ( args.size()<1 ) {
                myStream->writeLine( "Invalid input.\n" + usage() );
            }
            else {
                string cmdName(args.shift().token());

                lastDataBucket.reset();
                Result res = myEngine->runDeviceApplet(deviceId,cmdName,args,lastDataBucket);
                if ( res.code()==0 ) {
                    logger.debug( format("Command %s result: OK (%d)",cmdName,res.code()) );
                }
                else {
                    logger.warning( format("Command %s gave error code=%d", cmdName,res.code()) );
                }            
                myStream->writeLine( res.message() );
                if ( !lastDataBucket.dataPoints.empty() ) {
                    printBucket(myStream,lastDataBucket);
                    myAccumulator->append(lastDataBucket);
                    lastDataBucket.dataPoints.clear();
                }
            }
        }
    }
}
