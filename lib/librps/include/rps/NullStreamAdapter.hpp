#pragma once

#include <string>

#include "StreamAdapter.hpp"

namespace rps
{

class NullStreamAdapter : public StreamAdapter
{
public:
    NullStreamAdapter() = default;

    ~NullStreamAdapter() override = default;

    std::string readLine() override
    {
        return "";
    }

    char readChar() override
    {
        return 0;
    }

//    void write(std::string const &s) override
//    {
//    }

    void putc(char c) override
    {
    }

}; /* class NullStreamAdapter */


} /* namespace rps */
