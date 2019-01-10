#include "daScript/misc/platform.h"

#include "daScript/simulate/runtime_string.h"
#include "daScript/simulate/simulate.h"

namespace das
{
    // string operations
    
    vec4f SimPolicy_String::Add ( vec4f a, vec4f b, Context & context ) {
        const char *  sA = to_rts(a); auto la = strlen(sA);
        const char *  sB = to_rts(b); auto lb = strlen(sB);
        char * sAB = (char * ) context.allocate(uint32_t(la + lb + 1));
        memcpy ( sAB, sA, la );
        memcpy ( sAB+la, sB, lb+1 );
        return from_rts(sAB);
    }
  
    void SimPolicy_String::SetAdd ( char * a, vec4f b, Context & context ) {
		char ** pA = (char **)a;
        const char *  sA = *pA ? *pA : rts_null; auto la = strlen(sA);
        const char *  sB = to_rts(b); auto lb = strlen(sB);
        *pA = (char * ) context.allocate(uint32_t(la + lb + 1));
        memcpy ( *pA , sA, la );
        memcpy ( *pA +la, sB, lb+1 );
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
        DAS_EXCEPTION_POINT;
        stringstream ssw;
        for ( int32_t i = 0; i!=nArguments; ++i ) {
            // TODO: different output for strings?
            ssw << debug_value(argValues[i], types[i], PrintFlags::string_builder);
        }
        auto pStr = context.allocateName(ssw.str());
        return cast<char *>::from(pStr);
    }
}
