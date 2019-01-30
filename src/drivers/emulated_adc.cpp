

#include <Poco/Random.h>

#include <emulated_adc.hpp>

static double ADC_FULL_SCALE = 5.0;
static long int ADC_MAX_CODE = 0x7fffffff;


double EmulatedAdc::readAnalog( unsigned int ch)
{
    if ( ch>=myNumChannels ) { return 0.0; }

    return myRandom.nextDouble()*ADC_FULL_SCALE;
}

long int EmulatedAdc::readDigital(unsigned int ch)
{
    if ( ch!=myNumChannels ) {
        return 0;
    }

    return (2*myRandom.nextDouble()-1.0)*ADC_MAX_CODE;
}


double EmulatedAdc::readDifferential( unsigned int chp , unsigned int chn )
{
    return readAnalog(chp)-readAnalog(chn);
}


std::list<std::string> EmulatedAdc::getCurSourceMagnitudes( unsigned int srcId ) const
{
    return { "1u" , "2u" , "5u" , "10u" , "20u" , "50u" , "100u" , "200u" , "500u" , "1m" , "2m" , "5m" };
}

