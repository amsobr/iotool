#ifndef CMD_RELAY_SET_HPP
#define CMD_RELAY_SET_HPP

#include <string>
#include <memory>

#include <common/result.hpp>
#include <common/cmd_arguments.hpp>
#include <common/device_applet.hpp>
#include <common/PeripheralType.hpp>
#include <common/digital_output.hpp>
#include <common/data_bucket.hpp>

class CmdRelaySet : public DeviceApplet
{
private:
    std::string myHelp;

public:
    CmdRelaySet() :
    DeviceApplet(PeripheralType::DOUT,"set","Set one ore more relays")
    {
        myHelp  =   "Usage:\n"
                    "  set name1=on|off [name2=on|off ...]\n"
                    "\n"
                    "Arguments:\n"
                    "name      Name of the relay\n"
                    "on|off    Desired relay state\n"
                    ;
    }

    ~CmdRelaySet() { }

    virtual std::string help() const { return myHelp; }

    virtual Result execute( CmdArguments &args , PeripheralPtr p , DataBucket &db )
    {
        DigitalOutputPtr  douts = std::dynamic_pointer_cast<DigitalOutput>(p);

        if ( args.size()==0 ) {
            return Result::E_BAD_ARGS;
        }

        while ( args.size()!=0 ) {
            Argument a(args.shift());
            if ( a.isToken() ) {
                return Result::E_BAD_ARGS;
            }
            std::string relayName = a.name();
            
            std::string relaySttStr = a.value();
            bool relayState;
            if ( relaySttStr=="on" ) {
                relayState  = true;
            }
            else if ( relaySttStr=="off" ) {
                relayState  = false;
            }
            else {
                return Result::E_BAD_ARGS;
            }

            int res = douts->setOut(relayName,relayState);
            if ( res!=0 ) {
                return Result(res,"HW ERROR");
            }
        }
        return Result::OK;
    }
}; /* class CmdRelaySet */


#endif
