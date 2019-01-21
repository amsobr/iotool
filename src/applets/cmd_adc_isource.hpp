#ifndef CMD_ADC_ISOURCE_HPP
#define CMD_ADC_ISOURCE_HPP

#include <string>
#include <memory>
#include <iostream>

#include <Poco/Format.h>
#include <Poco/String.h>
#include <Poco/NumberParser.h>

#include <common/result.hpp>
#include <common/cmd_arguments.hpp>
#include <common/device_applet.hpp>
#include <common/adc.hpp>
#include <common/data_bucket.hpp>


/**
 * @brief Confgure one ADC current source
 * 
 */
class CmdAdcIsource : public DeviceApplet
{
private:
    std::string myHelp;

public:
    CmdAdcIsource() :
    DeviceApplet( PeripheralType::ADC , "isource","Configure one ADC current source")
    {
        myHelp  =   "Usage:\n"            
                    "  DEV isource info     : Print current source descriptor\n"
                    "  DEV isource disable  : Disable the current source\n"
                    "      source=ARG\n"
                    "  DEV isource set      : Configure the current source\n"
                    "      source=ARG ch=ARG\n"
                    "      mag=ARG\n"                    
                    "\n"
                    "Arguments:\n"
                    "source    ID of the current source\n"
                    "ch        ID of the ADC channel: 0 .. 9 where current is routed to\n"
                    "mag       Magnitude of the current source. Use info to check for\n"
                    "          supported values\n"
                    ;
    }

    virtual ~CmdAdcIsource() { }

    Result executeInfo( AdcPtr adc , DataBucket &db )
    {
        /*
        adc0.isource.count=N
        adc0.isource0.mags=mag1,mag2,mag3,mag
        ...
        */
        size_t nSources      = adc->getNumCurrentSources();
        db.addDataPoint("isource.count",nSources,adc.get());
        //std::string output   = Poco::format("%s.isource.count=%u\n",adcName,adc->getNumCurrentSources());
       for ( size_t i=0 ; i<nSources ; i++ ) {
           std::list<std::string> mags   = adc->getCurSourceMagnitudes(i);
           //output  += Poco::format("%s.isource%u.mags=%s\n",adcName,i,Poco::cat(std::string(","),mags.begin(),mags.end()));
            db.addDataPoint( Poco::format("isource%u.mags\n",i) ,
                             Poco::cat(std::string(","),mags.begin(),mags.end()) ,
                             adc.get()
            );
       }
       //stream.writeLine(output);
       return Result::OK;
    }

    Result executeDisable(CmdArguments &args , AdcPtr adc )
    {
        try {
            Argument arg    = args.shift();
            if ( arg.name()!="source" ) {
                return Result(1,"Invalid argument: " + std::string(arg) );
            }
            unsigned int source = Poco::NumberParser::parseUnsigned(arg.value());
            int res = adc->setCurrentSource(source,false);
            return Result(res,"");            
        }
        catch ( Poco::SyntaxException &e ) {
            return Result(1,"Invalid number format.");
        }
        return Result(1,"Unknown error!");
    }

    Result executeSet( CmdArguments &args , AdcPtr adc )
    {
        try {
            if ( !args.hasArg("ch") || !args.hasArg("source") || !args.hasArg("mag") ) {
                return Result(1,"Missing arguments");
            }
            unsigned int source = Poco::NumberParser::parseUnsigned(args.getValue("source"));
            unsigned int ch     = Poco::NumberParser::parseUnsigned(args.getValue("ch"));
            std::string  mag    = args.getValue("mag");
            int res             = adc->setCurrentSource(source,true,ch,mag);
            return Result(res,"");
        }
        catch ( Poco::SyntaxException &e ) {
            return Result(1,"Invalid number format");
        }
        return Result(1,"Unknown Error");
    }

    virtual std::string help() const { return myHelp; }

    virtual Result execute( CmdArguments &args , PeripheralPtr p , DataBucket &dataBucket )
    {
        AdcPtr adc  = std::dynamic_pointer_cast<Adc>(p);
        if ( args.size()<1 ) {
            return Result(1,"Invalid arguments");
        }

        Argument arg    = args.shift();
        if ( ! arg.isToken() ) {
            return Result(1,"Invalid argument: " + std::string(arg));
        }

        if ( arg.token()=="info" ) {
            return executeInfo(adc,dataBucket);
        }
        else if ( arg.token()=="disable" ) {
            return executeDisable(args,adc);
        }
        else if ( arg.token()=="set" ) {
            return executeSet(args,adc);
        }
        else {
            return Result(1,"Invalid isource command: " + arg.token() );
        }
    }
        
}; /* class CmdAdcIsource */

#endif /* !defined CMD_ADC_ISOURCE_HPP */
