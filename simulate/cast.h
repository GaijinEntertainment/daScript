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
    
    template <typename TT>
    struct cast_fVec {
        static __forceinline TT to ( __m128 x )                 { return *((TT *)&x); }
        static __forceinline __m128 from ( const TT & x )       { return _mm_loadu_ps(&x.x); }
    };
    
    template <> struct cast <float2>  : cast_fVec<float2> {};
    template <> struct cast <float3>  : cast_fVec<float3> {};
    template <> struct cast <float4>  : cast_fVec<float4> {};
    
    template <typename TT>
    struct cast_iVec {
        static __forceinline TT to ( __m128 x )                 { return *((TT *)&x); }
        static __forceinline __m128 from ( const TT & x )       { return _mm_loadu_si128((__m128i*)&x.x); }
    };
    
    template <> struct cast <int2>  : cast_iVec<int2> {};
    template <> struct cast <int3>  : cast_iVec<int3> {};
    template <> struct cast <int4>  : cast_iVec<int4> {};
    
    template <> struct cast <uint2>  : cast_iVec<uint2> {};
    template <> struct cast <uint3>  : cast_iVec<uint3> {};
    template <> struct cast <uint4>  : cast_iVec<uint4> {};
}

#endif /* cast_h */
