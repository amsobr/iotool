#ifndef APPLET_FACTORY_HPP
#define APPLET_FACTORY_HPP

#include <vector>

#include "device_applet.hpp"
#include "system_applet.hpp"

extern std::vector<DeviceAppletPtr> createDeviceApplets();
extern std::vector<SystemAppletPtr> createSystemApplets();


#endif /* !defined APPLET_FACTORY_HPP */
