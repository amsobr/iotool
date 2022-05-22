//
// Created by to on 18-05-2022.
//


#pragma once

#include <cstdint>
#include <string>
#include <vector>


class I2cTransaction
{
public:
    /**
     * @brief Create an I2C transaction
     * @param charDev Name of the character device
     * @param slaveAddr 7 bit address of the slave device
     */
    I2cTransaction( std::string charDev , uint16_t slaveAddr  );
    
    /**
     * @brief destroy a transaction
     */
    ~I2cTransaction();
    
    /**
     * @brief add a read step
     *
     * Adds a a read step to the transaction. The actual read will only be
     * executed on a call to execute().
     *
     * @param buf buffer which will hold read data
     * @param len length of the read, in bytes
     * @return the transaction itself. May be chained with additional method
     *          calls.
     */
    I2cTransaction& read(uint8_t* buf, int len );
    
    /**
     * @brief add a write step
     *
     * Adds a write step to the transaction. The actual write will only be
     * executed on a call to execute()
     *
     * @param buf buffer holding the data to be written
     * @param len number of bytes to write
     * @return The transaction itself.
     */
    I2cTransaction& write(uint8_t const* buf , int len );
    
    /**
     * @brief execyte the transaction.
     *
     * Executes the all steps of the transaction. The buffers provided
     * while setting up the transaction are written or read as requested.
     * If the transaction is empty, this method has no effect.
     *
     * The same transaction can be executed any number of times without setting up
     * the steps from start. Use clear() to remove all steps from the transaction.
     *
     * @note This is the only method when actual communication with the slave
     *      device takes place over the I^2C bus.

     * @return The transaction itself.
     * @throws runtime_error - some error occurrs while executing the I2C transaction.
     */
    I2cTransaction& execute();
    
    /**
     * @brief Clear the transaction. All steps are removed.
     * @return The transaction itself.
     */
    I2cTransaction& clear();
    
    
private:
    struct Step
    {
        uint16_t address;
        uint16_t flags;
        uint16_t len;
        uint8_t* buf;
        
        Step( uint16_t addr_, uint16_t flags_, uint16_t len_, uint8_t* buf_  ) :
        address{ addr_ } ,
        flags{ flags_ } ,
        len{ len_ } ,
        buf{ buf_ }
        {
        }
        
        ~Step() = default;
        
        Step( Step const& rhs ) = default;
        Step& operator=( Step const& rhs ) = default;
    };
    
    std::string         myCharDev;
    uint16_t            mySlaveAddress;
    int                 myFd;
    std::vector<Step>   mySteps;
};
