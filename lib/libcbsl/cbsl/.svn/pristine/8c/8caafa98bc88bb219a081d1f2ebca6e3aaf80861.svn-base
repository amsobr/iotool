#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "addrmap.h"

addr_map_t *open_mem_map( void *basePhysAddr, long int length){
	char const *fName="/dev/mem";
	long int   RealStart, RealLen;
	long int givenBaseAddr	= (long int)basePhysAddr;
	
	addr_map_t	*pAddrMap	= (addr_map_t*)malloc( sizeof(addr_map_t) );
	
	if ( NULL == pAddrMap ) {
        fprintf( stderr , "Unable to create a memory map [phys=%p]\n",basePhysAddr );
		exit(-1);
	}
	
	if ((pAddrMap->fd = open(fName, O_RDWR)) < 0){
		fprintf( stderr , "Unable to open /dev/mem!\n" );
		free(pAddrMap);
		exit(-1);
	}
	
	RealStart = givenBaseAddr & ~(getpagesize()-1);
	RealLen   = length + (givenBaseAddr - RealStart);
	
	pAddrMap->baseVirtAddr= mmap(NULL, RealLen, PROT_READ|PROT_WRITE, MAP_SHARED, pAddrMap->fd, RealStart);
	pAddrMap->length  	= RealLen;
	pAddrMap->parent	= getpid();
	
	if ( RealStart != givenBaseAddr ) {
		fprintf( stderr , "Actual map start address ajusted from %p to %p\n"
                          "to meet page alignment constraints.\n"
                          "PAGESIZE          = 0x%08x\n"
                          "BASE (input/real) = 0x%08lx / 0x%08lx\n"
                          "SIZE (input/real) = 0x%08lx / 0x%08lx\n"
                          , (void*)basePhysAddr , (void*)RealStart , getpagesize()
                 , givenBaseAddr , RealStart , length  , RealLen
               );
	}
	
	if (MAP_FAILED == pAddrMap->baseVirtAddr ) {
		free( pAddrMap );
		exit(-1);
	}
	pAddrMap->requestedVirtAddr	= pAddrMap->baseVirtAddr + (givenBaseAddr - RealStart);
	return pAddrMap;
}

int close_mem_map(addr_map_t *pAddrMap)
{
	if ( pAddrMap->parent != getpid() ) {
		fprintf( stderr , "Closing AddrMap created by other process (parent=%d)\n",
			 pAddrMap->parent );
	}
	int RetValue	= munmap(pAddrMap->baseVirtAddr, pAddrMap->length); 
	close(pAddrMap->fd);
	free(pAddrMap);
	if ( 0==RetValue )
		return ADDRUNMAP_OK;
	
	return ADDRUNMAP_ERROR;
}
