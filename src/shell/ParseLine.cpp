//
// Created by to on 20/02/22.
//

#include <common/Strings.hpp>

#include "ParseLine.hpp"

std::tuple<std::string, std::string, rps::ArgumentList> parseLine(std::string const& line)
{
    if ( line.empty() ) {
        return {};
    }

    auto tokens = Strings::split(line," ");
    if ( tokens.empty() ) {
        return {};
    }
    if ( tokens.size()==1 ) {
        if ( tokens[0].starts_with("/") ) {
            return { tokens[0] , "", {} };
        }
        else {
            return { "" , tokens[0] , {} };
        }
    }

    if ( tokens[0].starts_with("/") ) {
        if ( tokens.size()>2 ) {
            return { tokens[0], tokens[1] ,
                     rps::ArgumentList{ {tokens.begin()+2,tokens.end()}} };
        }
        else {
            return { tokens[0], tokens[1], {} };
        }
    }
    else {
        return { "" , tokens[0] ,
                 rps::ArgumentList{ { tokens.begin()+1 , tokens.end() } } };
    }
}
