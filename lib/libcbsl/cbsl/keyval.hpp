#ifndef GETKEYVAL_HPP
#define GETKEYVAL_HPP

#include <string>

#include <stdlib.h>


namespace cbsl 
{
    
namespace KeyVal
{
    class Pair
    {
        public:
            std::string key;
            std::string value;

            Pair() :
            key() ,
            value()
            {
            }

            Pair(std::string k , std::string v ) :
            key(k) ,
            value(v)
            {
            }

            Pair( char const *k , char const *v )
            {
                if (k!=0) key.assign(k);
                if (v!=0) value.assign(v);
            }

            Pair( Pair const &other ) :
            key(other.key) ,
            value(other.value)
            {
            }

        
            inline bool isAnonymous() const { return key.empty(); }
    }; /* class cbsl::KeyVal::Pair */

/** \brief Extract a KeyValue::Pair from an argument token.
 * \details Parses a token in the form 'key=value' into key and value elements.
 * Parses a token in the form 'value' into a value without a key (blank)
 * \param arg String to parse. Must be a '\0' terminated string. 
 * \param dest If non null, key and value will be set in \a dest
 * \return Returns a Pair holding the parsed key and value.
 *  If \a dest is not 0, the return value is a copy of *dest
 */
extern Pair const parseArg( char const *arg , Pair *dest=0 );
}; /* namespace cbsl::KeyVal */

}; /* namespace cbsl */


#endif /* ! GETKEYVAL_HPP */
