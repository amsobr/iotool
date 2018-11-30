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

#include <cbsl/strings.hpp>

#include <common/adc.hpp>
#include <common/peripheral_type.hpp>

#include <ads126x.hpp>
#include <ads126x_config.hpp>
#include <ads126x_defs.hpp>

using namespace std;

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
    
    usleep(100);
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





class Ads126x::Ads126xImpl
{
private:
    
    
    std::string mySpiDev;
    int         myFd;
    double      myVref;
    std::vector<double> myInpGain;
    
    std::string myModel;
    std::string myRevision;
    
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
    
    Ads126xImpl( Ads126xConfig const &cfg )
    {
        mySpiDev    = cfg.spiDevice;
        myVref      = 2.5;
        assert( cfg.inpGain.size()==10 );
        myInpGain   = cfg.inpGain;
    }
    
    ~Ads126xImpl()
    {
        close( myFd );
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
        
        writeSingleReg(ADS126X_REG_MODE0,0x1b);
        writeSingleReg(ADS126X_REG_MODE1,0x00);
        writeSingleReg(ADS126X_REG_MODE2,0x04);
        writeSingleReg(ADS126X_REG_IDACMUX,0xbb);
        writeSingleReg(ADS126X_REG_IDACMAG,0x00);
        writeSingleReg(ADS126X_REG_REFMUX,0x00);
        writeSingleReg(ADS126X_REG_TDACP,0x00);
        writeSingleReg(ADS126X_REG_TDACN,0x00);
        writeSingleReg(ADS126X_REG_GPIOCON,0x00);
        writeSingleReg(ADS126X_REG_FSCAL0,0x00);
        writeSingleReg(ADS126X_REG_FSCAL1,0x00);
        writeSingleReg(ADS126X_REG_FSCAL2,0x40);
        
        uint8_t rd[3];
        rd[0] = rd[1] = rd[2] = 0;
        
        readRegisters(ADS126X_REG_FSCAL0,3,rd);
        printf( "fscal0=0x%02x\nfscal1=0x%02x\nfscal2=0x%02x\n",rd[0],rd[1],rd[2] );        
    }

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

    void dumpRegisters()
    {
        uint8_t regDump[0x1b];
        fprintf( stderr , "Register dump:\n");
        readRegisters(ADS126X_REG_ID,0x1b,regDump);
        for ( int k=0 ; k<0x1b ; k++ ) {
            fprintf( stderr , "0x%02x: 0x%02x\n",k,regDump[k]);
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
            fprintf( stderr , "readData() called for invalid ADC id: %d\n" , adcId );
            return -1;
        }
        
        uint8_t wr[7];
        uint8_t rd[7];
    
        memset(wr,0,7);
        wr[0]   = cmd;

        bool adcDone    = false;
        int iteration  = 0;
        do {
            iteration++;
            memset(rd,0,7);
            int res = spi_raw_xfer(myFd,wr,rd,7);
            if ( res!=0 ) {                
                cerr << "OOPS. Not expecting this...\n";
                exit(1);
                return res;
            }
            if ( rd[1]&doneMask ) {
                adcDone = true;
            }
            else {
                if ( rd[1]!=0x00 ) {
                    cerr << "STATUS is " << hex << (int)rd[1] << "\n";
                    //exit(1);                
                }
                if ( iteration>=10000 ) {
                    cerr << "Number of iterations exceeded. Giving up.\n";
                    break;
                }
                usleep(100);
            }
        } while ( !adcDone );
        if ( iteration>10 ) {
            //cerr << "Conversion complete after " << iteration << " iterations\n";
        }

        uint8_t cksum   = rd[2]+rd[3]+rd[4]+rd[5]+0x9b;
        if ( cksum!=rd[6] ) {
            fprintf( stderr , "cksum mismatch. calculated=0x%02hhx , read=0x%02hhx\n" , cksum , rd[6] );
            fprintf( stderr , "rd[0]=0x%02x (d/c)\nrd[1]=0x%02x (stt)\nrd[2]=0x%02x\nrd[3]=0x%02x\nrd[4]=0x%02x\nrd[5]=0x%02x\nrd[6]=0x%02x (cksum)\n"
                    , rd[0],rd[1],rd[2],rd[3],rd[4],rd[5],rd[6]); 
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
        usleep(50000); /* hardcoded for ~20SPS */
    }
    
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
    

}; /* class Ads126x::Ads126xImpl */







Ads126x::Ads126x( unsigned int id , Ads126xConfig const &cfg ) :
Adc(id)
{
    impl    = new Ads126xImpl( cfg );
    impl->init();
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

long int Ads126x::readDigital( unsigned int ch)
{
    return impl->readDigital(ch);
}

double Ads126x::readAnalog( unsigned int ch)
{
    return impl->readAnalog(ch);
}

string Ads126x::getUnits() const
{
    return "V";
}

unsigned int Ads126x::setSampleRate( unsigned int sampleRate )
{ 
    return impl->setSampleRate(sampleRate);
}

    
