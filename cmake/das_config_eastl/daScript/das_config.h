#pragma once

// Dagor-flavored das_config.h for CI compile-coverage (build_eastl job).
//
// Tracks DagorEngine's prog/1stPartyLibs/daScript/das_config.h as closely as a
// standalone build can: EASTL containers, dag::Vector as das::vector (uint32_t
// size_type!), ska::flat_hash_map as das_map/das_hash_map, DAS_CUSTOM_HASH left
// undefined, no global new/delete, DAS_BIND_EXTERNAL=0, no fileio off-PC.
// EASTL, dag/ and ska_hash_map/ headers come from a DagorEngine sparse checkout,
// so container-API drift is caught against the real thing.
//
// Deliberate deviations from dagor (no engine kernel here):
// - asserts go to assert(), fatals to abort(), logs to stderr
// - dag_config.h is shadowed (see ../dag/dag_config.h)
// - FMT_THROW block is kept from the stock config; dagor's fmt is configured
//   elsewhere in the engine build
// The job is satisfied if daslang compiles; running tests is not required.

#include <EASTL/sort.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/set.h>
#include <EASTL/map.h>
#include <EASTL/unordered_set.h>
#include <EASTL/unordered_map.h>
#include <EASTL/string.h>
#include <EASTL/deque.h>
#include <EASTL/memory.h>
#include <EASTL/type_traits.h>
#include <EASTL/initializer_list.h>
#include <EASTL/functional.h>
#include <EASTL/algorithm.h>
#include <dag/dag_relocatable.h>
#include <dag/dag_vector.h>
#include <cstddef>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iosfwd>
#include <sstream>
#include <climits>
#include <limits.h>
#ifdef _MSC_VER
#include <malloc.h>
#endif

// dagor build settings the engine passes on the command line. A console build
// passes _TARGET_PC=0 (which turns on DAS_NO_FILEIO below) and DAS_TARGET_PS=1
// (no environment; see misc/env_cfg.h); the noplatform lane sets both by hand.
#ifndef DAGOR_DBGLEVEL
#define DAGOR_DBGLEVEL 0
#endif
#ifndef _TARGET_PC
#define _TARGET_PC 1
#endif
#ifndef _TARGET_64BIT
#define _TARGET_64BIT 1
#endif

namespace das
{
template <typename T, typename Allocator = EASTLAllocatorType, bool init_constructing = dag::is_type_init_constructing<T>::value,
  typename Counter = uint32_t>
using vector = dag::Vector<T, Allocator, init_constructing, Counter>;
using namespace eastl;
using eastl::deque;
using eastl::max;
using eastl::min;
using std::atomic;
using std::condition_variable;
using std::lock_guard;
using std::mutex;
using std::nullptr_t;
using std::recursive_mutex;
using std::stringstream;
using std::thread;
using std::unique_lock;
namespace this_thread
{
using std::this_thread::yield;
}
namespace chrono
{
using std::chrono::milliseconds;
}
} // namespace das
#define DAS_USE_EASTL 1

// one of three identical copies - include/daScript/das_config.h, include/misc/include_fmt.h,
// cmake/das_config_eastl/daScript/das_config.h; whichever a TU includes first must stand alone
namespace das {
  void das_throw(const char * msg);
}
#if (!defined(DAS_ENABLE_EXCEPTIONS)) || (!DAS_ENABLE_EXCEPTIONS)
#ifndef DAS_FMT_THROW_DEFINED
#define DAS_FMT_THROW_DEFINED
namespace das {
  void das_stash_throw(const char * msg);
  void das_throw_stashed();
  // the panic longjmp never unwinds (POSIX never does; the JIT stack cannot be unwound);
  // the temporary dies when the stash statement ends - the jump crosses nothing live
  template <typename BuildAndStash>
  inline void das_stash_then_throw(BuildAndStash && buildAndStash) {
      buildAndStash();
      das_throw_stashed();
  }
}
#define FMT_THROW(x)    das::das_stash_then_throw([&]{ das::das_stash_throw(((x).what())); })
#endif
#endif

#define DAS_GLOBAL_NEW 1

#define DAS_BIND_EXTERNAL 0

#define DAS_SMART_PTR_TRACKER 0
#if defined(_DEBUG_TAB_)
#define DAS_SMART_PTR_MAGIC 1
#else
#define DAS_SMART_PTR_MAGIC 0
#endif

#define DAS_NO_GLOBAL_NEW_AND_DELETE 1

#define DAS_FAST_INTEGER_MOD 0 // fast-math does not work with integer mod

#ifdef __SANITIZE_ADDRESS__
#define DAS_SAFE_HASH 1
#elif defined(__has_feature)
#if __has_feature(address_sanitizer) || __has_feature(hwaddress_sanitizer)
#define DAS_SAFE_HASH 1
#endif
#endif

#if defined(_DEBUG_TAB_) || DAGOR_DBGLEVEL > 1
#define DAS_ASSERT(cond) assert(cond)
#define DAS_ASSERTF(cond, ...) assert(cond)
#else
#define DAS_ASSERT(cond) assert(cond)
#define DAS_ASSERTF(cond, ...) DAS_ASSERT(cond)
#endif
#define DAS_FATAL_LOG das_dagor_ci_logerr
#define DAS_FATAL_ERROR(...)         \
  {                                  \
    das_dagor_ci_logerr(__VA_ARGS__); \
    abort();                         \
  }
#if !_TARGET_PC
#define DAS_NO_FILEIO 1
#endif

#ifndef DAS_USE_BASE_CRASH_HANDLER
#define DAS_USE_BASE_CRASH_HANDLER 0
#endif

// dagor's config includes <vecmath/dag_vecMathDecl.h> here so the engine's vec4f
// wins; skipped here because it is the same header daslang vendors, and at this
// point platform.h has not defined __forceinline yet.

#define DAS_ALIGNED_ALLOC 1
inline void *das_aligned_alloc16(uint64_t size) { return new char[size]; }
inline void das_aligned_free16(void *ptr) { delete[] (char *)ptr; }
inline size_t das_aligned_memsize(void *) { return 0; } // dagor asks defaultmem; no allocator here

// if enabled, the generated interop will be marginally slower
// the upside is that it well generate significantly less templated code, thus reducing compile time (and binary size)
#ifndef DAS_SLOW_CALL_INTEROP
#define DAS_SLOW_CALL_INTEROP 0
#endif

#ifndef DAS_FUSION
#define DAS_FUSION 0
#endif

#if DAS_SLOW_CALL_INTEROP
#undef DAS_FUSION
#define DAS_FUSION 0
#endif

#ifndef DAS_DEBUGGER
#if DAGOR_DBGLEVEL > 0
#define DAS_DEBUGGER 1
#endif
#endif

#ifndef DAS_MAX_FUNCTION_ARGUMENTS
#define DAS_MAX_FUNCTION_ARGUMENTS 32
#endif

#ifndef DAS_PRINT_VEC_SEPARATROR
#define DAS_PRINT_VEC_SEPARATROR ", "
#endif

#include <ska_hash_map/flat_hash_map2.hpp>
namespace das
{
// Dagor has no in-tree das_hash_map: DAS_CUSTOM_HASH stays undefined and every
// das_* map is ska, so daslang_hash lives here and forwards to eastl::hash.
// Headers may specialize it for their own key types and have that picked up.
template <typename T, typename Enable = void>
struct daslang_hash
{
  size_t operator()(const T &key) const noexcept { return das::hash<T>()(key); }
};
template <typename K, typename V, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_map = ska::flat_hash_map<K, V, H, E>;
template <typename K, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_set = ska::flat_hash_set<K, H, E>;
template <typename K, typename V, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_hash_map = ska::flat_hash_map<K, V, H, E>;
template <typename K, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_hash_set = ska::flat_hash_set<K, H, E>;
template <typename K, typename V>
using das_safe_map = eastl::map<K, V>;
template <typename K, typename C = das::less<K>>
using das_safe_set = eastl::set<K, C>;
// Insert-only flavors alias to the same ska type: the insert-only overloads in
// the serializer and das_common.h are guarded behind DAS_CUSTOM_HASH precisely
// so the regular das_hash_map overloads serve insert-only args here.
template <typename K, typename V, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_insert_only_map = ska::flat_hash_map<K, V, H, E>;
template <typename K, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_insert_only_set = ska::flat_hash_set<K, H, E>;
template <typename K, typename V, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_insert_only_hash_map = ska::flat_hash_map<K, V, H, E>;
template <typename K, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_insert_only_hash_set = ska::flat_hash_set<K, H, E>;
} // namespace das

// PS libc has no environment functions, so every read goes through das_getenv in
// misc/env_cfg.cpp - the one TU that defines DAS_ALLOW_GETENV. Poisoning here (after
// <stdlib.h>/<cstdlib> have declared it) turns a new direct call into a compile error
// instead of a link-time surprise on console.
#include <cstdlib>
#ifndef DAS_ALLOW_GETENV
#pragma GCC poison getenv secure_getenv putenv setenv
#endif

// stands in for dagor's logerr / logmessage_
inline void das_dagor_ci_logerr(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  fputc('\n', stderr);
  fflush(stderr);
}

// dagor routes this to logmessage_, which drops the prefix argument
#ifndef das_to_stdout_level_prefix_text
#define das_to_stdout_level_prefix_text(level, prefix, text) \
  {                                                          \
    (void)(prefix);                                          \
    if (level >= das::LogLevel::error)                       \
    {                                                        \
      fprintf(stderr, "%s", text);                           \
      fflush(stderr);                                        \
    }                                                        \
    else                                                     \
    {                                                        \
      fprintf(stdout, "%s", text);                           \
      fflush(stdout);                                        \
    }                                                        \
  }
#endif
