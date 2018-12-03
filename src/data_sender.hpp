#ifndef DATA_SENDER_HPP
#define DATA_SENDER_HPP

#include <cbsl/timer.hpp>
#include <cbsl/net/udp_skt.hpp>

#include <common/board.hpp>

class DataSender
{
private:
    BoardPtr myBoard;
    unsigned int myDestPort;
    unsigned int mysendInterval;
    cbsl::Timer     myTimer;
    cbsl::UdpSkt *mySocket;

    static void timerCallback(void*);


public:
    DataSender( BoardPtr board , unsigned int port , unsigned int interval );
    ~DataSender();

    void iterate();
    void start();
    void stop();

}; /* class DataSender */



#endif /* !defined DATA_SENDER_HPP */
