
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

#include <shell-common/shell_engine.hpp>
#include <shell-common/shell_frontend.hpp>

#include <drivers/ads126x_config.hpp>
#include <hal/adc.hpp>
#include <drivers/ads126x.hpp>
#include <drivers/cmd_ads126x_read.hpp>
#include <drivers/cmd_ads126x_cal.hpp>
#include <applets/cmd_exit.hpp>


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
    Ads126xConfig adcConfig;
    adcConfig.spiDevice = "/dev/spidev0.0";
    adcConfig.inpGain   = { 
        1, /* AIN1 to AIN6 - gain=1.0 (direct connection to ADC) */
        1,
        1,
        1,
        1,
        1,
        0.5, /* AIN7 to AIN10 - gain=0.5 (buffer + 100k/100k) */
        0.5,
        0.5,
        0.5
        };
    cout << "Creating ADC...\n";
    Ads126xPtr adc(new Ads126x("adc0" ,adcConfig));

    cout << "Loading commands...\n";
    std::list<CmdHandlerPtr> allCmds;
    allCmds.push_back( CmdHandlerPtr(new CmdExit()) );
    allCmds.push_back( CmdHandlerPtr(new CmdAds126xRead(adc)) );
    allCmds.push_back( CmdHandlerPtr(new CmdAds126xCal(adc)));

    cout << "Creating engine...\n";
    ShellEnginePtr engine(new ShellEngine(allCmds));
    cout << "Setting up shell frontend...\n";
    ShellFrontendPtr frontend( new ShellFrontend(cin,cerr,cout,engine) );
    cout << "Launching shell frontend...\n";
    frontend->start();
    cout << "Waiting for shell to terminate...\n";
    frontend->join();
    cout << "Shell terminated. Exit.\n";
    return 0;
}
