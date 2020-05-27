#pragma once

#include "daScript/misc/vectypes.h"
#include "daScript/misc/arraytype.h"
#include "daScript/misc/rangetype.h"

namespace das {

    template <typename TT>
    struct typeName;

    template <typename TT> struct typeName<const TT> : typeName<TT> {};

    template <typename TT, typename PP> struct typeName<smart_ptr<TT,PP>> {
        static string name() {
            return "smart_ptr`" + typeName<TT>::name();
        }
    };

    template <typename FT, typename ST> struct typeName<pair<FT,ST>> {
        static string name() {
            return "pair`" + typeName<FT>::name() + "`" + typeName<ST>::name();
        }
    };

    template <> struct typeName<int32_t>  { static string name() { return "int"; } };
    template <> struct typeName<uint32_t> { static string name() { return "uint"; } };
    template <> struct typeName<Bitfield> { static string name() { return "bitfield"; } };
    template <> struct typeName<int8_t>   { static string name() { return "int8"; } };
    template <> struct typeName<uint8_t>  { static string name() { return "uint8"; } };
    template <> struct typeName<int16_t>  { static string name() { return "int16"; } };
    template <> struct typeName<uint16_t> { static string name() { return "uint16"; } };
    template <> struct typeName<int64_t>  { static string name() { return "int64"; } };
    template <> struct typeName<uint64_t> { static string name() { return "uint64"; } };
    template <> struct typeName<bool>     { static string name() { return "bool"; } };
    template <> struct typeName<float>    { static string name() { return "float"; } };
    template <> struct typeName<double>   { static string name() { return "double"; } };
    template <> struct typeName<int2>     { static string name() { return "int2"; } };
    template <> struct typeName<int3>     { static string name() { return "int3"; } };
    template <> struct typeName<int4>     { static string name() { return "int4"; } };
    template <> struct typeName<uint2>    { static string name() { return "uint2"; } };
    template <> struct typeName<uint3>    { static string name() { return "uint3"; } };
    template <> struct typeName<uint4>    { static string name() { return "uint4"; } };
    template <> struct typeName<float2>   { static string name() { return "float2"; } };
    template <> struct typeName<float3>   { static string name() { return "float3"; } };
    template <> struct typeName<float4>   { static string name() { return "float4"; } };
    template <> struct typeName<range>    { static string name() { return "range"; } };
    template <> struct typeName<urange>   { static string name() { return "urange"; } };
    template <> struct typeName<char *>   { static string name() { return "string"; } };
    template <> struct typeName<void *>   { static string name() { return "pointer"; } };
    template <> struct typeName<Func>     { static string name() { return "Func"; } };
    template <> struct typeName<Lambda>   { static string name() { return "Lambda"; } };
    template <> struct typeName<Block>    { static string name() { return "Block"; } };
    template <> struct typeName<Tuple>    { static string name() { return "Tuple"; } };
    template <> struct typeName<Variant>  { static string name() { return "Variant"; } };
}
