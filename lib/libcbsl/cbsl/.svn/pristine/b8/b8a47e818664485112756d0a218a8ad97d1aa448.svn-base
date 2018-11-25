
#include <stdlib.h>

#include "util.h"

namespace Pti {
	
/* Convert string to integer */
long strtoint( char const *str , bool *ok , int base )
{
    char        *endptr;
    long int    i;
    
    if ( str[0]=='\0' ) {
        *ok = false;
        return 0;
    }
    
    i   = strtol( str , &endptr , base );
    
    if ( 0==*endptr ) {
        *ok = true;
    }
    else {
        *ok = false;
    }
    return i;
}

}; /* end namespace Pti */
