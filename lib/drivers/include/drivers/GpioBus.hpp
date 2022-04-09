
#pragma once

#include <memory>
#include <vector>
#include <string>
#include <list>
#include <memory>

#include <gpiod.hpp>


class GpioBus
{
public:
    
    enum class Direction {
        OUTPUT ,
        INPUT
    };
    
    
    /**
     * @brief Create a GPIO bus
     *
     * Creates a GPIO bus using the symbolic name of the GPIO lines that
     * constitute the bus. The line at position 0 is the MSB.
     * @param name The name of the bus
     * @param lines Array of the pins that form the bus. Position 0 is MSB
     * @param dir Bus direction. Default is input
     * @param value Initial value. Initial is all '0'
     */
    GpioBus( std::string name , std::vector<std::string> lines, Direction dir=Direction::INPUT , uint32_t value=0 );
    
    /**
     * @brief Create a GPIO bus
     *
     * Creates a GPIO bus from an array of lines. The line at index 0 of the
     * array is the least-signigicant bit, and so forth, until the line at the
     * last position of the array, which will be the most significant bit of
     * the bus.
     * @param name Name of the bus
     * @param lines List of lines contained by the BUS.
     * @param direction Direction of the bus. By default bus is set as input
     * @þaram value Initial value of the bus. Ignored if direction is input.
     *          Bits above the most significant bit are ignored.
     */
    GpioBus( std::string name , std::vector<gpiod::line> lines , Direction dir=Direction::INPUT, uint32_t value=0 );
    
    /**
     * @brief Destroy a GPIO Bus
     */
    ~GpioBus();

    /**
     * @brief Set the direction and value of the bus
     *
     * Sets the direction and optional value of the bus.
     *
     * @param dir Direction.
     * @param value Value to write into the lines. Bits above MSB are ignored.
     *              Value is ignored when setting direction to Input
     * @return true on success, otherwise, false
     */
    bool setDirection( Direction dir , uint32_t value=0 );
    
    /**
     * @brief Set the bus value.
     *
     * Sets the bus latch to the provided value.
     *
     * @note The behaviour is not entirely defined in case value is set before
     *      setting the direction of the bus...
     * @param value The value to write on the bus.
     * @return true on sucess, otherwise, false
     */
    bool setValue( uint32_t value );
    
    
    /**
     * @brief Read the current state of the bus
     * @return The current value of the bus lines
     */
    uint32_t readValue();
    
    [[nodiscard]] std::string const& getName() const
    {
        return myName;
    }

private:
    std::string                 myName;
    std::vector<gpiod::line>    myLines;
}; /* class GpioBus */

typedef std::shared_ptr<GpioBus> GpioBusPtr;

