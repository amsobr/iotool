#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <string.h>

namespace cbsl
{
    
namespace Strings
{
    inline bool equal( char const *s1 , char const *s2 ) {
        return strcmp(s1,s2)==0;
    }
    
    /** copy, at most, len bytes from src to dst, and make sure that
     * dst is null-terminated.
     */
    inline void safeCopy( char *dst , char const *src , size_t len )
    {
        strncpy( dst , src , len );
        dst[len-1]  = '\0';
    }
    

    extern bool toUlong( char const *str , unsigned long *dst , unsigned int base=10 );
    extern bool toLong( char const *str , unsigned long *dst , unsigned int base=10 );

    extern std::string fromInt(int i);
    

}; /* namespace cbsl::Strings */


}; /* namespace cbsl */

#endif /* !defined(STRINGS_HPP) */
