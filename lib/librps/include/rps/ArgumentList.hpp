//
// Created by to on 03/03/21.
//

#pragma once

#include <vector>

#include "Argument.hpp"

namespace rps
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

    /* default move assignment and construction are suitable */
    ArgumentList( ArgumentList const& other ) = default;
    ArgumentList( ArgumentList&& other ) = default;
    ArgumentList& operator=( ArgumentList&& rhs )   = default;


    ~ArgumentList() = default;

    [[nodiscard]] size_t size() const
    {
        return myArgs.size();
    }

    [[nodiscard]] Argument const& at(int pos) const
    {
        return myArgs.at(pos);
    }

    [[nodiscard]] Argument const& get( std::string const& key ) const
    {
        for ( auto const& arg : myArgs ) {
            if ( arg.getKey()==key ) {
                return arg;
            }
        }
        throw NotFoundException("argument not found: " + key );
    }

    [[nodiscard]] bool has( std::string const& key ) const
    {
        for ( auto const& arg : myArgs ) {
            if ( arg.getKey()==key || arg.getToken()==key ) {
                return true;
            }
        }
        return false;
    }
    
    [[nodiscard]] bool isEmpty() const
    {
        return myArgs.empty();
    }
    
    [[nodiscard]] auto begin() const
    {
        return myArgs.begin();
    }
    
    [[nodiscard]] auto end() const
    {
        return myArgs.end();
    }
    
    [[nodiscard]] auto cbegin() const
    {
        return myArgs.cbegin();
    }
    
    [[nodiscard]] auto cend() const
    {
        return myArgs.cend();
    }

private:
    std::vector<Argument> myArgs;
};

} /* namespace rps */

