#ifndef DEVICE_APPLET_HPP
#define DEVICE_APPLET_HPP

#include <string>
#include <memory>

#include "peripheral_type.hpp"
#include "peripheral.hpp"

#include "applet.hpp"
#include "result.hpp"
#include "cmd_arguments.hpp"

class DeviceApplet : public Applet
{
private:
    DeviceApplet();    

protected:
    PeripheralType const myType;

    DeviceApplet( PeripheralType t , std::string name , std::string brief="" ) :
    Applet(name,brief) ,
    myType(t)
    {
    }


public:

    virtual ~DeviceApplet()
    {
    }

    PeripheralType getType() const { return myType; }
    
    virtual Result execute( CmdArguments &args , PeripheralPtr dev )    = 0;

};

typedef std::shared_ptr<DeviceApplet> DeviceAppletPtr;


#endif /* !defined DEVICE_APPLET_HPP */
