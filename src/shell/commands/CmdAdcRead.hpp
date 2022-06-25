
#pragma once

#include <string>
#include <memory>
#include <iostream>

#include <Poco/Format.h>
#include <Poco/String.h>
#include <Poco/Stopwatch.h>
#include <Poco/NumberParser.h>
#include <Poco/Format.h>

#include <common/Adc.hpp>
#include <common/Board.hpp>
#include <common/result.hpp>

#include <rps/ArgumentList.hpp>
#include <rps/AbstractCommand.hpp>
#include <rps/Context.hpp>

/**
 * @brief Read one ADC channel
 * 
 */
class CmdAdcRead : public rps::AbstractCommand // , public PeripheralCommand<Adc>
{
public:
    CmdAdcRead() :
    rps::AbstractCommand{"read"}
    {
        setBrief("read ADC inputs");
        setHelp(
            "Usage:\n"
            "  read ch=ARG           : read single ended\n"
            "  read chp=ARG chn=ARG  : read differential\n"
            "\n"
            "Arguments:\n"
            "  ch        ID of the ADC channel: 0 .. n\n"
            "  chp       ID of the positive side input\n"
            "  chn       ID of the negative side input\n"
            "\n"
        );
    }

    ~CmdAdcRead() override = default;

    void run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const override
    {
        auto adc  = Board::get()->getPeripheral<Adc>(ctx->getCwd());

        int count(1);
        if ( args.has("count") ) {
            count   = (int)args.get("count").getValueAsInteger();
        }
        
        if ( count<1 ) {
            throw std::runtime_error{"count must be positive integer (>=1)"};
        }

        Poco::Stopwatch stopwatch;
        stopwatch.start();
        if ( args.has("ch") ) {
            if ( args.has("chp") || args.has("chn") ) {
                throw std::runtime_error{"Cannot mix single ended and differential reads."};
            }

            int ch = (int)args.get("ch").getValueAsInteger();
            
            if ( count==1 ) {
                ctx->stack.push( adc->readAnalog(ch) );
            }
            else {
                std::vector<int32_t> digVal;
                adc->read(ch, count, digVal);
                std::vector<double> anVal(digVal.size());
                double const res    = adc->getResolution(ch);
                //poco_debug( logger, Poco::format("resolution=%.5e full_scale=%f\n",res,0x100000000ll*res));
                for (size_t i=0 ; i<digVal.size() ; i++ ) {
                    anVal[i]   = digVal[i]*res;
                }
                ctx->stack.push( anVal );
                //std::cout << Poco::format("ADS126x: %d reads took %?ums\n",count,stopwatch.elapsed()/1000);
            }
            return;
        }
        else if ( args.has("chp") && args.has("chn") ) {
            int chp = (int)args.get("chp").getValueAsInteger();
            int chn = (int)args.get("chn").getValueAsInteger();

            for ( int i=0 ; i<count ; i++ ) {
                double val  = adc->readDifferential(chp,chn);
                ctx->stack.push(val);
            }
            //if ( count>1 ) {
            //    std::cout << Poco::format("ADS126x: %d reads took %?ums\n",count,stopwatch.elapsed()/1000);
            //}
            return;
        }
        else {
            throw std::runtime_error{ "Either ch or combination of chp+chn must be provided"};
        }
    }

}; /* class CmdAdcRead */

