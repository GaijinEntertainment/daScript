#include <doctest/doctest.h>

#include "daScript/daScript.h"

namespace das {
    // forward declaration from module_builtin_fio.cpp (same as module_jit.cpp uses)
    void * register_dynamic_module(const char *, const char *, int, Context *, LineInfoArg *);
}

namespace {

// A library guaranteed loadable on each CI platform but which is NOT a das
// module — drives register_dynamic_module into the registrator-missing branch.
const char * loadable_non_das_library() {
#if defined(_WIN32) || defined(_WIN64)
    return "kernel32.dll";
#elif defined(__APPLE__)
    return "/usr/lib/libSystem.B.dylib";
#else
    return "libm.so.6";
#endif
}

enum { Quiet = 0, ErrorMsg = 1, Fail = 2 }; // mirrors RegisterOnError in module_builtin_fio.cpp

} // anon

// Issue #2580: these calls used to segfault — the registrator-missing branch
// called context->to_err unconditionally, and JIT/standalone callers pass
// context=nullptr. The failure must now report via LOG(error) and return null.
TEST_CASE("register_dynamic_module — registrator missing, nullptr context") {
    SUBCASE("Quiet (the jit_register_dynamic_module signature)") {
        CHECK(das::register_dynamic_module(loadable_non_das_library(),
            "Module_DoesNotExist", Quiet, nullptr, nullptr) == nullptr);
    }
    SUBCASE("ErrorMsg") {
        CHECK(das::register_dynamic_module(loadable_non_das_library(),
            "Module_DoesNotExist", ErrorMsg, nullptr, nullptr) == nullptr);
    }
    SUBCASE("Fail must not throw without a context to throw through") {
        void * result = &result; // poison — must come back null
        CHECK_NOTHROW(result = das::register_dynamic_module(loadable_non_das_library(),
            "Module_DoesNotExist", Fail, nullptr, nullptr));
        CHECK(result == nullptr);
    }
}

TEST_CASE("register_dynamic_module — dlopen failure, nullptr context, non-Quiet") {
    // Quiet dlopen failures are deferred to the retry queue by design; non-Quiet
    // ones with no context must report via LOG(error) and return null, not crash.
    CHECK(das::register_dynamic_module("no/such/path.no_such_ext",
        "Module_DoesNotExist", ErrorMsg, nullptr, nullptr) == nullptr);
}
