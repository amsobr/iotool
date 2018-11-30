#ifndef CMD_ARGUMENTS_HPP
#define CMD_ARGUMENTS_HPP

#include <memory>
#include <list>
#include <algorithm>

#include <cbsl/keyval.hpp>

class CmdArguments
{
private:
    std::list<cbsl::KeyVal::Pair> myArgs;

public:

CmdArguments()
{
}

CmdArguments( std::list<cbsl::KeyVal::Pair> const &args ) :
myArgs(args)
{
    
}

~CmdArguments() {
    myArgs.clear();
}


void addArg( cbsl::KeyVal::Pair const &arg )
{
    myArgs.push_back(arg);
}

bool hasArg( std::string const &name ) const
{
    for( auto const &arg : myArgs ) {
        if ( arg.isAnonymous() ) {
            if ( arg.value==name ) return true;
        }
        else {
            if ( arg.key==name ) return true;
        }
    }
    return false;
}

std::string getValue( std::string const &name) const
{
    for( auto &arg : myArgs ) {
        if ( !arg.isAnonymous() ) { 
            if ( arg.key==name ) return arg.value;
        }
    }
    return "";
}

size_t size() const
{
    return myArgs.size();
}
    

}; /* class CmdArguments */


typedef std::shared_ptr<CmdArguments> CmdArgumentsPtr;



#endif /* !defined CMD_ARGUMENTS_HPP */
