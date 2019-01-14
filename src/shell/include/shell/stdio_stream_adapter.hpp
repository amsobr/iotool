#ifndef STDIO_STREAM_ADAPTER_HPP
#define STDIO_STREAM_ADAPTER_HPP

#include <iostream>

#include <common/stream_adapter.hpp>

class StdioStreamAdapter : public StreamAdapter
{
public:
    StdioStreamAdapter() { };

    virtual ~StdioStreamAdapter() {}

    virtual std::string readLine() // throw ( StreamDisconnectedException )
    {
        std::string s;
        std::getline(std::cin,s);
        return s;
    }

    virtual char readChar() // throw ( StreamDisconnectedException )
    {
        char c;
        std::cin.read(&c,1);
        return c;
    }

    virtual void writeLine(std::string const &s) // throw ( StreamDisconnectedException )
    {
        std::cout << s << "\n";
    }


    virtual void putc(char c) // throw ( StreamDisconnectedException ) = 0;
    {
        std::cout.write(&c,1);
    }

};


#endif /* !defomed IOSTREAM_ADAPTER */
