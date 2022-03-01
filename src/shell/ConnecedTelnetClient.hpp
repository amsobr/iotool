
#pragma once

#include <Poco/Net/TCPServerConnection.h>

#include <rps/ContextFactory.hpp>

#include "TelnetStreamAdapter.hpp"
#include "ShellFrontend.hpp"

class ConnectedTelnetClient : public Poco::Net::TCPServerConnection
{
public:
    explicit ConnectedTelnetClient( Poco::Net::StreamSocket skt, rps::ContextFactoryPtr const& ctxFactory  ) :
    Poco::Net::TCPServerConnection{ skt }
    {
        auto stream = std::make_shared<TelnetStreamAdapter>(skt);
        auto ctx    = ctxFactory->create(stream);
        myFrontend  = std::make_shared<ShellFrontend>(ctx);
    }

    ~ConnectedTelnetClient() override   = default;

    void run() override
    {
        myFrontend->run();
    }

private:
    ShellFrontendPtr myFrontend;
}; /* class ConnectedTelnetClient */


