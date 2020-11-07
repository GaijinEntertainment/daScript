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
#pragma warning(disable:4316)    // '__m128': object allocated on the heap may not be aligned 16
#pragma warning(disable:4714)    // marked as __forceinline not inlined
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-value"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#pragma clang diagnostic ignored "-Wmissing-braces"
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#pragma clang diagnostic ignored "-Wextra-semi"
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wignored-qualifiers"
#endif

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wmissing-braces"
#pragma GCC diagnostic ignored "-Wignored-attributes"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#pragma GCC diagnostic ignored "-Wsequence-point"
#if __GNUC__>=9
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif
#endif

#include <assert.h>
#include <string.h>

#ifdef _MSC_VER
#include <intsafe.h>
#include <stdarg.h>
#endif

#ifdef _MSC_VER
    #define DAS_NON_POD_PADDING 1
#else
    #define DAS_NON_POD_PADDING 0
#endif

#ifdef _MSC_VER
  #ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES 1
  #endif
#endif
#include <math.h>

#include <stdint.h>
#include <float.h>
#include <daScript/das_config.h>

#ifndef _MSC_VER
    #define __forceinline inline __attribute__((always_inline))
#endif

#if defined(__has_feature)
    #if __has_feature(address_sanitizer)
        #define DAS_SUPRESS_UB  __attribute__((no_sanitize("undefined")))
    #endif
#endif

#ifndef DAS_SUPRESS_UB
#define DAS_SUPRESS_UB
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define DAS_NORETURN_PREFIX
    #define DAS_NORETURN_SUFFIX  __attribute__((noreturn))
#elif defined(_MSC_VER)
    #if _MSC_VER>=1900
        #define DAS_NORETURN_PREFIX  __declspec(noreturn)
        #define DAS_NORETURN_SUFFIX
    #else
        #define DAS_NORETURN_PREFIX
        #define DAS_NORETURN_SUFFIX
    #endif
#else
    #define DAS_NORETURN_PREFIX
    #define DAS_NORETURN_SUFFIX
#endif

#if defined(_MSC_VER) && !defined(__clang__)
__forceinline uint32_t __builtin_clz(uint32_t x) {
    unsigned long r = 0;
    _BitScanReverse(&r, x);
    return uint32_t(31 - r);
}
#endif

#ifdef _MSC_VER

__forceinline uint32_t rotl_c(uint32_t a, uint32_t b) {
    return _rotl(a, b);
}

__forceinline uint32_t rotr_c(uint32_t a, uint32_t b) {
    return _rotr(a, b);
}

#else

__forceinline uint32_t rotl_c(uint32_t a, uint32_t b) {
    return (a << (b & 31)) | (a >> ((32 - b) & 31));
}

__forceinline uint32_t rotr_c(uint32_t a, uint32_t b) {
    return (a >> (b &31)) | (a << ((32 - b) & 31));
}

#endif

#include "daScript/misc/hal.h"

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
    #define DAS_ASSERTF(a, msg, ...)   assert((a) && (msg))
#endif

#ifndef DAS_VERIFYF
    #ifdef NDEBUG
        #define DAS_VERIFYF(a, msg, ...)   (a)
    #else
        #define DAS_VERIFYF(a, msg, ...)   assert((a) && (msg))
    #endif
#endif

#ifndef DAS_ALIGNED_ALLOC
#define DAS_ALIGNED_ALLOC 1
inline void *das_aligned_alloc16(uint32_t size) {
#if defined(_MSC_VER)
    return _aligned_malloc(size, 16);
#else
    void * mem = nullptr;
    if (posix_memalign(&mem, 16, size)) {
        DAS_ASSERTF(0, "posix_memalign returned nullptr");
        return nullptr;
    }
    return mem;
#endif
}
inline void das_aligned_free16(void *ptr) {
#if defined(_MSC_VER)
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}
#endif

#ifndef DAS_TRACK_ALLOCATIONS
#define DAS_TRACK_ALLOCATIONS   0
#endif

#ifndef DAS_SANITIZER
#define DAS_SANITIZER   0
#endif

#include "daScript/misc/smart_ptr.h"

