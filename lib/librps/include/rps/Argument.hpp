//
// Created by to on 03/03/21.
//

#pragma once

#include <string>

#include "Exception.hpp"

namespace rps
{

class Argument
{
public:
    explicit Argument(std::string token );

    ~Argument() = default;

    [[nodiscard]] bool isKvPair() const
    {
        return myIsKvPair;
    }
    
    /**
     * @brief Check if argument is a simple token
     *
     * Checks if an argument is not of the format `KEY=VALUE`, ie, if it can be
     * parsed as a single token as a whole.
     
     * This is equivalent to a call of `!arg.isKvPair()`
     *
     * @return true if the argument is a simple token, ie, not in the form
     *         `key=value`
     */
    [[nodiscard]] bool isToken() const
    {
        return !isKvPair();
    }


    /**
     * @brief get the argument as a string
     *
     * Checks if the argument is an actual token before returning.
     * @return The whole token. Throws an exception if the argument is of
     *          Key-Value type
     */
    [[nodiscard]] std::string const& getAsString() const
    {
        if ( isKvPair() ) {
            throw InvalidArgumentsException{"argument is KV (not a token): " + myKey};
        }
        return myToken;
    }

    [[nodiscard]] unsigned long long int getAsUnsigned() const
    {
        if ( isKvPair() ) {
            throw InvalidArgumentsException{"argument is KV (not an unsigned): " + myToken };
        }
        size_t cnt=0;
        auto v = std::stoull(myToken,&cnt);
        if ( cnt!=myToken.size() ) {
            throw InvalidArgumentsException{"argument is not an unsigned: " + myToken };
        }
        return v;
    }

    [[nodiscard]] long long int getAsInteger() const
    {
        if ( isKvPair() ) {
            throw InvalidArgumentsException{"argument is a KV (not an integer): " + myToken };
        }
        size_t cnt=0;
        auto v = std::stoll(myToken,&cnt);
        if ( cnt!=myToken.size() ) {
            throw InvalidArgumentsException{"argument is not a valid integer: " + myToken };
        }
        return v;
    }


    [[nodiscard]] double getAsDouble() const
    {
        if ( isKvPair() ) {
            throw InvalidArgumentsException{"argument is a KV (not a double): " + myToken };
        }
        size_t cnt=0;
        auto v = std::stod(myToken,&cnt);
        if ( cnt!=myToken.size() ) {
            throw InvalidArgumentsException{"argument is not a valid double: " + myToken };
        }
        return v;
    }


    [[nodiscard]] unsigned long long int getValueAsUnsigned() const
    {
        if ( !isKvPair() ) {
            throw InvalidArgumentsException{"argument is a KV (not an unsigned): " + myToken };
        }
        size_t cnt=0;
        auto v = std::stoull(myValue,&cnt);
        if ( cnt!=myValue.size() ) {
            throw InvalidArgumentsException{"argument is not a valid unsigned: " + myToken };
        }
        return v;
    }

    [[nodiscard]] long long int getValueAsInteger() const
    {
        if ( !isKvPair() ) {
            throw InvalidArgumentsException{"argument is a KV (not an integer): " + myToken };
        }
        size_t cnt=0;
        auto v = std::stoll(myValue,&cnt);
        if ( cnt!=myValue.size() ) {
            throw InvalidArgumentsException{"argument is not a valid integer: " + myToken };
        }
        return v;
    }


    [[nodiscard]] double getValueAsDouble() const
    {
        if ( !isKvPair() ) {
            throw InvalidArgumentsException{"argument is a KV (not a double): " + myToken };
        }
        size_t cnt=0;
        auto v = std::stod(myValue,&cnt);
        if ( cnt!=myValue.size() ) {
            throw InvalidArgumentsException{"argument is not a valid double: " + myToken };
        }
        return v;
    }



    [[nodiscard]] std::string const& getKey() const
    {
        return myKey;
    }

    [[nodiscard]] std::string const& getValue() const
    {
        return myValue;
    }

    [[nodiscard]] std::string const& getToken() const
    {
        return myToken;
    }


private:
    std::string myToken;
    std::string myKey;
    std::string myValue;
    bool        myIsKvPair;
};

} /* namespace Rpn */
