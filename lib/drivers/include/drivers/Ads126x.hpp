#ifndef ADS126X_HPP
#define ADS126X_HPP

#include <memory>
#include <list>
#include <string>

#include <Poco/Logger.h>

#include <common/Adc.hpp>
#include <common/Peripheral.hpp>

#include "ads126x_config.hpp"

class SpiTransaction;

class Ads126x : public Adc
{
public:
    enum class Input {
        AIN0    = 0 ,
        AIN1    = 1 ,
        AIN2    = 2 ,
        AIN3    = 3 ,
        AIN4    = 4 ,
        AIN5    = 5 ,
        AIN6    = 6 ,
        AIN7    = 7 ,
        AIN8    = 8 ,
        AIN9    = 9 ,
        AINCOM  = 10 ,
        TSENS   = 11 ,
        AVCC    = 12 ,
        DVCC    = 13 ,
        TDAC    = 14 ,
        FLOAT   = 15 ,
    };
    
    
    Ads126x( int id , Ads126xConfig const &cfg );
    
    ~Ads126x() override;
   
    void calibrate();

    [[nodiscard]] std::string getVendor() const override;
    [[nodiscard]] std::string getModel() const override;
    [[nodiscard]] std::string getRevision() const override;
    [[nodiscard]] std::string getDriverVersion() const override;
    [[nodiscard]] std::string getAuthor() const override;
    [[nodiscard]] int getNumChannels() const override;
    
    /** \brief Read one channel, as digital
     * \param ch  ID of the channel: 0 to 9
     */
    long int readDigital( unsigned int ch ) override;

    double readDifferential( unsigned int chP , unsigned int chN ) override;

    /** \brief Read one channel, as real value
     * \details Read and convert the channel, referenced to
     * board input, considering PGA, and board gain.
     * \paran ch Channel to convert: 0 to 9
     */
    double readAnalog( unsigned int ch ) override;

    [[nodiscard]] std::string getUnits() const override;

    unsigned int setSampleRate( unsigned int sampleRate );

    [[nodiscard]] size_t getNumCurrentSources() const override;

    [[nodiscard]] std::list<std::string> getCurSourceMagnitudes( unsigned int srcId ) const override;

    int setCurrentSource(int srcId , bool enabled , int ch , std::string const& mag ) override;
    
    bool setOption(std::string const& optName, std::string const& optValue) override;
    
    std::string getOption(std::string const& optName) override;
    
    std::vector<OptionHelpPtr> getOptionHelp() override;
    
    int disableCurrentSource(int srcId) override;
    
private:
    void dumpRegisters( std::ostream& os );
    
    int32_t readData(SpiTransaction& spi, int adcId);
    
    
    /**
     * @brief Recalculate conversion delays.
     * Call this method once myCurFilterId and myCurSpsId are set
     * to recalculate the optimal delays to be used when polling
     * the ADC for conversion completion.
     */
    void recalculateDelays();
    
    [[nodiscard]] double getResolution() const;
    
    void startConversion( SpiTransaction& spi, unsigned int chId ) const;
    
    void startConversion( SpiTransaction& spi, unsigned int chp, unsigned int chn ) const;
    
    void setSps( std::string const &value );
    
    void setFilter(std::string const& value);
    
    
    Poco::Logger&       logger;
    std::string         mySpiDev;
    double              myVref;
    std::vector<double> myInpGain;
    std::list<std::string>  myIdacMagnitudes;
    std::list<std::string>  mySpsValues;
    std::list<std::string>  myFilterValues;
    
    std::string     myModel;
    std::string     myRevision;
    
    /* options */
    std::string     myOptSps; /*< Samples Per Second */
    std::string     myOptFilter; /*< Filter */
    int             myCurSpsId;
    int             myCurFilterId;
    int             myCurConvTime{};
    int             myCurConvPoll{};
}; /* class Ads126x */

typedef std::shared_ptr<Ads126x> Ads126xPtr;



#endif /* !defined ADS126X_HPP */
