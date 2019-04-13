#include "daScript/misc/platform.h"

#include "daScript/simulate/runtime_string.h"
#include "daScript/simulate/simulate.h"
#include "daScript/simulate/hash.h"
#include "daScript/simulate/debug_print.h"
#include "daScript/simulate/runtime_string_delete.h"

#include "daScript/simulate/sim_policy.h"

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
                    case 'u':   DAS_ASSERTF(0, "utf-8 characters not supported yet"); break;
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

    string getFewLines ( const char* st, int ROW, int COL ) {
        TextWriter text;
        int col=1, row=1;
        auto it = st;
        for ( ; ROW>=row && *it; ++it, ++col ) {
            if ( row==ROW /* || row==ROW-1 */ )
                text << *it;
            if ( *it=='\n' ) {
                row ++;
                col = 1;
            }
        }
        text << string(das::max(COL-2,0), ' ') << "^" << "\n";
//        while ( *it!='\n' && it!=st.end() )
//            text << *it++;
        return text.str();
    }

    string to_string_ex ( double dnum ) {
        char buffer[32];
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%.17g", dnum);
        string stst(buffer);
        if ( stst.find_first_of(".e")==string::npos )
            stst += ".";
        return stst;
    }

    string to_string_ex ( float dnum ) {
        char buffer[32];
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%.9g", dnum);
        string stst(buffer);
        if ( stst.find_first_of(".e")==string::npos )
            stst += ".";
        return stst;
    }

    string reportError(const struct LineInfo & at, const string & message, CompilationError erc) {
        return reportError(
                at.fileInfo ? at.fileInfo->source : nullptr,
                at.fileInfo ? at.fileInfo->name.c_str() : nullptr,
                at.line, at.column, message, erc );
    }

    string reportError ( const char * st, const char * fileName, int row, int col, const string & message, CompilationError erc ) {
        TextWriter ssw;
        if ( row && col ) {
            auto text = st ? getFewLines(st, row, col ) : "";
            ssw << fileName << ":" << row << ":" << col << ":\n" << text;
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
        StringBuilderWriter writer(context.heap);
        DebugDataWalker<StringBuilderWriter> walker(writer, PrintFlags::string_builder);
        for ( int i = 0; i!=nArguments; ++i ) {
            walker.walk(argValues[i], types[i]);
        }
        auto pStr = writer.c_str();
        if ( !pStr ) {
            context.throw_error("can't allocate string builder result, out of heap");
        }
        return cast<char *>::from(pStr);
    }

    vec4f SimNode_DeleteString::eval ( Context & context ) {
        char ** str = (char**)(subexpr->evalPtr(context));
        str = str + total - 1;
        for ( uint32_t i=0; i!=total; ++i, str-- ) {
            char * string = *str;
            if ( context.heap.isHeapPtr(string) ) {
                auto header = ((StringHeader *) string) - 1;
                const uint32_t size = header->length + sizeof(StringHeader) + 1;
                context.heap.free((char *)header, size);
                *str = nullptr;
            }
        }
        return v_zero();
    }
}
