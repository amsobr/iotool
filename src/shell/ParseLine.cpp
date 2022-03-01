//
// Created by to on 20/02/22.
//

#include <common/Strings.hpp>

#include "ParseLine.hpp"

std::tuple<std::string, rps::ArgumentList> parseLine(std::string const& line)
{
    if ( line.empty() ) {
        return {};
    }

    auto tokens = Strings::split(line," ");
    if ( tokens.empty() ) {
        return {};
    }
    if ( tokens.size()==1 ) {
        return { tokens[0] , {} };
    }

    return {
        tokens[0] ,
        rps::ArgumentList{ { tokens.begin()+1 , tokens.end() } }
    };
}
