#ifndef ADC_EMULATOR_HPP
#define ADC_EMULATOR_HPP


#include <string>
#include <list>

#include <Poco/Random.h>

#include <common/Adc.hpp>

class EmulatedAdc : public Adc
{
private:
    unsigned int myNumChannels;
    unsigned int myNumCurrentSources;
    Poco::Random myRandom;

public:
    EmulatedAdc( int id , unsigned int numChannels) :
    Adc( id ) ,
    myNumChannels(numChannels) ,
    myNumCurrentSources(1) ,
    myRandom()
    {
        myRandom.seed();
    }


    ~EmulatedAdc() override = default;


    virtual std::string getVendor() const { return "SYNAPSENSE"; }
    
    virtual std::string getModel() const { return "EMU-ADC-01"; }
    
    virtual std::string getRevision() const { return "1"; }
    
    virtual std::string getDriverVersion() const { return "1"; }

    
    virtual size_t getNumChannels() const { return myNumChannels; }
    virtual double readAnalog( unsigned int ch);
    virtual long int readDigital(unsigned int ch);
    virtual std::string getUnits() const { return "V"; }

    /**
     * @brief Read a differential input
     * 
     * @param chp Positive input 
     * @param chn  Negative input
     * @return The sampled value.
     * @note On early versions, this method returns 0, leavind up to the
     *      client code to make sure that differential read is supported.
     */
    virtual double readDifferential( unsigned int chp , unsigned int chn );

    /**
     * @brief Get the Number of current sources
     * 
     * @return The number of current sources. By default returns 0;
     */
    virtual size_t getNumCurrentSources() const { return myNumCurrentSources; }

    /**
     * @brief Get the list of supported magnitudes for a current source.
     * 
     * @param srcId ID of the current source
     * @return A list of the valid magnitudes for use with @a setCurrentSource().
     * By default returns an empty list.
     */
    virtual std::list<std::string> getCurSourceMagnitudes( unsigned int srcId ) const;
    
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
    virtual int setCurrentSource( unsigned int srcId , bool enabled , int ch=-1 , std::string mag="off" ) { return 0; }
}; /* class EmulatedAdc */

typedef std::shared_ptr<EmulatedAdc> EmulatedAdcPtr;


#endif /* ADC_EMULATOR_HPP */
