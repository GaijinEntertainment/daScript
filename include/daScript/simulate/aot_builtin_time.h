#pragma once

#include <ctime>

namespace das {

    struct Time {
        time_t time;
    };

    template <>
    struct cast <Time> {
        static __forceinline Time to ( vec4f x )               { union { Time t; vec4f vec; } T; T.vec = x; return T.t; }
        static __forceinline vec4f from ( Time x )             { union { Time t; vec4f vec; } T; T.t = x; return T.vec; }
    };

    template<>
    struct SimPolicy<Time> {
        static __forceinline auto to_time ( vec4f a ) {
            return cast<Time>::to(a).time;
        }
        static __forceinline bool Equ     ( vec4f a, vec4f b, Context & ) {
            return to_time(a) == to_time(b);
        }
        static __forceinline bool NotEqu  ( vec4f a, vec4f b, Context & ) {
            return to_time(a) != to_time(b);
        }
        static __forceinline bool GtEqu  ( vec4f a, vec4f b, Context & ) {
            return to_time(a) >= to_time(b);
        }
        static __forceinline bool LessEqu  ( vec4f a, vec4f b, Context & ) {
            return to_time(a) <= to_time(b);
        }
        static __forceinline bool Gt  ( vec4f a, vec4f b, Context & ) {
            return to_time(a) > to_time(b);
        }
        static __forceinline bool Less  ( vec4f a, vec4f b, Context & ) {
            return to_time(a) < to_time(b);
        }
        static __forceinline float Sub  ( vec4f a, vec4f b, Context & ) {
            return float(difftime(to_time(a), to_time(b)));
        }
    };

    Time builtin_clock();
    void builtin_sleep ( uint32_t msec );
}
