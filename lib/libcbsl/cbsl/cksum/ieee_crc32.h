#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>
namespace Pti {
namespace Cksum {
namespace IeeeCrc32 {

extern void crc32Init();

extern uint32_t crc32Fast( char const *buf , unsigned int len );

/** slowCrc32 does not require table lookups */
extern uint32_t crc32Slow( char const *buf , unsigned int len );


/** \brief Starts the calculation of a new incremental CRC.
 */
extern uint32_t crc32Start();

/** \brief Update a CRC with values from a buffer.
 * \details Updates \a crc with the data from a buffer pointed to by \a buf
 * with size \a len. The crc pointed to by \a crc is updated. Also returns
 * the final value of the CRC.
 */
extern uint32_t crc32Update( uint32_t *crc , char const *buf , unsigned int len );

/** \brief Return the final value of the CRC.
 * \details Return the final value of the CRC. The crc may still be updated
 * with calls to crc32Update. The value pointed to by \a crc is not modified.
 */
extern uint32_t crc32Finalize( uint32_t const *crc );

}; /* namespace Pti::Cksum::IeeeCrc32 */
}; /* namespace Pti::Cksum */
}; /* namespace Pti */

#endif /* !defined( CRC32_H ) */
