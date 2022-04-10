#pragma once

#include <string>
#include <list>
#include <vector>
#include <memory>
#include <tuple>

#include <gpiod.hpp>

#include <common/Peripheral.hpp>
#include <common/PeripheralType.hpp>
#include <common/DigitalOut.hpp>

class BasicDout : public DigitalOut
{
public:
    /**
     * @brief Digital Output configuration
     * @li first element: the name of the PIN
     * @li second element: the name of the GPIO that handles PIN with libgpiod
     * @li third element: initial state of the pin
     */
    using Config    = std::vector< std::tuple<std::string,std::string,bool> >;
//    using Output    = DigitalOut::Output;
    
    /**
     * @brief Create a simple digital output
     * @param id ID of the DigitalOut peripheral. Should be sequential within
     *          a board.
     * @param outs Vector of tuples with the configuration of the outputs.
     *              For each tuple:
     *              * the first element is the symbolic to assign the output
     *              * the second element is the name of the gpio that controls
     *                  the output through libgpiod
     *              * the third element is the initial state of the output
     */
    BasicDout(int id , Config const& outs );
    
    ~BasicDout() override = default;

    [[nodiscard]] std::string getVendor() const override { return "TBD"; }
    
    [[nodiscard]] std::string getModel() const override { return "BASIC Digital Out"; }
    
    [[nodiscard]] std::string getRevision() const override { return "1.0.0"; }
    
    [[nodiscard]] std::string getDriverVersion() const override { return "1.0.0"; }
    
    [[nodiscard]] std::string getAuthor() const override { return "amsobr@github"; }
    
    [[nodiscard]] std::vector<Output> getOutputs() const override;
    
    int setOut( std::string name , bool value ) override;

private:
    struct DoutHolder
    {
        std::string name;
        bool        state;
        gpiod::line io;
        
        DoutHolder( BasicDout const& self, std::string oName, std::string const& ioName , bool ini ) :
        name{ std::move(oName) } ,
        state{ini} ,
        io{ gpiod::find_line(ioName) }
        {
            gpiod::line_request req;
            req.request_type    = gpiod::line_request::DIRECTION_OUTPUT;
            req.consumer        = Poco::format("basic-dout/%d/%s",self.getId(),name);
            io.request(req,0);
            io.set_value(ini);
        }
    };
    
    std::vector<DoutHolder> myOutputs;
}; /* class Agp01Indicators */

typedef std::shared_ptr<BasicDout> BasicDoutPtr;

