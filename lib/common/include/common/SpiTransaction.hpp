//
// Created by to on 23-05-2022.
//


#pragma once

#include <string>
#include <cassert>
#include <cstdint>
#include <vector>


class SpiTransaction
{
public:
    enum class SpiMode
    {
        MODE_0  = 0  , ///< CPOL=0 , CPHA=0
        MODE_1  , ///< CPOL=0 , CPHA=1
        MODE_2  , ///< CPOL=1 , CPHA=0
        MODE_3    ///< CPOL=1 , CPHA=1
    };
    
    SpiTransaction( std::string device , SpiMode mode, int speed );
    
    ~SpiTransaction();
    
    SpiTransaction& transfer8( void const* wrData, void* rdData, int len);
    SpiTransaction& transfer16( void const* wrData, void* rdData, int len);
    SpiTransaction& transfer32( void const* wrData, void* rdData, int len);
    SpiTransaction& execute();
    SpiTransaction& clear();
    
    
    
    
private:
    struct Step
    {
        void const* wrBuf{ nullptr };
        void *      rdBuf{ nullptr };
        uint8_t     bitsPerWord{ 0 };
        int         len{ 0 };
        
        Step( void const* wr_ , void* rd_, int bpw, int len_ ) :
        wrBuf{ wr_ } ,
        rdBuf{ rd_ } ,
        bitsPerWord{ (uint8_t)(bpw&0xff) } ,
        len{ len_ }
        {
            assert( bpw==8 || bpw==16 || bpw==32 );
            assert( len>0 );
        }
        
        ~Step() = default;
        Step( Step const& other ) = default;
        Step& operator=( Step const& rhs )  = default;
    };
    
    std::string         myDevice;
    std::vector<Step>   mySteps{};
    int                 myFd{ 0 };

};
