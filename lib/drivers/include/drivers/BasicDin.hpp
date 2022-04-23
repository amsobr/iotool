#pragma once

#include <string>
#include <list>
#include <vector>
#include <memory>
#include <tuple>

#include <gpiod.hpp>

#include <common/DigitalIn.hpp>

class BasicDin : public DigitalIn
{
public:
    /**
     * @brief Configuration flags for digital inputs
     */
    enum DinFlag
    {
        INVERTED    = 0x0001
    };
    
    /**
     * @brief Digital Input configuration
     * @li first element: the name of the PIN
     * @li second element: the name of the GPIO that handles PIN with libgpiod
     * @li third element: configuration flags
     */
    using Config    = std::vector< std::tuple<std::string,std::string,unsigned> >;
    
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
    BasicDin(int id , Config const& pins );
    
    ~BasicDin() override = default;

    [[nodiscard]] std::string getVendor() const override { return "amsobr"; }
    
    [[nodiscard]] std::string getModel() const override { return "BASIC Digital Input"; }
    
    [[nodiscard]] std::string getRevision() const override { return "1.0.0"; }
    
    [[nodiscard]] std::string getDriverVersion() const override { return "1.0.0"; }
    
    [[nodiscard]] std::string getAuthor() const override { return "amsobr@github"; }
    
    [[nodiscard]] std::vector<std::string> listNames() const override
    {
        return myNames;
    }
    
    [[nodiscard]] std::vector<Input> getAll() const override;
    
    [[nodiscard]] int get(std::string const& name) const override;

private:
    struct DinHolder
    {
        std::string         name;
        mutable gpiod::line io;
        bool                inverted;
        
        DinHolder( BasicDin const& self, std::string iName, std::string const& ioName, unsigned int flags ) :
        name{ std::move(iName) } ,
        io{ gpiod::find_line(ioName) } ,
        inverted{ (flags&DinFlag::INVERTED)!=0 }
        {
            gpiod::line_request req;
            req.request_type    = gpiod::line_request::DIRECTION_INPUT;
            req.consumer        = Poco::format("basic-din/%d/%s",self.getId(),name);
            io.request(req,0);
        }
    };
    
    std::vector<std::string> myNames;
    std::vector<DinHolder> myPins;
};

typedef std::shared_ptr<BasicDin> BasicDinPtr;

