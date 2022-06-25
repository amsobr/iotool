#ifndef ADS126X_HPP
#define ADS126X_HPP

#include <memory>
#include <list>
#include <string>

#include <Poco/Logger.h>

#include <common/Adc.hpp>
#include <common/Peripheral.hpp>
#include <common/SpiTransaction.hpp>

#include "ads126x_config.hpp"

class SpiTransaction;

class Ads126x : public Adc
{
public:
    
    Ads126x( int id , Ads126xConfig const &cfg );
    
    ~Ads126x() override;
   
    void calibrate();

    [[nodiscard]] std::string getVendor() const override;
    [[nodiscard]] std::string getModel() const override;
    [[nodiscard]] std::string getRevision() const override;
    [[nodiscard]] std::string getDriverVersion() const override;
    [[nodiscard]] std::string getAuthor() const override;
    [[nodiscard]] int getNumChannels() const override;
    
    /**
     * @brief get the name of an input
     * @param idx index of the input, from 0 to getNumChannels()-1, inclusive
     * @return the name of the input
     * @throws runtime_error - index out of bounds
     */
    [[nodiscard]] std::string getInputName( int idx ) const;
    
    
    /** \brief Read one channel, as digital
     * \param ch  ID of the channel: 0 to 9
     */
    int32_t read(int ch ) override;

    double readDifferential(int chp , int chn ) override;

    /** \brief Read one channel, as real value
     * \details Read and convert the channel, referenced to
     * board input, considering PGA, and board gain.
     * \paran ch Channel to convert: 0 to 9
     */
    double readAnalog(int ch ) override;

    [[nodiscard]] std::string getUnits() const override;
    
    void read(int ch, int count, std::vector<int32_t>& values) override;
    
    double getResolution(int ch) const override;
    
    
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
    
    void startConversion(int ch);
    
    void startConversion(int chp, int chn);
    
    void setSps( std::string const &value );
    
    void setFilter(std::string const& value);
    
    
    Poco::Logger&       logger;
    std::string         mySpiCharDev;
    SpiTransaction      mySpi;
    double              myVref;
    std::vector<double> myInpGain;
    std::list<std::string>  myIdacMagnitudes;
    std::list<std::string>  mySpsValues;
    std::list<std::string>  myFilterValues;
    int                 myReadCount{ 0 };
    
    std::string     myModel;
    std::string     myRevision;
    
    /* options */
    std::string     myOptSps; /*< Samples Per Second */
    std::string     myOptFilter; /*< Filter */
    int             myCurSpsId;
    int             myCurFilterId;
    int             myCurChp{ -1 };
    int             myCurChn{ -1 };
    int             myCurConvTime{};
    int             myCurConvPoll{};
}; /* class Ads126x */

typedef std::shared_ptr<Ads126x> Ads126xPtr;



#endif /* !defined ADS126X_HPP */
