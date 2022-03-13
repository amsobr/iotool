//
// Created by to on 05/05/19.
//

#pragma once

#include <string>
#include "option_help.hpp"

class Configurable
{
protected:
public:
    Configurable() = default;

    virtual ~Configurable() = default;

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
        return {};
    }
};

