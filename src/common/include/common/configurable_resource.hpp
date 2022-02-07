//
// Created by to on 05/05/19.
//

#ifndef IOTOOL_CONFIGURABLE_RESOURCE_HPP
#define IOTOOL_CONFIGURABLE_RESOURCE_HPP


#include <string>
#include "option_help.hpp"

class ConfigurableResource
{
protected:
public:
    ConfigurableResource()
    {}

    virtual ~ConfigurableResource()
    {}

    virtual bool setOption( std::string const &optName , std::string const &optValue)
    {
        return false;
    }

    virtual std::string getOption( std::string const &optName )
    {
        return "";
    }

    virtual std::vector<OptionHelpPtr> getOptionHelp()
    {
        return std::vector<OptionHelpPtr>();
    }






};


#endif //IOTOOL_CONFIGURABLE_RESOURCE_HPP
