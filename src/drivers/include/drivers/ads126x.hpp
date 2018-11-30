#ifndef ADS126X_HPP
#define ADS126X_HPP

#include <memory>

#include <common/adc.hpp>
#include <common/peripheral.hpp>

#include "ads126x_config.hpp"

typedef enum {
    ADS126x_AIN0    = 0 ,
    ADS126x_AIN1    = 1 ,
    ADS126x_AIN2    = 2 ,
    ADS126x_AIN3    = 3 ,
    ADS126x_AIN4    = 4 ,
    ADS126x_AIN5    = 5 ,
    ADS126x_AIN6    = 6 ,
    ADS126x_AIN7    = 7 ,
    ADS126x_AIN8    = 8 ,
    ADS126x_AIN9    = 9 ,
    ADS126x_AINCOM  = 10 ,
    ADS126x_TSENS   = 11 ,
    ADS126x_AVCC    = 12 ,
    ADS126x_DVCC    = 13 ,
    ADS126x_TDAC    = 14 ,
    ADS126x_FLOAT   = 15 ,
} Ads126xInput;

class Ads126x : public Adc
{
    class Ads126xImpl;
private:
    
    Ads126xImpl *impl;
    
public:
    Ads126x( unsigned int id , Ads126xConfig const &cfg );
    
    virtual ~Ads126x();
   
    void init();
    void calibrate();

    virtual std::string getVendor() const;
    virtual std::string getModel() const;
    virtual std::string getRevision() const;
    virtual std::string getDriverVersion() const;
    virtual std::string getAuthor() const;
    
    /** \brief Read one channel, as digital
     * \param ch  ID of the channel: 0 to 9
     */
    virtual long int readDigital( unsigned int ch );

    /** \brief Read one channel, as real value
     * \details Read and convert the channel, referenced to
     * board input, considering PGA, and board gain.
     * \paran ch Channel to convert: 0 to 9
     */
    virtual double readAnalog( unsigned int ch );

    std::string getUnits() const;

    unsigned int setSampleRate( unsigned int sampleRate );
}; /* class Ads126x */

typedef std::shared_ptr<Ads126x> Ads126xPtr;



#endif /* !defined ADS126X_HPP */
