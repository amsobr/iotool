#ifndef HOST_STATS_H
#define HOST_STATS_H

namespace Pti {
    

struct HostStats;

#if 0
struct GlobalInterfaceData
{
   unsigned int id;
   unsigned long network_5min_rx_bitrate;
   unsigned long network_5min_tx_bitrate;
   unsigned long network_5min_rx_packet_rate;
   unsigned long network_5min_tx_packet_rate;
   unsigned long network_15min_rx_bitrate;
   unsigned long network_15min_tx_bitrate;
   unsigned long network_15min_rx_packet_rate;
   unsigned long network_15min_tx_packet_rate;
};

/**
 * This structure is used to store, in a global scope, the Network info obtained by the Network Monitor
 */
struct GlobalNetworkData
{
   pthread_mutex_t access_mutex;
   unsigned int numInterfaces;
   struct GlobalInterfaceData* interfaces;
};

struct GlobalNetworkData GLOBAL_NETWORK_DATA;

struct InterfaceInfo
{
   unsigned int id;
   char* name;
   float rx_load;
   float tx_load;
   unsigned int rx_packet_rate;
   unsigned int tx_packet_rate;
};
#endif


#if 0
/**
 * This is the structure that the Network parser must fill
 */
struct NetworkParsingData
{
   unsigned long rx_bytes;
   unsigned long tx_bytes;
   unsigned long rx_packets;
   unsigned long tx_packets;
};
#endif

/** \brief Create a new monitor for CPU stats and initializes internal data */
extern HostStats *hostStatsCreate();

/** \brief Refresh CPU stats.
 * \details This function must be called before new stats cab be read
 * \return Returns 0 on success.
 */
extern int hostStatsRefresh(HostStats *stats);

/** \brief Get CPU Idle percentage.
 * \param stats Pointer to CPU statistics handler.
 * \param cur   Pointer to current CPU time. It is saved in units of 0.01%
 * \param total Pointer to total CPU idle time. Saved in units of 0.01%
 * \return Returns 0 on success
 */
extern int hostGetCpuIdle( HostStats const *stats , int *cur , int *total );

/** \brief Get Memory Usage data
 * \param freeMem Pointer to location where free memory will be saved.
 *      Information is saved in units of 0.01 %
 */
extern int hostGetMemFree( HostStats const *stats , int *freeMem );

/** \brief Get Filesystem usage 
 * \param usage Pointer to location where FS usage will be saved, in units of
 *  0.01%
 * \return returns 0
 */
extern int hostGetFsUsage( HostStats const *stats , int *usage);

/** \brief Destroy a CPU stats handler */
extern void hostStatsDestroy(HostStats *stats);

    
}; /* namespace Pti */


#endif /* !defined(HOST_STATS_H) */
