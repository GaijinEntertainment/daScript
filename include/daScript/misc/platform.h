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

#ifdef __APPLE__
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#pragma clang diagnostic ignored "-Wmissing-braces"
#endif

#include <signal.h>
#include <assert.h>
#include <setjmp.h>

#ifdef _MSC_VER
#include <intsafe.h>
#endif

#include <stdint.h>

#include <cmath>
#include <set>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <type_traits>
#include <initializer_list>
#include <functional>
#include <algorithm>

#ifdef _MSC_VER
    #define NOMINMAX
#else
    #define __forceinline inline __attribute__((always_inline))
#endif

#ifdef _MSC_VER
__forceinline uint32_t __builtin_clz(uint32_t x) {
    unsigned long r = 0;
    _BitScanReverse(&r, x);
    return (31 - r);
}
#endif

#include "daScript/misc/hal.h"



