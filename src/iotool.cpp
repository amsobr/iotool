
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <csignal>


#include <Poco/FileChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/Logger.h>
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/String.h>

#include <rps/StreamAdapter.hpp>
#include <rps/StdioStreamAdapter.hpp>
#include <rps/ContextFactory.hpp>
#include <rps/BuiltinCommands.hpp>

#include <common/Board.hpp>

#include "shell/ShellFrontend.hpp"
#include "shell/PeripheralCmdProvider.hpp"
#include "shell/TelnetSessionFactory.hpp"
#include "shell/commands/CmdCd.hpp"
#include "shell/commands/CmdHelp.hpp"
#include "shell/commands/CmdShow.hpp"


#include "iotool_config.hpp"



using namespace std;
using namespace Poco;


static inline std::string pathForPeripheral( PeripheralPtr const& p )
{
    return Poco::format("/%s/%d",Poco::toLower(str(p->getType())),p->getId());
}

static inline std::string pathForPeripheralType( PeripheralType t )
{
    return Poco::format("/%s",Poco::toLower(str(t)));
}

void version()
{
    fprintf( stdout , "iotool version %s\n" , IOTOOL_VERSION );
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


void signalHandler( int sig )
{
    std::cerr << "Caught signal: " << sig << "\n";
    exit(0);
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


    if ( argc<2 ) {
        fprintf( stderr , "invalid arguments.\n");
        return 1;        
    }

    for ( int i(1) ; i<argc ; i++ ) {
        std::string arg{ argv[i] };

        if ( arg=="--version" ) {
            version();
            return 0;
        }
        else if ( arg=="--help" ) {
            help();
            return 0;
        }
        else if ( arg=="--foreground" ) {
            startShell = true;
        }
        else if ( arg=="--daemon" ) {
            startServer = true;
        }
        else {
            cerr << "Unsupported argument: " << arg << "\n";
            return 1;
        }
    }

    if ( startServer && startShell ) {
        fprintf( stderr , "Please invoke with only one of --daemon or --foreground options...\n" );
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
    
    
    struct sigaction sa{};
    sa.sa_handler   = &signalHandler;
    sigaction(SIGINT,&sa,nullptr);
    sigaction(SIGTERM,&sa,nullptr);
    sigaction(SIGHUP,&sa,nullptr);


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
    Board::create();
    BoardPtr board  = Board::get();

    PeripheralCmdProvider perCmdIndex;

    auto cmdTree{make_shared<rps::CommandTree>() };
    for ( auto const& p : board->getPeripherals() ) {
        rps::CliPath path { pathForPeripheral(p) };
        cmdTree->addPath(path );
        p->setAlias( path.toString() );
    }

    for ( auto [ type , cmds ] : perCmdIndex.groups ) {
        auto decoder    = make_shared<rps::Decoder>();
        for_each( cmds.begin() , cmds.end() , [&decoder](auto const& cmd) {
            decoder->addCommand(cmd);
        } );
        rps::CliPath path { pathForPeripheralType(type) };
        cmdTree->addDecoder(path, decoder);
    }

    auto builtinsDecoder    = make_shared<rps::Decoder>();
    auto builtins           = rps::getBuiltinCommands();
    std::for_each(builtins.begin(),builtins.end(),[&builtinsDecoder](auto const&c) {
        builtinsDecoder->addCommand(c);
    } );
    cmdTree->addDecoder(rps::CliPath{"/"}, builtinsDecoder);
    
    auto cmdCd  = make_shared<CmdCd>();
    cmdTree->addCommand("/",cmdCd);
    auto cmdHelp= make_shared<CmdHelp>(cmdTree,perCmdIndex);
    cmdTree->addCommand("/",cmdHelp);
    
    auto cmdShow= make_shared<CmdShow>();
    cmdTree->addCommand("/",cmdShow);
    
    /* Improve set up of command tree.
     * Dedicated module, "CreateCommandTree", creates the CmdTreePtr,
     * including builtins, and custom commands.
     */
    
    auto ctxFactory{ make_shared<rps::ContextFactory>(cmdTree) };


    if ( startServer ) {
        Poco::Net::TCPServer *tcpServer;
        Poco::Net::TCPServerConnectionFactory::Ptr connectionFactory{
            new TelnetSessionFactory(ctxFactory)
        };
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
        auto stream = make_shared<rps::StdioStreamAdapter>();
        auto ctx    = ctxFactory->create(stream);
        auto consoleFrontend  = make_unique<ShellFrontend>(ctx);
        consoleFrontend->run();
        logger.information( "Returned from shell. Moving on...");
    }

    return 0;
}
