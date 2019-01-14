
#include <vector>
#include <iostream>

#include <common/device_applet.hpp>
#include <common/system_applet.hpp>
#include <common/applet_factory.hpp>

#include "cmd_exit.hpp"
#include "cmd_adc_read.hpp"
#include "cmd_dac_set_out.hpp"
#include "cmd_adc_isource.hpp"
#include "cmd_led_set.hpp"
#include "cmd_pm_state.hpp"

using namespace std;

vector<DeviceAppletPtr> createDeviceApplets()
{
    vector<DeviceAppletPtr> applets;
    
    applets.push_back( DeviceAppletPtr(new CmdAdcRead()));
    applets.push_back( DeviceAppletPtr(new CmdDacSetOut()));
    applets.push_back( DeviceAppletPtr(new CmdLedSet()));
    applets.push_back( DeviceAppletPtr(new CmdAdcIsource()));
    applets.push_back( DeviceAppletPtr(new CmdPmState()));

    return applets;
}


vector<SystemAppletPtr> createSystemApplets()
{
    vector<SystemAppletPtr> applets;

    applets.push_back( SystemAppletPtr( new CmdExit() ));

    return applets;
}
