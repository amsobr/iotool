#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <sstream>

#include <cbsl/keyval.hpp>

#include "shell_frontend.hpp"
#include "cmd_arguments.hpp"
#include "shell_engine.hpp"

using namespace std;

ShellFrontend::ShellFrontend( std::istream &is , std::ostream &er , std::ostream &os, ShellEnginePtr engine ) :
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

void ShellFrontend::shellLoop()
{
    while( !myTerminate ) {
        cout << "ADS126xUTIL> " << std::flush;
        string cmdLine;
        getline(myInputStream,cmdLine);
        istringstream is(cmdLine);
        vector<string> tokens;
        string t;
        cout << "command is " << cmdLine << "\n";
        while ( getline(is, t,' ') ) {
            tokens.push_back(t);
        }
        cout << "tokenization complete...\n";

        if ( tokens.size()==0 ) {
            cerr << "No Input given...\n";
            continue;
        }

        string cmdName  = tokens[0];
        cout << "cmd name is " << cmdName << "\n";

        if ( cmdName=="help" ) {
            if ( tokens.size()==2 ) {
                myOut << myEngine->help(tokens[1]) << "\n";
            }
            else {
                myOut << myEngine->help() << "\n";
            }
        }
        else {
            CmdArguments args;
            for ( size_t i=1 ; i<tokens.size() ; i++ ) {
                cbsl::KeyVal::Pair pair;
                cbsl::KeyVal::parseArg(tokens[i].c_str(),&pair);
                args.addArg(pair);
            }
            CmdResult res = myEngine->runCommand(cmdName,args);
            if ( res.code()==0 ) {
                cout << res.message();
            }
            else {
                cerr << res.message();
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
