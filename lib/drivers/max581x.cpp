
#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdint.h>

#include <iostream>

#include <common/PeripheralType.hpp>

#include <drivers/max581x.hpp>

using namespace std;

struct max581x_cmd 
{
    uint8_t  cmd;
    uint16_t data;
};


class Max581x::Impl {
private:
    static unsigned getChBits(unsigned int ch )
    {
        if ( ch<4 ) {
            return ch;
        }
        return 4;
    }

    Config myConfig;
    int    myFd;
    int    myEffVref; /*< Effective Reference voltage */

    enum RefMode {
        MAX581X_REF_EXT = 0x0   ,
        MAX581X_REF_2500= 0x1   ,
        MAX581X_REF_2048= 0x2   ,
        MAX581X_REF_4096= 0x3   ,
    };



    int runCommand( struct max581x_cmd const *cmd )
    {
        struct i2c_rdwr_ioctl_data packets;
        struct i2c_msg messages[2];
        uint8_t wrBuf[3];
        uint8_t rdBuf[3];
        
        wrBuf[0]    = cmd->cmd;
        wrBuf[1]    = cmd->data>>8;
        wrBuf[2]    = cmd->data&0xff;

        messages[0].addr  = myConfig.i2cAddr;
        messages[0].flags = 0;
        messages[0].len   = 3;
        messages[0].buf   = wrBuf;

        /* The data will get readback in this structure */
        messages[1].addr  = myConfig.i2cAddr;
        messages[1].flags = I2C_M_RD/* | I2C_M_NOSTART*/;
        messages[1].len   = 3;
        messages[1].buf   = rdBuf;

        /* Send the request to the kernel and get the result back */
        packets.msgs      = messages;
        packets.nmsgs     = 2;
        if(ioctl(myFd, I2C_RDWR, &packets)<0) {
            perror("During I2C_RDWR");
            return 1;
        }
        return 0;
    }

    uint16_t calculateCode(unsigned int outputLevel )
    {
        return (outputLevel*0xffffu)/(myEffVref*myConfig.outputGain);
    }






public:

    Impl(Max581x::Config const &config ) :
    myConfig(config)
    {
        myFd    = open( myConfig.i2cDev.c_str() , O_RDWR );
        if ( myFd==-1 ) {
            perror( "Opening I2C-dev node" );
            exit(1);
        }
    }

    ~Impl() {
        close(myFd);
    }


    string getVendor()
    {
        return "Maxim Integrated";
    }


    string getModel()
    {
        return "MAX581X";
    }

    string getRevision()
    {
        return "N/A";
    }

    string getDriverVersion()
    {
        return "0.0.1";
    }

    string getAuthor()
    {
        return "Antonio Oliveira - to DOT oliveira AT gmail DOT com";
    }



    /**
     * Set reference mode of the chip
     * \param refMode Reference mode to select
     * \param autoStandby If internal reference is enabled
     */
    int setReference(RefMode refMode)
    {
        struct max581x_cmd cmd = {
            .cmd    = (uint8_t)(0x70 | (uint8_t)refMode) ,
            .data   = 0x0000         ,
        };
        return runCommand(&cmd);
    }


    int init()
    {
        double dacFs    = myConfig.fullScale/myConfig.outputGain;
        RefMode refMode;
        if ( dacFs>2500 ) {
            refMode     = MAX581X_REF_4096;
            myEffVref   = 4096;
        }
        else if ( dacFs>2048 ) {
            refMode     = MAX581X_REF_2500;
            myEffVref   = 2500;
        }
        else {
            refMode     = MAX581X_REF_2048;
            myEffVref   = 2048;
        }
        cout << "Selected VREF=" << myEffVref << " for fullScale of " << myEffVref*myConfig.outputGain << " mV\n";
        return setReference(refMode);
    }


    int setOutput( unsigned int ch , unsigned int level )
    {
        if ( level>myConfig.fullScale ) {
            level   = myConfig.fullScale;
        }

        struct max581x_cmd cmd = {
            .cmd    = (uint8_t)(0x30 | (uint8_t)getChBits(ch))      ,
            .data   = calculateCode(level) ,
        };
        return runCommand(&cmd);
    }


}; /* class Max581x::Impl */



Max581x::Max581x( unsigned int id , Config const &config ) :
Dac(id)
{
    impl    = new Impl(config);

}

Max581x::~Max581x()
{
    delete impl;
}

string Max581x::getVendor() const
{
    return impl->getVendor();
}


string Max581x::getModel() const
{
    return impl->getModel();
}

string Max581x::getRevision() const
{
    return impl->getRevision();
}

string Max581x::getDriverVersion() const
{
    return impl->getDriverVersion();
}

string Max581x::getAuthor() const
{
    return impl->getAuthor();
}

int Max581x::init()
{
    return impl->init();
}

int Max581x::setOutput(unsigned int ch , int level )
{
    return impl->setOutput(ch,level);
}

std::string Max581x::getUnit() const
{
    return "1mV";
}


#if 0
int max581xSetLatch( MAX581X *chip , int selMask , int level )
{
    struct max581x_cmd cmd  = {
        .cmd    = selMask   ,
        .data   = calculateCode(chip,level) ,
    };
    return runCommand(chip,&cmd);
}

int max581xUpdateOutput( MAX581X *chip , int selMask )
{
    struct max581x_cmd cmd  = {
        .cmd    = 0x10 | selMask    ,
        .data   = 0x0000            ,
    };
    return runCommand(chip,&cmd);
}


int max581xSetLatchUpdateAll( MAX581X *chip , int selMask , int level )
{
    struct max581x_cmd cmd = {
        .cmd    = 0x20 | selMask            ,
        .data   = calculateCode(chip,level) ,
    };
    return runCommand(chip,&cmd);
}



    int setPowerMode( unsigned int ch , PowerMode mode )
    {
        struct max581x_cmd cmd  = {
            .cmd    = 0x40 | mode   ,
            .data   = selMask<<8    ,
        };
        
        return runCommand(&cmd);
    }


#endif




