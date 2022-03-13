#ifndef INA21X_HPP
#define INA21X_HPP

#include <string>
#include <memory>

#include <common/PowerMonitor.hpp>


class Ina21x: public PowerMonitor
{
public:
    struct Config {
        /** Name of the I2C device */
        std::string i2cDev;

        /** I2C address (7-bit) */
        int         i2cAddr;

        /** Shunt resistor value, in uOhm*/
        unsigned long int shuntResistance;

        /** Maximum readable current, in mA */
        unsigned long int maxCurrent;

        /** Maximum bus Voltage, in mV
         * 
         * @note Values beyond 32V are outside the chip's capabilities
         */
        unsigned long int maxVoltage;
    };

  
private:
    Config myConfig;

    class Impl;
    Impl *impl;

public:

    Ina21x( unsigned int id , Config const& config );

    virtual ~Ina21x();

    virtual std::string getVendor() const;
    virtual std::string getModel() const;
    virtual std::string getRevision() const;
    virtual std::string getDriverVersion() const ;
    virtual std::string getAuthor() const;

    int init();
    virtual size_t getNumChannels() const;

    virtual double getCurrent( unsigned int ch );
    virtual double getVoltage( unsigned int ch );
    virtual double getPower( unsigned int ch );


}; /* class Ina21x */

typedef std::shared_ptr<Ina21x> Ina21xPtr;



#endif /* !defined(INA21X_HPP) */

