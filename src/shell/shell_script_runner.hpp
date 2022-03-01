//
// Created by to on 19/04/19.
//

#ifndef IOTOOL_SHELL_SCRIPT_RUNNER_HPP
#define IOTOOL_SHELL_SCRIPT_RUNNER_HPP

#include "shell_backend.hpp"

class ShellScriptRunner
{
public:
    static Result runScript( std::vector<std::string> const &script , ShellBackendPtr backend )
    {
        Poco::Logger &logger  = Poco::Logger::get("iotool");
        for ( auto line : script ) {
            logger.debug( Poco::format("ScriptRunner: executing line '%s'...",line));
            Result r = backend->runCommand(line, nullptr);
            if ( !r.isSuccess() ) {
                logger.error( Poco::format("ScriptRunner: error while executing '%s' - result is %s",line,r.toString()));
                return r;
            }
        }
        return Result::OK;
    }
};


#endif //IOTOOL_SHELL_SCRIPT_RUNNER_HPP
