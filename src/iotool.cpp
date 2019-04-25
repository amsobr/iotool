
#include <iostream>
#include <string>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>


#include <cbsl/keyval.hpp>
#include <cbsl/strings.hpp>

#include <common/board.hpp>
#include <common/board_factory.hpp>
#include <common/applet_factory.hpp>

#include <shell/shell_backend.hpp>
#include <shell/shell_frontend.hpp>
#include <shell/stdio_stream_adapter.hpp>
#include <shell/shell_backend_factory.hpp>

#include "iotool_config.hpp"
#include "tcp_server.hpp" /* telnet server. could use a better name >.< */
#include "tcp_data_server.hpp"
#include "shell/connected_telnet_client_factory.hpp"
#include "csv_writer.hpp"
#include "output_channel_manager.hpp"
#include "io_core.hpp"

#include <Poco/FileChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/Logger.h>
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <common/output_channel.hpp>
#include <shell/shell_peripheral_provider.hpp>

using namespace std;
using namespace Poco;

void version()
{
    fprintf( stdout , "iotool version %s.%s.%s-%s\n" , VER_MAJOR,VER_MINOR,VER_PATCH,VER_EXTRA);
}

void help()
{
    static char const *msg =
    "Usage:\n"
    "  iotool --help | --version | --foreground | --daemon\n"
    "\n"
    "Available Options:\n"
    "  --help          Display this help message and exit.\n"
    "  --version       Display iotool's version and exit.\n"
    "  --foreground    Start local shell with STDIN/STDOUT\n"
    "  --daemon        Start the HW I/O server\n"
    ;

    version();
    fprintf( stdout , "%s\n" , msg );
}


/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char **argv)
{
    bool startShell=false;
    bool startServer=false;
    bool launchJobEngine=false;
    string jobEngineConfiguration;


    if ( argc<2 ) {
        fprintf( stderr , "invalid arguments.\n");
        return 1;        
    }

    for ( int i(1) ; i<argc ; i++ ) {
        cbsl::KeyVal::Pair  arg = cbsl::KeyVal::parseArg(argv[i]);

        if ( arg.isAnonymous() ) {
            if (arg.value == "--version") {
                version();
                return 0;
            } else if (arg.value == "--help") {
                help();
                return 0;
            } else if (arg.value == "--foreground") {
                startShell = true;
            } else if (arg.value == "--daemon") {
                startServer = true;
            } else {
                fprintf(stderr, "Unsupported argument: %s\n", argv[i]);
                return 1;
            }
        }
        else {
            if ( arg.key == "--job-config" ) {
                launchJobEngine = true;
                jobEngineConfiguration  = arg.value;
            }
            else {
                fprintf(stderr, "Unsupported argument: %s\n",argv[i]);
                return 1;
            }
        }
    }

    if ( startServer && startShell ) {
        fprintf( stderr , "Please invoke with only one of --daemon or --foreground options...\n" );
        return 1;
    }

    if ( launchJobEngine && !startServer ) {
        fprintf(stderr," --job-config requires --daemon\n");
        return 1;
    }

    /* Following code must be moved to some sort of board init module
     *
     * TODO: A SharedBus mechanism may provide concurrent access to the 
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


    if ( !startServer && !startShell ) {
        fprintf( stderr , "Nothing to do.\n" );
        return 0;
    }

    /* We don't care much about memory management for the loggers as, these will
     * live for the lifetime of the process...
     */
    FileChannel *logFile  = new FileChannel(Iotool::LOG_FILE);
    logFile->open();
    logFile->setProperty("rotation","5 M");
    logFile->setProperty("compress","true");
    logFile->setProperty("archive","number");
    logFile->setProperty("purgeCount","10");

    PatternFormatter *formatter   = new PatternFormatter("%Y/%m/%e %H:%M:%S.%F %p %d - %t");
    formatter->setProperty("times","local");

    FormattingChannel *fmtChannel = new FormattingChannel(formatter,logFile);

    Logger::create(Logger::ROOT,fmtChannel,Message::PRIO_INFORMATION);

    Logger &logger = Logger::get("iotool");
    logger.setLevel(Message::PRIO_DEBUG);

    logger.information( Poco::format("\n\n\n-------------------------------------\niotool-%s starting...\n-------------------------------------" , string(Iotool::VERSION)) );

    logger.information( "Creating board...");
    BoardPtr board  = createBoard();
    logger.information( "Creating device applets...");
    vector<DeviceAppletPtr> deviceApplets( createDeviceApplets() );
    logger.debug( "Creating system applets...");
    vector<SystemAppletPtr> systemApplets( createSystemApplets() );

    /* set up the shell engine */
    ShellPeripheralProviderPtr peripheralProviderPtr( new ShellPeripheralProvider() );
    peripheralProviderPtr->setDeviceApplets(deviceApplets);
    peripheralProviderPtr->setPeripherals(board->getPeripherals());
    peripheralProviderPtr->rebuildIndex();

    ShellBackendFactory::addProvider(peripheralProviderPtr);


    IoCore *ioCore(nullptr);
    if ( launchJobEngine ) {
        ioCore  = new IoCore(jobEngineConfiguration);
        ioCore->start();
    }

    
    if ( startServer ) {
        TcpDataServer *dataServer = new TcpDataServer(Iotool::TCP_DATA_SERVER_PORT);
        dataServer->start();
        Poco::Net::TCPServer *tcpServer;
        ShellBackendPtr shellBackend    = ShellBackendFactory::createInstance();
        Poco::Net::TCPServerConnectionFactory::Ptr connectionFactory(new ConnectedTelnetClientFactory(shellBackend,dataServer));
        Poco::Net::ServerSocket srvSkt;
        logger.information( Poco::format("telnet server: binding to 0.0.0.0:%u...",Iotool::TCP_LISTEN_PORT) );
        srvSkt.bind( Poco::Net::SocketAddress("0.0.0.0",Iotool::TCP_LISTEN_PORT),true,true);
        logger.information( "telnet server: entering LISTENING state..." );
        srvSkt.listen();
        logger.information( "telnet server: launching server..." );
        tcpServer  = new Poco::Net::TCPServer(connectionFactory,srvSkt);
        tcpServer->start();

        /* once the server is started, we can hang forever.
        Just don't burn the CPU in the process
        */
        while(true) {
            sleep(5);
        }
    }



    
    if ( startShell ) {
        logger.debug( "Starting shell...");
        StdioStreamAdapter *iostreams   = new StdioStreamAdapter();
        ShellBackendPtr shellBackend    = ShellBackendFactory::createInstance();
        ShellFrontend *shellFrontend    = new ShellFrontend(iostreams, shellBackend);
        shellFrontend->run();
        delete shellFrontend;
        delete iostreams;
        logger.information( "Returned from shell. Moving on...");
    }

    return 0;
}
