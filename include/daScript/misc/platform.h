#pragma once

#ifdef _MSC_VER
#pragma warning(disable:4005)    // macro redifinition (in flex file)
#pragma warning(disable:4146)    // unsigned unary minus
#pragma warning(disable:4996)    // swap ranges
#pragma warning(disable:4201)    // nonstandard extension used : nameless struct / union
#pragma warning(disable:4324)    // structure was padded due to alignment specifier
#pragma warning(disable:4067)    // unexpected tokens following preprocessor directive - expected a newline
#pragma warning(disable:4800)    // forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable:4127)    // conditional expression is constant
#pragma warning(disable:4702)    // unreachable code (due to exceptions)
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#pragma clang diagnostic ignored "-Wmissing-braces"
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

#include <assert.h>
#include <string.h>

#ifdef _MSC_VER
#include <intsafe.h>
#include <stdarg.h>
#endif

#ifdef _MSC_VER
  #ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES 1
  #endif
#endif
#include <math.h>

#include <stdint.h>
#include <daScript/das_config.h>

#ifndef _MSC_VER
    #define __forceinline inline __attribute__((always_inline))
#endif

#if defined(_MSC_VER) && !defined(__clang__)
__forceinline uint32_t __builtin_clz(uint32_t x) {
    unsigned long r = 0;
    _BitScanReverse(&r, x);
    return (31 - r);
}
#endif

__forceinline uint32_t rotl_c(uint32_t a, uint32_t b) {
    b &= 31;
    return (a << b) | (a >> (32 - b));
}

#include "daScript/misc/hal.h"

#ifndef DAS_ALIGNED_ALLOC
#define DAS_ALIGNED_ALLOC 1
inline void *das_aligned_alloc16(uint32_t size) {return (char *)(new vec4f[(size+15)/16]); }
inline void das_aligned_free16(void *ptr) {return delete[] (vec4f*)ptr; }
#endif

#ifndef DAS_FATAL_LOG
#define DAS_FATAL_LOG   printf
#endif

#ifndef DAS_FATAL_ERROR
#define DAS_FATAL_ERROR { assert(0 && "fatal error"); exit(-1); }
#endif

#ifndef DAS_ASSERT
    #define DAS_ASSERT(a)   assert(a)
#endif

#ifndef DAS_VERIFY
    #ifdef NDEBUG
        #define DAS_VERIFY(a)   (a)
    #else
        #define DAS_VERIFY(a)   assert(a)
    #endif
#endif

#ifndef DAS_ASSERTF
    #define DAS_ASSERTF(a, msg, ...)   assert(a && msg)
#endif

#ifndef DAS_VERIFYF
    #ifdef NDEBUG
        #define DAS_VERIFYF(a, msg, ...)    (a)
    #else
        #define DAS_VERIFYF(a, msg, ...)   assert(a && msg)
    #endif
#endif

