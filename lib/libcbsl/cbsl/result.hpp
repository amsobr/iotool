#ifndef CBSL_RESULT_HPP
#define CBSL_RESULT_HPP

namespace cbsl
{
    typedef enum
    {
        OK                  = 0 ,
        ERR_BAD_VALUE       = 1 ,
        ERR_BAD_STATE       = 2 ,
        ERR_IO              = 3 ,
        ERR_BUSY            = 4 ,
        ERR_NOT_FOUND       = 5 ,
    } Result;
}; /* namespace cbsl */


#endif /* !defined(CBSL_RESULT_HPP) */