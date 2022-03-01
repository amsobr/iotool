
#include <string>
#include <cstring>

#include <common/Strings.hpp>


using namespace std;


vector<string> Strings::split(string const& in, string const& delimiter, bool mergeDelimiters)
{
    vector<string> out;

    size_t curPos  = 0;
    while ( curPos<in.size() ) {
        size_t pos = in.find(delimiter,curPos);
        if ( pos!=string::npos ) {
            if ( pos==curPos ) {
                if ( !mergeDelimiters ) {
                    out.emplace_back();
                }
            }
            //if ( pos!=string::npos ) {
                out.emplace_back(in.substr(curPos,pos-curPos));
            //}
            curPos  = pos += delimiter.size();
        }
        else {
            if ( curPos<in.size() ) {
                out.emplace_back(in.substr(curPos));
            }
            break;
        }
    }
    return out;
}

int Strings::replace(string* in, string const& from, string const& to)
{
    if ( from.empty() ) {
        return 0;
    }

    int count        = 0;
    size_t start_pos = 0;
    while( (start_pos=(*in).find(from,start_pos))!=std::string::npos ) {
        (*in).replace(start_pos,from.length(),to);
        start_pos += to.length();
        count++;
    }
    return count;
}

bool Strings::endsWith(string const& in, string const& ending)
{
    size_t inSz = in.size();
    size_t eSz  = ending.size();
    if ( inSz<eSz ) {
        return false;
    }

    if ( in.substr(inSz-eSz)==ending ) {
        return true;
    }

    return false;
}

