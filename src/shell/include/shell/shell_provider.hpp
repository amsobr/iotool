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
#include "command_help.hpp"

class ShellProvider
{
private:
    std::vector<std::string> _mySHellProviderPrefixes;

public:
    ShellProvider( std::vector<std::string> prefixes ) :
    _mySHellProviderPrefixes(prefixes)
    {

    }
    virtual ~ShellProvider(){}

    std::vector<std::string> getPrefixes() const
    {
        return _mySHellProviderPrefixes;
    }

    bool provides( std::string const &prefix ) const
    {
        for ( auto p : _mySHellProviderPrefixes ) {
            if ( p==prefix ) {
                return true;
            }
        }
        return false;
    }


    /**
     * \brief Get the list of command HELP descriptors
     * \param helpVec If provided, it will be populated with CommandHelp descriptors provided by the shell provider.
     * \return If helpVec is not nullptr, returns the vector itseld. Otherwise returns a new vector.
     */
    virtual std::vector<CommandHelpPtr> getCommandHelp(std::vector<CommandHelpPtr> *helpVec = nullptr) = 0;

    /**a
     * \brief Run a command, given its prefix and arguments
     * \param prefix Prefix of the invoked command
     * \param args Remaining arguments
     * \param accumulator Accumulator bucket
     * \return Returns a result with code and expalanatory message
     */
    virtual Result runCommand(std::string const &prefix, CmdArguments &args, DataBucket &accumulator) = 0;
    virtual std::string helpBrief() = 0;
    virtual std::string helpFamily( std::string const &prefix ) = 0;
    virtual std::string helpCommand( std::string const &prefix , std::string const &cmd ) = 0;

};

typedef std::shared_ptr<ShellProvider> ShellProviderPtr;



#endif //IOTOOL_SHELL_PROVIDER_HPP
