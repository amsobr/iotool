
#include <string>
#include <memory>
#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdint.h>


#include <common/PowerMonitor.hpp>

#include <drivers/ina21x.hpp>

using namespace std;

class Ina21x::Impl {
private:
    Config  myConfig;
    int     myFd;

    static uint8_t const ADDR_REG_CONFIG = 0x00;
    static uint8_t const ADDR_REG_SHUNT  = 0x01;
    static uint8_t const ADDR_REG_BUS    = 0x02;
    static uint8_t const ADDR_REG_PWR    = 0x03;
    static uint8_t const ADDR_REG_CURR   = 0x04;
    static uint8_t const ADDR_REG_CAL    = 0x05;
    

    int readRegister( uint8_t address , uint16_t *buf )
    {
        struct i2c_rdwr_ioctl_data packets;
        struct i2c_msg messages[2];
        uint8_t wrBuf[1];
        uint8_t rdBuf[2];
        
        wrBuf[0]    = address;

        messages[0].addr  = myConfig.i2cAddr;
        messages[0].flags = 0;
        messages[0].len   = 1;
        messages[0].buf   = wrBuf;

        /* The data will get readback in this structure */
        messages[1].addr  = myConfig.i2cAddr;
        messages[1].flags = I2C_M_RD/* | I2C_M_NOSTART*/;
        messages[1].len   = 2;
        messages[1].buf   = rdBuf;

        /* Send the request to the kernel and get the result back */
        packets.msgs      = messages;
        packets.nmsgs     = 2;
        if(ioctl(myFd, I2C_RDWR, &packets)<0) {
            perror("During I2C_RDWR");
            return 1;
        }
        *buf    = (rdBuf[0]<<8) | rdBuf[1];
        return 0;

    }

    int writeRegister( uint8_t address , uint16_t value )
    {
        struct i2c_rdwr_ioctl_data packets;
        struct i2c_msg messages[1];
        uint8_t wrBuf[3];
        
        wrBuf[0]    = address;
        wrBuf[1]    = value>>8;
        wrBuf[2]    = value&0xff;

        messages[0].addr  = myConfig.i2cAddr;
        messages[0].flags = 0;
        messages[0].len   = 3;
        messages[0].buf   = wrBuf;

        /* Send the request to the kernel and get the result back */
        packets.msgs      = messages;
        packets.nmsgs     = 2;
        if(ioctl(myFd, I2C_RDWR, &packets)<0) {
            perror("During I2C_RDWR");
            return 1;
        }
        return 0;
    }


public:

    Impl(Ina21x::Config const &config ) :
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
        return "Texas Instruments";
    }


    string getModel()
    {
        return "INA21X";
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

    int init()
    {
        uint16_t configReg  = 0x0000;
        if ( myConfig.maxVoltage>=16000 ) {
            configReg      |= (1<<13);
        }

        double maxShuntVoltage  = myConfig.shuntResistance/1000000.0 * myConfig.maxCurrent/1000.0;
        uint16_t pgBits;
        if ( maxShuntVoltage>0.16 ) {
            pgBits  = 0x3;
        }
        else if ( maxShuntVoltage>0.08 ) {
            pgBits  = 0x2;
        }
        else if ( maxShuntVoltage>0.04 ) {
            pgBits  = 0x1;            
        }
        else {
            pgBits  = 0x0;
        }
        configReg |= (pgBits<<11);
        configReg |= (0x9<<7); // 12 bit, average of 2
        configReg |= (0x9<<3); // 12 bit, average of 2
        configReg |= 0x7; // sample shunt and bus continuously
        return writeRegister(ADDR_REG_CONFIG,configReg);
    }



    size_t getNumChannels() const { return 1; }

    double getCurrent( unsigned int ch )
    {
        uint16_t regVal = 0;
        readRegister(ADDR_REG_SHUNT,&regVal);
        int intVal  = regVal;
        if ( intVal&0x00008000 ) {
            intVal |= 0xffff0000; /* sign-extend negative values */
        }
        return intVal*0.00001/(myConfig.shuntResistance*0.000001);        
    }

    double getVoltage( unsigned int ch )
    {
        uint16_t regVal = 0;
        readRegister(ADDR_REG_BUS,&regVal);
        return (regVal>>3)*0.004;
    }


    double getPower( unsigned int ch )
    {
        return getVoltage(ch)*getCurrent(ch);
    }
}; /* class Ina21x::Impl */


Ina21x::Ina21x( unsigned int id , Config const& config ) :
PowerMonitor(id)
{
    impl    = new Impl(config);
}

Ina21x::~Ina21x()
{
    delete impl;
}

string Ina21x::getVendor() const { return impl->getVendor(); }
string Ina21x::getModel() const { return impl->getModel(); }
string Ina21x::getRevision() const { return impl->getRevision(); }
string Ina21x::getDriverVersion() const { return impl->getDriverVersion(); }
string Ina21x::getAuthor() const { return impl->getAuthor(); }

int Ina21x::init() { return impl->init(); }
size_t Ina21x::getNumChannels() const { return impl->getNumChannels(); }
double Ina21x::getCurrent( unsigned int ch ) { return impl->getCurrent(ch); }
double Ina21x::getVoltage( unsigned int ch ) { return impl->getVoltage(ch); }
double Ina21x::getPower( unsigned int ch ) { return impl->getPower(ch); }
