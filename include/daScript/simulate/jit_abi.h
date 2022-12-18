#pragma once

#include "daScript/simulate/simulate.h"

namespace das {

template <typename TT> struct IsVectorType { enum { value = false }; };
template <> struct IsVectorType<float2> { enum { value = true }; };
template <> struct IsVectorType<float3> { enum { value = true }; };
template <> struct IsVectorType<float4> { enum { value = true }; };
template <> struct IsVectorType<int2> { enum { value = true }; };
template <> struct IsVectorType<int3> { enum { value = true }; };
template <> struct IsVectorType<int4> { enum { value = true }; };
template <> struct IsVectorType<uint2> { enum { value = true }; };
template <> struct IsVectorType<uint3> { enum { value = true }; };
template <> struct IsVectorType<uint4> { enum { value = true }; };
template <> struct IsVectorType<range> { enum { value = true }; };
template <> struct IsVectorType<urange> { enum { value = true }; };

template <typename... Ts> struct AnyVectorType;
template <> struct AnyVectorType<> { enum { value = false }; };
template <typename T> struct AnyVectorType<T> { enum { value = IsVectorType<T>::value }; };
template <typename Head, typename... Tail> struct AnyVectorType<Head, Tail...>
    { enum { value = IsVectorType<Head>::value || AnyVectorType<Tail...>::value }; };

template <typename TT> struct NeedVectorWrap;
template <typename Ret, typename ... Args> struct NeedVectorWrap< Ret(*)(Args...) >
    { enum { value = IsVectorType<Ret>::value || AnyVectorType<Args...>::value }; };

template <typename TT> struct WrapType { typedef TT type; };
template <> struct WrapType<float2> { typedef vec4f type; };
template <> struct WrapType<float3> { typedef vec4f type; };
template <> struct WrapType<float4> { typedef vec4f type; };
template <> struct WrapType<int2> { typedef vec4f type; };
template <> struct WrapType<int3> { typedef vec4f type; };
template <> struct WrapType<int4> { typedef vec4f type; };
template <> struct WrapType<uint2> { typedef vec4f type; };
template <> struct WrapType<uint3> { typedef vec4f type; };
template <> struct WrapType<uint4> { typedef vec4f type; };
template <> struct WrapType<range> { typedef vec4f type; };
template <> struct WrapType<urange> { typedef vec4f type; };

template <int needWarap, typename FuncT, FuncT fn> struct ImplWrapCall;

template <typename RetT, typename ...Args, RetT(*fn)(Args...)> struct ImplWrapCall<true,RetT(*)(Args...),fn> {
    static typename WrapType<RetT>::type static_call ( typename WrapType<Args>::type... args ) {
        return fn(args...);
    };
    static void * get_builtin_address() {
        return (void *) &static_call;
    }
};

template <typename FuncT, FuncT fn> struct ImplWrapCall<false,FuncT,fn> {
    static void * get_builtin_address() {
        return (void *) fn;
    }
};

}
