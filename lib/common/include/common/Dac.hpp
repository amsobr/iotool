#ifndef DAC_HPP
#define DAC_HPP


#include <string>

#include "Peripheral.hpp"
#include "PeripheralType.hpp"

class Dac : public Peripheral
{
public:
    ~Dac() override  = default;

    [[nodiscard]] virtual size_t getNumChannels() const = 0;
    
    /**
     * @brief get the output reference unit.
     *
     * @return The reference output unit of the unit.
     */
    [[nodiscard]] virtual std::string getUnit() const = 0;

    virtual int setOutput(unsigned int ch , int level ) = 0;

protected:
    explicit Dac( int id) :
        Peripheral(PeripheralType::DAC,id)
    {
    }
    
    
}; /* class Dac */

typedef std::shared_ptr<Dac> DacPtr;


#endif /* DAC_HPP */
