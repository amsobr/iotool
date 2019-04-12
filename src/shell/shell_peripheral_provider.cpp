#include <iostream>
#include <mutex>

#include <Poco/Logger.h>
#include <Poco/Format.h>

#include <shell_backend.hpp>
#include <shell_provider.hpp>
#include <common/string_utils.hpp>

#include "shell_peripheral_provider.hpp"

using namespace std;
using namespace Poco;

ShellPeripheralProvider::ShellPeripheralProvider() :
ShellProvider({"with"}) ,
myMutex() ,
logger( Logger::get("iotool"))
{


}

ShellPeripheralProvider::~ShellPeripheralProvider()
{

}


PeripheralPtr ShellPeripheralProvider::getPeripheral( string name ) const
{
    auto it = myPeripheralsByName.find(name);
    if ( it==myPeripheralsByName.end() ) {
        return nullptr;
    }
    return it->second;
}


DeviceAppletPtr ShellPeripheralProvider::getDeviceApplet( PeripheralType t , std::string cmdName ) const
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


void ShellPeripheralProvider::rebuildIndex()
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

Result ShellPeripheralProvider::runDeviceApplet( string const &devName , string const &cmdName , CmdArguments &args , DataBucket &db )
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


string ShellPeripheralProvider::help( string const &devType , string const &cmdName )
{
    lock_guard<recursive_mutex> locker(myMutex);

    Logger &logger  = Logger::get("iotool");

    if ( devType=="" ) {
        logger.information( "No devType given. Generating full help" );
        string msg;
        for ( auto node : myAppletsByType ) {
            msg += generateFamilyHelp( node.first.toString() , node.second );
        }
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


Result ShellPeripheralProvider::runCommand(std::string const &prefix, CmdArguments &args, DataBucket &accumulator)
{
    if ( prefix=="with" ) {
        string periphName;
        if ( ! args.shiftToken(&periphName) ) {
            logger.error( Poco::format("with: invalid peripheral name - '%s'" , args.shift().toString()));
            return Result::E_INVALID_SYNTAX;
        }
        
        string cmdName;
        if ( !args.shiftToken(&cmdName) ) {
            logger.error( Poco::format("with <peripheral>: invalid command name - '%s'" , args.shift().toString()));
            return Result::E_INVALID_SYNTAX;
        }
        
        return runDeviceApplet(periphName,cmdName,args,accumulator);
    }
    else {
        logger.error( Poco::format("ShellPeripheralProvider: The impossible happened - called with invalid prefix: '%s'",prefix));
        return Result::E_NOT_SUPPORTED;
    }
}

string ShellPeripheralProvider::helpBrief()
{
    return "with <peripheral> <cmd> args";
}

string ShellPeripheralProvider::helpFamily(std::string const &prefix)
{
    

}

string ShellPeripheralProvider::helpCommand(std::string const &prefix, std::string const &cmd)
{
    
}
