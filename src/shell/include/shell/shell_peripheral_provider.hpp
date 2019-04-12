#ifndef SHELL_PERIPHERAL_PROVIDER_HPP
#define SHELL_PERIPHERAL_PROVIDER_HPP

#include <list>
#include <string>
#include <memory>
#include <map>
#include <mutex>

#include <Poco/Logger.h>

#include <common/device_applet.hpp>
#include <common/cmd_arguments.hpp>
#include <common/result.hpp>
#include <common/peripheral.hpp>
#include "shell_provider.hpp"


class ShellPeripheralProvider : public ShellProvider {
private:
    std::recursive_mutex myMutex;

    std::vector<PeripheralPtr>    myPeripherals;
    std::vector<DeviceAppletPtr>  myDevApplets;

    /* cache */
    std::map<std::string,PeripheralPtr> myPeripheralsByName;
    std::map<PeripheralType,std::list<DeviceAppletPtr>> myAppletsByType;

    PeripheralPtr getPeripheral( std::string name ) const;
    DeviceAppletPtr getDeviceApplet( PeripheralType t , std::string cmdName ) const;

    Poco::Logger &logger;

public:
    ShellPeripheralProvider();
    ~ShellPeripheralProvider();

    void setPeripherals( std::vector<PeripheralPtr> peripherals )
    {
        myPeripherals   = peripherals;
    }

    void setDeviceApplets( std::vector<DeviceAppletPtr> const &applets)
    {
        myDevApplets   = applets;
    }

    void rebuildIndex();

    Result runDeviceApplet( std::string const &devName , std::string const &cmdName , CmdArguments &args , DataBucket &dataBucket );

    std::string help( std::string const &devType="" , std::string const &cmdName="" );


    Result runCommand(std::string const &prefix, CmdArguments &args, DataBucket &accumulator) override;

    std::string helpBrief() override;

    std::string helpFamily(std::string const &prefix) override;

    std::string helpCommand(std::string const &prefix, std::string const &cmd) override;
}; /* class ShellPeripheralProvider */


//typedef std::shared_ptr<ShellPeripheralProvider> ShellPeripheralProviderPtr;



#endif /* !defined SHELL_PERIPHERAL_PROVIDER_HPP */
