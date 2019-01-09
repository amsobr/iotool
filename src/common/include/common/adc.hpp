#ifndef ADC_HPP
#define ADC_HPP


#include <string>
#include <list>

#include "peripheral.hpp"
#include "peripheral_type.hpp"

class Adc : public Peripheral
{
protected:
    Adc( unsigned int id) :
    Peripheral(PeripheralType::ADC,id)
    {

    }

public:
    virtual ~Adc()
    {

    }   

    virtual std::string getVendor() const = 0;
    
    virtual std::string getModel() const = 0;
    
    virtual std::string getRevision() const = 0;
    
    virtual std::string getDriverVersion() const = 0;

    
    virtual size_t getNumChannels() const = 0;
    virtual double readAnalog( unsigned int ch) = 0;
    virtual long int readDigital(unsigned int ch) = 0;
    virtual std::string getUnits() const = 0;

    /**
     * @brief Read a differential input
     * 
     * @param chp Positive input 
     * @param chn  Negative input
     * @return The sampled value.
     * @note On early versions, this method returns 0, leavind up to the
     *      client code to make sure that differential read is supported.
     */
    virtual double readDifferential( unsigned int chp , unsigned int chn ) { return 0; }

    /**
     * @brief Get the Number of current sources
     * 
     * @return The number of current sources. By default returns 0;
     */
    virtual size_t getNumCurrentSources() const { return 0; }

    /**
     * @brief Get the list of supported magnitudes for a current source.
     * 
     * @param srcId ID of the current source
     * @return A list of the valid magnitudes for use with @a setCurrentSource().
     * By default returns an empty list.
     */
    virtual std::list<std::string> getCurSourceMagnitudes( unsigned int srcId ) const { return std::list<std::string>(); }
    
    /**
     * @brief Configure a current source.
     * Configures (or disables) a current source.
     * 
     * Current sources drive the ADC pins and can be used as sensor excitation
     * 
     * @param srcId ID of the current source. Accepts values from 0 to @a numCurrentSources() -1
     * @param enabled Enables (true) or disables (false) the current source
     * @param ch The channel where the current source will be sourced to. Mandatory if enabled=true
     * @param mag Magnitude of the current source. Possible values are device dependent.
     * @return int 0 on success. Error codes TBD
     */
    virtual int setCurrentSource( unsigned int srcId , bool enabled , int ch=-1 , std::string mag="off" ) = 0;
}; /* class Adc */

typedef std::shared_ptr<Adc> AdcPtr;


#endif /* ADC_HPP */
