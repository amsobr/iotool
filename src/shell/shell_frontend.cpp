#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <sstream>

#include <cbsl/keyval.hpp>

#include <Poco/Logger.h>
#include <Poco/Format.h>

#include "shell_frontend.hpp"
#include <common/cmd_arguments.hpp>
#include "shell_backend.hpp"

using namespace std;
using namespace Poco;

ShellFrontend::ShellFrontend( std::istream &is , std::ostream &er , std::ostream &os, ShellBackendPtr engine ) :
myInputStream(is) ,
myErr(er) ,
myOut(os) ,
myTerminate(false) ,
myThread() ,
myEngine(engine)
{

}

ShellFrontend::~ShellFrontend()
{
    stop();
    myThread.join();
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

void ShellFrontend::shellLoop()
{
    Logger &logger  = Logger::get("iotool");

    cout << "Welcome to HW manager shell.\n";
    cout << usage();
    
    while( !myTerminate ) {
        cout << "\n> " << std::flush;
        string cmdLine;
        getline(myInputStream,cmdLine);
        istringstream is(cmdLine);
        vector<string> tokens;
        string t;
        logger.debug( format("Got input: '%s'. Tokenizing...",cmdLine) );
        while ( getline(is, t,' ') ) {
            tokens.push_back(t);
        }
        logger.debug( "tokenization complete..." );

        if ( tokens.size()==0 ) {
            cerr << "No Input given...\n";
            continue;
        }

        string first  = tokens[0];
        logger.debug( format("First/Command is '%s' numTokens=%u",first,tokens.size()) );

        if ( first=="help" || first=="h" || first=="?" ) {
            logger.debug( "Checking help modes...");
            if ( first=="help" ) {
                /* Possibilities are:
                    help         --> list commands
                    help devType --> list commands for devType
                    help devType cmd --> detailed help for command of devType
                */
                if ( tokens.size()>=3 ) {
                    logger.debug( format("Showing help for class=%s cmd=%s",tokens[1],tokens[2]));
                    myOut << myEngine->help(tokens[1] , tokens[2]) << "\n";
                }
                else if ( tokens.size()==2 ) {
                    logger.debug( format("Showing help for class=%s",tokens[1]));
                    myOut << myEngine->help(tokens[1]) << "\n";
                }
                else {
                    logger.debug( "Showing full help" );
                    myOut << usage();
                    myOut << myEngine->help() << "\n";
                }
            }
        }
        else if ( first[0]=='!' ) {
            string cmdName  = first.substr(1);
            if ( cmdName=="usage" ) {
                cout << usage();
            }
            else {
                cout << cmdName << ": Command not found\n";
            }

        }
        else {
            string deviceId(first);
            if ( tokens.size()<2 ) {
                cerr << "Invalid input.\n";
                cerr << usage();
            }
            else {
                string cmdName(tokens[1]);
                CmdArguments args;
                for ( size_t i=2 ; i<tokens.size() ; i++ ) {
                    cbsl::KeyVal::Pair pair;
                    cbsl::KeyVal::parseArg(tokens[i].c_str(),&pair);
                    args.addArg(pair);
                }

                /* handle special commands first... */
                if ( cmdName=="exit") {
                    logger.information( "received 'exit'. Leaving shell event loop.");
                    return;
                }

                Result res = myEngine->runDeviceApplet(deviceId,cmdName,args);
                if ( res.code()==0 ) {
                    cout << res.message();
                }
                else {
                    cerr << res.message();
                }            
            }
        }
    }
}

void ShellFrontend::start()
{
    myThread = thread( [=] {shellLoop();} );
}

void ShellFrontend::stop()
{
    myTerminate = true;
}
    
void ShellFrontend::join()
{
    myThread.join();
}
