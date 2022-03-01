//
// Created by to on 23/02/22.
//


#pragma once

#include <map>
#include <string>

#include "Peripheral.hpp"

/**
 * @brief maps peripherals to symbolic names
 */
class DeviceMapper
{
public:
    DeviceMapper()  = default;
    ~DeviceMapper() = default;
    
    void set( std::string const& name , PeripheralPtr p )
    {
        myMap[name] = std::move(p);
    }
    
    [[nodiscard]] PeripheralPtr getPeripheral( std::string const& name ) const
    {
        try {
            return myMap.at(name);
        }
        catch( ... ) {
            return nullptr;
        }
    }
    
private:
    std::map< std::string,PeripheralPtr > myMap;
};

typedef std::shared_ptr<DeviceMapper> DeviceMapperPtr;



