#ifndef ADC_EMULATOR_HPP
#define ADC_EMULATOR_HPP


#include <string>
#include <list>

#include <Poco/Random.h>

#include <common/Adc.hpp>

class EmulatedAdc : public Adc
{
private:
    int myNumChannels;
    int myNumCurrentSources;
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


    [[nodiscard]] std::string getVendor() const override { return "SYNAPSENSE"; }
    
    [[nodiscard]] std::string getModel() const override { return "EMU-ADC-01"; }
    
    [[nodiscard]] std::string getRevision() const override { return "1"; }
    
    [[nodiscard]] std::string getDriverVersion() const override { return "1"; }

    
    [[nodiscard]] int getNumChannels() const override { return myNumChannels; }
    [[nodiscard]] double readAnalog(int ch) override;
    [[nodiscard]] int32_t read(int ch) override;
    [[nodiscard]] std::string getUnits() const override { return "V"; }
    
    double getResolution(int ch) const override;
    
    /**
     * @brief Read a differential input
     * 
     * @param chp Positive input 
     * @param chn  Negative input
     * @return The sampled value.
     * @note On early versions, this method returns 0, leavind up to the
     *      client code to make sure that differential read is supported.
     */
    [[nodiscard]] double readDifferential(int chp , int chn ) override;

    /**
     * @brief Get the Number of current sources
     * 
     * @return The number of current sources. By default returns 0;
     */
    [[nodiscard]] size_t getNumCurrentSources() const override { return myNumCurrentSources; }

    /**
     * @brief Get the list of supported magnitudes for a current source.
     * 
     * @param srcId ID of the current source
     * @return A list of the valid magnitudes for use with @a setCurrentSource().
     * By default returns an empty list.
     */
    [[nodiscard]] std::list<std::string> getCurSourceMagnitudes( unsigned int srcId ) const override;
    
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
    [[nodiscard]] int setCurrentSource(int srcId , bool enabled , int ch , std::string const& mag ) override
    {
        return 0;
    }
    
    int disableCurrentSource(int srcId) override
    {
        return 0;
    }
}; /* class EmulatedAdc */

typedef std::shared_ptr<EmulatedAdc> EmulatedAdcPtr;


#endif /* ADC_EMULATOR_HPP */
