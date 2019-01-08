#ifndef CMD_ARGUMENTS_HPP
#define CMD_ARGUMENTS_HPP

#include <memory>
#include <list>
#include <algorithm>

#include <cbsl/keyval.hpp>

class Argument
{
private:
    cbsl::KeyVal::Pair pair;

public:
    Argument() {}

    Argument( cbsl::KeyVal::Pair const &p ) :
    pair(p)
    {

    }

    Argument( Argument const &other ) :
    pair(other.pair)
    {
    }

    Argument operator=( Argument const &other )
    {
        pair    = other.pair;
        return *this;
    }

    std::string name() const {
        return pair.isAnonymous() ? pair.value : pair.key;
    }

    std::string value() const {
        return pair.value;
    }

    std::string token() const {
        return pair.isAnonymous() ? pair.value : pair.key;
    }

    bool isToken() const { return pair.isAnonymous(); }

}; /* class Argument */

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

Argument shift()
{
    if ( myArgs.empty() ) {
        return Argument();
    }
    else {
        Argument a(myArgs.front());
        myArgs.pop_front();
        return a;
    }
}
    

}; /* class CmdArguments */


typedef std::shared_ptr<CmdArguments> CmdArgumentsPtr;



#endif /* !defined CMD_ARGUMENTS_HPP */
