#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <sstream>

#include <cbsl/keyval.hpp>

#include <Poco/Logger.h>
#include <Poco/Format.h>

#include <common/cmd_arguments.hpp>

#include "shell_frontend.hpp"
#include "shell_backend.hpp"

using namespace std;
using namespace Poco;

ShellFrontend::ShellFrontend(StreamAdapter *ioStream, ShellBackendPtr engine) :
myStream(ioStream) ,
myEngine(engine)
{

}

ShellFrontend::~ShellFrontend()
{
}


static string const greeter = "Welcome to HW manager shell.\n"
                              "Useful commands:\n"
                              "exit - leave the shell\n"
                              "help - shell help\n\n"
                              ;

void ShellFrontend::run()
{
    Logger &logger  = Logger::get("iotool");

    myStream->writeLine(greeter);

    DataBucket lastDataBucket;
    
    while( true ) {
        myStream->putc('>');
        string cmdLine;
        cmdLine = myStream->readLine();
        if ( cmdLine=="exit" ) {
            logger.information("Shell exiting now.");
            break;
        }

        DataBucket outcome;
        Result res  = myEngine->runCommand(cmdLine,&outcome);

        if ( res.isSuccess() ) {
            for ( auto entry : outcome.dataPoints ) {
                myStream->writeLine( format("%-15s : %s",entry.label(),entry.value()) );
            }
        }
        myStream->writeLine(res);
        myStream->writeLine("\n");
    }
}

