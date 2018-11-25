#ifndef ADS126X_CAL_HPP
#define ADS126X_CAL_HPP

#include <string>
#include <memory>

#include <shell-common/cmd_result.hpp>
#include <shell-common/cmd_arguments.hpp>
#include <shell-common/cmd_handler.hpp>
#include "ads126x.hpp"

class CmdAds126xCal : public CmdHandler
{
private:
    Ads126xPtr myAdc;

public:
    CmdAds126xCal( Ads126xPtr adc ) :
    CmdHandler("ads126x.cal","Calibrate ADS126x (whatever that means...)") ,
    myAdc(adc) 
    {
/*        myHelp  =   "Usage:\n"
                    "  ads126x.read ch=ARG\n"
                    "\n"
                    "Arguments:\n"
                    "ch        ID of the ADC channel: 0 .. 9\n"
                    "\n"
                    "Notes:\n"
                    "Currently only single ended reads are supported.\n"
                    ;
*/
    }

    ~CmdAds126xCal() { }

    virtual CmdResult execute( CmdArguments const &/*args*/ )
    {        
        myAdc->calibrate();
        return CmdResult(0,"ADC calibration complete");
    }
}; /* class CmdAds126xCal */


#endif /* !defined ADS126X_CAL_HPP */
