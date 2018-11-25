/** \file	addrmap.h
 * \brief	Module to create and destroy virtual<->physical memory maps
 * \details	This module contais wrappers arounf mmap and munmap to ease
 *			the creation and destruction of memory maps.
 */

#ifndef __ADDRMAP_H__
#define __ADDRMAP_H__

#include <sys/types.h>

#undef ADDRMAP_EXTERN
#ifdef __cplusplus
#define	ADDRMAP_EXTERN	extern "C"
#else
#define ADDRMAP_EXTERN extern
#endif

/** \brief Physical memory map descriptor. */
typedef struct {
	/** \brief Base Virtual address where physical memory is mapped into */
	void		*baseVirtAddr;
	/** \brief 	Requested Address mapped into a virtual address.
	 * \details	Usually this address is the same as baseVirtAddr,
	 *			but in cases where the requested physical address is not
	 *			page aligned, requestedVirtAddr will be set to the virtual
	 *			address matching the physical address, whereas baseVirtAddr
	 *			will still point to the base location of the memory
	 *			mapping.
	 * \note	It is recommended to always use this address instead of
	 *			baseVirtAddr->
	 */
	void		*requestedVirtAddr;
	/** \brief Underlying file descriptor.	*/
	int    	fd;
	/** \brief Lentgh of the mapped region.	*/
	long int	length;
	/** \brief PID of the process that created the map	*/
	pid_t		parent;
} addr_map_t;

/** \brief Error code for open_mem_map() */
#define ADDRMAP_ERROR	((void*)-1)

/** \brief 	Map a physical memory region into virtual memory
 * \details	Maps a region of physical memory into virtual memory. The pid
 *			of the creating process is saved in the pAddrMap structure,
 *			so child processes can find wether they are the creators of
 *			the structure.
 * \param basePhysAddr	Base physical address. This address must be a
 *						multiple of the page size, as given by \code
 *						sysconf(PAGESIZE) \endcode
 *						If the given basePhysAddr is not page-aligned,
 *						open_mem_map() will try to find the nearest page
 *						and return the address within the mapped region
 *						that matches the desired physical address.
 * \param	length		Length of the mapped region, in bytes.
 * \return	Returns a pointer to the descriptor of the address map
 *			descriptor. If the map creation fails, the subroutine
 *			does not return and calls \code exit(-1) \endcode
 * \note	Memory maps created by open_mem_map() are preserved across
 *			forks. The behaviour of these mappings after a call to
 *			close_mem_map() is not entirely defined or known at this
 *			point.
 * \sa close_mem_map()
 */
ADDRMAP_EXTERN 
addr_map_t *open_mem_map( void *basePhysAddr , long int length );


typedef enum addrunmap_ret {
	ADDRUNMAP_OK		= 0		,
	ADDRUNMAP_ERROR		= -1 	,
} addrunmap_ret_t;
	
/** \brief	Unmap a mapped physical memory region
 * \details	Closes the mapping from physical to virtual memory, which
 *			is mapped to pAddrMap->baseVirtAddr
 *	
 * \param	pAddrMap	Pointer to the strucute holding the description
 *						of the memory map.
 * \return	If the operation is successful, returns ADDRUNMAP_OK,
 *			otherwise, returns ADDRUNMAP_ERROR.
 * \note	The pid of the creating process is checked and, if doesn't
 *			match the current pid, issues a warning.
 * \note	*pAddrMap becomes invalid after the call to close_mem_map().
 *			Any usage of the data pointed to by it will likely result in
 *			a crash with SIGSEGV
 * \sa open_mem_map()
 */
ADDRMAP_EXTERN
int	close_mem_map( addr_map_t *pAddrMap );


#endif
