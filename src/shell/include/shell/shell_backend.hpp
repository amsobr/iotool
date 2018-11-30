#ifndef SHELL_BACKEND_HPP
#define SHELL_BACKEND_HPP

#include <list>
#include <string>
#include <memory>
#include <map>

#include <common/system_applet.hpp>
#include <common/device_applet.hpp>
#include <common/cmd_arguments.hpp>
#include <common/result.hpp>
#include <common/peripheral.hpp>


class ShellBackend {
private:
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

    Result runDeviceApplet( std::string const &devName , std::string const &cmdName , CmdArguments const &args );
    Result runSystemApplet( std::string const &cmdName , CmdArguments const &args );

    std::string help( std::string const &devType="" , std::string const &cmdName="" ) const;

}; /* class ShellBackend */


typedef std::shared_ptr<ShellBackend> ShellBackendPtr;



#endif /* !defined SHELL_BACKEND_HPP */
