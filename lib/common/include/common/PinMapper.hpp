#ifndef PIN_MAPPER_HPP
#define PIN_MAPPER_HPP

#include <cassert>
#include <cstddef>
#include <string>
#include <memory>
#include <iostream>


class PinMapper
{
public:

    virtual ~PinMapper() = default;

    virtual std::string nameOf( unsigned int id) {
        std::cerr << "FATAL: method not implemented...\n";
        assert(0);
    }
}; /* class PinMapper */

typedef std::shared_ptr<PinMapper> PinMapperPtr;


#endif
