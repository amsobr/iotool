
#include <cassert>

#include <common/I2cTransaction.hpp>


#include <drivers/Max518x.hpp>
#include <utility>

using namespace std;



struct Max581x::ChipCommand
{
    uint8_t  cmd;
    uint16_t data;
};


inline static unsigned getChBits(unsigned int ch )
{
    if ( ch<4 ) {
        return ch;
    }
    return 4;
}


void Max581x::runCommand( ChipCommand const& cmd ) const
{
        uint8_t wrBuf[3];
        uint8_t rdBuf[3];
        wrBuf[0]    = cmd.cmd;
        wrBuf[1]    = cmd.data>>8;
        wrBuf[2]    = cmd.data&0xff;
    
        I2cTransaction{myConfig.i2cDev,(uint16_t)myConfig.i2cAddr}
            .write(wrBuf,3)
            .read(rdBuf,3)
            .execute();

}

uint16_t Max581x::calculateCode(unsigned int outputLevel ) const
{
    return (uint16_t)((outputLevel*0xffffu)/(myEffVref*myConfig.outputGain));
}


string Max581x::getVendor() const
{
    return "Maxim Integrated";
}


string Max581x::getModel() const
{
    return "MAX581x";
}

string Max581x::getRevision() const
{
    return "0";
}

string Max581x::getDriverVersion() const
{
    return "0.1.0";
}


string Max581x::getAuthor() const
{
    return "amsobr @ github";
}



/**
 * Set reference mode of the chip
 * \param refMode Reference mode to select
 */
void Max581x::setReference(RefMode refMode)
{
    struct ChipCommand cmd = {
        .cmd    = (uint8_t)(0x70 | (uint8_t)refMode) ,
        .data   = 0x0000         ,
    };
    runCommand(cmd);
}




int Max581x::setOutput( unsigned int ch , int level )
{
    if ( level>myConfig.fullScale ) {
        level   = myConfig.fullScale;
    }

    struct ChipCommand cmd = {
        .cmd    = (uint8_t)(0x30 | (uint8_t)getChBits(ch))      ,
        .data   = calculateCode(level) ,
    };
    runCommand(cmd);
    return 0;
}



Max581x::Max581x(int id , Config config ) :
Dac{ id } ,
myConfig{std::move( config )}
{
    assert( myConfig.fullScale>0 );
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
    //cout << "Selected VREF=" << myEffVref << " for fullScale of " << myEffVref*myConfig.outputGain << " mV\n";
    setReference(refMode);
}

Max581x::~Max581x() = default;


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




