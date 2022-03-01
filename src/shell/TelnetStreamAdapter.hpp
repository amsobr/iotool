
#pragma once

#include <string>

#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/DialogSocket.h>

#include <rps/StreamAdapter.hpp>

class TelnetStreamAdapter : public rps::StreamAdapter
{
public:
    explicit TelnetStreamAdapter( Poco::Net::StreamSocket& stream ) :
    myStream{ stream }
    {
    }

    ~TelnetStreamAdapter() override;

    std::string readLine() override
    {
        std::string s;
        myStream.receiveMessage(s);
        return s;
    }

    char readChar() override
    {
        return (char) myStream.get();
    }

    void writeLine(std::string const &s) override
    {
        myStream.sendMessage(s);
    }

    void putc(char c) override
    {
        myStream.sendByte(c);
    }

private:
    Poco::Net::DialogSocket myStream;

}; /* class TelnetStreamAdapter */


