//
// Created by to on 18-05-2022.
//

#include <sys/fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <cerrno>


#include <Poco/Format.h>

#include <common/Strings.hpp>
#include <common/I2cTransaction.hpp>


I2cTransaction::I2cTransaction(std::string charDev, uint16_t slaveAddr) :
myCharDev{ std::move(charDev) } ,
mySlaveAddress{ slaveAddr }
{
    myFd    = open( myCharDev.c_str() , O_RDWR );
    if ( myFd==-1 ) {
        int e = errno;
        throw std::runtime_error{ Poco::format("error opening '%s': errno %d - %s",myCharDev,e,Strings::fromErrno(e))};
    }
}

I2cTransaction::~I2cTransaction()
{
    close(myFd);
}

I2cTransaction& I2cTransaction::read(uint8_t* buf, int len)
{    
    mySteps.emplace_back(mySlaveAddress,I2C_M_RD,len,(uint8_t*)buf);    
    return *this;
}

I2cTransaction& I2cTransaction::write(uint8_t const* buf, int len)
{
    uint8_t* tmp    = const_cast<uint8_t*>(buf);
    mySteps.emplace_back(mySlaveAddress,0,len,tmp);
    return *this;
}

I2cTransaction& I2cTransaction::execute()
{
    struct i2c_rdwr_ioctl_data packets{};
    struct i2c_msg i2cMsg[mySteps.size()];
    
    for ( size_t i=0 ; i<mySteps.size() ; i++ ) {
        i2cMsg[i].addr  = mySteps[i].address;
        i2cMsg[i].flags = mySteps[i].flags;
        i2cMsg[i].len   = mySteps[i].len;
        i2cMsg[i].buf   = mySteps[i].buf;
    }
    
    packets.msgs      = &i2cMsg[0];
    packets.nmsgs     = mySteps.size();
    if(ioctl(myFd, I2C_RDWR, &packets)<0) {
        int e = errno;
        throw std::runtime_error{ Poco::format("%s: errno %d - %s",myCharDev,e,Strings::fromErrno(e))};
    }
    return *this;
}

I2cTransaction& I2cTransaction::clear()
{
    mySteps.clear();
    return *this;
}
