#include <iostream>
#include <mutex>

#include <Poco/Logger.h>
#include <Poco/Format.h>

#include <shell_backend.hpp>
#include <shell_provider.hpp>
#include <common/string_utils.hpp>

using namespace std;
using namespace Poco;

ShellBackend::ShellBackend() :
myMutex() ,
logger( Logger::get("iotool"))
{


}

ShellBackend::~ShellBackend()
{

}


//std::map<std::string,PeripheralPtr> myPeripheralsByName;
//std::map<PeripheralType,std::list<AppletPtr>> myAppletsByType;

PeripheralPtr ShellBackend::getPeripheral( string name ) const
{
    auto it = myPeripheralsByName.find(name);
    if ( it==myPeripheralsByName.end() ) {
        return nullptr;
    }
    return it->second;
}



DeviceAppletPtr ShellBackend::getDeviceApplet( PeripheralType t , std::string cmdName ) const
{
    auto it = myAppletsByType.find(t);
    if ( it==myAppletsByType.end() ) {
        return nullptr;
    }

    std::list<DeviceAppletPtr> const &applets(it->second);
    for ( DeviceAppletPtr p : applets ) {
        if ( p->getName()==cmdName ) {
            return p;
        }
    }
    return nullptr;
}

/**
 * @brief 
 * 
 * @param name 
 * @return SystemAppletPtr 
 */
SystemAppletPtr ShellBackend::getSystemApplet( std::string name ) const
{
    for ( SystemAppletPtr p : mySysApplets ) {
        if ( p->getName()==name ) return p;
    }
    return nullptr;
}


void ShellBackend::rebuildIndex()
{
    lock_guard<recursive_mutex> locker(myMutex);

    Logger &logger  = Logger::get("iotool");

    logger.information("Rebuilding Index...");
    myPeripheralsByName.clear();
    myAppletsByType.clear();

    logger.information("Indexing peripherals...");
    for ( PeripheralPtr p : myPeripherals ) {
        string name = p->getName();

        if ( myPeripheralsByName.find(name)!=myPeripheralsByName.end() ) {
            logger.warning(format("OOPS: peripheral '%s' has already been indexed!",name));
        }
        else {
            logger.information( format("Indexed peripheral: '%s'",name) );
            myPeripheralsByName[name]   = p;
        }
    }

    logger.information("Indexing applets for each peripheral type...");
    for ( DeviceAppletPtr p : myDevApplets ) {
        auto it = myAppletsByType.find(p->getType());
        if ( it==myAppletsByType.end() ) {
            logger.information( format("Found applet group: '%s'",(string)p->getType()) );
            list<DeviceAppletPtr> l;
            l.push_back(p);
            myAppletsByType[p->getType()]   = l;
            logger.information( format("'%s' added to group '%s'",p->getName(),(string)p->getType()) );
        }
        else {
            list<DeviceAppletPtr> &applets(it->second);
            bool dupe = false;
            for ( AppletPtr app : applets ) {
                if ( app->getName()==p->getName() ) {
                    logger.warning( format("Applet '%s' is already registered for group '%s'",p->getName(),p->getType().toString()) );
                    dupe    = true;
                    break;
                }
            }
            if ( !dupe ) {
                applets.push_back(p);
                logger.information( format("'%s' added to group '%s'",p->getName(),p->getType()) );
            }
        }
    }
}

Result ShellBackend::runDeviceApplet( string const &devName , string const &cmdName , CmdArguments &args , DataBucket &db )
{
    lock_guard<recursive_mutex> locker(myMutex);

    PeripheralPtr peripheral    = getPeripheral(devName);
    if ( peripheral==nullptr) {
        return Result(1,"Device not found: " + devName );
    }

    DeviceAppletPtr applet      = getDeviceApplet(peripheral->getType(),cmdName);
    if ( applet==nullptr ) {
        return Result(1,"Command " + cmdName + " undefined for " + peripheral->getType() + "peripherals\n ");
    }
    return applet->execute(args,peripheral,db);
}

Result ShellBackend::runSystemApplet( string const &cmdName , CmdArguments &args , StreamAdapter &stream )
{
        lock_guard<recursive_mutex> locker(myMutex);

    SystemAppletPtr applet  = getSystemApplet(cmdName);
    if ( applet==nullptr ) {
        return Result(1,"command not found:" + cmdName );
    }
    return applet->execute(args,stream);
}


static string generateFamilyHelp( string const &family , list<DeviceAppletPtr> const &applets )
{
    string msg  = family + " peripheral commands:\n";
    for ( DeviceAppletPtr a : applets ) {
        char str[128];
        snprintf( str , sizeof(str) , "  %-12s%s\n" , a->getName().c_str() , a->brief().c_str() );
        msg += str;
    }
    msg += "\n";

    return msg;
}

static string generateSystemHelp( vector<SystemAppletPtr> const &applets )
{
    string msg = "SYSTEM commands:\n";
    for ( SystemAppletPtr a : applets ) {
        char str[128];
        snprintf( str , sizeof(str) , "  %-12s%s\n" , a->getName().c_str() , a->brief().c_str() );
        msg += str;
    }
    msg += "\n";

    return msg;
}

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

ShellProviderPtr ShellBackend::getProvider(std::string const &prefix)
{
    lock_guard<recursive_mutex> locker(myMutex);

    for ( auto provider : myProviders ) {
        if ( provider->provides(prefix) ) {
            return provider;
        }
    }
    return nullptr;
}

bool ShellBackend::addProvider(ShellProviderPtr provider)
{
    lock_guard<recursive_mutex> locker(myMutex);
    for ( auto prefix : provider->getPrefixes() ) {
        if ( getProvider(prefix)!=nullptr ) {
            return false;
        }
    }
    myProviders.push_back(provider);
    return true;
}

Result ShellBackend::runCommand(CmdArguments &args)
{
    Logger &logger  = Logger::get("iotool");
    logger.debug( "ShellBackend: parsing command..." );

    string prefix;
    if ( !args.shiftToken(&prefix) ) {
        logger.error( Poco::format("First argument '%s' is an invalid prefix", args.shift().toString() ) );
        return Result::E_NOT_SUPPORTED;
    }

    logger.debug( "ShellBackend: command prefix is '%s'",prefix));

    if ( prefix.find("help")==0 ) {
        return runHelp(prefix, args);
        // TODO: print output of HELP here!!!
    }

    /* <prefix> args
     * dispatch the call to the appropriate provider.
     */
    ShellProviderPtr provider   = getProvider(prefix);
    if ( provider==nullptr ) {
        logger.error( Poco::format("ShellBackend: no provider found for prefix '%s'",prefix) );
        return Result::E_NOT_SUPPORTED;
    }
    DataBucket outcome;
    Result res = provider->runCommand(prefix,args,outcome);






    Argument arg0 = args.shift();
    if ( !arg0.isToken() ) {
        myStream->writeLine("Invalid syntax.");
        continue;
    }
    string first  = arg0.token();
    logger.debug( format("First/Command is '%s' numTokens=%u",first,args.size()) );

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

Result ShellBackend::runHelp(std::string const &helpCmd, CmdArguments &args)
{
    Logger &logger  = Logger::get("iotool");

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

Result ShellBackend::simpleHelp(CmdArguments arguments)
{
    /* help
     * help <args>
     */
    return Result(0,"Not Implemented Yet...");
}

Result ShellBackend::helpPrefix(const std::string const &prefix, CmdArguments &arguments)
{
    /* help-<prefix>
     * help-<prefix> args
     */
    return Result(0,"Not Implemented yet...");
}
