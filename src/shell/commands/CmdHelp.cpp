//
// Created by to on 28/02/22.
//

#include <rps/Context.hpp>
#include <rps/Script.hpp>
#include <rps/AbstractCommand.hpp>

#include "CmdHelp.hpp"

void CmdHelp::run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const
{
    if ( args.isEmpty() ) {
        auto [ cmds, scripts, children ]    = ctx->listAll();
        auto curPath    = ctx->getCurPath().toString();
        
        std::ostringstream oss;
        oss << Poco::format("Currently on path: %s\n",curPath);
        oss << "Available commands:\n";
        for ( auto const& c: cmds ) {
            oss << Poco::format("  %10s - %s\n",c->getName(),c->getBrief());
        }
        
        if ( !scripts.empty() ) {
            oss << "\nAvailable scripts:\n";
            for ( auto const& c: scripts ) {
                oss << Poco::format("  %10s\n",c->getName());
            }
        }
        
        oss << "\nAvailable paths:\n";
        for ( auto const& p: children ) {
            oss << Poco::format( "%s/%s\n",curPath,p);
        }
        ctx->stream->writeLine(oss.str());
    }
    else {
        auto cmd    = ctx->findCommand(args.at(0).getToken());
        std::ostringstream oss;
        oss << Poco::format("%s: %s\n",cmd->getName(),cmd->getBrief());
        oss << cmd->help();
        ctx->stream->writeLine(oss.str());
    }
}

/*
 * Available commands at '/adc':
 *   * cmd1 - brief_1
 *   * cmd2 - brief_2
 *   * cmd3 - brief_3
 *
 *   Available Paths
 *   * /adc/0
 *   * /adc/1
 *   * /adc/2
 */
