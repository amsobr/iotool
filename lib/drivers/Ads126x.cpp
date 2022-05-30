#include <sstream>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#include <Poco/Logger.h>
#include <Poco/Format.h>
#include <Poco/Stopwatch.h>


#include <common/Adc.hpp>
#include <common/PeripheralType.hpp>
#include <common/SpiTransaction.hpp>

#include <drivers/Ads126x.hpp>
#include <drivers/ads126x_config.hpp>


#include "ads126x_defs.hpp"


using namespace std;
using namespace Poco;

using MICROS    = std::chrono::microseconds;
using SECONDS   = std::chrono::seconds;
using SpiMode   = SpiTransaction::SpiMode;

static constexpr SpiMode ADS126X_SPI_MODE   = SpiMode::MODE_1;
static constexpr int     ADS126X_SPI_SPEED  = 25000000;

static constexpr int ADS126X_INPUT_AINCOM   = 10;
static constexpr int ADS126X_INPUT_FLOAT    = 15;

static constexpr auto ADS1262_DRIVER_VERSION= "1.0.0";



namespace Adc1 {

/**
 * @brief Get the Index of Samples per Second for ADC1 (32b core)
 * 
 * @param sps Samples Per Second string, as given in option "sps"
 * @return int The Samples per Second bits. This returned value is valid
 *              for lookups in constants tables, as well as for direct
 *              usage in the register config for DAC1_SPS
 */
static int getSpsId(std::string const &sps )
{
    if ( sps == "2.5" )     return 0;
    if ( sps == "5" )       return 1;
    if ( sps == "10" )      return 2;
    if ( sps == "16.6" )    return 3;
    if ( sps == "20" )      return 4;
    if ( sps == "50" )      return 5;
    if ( sps == "60" )      return 6;
    if ( sps == "100" )     return 7;
    if ( sps == "400" )     return 8;
    if ( sps == "1200" )    return 9;
    if ( sps == "2400" )    return 10;
    if ( sps == "4800" )    return 11;
    if ( sps == "7200" )    return 12;
    if ( sps == "14400" )   return 13;
    if ( sps == "19200" )   return 14;
    if ( sps == "38400" )   return 15;
    return -1;
}

static int getFilterId(std::string const& filter )
 {
    if ( filter == "sinc1" ) return 0;
    if ( filter == "sinc2" ) return 1;
    if ( filter == "sinc3" ) return 2;
    if ( filter == "sinc4" ) return 3;
    if ( filter == "sinc5" ) return 4;
    if ( filter == "sinc1" ) return 5;
    return -1;
}

/* Ideal number of iterations until completion of ADC conversion*/
static int const NUM_POLL_ITERATIONS(4);

/* Convertion times, in microseconds */
static int CONV_DURATION[16][6] = {
/* SPS          SINC1     SINC2     SINC3     SINC4   SINC5      FIR */
/*     2.5*/{  400400 ,  800400 , 1200000 , 1600000 ,    -1 , 402200 } ,
/*     5 */ {  200400 ,  400400 ,  600400 ,  800400 ,    -1 , 202200 } , 
/*    10 */ {  100400 ,  200400 ,  300400 ,  400400 ,    -1 , 102200 } ,
/*    16.6*/{   60350 ,  120400 ,  180400 ,  240400 ,    -1 ,     -1 } ,
/*    20 */ {   50350 ,  100400 ,  150400 ,  200400 ,    -1 ,  52220 } , 
/*    50 */ {   20350 ,   40420 ,   60420 ,   80420 ,    -1 ,     -1 },
/*    60 */ {   17020 ,   33760 ,   50420 ,   67090 ,    -1 ,     -1 } ,
/*   100 */ {   10350 ,   20420 ,   30420 ,   40420 ,    -1 ,     -1 } ,
/*   400 */ {    2855 ,    5424 ,    7924 ,   10420 ,    -1 ,     -1 } ,
/*  1200 */ {    1188 ,    2091 ,    2924 ,    3758 ,    -1 ,     -1 } ,
/*  2400 */ {     771 ,    1258 ,    1674 ,    2091 ,    -1 ,     -1 } ,
/*  4800 */ {     563 ,     841 ,    1049 ,    1258 ,    -1 ,     -1 } ,
/*  7200 */ {     494 ,     702 ,     841 ,     980 ,    -1 ,     -1 } ,
/* 14400 */ {      -1 ,      -1 ,      -1 ,      -1 ,   424 ,     -1 } ,
/* 19200 */ {      -1 ,      -1 ,      -1 ,      -1 ,   337 ,     -1 } ,
/* 38400 */ {      -1 ,      -1 ,      -1 ,      -1 ,   207 ,     -1 }
};


/** Map SPS_ID into register value */
static int SPS_REG_VAL[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

/** Map FILTER_ID into register value
 * @note SINC5 is special case for SPS>=14400, meaning the value will
 *          be actually ifgnored and 2nd stage SINC bypassed.
 */
static int FILTER_REG_VAL[6] =
{
    0 , /* SINC1 */
    1 , /* SINC2 */
    2 , /* SINC3 */
    3 , /* SINC4 */
    4 , /* SINC5 - register value is ignored, as the second SINC stage */
    4 , /* FIR   */
};





enum ads1262_filter_t {
    ADS1262_FILTER_SINC1 = 0 ,
    ADS1262_FILTER_SINC2 = 1 ,
    ADS1262_FILTER_SINC3 = 2 ,    
    ADS1262_FILTER_SINC4 = 3 ,
    ADS1262_FILTER_FIR   = 4 ,
};
    
} /* namespace Adc1 */


static inline void throwIfInvalidInput(int idx )
{
    if ( idx<0 || idx>ADS126X_INPUT_AINCOM ) {
        throw std::runtime_error{"invalid input index"};
    }
}


static inline void doCommand(SpiTransaction& spi , Ads126xCmd cmd )
{
    uint8_t     wr   = (uint8_t)cmd;
    uint8_t     rd;
    spi.clear()
        .transfer8(&wr,&rd,1)
        .execute();
}


static inline void readRegisters( SpiTransaction& spi, Ads126xRegister reg , int count , void *buf )
{
    assert( count>=1 );
    
    std::vector<uint8_t> wrBuf(count+2,0);
    std::vector<uint8_t> rdBuf(count+2,0);
    wrBuf[0]    = 0x20 | reg;
    wrBuf[1]    = count - 1;
    
    spi.clear()
        .transfer8(&wrBuf[0],&rdBuf[0],count+2)
        .execute();
    
    memcpy(buf,&rdBuf[2],count);
}


static inline void writeRegisters(SpiTransaction& spi , Ads126xRegister reg , int count , void const *buf )
{
    assert( count>=1 );
    
    std::vector<uint8_t> wrBuf(count+2, 0);
    std::vector<uint8_t> rdBuf(count+2, 0);
    
    memcpy(&wrBuf[2],buf,count);
    wrBuf[0]    = 0x40 | reg;
    wrBuf[1]    = count - 1;
    
    spi.clear()
        .transfer8(&wrBuf[0],&rdBuf[0],count+2)
        .execute();
}


static inline void writeSingleReg( SpiTransaction& spi, Ads126xRegister reg , unsigned char value )
{
    writeRegisters(spi,reg,1,&value);
    uint8_t u8 = 0;
    readRegisters(spi,reg,1,&u8);
    if ( value!=u8 ) {
        throw std::runtime_error{ Poco::format("REG 0x%02x: wrote 0x%02x but read 0x%02x",reg,value,u8) };
    }
}


static inline void setInput(SpiTransaction& spi, int idxP , int idxN )
{
    writeSingleReg( spi, ADS126X_REG_INPMUX , (idxP<<4) | idxN );
}


static inline void setInput( SpiTransaction& spi, int idx )
{
    setInput(spi,idx,ADS126X_INPUT_AINCOM );
}


/**
 * @brief Set the ADC1 FILTER in reg. MODE1
 * Sets bits 7:5 into value provided in parameter.
 *
 * @param filterRegVal value for FILTER setting, before bitshifts.
 */
static inline void setAdc1Filter( SpiTransaction& spi, int filterRegVal )
{
    uint8_t regVal = 0;
    readRegisters( spi, ADS126X_REG_MODE1 , 1 , &regVal );
    regVal &= 0x1f;
    regVal |= (filterRegVal<<5);
    writeSingleReg( spi ,ADS126X_REG_MODE1,regVal);
}

/**
 * @brief Set ADC1 DR in reg. MODE2
 * Sets bits 3:0 of register to value provided in parameter.
 *
 * @param spsRegVal value for DR setting, before shifts
 */
static inline void setAdc1Sps( SpiTransaction& spi, int spsRegVal )
{
    uint8_t regVal = 0;
    readRegisters(spi,ADS126X_REG_MODE2,1,&regVal);
    regVal &= 0xf0;
    regVal |= spsRegVal;
    writeSingleReg(spi,ADS126X_REG_MODE2,regVal);
}






Ads126x::Ads126x( int id , Ads126xConfig const &cfg ) :
    Adc{ id } ,
    logger{ Poco::Logger::get("ads126x") } ,
    mySpiCharDev{cfg.spiDevice } ,
    mySpi{mySpiCharDev,ADS126X_SPI_MODE,ADS126X_SPI_SPEED},
    myVref{2.5} ,
    myInpGain{ cfg.inpGain } ,
    myIdacMagnitudes{"50u" ,"100u","250u","500u","750u","1000u","1500u","2000u","3000u" } ,
    mySpsValues{ "2.5","5","10","16.6","20","50","60","100","400","1200",
                            "2400","4800","7200","14400","19200","38400" } ,
    myFilterValues{"sinc1","sinc2","sinc3","sinc4","fir"}
{
    /* SINC5 filter not offered as option as it becomes active
     * for SPS>=14400, enforced by the chip itself...
     */
    assert( cfg.inpGain.size()==10 );

    uint8_t value;

    doCommand(mySpi, Ads126xCmd::RESET);
    std::this_thread::sleep_for( MICROS{200000} );

    value    = ADS126X_INTERFACE_STATUS_INCLUDED
               | ADS126X_INTERFACE_CKSUM;
    writeSingleReg(mySpi, ADS126X_REG_INTERFACE, value);

    uint8_t u8=0;
    readRegisters(mySpi, ADS126X_REG_INTERFACE, 1, &u8);
    if ( u8!=value ) {
        cerr << "Mismatching values in INTERFACE register: written=" << value << " readback=" << u8 << "\n";
    }


    readRegisters(mySpi, ADS126X_REG_ID, 1, &value);
    if ( (value&0xe0)==0x00 ) {
        myModel = "ADS1262";
    }
    else if ( (value&0xe0)==0x20 ) {
        myModel = "ADS1263";
    }
    else {
        //fprintf( stderr , "Detected unknown DAC: DEV_ID=0x%x\n" , value>>5 );
        myModel = "DEV_ID_UNKNOWN(" + std::to_string(value>>5) + ")";
    }

    myRevision = std::to_string(value&0x1f);
    //std::cerr << "Detected ADC: " << myModel << " rev. " << myRevision << "\n";

    value   = ADS126X_POWER_VBIAS_OFF | ADS126X_POWER_INTREF_ON;
    writeSingleReg(mySpi, ADS126X_REG_POWER, value);
    readRegisters(mySpi, ADS126X_REG_POWER, 1, &u8);
    if ( u8!=value ) {
        throw std::runtime_error{ Poco::format("failed setting POWER mode: wr=%02?x rd=%02?",value,u8) };
    }

    /* several configs mostly for testing..... */
    writeSingleReg(mySpi, ADS126X_REG_MODE0, 0x00);

    /* MODE1 set to SINC1 */
    myCurFilterId   = Adc1::getFilterId("sinc1");
    myOptFilter     = "sinc1";
    writeSingleReg(mySpi, ADS126X_REG_MODE1, 0x00);

    /* MODE2 set to 2400 sps */
    myCurSpsId      = Adc1::getSpsId("2400");
    myOptSps        = "2400";
    writeSingleReg(mySpi, ADS126X_REG_MODE2, 0x0a);
    writeSingleReg(mySpi, ADS126X_REG_IDACMUX, 0xbb);
    writeSingleReg(mySpi, ADS126X_REG_IDACMAG, 0x00);
    writeSingleReg(mySpi, ADS126X_REG_REFMUX, 0x00);
    writeSingleReg(mySpi, ADS126X_REG_TDACP, 0x00);
    writeSingleReg(mySpi, ADS126X_REG_TDACN, 0x00);
    writeSingleReg(mySpi, ADS126X_REG_GPIOCON, 0x00);
    writeSingleReg(mySpi, ADS126X_REG_FSCAL0, 0x00);
    writeSingleReg(mySpi, ADS126X_REG_FSCAL1, 0x00);
    writeSingleReg(mySpi, ADS126X_REG_FSCAL2, 0x40);
   
    setSps("2400");
    recalculateDelays();
    
    calibrate();
    
    uint8_t rd[3] { 0 };
    readRegisters(mySpi, ADS126X_REG_FSCAL0, 3, rd);
    logger.information( Poco::format("fscal0=0x%02?x\nfscal1=0x%02?x\nfscal2=0x%02?x\n",rd[0],rd[1],rd[2]) );
}
    
Ads126x::~Ads126x()  = default;


void Ads126x::dumpRegisters( std::ostream& os )
{
    uint8_t regDump[0x1b];
    readRegisters(mySpi, ADS126X_REG_ID, 0x1b, regDump);
    stringstream ss;
    ss << "ADS126x Register dump:\n";
    for (int k = 0; k < 0x1b; k++) {
        ss << format("0x%02x: 0x%02x\n", k, regDump[k]);
    }
    os << ss.str();
}
    

int32_t Ads126x::readData(SpiTransaction& spi, int adcId)
{
    logger.debug( format("ADS126x: reading data from ADC%d...",adcId) );
    Ads126xCmd cmd;
    uint8_t doneMask;
    if ( adcId==1 ) {
        cmd = Ads126xCmd::RDATA1;
        doneMask    = 0x40;
    }
    else if ( adcId==2 ) {
        cmd = Ads126xCmd::RDATA2;
        doneMask    = 0x80;
    }
    else {
        throw std::runtime_error{ Poco::format("invalid ADC id: %d",adcId) };
    }
    
    uint8_t wr[7];
    uint8_t rd[7];

    memset(wr,0,7);
    wr[0]   = (uint8_t)cmd;

    bool adcDone    = false;
    /* perform one first wait, to allow conversion to proceed */
    //std::this_thread::sleep_for( MICROS{myCurConvTime} );
    int iteration  = 0;
    Stopwatch stopwatch;
    do {
        iteration++;
        bzero(rd,sizeof(rd));
        spi.clear()
            .transfer8(wr,rd,7)
            .execute();
            
        if ( rd[1]&doneMask ) {
            logger.debug( format("ADS126x: transfer completed. iteration=%d  status=0x%02?x elapsed=%?dus",iteration,rd[1],stopwatch.elapsed()) );
            adcDone = true;
        }
        else {
            if ( rd[1]!=0x00 ) {
                logger.error(format("ADS126x: ADC STATUS is %x\n",rd[1]));
            }
            if (stopwatch.elapsed() >= 10*myCurConvTime ) {
                logger.error( format("ADS126x: Number of iterations (%d) to read() was exceeded. Giving up.\n",Adc1::NUM_POLL_ITERATIONS*100) );
                break;
            }
            if ( myCurConvPoll>500 ) {
                std::this_thread::sleep_for( MICROS{myCurConvPoll} );
            }
        }
    } while ( !adcDone );
    if ( iteration>Adc1::NUM_POLL_ITERATIONS ) {
        logger.warning( format("ADS126x: read from ADC%d took longer than expected. iterarions=%d expected=%d",adcId,iteration,Adc1::NUM_POLL_ITERATIONS) );
    }
    else {
        logger.debug( format("ADS126x: read from ADC%d complete!",adcId) );
    }

    uint8_t cksum   = rd[2]+rd[3]+rd[4]+rd[5]+0x9b;
    if ( cksum!=rd[6] ) {
        std::string msg = Poco::format("ADC%d - cksum mismatch: expect=0x%02?x read=0x%02?x", adcId,cksum,rd[6]);
        throw std::runtime_error{msg};
    }
    
    int32_t v   = ((int32_t)rd[2]<<24) | ((int32_t)rd[3]<<16) | ((int32_t)rd[4]<<8) | rd[5];
    logger.information(Poco::format("conversion result: 0x%08?x",v));
    return v;
}
    



/**
 * @brief Recalculate conversion delays.
 * Call this method once myCurFilterId and myCurSpsId are set
 * to recalculate the optimal delays to be used when polling
 * the ADC for conversion completion.
 */
void Ads126x::recalculateDelays()
{
    myCurConvTime = Adc1::CONV_DURATION[myCurSpsId][myCurFilterId];
    myCurConvPoll = myCurConvTime/Adc1::NUM_POLL_ITERATIONS;
    if ( myCurConvPoll<50 ) {
        myCurConvPoll   = 50;
    }
    logger.information(
        format("ADS126x: recalculated ADC1 times for sps=%s\nconversion time : %d us\npoll time  : %d us",myOptSps,myCurConvTime,myCurConvPoll) );
}

    

void Ads126x::calibrate()
{
    setInput(mySpi, ADS126X_INPUT_FLOAT, ADS126X_INPUT_FLOAT);
    doCommand(mySpi, Ads126xCmd::SFOCAL1);
    std::this_thread::sleep_for(MICROS{20000});
    //printf( "Performing self-calibration...\n");
    setInput(mySpi, ADS126X_INPUT_AINCOM, ADS126X_INPUT_AINCOM);
    doCommand(mySpi, Ads126xCmd::SYOCAL1);
    std::this_thread::sleep_for(MICROS{20000});
//    ads126x_sygcal1(adc);
//    sleep(2);
}

    

std::string Ads126x::getModel() const
{
    return myModel;
}


std::string Ads126x::getRevision() const
{
    return myRevision;
}


string Ads126x::getDriverVersion() const
{
    return ADS1262_DRIVER_VERSION;
}


std::string Ads126x::getAuthor() const
{
    return "amsobr";
}

double Ads126x::getResolution() const
{
    return myVref/0x7fffffff;
}
   
void Ads126x::startConversion(int ch)
{
    setInput( mySpi, ch);
    doCommand( mySpi, Ads126xCmd::START1);
}

void Ads126x::startConversion(int chp, int chn)
{
    setInput( mySpi, chp, chn);
    doCommand(mySpi,Ads126xCmd::START1);
}


int Ads126x::getNumChannels() const
{
    return 10;
}


int32_t Ads126x::read(int ch )
{
    throwIfInvalidInput(ch);
    startConversion(ch);
    return readData(mySpi, 1);
}


double Ads126x::readAnalog(int ch )
{
    throwIfInvalidInput(ch);
    return read(ch) * getResolution()/myInpGain[ch];
}


double Ads126x::readDifferential(int chp , int chn )
{
    throwIfInvalidInput(chp);
    throwIfInvalidInput(chn);
    startConversion(chp, chn);
    return readData(mySpi, 1) * getResolution() / myInpGain[chp];
}


size_t Ads126x::getNumCurrentSources() const
{
    return 2;
}


std::list<std::string> Ads126x::getCurSourceMagnitudes( unsigned int srcId ) const
{
    if ( srcId>=2 ) {
        return {};
    }
        
    return myIdacMagnitudes;
}


int Ads126x::setCurrentSource(int srcId, bool enabled, int ch, string const& mag)
{
    if ( srcId<0 || srcId>=2 ) {
        throw std::runtime_error{"out of bounds: isource ID"};
    }
    
    if ( enabled ) {
        throwIfInvalidInput(ch);
    }

    uint8_t idacCtl[2];
    readRegisters(mySpi, ADS126X_REG_IDACMUX, 2, idacCtl);

    uint8_t muxVal;
    uint8_t magVal;
    if ( !enabled || mag=="off" ) {
        muxVal  = 0x0b;
        magVal  = 0x00;
    }
    else {
        muxVal  = (uint8_t)ch; /* guaranteed OK b/c ch<numChannels... */

        if ( mag=="50u" ) {
            magVal  = 0x01;
        }
        else if ( mag=="100u" ) {
            magVal  = 0x02;
        }
        else if ( mag=="250u" ) {
            magVal  = 0x03;
        }
        else if ( mag=="500u" ) {
            magVal  = 0x04;
        }
        else if ( mag=="750u" ) {
            magVal  = 0x05;
        }
        else if ( mag=="1000u" ) {
            magVal  = 0x06;
        }
        else if ( mag=="1500u" ) {
            magVal  = 0x07;
        }
        else if ( mag=="2000u" ) {
            magVal  = 0x08;
        }
        else if ( mag=="2500u" ) {
            magVal  = 0x09;
        }
        else if ( mag=="3000u" ) {
            magVal  = 0x0a;
        }
        else {
            return -1;
        }
    }
    
    if ( srcId==0 ) {
        idacCtl[0]  &= 0xf0;
        idacCtl[1]  &= 0xf0;
        idacCtl[0]  |= muxVal;
        idacCtl[1]  |= magVal;
    }
    else {
        /* must be srcId==1 , which is IDAC2 */
        idacCtl[0]  &= 0x0f;
        idacCtl[1]  &= 0x0f;
        idacCtl[0]  |= (muxVal<<4);
        idacCtl[1]  |= (magVal<<4);
    }
    //fprintf(stdout,"WRITING: mux=0x%02x mag=0x%02x\n",idacCtl[0],idacCtl[1]);

    writeRegisters(mySpi, ADS126X_REG_IDACMUX, 2, idacCtl);

    //readRegisters(ADS126X_REG_IDACMUX,2,idacCtl);
    //fprintf(stdout,"READ(again): mux=0x%02x mag=0x%02x\n",idacCtl[0],idacCtl[1]);
    return 0;

}

void Ads126x::setSps( std::string const &value )
{
    
    int spsId = Adc1::getSpsId(value);
    if ( spsId==-1 ) {
        throw std::runtime_error{Poco::format("invalid 'sps' value - '%s'", value)};
    }
    logger.information( format("ADS126x: setting 'sps' to '%s'...",value) );

    int delay = Adc1::CONV_DURATION[spsId][myCurFilterId];
    if ( delay == -1 ) {
        /* SPS and FILTER don't combine.
         * try setting filter to "SINC1" as it delivers nominal SPS
         * up to 14400sps...
         */
        logger.debug( format("ADS126x: sps=%s and filter=%s will not work. Trying with filter='sinc1'...",value,myOptFilter) );
        int filterId    = Adc1::getFilterId("sinc1");
        delay           = Adc1::CONV_DURATION[spsId][filterId];
        if ( delay == -1 ) {
            logger.debug( format("ADS126x: sps=%s and filter='sinc1' will not work. Falling back to filter='sinc5'...",value) );
            /* only way of landing here is to have SPS > 14400
             * in this case the chip will enforce SINC5 do to
             * the bypass of the 2nd stage SINC.
             */
            myOptFilter   = "sinc5";
            myCurFilterId = Adc1::getFilterId("sinc5");
            /* no need to configure a filter, as it is a setting
             * enforced by the chip.
             */
        }
        else {
            myOptFilter     = "sinc1";
            myCurFilterId   = Adc1::getFilterId("sinc1");
            setAdc1Filter(mySpi, Adc1::FILTER_REG_VAL[myCurFilterId] );
            logger.information( format("ADS126x: selected sps=%s and filter=%s",value,myOptFilter));
        }
    }
    myOptSps    = value;
    myCurSpsId  = spsId;
    setAdc1Sps(mySpi, Adc1::SPS_REG_VAL[myCurSpsId] );
    recalculateDelays();
}

void Ads126x::setFilter(std::string const& value)
{
    int filterId = Adc1::getFilterId(value);
    if ( filterId == -1 ) {
        throw std::runtime_error{"bad filter name"};
    }

    int delay   = Adc1::CONV_DURATION[myCurSpsId][filterId];
    if ( delay == -1 ) {
        throw std::runtime_error{"filter not supported with current SPS"};
    }

    setAdc1Filter(mySpi, Adc1::FILTER_REG_VAL[filterId] );
    myCurFilterId   = filterId;
    myOptFilter     = value;
    recalculateDelays();
}

bool Ads126x::setOption( std::string const &name , std::string const &value )
{
    if ( name=="sps" ) {
        setSps(value);
        return true;
    }
    else if ( name=="filter" ) {
        setFilter(value);
        return true;
    }
    return false;
}










string Ads126x::getVendor() const
{
    return "Texas Instruments";
}
    


string Ads126x::getUnits() const
{
    return "V";
}



string Ads126x::getOption(string const& optName)
{
    return Configurable::getOption(optName);
}

vector<OptionHelpPtr> Ads126x::getOptionHelp()
{
    return Configurable::getOptionHelp();
}

int Ads126x::disableCurrentSource(int srcId)
{
    return setCurrentSource(srcId,false,0,"");
}

std::string Ads126x::getInputName(int idx) const
{
    if (idx<0 || idx>=getNumChannels() ) {
        throw std::runtime_error{"input index out of bounds"};
    }
    static std::vector<std::string> channelNames {
            "AIN0",
            "AIN1",
            "AIN2",
            "AIN3",
            "AIN4",
            "AIN5",
            "AIN6",
            "AIN7",
            "AIN8",
            "AIN9",
            "AINCOM",
            "TSENS",
            "AVCC",
            "DVCC",
            "TDAC",
            "FLOAT"
    };
    return channelNames[idx];
}


    
