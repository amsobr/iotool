//
// Created by to on 05/03/22.
//

#include <rps/Exception.hpp>
#include <rps/Context.hpp>
#include <rps/Stack.hpp>

#include <common/Board.hpp>

#include "CmdShow.hpp"


void CmdShow::run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const
{
    auto const numArgs    = args.size();
    if ( numArgs<1 ) {
        throw rps::InvalidArgumentsException{"show needs at least one argument"};
    }
    
    if ( !args.at(0).isToken() ) {
        throw rps::InvalidArgumentsException{"show needs exactly one token argument (got one KV pair)"};
    }
        
    auto const& mode    = args.at(0).getToken();
    
    if ( numArgs==1 && mode=="stack" ) {
        ctx->stream->writeLine("Stack:");
        auto stack  = ctx->stack.getStack();
        auto depth  = stack.size() - 1;
        for ( auto const& v : stack ) {
            ctx->stream->writeLine(Poco::format("[%02z]: %f",depth,v));
            depth--;
        }
    }
    else if ( numArgs==1 && (mode=="vars" || mode=="variables") ) {
        ctx->stream->writeLine("Variables:");
        for ( auto const& var: ctx->stack.getVariables() ) {
            ctx->stream->writeLine(Poco::format("  '%12s' = %f\n",var.name(),var.value()));
        }
        ctx->stream->writeLine("");
    }
    else if ( numArgs==1 && (mode=="consts" || mode=="constants") ) {
        auto allConsts  = ctx->stack.getConstants();
        ctx->stream->writeLine("Constants:");
        for ( auto const& ct : ctx->stack.getConstants() ) {
            ctx->stream->writeLine(Poco::format("  '%12s' = %f",ct.name(),ct.value()));
        }
        ctx->stream->writeLine("\n");
    }
    else if ( numArgs==1 && mode=="devices" ) {
        ctx->stream->writeLine("Available Peripherals:\n");
        for ( auto const& p : Board::get()->getPeripherals() ) {
            ctx->stream->writeLine( Poco::format(
                "%-10s  :\n"
                "  class, id      : %s , %d\n"
                "  vendor         : %s\n"
                "  model (rev)    : %s (rev. %s)\n"
                "  driver version : %s\n"
                "  author         : %s\n" ,
                p->getAlias() ,
                str(p->getType()) , p->getId() ,
                p->getVendor() ,
                p->getModel() , p->getRevision() ,
                p->getDriverVersion() ,
                p->getAuthor()
            ) );
        }
    }
    else {
        throw rps::InvalidArgumentsException{"invalid mode: " + mode};
    }
}
    
    
