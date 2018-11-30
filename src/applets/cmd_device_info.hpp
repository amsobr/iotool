#ifndef CMD_DEVICE_INFO_HPP
#define CMD_DEVICE_INGO_HPP

#include <string>
#include <memory>

#include <shell-common/cmd_handler.hpp>
#include <shell-common/cmd_result.hpp>
#include <shell-common/cmd_arguments.hpp>


#include <common/peripheral.hpp>

class CmdDeviceInfo : public SystemApplet
{
private:
    std::string myHelp;


private:
    std::string getPeripheralBlock( PeripheralPtr p )
    {
        return "device         : " " Class=" + peripheralTypeName(p->getType()) + "Id=" + std::to_string(p->getId()) + "\n" +
               "vendor         : " + p->getVendor() + "\n" +
               "model (rev)    : " + p->getModel() + " (rev. " + p->getRevision() + ")\n" +
               "driver version : " + p->getDriverVersion() + "\n" +
               "driver author  : " + p->getAuthor() + "\n\n"
            ;
    }

public:
    CmdDeviceInfo():
    CmdHandler("devinfo" , "Shows information about a device" )
    {
        myHelp  = 
        "Usage: \n"
        "dev-info list        Show all known devices\n"
        "dev-info device=ARG  Show details for one device\n"
        "\nArguments\n"
        "device               Name of the device to show. Can be the name\n"
        "                     of one device (listed with 'dev-info list'\n"
        "                     or '*' to show details of all known devices\n"
        ;
    }

    ~CmdDeviceInfo()
    {
    }


    virtual std::string help() const
    {
        return myHelp;
    }


    virtual CmdResult execute( CmdArguments const &args )
    {
        if ( args.size()!=1 ) {
            return CmdResult(1,"Invalid number of parameters.");
        }
        std::string msg;

        return CmdResult(1,"devinfo::execute() <-- please implement me");
#if 0
        if ( args.hasArg("list") ) {
            std::string msg;
            for ( PeripheralPtr p : myPeripherals ) {
                msg += std::string("Class=") + peripheralTypeName(p->getType()) + " Id=" + std::to_string(p->getId()) + "\n";
            }
            return CmdResult(0,msg+"\n");
        }
        else if ( args.hasArg("device") ) {
            #if 0
            std::string v   = args.getValue("device");
            if ( v=="*" ) {
                std::string msg;
                for ( PeripheralPtr p : myPeripherals ) {
                    msg += getPeripheralBlock(p);
                }
                return CmdResult(0,msg);
            }
            else {
                for ( PeripheralPtr p : myPeripherals ) {
                    if ( p->getName()==v ) {
                        return CmdResult(0,getPeripheralBlock(p));
                    }
                }
                return CmdResult(1,"Unknown device: " + v );
            }
            #endif
            return CmdResult(0,"TO BE REFACTORED...");
        }
        else {
            return CmdResult(1,"Unsupported argument");
        }
        
    }
#endif

};


#endif /* !defined CMD_EXIT_HPP */

