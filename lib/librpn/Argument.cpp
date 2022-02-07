//
// Created by to on 03/03/21.
//

#include <rpn/Argument.h>

namespace Rpn
{

Argument::Argument(const std::string& token)
{
    auto pos = token.find('=');
    if ( pos==std::string::npos ) {
        myIsKvPair  = false;
        myKey       = token;
    }
    else {
        myIsKvPair  = true;
        myKey       = token.substr(0,pos-1);
        if ( pos<token.size()-1 ) {
            myValue = token.substr(pos+1);
        }
    }
}


} /* namespace Rpn */