//
// Created by to on 03/03/21.
//

#ifndef IOTOOL_ARGUMENTLIST_H
#define IOTOOL_ARGUMENTLIST_H

#include <vector>

#include "Argument.h"

namespace Rpn
{

class ArgumentList
{
public:
    ArgumentList() = default;

    explicit ArgumentList( std::vector<std::string> const& tokens )
    {
        myArgs.reserve(tokens.size());
        for ( auto const& t : tokens ) {
            myArgs.emplace_back(t);
        }
    }

    virtual ~ArgumentList() = default;

    [[nodiscard]] size_t size() const
    {
        return myArgs.size();
    }

    [[nodiscard]] Argument const& at(int pos) const
    {
        return myArgs.at(pos);
    }

    [[nodiscard]] Argument const& get( std::string const& key )
    {
        for ( auto const& arg : myArgs ) {
            if ( arg.getKey()==key ) {
                return arg;
            }
        }
        throw ErrNotFound( "argument not found: " + key );
    }

    [[nodiscard]] bool hasKey( std::string const& key )
    {
        for ( auto const& arg : myArgs ) {
            if ( arg.getKey()==key ) {
                return true;
            }
        }
        return false;
    }

private:
    std::vector<Argument> myArgs;
};

} /* namespace Rpn */

#endif //IOTOOL_ARGUMENTLIST_H
