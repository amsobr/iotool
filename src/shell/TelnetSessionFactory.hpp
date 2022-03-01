//
// Created by to on 22/02/22.
//


#pragma once

#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Net/TCPServerConnection.h>

#include <rps/ContextFactory.hpp>

#include "ConnecedTelnetClient.hpp"

class TelnetSessionFactory : public Poco::Net::TCPServerConnectionFactory
{
public:
    explicit TelnetSessionFactory( rps::ContextFactoryPtr ctxFactory) :
    myCtxFactory{ std::move(ctxFactory) }
    {
    }
    
    ~TelnetSessionFactory() override    = default;
    
    
    Poco::Net::TCPServerConnection*
    createConnection(Poco::Net::StreamSocket const& socket) override
    {
        return new ConnectedTelnetClient(socket,myCtxFactory);
    }

private:
    rps::ContextFactoryPtr myCtxFactory;
};



