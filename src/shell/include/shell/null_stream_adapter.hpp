#ifndef NULL_STREAM_ADAPTER_HPP
#define NULL_STREAM_ADAPTER_HPP

#include <string>

#include <common/stream_adapter.hpp>

class NullStreamAdapter : public StreamAdapter
{
public:
    NullStreamAdapter() { }

    virtual ~NullStreamAdapter() {}

    virtual std::string readLine()
    {
        return "";
    }

    virtual char readChar()
    {
        return 0;
    }

    virtual void write(std::string const &s)
    {
    }

    virtual void putc(char c)
    {
    }

}; /* class NullStreamAdapter */


#endif /* ! defined NULL_STREAM_ADAPTER_HPP */