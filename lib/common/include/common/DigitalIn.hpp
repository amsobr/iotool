#pragma once

#include <cassert>
#include <string>
#include <list>
#include <memory>

#include "Peripheral.hpp"
#include "PeripheralType.hpp"

class DigitalIn : public Peripheral
{
public:
    struct Input {
        std::string const name;
        bool value;

        explicit Input( std::string n , bool v=false) : name{ std::move(n) } , value(v) {}
    };

    ~DigitalIn() override = default;

    /**
     * @brief List all available inputs
     * @return a list of strings, each being the name of one input
     */
    [[nodiscard]] virtual std::vector<std::string> listNames() const = 0;
    
    /**
     * @brief get the state of all inputs
     *
     * Reads and returns the state of all inputs.
     *
     * @return A list of input state objects
     */
    [[nodiscard]] virtual std::vector<Input> getAll() const = 0;
    
    /**
     * @brief Get the current state of one specific input
     * @param name the name of the input
     * @return 0 if pin is low, 1 if pin is high
     */
    [[nodiscard]] virtual int get( std::string const& name ) const = 0;

protected:
    explicit DigitalIn(int id) :
        Peripheral(PeripheralType::DIN,id)
    {
        assert(id>=0);
    }
    
}; /* class DigitalIn */

typedef std::shared_ptr<DigitalIn> DigitalInPtr;

