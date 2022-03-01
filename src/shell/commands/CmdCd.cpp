//
// Created by to on 22/02/22.
//

#include <rps/ContextFactory.hpp>
#include "CmdCd.hpp"

CmdCd::CmdCd() :
rps::AbstractCommand{"cd","change working path"}
{
    setHelp(
        "cd - change working path\n"
        " Usage:\n"
        "   cd PATH\n"
        "Parameters:\n"
        "  PATH   new working Path"
    );

}

void CmdCd::run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const
{
    if ( args.size()!=1 ) {
        ctx->stream->writeLine( "cd: need exactly 1 argument (path)" );
    }
    else {
        ctx->changePath( args.at(0).getToken() );
    }

}
