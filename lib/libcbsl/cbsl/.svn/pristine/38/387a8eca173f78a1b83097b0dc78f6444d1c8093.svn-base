/**********************************************************************
 *
 * Filename:    crc.c
 * 
 * Description: Slow and fast implementations of the CRC standards.
 *
 * Notes:       The parameters for each supported CRC standard are
 *              defined in the header file crc.h.  The implementations
 *              here should stand up to further additions to that list.
 *
 * 
 * Copyright (c) 2000 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 **********************************************************************/

#include <stdint.h>

#include "ieee_crc32.h"

namespace Pti {
namespace Cksum {
namespace IeeeCrc32 {

enum {
    POLYNOMIAL          = 0x04C11DB7 ,
    INITIAL_REMAINDER   = 0xFFFFFFFF ,
    FINAL_XOR_VALUE     = 0xFFFFFFFF ,
    WIDTH               = 32         ,
    TOPBIT              = 1<<31      ,
};

static uint32_t  crcTable[256];
    
/*********************************************************************
 *
 * Function:    reflect()
 * 
 * Description: Reorder the bits of a binary sequence, by reflecting
 *              them about the middle position.
 *
 * Notes:       No checking is done that nBits <= 32.
 *
 * Returns:     The reflection of the original data.
 *
 *********************************************************************/
// static unsigned long reflect(unsigned long data, unsigned char nBits)
// {
//     unsigned long  reflection = 0x00000000;
//     unsigned char  bit;
// 
//     /* Reflect the data about the center bit. */
//     for (bit = 0; bit < nBits; ++bit) {
//         /* If the LSB bit is set, set the reflection of it. */
//         if (data & 0x01) {
//             reflection |= (1 << ((nBits - 1) - bit));
//         }
//         data = (data >> 1);
//     }
// 
//     return (reflection);
// }   /* reflect() */


/*********************************************************************
 *
 * Function:    crcSlow()
 * 
 * Description: Compute the CRC of a given message.
 *
 * Notes:       
 *
 * Returns:     The CRC of the message.
 *
 *********************************************************************/
uint32_t crc32Slow(char const *buf, unsigned int len)
{
    uint32_t       remainder = INITIAL_REMAINDER;
    unsigned int   byte;
    unsigned char  bit;


    /* Perform modulo-2 division, a byte at a time. */
    for (byte = 0; byte < len; ++byte) {
        /* Bring the next byte into the remainder. */
        remainder ^= (buf[byte] << (WIDTH - 8));
        /* Perform modulo-2 division, a bit at a time. */
        for (bit = 8; bit > 0; --bit) {
            /* Try to divide the current data bit. */
            if (remainder & TOPBIT) {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else {
                remainder = (remainder << 1);
            }
        }
    }

    /* The final remainder is the CRC result. */
    return (remainder ^ FINAL_XOR_VALUE);

}   /* crcSlow() */


/*********************************************************************
 *
 * Function:    crcInit()
 * 
 * Description: Populate the partial CRC lookup table.
 *
 * Notes:       This function must be rerun any time the CRC standard
 *              is changed.  If desired, it can be run "offline" and
 *              the table results stored in an embedded system's ROM.
 *
 * Returns:     None defined.
 *
 *********************************************************************/
void crc32Init()
{
    uint32_t       remainder;
    int            dividend;
    unsigned char  bit;


    /* Compute the remainder of each possible dividend. */
    for (dividend = 0; dividend < 256; ++dividend) {
        /* Start with the dividend followed by zeros. */
        remainder = dividend << (WIDTH - 8);

        /* Perform modulo-2 division, a bit at a time. */
        for (bit = 8; bit > 0; --bit) {
            /* Try to divide the current data bit. */         
            if (remainder & TOPBIT) {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else {
                remainder = (remainder << 1);
            }
        }

        /* Store the result into the table. */
        crcTable[dividend] = remainder;
    }
}   /* crcInit() */


/*********************************************************************
 *
 * Function:    crcFast()
 * 
 * Description: Compute the CRC of a given message.
 *
 * Notes:       crcInit() must be called first.
 *
 * Returns:     The CRC of the message.
 *
 *********************************************************************/
uint32_t crc32Fast(char const *buf, unsigned int len)
{
    uint32_t       remainder = INITIAL_REMAINDER;
    unsigned char  data;
    unsigned int   byte;

    /* Divide the message by the polynomial, a byte at a time. */
    for (byte = 0; byte < len; ++byte) {
        data = buf[byte] ^ (remainder >> (WIDTH - 8));
        remainder = crcTable[data] ^ (remainder << 8);
    }

    /* The final remainder is the CRC. */
    return (remainder ^ FINAL_XOR_VALUE);
}   /* crcFast() */


/* Start the calculation of a new incremental CRC. */
uint32_t crc32Start()
{
    return INITIAL_REMAINDER;
}


/* Update a CRC with values from a buffer.  */
uint32_t crc32Update( uint32_t *crc , char const *buf , unsigned int len )
{
    uint32_t remainder  = *crc;
    
    unsigned char data;
     /* Divide the message by the polynomial, a byte at a time. */
    for ( unsigned int i=0; i<len; ++i) {
        data = buf[i] ^ (remainder >> (WIDTH - 8));
        remainder = crcTable[data] ^ (remainder << 8);
    }
    
    *crc    = remainder;
    return remainder;
}


uint32_t crc32Finalize( uint32_t const *crc )
{
    return *crc ^ FINAL_XOR_VALUE;
}


}; /* namespace Pti::Cksum::IeeeCrc32 */
}; /* namespace Pti::Cksum */
}; /* namespace Pti */

