#ifndef CMD_ARGUMENTS_HPP
#define CMD_ARGUMENTS_HPP

#include <memory>
#include <list>
#include <algorithm>
#include <string>
#include <sstream>

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

    operator std::string() const
    {
        return toString();
    }

    std::string toString() const
    {
        if ( isToken() ) {
            return token();
        }
        return pair.key + "=" + pair.value;
    }
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

CmdArguments( std::list<std::string> tokens )
{
    for ( std::string &tok : tokens ) {
        cbsl::KeyVal::Pair pair;
        cbsl::KeyVal::parseArg(tok.c_str(),&pair);
        addArg(pair);
    }
}

~CmdArguments() {
    myArgs.clear();
}

void loadFrom( std::string line )
{
    myArgs.clear();
    std::istringstream is(line);
    std::string t;
    while ( std::getline(is, t,' ') ) {
        cbsl::KeyVal::Pair pair;
        cbsl::KeyVal::parseArg(t.c_str(),&pair);
        addArg(pair);
    }
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

/**
 * \brief Check if the list of arguments is empty
 * \return True if the list if empty. False otherwise
 */
bool empty() const
{
    return myArgs.empty();
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

bool shiftToken( std::string *dest )
{
    if ( myArgs.empty() ) {
        return false;
    }
    else {
        Argument a(myArgs.front());
        myArgs.pop_front();
        if ( a.isToken() ) {
            dest    = a.token();
            return true;
        }
    }
    return false;
}

bool shiftKeyVal( std::string &key , std::string &val )
{
    if ( myArgs.empty() ) {
        return false;
    }
    else {
        Argument a(myArgs.front());
        myArgs.pop_front();
        if ( !a.isToken() ) {
            key = a.name();
            val = a.value();
            return true;
        }
    }
    return false;
}



}; /* class CmdArguments */


typedef std::shared_ptr<CmdArguments> CmdArgumentsPtr;



#endif /* !defined CMD_ARGUMENTS_HPP */
