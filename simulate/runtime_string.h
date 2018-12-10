#pragma once

#include "cast.h"
#include "compilation_errors.h"

namespace yzg
{
    extern const char * rts_null;
    
    class Context;
    
    __forceinline const char * safe_str ( void * pX ) {
        const char * s = *((char **)pX);
        return s ? s : rts_null;
    }
    
    __forceinline const char * to_rts ( __m128 a ) {
        const char * s = cast<const char *>::to(a);
        return s ? s : rts_null;
    }
    
    __forceinline __m128 from_rts ( const char * a ) {
        return cast<const char *>::from(a==rts_null ? nullptr : a);
    }
    
    struct SimPolicy_String {
        // basic
        static __forceinline __m128 Equ     ( __m128 a, __m128 b, Context & )
            { return cast<bool>::from(strcmp(to_rts(a), to_rts(b))==0); }
        static __forceinline __m128 NotEqu  ( __m128 a, __m128 b, Context & )
            { return cast<bool>::from(strcmp(to_rts(a), to_rts(b))!=0); }
        // ordered
        static __forceinline __m128 LessEqu ( __m128 a, __m128 b, Context & )
            { return cast<bool>::from(strcmp(to_rts(a), to_rts(b))<=0); }
        static __forceinline __m128 GtEqu   ( __m128 a, __m128 b, Context & )
            { return cast<bool>::from(strcmp(to_rts(a), to_rts(b))>=0); }
        static __forceinline __m128 Less    ( __m128 a, __m128 b, Context & )
            { return cast<bool>::from(strcmp(to_rts(a), to_rts(b))<0); }
        static __forceinline __m128 Gt      ( __m128 a, __m128 b, Context & )
            { return cast<bool>::from(strcmp(to_rts(a), to_rts(b))>0); }
        
        static __m128 Add     ( __m128 a, __m128 b, Context & context );
        static __m128 SetAdd  ( __m128 a, __m128 b, Context & context );
    };
    
    string unescapeString ( const string & input );
    string escapeString ( const string & input );
    string to_string_ex ( double dnum );
    string reportError ( const string * st, int row, int col, const string & message, CompilationError erc = CompilationError::unspecified );
}

