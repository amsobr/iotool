#include <string>

#include "keyval.hpp"

namespace cbsl
{
    
namespace KeyVal
{

Pair const parseArg( char const *arg , Pair *dest )
{   
    if ( *arg=='\0' ) {
        if ( dest!=0 ) {
            dest->key.clear();
            dest->value.clear();
            return *dest;
        }
        else {
            return Pair();
        }
    }
    else {
        std::string key;
        std::string val;

        int i(0);
        for ( ; arg[i]!='=' && arg[i]!='\0' ; i++ )
            ;

        if ( arg[i]=='\0' ) {
            key = "";
            val.assign(arg);
        }
        else {
            /* okay... we stopped at a '=' */
            key.assign(arg,i);
            i++;
            val.assign( arg[i]=='\0' ? "" : arg+i );
        }

        if ( dest==0 ) {
            return Pair(key,val);            
        }
        else {
            dest->key   = key;
            dest->value = val;
            return *dest;
        }
    }
}

}; /* namespace cbsl::KeyVal */

}; /* namespace cbsl */

