#ifndef DAC_SET_OUT_HPP
#define DAC_SET_OUT_HPP

#include <string>
#include <memory>

#include <common/result.hpp>
#include <common/cmd_arguments.hpp>
#include <common/dac.hpp>
#include <common/device_applet.hpp>
#include <common/PeripheralType.hpp>
#include <common/data_bucket.hpp>

class CmdDacSetOut : public DeviceApplet
{
private:
    std::string myHelp;

public:
    CmdDacSetOut() :
    DeviceApplet(PeripheralType::DAC,"set","Set one DAC output")
    {
        myHelp  =   "Usage:\n"
                    "  set ch=ARG level=ARG\n"
                    "\n"
                    "Arguments:\n"
                    "ch        ID of the DAC channel: 0 .. n\n"
                    "level     Desired voltage, in mV\n"
                    ;

    }

    ~CmdDacSetOut() { }

    virtual Result execute( CmdArguments &args , PeripheralPtr p , DataBucket &dataBucket )
    {
        DacPtr  dac = std::dynamic_pointer_cast<Dac>(p);

        if ( !args.hasArg("ch") ) {
            return Result(1,"Missing argument: ch\n");
        }

        if ( !args.hasArg("level") ) {
            return Result(1,"Missing argument: level\n");
        }

        if ( args.size()!=2 ) {
            return Result(1,"Invalid number of arguments\n");
        }

        unsigned int ch     = std::atoi(args.getValue("ch").c_str());
        unsigned int level  = std::atoi(args.getValue("level").c_str());
        
        dac->setOutput(ch,level);
        return Result(0,"OK");
    }
}; /* class CmdDacSetOut */


#endif /* !defined ADS126X_READ_HPP */
