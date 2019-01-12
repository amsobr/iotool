#ifndef CONNECTED_TELNET_CLIENT_HPP
#define CONNECTED_TELNET_CLIENT_HPP

#include <Poco/Net/TCPServerConnection.h>

#include "telnet_stream_adapter.hpp"
#include "shell_frontend.hpp"
#include "shell_backend.hpp"

class ConnectedTelnetClient : public Poco::Net::TCPServerConnection
{
private:
    TelnetStreamAdapter myStream;
    ShellBackendPtr myShellBackend;

public:
    ConnectedTelnetClient( Poco::Net::StreamSocket skt , ShellBackendPtr shellBackend ) :
    Poco::Net::TCPServerConnection(skt) ,
    myStream(skt) ,
    myShellBackend(shellBackend)
    {
    }

    virtual ~ConnectedTelnetClient() {}

    virtual void run()
    {
        ShellFrontend *shellFrontend    = new ShellFrontend( &myStream,myShellBackend );
        shellFrontend->run();
    }

}; /* class ConnectedTelnetClient */




#endif /* ! CONNECTED_TELNET_CLIENT_HPP */