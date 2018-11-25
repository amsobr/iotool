#ifndef ADS126X_CONFIG_HPP
#define ADS126X_CONFIG_HPP

#include <string>
#include <vector>

class Ads126xConfig
{
public:
    std::string spiDevice;
    std::vector<double> inpGain;

}; /* class Ads126xConfig */


#endif
