#ifndef CMD_ADC_READ_HPP
#define CMD_ADC_READ_HPP

#include <string>
#include <memory>
#include <iostream>

#include <Poco/Format.h>
#include <Poco/String.h>
#include <Poco/NumberParser.h>
#include <Poco/Format.h>


#include <common/result.hpp>
#include <common/cmd_arguments.hpp>
#include <common/device_applet.hpp>
#include <common/data_bucket.hpp>
#include <common/adc.hpp>
#include <drivers/ads126x.hpp>
#include <common/stream_adapter.hpp>

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
                    "  DEV read ch=ARG           : read single ended\n"
                    "  DEV read chp=ARG chn=ARG  : read differential\n"
                    "\n"
                    "Arguments:\n"
                    "ch        ID of the ADC channel: 0 .. 9\n"
                    "chp       ID of the positive side input\n"
                    "chn       ID of the negative side input\n"
                    "\n"
                    ;

    }

    virtual ~CmdAdcRead() { }

    virtual Result execute( CmdArguments &args , PeripheralPtr p , DataBucket &dataBucket )
    {
        try { 
            AdcPtr adc  = std::dynamic_pointer_cast<Adc>(p);

            unsigned int count(1);
            if ( args.hasArg("count") ) {
                count   = Poco::NumberParser::parseUnsigned(args.getValue("count"));
            }
        
            if ( args.hasArg("ch") ) {
                if ( args.hasArg("chp") || args.hasArg("chn") ) {
                    return Result(1,"Cannot mix single ended and differential reads.");
                }

                unsigned int ch = Poco::NumberParser::parseUnsigned(args.getValue("ch"));

                for ( unsigned int i=0 ; i<count ; i++ ) {
                    double val      = adc->readAnalog(ch);
                    dataBucket.addDataPoint(ch,val,p.get());
                }
                return Result(0,"OK");
            }
            else if ( args.hasArg("chp") && args.hasArg("chn") ) {
                unsigned int chp = Poco::NumberParser::parseUnsigned(args.getValue("chp"));
                unsigned int chn = Poco::NumberParser::parseUnsigned(args.getValue("chn"));
                
                std::string label( Poco::format("%u-%u",chp,chn) );
                for ( unsigned int i=0 ; i<count ; i++ ) {
                    double val  = adc->readDifferential(chp,chn);
                    dataBucket.addDataPoint(label,val,adc.get());
                }
                return Result(0,"OK");
            }
            else {
                return Result(1,"Either ch or combination of chp+chn must be provided");
            }
        }
        catch ( Poco::SyntaxException &e ) {
            return Result(1,"Invalid number format.");
        }
        catch (...) { }
        return Result(1,"UNKNOWN EXCEPTION");
    }

    virtual std::string help() const { return myHelp; }
}; /* class CmdAdcRead */


#endif /* !defined CMD_ADC_READ_HPP */
