
#pragma once

#include <string>

#include <Poco/Logger.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/DialogSocket.h>

#include <rps/StreamAdapter.hpp>

class TelnetStreamAdapter : public rps::StreamAdapter
{
public:
    explicit TelnetStreamAdapter( Poco::Net::StreamSocket& stream ) :
    myStream{ stream } ,
    myEof{ false }
    {
    }

    ~TelnetStreamAdapter() override;

    std::string readLine() override
    {
        std::string s;
        try {
            myStream.receiveMessage(s);
        }
        catch ( Poco::Exception const& e ) {
            auto& logger    = Poco::Logger::get("telnet");
            poco_notice(logger,Poco::format("exception reading from socket: code=%d msg='%s'",e.code(),e.displayText()));
            myEof           = true;
        }
        return s;
    }
    
    [[nodiscard]] bool isEof() const override
    {
        return myEof;
    }
    
    char readChar() override
    {
        return (char) myStream.get();
    }

    void writeLine(std::string const &s) override
    {
        myStream.sendMessage(s);
    }

    void write(std::string const &s) override
    {
        myStream.sendString(s);
    }

    void putc(char c) override
    {
        myStream.sendByte(c);
    }

private:
    Poco::Net::DialogSocket myStream;
    bool                    myEof;

}; /* class TelnetStreamAdapter */


