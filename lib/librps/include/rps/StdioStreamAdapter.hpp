
#pragma once

#include <iostream>

#include "NoCopy.hpp"
#include "Exception.hpp"
#include "StreamAdapter.hpp"

namespace rps
{


class StdioStreamAdapter : public StreamAdapter
{
public:
    NO_COPY(StdioStreamAdapter)

    StdioStreamAdapter() = default;

    ~StdioStreamAdapter() override = default;

    std::string readLine() override
    {
        std::string s;
        std::getline(std::cin,s);
        return s;
    }
    
    [[nodiscard]] bool isEof() const override
    {
        return std::cin.eof();
    }
    
    char readChar() override
    {
        char c;
        std::cin.read(&c,1);
        return c;
    }

    void writeLine(std::string const &s) override
    {
        std::cout << s << "\n";
    }
    
    void write(std::string const &s) override
    {
        std::cout << s;
    }


    void putc(char c) override
    {
        std::cout.write(&c,1);
    }

};

} /* namespace rps */
