//
//  runtime_string.cpp
//  yzg
//
//  Created by Boris Batkin on 11/14/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#include "runtime_string.hpp"

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
}
