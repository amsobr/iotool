#pragma once

#include <cassert>
#include <string>
#include <memory>
#include <iostream>

#include <Poco/Format.h>
#include <Poco/String.h>
#include <Poco/NumberParser.h>

#include <rps/AbstractCommand.hpp>
#include <rps/Context.hpp>

#include <common/Board.hpp>


#include <common/result.hpp>
#include <common/Adc.hpp>

/**
 * @brief Confgure one ADC current source
 * 
 */
class CmdAdcIsource : public rps::AbstractCommand
{
public:
    CmdAdcIsource() :
        rps::AbstractCommand{ "isource","Configure one ADC current source" }
    {
        setHelp(    "Usage:\n"
                    "  isource info\n"
                    "  isource disable source=SRC\n"
                    "  isource set source=SRC ch=CH mag=MAG\n"
                    "Commands:\n"
                    "  info     : Print current source descriptor\n"
                    "  disable  : Disable a current source\n"
                    "  set      : Enable and route a current source into\n"
                    "             an ADC channel\n"
                    "\n"
                    "Arguments:\n"
                    "  source   : specifies the current source\n"
                    "  ch       : specified the ADC channel\n"
                    "  mag      : selects the source current. Use info\n"
                    "             to get list of available values\n"
                    "\n"
                    "Examples:\n"
                    "  Set an 1uA current source 1 on ADC channel 2:\n"
                    "    isource set source=1 ch=2 mag=1u\n"
                    "\n"
                    "  Disable current source 1:\n"
                    "    isource disable source=1\n"
                    "\n"
        );
    }

    ~CmdAdcIsource() override = default;

    static Result executeInfo( AdcPtr const& adc , rps::ContextPtr const& ctx )
    {
        size_t numSources   = adc->getNumCurrentSources();
        if ( numSources==0 ) {
            ctx->stream->writeLine( Poco::format("%s: no current sources",adc->getAlias()));
        }
        else {
            ctx->stream->writeLine( Poco::format("Current sources for %s:\n",adc->getAlias()));
            for ( size_t i=0 ; i<numSources ; i++ ) {
                ctx->stream->writeLine( Poco::format("source %z:",i));
                auto sourceMags      = adc->getCurSourceMagnitudes(i);
                std::ostringstream oss;
                oss << "magnitudes : ";
                bool first  = true;
                for ( auto const& m : sourceMags ) {
                    if ( first ) {
                        first   = false;
                    }
                    else {
                        oss << ", ";
                    }
                    oss << m;
                }
                oss << "\n";
                ctx->stream->writeLine( oss.str() );
            }
        }
        return Result::OK;
    }

    static Result executeDisable( AdcPtr const& adc , int source )
    {
        adc->setCurrentSource(source,false);
        return Result::OK;
    }

    static Result executeSet( AdcPtr const& adc, int source, int ch, std::string const& mag )
    {
        adc->setCurrentSource(source,true,ch,mag);
        return Result::OK;
    }
    
    void run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const override
    {
        auto adc    = Board::get()->getPeripheral<Adc>(ctx->getCwd());
        assert(adc!=nullptr);
        
        if ( args.size()<1 ) {
            throw rps::InvalidArgumentsException{"isource needs at least one argument"};
        }
        
        auto mode   = args.at(0).getToken();
        if ( mode=="info" ) {
            executeInfo(adc,ctx);
        }
        else if ( mode=="disable" ) {
            int source  = (int)args.get("source").getValueAsUnsigned();
            executeDisable(adc,source);
        }
        else if ( mode=="set" ) {
            int source  = (int)args.get("source").getValueAsUnsigned();
            int ch      = (int)args.get("source").getValueAsUnsigned();
            std::string mag = args.get("mag").getValue();
            executeSet(adc,source,ch,mag);
        }
        else {
            throw rps::InvalidArgumentsException{"invalid arguments"};
        }
    }

}; /* class CmdAdcIsource */

