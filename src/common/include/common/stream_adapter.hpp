#ifndef STREAM_ADAPTER_HPP
#define STREAM_ADAPTER_HPP

#include <string>
#include <exception>

class StreamDisconnectedException : public std::exception
{
    /*
public:
    StreamDisconnectedException() : std::exception()
    {
    }

    virtual ~StreamDisconnectedException() {}
    */

}; /* class StreamDisconnectedException */



class StreamAdapter
{
public:
    StreamAdapter() {}
    virtual ~StreamAdapter() {}


    virtual std::string readLine() = 0;
    virtual char readChar() = 0;
    virtual void write(std::string const &s) = 0;
    virtual void putc(char c) = 0;

}; /* class StreamAdapter */





#endif /* STREAM_ADAPTER_HPP */
