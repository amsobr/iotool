#include <stdlib.h>
#include <string.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include <string>
#include <iostream>

#include <Poco/Logger.h>
#include <Poco/Format.h>
#include <Poco/Stopwatch.h>

#include <cbsl/strings.hpp>

#include <common/adc.hpp>
#include <common/peripheral_type.hpp>
#include <common/result.hpp>

#include <ads126x.hpp>
#include <ads126x_config.hpp>
#include <ads126x_defs.hpp>
#include <sstream>


using namespace std;
using namespace Poco;

static string ADS1262_DRIVER_VERSION("1");

static int spi_raw_xfer( int fd , void const *wrBuf , void *rdBuf , size_t len )
{
    struct spi_ioc_transfer xfer[1];
    int status;
    
    memset(xfer,0,sizeof(xfer));
    
    xfer[0].tx_buf  = (__u64)wrBuf;
    xfer[0].rx_buf  = (__u64)rdBuf;
    xfer[0].len     = len;
    xfer[0].bits_per_word = 8;
    xfer[0].speed_hz=25000000;
    
    //usleep(100);???
    status  = ioctl(fd,SPI_IOC_MESSAGE(1),xfer);
    if ( status<0 ) {
        perror("SPI_IOC_MESSAGE");
        return -1;
    }
    return 0;
}


static void spi_dumpstat(const char *name, int fd)
{
    __u8    lsb, bits;
    __u32   mode, speed;
    

    if (ioctl(fd, SPI_IOC_RD_MODE32, &mode) < 0) {
        perror("SPI rd_mode");
        return;
    }
    if (ioctl(fd, SPI_IOC_RD_LSB_FIRST, &lsb) < 0) {
        perror("SPI rd_lsb_fist");
        return;
    }
    if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) {
        perror("SPI bits_per_word");
        return;
    }
    if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) {
        perror("SPI max_speed_hz");
        return;
    }

    printf("%s: spi mode 0x%x, %d bits %sper word, %d Hz max\n",
        name, mode, bits, lsb ? "(lsb first) " : "", speed);
}



namespace Tables {
namespace Adc1 {

/**
 * @brief Get the Index of Samples per Second for ADC1 (32b core)
 * 
 * @param sps Samples Per Second string, as given in option "sps"
 * @return int The Samples per Second bits. This returned value is valid
 *              for lookups in constants tables, as well as for direct
 *              usage in the register config for DAC1_SPS
 */
static int getIdOfSps( std::string const &sps )
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

static int getIdOfFilter( std::string const filter )
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

}; /* namespace Tables::Adc1 */
}; /* namespace Tables */




typedef enum {
    ADS1262_FILTER_SINC1 = 0 ,
    ADS1262_FILTER_SINC2 = 1 ,
    ADS1262_FILTER_SINC3 = 2 ,    
    ADS1262_FILTER_SINC4 = 3 ,
    ADS1262_FILTER_FIR   = 4 ,
} ads1262_filter_t;


class Ads126x::Ads126xImpl
{
private:
    
    
    std::string mySpiDev;
    int         myFd;
    double      myVref;
    std::vector<double> myInpGain;
    std::list<string> myIdacMagnitudes;
    std::list<string> mySpsValues;
    std::list<string> myFilterValues;
    
    std::string myModel;
    std::string myRevision;

    /* options */
    std::string myOptSps; /*< Samples Per Second */
    std::string myOptFilter; /*< Filter */
    int myCurSpsId;
    int myCurFilterId;
    int myCurConvTime;
    int myCurConvPoll;

    Logger &logger;

    
    Ads126xInput getChInput( unsigned int chId )
    {
        Ads126xInput channel[10]   = {
            ADS126x_AIN0    ,
            ADS126x_AIN1    ,
            ADS126x_AIN2    ,
            ADS126x_AIN3    ,
            ADS126x_AIN4    ,
            ADS126x_AIN5    ,
            ADS126x_AIN6    ,
            ADS126x_AIN7    ,
            ADS126x_AIN8    ,
            ADS126x_AIN9    ,
        };
        if ( chId<10 ) {
            return channel[chId];
        }
        return ADS126x_AIN0;
    }
    
    
public:
    
    Ads126xImpl( Ads126xConfig const &cfg ) :
    logger(Logger::get("peripherals.ads126x"))
    {
        mySpiDev    = cfg.spiDevice;
        myVref      = 2.5;
        assert( cfg.inpGain.size()==10 );
        myInpGain   = cfg.inpGain;
        myIdacMagnitudes    = {"50u" ,"100u","250u","500u","750u","1000u","1500u","2000u","3000u" };
        mySpsValues         = { "2.5","5","10","16.6","20","50","60","100","400","1200",
                                "2400","4800","7200","14400","19200","38400" };
        /* SINC5 filter not offered as option as it becomes active
         * for SPS>=14400, enforced by the chip itself...
         */
        myFilterValues      = {"sinc1","sinc2","sinc3","sinc4","fir"};
    }
    
    ~Ads126xImpl()
    {
        close( myFd );
    }

private:
    
    void dumpRegisters()
    {
        uint8_t regDump[0x1b];
        readRegisters(ADS126X_REG_ID,0x1b,regDump);
        if ( logger.notice() ) {
            stringstream ss;
            ss << "ADS126x Register dump:\n";
            for (int k = 0; k < 0x1b; k++) {
                ss << format("0x%02x: 0x%02x\n", k, regDump[k]);
            }
            logger.notice(ss.str());
        }
    }
    
    int doCommand(Ads126xCommand cmd )
    {
        uint8_t     wr[1];
        uint8_t     rd[1];
    
        wr[0]   = cmd;
        return spi_raw_xfer(myFd , wr , rd , 1);
    }
    
    int readData( int adcId , int32_t *result )
    {
        Stopwatch stopwatch;

        logger.debug( format("ADS126x: reading data from ADC%d...",adcId) );
        Ads126xCommand cmd;
        uint8_t doneMask;
        if ( adcId==1 ) {
            cmd = ADS126X_CMD_RDATA1;
            doneMask    = 0x40;
        }
        else if ( adcId==2 ) {
            cmd = ADS126X_CMD_RDATA2;
            doneMask    = 0x80;
        }
        else {
            logger.error( format("ADS126x: readData() called for invalid ADC id: %d\n",adcId) );
            return -1;
        }
        
        uint8_t wr[7];
        uint8_t rd[7];
    
        memset(wr,0,7);
        wr[0]   = cmd;

        bool adcDone    = false;
        /* perform one first wait, to allow conversion to proceed */
        usleep(myCurConvTime);
        int iteration  = 0;
        do {
            iteration++;
            memset(rd,0,7);
            int res = spi_raw_xfer(myFd,wr,rd,7);
            if ( logger.debug() ) {
                logger.debug( format("ADS126x: transfer completed. iteration=%d elapsed=%dus",iteration,stopwatch.elapsed()) );
                stopwatch.restart();
            }
            if ( res!=0 ) {                
                logger.error("ADS126x: OOPS. This was unexpected x(...\n");
                exit(1);
                return res;
            }
            if ( rd[1]&doneMask ) {
                adcDone = true;
            }
            else {
                if ( rd[1]!=0x00 ) {
                    logger.error(format("ADS126x: ADC STATUS is %x\n",rd[1]));
                    //exit(1);                
                }
                if ( iteration>=100*Tables::Adc1::NUM_POLL_ITERATIONS ) {
                    logger.error( format("ADS126x: Number of iterations (%d) to read() was exceeded. Giving up.\n",Tables::Adc1::NUM_POLL_ITERATIONS*100) );
                    break;
                }
                usleep(myCurConvPoll);
            }
        } while ( !adcDone );
        if ( iteration>Tables::Adc1::NUM_POLL_ITERATIONS ) {
            logger.warning( format("ADS126x: read from ADC%d took longer than expected. iterarions=%d expected=%d",adcId,iteration,Tables::Adc1::NUM_POLL_ITERATIONS) );
        }
        else {
            logger.debug( format("ADS126x: read from ADC%d complete!",adcId) );
        }

        uint8_t cksum   = rd[2]+rd[3]+rd[4]+rd[5]+0x9b;
        if ( cksum!=rd[6] ) {
            if ( logger.error() ) {
                logger.error(
                    format("ADS126x: read ADC%d - cksum mismatch. calculated=0x%02x , read=0x%02x\n", adcId, cksum,
                           rd[6]));
                logger.error(format(
                    "ADS126x: read ADC%d dump:\nrd[0]=0x%02x (d/c)\nrd[1]=0x%02x (stt)\nrd[2]=0x%02x\nrd[3]=0x%02x\nrd[4]=0x%02x\nrd[5]=0x%02x\nrd[6]=0x%02x (cksum)\n",
                    adcId, rd[0], rd[1], rd[2], rd[3], rd[4], rd[5], rd[6]));
            }
            return -1;
        }

        *result     = rd[5] | (rd[4]<<8) | (rd[3]<<16) | (rd[2]<<24);
        
        return 0;
    }
    
    int readRegisters(Ads126xRegister reg , unsigned int count , void *buf )
    {
        uint8_t wrBuf[count+2];
        uint8_t rdBuf[count+2];
    
        memset(wrBuf,0,sizeof(wrBuf));
        memset(rdBuf,0,sizeof(rdBuf));
        wrBuf[0]    = 0x20 | reg;
        wrBuf[1]    = count - 1;
    
        int res = spi_raw_xfer(myFd,wrBuf,rdBuf,count+2);
    
        if ( res!=0 ) {
            return res;
        }
        memcpy(buf,rdBuf+2,count);
        return 0;
    }
    
    
    int writeRegisters(Ads126xRegister reg , unsigned int count , void const *buf )
    {
        uint8_t wrBuf[count+2];
        uint8_t rdBuf[count+2];
        
        memcpy(wrBuf+2,buf,count);
        memset(rdBuf,0,sizeof(rdBuf));
        wrBuf[0]    = 0x40 | reg;
        wrBuf[1]    = count - 1;
        
        int res = spi_raw_xfer(myFd,wrBuf,rdBuf,count+2);
        
        if ( res!=0 ) {
            return res;
        }
        return 0;
    }


    int writeSingleReg( Ads126xRegister reg , unsigned char value )
    {
        /* return */ writeRegisters(reg,1,&value);
        uint8_t u8 = 0;
        readRegisters(reg,1,&u8);
        if ( value!=u8 ) {
            fprintf( stderr , "REG 0x%02x: wrote 0x%02x but read 0x%02x\n",reg,value,u8);
        }
        else {
            //fprintf( stderr , "REG 0x%02x: wrote 0x%02x\n",reg,value);
        }
        return 0;
    }

    /**
     * @brief Set the ADC1 FILTER in reg. MODE1
     * Sets bits 7:5 into value provided in parameter.
     * 
     * @param filterRegVal value for FILTER setting, before bitshifts.
     * @return Result 
     */
    Result setAdc1Filter( int filterRegVal )
    {
        uint8_t regVal;
        readRegisters( ADS126X_REG_MODE1 , 1 , &regVal );
        regVal &= 0x1f;
        regVal |= (filterRegVal<<5);
        writeSingleReg(ADS126X_REG_MODE1,regVal);
        return Result::OK;
    }

    /**
     * @brief Set ADC1 DR in reg. MODE2
     * Sets bits 3:0 of register to value provided in parameter.
     * 
     * @param spsRegVal value for DR setting, before shifts
     * @return Result 
     */
    Result setAdc1Sps( int spsRegVal )
    {
        uint8_t regVal;
        readRegisters(ADS126X_REG_MODE2,1,&regVal);
        regVal &= 0xf0;
        regVal |= spsRegVal;
        writeSingleReg(ADS126X_REG_MODE2,regVal);
        return Result::OK;
    }

    /**
     * @brief Recalculate conversion delays.
     * Call this method once myCurFilterId and myCurSpsId are set
     * to recalculate the optimal delays to be used when polling 
     * the ADC for conversion completion.
     */
    void recalculateDelays()
    {
        myCurConvTime = Tables::Adc1::CONV_DURATION[myCurSpsId][myCurFilterId];
        myCurConvPoll = myCurConvTime/Tables::Adc1::NUM_POLL_ITERATIONS;
        if ( myCurConvPoll<50 ) {
            myCurConvPoll   = 50;
        }
        logger.information(
            format("ADS126x: recalculated ADC1 times for sps=%s\nconversion time : %d us\npoll time  : %d us",myOptSps,myCurConvTime,myCurConvPoll) );
    }

    
public:

    void calibrate()
    {
        //printf( "Performing self-calibration...\n");
        setInput(ADS126x_AINCOM,ADS126x_AINCOM);
        doCommand(ADS126X_CMD_SYOCAL1);
        sleep(1);
        setInput(ADS126x_FLOAT,ADS126x_FLOAT);
        doCommand(ADS126X_CMD_SFOCAL1);
        sleep(1);
    //    ads126x_sygcal1(adc);
    //    sleep(2);
    }

    
    int setInput( Ads126xInput input )
    {
        return writeSingleReg( ADS126X_REG_INPMUX , (input<<4)|ADS126x_AINCOM );
    }
    
    int setInput( Ads126xInput inputPos , Ads126xInput inputNeg )
    {
        uint8_t inpmux  = (inputPos<<4) | inputNeg;
        return writeSingleReg( ADS126X_REG_INPMUX , inpmux );
    }
       
    std::string getModel() const
    {
        return myModel;
    }
    
    std::string getRevision() const
    {
        return myRevision;
    }
    
    string getDriverVersion() const
    {
        return ADS1262_DRIVER_VERSION;
    }

    string getAuthor() const
    {
        return "to.oliveira AT gmail DOT com";
    }

    
    
    double getResolution()
    {
        return myVref/0x7fffffff;
    }
   
    void startConversion( unsigned int chId )
    {
        setInput( getChInput(chId) );
        doCommand(ADS126X_CMD_START1);
        usleep(myCurConvTime);
    }

    void startConversion( unsigned int chp, unsigned int chn )
    {
        setInput( getChInput(chp) , getChInput(chn) );
        doCommand(ADS126X_CMD_START1);
        usleep(myCurConvTime);
    }


    size_t getNumChannels() const { return 10; }
    
    long int readDigital( unsigned int ch )
    {
        startConversion(ch);
        int32_t result = 0;
        readData(1,&result);
        //fprintf( stderr , "Result from ADC (int) is 0x%08x\n" , result );
        return result;
    }
    
    double readAnalog( unsigned int ch )
    {
        return readDigital(ch)*getResolution();
    }

    double readDifferential( unsigned int chp , unsigned int chn )
    {
        startConversion(chp,chn);
        int32_t result = 0;
        readData(1,&result);;
        return result*getResolution();
    }

    unsigned int setSampleRate( unsigned int sampleRate )
    {
        return sampleRate;

    }

    
    
    double getUpperValue()
    {
        return 0;
    }
    
    double getLowerValue()
    {
        return 0;
    }
    
    bool isSingleEnded()
    {
        return true;
    }

    size_t getNumCurrentSources() const { return 2; }

    list<string> getCurSourceMagnitudes( unsigned int srcId ) const {
        if ( srcId>=2 ) {
            return list<string>();
        }
        
        return myIdacMagnitudes;
    }

    int setCurrentSource(unsigned int srcId, bool enabled, unsigned int ch, std::string mag)
    {
        if ( srcId>=2 ) {
            cout << "ERROR srcId>=2\n";
            return -1;
        }

        if ( (ch>0) && ch>=getNumChannels() ) {
            cout << "ERROR ch>=numChannels!!\n";
            cout << "ERROR ch value is " << ch << " numCh is " << getNumChannels() << "\n";
            return -1;
        }

        uint8_t idacCtl[2];
        readRegisters(ADS126X_REG_IDACMUX,2,idacCtl);
        fprintf(stdout,"READ: mux=0x%02x mag=0x%02x\n",idacCtl[0],idacCtl[1]);

        uint8_t muxVal;
        uint8_t magVal;
        if ( enabled==false || mag=="off" ) {
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
        fprintf(stdout,"WRITING: mux=0x%02x mag=0x%02x\n",idacCtl[0],idacCtl[1]);

        writeRegisters(ADS126X_REG_IDACMUX,2,idacCtl);

        readRegisters(ADS126X_REG_IDACMUX,2,idacCtl);
        fprintf(stdout,"READ(again): mux=0x%02x mag=0x%02x\n",idacCtl[0],idacCtl[1]);
        return 0;

    }

    Result setSps( std::string const &value )
    {
        int spsId = Tables::Adc1::getIdOfSps(value);
        if ( spsId==-1 ) {
            logger.error(format("ADS126x: invalid 'sps' value - '%s'", value));
            return Result::E_BAD_ARGS;
        }
        logger.information( format("ADS126x: setting 'sps' to '%s'...",value) );

        int delay = Tables::Adc1::CONV_DURATION[spsId][myCurFilterId];
        if ( delay == -1 ) {
            /* SPS and FILTER don't combine.
             * try setting filter to "SINC1" as it delivers nominal SPS
             * up to 14400sps...
             */
            logger.debug( format("ADS126x: sps=%s and filter=%s will not work. Trying with filter='sinc1'...",value,myOptFilter) );
            int filterId    = Tables::Adc1::getIdOfFilter("sinc1");
            delay           = Tables::Adc1::CONV_DURATION[spsId][filterId];
            if ( delay == -1 ) {
                logger.debug( format("ADS126x: sps=%s and filter='sinc1' will not work. Falling back to filter='sinc5'...",value) );
                /* only way of landing here is to have SPS > 14400
                 * in this case the chip will enforce SINC5 do to
                 * the bypass of the 2nd stage SINC.
                 */
                myOptFilter   = "sinc5";
                myCurFilterId = Tables::Adc1::getIdOfFilter("sinc5");
                /* no need to configure a filter, as it is a setting
                 * enforced by the chip.
                 */
            }
            else {
                myOptFilter     = "sinc1";
                myCurFilterId   = Tables::Adc1::getIdOfFilter("sinc1");
                setAdc1Filter( Tables::Adc1::FILTER_REG_VAL[myCurFilterId] );
                logger.information( format("ADS126x: selected sps=%s and filter=%s",value,myOptFilter));
            }
        }
        myOptSps    = value;
        myCurSpsId  = spsId;
        setAdc1Sps( Tables::Adc1::SPS_REG_VAL[myCurSpsId] );
        recalculateDelays();

        return Result::OK;
    }

    Result setFilter( std::string const &value )
    {
        int filterId = Tables::Adc1::getIdOfFilter(value);
        if ( filterId == -1 ) return Result::E_BAD_ARGS;

        int delay   = Tables::Adc1::CONV_DURATION[myCurSpsId][filterId];
        if ( delay == -1 ) return Result::E_NOT_SUPPORTED;

        setAdc1Filter( Tables::Adc1::FILTER_REG_VAL[filterId] );
        myCurFilterId   = filterId;
        myOptFilter     = value;
        recalculateDelays();
        return Result::OK;
    }

    Result setOption( std::string const &name , std::string const &value )
    {
        if ( name=="sps" ) return setSps(value);
        if ( name=="filter" ) return setFilter(value);
        return Result::E_NOT_SUPPORTED;
    }

    void init()
    {   
        uint8_t value;
        
        myFd    = open(mySpiDev.c_str(), O_RDWR);
        if ( myFd < 0) {
            perror("opening spidev:");
            return;
        }
        
        uint32_t mode   = 1;
        int res         = ioctl(myFd,SPI_IOC_WR_MODE32,&mode);
        if ( res!=0 ) {
            perror("SPI_IOC_WR_MODE32:");
            close(myFd);
            return;
        }


        spi_dumpstat(mySpiDev.c_str(),myFd);

         /*spi_dumpstat(mySpiDev,myFd);*/
         doCommand(ADS126X_CMD_RESET);
         usleep( 200000 );
        
        value    = ADS126X_INTERFACE_STATUS_INCLUDED
                                    | ADS126X_INTERFACE_CKSUM;
        writeSingleReg(ADS126X_REG_INTERFACE,value);

        uint8_t u8=0;
        readRegisters(ADS126X_REG_INTERFACE,1,&u8);
        if ( u8!=value ) {
            cerr << "Mismatching values in INTERFACE register: written=" << value << " readback=" << u8 << "\n";
        }
        
        
        readRegisters(ADS126X_REG_ID,1,&value);
        if ( (value&0xe0)==0x00 ) {
            myModel = "ADS1262";
        }
        else if ( (value&0xe0)==0x20 ) {
            myModel = "ADS1263";
        }
        else {
            fprintf( stderr , "Detected unknown DAC: DEV_ID=0x%x\n" , value>>5 );
            myModel = "DEV_ID_UNKNOWN(" + std::to_string(value>>5) + ")";
        }
        
        myRevision = std::to_string(value&0x1f);
        std::cerr << "Detected ADC: " << myModel << " rev. " << myRevision << "\n";

        value   = ADS126X_POWER_VBIAS_OFF | ADS126X_POWER_INTREF_ON;
        writeSingleReg(ADS126X_REG_POWER,value);
        readRegisters(ADS126X_REG_POWER,1,&u8);
        if ( u8!=value ) {
            cerr << "Mismatching values in POWER register: written=" << value << " readback=" << u8 << "\n";
        }

        /* several configs mostly for testing..... */
        
        writeSingleReg(ADS126X_REG_MODE0,0x00);

        /* MODE1 set to SINC1 */
        myCurFilterId   = Tables::Adc1::getIdOfFilter("sinc1");
        myOptFilter     = "sinc1";
        writeSingleReg(ADS126X_REG_MODE1,0x00);

        /* MODE2 set to 2400 sps */
        myCurSpsId      = Tables::Adc1::getIdOfSps("2400");
        myOptSps        = "2400";
        writeSingleReg(ADS126X_REG_MODE2,0x0a);
        writeSingleReg(ADS126X_REG_IDACMUX,0xbb);
        writeSingleReg(ADS126X_REG_IDACMAG,0x00);
        writeSingleReg(ADS126X_REG_REFMUX,0x00);
        writeSingleReg(ADS126X_REG_TDACP,0x00);
        writeSingleReg(ADS126X_REG_TDACN,0x00);
        writeSingleReg(ADS126X_REG_GPIOCON,0x00);
        writeSingleReg(ADS126X_REG_FSCAL0,0x00);
        writeSingleReg(ADS126X_REG_FSCAL1,0x00);
        writeSingleReg(ADS126X_REG_FSCAL2,0x40);

        recalculateDelays();
        
        uint8_t rd[3];
        rd[0] = rd[1] = rd[2] = 0;
        
        readRegisters(ADS126X_REG_FSCAL0,3,rd);
        printf( "fscal0=0x%02x\nfscal1=0x%02x\nfscal2=0x%02x\n",rd[0],rd[1],rd[2] );        
    }
   

}; /* class Ads126x::Ads126xImpl */







Ads126x::Ads126x( unsigned int id , Ads126xConfig const &cfg ) :
Adc(id)
{
    impl    = new Ads126xImpl( cfg );
    impl->init();
    impl->setOption("sps","7200");
}

Ads126x::~Ads126x()
{
    delete impl;
}

void Ads126x::init()
{
    impl->init();
}

void Ads126x::calibrate()
{
    impl->calibrate();
}

string Ads126x::getVendor() const
{
    return "Texas Instruments";
}
    

std::string Ads126x::getModel() const
{
    return impl->getModel();
}
    
std::string Ads126x::getRevision() const
{
    return impl->getRevision();
}

    
std::string Ads126x::getDriverVersion() const
{
    return impl->getDriverVersion();
}

string Ads126x::getAuthor() const
{
    return impl->getAuthor();
}

size_t Ads126x::getNumChannels() const
{
    return impl->getNumChannels();
}

long int Ads126x::readDigital( unsigned int ch)
{
    return impl->readDigital(ch);
}

double Ads126x::readAnalog( unsigned int ch)
{
    return impl->readAnalog(ch);
}

double Ads126x::readDifferential( unsigned int chP , unsigned int chN )
{
    return impl->readDifferential(chP,chN);
}

string Ads126x::getUnits() const
{
    return "V";
}

unsigned int Ads126x::setSampleRate( unsigned int sampleRate )
{ 
    return impl->setSampleRate(sampleRate);
}


size_t Ads126x::getNumCurrentSources() const
{
    return impl->getNumCurrentSources();
}

list<string> Ads126x::getCurSourceMagnitudes( unsigned int srcId ) const
{
    return impl->getCurSourceMagnitudes(srcId);
}

int Ads126x::setCurrentSource( unsigned int srcId , bool enabled , int ch , std::string mag )
{
    return impl->setCurrentSource(srcId,enabled,ch,mag);
}


    
