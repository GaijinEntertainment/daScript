//
//  cast.h
//  yzg
//
//  Created by Boris Batkin on 11/8/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#ifndef cast_h
#define cast_h

#include <xmmintrin.h>

namespace yzg
{
    template <typename TT>
    struct cast;
    
    template <typename TT>
    struct cast <TT *> {
        static __forceinline TT * to ( __m128 a )               { return ((TT **)&a)[0]; }
        static __forceinline __m128 from ( TT * p )             { __m128 x; ((TT **)&x)[0] = p; return x; }
    };
    
    template <>
    struct cast <bool> {
        static __forceinline bool to ( __m128 x )               { return *((int *)&x) != 0; }
        static __forceinline __m128 from ( bool x )             { return _mm_set1_epi32(x); }
    };
    
    template <>
    struct cast <int32_t> {
        static __forceinline int32_t to ( __m128 x )            { return *((int32_t *)&x); }
        static __forceinline __m128 from ( int32_t x )          { __m128 a; *((int32_t *)&a) = x; return a; }
    };
    
    template <>
    struct cast <uint32_t> {
        static __forceinline uint32_t to ( __m128 x )           { return *((uint32_t *)&x); }
        static __forceinline __m128 from ( uint32_t x )         { __m128 a; *((uint32_t *)&a) = x; return a; }
    };
    
    template <>
    struct cast <float> {
        static __forceinline float to ( __m128 x )              { return *((float *)&x); }
        static __forceinline __m128 from ( float x )            { return _mm_set_ss(x); }
    };
    
    template <>
    struct cast <float2> {
        static __forceinline float2 to ( __m128 x )             { return *((float2 *)&x); }
        static __forceinline __m128 from ( const float2 & x )   { return _mm_loadu_ps(&x.x); }
    };
    
    template <>
    struct cast <float3> {
        static __forceinline float3 to ( __m128 x )             { return *((float3 *)&x); }
        static __forceinline __m128 from ( const float3 & x )   { return _mm_loadu_ps(&x.x); }
    };
    
    template <>
    struct cast <float4> {
        static __forceinline float4 to ( __m128 x )             { return *((float4 *)&x); }
        static __forceinline __m128 from ( const float4 & x )   { return _mm_loadu_ps(&x.x); }
    };
}

#endif /* cast_h */
