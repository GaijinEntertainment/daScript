#include "precomp.h"

#include "runtime_string.h"

namespace yzg
{
    const char * rts_null = "";
    
    string unescapeString ( const string & input )
    {
        const char* str = input.c_str();
        const char* strEnd = str + input.length();
        string result;
        for( ; str < strEnd; ++str ) {
            if ( *str=='\\' ) {
                ++str;
                if ( str == strEnd ) throw runtime_error("Invalid escape sequence");
                switch ( *str ) {
                    case '"':
                    case '/':
                    case '\\':  result += *str;    break;
                    case 'b':   result += '\b';    break;
                    case 'f':   result += '\f';    break;
                    case 'n':   result += '\n';    break;
                    case 'r':   result += '\r';    break;
                    case 't':   result += '\t';    break;
                    case 'u':   throw runtime_error("utf-8 characters not supported yet");
                    default:    throw runtime_error("Invalid escape character");
                }
            } else
                result += *str;
        }
        return result;
    }
    
    string escapeString ( const string & input )
    {
        const char* str = input.c_str();
        const char* strEnd = str + input.length();
        string result;
        for( ; str < strEnd; ++str ) {
            switch ( *str ) {
                case '\"':  result.append("\\\"");  break;
                case '\\':  result.append("\\\\");  break;
                case '\b':  result.append("\\b");   break;
                case '\f':  result.append("\\f");   break;
                case '\n':  result.append("\\n");   break;
                case '\r':  result.append("\\r");   break;
                case '\t':  result.append("\\t");   break;
                default:    result.append(1, *str); break;
            }
        }
        return result;
    }
    
    string::const_iterator positionToRowCol ( const string & st, long AT, int & col, int & row )
    {
        col = 1;
        row = 1;
        auto at = st.begin() + AT;
        auto text = st.begin();
        auto it = st.begin();
        while ( it != at && it != st.end() ) {
            if ( *it=='\n' ) {
                text = it + 1;
                row ++;
                col = 1;
            }
            ++ it;
            col ++;
        }
        return text;
    }
}
