#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/misc/sysos.h"

#include <filesystem>
#include <set>
#include <string>

// Test entry points exposed by src/builtin/module_jit.cpp.
extern "C" {
    DAS_API void jit_set_exe_file_for_test_( const char * (*fn)() );
    DAS_API void jit_set_path_exists_for_test_( bool (*fn)(const char *) );
    DAS_API bool jit_resolve_dynamic_module_path_for_test_(
        const char * rel_path,
        const char * fallback_abs_path,
        char * out_buf,
        size_t buf_size );
}

namespace {

// Synthetic exe-file source for the resolve helper to read.
static std::string g_exe_file;
static const char * fake_exe_file() { return g_exe_file.c_str(); }

// Synthetic existence predicate — succeeds for any path in the set, fails for
// everything else. No real filesystem access.
//
// Both sides are normalized to forward-slash form via `generic_string()` so
// the test is host-agnostic: on Windows the resolver builds candidates via
// std::filesystem::path which emits backslashes, but the test inputs and
// expectations all use forward slashes for readability.
static std::set<std::string> g_existing;
static bool fake_path_exists(const char * p) {
    return g_existing.count(std::filesystem::path(p).generic_string()) != 0;
}

struct TestSeams {
    TestSeams() {
        g_exe_file.clear();
        g_existing.clear();
        jit_set_exe_file_for_test_(fake_exe_file);
        jit_set_path_exists_for_test_(fake_path_exists);
    }
    ~TestSeams() {
        jit_set_exe_file_for_test_(nullptr);
        jit_set_path_exists_for_test_(nullptr);
        // Leave das_root untouched — we don't override it; tests pick rel_paths
        // that don't accidentally match getDasRoot()/<rel> on the dev machine
        // by populating g_existing exclusively via fake_path_exists.
    }
};

static std::string resolve(const char * rel, const char * abs) {
    char buf[4096] = {0};
    REQUIRE(jit_resolve_dynamic_module_path_for_test_(rel, abs, buf, sizeof(buf)));
    // Normalize to forward-slash form so CHECK comparisons are host-agnostic.
    return std::filesystem::path(buf).generic_string();
}

} // anon

TEST_CASE("jit module resolve — exe-relative wins when present") {
    TestSeams seams;
    g_exe_file = "/bundle/myapp";
    g_existing.insert("/bundle/modules/dasFoo/dasFoo.shared_module");
    auto chosen = resolve("modules/dasFoo/dasFoo.shared_module",
                          "/build/abs/modules/dasFoo/dasFoo.shared_module");
    CHECK(chosen == "/bundle/modules/dasFoo/dasFoo.shared_module");
}

TEST_CASE("jit module resolve — falls back to das_root when exe-relative misses") {
    TestSeams seams;
    g_exe_file = "/somewhere/myapp";
    // exe-relative does NOT exist; das_root path DOES (use the dev machine's
    // real das_root so existence check passes against our fake predicate)
    auto dasRoot = das::getDasRoot();
    auto dasRootCandidate = dasRoot + "/modules/dasFoo/dasFoo.shared_module";
    g_existing.insert(dasRootCandidate);
    auto chosen = resolve("modules/dasFoo/dasFoo.shared_module",
                          "/build/abs/modules/dasFoo/dasFoo.shared_module");
    CHECK(chosen == dasRootCandidate);
}

TEST_CASE("jit module resolve — falls back to baked absolute when both miss") {
    TestSeams seams;
    g_exe_file = "/somewhere/myapp";
    // No path exists anywhere
    auto chosen = resolve("modules/dasFoo/dasFoo.shared_module",
                          "/build/abs/modules/dasFoo/dasFoo.shared_module");
    CHECK(chosen == "/build/abs/modules/dasFoo/dasFoo.shared_module");
}

TEST_CASE("jit module resolve — exe-relative wins over das_root when both exist") {
    TestSeams seams;
    g_exe_file = "/bundle/myapp";
    g_existing.insert("/bundle/modules/dasFoo/dasFoo.shared_module");
    auto dasRoot = das::getDasRoot();
    g_existing.insert(dasRoot + "/modules/dasFoo/dasFoo.shared_module");
    auto chosen = resolve("modules/dasFoo/dasFoo.shared_module",
                          "/build/abs/modules/dasFoo/dasFoo.shared_module");
    CHECK(chosen == "/bundle/modules/dasFoo/dasFoo.shared_module");
}

TEST_CASE("jit module resolve — empty rel_path skips tier 1+2 and uses absolute") {
    TestSeams seams;
    g_exe_file = "/bundle/myapp";
    auto chosen = resolve("", "/baked/abs/path.shared_module");
    CHECK(chosen == "/baked/abs/path.shared_module");
}

TEST_CASE("jit module resolve — empty exe_file still tries das_root") {
    TestSeams seams;
    g_exe_file = "";  // platform with no exe-file (fall through to tier 2)
    auto dasRoot = das::getDasRoot();
    auto dasRootCandidate = dasRoot + "/modules/dasFoo/dasFoo.shared_module";
    g_existing.insert(dasRootCandidate);
    auto chosen = resolve("modules/dasFoo/dasFoo.shared_module",
                          "/build/abs/modules/dasFoo/dasFoo.shared_module");
    CHECK(chosen == dasRootCandidate);
}

#ifndef DAS_NO_ASSERTIONS
TEST_CASE("jit module resolve — picks rel_path even when only _debug variant exists (debug builds)") {
    TestSeams seams;
    g_exe_file = "/bundle/myapp";
    // Only the _debug variant exists in the bundle. The helper should still
    // return the non-_debug name (register_dynamic_module rewrites internally).
    g_existing.insert("/bundle/modules/dasFoo/dasFoo_debug.shared_module");
    auto chosen = resolve("modules/dasFoo/dasFoo.shared_module",
                          "/build/abs/modules/dasFoo/dasFoo.shared_module");
    CHECK(chosen == "/bundle/modules/dasFoo/dasFoo.shared_module");
}
#endif

TEST_CASE("jit module resolve — POSIX filesystem root exe path is handled") {
    // Bundle layout where the exe lives at filesystem root (e.g. minimal Docker
    // image, embedded). std::filesystem::path("/myapp").parent_path() is "/",
    // which path::operator/ joins correctly without doubling separators.
    TestSeams seams;
    g_exe_file = "/myapp";
    g_existing.insert("/modules/dasFoo/dasFoo.shared_module");
    auto chosen = resolve("modules/dasFoo/dasFoo.shared_module",
                          "/build/abs/modules/dasFoo/dasFoo.shared_module");
    CHECK(chosen == "/modules/dasFoo/dasFoo.shared_module");
}

#ifdef _WIN32
// Backslash-separator paths are only meaningful on Windows; std::filesystem on
// POSIX treats `\` as a regular filename character. The two cases below verify
// the resolver builds correct candidates on Windows for both an exe in a
// subdirectory and an exe in the drive root.
//
// Inputs use backslashes (the form Windows produces from getExecutableFileName);
// expected values use forward slashes because the test helper normalizes via
// generic_string() — see fake_path_exists and resolve() above.
TEST_CASE("jit module resolve — Windows backslash exe path is handled") {
    TestSeams seams;
    g_exe_file = "C:\\bundle\\myapp.exe";
    g_existing.insert("C:/bundle/modules/dasFoo/dasFoo.shared_module");
    auto chosen = resolve("modules/dasFoo/dasFoo.shared_module",
                          "C:\\build\\abs\\modules\\dasFoo\\dasFoo.shared_module");
    CHECK(chosen == "C:/bundle/modules/dasFoo/dasFoo.shared_module");
}

TEST_CASE("jit module resolve — Windows drive-root exe path is handled") {
    // Exe at C:\myapp.exe — drive root. parent_path must return "C:\" (not
    // "C:") so the joined candidate stays on the drive root rather than
    // becoming the malformed drive-relative "C:modules\...".
    TestSeams seams;
    g_exe_file = "C:\\myapp.exe";
    g_existing.insert("C:/modules/dasFoo/dasFoo.shared_module");
    auto chosen = resolve("modules/dasFoo/dasFoo.shared_module",
                          "C:\\build\\abs\\modules\\dasFoo\\dasFoo.shared_module");
    CHECK(chosen == "C:/modules/dasFoo/dasFoo.shared_module");
}
#endif
