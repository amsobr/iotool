#ifndef TELNET_STREAM_ADAPTER_HPP
#define STREAM_SOCKET_ADAPTER_HPP

#include <string>

#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/DialogSocket.h>

#include <common/stream_adapter.hpp>

class TelnetStreamAdapter : public StreamAdapter
{
private:
    Poco::Net::DialogSocket myStream;
public:
    TelnetStreamAdapter( Poco::Net::StreamSocket stream ) :
    myStream(stream)
    {
    }

    virtual ~TelnetStreamAdapter() {}

    virtual std::string readLine()
    {
        std::string s;
        myStream.receiveMessage(s);
        return s;
    }

    virtual char readChar()
    {
        return (char) myStream.get();
    }

    virtual void writeLine(std::string const &s)
    {
        myStream.sendMessage(s);
    }

    virtual void putc(char c)
    {
        myStream.sendByte(c);
    }

}; /* class TelnetStreamAdapter */


#endif /* ! defined STREAM_SOCKET_ADAPTER_HPP */