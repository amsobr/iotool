#ifndef ARP_QUERIER_HPP
#define ARP_QUERIER_HPP

#include <stdint.h>

namespace cbsl {
    
class UdpSkt;

/** \brief Find a MAC address using ARP queries
 * \param ipAddr IPv4 Address whose MAC should be found.
 * \param skt   Pointer to a socket on which the query will be performed
 * \param macAddr   Location where the MAC address will be saved into
 * \param iface     Interface on which the ARP entry is bound(??)
 * \param allowTx   Allows the querier to send a dummy packet to the host
 *                  to force a refresh in the ARP table
 * \param txDport   UDP destination port of the dummy transfer
 */
bool findMac( uint32_t ipAddr , UdpSkt *skt , uint64_t *macAddr , char const *iface , bool allowTx , bool txDport );

}; /* namespace Wcommon */

#endif /* !defined(ARP_QUERIER_HPP) */

