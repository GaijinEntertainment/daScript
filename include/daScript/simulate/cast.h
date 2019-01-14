#pragma once

#include "daScript/misc/vectypes.h"
#include "daScript/misc/arraytype.h"
#include "daScript/misc/rangetype.h"

namespace das
{
    using namespace std;
    
    template <typename TT>
    struct cast;
    
    template <typename TT>
    struct cast <TT *> {
        static __forceinline TT * to ( vec4f a )               { return (TT *) vec_cast_esi_ptr(v_cast_vec4i((a))); }
        static __forceinline vec4f from ( TT * p )             { return v_cast_vec4f(vec_cast_ptr_esi((void *)p)); }
    };
    
    template <typename TT>
    struct cast <TT &> {
        static __forceinline TT & to ( vec4f a )               { return *(TT *) vec_cast_esi_ptr(v_cast_vec4i((a))); }
        static __forceinline vec4f from ( TT & p )             { return v_cast_vec4f(vec_cast_ptr_esi((void *)&p)); }
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
        static __forceinline bool to ( vec4f x )               { return v_extract_xi(v_cast_vec4i(x)) != 0; }
        static __forceinline vec4f from ( bool x )             { return v_cast_vec4f(v_splatsi(x)); }
    };
    
    template <>
    struct cast <int32_t> {
        static __forceinline int32_t to ( vec4f x )            { return v_extract_xi(v_cast_vec4i(x)); }
        static __forceinline vec4f from ( int32_t x )          { return v_cast_vec4f(v_splatsi(x)); }
    };
    
    template <>
    struct cast <uint32_t> {
        static __forceinline uint32_t to ( vec4f x )           { return v_extract_xi(v_cast_vec4i(x)); }
        static __forceinline vec4f from ( uint32_t x )         { return v_cast_vec4f(v_splatsi(x)); }
    };
    
    template <>
    struct cast <int64_t> {
        static __forceinline int64_t to ( vec4f x )            { return v_splatsi64(v_cast_vec4i(x)); }
        static __forceinline vec4f from ( int64_t x )          { return v_cast_vec4f(v_extract_xi64(x)); }
    };
    
    template <>
    struct cast <uint64_t> {
        static __forceinline uint64_t to ( vec4f x )           { return v_splatsi64(v_cast_vec4i(x)); }
        static __forceinline vec4f from ( uint64_t x )         { return v_cast_vec4f(v_extract_xi64(x)); }
    };
    
    template <>
    struct cast <float> {
        static __forceinline float to ( vec4f x )              { return v_extract_x(x); }
        static __forceinline vec4f from ( float x )            { return v_splats(x); }
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
        static __forceinline TT to ( vec4f x ) {
            return *((TT *)&x);
        }
        static __forceinline vec4f from ( const TT & x )       {
            return v_ldu(&x.x);
        }
    };
    
    template <> struct cast <float2>  : cast_fVec<float2> {};
    template <> struct cast <float3>  : cast_fVec<float3> {};
    template <> struct cast <float4>  : cast_fVec<float4> {};
    
    template <typename TT>
    struct cast_iVec {
        static __forceinline TT to ( vec4f x ) {
            return *((TT *)&x);
        }
        static __forceinline vec4f from ( const TT & x ) {
            return  v_cast_vec4f(v_ldu_w((vec4i*)&x.x));
        }
    };
    
    template <> struct cast <int2>  : cast_iVec<int2> {};
    template <> struct cast <int3>  : cast_iVec<int3> {};
    template <> struct cast <int4>  : cast_iVec<int4> {};
    
    template <> struct cast <uint2>  : cast_iVec<uint2> {};
    template <> struct cast <uint3>  : cast_iVec<uint3> {};
    template <> struct cast <uint4>  : cast_iVec<uint4> {};
}
