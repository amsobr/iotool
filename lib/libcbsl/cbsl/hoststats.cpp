/**
 * globaldefs.c
 *
 *  Created on: Mar 21, 2012
 *      Author: Daniel Figueira
 * Ported to libpti by Antonio Oliveira - 2012/06/12
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/vfs.h>

#include "hoststats.h"

static inline bool STR_EQ(char const *s1 , char const *s2 ) {
    return (strcmp(s1,s2)==0);
}


namespace Pti {
    
    
struct CpuTimes {
    unsigned long long user;
    unsigned long long nice;
    unsigned long long sys;
    unsigned long long idle;
    unsigned long long ioWait;    
    unsigned long long irq;
    unsigned long long softIrq;
    unsigned long long steal;
    unsigned long long guest;
    unsigned long long total;
};

struct MemStats {
   unsigned long total;
   unsigned long free;
//    unsigned long used;
   unsigned long cached;
   unsigned long committed;
};


struct FsStats {
    unsigned int bTotal;
    unsigned int bFree;
    unsigned int bUFree;      
};


struct HostStats {    
    CpuTimes prev;
    CpuTimes cur;
    MemStats mem;
    FsStats  fs;
};


#define PROC_STAT_PATH "/proc/stat"
#define PROC_MEMINFO_PATH "/proc/meminfo"


HostStats *hostStatsCreate()
{
    HostStats *p = new HostStats;
    memset(p,0,sizeof(HostStats));
    return p;
}


int hostStatsRefresh(HostStats *stats)
{
    FILE *f;
 
    f = fopen(PROC_STAT_PATH, "r");
    if ( f==NULL ) {
        perror( "Unable to open file.\n" );
        return -1;
    }
    
    memcpy(&(stats->prev) , &(stats->cur) , sizeof(CpuTimes) );
    
    fscanf( f , "%*10s %10llu %10llu %10llu %10llu %10llu %10llu %10llu %10llu %10llu",
            &(stats->cur.user)   ,
            &(stats->cur.nice)   ,
            &(stats->cur.sys )   ,
            &(stats->cur.idle)   ,
            &(stats->cur.ioWait) ,
            &(stats->cur.irq)    ,
            &(stats->cur.softIrq),
            &(stats->cur.steal)  ,
            &(stats->cur.guest)
    );
    fclose(f);
    stats->cur.total    =     stats->cur.user
                            + stats->cur.nice
                            + stats->cur.sys
                            + stats->cur.idle
                            + stats->cur.ioWait
                            + stats->cur.irq
                            + stats->cur.softIrq
                            + stats->cur.steal
                            + stats->cur.guest
                            ;

    
    f    = fopen(PROC_MEMINFO_PATH, "r");
    if ( f==NULL) {
        perror( "Unable to open mem info file.\n" );
        return -1;
    }

    int remaining   = 4;
    int ret;
    char memTag[32];
    memset( memTag , 0 , 32 );
    do {
        unsigned long int val;
        ret     = fscanf(f, "%31s %10lu", memTag , &val);

        if ( STR_EQ(memTag,"MemTotal:") ) {
            stats->mem.total        = val;
            --remaining;
        }
        else if ( STR_EQ(memTag,"MemFree:") ) {
            stats->mem.free         = val;
            --remaining;
        }
        else if ( STR_EQ(memTag,"Cached:") ) {
            stats->mem.cached       = val;
            --remaining;
        }
        else if ( STR_EQ(memTag,"Committed_AS:") ) {
            stats->mem.committed    = val;
         --remaining;
        }
        memTag[0]    = '\0';
    } while ( ret!=EOF && remaining>0 );
    fclose(f);
    
    struct statfs fsInfo;
    statfs( "/" , &fsInfo );
    stats->fs.bTotal    = fsInfo.f_blocks;
    stats->fs.bFree     = fsInfo.f_bfree;
    stats->fs.bUFree    = fsInfo.f_bavail;    

   return 0;
}


int hostGetCpuIdle( HostStats const *stats , int *cur , int *total )
{
    unsigned int dTot   = stats->cur.total - stats->prev.total;
    unsigned int dIdl   = stats->cur.idle  - stats->prev.idle;
    
    *cur    = (dIdl*10000)/dTot;
    *total  = (stats->cur.idle*10000)/stats->cur.total;    
    return 0;
}


int hostGetMemFree( HostStats const *stats , int *freeMem )
{
    *freeMem    = (stats->mem.free*10000)/stats->mem.total;
    return 0;
}


int hostGetFsUsage( HostStats const *stats , int *usage)
{
    *usage      = 10000 - ((stats->fs.bUFree*10000)/stats->fs.bTotal);
    return 0;
}


void hostStatsDestroy(HostStats *stats)
{
    delete stats;
}


#if 0
struct CpuLoadParsingData GetCurrentCpuLoad()
{
   FILE *f = popen("uptime", "r");

   struct CpuLoadParsingData cpu_load = { 0 };

   fscanf(f, "%*s %*s %*d %*s %*s %*s %f%*s %f%*s %f", &cpu_load.cpuLoad1min, &cpu_load.cpuLoad5min, &cpu_load.cpuLoad15min);
   fclose(f);

   return cpu_load;
}



struct NetworkParsingData GetCurrentInterfaceUsage(int interface_id)
{
   response_t *response;
   brg_xml_path_t path;
   xmlNodePtr root;
   struct NetworkParsingData interface_info = {0};
   unsigned long rx_bytes, rx_packets, tx_bytes, tx_packets;
   int err = 0;

   mgmt_get(mgmt_printf_path(path, "/interfaces/interface[@id=\"%d\"]/counters/emac/rx", interface_id));
   mgmt_get(mgmt_printf_path(path, "/interfaces/interface[@id=\"%d\"]/counters/emac/tx", interface_id));

   response = mgmt_submit_query();
   if (!response || response->rc)
   {
      LOGTRACE(TRACE_MODULE_ALL | TRACE_LAYER_APP, ERROR_SEVERITY_ERROR, "Invalid BRG response");
   }

   //Get rx counters
   brg_xml_get_root_element(response->command_responses[0].bodies[0], &root);
   rx_bytes = brg_xml_get_path_uint(root, "/byte", &err);
   if (err != 0)
   {
      goto Error;
   }
   rx_packets = brg_xml_get_path_uint(root, "/packet", &err);
   if (err != 0)
   {
      goto Error;
   }

   //Get tx counters
   brg_xml_get_root_element(response->command_responses[1].bodies[0], &root);
   tx_bytes = brg_xml_get_path_uint(root, "/byte", &err);
   if (err != 0)
   {
      goto Error;
   }
   tx_packets = brg_xml_get_path_uint(root, "/packet", &err);
   if (err != 0)
   {
      goto Error;
   }

   interface_info.rx_bytes = rx_bytes;
   interface_info.rx_packets = rx_packets;
   interface_info.tx_bytes = tx_bytes;
   interface_info.tx_packets = tx_packets;

   return interface_info;

Error:

   LOGTRACE(TRACE_MODULE_ALL | TRACE_LAYER_APP, ERROR_SEVERITY_ERROR, "%s: Error while querying BRG for interface %d", __FUNCTION__, interface_id);
   return interface_info;
}

#endif

}; /* namespace Pti */
