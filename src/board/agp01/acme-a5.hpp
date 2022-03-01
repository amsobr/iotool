#ifndef PINS_ACME_A5_H
#define PINS_ACME_A5_H

#include <string>

#include <common/pin_mapper.hpp>

static inline unsigned int PIN_A( int n) { return n; }
static inline unsigned int PIN_B( int n) { return 32+n; }
static inline unsigned int PIN_C( int n) { return 64+n; }
static inline unsigned int PIN_D( int n) { return 96+n; }
static inline unsigned int PIN_E( int n) { return 128+n; }

class AcmeA5PinMapper : public PinMapper
{
public:
    AcmeA5PinMapper() = default;

    ~AcmeA5PinMapper() override = default;

    std::string nameOf( unsigned int id ) override
    {
        if ( id>PIN_E(31) ) {
            return "";
        }
        if ( id>PIN_D(31) ) {
            return "pioE"+std::to_string(id%32);
        }
        else if ( id>PIN_C(31) ) {
            return "pioD"+std::to_string(id%32);
        }
        else if ( id>PIN_B(31) ) {
            return "pioC"+std::to_string(id%32);
        }
        else if ( id>PIN_A(31) ) {
            return "pioB"+std::to_string(id%32);
        }
        else {
            return "pioA"+std::to_string(id%32);
        }
    }

}; /* class AcmeA5PinMapper */





#endif /* ! defined(PINS_ACME_A5_H) */
