#pragma once

#include "daScript/misc/vectypes.h"
#include "daScript/misc/arraytype.h"
#include "daScript/misc/rangetype.h"

namespace das
{
    template <typename TT>
    struct cast;

    template <typename TT>
    struct cast <const TT> : cast<TT> {};

    template <typename TT>
    __forceinline vec4f cast_result ( TT arg ) {
        return cast<TT>::from(arg);
    }

    template <typename TT>
    struct cast <TT *> {
        static __forceinline TT * to ( vec4f a )               { return (TT *) v_extract_ptr(v_cast_vec4i((a))); }
        static __forceinline vec4f from ( TT * p )             { return v_cast_vec4f(v_splats_ptr((void *)p)); }
    };

    template <>
    struct cast <char *> {
        static __forceinline char * to ( vec4f a )             { return (char *) v_extract_ptr(v_cast_vec4i((a))); }
        static __forceinline vec4f from ( const char * p )     { return v_cast_vec4f(v_splats_ptr((void *)p)); }
    };

    template <typename TT>
    struct cast <TT &> {
        static __forceinline TT & to ( vec4f a )               { return *(TT *) v_extract_ptr(v_cast_vec4i((a))); }
        static __forceinline vec4f from ( TT & p )             { return v_cast_vec4f(v_splats_ptr((void *)&p)); }
    };

    template <typename TT>
    struct cast <const TT *> {
        static __forceinline const TT * to ( vec4f a )         { return (const TT *) v_extract_ptr(v_cast_vec4i((a))); }
        static __forceinline vec4f from ( const TT * p )       { return v_cast_vec4f(v_splats_ptr((void *)p)); }
    };

    template <typename TT>
    struct cast <const TT &> {
        static __forceinline const TT & to ( vec4f a )         { return *(const TT *) v_extract_ptr(v_cast_vec4i((a))); }
        static __forceinline vec4f from ( const TT & p )       { return v_cast_vec4f(v_splats_ptr((void *)&p)); }
    };

    template <>
    struct cast <vec4f> {
        static __forceinline vec4f to ( vec4f x )              { return x; }
        static __forceinline vec4f from ( vec4f x )            { return x; }
    };

    template <>
    struct cast <bool> {
        static __forceinline bool to ( vec4f x )               { return (bool) v_extract_xi(v_cast_vec4i(x)); }
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
        static __forceinline int64_t to ( vec4f x )            { return v_extract_xi64(v_cast_vec4i(x)); }
        static __forceinline vec4f from ( int64_t x )          { return v_cast_vec4f(v_splatsi64(x)); }
    };

    template <>
    struct cast <uint64_t> {
        static __forceinline uint64_t to ( vec4f x )           { return v_extract_xi64(v_cast_vec4i(x)); }
        static __forceinline vec4f from ( uint64_t x )         { return v_cast_vec4f(v_splatsi64(x)); }
    };

    template <>
    struct cast <float> {
        static __forceinline float to ( vec4f x )              { return v_extract_x(x); }
        static __forceinline vec4f from ( float x )            { return v_splats(x); }
    };

    template <>
    struct cast <double> {
        static __forceinline double to(vec4f x)                { union { vec4f v; double t; } A; A.v = x; return A.t; }
        static __forceinline vec4f from ( double x )           { union { vec4f v; double t; } A; A.t = x; return A.v; }
    };

    template <>
    struct cast <Func> {
        static __forceinline Func to ( vec4f x )             { union { vec4f v; Func t; } A; A.v = x; return A.t; }
        static __forceinline vec4f from ( const Func x )     { union { vec4f v; Func t; } A; A.t = x; return A.v; }
    };

    template <>
    struct cast <Lambda> {
        static __forceinline Lambda to ( vec4f x )           { union { vec4f v; Lambda t; } A; A.v = x; return A.t; }
        static __forceinline vec4f from ( const Lambda x )   { union { vec4f v; Lambda t; } A; A.t = x; return A.v; }
    };

    template <typename TT>
    struct cast_fVec {
        static __forceinline TT to ( vec4f x ) {
            return *((TT *)&x);
        }
        static __forceinline vec4f from ( const TT & x )       {
            return v_ldu((const float*)&x);
        }
    };

    template <typename TT>
    struct cast_fVec_half {
        static __forceinline TT to ( vec4f x ) {
            return *((TT *)&x);
        }
        static __forceinline vec4f from ( const TT & x )       {
            return v_ldu_half((const float*)&x);
        }
    };

    template <> struct cast <float2>  : cast_fVec_half<float2> {};
    template <> struct cast <float3>  : cast_fVec<float3> {};
    template <> struct cast <float4>  : cast_fVec<float4> {};

    template <typename TT>
    struct cast_iVec {
        static __forceinline TT to ( vec4f x ) {
            return *((TT *)&x);
        }
        static __forceinline vec4f from ( const TT & x ) {
            return  v_cast_vec4f(v_ldu_w((const int*)&x));
        }
    };

    template <typename TT>
    struct cast_iVec_half {
        static __forceinline TT to ( vec4f x ) {
            return *((TT *)&x);
        }
        static __forceinline vec4f from ( const TT & x ) {
            return  v_cast_vec4f(v_ldu_half_w((const int*)&x));
        }
    };

    template <> struct cast <int2>  : cast_iVec_half<int2> {};
    template <> struct cast <int3>  : cast_iVec<int3> {};
    template <> struct cast <int4>  : cast_iVec<int4> {};

    template <> struct cast <uint2>  : cast_iVec_half<uint2> {};
    template <> struct cast <uint3>  : cast_iVec<uint3> {};
    template <> struct cast <uint4>  : cast_iVec<uint4> {};

    template <> struct cast <range> : cast_iVec_half<range> {};
    template <> struct cast <urange> : cast_iVec_half<urange> {};

}
