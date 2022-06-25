#ifndef ADC_HPP
#define ADC_HPP


#include <string>
#include <list>

#include "Peripheral.hpp"
#include "PeripheralType.hpp"

class Adc : public Peripheral
{
public:
    ~Adc() override = default;

    [[nodiscard]] virtual int getNumChannels() const = 0;
    [[nodiscard]] virtual double readAnalog(int ch) = 0;

    virtual int32_t read(int ch) = 0;
    
    /**
     * @brief read multiple values
     *
     * Reads the requested number of samples from the given channel.
     *
     * By default, iterates count times to fill the values vector. The default
     * implementation is quite simple; derived classes may override this to
     * provide optimized behaviour.
     *
     * @param ch The channel to read
     * @param count Number of samples to read
     * @param values A vector where samples will be saved. Its contents will be
     *              replaced with the samples actually read. Samples are stored
     *              in the same order as they are read.
     */
    virtual void read( int ch, int count, std::vector<int32_t>& values)
    {
        values.clear();
        values.reserve(count);
        for ( int i=0 ; i<count ; i++ ) {
            values.push_back(read(ch));
        }
    }

    [[nodiscard]] virtual std::string getUnits() const = 0;
    
    /**
     * @brief get the resolution of a channel
     *
     * Gets the resolution of the ADC channel. To calculate the actual analog
     * value of a channel, multiply the result of read by the resolution.
     *
     * @param ch the channel
     * @return The resolution of the channel
     */
    [[nodiscard]] virtual double getResolution( int ch) const = 0;

    /**
     * @brief Read a differential input
     * 
     * @param chp Positive input 
     * @param chn  Negative input
     * @return The sampled value.
     * @note On early versions, this method returns 0, leavind up to the
     *      client code to make sure that differential read is supported.
     */
    virtual double readDifferential(int chp , int chn ) { return 0; }

    /**
     * @brief Get the Number of current sources
     * 
     * @return The number of current sources. By default returns 0;
     */
    [[nodiscard]] virtual size_t getNumCurrentSources() const { return 0; }

    /**
     * @brief Get the list of supported magnitudes for a current source.
     * 
     * @param srcId ID of the current source
     * @return A list of the valid magnitudes for use with @a setCurrentSource().
     * By default returns an empty list.
     */
    [[nodiscard]] virtual std::list<std::string> getCurSourceMagnitudes( unsigned int srcId ) const { return {}; }
    
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
    virtual int setCurrentSource(int srcId , bool enabled , int ch , std::string const& mag ) = 0;
    
    /**
     * @brief disable a current source
     * @param srcId the ID of the source
     * @return 0 or TBD
     */
    virtual int disableCurrentSource(int srcId ) = 0;


protected:
    explicit Adc( int id ) :
        Peripheral(PeripheralType::ADC,id)
    {

    }


}; /* class Adc */

typedef std::shared_ptr<Adc> AdcPtr;


#endif /* ADC_HPP */
