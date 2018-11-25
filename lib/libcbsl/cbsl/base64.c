#include <inttypes.h>
#include <string.h>

int base64_encode(const void* data_buf, size_t dataLength, char * result, size_t resultSize)
{
    const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const uint8_t *data = (const uint8_t *)data_buf;
    size_t resultIndex  = 0;
    int padCount        = dataLength % 3;
    uint8_t n0, n1, n2, n3;
    size_t inpIdx       = 0;
   
    /* for each 3 input bytes we get 4 output bytes. */
    if ( resultSize< (((dataLength/3)+1)*4+1) ) {
        return -1;
    }

   
    /* increment over the length of the string, three characters at a time */
    for ( inpIdx=0; inpIdx<dataLength-padCount ; inpIdx+=3 ) {
        /* these three 8-bit (ASCII) characters become one 24-bit number */
        uint32_t n;
        n =   (((uint32_t)data[inpIdx]) << 16)
          + (((uint32_t)data[inpIdx+1]) << 8)
          + data[inpIdx+2];

        /* this 24-bit number gets separated into four 6-bit numbers */
        n0 = (uint8_t)(n >> 18) & 63;
        n1 = (uint8_t)(n >> 12) & 63;
        n2 = (uint8_t)(n >> 6) & 63;
        n3 = (uint8_t)n & 63;
            
        result[resultIndex++] = base64chars[n0];
        result[resultIndex++] = base64chars[n1];
        result[resultIndex++] = base64chars[n2];
        result[resultIndex++] = base64chars[n3];
    }  

    /*
     * create and add padding that is required if we did not have a multiple of 3
     * number of characters available
     */
    if (padCount > 0) { 
        uint32_t n   = 0;
        if ( padCount==2 ) {
            n =   (((uint32_t)data[inpIdx]) << 16)
                + (((uint32_t)data[inpIdx+1]) << 8);
            n0 = (uint8_t)(n >> 18) & 63;
            n1 = (uint8_t)(n >> 12) & 63;
            n2 = (uint8_t)(n >> 6) & 63;
            result[resultIndex++] = base64chars[n0];
            result[resultIndex++] = base64chars[n1];
            result[resultIndex++] = base64chars[n2];
            result[resultIndex++] = '=';
        }
        else if ( padCount==1 ) {
            n =   ((uint32_t)data[inpIdx]) << 16;
            n0 = (uint8_t)(n >> 18) & 63;
            n1 = (uint8_t)(n >> 12) & 63;
            result[resultIndex++] = base64chars[n0];
            result[resultIndex++] = base64chars[n1];
            result[resultIndex++] = '=';
            result[resultIndex++] = '=';
        }
    }
    result[resultIndex++]   = '\0';
   return 0;   /* indicate success */
}

#define WHITESPACE 64
#define EQUALS     65
#define INVALID    66

static const unsigned char d[] = {
    66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
    54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66
};


size_t base64_decode (char *in, unsigned char *out, size_t outLen)
{ 
    size_t inpLen   = strlen(in);
    
    if ( inpLen > ((outLen/3)+1)*4 ) {
        return 0;
    }
    
    
    char *end = in + inpLen;
    char iter = 0;
    size_t buf = 0, len = 0;
    
    
    while (in < end) {
        unsigned char c = d[(int)*in++];
        
        switch (c) {
        case WHITESPACE: continue;   /* skip whitespace */
        case INVALID:    return 1;   /* invalid input, return error */
        case EQUALS:                 /* pad character, end of data */
            in = end;
            continue;
        default:
            buf = buf << 6 | c;
            iter++; // increment the number of iteration
            /* If the buffer is full, split it into bytes */
            if (iter == 4) {
                len += 3;
                *(out++) = (buf >> 16) & 255;
                *(out++) = (buf >> 8) & 255;
                *(out++) = buf & 255;
                buf = 0; iter = 0;

            }   
        }
    }
   
    if (iter == 3) {
        len += 2;
        *(out++) = (buf >> 10) & 255;
        *(out++) = (buf >> 2) & 255;
    }
    else if (iter == 2) {
        ++len;
        *(out++) = (buf >> 4) & 255;
    }

    return len; /* modify to reflect the actual output size */
}