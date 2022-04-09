#ifndef EMULATED_DAC_HPP
#define EMULATED_DAC_HPP

#include <string>
#include <memory>

#include <common/Dac.hpp>


class EmulatedDac: public Dac
{
private:
    unsigned int myNumChannels;

public:

    EmulatedDac( unsigned int id , unsigned int numChannels ) :
    Dac(id) ,
    myNumChannels(numChannels)
    {
    }

    virtual ~EmulatedDac()
    {
    }

    virtual std::string getVendor() const { return "SynapSense"; }
    virtual std::string getModel() const { return "EMU-DAC-01"; }
    virtual std::string getRevision() const { return "1"; }
    virtual std::string getDriverVersion() const { return "1.0.0"; }
    virtual std::string getAuthor() const { return "antonio.oliveira@example.com"; }

    virtual size_t getNumChannels() const { return myNumChannels; }
    
    std::string getUnit() const override { return "1mV"; }
    
    virtual int setOutput(unsigned int ch , int level ) { return 0; }
}; /* class MAX581x */

typedef std::shared_ptr<EmulatedDac> EmulatedDacPtr;



#endif /* !defined(EMULATED_DAC_HPP) */

