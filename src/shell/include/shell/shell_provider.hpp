//
// Created by to on 29/03/19.
//

#ifndef IOTOOL_SHELL_PROVIDER_HPP
#define IOTOOL_SHELL_PROVIDER_HPP

#include <memory>
#include <vector>
#include <string>
#include <common/cmd_arguments.hpp>
#include <common/data_bucket.hpp>
#include <common/result.hpp>

class ShellProvider
{
public:
    ShellProvider() {}
    virtual ~ShellProvider(){}

    virtual std::vector<std::string> getPrefixes() const;

    bool provides( std::string const &prefix )
    {
        for ( auto p : getPrefixes() ) {
            if ( p==prefix ) {
                return true;
            }
        }
        return false;
    }


    virtual Result runCommand(std::string const &prefix, CmdArguments &args, DataBucket &outcome) = 0;
    virtual std::string helpBrief() = 0;
    virtual std::string helpFamily( std::string const &prefix );
    virtual std::string helpCommand( std::string const &prefix , std::string const &cmd ) = 0;

};

typedef std::shared_ptr<ShellProvider> ShellProviderPtr;



#endif //IOTOOL_SHELL_PROVIDER_HPP
