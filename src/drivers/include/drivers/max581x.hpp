#ifndef MAX581X_HPP
#define MAX581X_HPP

#include <string>
#include <memory>

#include <common/Dac.hpp>


class Max581x: public Dac
{
public:
    struct Config {
        /** Name of the I2C device */
        std::string i2cDev;

        /** I2C address (7-bit) */
        int         i2cAddr;

        /** Full-scale output, in mV */
        unsigned int fullScale;
        double      outputGain;
    };

    enum PowerMode {
        MAX581X_ON      = 0x0 ,
        MAX581X_OFF_1K  = 0x1 ,
        MAX581X_OFF_100K= 0x2 ,
        MAX581X_OFF_HIZ = 0x3 ,
    };

  
private:
    Config myConfig;

    class Impl;
    Impl *impl;

public:

    Max581x( unsigned int id , Config const& config );

    virtual ~Max581x();

    virtual std::string getVendor() const;
    virtual std::string getModel() const;
    virtual std::string getRevision() const;
    virtual std::string getDriverVersion() const ;
    virtual std::string getAuthor() const;

    int init();
    int setPower( unsigned int ch , PowerMode mode );
    virtual size_t getNumChannels() const { return 4; }
    virtual int setOutput(unsigned int ch , int level );
    int setAutoStandby( bool autoStandBy );
}; /* class MAX581x */

typedef std::shared_ptr<Max581x> Max581xPtr;



#endif /* !defined(MAX581X_HPP) */

