#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <sstream>

#include <Poco/Logger.h>
#include <Poco/Format.h>

#include <rps/ArgumentList.hpp>

#include "ParseLine.hpp"
#include "ShellFrontend.hpp"

using namespace std;
using namespace Poco;



constexpr auto greeter = "Welcome to HW manager shell.\n"
                              "Useful commands:\n"
                              "exit - leave the shell\n"
                              "help - shell help\n\n"
                              ;

void ShellFrontend::run()
{
    Logger &logger  = Logger::get("iotool");
    myCtx->stream->writeLine(greeter);

    while( true ) {
        myCtx->stream->putc('>');
        string cmdLine;
        try {
            if ( myCtx->stream->isEof() ) {
                std::cerr << "lost connection. exit shell.\n";
                return;
            }
            cmdLine = myCtx->stream->readLine();
        }
        catch ( std::exception const& ex ) {
            std::cerr << "Exception: " << ex.what() << "\n";
            exit(1);
        }
        if ( cmdLine.empty() ) {
            continue;
        }
        
        if ( cmdLine=="exit" ) {
            logger.information("Shell exiting now.");
            break;
        }

        auto [ cmdName , args ] = parseLine(cmdLine);
        if ( cmdName=="exit" ) {
            break;
        }
        else {
            rps::AbstractCommandConstPtr cmd = myCtx->findCommand(cmdName);
            if ( cmd!=nullptr ) {
                try {
                    cmd->run(myCtx,args);
                    auto diff   = myCtx->stack.getNewElements();
                    if ( !diff.empty() ) {
                        size_t cnt = diff.size()-1;
                        for ( auto const&o : diff ) {
                            myCtx->stream->writeLine( Poco::format("%02z: %f",cnt,o) );
                            cnt--;
                        }
                    
                    }
                }
                catch ( rps::Exception const& ex ) {
                    std::string msg{ Poco::format("error: type='%s' msg='%s'",ex.getType(),ex.getText()) };
                    logger.error(msg);
                    myCtx->stream->writeLine(msg);
                }
                catch ( std::exception const& ex) {
                    std::string msg{ Poco::format("error: msg='%s'",string{ex.what()}) };
                    logger.error(msg);
                    myCtx->stream->writeLine(msg);
                }
            }
            else {
                myCtx->stream->writeLine( "Command not found: " + cmdName );
            }
        }
    }
}

