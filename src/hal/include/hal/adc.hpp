#ifndef ADC_HPP
#define ADC_HPP


#include <string>

class Adc
{
public:
    virtual unsigned int getNumChannels() const = 0;
    
    virtual std::string getVendor() const
    {
        return std::string("");
    }
    
    virtual std::string getModel() const
    {
        return "";
    }
    
    virtual std::string getHwVersion() const
    {
        return "";
    }
    
    virtual int getDriverVersion() const
    {
        return 0;
    }
    
    virtual std::string getUnits() const = 0;
    virtual double getResolution() const = 0;
   
    virtual std::string getChName( unsigned int chId ) const = 0;
    virtual void startConversion(unsigned int chId ) = 0;
    virtual long getRegisterValue() = 0;
    virtual double getValue() = 0;
    virtual double getUpperValue() const = 0;
    virtual double getLowerValue() const = 0;
    virtual bool isSingleEnded() const = 0;    
    
    
}; /* class Adc */


#endif /* ADC_HPP */
