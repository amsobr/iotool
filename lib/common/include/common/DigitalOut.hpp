#pragma once

#include <cassert>
#include <string>
#include <list>
#include <memory>

#include "Peripheral.hpp"
#include "PeripheralType.hpp"

class DigitalOut : public Peripheral
{
public:
    struct Output {
        std::string const name;
        bool value;

        explicit Output( std::string n , bool v=false) : name{ std::move(n) } , value(v) {}
    };

    ~DigitalOut() override = default;

    
    [[nodiscard]] virtual std::vector<Output> getOutputs() const = 0;
    
    virtual int setOut( std::string name , bool value ) = 0;

protected:
    explicit DigitalOut(int id) :
        Peripheral(PeripheralType::DOUT,id)
    {
        assert(id>=0);
        
    }
    
}; /* class DigitalOut */

typedef std::shared_ptr<DigitalOut> DigitalOutPtr;


