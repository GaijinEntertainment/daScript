#pragma once

#include "daScript/simulate/simulate.h"

namespace das {

template <typename TT> struct WrapType { enum { value = false }; typedef TT type; };
template <> struct WrapType<float2> { enum { value = true }; typedef vec4f type; };
template <> struct WrapType<float3> { enum { value = true }; typedef vec4f type; };
template <> struct WrapType<float4> { enum { value = true }; typedef vec4f type; };
template <> struct WrapType<int2> { enum { value = true }; typedef vec4f type; };
template <> struct WrapType<int3> { enum { value = true }; typedef vec4f type; };
template <> struct WrapType<int4> { enum { value = true }; typedef vec4f type; };
template <> struct WrapType<uint2> { enum { value = true }; typedef vec4f type; };
template <> struct WrapType<uint3> { enum { value = true }; typedef vec4f type; };
template <> struct WrapType<uint4> { enum { value = true }; typedef vec4f type; };
template <> struct WrapType<range> { enum { value = true }; typedef vec4f type; };
template <> struct WrapType<urange> { enum { value = true }; typedef vec4f type; };

template <typename... Ts> struct AnyVectorType;
template <> struct AnyVectorType<> { enum { value = false }; };
template <typename T> struct AnyVectorType<T> { enum { value = WrapType<T>::value }; };
template <typename Head, typename... Tail> struct AnyVectorType<Head, Tail...>
    { enum { value = WrapType<Head>::value || AnyVectorType<Tail...>::value }; };

template <typename TT> struct NeedVectorWrap;
template <typename Ret, typename ... Args> struct NeedVectorWrap< Ret(*)(Args...) > {
    enum {
        result = WrapType<Ret>::value,
        arguments = AnyVectorType<Args...>::value,
        value = result || arguments
    };
};

template <int CMRES, int wrap, typename FuncT, FuncT fn> struct ImplWrapCall;

template <typename FuncT, FuncT fn>     // no cmres, no wrap
struct ImplWrapCall<false,false,FuncT,fn> {
    static void * get_builtin_address() { return (void *) fn; }
};

template <int wrap, typename RetT, typename ...Args, RetT(*fn)(Args...)>    // cmres
struct ImplWrapCall<true,wrap,RetT(*)(Args...),fn> {                        // when cmres, we always wrap
    static void static_call ( RetT * result, typename WrapType<Args>::type... args ) {
        *result = fn(args...);
    };
    static void * get_builtin_address() { return (void *) &static_call; }
};


template <typename RetT, typename ...Args, RetT(*fn)(Args...)>
struct ImplWrapCall<false,true,RetT(*)(Args...),fn> {   // no cmres, wrap
    static typename WrapType<RetT>::type static_call ( typename WrapType<Args>::type... args ) {
        return fn(args...);
    };
    static void * get_builtin_address() { return (void *) &static_call; }
};

}
