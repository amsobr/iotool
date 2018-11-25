/** \file 	util.h
 * \brief	Small general purpose utils.
 * \details	To be added.
 * \author	Ant?nio Oliveira - antonio.oliveira@withus.pt
 */
#ifndef PTI_UTIL_H
#define PTI_UTIL_H

#include <string.h>

#define touch_var(var) \
do { (void*)var; } while(0);

namespace Pti {
	
/** \brief Convert string to integer
	* \details	To Be Added!
	*/
extern long strtoint( char const *str , bool *ok , int base=10 );

template<class T>
void copy( T *dest , T const *src )
{
	memcpy( dest , src , sizeof(T) );
}
	
}; /* end namespace Pti */

#endif /* PTI_UTIL_H */
