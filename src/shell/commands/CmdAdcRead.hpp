
#pragma once

#include <string>
#include <memory>
#include <iostream>

#include <Poco/Format.h>
#include <Poco/String.h>
#include <Poco/NumberParser.h>
#include <Poco/Format.h>

#include <common/adc.hpp>
#include <common/Board.hpp>
#include <common/result.hpp>

#include <rps/ArgumentList.hpp>
#include <rps/AbstractCommand.hpp>
#include <rps/Context.hpp>

#include <drivers/ads126x.hpp>

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

        unsigned int count(1);
        if ( args.has("count") ) {
            count   = args.get("count").getValueAsUnsigned();
        }

        if ( args.has("ch") ) {
            if ( args.has("chp") || args.has("chn") ) {
                throw std::runtime_error{"Cannot mix single ended and differential reads."};
            }

            unsigned int ch = args.get("ch").getValueAsUnsigned();

            for ( unsigned int i=0 ; i<count ; i++ ) {
                double val      = adc->readAnalog(ch);
                ctx->stack.push(val);
            }
            return;
        }
        else if ( args.has("chp") && args.has("chn") ) {
            unsigned int chp = args.get("chp").getValueAsUnsigned();
            unsigned int chn = args.get("chn").getValueAsUnsigned();

            for ( unsigned int i=0 ; i<count ; i++ ) {
                double val  = adc->readDifferential(chp,chn);
                ctx->stack.push(val);
            }
            return;
        }
        else {
            throw std::runtime_error{ "Either ch or combination of chp+chn must be provided"};
        }
    }

}; /* class CmdAdcRead */

