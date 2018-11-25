
#include <stdlib.h>
#include <string>
#include <iostream>

#include "strings.hpp"

namespace cbsl
{
    
namespace Strings
{

bool toUlong( char const *str , unsigned long *dst , unsigned int base )
{
    if ( str[0]=='\0' ) {
        return false;
    }
    
    char *endptr;
    *dst    = strtoul(str,&endptr,base);
    
    if ( *endptr==0 ) {
        return true;
    }
    
    return false;
}

bool toLong( char const *str , long *dst , unsigned int base )
{
    if ( str[0]=='\0' ) {
        return false;
    }
    
    char *endptr;
    *dst    = strtol(str,&endptr,base);
    
    if ( *endptr==0 ) {
        return true;
    }
    
    return false;
}

std::string fromInt( int i )
{
    char str[200];
    snprintf( str , sizeof(str) , "%d" , i );
    return std::string(str);
}



}; /* namespace cbsl::Strings */

}; /* namespace cbsl */