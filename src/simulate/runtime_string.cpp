#include "daScript/misc/platform.h"

#include "daScript/simulate/runtime_string.h"
#include "daScript/simulate/simulate.h"
#include "daScript/simulate/hash.h"
#include "daScript/simulate/debug_print.h"
#include "daScript/simulate/runtime_string_delete.h"
#include "daScript/simulate/simulate_nodes.h"
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
        } else if ( char * sAB = (char * ) context.stringHeap->allocateString(nullptr, commonLength) ) {
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
        } else if ( char * sAB = (char * ) context.stringHeap->allocateString(nullptr, commonLength) ) {
            memcpy ( sAB, sA, la );
            memcpy ( sAB+la, sB, lb+1 );
            *pA = sAB;
            context.stringHeap->recognize(sAB);
        } else {
            context.throw_error("can't add two strings, out of heap");
        }
    }

    // helper functions

    const char * rts_null = "";

    int hexChar ( char ch ) {
        if ( ch>='a' && ch<='f' ) {
            return ch - 'a' + 10;
        } else if ( ch>='A' && ch<='F' ) {
            return ch - 'A' + 10;
        } else if ( ch>='0' && ch<='9' ) {
            return ch - '0';
        } else {
            return -1;
        }
    }

    string unescapeString ( const string & input, bool * error, bool ) {
        if ( error ) *error = false;
        const char* str = input.c_str();
        const char* strEnd = str + input.length();
        string result;
        result.reserve(input.size());
        for( ; str < strEnd; ++str ) {
            if ( *str=='\\' ) {
                ++str;
                if ( str == strEnd ) {
                    if ( error ) *error = true;
                    return result; // invalid escape sequence
                }
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
                    case '\n':  break;  // skip LF
                    case '{':   result += '{';    break;
                    case '}':   result += '}';    break;
                    case '\r':  if ( str+1!=strEnd && str[1]=='\n' ) str++; break;  // skip CR LF or just CR
                    case 'x':   // \xA1 -> hex(A1)
                        if ( str+1!=strEnd && str+2!=strEnd ) {
                            int c0 = hexChar(str[1]);
                            int c1 = hexChar(str[2]);
                            if ( (c0<0 || c1<0) && error ) {
                                *error = true;
                                break;
                            }
                            result += (char)(c0*16 + c1);
                            str += 2;
                        } else {
                            if ( error ) *error = true;    // expecting \x12
                            break;
                        }
                        break;
                    default:    result += *str; if ( error ) *error = true; break;  // invalid escape character
                }
            } else
                result += *str;
        }
        return result;
    }

    string escapeString ( const string & input, bool das_escape ) {
        const char* str = input.c_str();
        const char* strEnd = str + input.length();
        string result;
        result.reserve(input.size());
        for( ; str < strEnd; ++str ) {
            switch ( *str ) {
                case '\"':  result.append("\\\"");  break;
                case '\\':  result.append("\\\\");  break;
                case '\b':  result.append("\\b");   break;
                case '\f':  result.append("\\f");   break;
                case '\n':  result.append("\\n");   break;
                case '\r':  result.append("\\r");   break;
                case '\t':  result.append("\\t");   break;
                case '{':   if (das_escape) result.append("\\{"); else result.append("{");  break;
                case '}':   if (das_escape) result.append("\\}"); else result.append("}");  break;
                default:    result.append(1, *str); break;
            }
        }
        return result;
    }

    string getFewLines ( const char* st, int ROW, int COL, int LROW, int LCOL, int TAB ) {
        TextWriter text;
        int col=0, row=1;
        auto it = st;
        if ( ROW>1 ) {
            while ( *it ) {
                auto CH = *it++;
                if ( CH=='\n' ) {
                    row++;
                    col=0;
                    if ( row==ROW ) break;
                }
            }
        }
        if ( row!=ROW ) return "";
        while ( *it ) {
            auto CH = *it++;
            if ( CH=='\t' ) {
                int tcol = (col + TAB) & ~(TAB-1);
                while ( col < tcol ) {
                    text << " ";
                    col ++;
                }
                continue;
            } else if ( CH=='\n' ) {
                row++;
                col=0;
                text << "\n";
                break;
            } else {
                text << CH;
            }
            col ++;
        }
        const char * tail = it + COL;
        while ( *it && it != tail ) {
            auto CH = *it++;
            if ( CH=='\t' ) {
                int tcol = (col + TAB) & ~(TAB-1);
                while ( col < tcol ) {
                    text << " ";
                    col ++;
                }
                continue;
            } else if ( CH=='\n' ) {
                break;
            } else {
                text << " ";
            }
            col ++;
        }
        text << string(das::max(LCOL-COL+1,1),'^') << "\n";
        text << COL << ":" << ROW << " - " << LCOL << ":" << LROW << "\n";
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

    string reportError(const struct LineInfo & at, const string & message,
        const string & extra, const string & fixme, CompilationError erc) {
        return reportError(
                at.fileInfo ? at.fileInfo->source : nullptr,
                at.fileInfo ? at.fileInfo->name.c_str() : nullptr,
                at.line, at.column, at.last_line, at.last_column,
                at.fileInfo ? at.fileInfo->tabSize : 4,
                message, extra, fixme, erc );
    }

    string reportError ( const char * st, const char * fileName,
        int row, int col, int lrow, int lcol, int tabSize, const string & message,
        const string & extra, const string & fixme, CompilationError erc ) {
        TextWriter ssw;
        if ( row ) {
            auto text = st ? getFewLines(st, row, col, lrow, lcol, tabSize) : "";
            ssw << fileName << ":" << row << ":" << col << ":\n" << text;
            if ( erc != CompilationError::unspecified ) ssw << int(erc) << ": ";
            ssw << message << "\n";
        } else {
            if ( erc != CompilationError::unspecified ) ssw << int(erc) << ": ";
            ssw << "error, " << message << "\n";
        }
        if (!extra.empty()) ssw << extra << "\n";
        if (!fixme.empty()) ssw << "\t" << fixme << "\n";
        return ssw.str();
    }

    vec4f SimNode_StringBuilder::eval ( Context & context ) {
        DAS_PROFILE_NODE
        vec4f * argValues = (vec4f *)(alloca(nArguments * sizeof(vec4f)));
        evalArgs(context, argValues);
        StringBuilderWriter writer;
        DebugDataWalker<StringBuilderWriter> walker(writer, PrintFlags::string_builder);
        for ( int i = 0; i!=nArguments; ++i ) {
            walker.walk(argValues[i], types[i]);
        }
        int length = writer.tellp();
        if ( length ) {
            auto pStr = context.stringHeap->allocateString(writer.c_str(), length);
            if ( !pStr  ) {
                context.throw_error("can't allocate string builder result, out of heap");
            }
            return cast<char *>::from(pStr);
        } else {
            return v_zero();
        }
    }

    // string iteration

    bool StringIterator::first ( Context &, char * _value )  {
        if ( str==nullptr || *str==0 ) return false;
        int32_t * value = (int32_t *) _value;
        *value = uint8_t(*str++);
        return true;
    }

    bool StringIterator::next  ( Context &, char * _value )  {
        int32_t * value = (int32_t *) _value;
        *value = uint8_t(*str++);
        return *value != 0;
    }

    void StringIterator::close ( Context & context, char * _value )  {
        if ( _value ) {
            int32_t * value = (int32_t *) _value;
            *value = 0;
        }
        context.heap->free((char *)this, sizeof(StringIterator));
    }

    vec4f SimNode_StringIterator::eval ( Context & context ) {
        DAS_PROFILE_NODE
        vec4f ll = source->eval(context);
        char * str = cast<char *>::to(ll);
        char * iter = context.heap->allocate(sizeof(StringIterator));
        context.heap->mark_comment(iter,"string iterator");
        context.heap->mark_location(iter,&debugInfo);
        new (iter) StringIterator(str);
        return cast<char *>::from(iter);
    }
}
