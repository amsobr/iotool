#pragma once

#include <string>
#include <list>
#include <memory>
#include <gpiod.hpp>

#include <common/Peripheral.hpp>
#include <common/PeripheralType.hpp>
#include <common/Indicator.hpp>

class Agp01Leds : public Indicator
{
public:
    explicit Agp01Leds(int id);
    
    ~Agp01Leds() override = default;

    [[nodiscard]] std::string getVendor() const override { return "Yet To Be Named..."; }
    
    [[nodiscard]] std::string getModel() const override { return "AGP01-STATUS-LEDS"; }
    
    [[nodiscard]] std::string getRevision() const override { return "1.0a"; }
    
    [[nodiscard]] std::string getDriverVersion() const override { return "1.0.0"; }
    
    [[nodiscard]] std::vector<Indicator::Led> getLeds() const override;
    
    int setLed( std::string const& name , bool value ) override;
    
private:
    struct LedHolder
    {
        std::string name;
        bool        state;
        gpiod::line io;
        
        LedHolder( std::string ledName, std::string const& ioName ) :
        name{ std::move(ledName) } ,
        state{false} ,
        io{ gpiod::find_line(ioName) }
        {
            gpiod::line_request req;
            req.request_type    = gpiod::line_request::DIRECTION_OUTPUT;
            req.consumer        = "agp01/led/" + name;
            io.request(req,0);
        }
    };
    
    gpiod::line     myLE; ///< Latch Enable, active high
    gpiod::line     myOE; ///< Output Enable, active low
    std::vector<LedHolder> myLeds;
}; /* class Agp01Indicators */

typedef std::shared_ptr<Agp01Leds> Agp01IndicatorsPtr;

