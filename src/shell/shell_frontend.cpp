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

#include "shell_frontend.hpp"
#include "shell_backend.hpp"

using namespace std;
using namespace Poco;

static void printBucket( StreamAdapter *s , DataBucket const &db )
{
    for ( auto entry : db.dataPoints ) {
        s->writeLine( Poco::format("%s=%s",entry.label,entry.value) );
    }
}

ShellFrontend::ShellFrontend( StreamAdapter *ioStream, ShellBackendPtr engine ) :
myStream(ioStream) ,
myEngine(engine)
{

}

ShellFrontend::~ShellFrontend()
{
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

        istringstream is(cmdLine);
        vector<string> tokens;
        string t;
        logger.debug( format("Got input: '%s'. Tokenizing...",cmdLine) );
        while ( getline(is, t,' ') ) {
            tokens.push_back(t);
        }
        logger.debug( "tokenization complete..." );

        if ( tokens.size()==0 ) {
            continue;
        }

        string first  = tokens[0];
        logger.debug( format("First/Command is '%s' numTokens=%u",first,tokens.size()) );

        if ( first=="help" || first=="h" || first=="?" ) {
            logger.debug( "Checking help modes...");
            if ( first=="help" || first=="h" || first=="?" ) {
                /* Possibilities are:
                    help         --> list commands
                    help devType --> list commands for devType
                    help devType cmd --> detailed help for command of devType
                */
                if ( tokens.size()>=3 ) {
                    logger.debug( format("Showing help for class=%s cmd=%s",tokens[1],tokens[2]));
                    myStream->writeLine(  myEngine->help(tokens[1],tokens[2]) );
                }
                else if ( tokens.size()==2 ) {
                    logger.debug( format("Showing help for class=%s",tokens[1]));
                    myStream->writeLine( myEngine->help(tokens[1]) );
                }
                else {
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
            if ( tokens.size()<2 ) {
                myStream->writeLine( "Invalid input.\n" + usage() );
            }
            else {
                string cmdName(tokens[1]);
                CmdArguments args;
                for ( size_t i=2 ; i<tokens.size() ; i++ ) {
                    cbsl::KeyVal::Pair pair;
                    cbsl::KeyVal::parseArg(tokens[i].c_str(),&pair);
                    args.addArg(pair);
                }

                lastDataBucket.reset();
                Result res = myEngine->runDeviceApplet(deviceId,cmdName,args,lastDataBucket);
                if ( res.code()==0 ) {
                    logger.debug( format("Command %s result: OK (%d)",cmdName,res.code()) );
                }
                else {
                    logger.warning( format("Command %s gave error code=%d", cmdName,res.code()) );
                }
                myStream->writeLine( res.message() );
                printBucket(myStream,lastDataBucket);
            }
        }
    }
}
