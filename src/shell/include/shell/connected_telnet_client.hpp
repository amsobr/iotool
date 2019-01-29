#ifndef CONNECTED_TELNET_CLIENT_HPP
#define CONNECTED_TELNET_CLIENT_HPP

#include <Poco/Net/TCPServerConnection.h>

#include <common/data_bucket_consumer.hpp>

#include "telnet_stream_adapter.hpp"
#include "shell_frontend.hpp"
#include "shell_backend.hpp"

class ConnectedTelnetClient : public Poco::Net::TCPServerConnection
{
private:
    TelnetStreamAdapter myStream;
    ShellBackendPtr myShellBackend;
    DataBucketConsumer *myDataConsumer;

public:
    ConnectedTelnetClient( Poco::Net::StreamSocket skt , ShellBackendPtr shellBackend , DataBucketConsumer *consumer ) :
    Poco::Net::TCPServerConnection(skt) ,
    myStream(skt) ,
    myShellBackend(shellBackend) ,
    myDataConsumer(consumer)
    {
    }

    virtual ~ConnectedTelnetClient() {}

    virtual void run()
    {
        ShellFrontend *shellFrontend    = new ShellFrontend( &myStream,myShellBackend,myDataConsumer );
        shellFrontend->run();
    }

}; /* class ConnectedTelnetClient */




#endif /* ! CONNECTED_TELNET_CLIENT_HPP */