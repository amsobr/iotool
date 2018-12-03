
#include <string>
#include <iostream>

#include <cbsl/timer.hpp>
#include <cbsl/net/udp_skt.hpp>

#include <common/board.hpp>
#include <common/adc.hpp>
#include "data_sender.hpp"

using namespace std;

void DataSender::timerCallback(void *p)
{
    DataSender *self    = (DataSender*)p;
    self->iterate();
}

DataSender::DataSender( BoardPtr board , unsigned int port , unsigned int interval ) :
myBoard(board ) ,
myDestPort(port) ,
myTimer(&timerCallback,this)
{
    myTimer.setOneShot(false);
    myTimer.setInterval(interval*1000);

    cbsl::UdpSkt::Config udpConfig;
    snprintf( udpConfig.boundIface , sizeof(udpConfig.boundIface) , "lo" );
    udpConfig.boundPort = 6969;
    mySocket    = new cbsl::UdpSkt(&udpConfig);
    mySocket->init();
}

DataSender::~DataSender()
{
    myTimer.stop();
    delete mySocket;
}

void DataSender::iterate()
{
    string msg  = "{";

    int entryCount = 0;
    for ( PeripheralPtr p : myBoard->getPeripherals() ) {
        if ( p->getType()==PeripheralType::ADC ) {
            AdcPtr adc  = dynamic_pointer_cast<Adc>(p);
            size_t numChannels  = adc->getNumChannels();
            for ( size_t ch=0 ; ch<numChannels ; ch++ ) {
                double v    = adc->readAnalog(ch);
                char str[128];
                snprintf( str , sizeof(str) , "%s    \"adc%d.%d\" : %.8f " , entryCount==0?"":",\n" , adc->getId() , ch , v );
                msg += str;
                entryCount++;
            }
        }
    }
    msg += "\n}";

    cout << "Sending message:\n" << msg << "\n";


    mySocket->sendMsg(0x7f000001,myDestPort,msg.c_str(),msg.size() );

}

    
void DataSender::start()
{
    myTimer.start();
}

void DataSender::stop()
{
    myTimer.stop();
}

