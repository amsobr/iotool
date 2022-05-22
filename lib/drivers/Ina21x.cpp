
#include <string>

#include <common/PowerMonitor.hpp>
#include <common/I2cTransaction.hpp>

#include <drivers/Ina21x.hpp>
#include <utility>

using namespace std;


static constexpr uint8_t ADDR_REG_CONFIG = 0x00;
static constexpr uint8_t ADDR_REG_SHUNT  = 0x01;
static constexpr uint8_t ADDR_REG_BUS    = 0x02;
static constexpr uint8_t ADDR_REG_PWR    = 0x03;
static constexpr uint8_t ADDR_REG_CURR   = 0x04;
static constexpr uint8_t ADDR_REG_CAL    = 0x05;
    

uint16_t Ina21x::readRegister( uint8_t address ) const
{
    uint8_t wrBuf[1]    = { address };
    uint8_t rdBuf[2];
    
    I2cTransaction{ myConfig.i2cDev, (uint16_t)myConfig.i2cAddr }
        .write(wrBuf,1)
        .read(rdBuf,2)
        .execute();
    
    return (rdBuf[0]<<8) | rdBuf[1];
}

void Ina21x::writeRegister( uint8_t address , uint16_t value ) const
{
    uint8_t wrBuf[3]    = {
        address ,
        (uint8_t)(value>>8) ,
        (uint8_t)(value&0xff)
    };
    
    I2cTransaction{myConfig.i2cDev,(uint16_t)myConfig.i2cAddr}
        .write(wrBuf,3)
        .execute();
}

string Ina21x::getVendor() const
{
    return "Texas Instruments";
}


string Ina21x::getModel() const
{
    return "INA21x";
}


string Ina21x::getRevision() const
{
    return "N/A";
}


string Ina21x::getDriverVersion() const
{
    return "0.1.0";
}


string Ina21x::getAuthor() const
{
    return "amsobr @ github";
}


size_t Ina21x::getNumChannels() const
{
    return 1;
}


double Ina21x::getCurrent( unsigned int ch )
{
    unsigned int intVal  = readRegister(ADDR_REG_SHUNT);
    if ( intVal&0x00008000 ) {
        intVal |= 0xffff0000; /* sign-extend negative values */
    }
    return intVal*0.00001/(myConfig.shuntResistance*0.000001);
}

double Ina21x::getVoltage( unsigned int ch )
{
    return (readRegister(ADDR_REG_BUS)>>3)*0.004;
}


double Ina21x::getPower( unsigned int ch )
{
    return getVoltage(ch)*getCurrent(ch);
}


Ina21x::Ina21x(int id , Config  config ) :
PowerMonitor{ id } ,
myConfig{std::move( config )}
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
    writeRegister(ADDR_REG_CONFIG,configReg);
}

Ina21x::~Ina21x()   = default;

