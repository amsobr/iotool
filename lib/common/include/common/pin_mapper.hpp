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

    virtual ~PinMapper() {}

    virtual std::string nameOf( unsigned int id) {
        std::cerr << "OOOOOOPS!!! should not enter here.....\n";
        assert(0);
        return {};
    }
}; /* class PinMapper */

typedef std::shared_ptr<PinMapper> PinMapperPtr;


#endif
