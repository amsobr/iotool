//
// Created by to on 23-05-2022.
//

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <stdexcept>
#include <iostream>

#include <Poco/Format.h>

#include <common/Strings.hpp>
#include <common/SpiTransaction.hpp>


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



std::string str( SpiTransaction::SpiMode m )
{
    switch (m) {
        case SpiTransaction::SpiMode::MODE_0: return "MODE_0";
        case SpiTransaction::SpiMode::MODE_1: return "MODE_1";
        case SpiTransaction::SpiMode::MODE_2: return "MODE_2";
        case SpiTransaction::SpiMode::MODE_3: return "MODE_3";
        default: return Poco::format("????(%d)",(int)m);
    }
}

SpiTransaction::SpiTransaction(std::string device, SpiTransaction::SpiMode mode, int speed) :
myDevice{ std::move(device) }
{
    assert(speed>0);
    
    myFd    = open(myDevice.c_str(), O_RDWR);
    if ( myFd < 0) {
        int e = errno;
        throw std::runtime_error{ Poco::format("opening '%s': errno %d - %s",myDevice,e,Strings::fromErrno(e))};
    }
    std::cout << "opened " + myDevice << " on fd " << myFd << "\n";
    
    uint32_t tmp   = (uint32_t)mode;
    int res         = ioctl(myFd,SPI_IOC_WR_MODE32,&tmp);
    if ( res!=0 ) {
        int e = errno;
        close(myFd);
        throw std::runtime_error{ Poco::format("setting %s on '%s': errno %d - %s",str(mode),myDevice,e,Strings::fromErrno(e))};
    }
    std::cout << "set mode=" << tmp << "\n";
    
    tmp = speed;
    res = ioctl(myFd,SPI_IOC_WR_MAX_SPEED_HZ,&tmp);
    if ( res!=0 ) {
        int e = errno;
        close(myFd);
        throw std::runtime_error{ Poco::format("setting speed to %d on '%s': errno %d - %s",speed,myDevice,e,Strings::fromErrno(e))};
    }
    std::cout << "set speed=" << tmp << "\n";
    spi_dumpstat(myDevice.c_str(),myFd);
}

SpiTransaction::~SpiTransaction()
{
    close( myFd );
}


SpiTransaction& SpiTransaction::transfer8(void const* wrData, void* rdData, int len)
{
    mySteps.emplace_back(wrData,rdData,8,len);
    return *this;
}

SpiTransaction& SpiTransaction::transfer16(void const* wrData, void* rdData, int len)
{
    mySteps.emplace_back(wrData,rdData,16,len);
    return *this;
}

SpiTransaction& SpiTransaction::transfer32(void const* wrData, void* rdData, int len)
{
    mySteps.emplace_back(wrData,rdData,32,len);
    return *this;
}

SpiTransaction& SpiTransaction::execute()
{
    struct spi_ioc_transfer xfer[mySteps.size()];
    bzero(xfer,sizeof(xfer));
    
    int cnt = 0;
    for( auto const& step: mySteps ) {
        xfer[cnt].tx_buf    = (__u64) step.wrBuf;
        xfer[cnt].rx_buf    = (__u64) step.rdBuf;
        xfer[cnt].len       = step.len;
        xfer[cnt].bits_per_word = step.bitsPerWord;
        xfer[cnt].speed_hz  = 25000000;
        cnt++;
    }
    
    int res = ioctl(myFd,SPI_IOC_MESSAGE(cnt),xfer);
    if ( res<0 ) {
        int e = errno;
        throw std::runtime_error{ Poco::format("running %d transfers on '%s' fd=%d: res=%d errno=%d - %s",cnt,myDevice,myFd,res,e,Strings::fromErrno(e))};
    }
    return *this;
}

SpiTransaction& SpiTransaction::clear()
{
    mySteps.clear();
    return *this;
}
