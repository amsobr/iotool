#ifndef ADS126X_READ_HPP
#define ADS126X_READ_HPP

#include <string>
#include <memory>

#include <shell-common/cmd_result.hpp>
#include <shell-common/cmd_arguments.hpp>
#include "ads126x.hpp"

class CmdAds126xRead : public CmdHandler
{
private:
    Ads126xPtr myAdc;

public:
    CmdAds126xRead( Ads126xPtr adc ) :
    CmdHandler("ads126x.read","Read one ADC channel of ADS126x") ,
    myAdc(adc)
    {
        myHelp  =   "Usage:\n"
                    "  ads126x.read ch=ARG\n"
                    "\n"
                    "Arguments:\n"
                    "ch        ID of the ADC channel: 0 .. 9\n"
                    "\n"
                    "Notes:\n"
                    "Currently only single ended reads are supported.\n"
                    ;

    }

    ~CmdAds126xRead() { }

    virtual CmdResult execute( CmdArguments const &args )
    {
        if ( !args.hasArg("ch") ) {
            return CmdResult(1,"Missing argument: ch\n");
        }

        std::string ch  = args.getValue("ch");
        int channel     = std::atoi(ch.c_str());
        int count       = args.hasArg("count") ? std::atoi(args.getValue("count").c_str()) : 1 ;
        for ( int i=0 ; i<count ; i++ ) {
            double val      = myAdc->readAnalog(channel);
            //char str[128];
            printf( "[% 2d/%d] ADS126x channel %d: %.8f V\n" , i+1 , count , channel , val );
        }
        return CmdResult(0,"OK");
    }
}; /* class CmdAds126xRead */


#endif /* !defined ADS126X_READ_HPP */
