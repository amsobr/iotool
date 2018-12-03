
#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <cbsl/keyval.hpp>
#include <cbsl/strings.hpp>

#include <common/board.hpp>
#include <common/board_factory.hpp>
#include <common/applet_factory.hpp>

#include <shell/shell_backend.hpp>
#include <shell/shell_frontend.hpp>

#include "data_sender.hpp"


using namespace std;


void exitWithUsage( int exitCode=0 , string message="" )
{
    ostream &ss = message.empty() ? cout : cerr;

    if ( !message.empty() )
    {
        ss << message << "\n";
    }
    ss << "Usage:\n"
    << "ads126x-util spidev=ARG ch=ARG [count=ARG] help\n"
    << "ads126x-util help\n"
    << "\n"
    << "Arguments:\n"
    << "spidev        SPI bus device (e.g. /dev/spidev.0.1)\n"
    << "ch            ADC channel to operate on. From 0 to 9\n"
    << "count         Number of reads to perform. Default is1\n"
    ;
    exit( exitCode );
}


int main(int /*argc*/, char ** /*argv*/)
{

    /* Following code must be moved to some sort of board init module
     *
     * A SharedBus mechanism may provide concurrent access to the 
     * multiple buses usually found on a board: SPI, I2C, RS232, etc.
     * SharedBus may provide  a factory which is used to get
     * ScopedBus objects. ScopedBus objects are neither copyable not
     * movable. The bus they relate to may be locked until the ScopedBus
     * object is destroyed.
     * To accelerate the access to scoped buses, consumers may keep an
     * opaque token that is used to request the ScopedBus from the factory.
     * 
     * Board support libraries build the ScopedBus factories, by registering
     * the corresponding drivers for SPI, I2C, etc...
     * They also create instances of the present device drivers, providing the
     * latter with the tokens which give them access to the propper shared
     * buses.
     * Each peripheral (or instance of the same type) may provide its own
     * set of commands. Commands may be implemented per peripheral class,
     * and the board support lib could handle multiplexing of configs and 
     * commands to execute.
     * 
     * The application's main task collects all commands from the BSL, and
     * feeds them into the ShellEngine.
     * The appropriate ShellFrontends are created as needed.
     * Examples of shell frontends:
     *  - stdin/sterr/stdout directly accessible in invocation of the app
     *  - a telnet session accessible over the network
     *  - a TCP socket used to communicate with external apps....
     */

    BoardPtr board  = createBoard();
    //cerr << "Current Board configutation:\n" << *(board->get()) << "\n";
    vector<DeviceAppletPtr> deviceApplets( createDeviceApplets() );
    vector<SystemAppletPtr> systemApplets( createSystemApplets() );

    ShellBackendPtr shellBackend    = make_shared<ShellBackend>();
    shellBackend->setPeripherals( board->getPeripherals() );
    shellBackend->setSystemApplets( systemApplets );
    shellBackend->setDeviceApplets( deviceApplets );
    shellBackend->rebuildIndex();



    cout << "Creating engine...\n";
    cout << "Setting up shell frontend...\n";
    //ShellFrontendPtr frontend( new ShellFrontend(cin,cerr,cout,shellBackend) );
    cout << "Launching shell frontend...\n";
    //frontend->start();
    DataSender sender( board , 3123 , 60 );
    sender.start();
    cout << "Waiting for shell to terminate...\n";
    //frontend->join();

    while(true)
        sleep(10);


    sender.stop();
    cout << "Shell terminated. Exit.\n";
    return 0;
}
