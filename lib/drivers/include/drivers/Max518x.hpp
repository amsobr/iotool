#ifndef MAX581X_HPP
#define MAX581X_HPP

#include <string>
#include <memory>

#include <common/Dac.hpp>

class Max581x: public Dac
{
public:
    struct Config {
        /** Name of the I2C device */
        std::string i2cDev;

        /** I2C address (7-bit) */
        int         i2cAddr;

        /** Full-scale output, in mV */
        int         fullScale;
        double      outputGain;
    };
    
    enum RefMode
    {
        MAX581X_REF_EXT = 0x0   ,
        MAX581X_REF_2500= 0x1   ,
        MAX581X_REF_2048= 0x2   ,
        MAX581X_REF_4096= 0x3   ,
    };
    
    enum PowerMode {
        MAX581X_ON      = 0x0 ,
        MAX581X_OFF_1K  = 0x1 ,
        MAX581X_OFF_100K= 0x2 ,
        MAX581X_OFF_HIZ = 0x3 ,
    };

 
    Max581x(int id , Config  config );

    ~Max581x() override;

    [[nodiscard]] std::string getVendor() const override;
    
    [[nodiscard]] std::string getModel() const override;
    
    [[nodiscard]] std::string getRevision() const override;
    
    [[nodiscard]] std::string getDriverVersion() const override;
    
    [[nodiscard]] std::string getAuthor() const override;
    
    [[nodiscard]] std::string getUnit() const override;
    
    
    //int setPower( unsigned int ch , PowerMode mode );
    
    [[nodiscard]] size_t getNumChannels() const override { return 4; }
    
    int setOutput(unsigned int ch , int level ) override;
    
    //int setAutoStandby( bool autoStandBy );

private:
    struct ChipCommand;
    
    void runCommand( ChipCommand const& cmd ) const;
    
    [[nodiscard]] uint16_t calculateCode(unsigned int outputLevel ) const;
    
    void setReference(RefMode refMode);
    
    Config  myConfig;
    int     myEffVref{ 0 }; /*< Effective Reference voltage */
    
    
    
}; /* class MAX581x */

typedef std::shared_ptr<Max581x> Max581xPtr;



#endif /* !defined(MAX581X_HPP) */

