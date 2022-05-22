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

  
    Ina21x(int id , Config  config );

    ~Ina21x() override;

    [[nodiscard]] std::string getVendor() const override;
    [[nodiscard]] std::string getModel() const override;
    [[nodiscard]] std::string getRevision() const override;
    [[nodiscard]] std::string getDriverVersion() const override;
    [[nodiscard]] std::string getAuthor() const override;

    [[nodiscard]] size_t getNumChannels() const override;

    [[nodiscard]] double getCurrent( unsigned int ch ) override;
    [[nodiscard]] double getVoltage( unsigned int ch ) override;
    [[nodiscard]] double getPower( unsigned int ch ) override;

private:
    [[nodiscard]] uint16_t readRegister( uint8_t address ) const;
    
    void writeRegister( uint8_t address , uint16_t value ) const;
    
    Config myConfig;
    
    
    
}; /* class Ina21x */

typedef std::shared_ptr<Ina21x> Ina21xPtr;



#endif /* !defined(INA21X_HPP) */

