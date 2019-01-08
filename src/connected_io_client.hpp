#ifndef CONNECTED_IO_CLIENT_HPP
#define CONNECTED_IO_CLIENT_HPP

#include <string>

#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/DialogSocket.h>

class ConnectedIoClient
{
private:
    Poco::Net::DialogSocket mySocket;
    bool myIsActive;
    bool myIsFinished;
    std::string myClientName;

public:
    ConnectedIoClient(Poco::Net::StreamSocket &connection);
    virtual ~ConnectedIoClient();

    bool isFinished() const;


}; /* class ConnectedIoClient */



#endif /* !defined CONNECTED_IO_CLIENT_HPP */
