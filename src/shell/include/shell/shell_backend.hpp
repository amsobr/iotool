#ifndef SHELL_BACKEND_HPP
#define SHELL_BACKEND_HPP

#include <list>
#include <string>
#include <memory>
#include <map>
#include <mutex>

#include <common/system_applet.hpp>
#include <common/device_applet.hpp>
#include <common/cmd_arguments.hpp>
#include <common/result.hpp>
#include <common/peripheral.hpp>
#include "shell_provider.hpp"


class ShellBackend {
private:
    std::recursive_mutex myMutex;

    std::vector<ShellProviderPtr> myProviders;

    std::vector<PeripheralPtr>    myPeripherals;
    std::vector<DeviceAppletPtr>  myDevApplets;
    std::vector<SystemAppletPtr>  mySysApplets;

    /* chache */
    std::map<std::string,PeripheralPtr> myPeripheralsByName;
    std::map<PeripheralType,std::list<DeviceAppletPtr>> myAppletsByType;

    PeripheralPtr getPeripheral( std::string name ) const;
    DeviceAppletPtr getDeviceApplet( PeripheralType t , std::string cmdName ) const;
    SystemAppletPtr getSystemApplet( std::string name ) const;

public:
    ShellBackend();
    ~ShellBackend();

    void setPeripherals( std::vector<PeripheralPtr> peripherals )
    {
        myPeripherals   = peripherals;
    }

    void setDeviceApplets( std::vector<DeviceAppletPtr> const &applets)
    {
        myDevApplets   = applets;
    }

    void setSystemApplets( std::vector<SystemAppletPtr> const &applets)
    {
        mySysApplets    = applets;
    }
    
    void rebuildIndex();

    Result runDeviceApplet( std::string const &devName , std::string const &cmdName , CmdArguments &args , DataBucket &dataBucket );
    Result runSystemApplet( std::string const &cmdName , CmdArguments &args , StreamAdapter &stream );

    std::string help( std::string const &devType="" , std::string const &cmdName="" );

    bool addProvider(ShellProviderPtr provider);

    ShellProviderPtr getProvider(const std::string &prefix);

    Result runCommand( CmdArguments &args );


}; /* class ShellBackend */


typedef std::shared_ptr<ShellBackend> ShellBackendPtr;



#endif /* !defined SHELL_BACKEND_HPP */
