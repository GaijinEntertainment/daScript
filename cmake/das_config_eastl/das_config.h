#pragma once

// EASTL-flavored das_config.h for CI compile-coverage.
//
// Goal: catch type-conversion bugs (e.g. eastl::string -> std::filesystem::path)
// that don't surface with the default std-only config. The job is satisfied if
// daslang compiles; full test execution is not required.
//
// Notes:
// - EASTL's smart pointers come from EASTL/shared_ptr.h / EASTL/unique_ptr.h.
// - We deliberately do NOT pull dagor-engine headers (dag::Vector etc.).

#ifndef DAS_CUSTOM_HASH
#define DAS_CUSTOM_HASH 1
#endif

#include <EASTL/sort.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/set.h>
#include <EASTL/map.h>
#include <EASTL/unordered_set.h>
#include <EASTL/unordered_map.h>
#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/deque.h>
#include <EASTL/memory.h>
#include <EASTL/type_traits.h>
#include <EASTL/initializer_list.h>
#include <EASTL/functional.h>
#include <EASTL/algorithm.h>

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iosfwd>
#include <climits>
#include <limits.h>
#include <setjmp.h>
#ifdef _MSC_VER
#include <malloc.h>
#endif

namespace das {
using namespace eastl;
using std::atomic;
using std::condition_variable;
using std::lock_guard;
using std::mutex;
using std::nullptr_t;
using std::recursive_mutex;
using std::stringstream;
using std::thread;
using std::unique_lock;
} // namespace das

#if (!defined(DAS_ENABLE_EXCEPTIONS)) || (!DAS_ENABLE_EXCEPTIONS)
#define FMT_THROW(x)    das::das_throw(((x).what()))
namespace das {
    void das_throw(const char * msg);
}
#endif

#if DAS_CUSTOM_HASH
#include <das_hash_map/das_hash_map.h>
namespace das {
template <typename K, typename V, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_map = das::daslang_hash_map<K,V,H,E>;
template <typename K, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_set = das::daslang_hash_set<K,H,E>;
template <typename K, typename V, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_hash_map = das::daslang_hash_map<K,V,H,E>;
template <typename K, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_hash_set = das::daslang_hash_set<K,H,E>;
template <typename K, typename V>
using das_safe_map = eastl::map<K,V>;
template <typename K, typename C=eastl::less<K>>
using das_safe_set = eastl::set<K,C>;
}
#else
namespace das {
template <typename K, typename V, typename H = eastl::hash<K>, typename E = eastl::equal_to<K>>
using das_map = eastl::unordered_map<K,V,H,E>;
template <typename K, typename H = eastl::hash<K>, typename E = eastl::equal_to<K>>
using das_set = eastl::unordered_set<K,H,E>;
template <typename K, typename V, typename H = eastl::hash<K>, typename E = eastl::equal_to<K>>
using das_hash_map = eastl::unordered_map<K,V,H,E>;
template <typename K, typename H = eastl::hash<K>, typename E = eastl::equal_to<K>>
using das_hash_set = eastl::unordered_set<K,H,E>;
template <typename K, typename V>
using das_safe_map = eastl::map<K,V>;
template <typename K, typename C=eastl::less<K>>
using das_safe_set = eastl::set<K,C>;
}
#endif

#define DAS_STD_HAS_BIND 1

#ifndef DAS_SLOW_CALL_INTEROP
  #define DAS_SLOW_CALL_INTEROP 0
#endif

#ifndef DAS_MAX_FUNCTION_ARGUMENTS
#define DAS_MAX_FUNCTION_ARGUMENTS 32
#endif

#ifndef DAS_FUSION
  #define DAS_FUSION  0
#endif

#if DAS_SLOW_CALL_INTEROP
  #undef DAS_FUSION
  #define DAS_FUSION  0
#endif

#ifndef DAS_DEBUGGER
  #define DAS_DEBUGGER  1
#endif

#ifndef DAS_BIND_EXTERNAL
  #if defined(_WIN32) && defined(_WIN64)
    #define DAS_BIND_EXTERNAL 1
  #elif defined(__APPLE__)
    #define DAS_BIND_EXTERNAL 1
  #elif defined(__linux__)
    #define DAS_BIND_EXTERNAL 1
  #elif defined __HAIKU__
    #define DAS_BIND_EXTERNAL 1
  #else
    #define DAS_BIND_EXTERNAL 0
  #endif
#endif

#ifndef DAS_PRINT_VEC_SEPARATROR
#define DAS_PRINT_VEC_SEPARATROR ","
#endif

#ifndef DAS_USE_BASE_CRASH_HANDLER
#if defined(_WIN32) || ((defined(__linux__) || defined(__APPLE__)) && !defined(__EMSCRIPTEN__))
#define DAS_USE_BASE_CRASH_HANDLER 1
#else
#define DAS_USE_BASE_CRASH_HANDLER 0
#endif
#endif

#ifndef das_to_stdout_level_prefix_text
#define das_to_stdout_level_prefix_text(level, prefix, text) { if (level >= LogLevel::error) { fprintf(stderr, "%s", text); fflush(stderr); } else { fprintf(stdout, "%s%s", prefix, text); fflush(stdout); } }
#endif
