//
// Created by to on 03/03/21.
//

#ifndef IOTOOL_ARGUMENT_H
#define IOTOOL_ARGUMENT_H

#include <string>

#include "exception.hpp"

namespace Rpn
{

class Argument
{
public:
    explicit Argument( std::string const& token );

    ~Argument() = default;

    [[nodiscard]] bool isKvPair() const
    {
        return myIsKvPair;
    }


    [[nodiscard]] std::string const& getAsString() const
    {
        if ( isKvPair() ) {
            throw ErrInvalidArguments{};
        }
        return myKey;
    }

    [[nodiscard]] unsigned long long int getAsUnsigned() const
    {
        if ( isKvPair() ) {
            throw ErrInvalidArguments{};
        }
        size_t cnt=0;
        auto v = std::stoull(myKey,&cnt);
        if ( cnt!=myKey.size() ) {
            throw ErrInvalidArguments{};
        }
        return v;
    }

    [[nodiscard]] long long int getAsInteger() const
    {
        if ( isKvPair() ) {
            throw ErrInvalidArguments{};
        }
        size_t cnt=0;
        auto v = std::stoll(myKey,&cnt);
        if ( cnt!=myKey.size() ) {
            throw ErrInvalidArguments{};
        }
        return v;
    }


    [[nodiscard]] double getAsDouble() const
    {
        if ( isKvPair() ) {
            throw ErrInvalidArguments{};
        }
        size_t cnt=0;
        auto v = std::stod(myKey,&cnt);
        if ( cnt!=myKey.size() ) {
            throw ErrInvalidArguments{};
        }
        return v;
    }


    [[nodiscard]] unsigned long long int getValueAsUnsigned() const
    {
        if ( !isKvPair() ) {
            throw ErrInvalidArguments{};
        }
        size_t cnt=0;
        auto v = std::stoull(myValue,&cnt);
        if ( cnt!=myValue.size() ) {
            throw ErrInvalidArguments{};
        }
        return v;
    }

    [[nodiscard]] long long int getValueAsInteger() const
    {
        if ( !isKvPair() ) {
            throw ErrInvalidArguments{};
        }
        size_t cnt=0;
        auto v = std::stoll(myValue,&cnt);
        if ( cnt!=myValue.size() ) {
            throw ErrInvalidArguments{};
        }
        return v;
    }


    [[nodiscard]] double getValueAsDouble() const
    {
        if ( !isKvPair() ) {
            throw ErrInvalidArguments{};
        }
        size_t cnt=0;
        auto v = std::stod(myValue,&cnt);
        if ( cnt!=myValue.size() ) {
            throw ErrInvalidArguments{};
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


private:
    std::string myKey;
    std::string myValue;
    bool        myIsKvPair;
};

} /* namespace Rpn */


#endif //IOTOOL_ARGUMENT_H
