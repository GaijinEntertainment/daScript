#pragma once

#include "daScript/simulate/cast.h"
#include "daScript/ast/compilation_errors.h"
#include "daScript/simulate/sim_policy.h"

namespace das
{
    extern const char * rts_null;

    class Context;

    __forceinline const char * safe_str ( void * pX ) {
        const char * s = *((char **)pX);
        return s ? s : rts_null;
    }

    __forceinline const char * to_rts ( vec4f a ) {
        const char * s = cast<const char *>::to(a);
        return s ? s : rts_null;
    }

    __forceinline vec4f from_rts ( const char * a ) {
        return cast<const char *>::from(a==rts_null ? nullptr : a);
    }

    struct SimPolicy_String {
        // basic
        static __forceinline bool Equ     ( vec4f a, vec4f b, Context & )
            { return strcmp(to_rts(a), to_rts(b))==0; }
        static __forceinline bool NotEqu  ( vec4f a, vec4f b, Context & )
            { return (bool) strcmp(to_rts(a), to_rts(b)); }
        // ordered
        static __forceinline bool LessEqu ( vec4f a, vec4f b, Context & )
            { return strcmp(to_rts(a), to_rts(b))<=0; }
        static __forceinline bool GtEqu   ( vec4f a, vec4f b, Context & )
            { return strcmp(to_rts(a), to_rts(b))>=0; }
        static __forceinline bool Less    ( vec4f a, vec4f b, Context & )
            { return strcmp(to_rts(a), to_rts(b))<0; }
        static __forceinline bool Gt      ( vec4f a, vec4f b, Context & )
            { return strcmp(to_rts(a), to_rts(b))>0; }
        static vec4f Add  ( vec4f a, vec4f b, Context & context );
        static void SetAdd ( char * a, vec4f b, Context & context );
    };

    template <> struct SimPolicy<char *> : SimPolicy_String {};

    string unescapeString ( const string & input );
    string escapeString ( const string & input );
    string to_string_ex ( double dnum );
    string reportError ( const char * st, int row, int col, const string & message, CompilationError erc = CompilationError::unspecified );
}

