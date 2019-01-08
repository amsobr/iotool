#ifndef CMD_ADC_READ_HPP
#define CMD_ADC_READ_HPP

#include <string>
#include <memory>

#include <common/result.hpp>
#include <common/cmd_arguments.hpp>
#include <common/device_applet.hpp>
#include <common/adc.hpp>
#include <drivers/ads126x.hpp>

/**
 * @brief Read one ADC channel
 * 
 */
class CmdAdcRead : public DeviceApplet
{
private:
    std::string myHelp;

public:
    CmdAdcRead() :
    DeviceApplet( PeripheralType::ADC , "read","Read one ADC channel")
    {
        myHelp  =   "Usage:\n"
                    "  DEV read ch=ARG\n"
                    "\n"
                    "Arguments:\n"
                    "ch        ID of the ADC channel: 0 .. 9\n"
                    "\n"
                    "Notes:\n"
                    "Currently only single ended reads are supported.\n"
                    ;

    }

    virtual ~CmdAdcRead() { }

    virtual Result execute( CmdArguments &args , PeripheralPtr p )
    {
        AdcPtr adc  = std::dynamic_pointer_cast<Adc>(p);
        if ( !args.hasArg("ch") ) {
            return Result(1,"Missing argument: ch\n");
        }

        std::string ch  = args.getValue("ch");
        int channel     = std::atoi(ch.c_str());
        int count       = args.hasArg("count") ? std::atoi(args.getValue("count").c_str()) : 1 ;
        for ( int i=0 ; i<count ; i++ ) {
            double val      = adc->readAnalog(channel);
            //char str[128];
            printf( "[% 2d/%d] ADS126x channel %d: %.8f V\n" , i+1 , count , channel , val );
        }
        return Result(0,"OK");
    }

    virtual std::string help() const { return myHelp; }
}; /* class CmdAdcRead */


#endif /* !defined CMD_ADC_READ_HPP */
