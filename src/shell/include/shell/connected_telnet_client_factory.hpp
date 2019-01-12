#ifndef CONNECTED_TELNET_CLIENT_FACTORY_HPP
#define CONNECTED_TELNET_CLIENT_FACTORY_HPP

#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Net/StreamSocket.h>

#include "shell_frontend.hpp"
#include "shell_backend.hpp"
#include "connected_telnet_client.hpp"


class ConnectedTelnetClientFactory: public Poco::Net::TCPServerConnectionFactory
{
private:
    ShellBackendPtr myShellBackend;
    unsigned int createdClients;

public:
    ConnectedTelnetClientFactory( ShellBackendPtr bknd ) :
    myShellBackend(bknd) ,
    createdClients(0)
    {
    }

    ~ConnectedTelnetClientFactory()
    {
    }

    virtual Poco::Net::TCPServerConnection *createConnection( Poco::Net::StreamSocket const &socket )
    {
        return new ConnectedTelnetClient(socket,myShellBackend);
    }

}; /* class ConnectedTerlnetClientFactory */



#endif /* !defined CONNECTED_TELNET_CLIENT_FACTORY_HPP */