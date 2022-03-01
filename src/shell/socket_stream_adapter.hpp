#ifndef STREAM_SOCKET_ADAPTER_HPP
#define STREAM_SOCKET_ADAPTER_HPP

#include <string>

#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>

#include <common/StreamAdapter.hpp>

class SocketStreamAdapter : public StreamAdapter
{
private:
    Poco::Net::SocketStream myStream;
public:
    SocketStreamAdapter( Poco::Net::StreamSocket stream ) :
    myStream(stream)
    {
    }

    virtual ~SocketStreamAdapter() {}

    virtual std::string readLine()
    {
        std::string s;
        std::getline(myStream,s);
        return s;
    }

    virtual char readChar()
    {
        char c;
        myStream.read(&c,1);
        return c;
    }

    virtual void write(std::string const &s)
    {
        myStream << s;
    }

    virtual void putc(char c)
    {
        myStream.write(&c,1);
    }

}; /* class SocketStreamAdapter */


#endif /* ! defined STREAM_SOCKET_ADAPTER_HPP */
