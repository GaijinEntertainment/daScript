#pragma once

#include "vectypes.h"
#include "arraytype.h"
#include "rangetype.h"

namespace das
{
    using namespace std;
    
    template <typename TT>
    struct cast;
    
    template <typename TT>
    struct cast <TT *> {
        static __forceinline TT * to ( vec4f a )               { return ((TT **)&a)[0]; }
        static __forceinline vec4f from ( TT * p )             { return vec_loadu_ps((float *) &p); }
    };
    
    template <typename TT>
    struct cast <TT &> {
        static __forceinline TT & to ( vec4f a )               { return *((TT **)&a)[0]; }
        static __forceinline vec4f from ( TT & p )             { vec4f x; ((TT **)&x)[0] = &p; return x; }
    };
    
    template <>
    struct cast <string> {
        static __forceinline string to ( vec4f x )             { return cast<char *>::to(x); }
        static __forceinline vec4f from ( const string & x )   { return cast<char *>::from((char *)x.c_str()); }
    };
    template<> struct cast <string &> : cast<string> {};
    template<> struct cast <const string &> : cast<string> {};
    
    template <>
    struct cast <bool> {
        static __forceinline bool to ( vec4f x )               { return *((int32_t *)&x) != 0; }
        static __forceinline vec4f from ( bool x )             { vec4f a; *((int32_t *)&a) = x; return a; }
    };
    
    template <>
    struct cast <int32_t> {
        static __forceinline int32_t to ( vec4f x )            { return *((int32_t *)&x); }
        static __forceinline vec4f from ( int32_t x )          { vec4f a; *((int32_t *)&a) = x; return a; }
    };
    
    template <>
    struct cast <uint32_t> {
        static __forceinline uint32_t to ( vec4f x )           { return *((uint32_t *)&x); }
        static __forceinline vec4f from ( uint32_t x )         { vec4f a; *((uint32_t *)&a) = x; return a; }
    };
    
    template <>
    struct cast <int64_t> {
        static __forceinline int64_t to ( vec4f x )            { return *((int64_t *)&x); }
        static __forceinline vec4f from ( int64_t x )          { vec4f a; *((int64_t *)&a) = x; return a; }
    };
    
    template <>
    struct cast <uint64_t> {
        static __forceinline uint64_t to ( vec4f x )           { return *((uint64_t *)&x); }
        static __forceinline vec4f from ( uint64_t x )         { vec4f a; *((uint64_t *)&a) = x; return a; }
    };
    
    template <>
    struct cast <float> {
        static __forceinline float to ( vec4f x )              { return *((float *)&x); }
        static __forceinline vec4f from ( float x )            { vec4f a; *((float *)&a) = x; return a; }
    };
    
    template <>
    struct cast <range> {
        static __forceinline range to ( vec4f x )              { return *((range *)&x); }
        static __forceinline vec4f from ( range x )            { vec4f a; *((range *)&a) = x; return a; }
    };
    
    template <>
    struct cast <urange> {
        static __forceinline urange to ( vec4f x )             { return *((urange *)&x); }
        static __forceinline vec4f from ( urange x )           { vec4f a; *((urange *)&a) = x; return a; }
    };
    
    template <>
    struct cast <Block> {
        static __forceinline Block to ( vec4f x )             { return *((Block *)&x); }
        static __forceinline vec4f from ( Block x )           { vec4f a; *((Block *)&a) = x; return a; }
    };
    
    template <typename TT>
    struct cast_fVec {
        static __forceinline TT to ( vec4f x )                 { return *((TT *)&x); }
        static __forceinline vec4f from ( const TT & x )       { return vec_loadu_ps(&x.x); }
    };
    
    template <> struct cast <float2>  : cast_fVec<float2> {};
    template <> struct cast <float3>  : cast_fVec<float3> {};
    template <> struct cast <float4>  : cast_fVec<float4> {};
    
    template <typename TT>
    struct cast_iVec {
        static __forceinline TT to ( vec4f x )                 { return *((TT *)&x); }
        static __forceinline vec4f from ( const TT & x )       { return  vec_cast_esi_ps(vec_loadu_esi((vec4i*)&x.x)); }
    };
    
    template <> struct cast <int2>  : cast_iVec<int2> {};
    template <> struct cast <int3>  : cast_iVec<int3> {};
    template <> struct cast <int4>  : cast_iVec<int4> {};
    
    template <> struct cast <uint2>  : cast_iVec<uint2> {};
    template <> struct cast <uint3>  : cast_iVec<uint3> {};
    template <> struct cast <uint4>  : cast_iVec<uint4> {};
}
