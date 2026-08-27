#pragma once

// nano shadow of daScript/das_config.h.
//
// This header is the reason nano works at all: everything in include/ reaches
// the standard library through it, so replacing it - by putting nano/include
// ahead of include/ in the search order - re-points the whole runtime at a
// smaller set of headers without one #ifdef in the shared sources.
//
// Pre-C++20 inclusion order note: platform.h pulls this in before it defines
// __forceinline / DAS_SUPPRESS_UB / NO_ASAN_INLINE, so anything included here
// must not use those macros without its own fallback.

#ifndef DAS_CUSTOM_HASH
#define DAS_CUSTOM_HASH 1
#endif

#include <map>
#include <set>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <type_traits>
#include <initializer_list>
#include <algorithm>
#include <climits>

#include <limits.h>
#include <setjmp.h>

#include <functional>

// libstdc++ without gthreads - every bare-metal cross-compiler - ships <mutex>
// empty, and nano is single-threaded there, so its locks are the no-ops they
// would have been anyway. Any other toolchain gets the real header.
#if defined(__GLIBCXX__) && !defined(_GLIBCXX_HAS_GTHREADS)
namespace das {
    struct mutex {
        void lock() {}
        void unlock() {}
        bool try_lock() { return true; }
    };
    struct recursive_mutex {
        void lock() {}
        void unlock() {}
        bool try_lock() { return true; }
    };
    template <typename TT> struct lock_guard {
        explicit lock_guard(TT &) {}
        lock_guard(const lock_guard &) = delete;
        lock_guard & operator = (const lock_guard &) = delete;
    };
}
#else
#include <mutex>
#endif

#if defined(__GLIBCXX__) && !defined(_GLIBCXX_HAS_GTHREADS)
// Single-threaded: thread-local storage needs a runtime the target has none of,
// and the smart-pointer leak ledger needs a 64-bit atomic it cannot do either.
// Nothing here reads either one - nano reports no leaks.
// TAG is what gives each declaration its own storage, exactly as the hosted
// DasThreadLocal uses it - without it every DAS_THREAD_LOCAL of the same type
// in a translation unit would share one function-local static.
template <typename TT, unsigned long long TAG> class DasSingleThreaded final {
public:
    inline TT & operator * () { static TT value_{}; return value_; }
    inline TT * operator -> () { return &(**this); }
};
#define DAS_THREAD_LOCAL(X)             DasSingleThreaded<X, das::hash_tag_file_name(DAS_FILE_LINE)>
#define DAS_NEW_SMART_PTR_ID
#define DAS_DELETE_SMART_PTR_ID
#define DAS_TRACK_SMART_PTR_ID
#define DAS_TRACK_SMART_PTR_ID_DTOR
#define DAS_SMART_PTR_NEW
#define DAS_SMART_PTR_DELETE
// Nothing reports a heap on a target with nowhere to report to, and the bodies
// walk every deck and format it.
#define DAS_HEAP_REPORT 0
#define DAS_DEBUG_INFO_NAME_CACHE 0
// Integer modulo via double division is a win where FP division is fast and a
// rout where there is no hardware double at all - it pulls the whole soft-float
// library in for one operator.
#define DAS_FAST_INTEGER_MOD 0
#endif

namespace das {using namespace std;}

#undef DAS_ENABLE_EXCEPTIONS
#define DAS_ENABLE_EXCEPTIONS 0

namespace das {
  void das_throw(const char * msg);
}
#ifndef DAS_FMT_THROW_DEFINED
#define DAS_FMT_THROW_DEFINED
namespace das {
  void das_stash_throw(const char * msg);
  void das_throw_stashed();
  // the panic longjmp never unwinds (POSIX never does; the JIT stack cannot be
  // unwound); the temporary dies when the stash statement ends - the jump
  // crosses nothing live
  template <typename BuildAndStash>
  inline void das_stash_then_throw(BuildAndStash && buildAndStash) {
      buildAndStash();
      das_throw_stashed();
  }
}
#define FMT_THROW(x)    das::das_stash_then_throw([&]{ das::das_stash_throw(((x).what())); })
#endif

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
template <typename K, typename V, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_insert_only_map = das::daslang_insert_only_hash_map<K,V,H,E>;
template <typename K, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_insert_only_set = das::daslang_insert_only_hash_set<K,H,E>;
template <typename K, typename V, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_insert_only_hash_map = das::daslang_insert_only_hash_map<K,V,H,E>;
template <typename K, typename H = das::daslang_hash<K>, typename E = das::equal_to<K>>
using das_insert_only_hash_set = das::daslang_insert_only_hash_set<K,H,E>;
template <typename K, typename V>
using das_safe_map = std::map<K,V>;
template <typename K, typename C=das::less<K>>
using das_safe_set = std::set<K,C>;
}

#define DAS_STD_HAS_BIND 1

#ifndef DAS_MAX_FUNCTION_ARGUMENTS
#define DAS_MAX_FUNCTION_ARGUMENTS 32
#endif

#undef DAS_FUSION
#define DAS_FUSION 0
#undef DAS_DEBUGGER
#define DAS_DEBUGGER 0
#undef DAS_BIND_EXTERNAL
#define DAS_BIND_EXTERNAL 0
#undef DAS_USE_BASE_CRASH_HANDLER
#define DAS_USE_BASE_CRASH_HANDLER 0

#ifndef DAS_PRINT_VEC_SEPARATROR
#define DAS_PRINT_VEC_SEPARATROR ","
#endif

namespace das {
    void das_nano_write ( int level, const char * prefix, const char * text );
}
#ifndef das_to_stdout_level_prefix_text
#define das_to_stdout_level_prefix_text(level, prefix, text) das::das_nano_write(int(level), prefix, text)
#endif
