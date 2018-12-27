#pragma once

#ifdef _MSC_VER
#include <intrin.h>
#endif

#include <xmmintrin.h>
#include <signal.h>
#include <assert.h>

#ifdef _MSC_VER
#include <intsafe.h>
#endif

#include <stdint.h>

#include <cmath>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
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

#ifdef _MSC_VER
#pragma warning(disable:4005)	// macro redifinition (in flex file)
#pragma warning(disable:4146)	// unsigned unary minus
#pragma warning(disable:4996)	// swap ranges
#endif


