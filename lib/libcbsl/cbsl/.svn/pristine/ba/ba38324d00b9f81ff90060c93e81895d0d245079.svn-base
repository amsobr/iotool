#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <linux/if_arp.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "arp_querier.hpp"
#include "udp_skt.hpp"

namespace cbsl {
 
bool findMac( uint32_t ipAddr , UdpSkt *skt , uint64_t *macAddr , char const *iface , bool allowTx , bool txDport )
{

    if ( allowTx ) {

        char dummy[10]  = {0, 0, 0, 0, 0, 0, 0, 0, 0,0};
        if ( skt->sendMsg( ipAddr , txDport , dummy , sizeof(dummy) )<10 ) {
            int e(errno);
            printf( "ArpQuerier: dummy transfer failed... errno=%d\n" , e );
        }
    }

    struct arpreq myarp;
    memset(&myarp,0,sizeof(myarp));
    /* arp_pa must have the queried address */
    struct sockaddr_in *arp_inaddr  = (struct sockaddr_in*)&(myarp.arp_pa);
    arp_inaddr->sin_addr.s_addr     = htonl(ipAddr);
    arp_inaddr->sin_family          = AF_INET;

    strncpy( myarp.arp_dev, iface , sizeof(myarp.arp_dev)-1 );

    int res     = ioctl( skt->fd() , SIOCGARP, &myarp );
    if ( res == -1 ) {
        int code    = errno;
        printf( "ArpQuerier: No entry in arp_cache for ip=%08x errno=%d\n" , ipAddr , code );
        return false;
    }

    *macAddr        =     (uint64_t) myarp.arp_ha.sa_data[0] << 8*5
                        | (uint64_t) myarp.arp_ha.sa_data[1] << 8*4
                        | (uint64_t) myarp.arp_ha.sa_data[2] << 8*3
                        | (uint64_t) myarp.arp_ha.sa_data[3] << 8*2
                        | (uint64_t) myarp.arp_ha.sa_data[4] << 8*1
                        | (uint64_t) myarp.arp_ha.sa_data[5];

    if ( *macAddr==0 ) {
        printf( "ArpQuerier: Query for ip_addr %08x gave NULL MAC address.\n" , ipAddr );
        return false;
    }

    return true;
}

}; /* namespace Wcommon */


