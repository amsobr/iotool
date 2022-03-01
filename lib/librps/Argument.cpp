//
// Created by to on 03/03/21.
//

#include <rps/Argument.hpp>

namespace rps
{

Argument::Argument(std::string token) :
myToken{ std::move(token) }
{
    auto pos = myToken.find('=');
    if ( pos==std::string::npos ) {
        myIsKvPair  = false;
        myKey       = token;
    }
    else {
        myIsKvPair  = true;
        myKey       = myToken.substr(0,pos);
        if ( pos<myToken.size()-1 ) {
            myValue = myToken.substr(pos+1);
        }
    }
}

} /* namespace rps */
