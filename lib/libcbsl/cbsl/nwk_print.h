#ifndef NWK_PRINT_H_
#define NWK_PRINT_H_

#define __STDC_FORMAT_MACROS 1
#include <inttypes.h>
#include <stdint.h>  /* Common data type definitions */
#include <stdio.h>

/** \brief Print 64bit extended MAC address into a string
 * \param mac Extended MAC address - 8 bytes
 * \param str Destination string
 * \param len Maximum length of the string, including terminating NULL
 * \return Returns the number of characters print into the string.
 */
static inline size_t
nwk_print_mac64(uint64_t mac, char *str, size_t len)
{
  return snprintf(str, len, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
                  (uint8_t) (mac >> (8 * 7)) & 0xff,
                  (uint8_t) (mac >> (8 * 6)) & 0xff,
                  (uint8_t) (mac >> (8 * 5)) & 0xff,
                  (uint8_t) (mac >> (8 * 4)) & 0xff,
                  (uint8_t) (mac >> (8 * 3)) & 0xff,
                  (uint8_t) (mac >> (8 * 2)) & 0xff,
                  (uint8_t) (mac >> (8 * 1)) & 0xff,
                  (uint8_t) (mac >> (8 * 0)) & 0xff);
}

/** \brief Print 48bit extended MAC address into a string
 * \param mac Extended MAC address - 8 bytes
 * \param str Destination string
 * \param len Maximum length of the string, including terminating NULL
 * \return Returns the number of characters print into the string.
 */
static inline size_t
nwk_print_mac48(uint64_t mac, char *str, size_t len)
{
  return snprintf(
          str, len,
          "%02x:%02x:%02x:%02x:%02x:%02x",
          (uint8_t) (mac >> (8 * 5)) & 0xff,
          (uint8_t) (mac >> (8 * 4)) & 0xff,
          (uint8_t) (mac >> (8 * 3)) & 0xff,
          (uint8_t) (mac >> (8 * 2)) & 0xff,
          (uint8_t) (mac >> (8 * 1)) & 0xff,
          (uint8_t) (mac >> (8 * 0)) & 0xff);
}

/** \brief print IP address into a string.
 * \param ip IP address to print
 * \param str Buffer to hold printed data
 * \param len Maximum length of the generated string, including a the terminating NULL
 */
static inline int
nwk_print_ip(uint32_t ip, char *str, size_t len)
{
  return snprintf(str, len, "%u.%u.%u.%u", (ip >> 24) & 0xff, (ip >> 16) & 0xff,
                  (ip >> 8) & 0xff, (ip) & 0xff);
}

#endif /* PLC_FW_LDR_H_ */
