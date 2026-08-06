#include <doctest/doctest.h>

#include "daScript/daScript.h"

namespace das {
    // forward declaration from module_builtin_fio.cpp (same as module_jit.cpp uses)
    void * register_dynamic_module(const char *, const char *, int, Context *, LineInfoArg *);
    DAS_API void retry_pending_dynamic_modules();
    DAS_API string describe_pending_dynamic_modules();
    DAS_API int report_pending_dynamic_modules();
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

// Deferred must not mean silent forever. A standalone exe that ships a dynamic
// module used to die much later on an unrelated-looking "Failed to find <fn> in
// module <mod>", and the compiler reported the missing module as a plain "file
// not found" path typo. Both diagnostics now read the surface below, so a
// still-unloadable module has to be enumerable BY NAME, with its dlerror.
TEST_CASE("register_dynamic_module — a deferred Quiet failure stays reportable") {
    // The pending list is process-wide and a permanently-missing entry never
    // drains, so assert on the DELTA rather than an empty starting state.
    das::retry_pending_dynamic_modules();
    const int before = das::report_pending_dynamic_modules();

    CHECK(das::register_dynamic_module("/nonexistent-dir-for-tests/dasModuleNoSuchThing.shared_module",
        "Module_NoSuchThing", Quiet, nullptr, nullptr) == nullptr);

    // This text is what the compiler appends to `missing prerequisite ...; file
    // not found` — the whole point is that a load failure stops reading as a
    // mistyped path.
    const das::string note = das::describe_pending_dynamic_modules();
    CHECK(note.find("Module_NoSuchThing") != das::string::npos);
    CHECK(note.find("dasModuleNoSuchThing.shared_module") != das::string::npos);
    CHECK(note.find('\n') != das::string::npos);   // one line per entry

    // The count is what the generated standalone-exe epilogue fatals on, and a
    // retry cannot rescue a file that is genuinely absent.
    CHECK(das::report_pending_dynamic_modules() == before + 1);
    das::retry_pending_dynamic_modules();
    CHECK(das::report_pending_dynamic_modules() == before + 1);
}
