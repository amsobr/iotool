#include <iostream>

#include <shell_backend.hpp>

using namespace std;

ShellBackend::ShellBackend()
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
    cerr << "Rebuilding Index...\n";
    myPeripheralsByName.clear();
    myAppletsByType.clear();

    cerr << "Indexing peripherals...\n";
    for ( PeripheralPtr p : myPeripherals ) {
        string name = "" + p->getType() + to_string(p->getId());

        if ( myPeripheralsByName.find(name)!=myPeripheralsByName.end() ) {
            cerr << "OOPS peripheral '" << name << "' has already been indexed!\n";
        }
        else {
            cerr << "Indexed peripheral: '" << name << "'\n";
            myPeripheralsByName[name]   = p;
        }
    }

    cerr << "Indexing applets for each peripheral type...\n";
    for ( DeviceAppletPtr p : myDevApplets ) {
        auto it = myAppletsByType.find(p->getType());
        if ( it==myAppletsByType.end() ) {
            cerr << "Found applet group: '" << p->getType() << "'\n";
            list<DeviceAppletPtr> l;
            l.push_back(p);
            myAppletsByType[p->getType()]   = l;
            cerr << p->getName() << " added to group '" << p->getType() << "'\n";
        }
        else {
            list<DeviceAppletPtr> &applets(it->second);
            bool dupe = false;
            for ( AppletPtr app : applets ) {
                if ( app->getName()==p->getName() ) {
                    cerr << "Applet " << p->getName() << " is already registered for group '" << p->getType() << "'!\n";
                    dupe    = true;
                    break;
                }
            }
            if ( !dupe ) {
                applets.push_back(p);
                cerr << p->getName() << " added to group '" << p->getType() << "'\n";
            }
        }
    }
}

Result ShellBackend::runDeviceApplet( string const &devName , string const &cmdName , CmdArguments const &args )
{
    PeripheralPtr peripheral    = getPeripheral(devName);
    if ( peripheral==nullptr) {
        return Result(1,"Device not found: " + devName );
    }

    DeviceAppletPtr applet      = getDeviceApplet(peripheral->getType(),cmdName);
    if ( applet==nullptr ) {
        return Result(1,"Command " + cmdName + " undefined for " + peripheral->getType() + "peripherals\n ");
    }
    return applet->execute(args,peripheral);
}

Result ShellBackend::runSystemApplet( string const &cmdName , CmdArguments const &args )
{
    SystemAppletPtr applet  = getSystemApplet(cmdName);
    if ( applet==nullptr ) {
        return Result(1,"command not found:" + cmdName );
    }
    return applet->execute(args);
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

string ShellBackend::help( string const &devType , string const &cmdName ) const
{
    if ( devType=="" ) {
        string msg;
        for ( auto node : myAppletsByType ) {
            msg += generateFamilyHelp( ""+node.first , node.second );
        }
        msg += generateSystemHelp( mySysApplets );
        return msg;
    }
    else {       
        for ( auto node : myAppletsByType ) {
            if ( ""+node.first == devType ) {
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