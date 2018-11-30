
#include <vector>
#include <iostream>

#include <common/device_applet.hpp>
#include <common/system_applet.hpp>
#include <common/applet_factory.hpp>

#include "cmd_exit.hpp"
#include "cmd_adc_read.hpp"

using namespace std;

vector<DeviceAppletPtr> createDeviceApplets()
{
    vector<DeviceAppletPtr> applets;
    
    applets.push_back( DeviceAppletPtr(new CmdAdcRead()) );

    return applets;
}


vector<SystemAppletPtr> createSystemApplets()
{
    vector<SystemAppletPtr> applets;

    applets.push_back( SystemAppletPtr( new CmdExit() ));

    return applets;
}
