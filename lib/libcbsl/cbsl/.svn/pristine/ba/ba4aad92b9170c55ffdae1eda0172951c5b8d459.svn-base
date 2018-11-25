#ifndef BASE64_H
#define BASE64_H

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else 
#define EXTERN_C extern
#endif

#include <inttypes.h>

EXTERN_C int base64_encode(void const *data_buf, size_t dataLength, char *result, size_t resultSize );

EXTERN_C int base64_decode(char const *in, unsigned char *out, size_t outLen);

#undef EXTERN_C

#endif /* BASE64_H */
