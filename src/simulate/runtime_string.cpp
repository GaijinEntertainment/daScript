#include "daScript/misc/platform.h"

#include "daScript/simulate/runtime_string.h"
#include "daScript/simulate/simulate.h"
#include "daScript/simulate/hash.h"

namespace das
{
    // string operations

    vec4f SimPolicy_String::Add ( vec4f a, vec4f b, Context & context ) {
        const char *  sA = to_rts(a);
        auto la = stringLength(context, sA);
        const char *  sB = to_rts(b);
        auto lb = stringLength(context, sB);
        uint32_t commonLength = la + lb;
        if ( !commonLength ) {
            return v_zero();
        } else if ( char * sAB = (char * ) context.heap.allocateString(nullptr, commonLength) ) {
            memcpy ( sAB, sA, la );
            memcpy ( sAB+la, sB, lb+1 );
            return cast<char *>::from(sAB);
        } else {
            context.throw_error("can't add two strings, out of heap");
            return v_zero();
        }
    }

    void SimPolicy_String::SetAdd ( char * a, vec4f b, Context & context ) {
        char ** pA = (char **)a;
        const char *  sA = *pA ? *pA : rts_null;
        auto la = stringLength(context, sA);
        const char *  sB = to_rts(b);
        auto lb = stringLength(context, sB);
        uint32_t commonLength = la + lb;
        if ( !commonLength ) {
            // *pA = nullptr; is unnecessary, because its already nullptr
            return;
        } else if ( char * sAB = (char * ) context.heap.allocateString(nullptr, commonLength) ) {
            memcpy ( sAB, sA, la );
            memcpy ( sAB+la, sB, lb+1 );
            *pA = sAB;
        } else {
            context.throw_error("can't add two strings, out of heap");
        }
    }

    // helper functions

    const char * rts_null = "";

    string unescapeString ( const string & input ) {
        const char* str = input.c_str();
        const char* strEnd = str + input.length();
        string result;
        for( ; str < strEnd; ++str ) {
            if ( *str=='\\' ) {
                ++str;
                if ( str == strEnd ) return result; // invalid escape sequence
                switch ( *str ) {
                    case '"':
                    case '/':
                    case '\\':  result += *str;    break;
                    case 'b':   result += '\b';    break;
                    case 'f':   result += '\f';    break;
                    case 'n':   result += '\n';    break;
                    case 'r':   result += '\r';    break;
                    case 't':   result += '\t';    break;
                    case 'u':   assert(0 && "utf-8 characters not supported yet"); break;
                    default:    break;  // invalid escape character
                }
            } else
                result += *str;
        }
        return result;
    }

    string escapeString ( const string & input ) {
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

    string getFewLines ( const string & st, int ROW, int COL ) {
        stringstream text;
        int col=1, row=1;
        auto it = st.begin();
        for ( ; ROW>=row && it!=st.end(); ++it, ++col ) {
            if ( row==ROW /* || row==ROW-1 */ )
                text << *it;
            if ( *it=='\n' ) {
                row ++;
                col = 1;
            }
        }
        text << string(max(COL-2,0), ' ') << "^" << "\n";
//        while ( *it!='\n' && it!=st.end() )
//            text << *it++;
        return text.str();
    }

    string to_string_ex ( double dnum ) {
        stringstream ss;
        ss << dnum;
        if ( ss.str().find_first_of(".e")==string::npos )
            ss << ".";
        return ss.str();
    }

    string reportError ( const string * st, int row, int col, const string & message, CompilationError erc ) {
        stringstream ssw;
        if ( row && col ) {
            auto text = st ? getFewLines(*st, row, col ) : "";
            ssw << "error at line " << row << " column " << col << "\n" << text;
            if ( erc != CompilationError::unspecified ) ssw << int(erc) << ": ";
            ssw << message << "\n";
        } else {
            if ( erc != CompilationError::unspecified ) ssw << int(erc) << ": ";
            ssw << "error, " << message << "\n";
        }
        return ssw.str();
    }

    vec4f SimNode_StringBuilder::eval ( Context & context ) {
        vec4f * argValues = (vec4f *)(alloca(nArguments * sizeof(vec4f)));
        evalArgs(context, argValues);
        stringstream ssw;
        for ( int32_t i = 0; i!=nArguments; ++i ) {
            // TODO: different output for strings?
            ssw << debug_value(argValues[i], types[i], PrintFlags::string_builder);
        }
        auto pStr = context.heap.allocateString(ssw.str());
        if ( !pStr ) {
            context.throw_error("can't allocate string builder result, out of heap");
        }
        return cast<char *>::from(pStr);
    }
}
