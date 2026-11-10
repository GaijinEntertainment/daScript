#include <doctest/doctest.h>

#include "daScript/daScript.h"

using namespace das;

// A `shared` module is promoted to the global module registry when first
// compiled as a dependency, so a later `require` finds it "no file needed" --
// even from a script in a different directory (see tutorials/integration/c/
// 13_shared_module.c). The match is by the module's canonical require STRING,
// not a directory-relative file path, so:
//   * `require my_xmod` resolves the promoted module from ANY directory, and
//   * a mis-qualified bare `require fio` (canonical `daslib/fio`) still fails.

namespace {

static ProgramPtr compileNamed(const char * name, const char * src, ModuleGroup & lib) {
    TextPrinter tout;
    auto fAccess = make_smart<FsFileAccess>();
    fAccess->setFileInfo(name, make_unique<TextFileInfo>(src, uint32_t(strlen(src)), /*own*/false));
    return compileDaScript(name, fAccess, tout, lib);
}

static const char * SHARED_MOD =
    "options gen2\n"
    "module my_xmod shared\n"
    "[export] def x_double(a : int) : int { return a * 2 }\n";

} // namespace

TEST_CASE("shared module resolves across directories via the promoted registry") {
    // 1) Promote my_xmod by compiling a loader that requires it. The loader and
    //    the module file both live at the root, and share ONE FileAccess.
    {
        ModuleGroup lib;
        TextPrinter tout;
        auto fAccess = make_smart<FsFileAccess>();
        fAccess->setFileInfo("my_xmod.das", make_unique<TextFileInfo>(SHARED_MOD, uint32_t(strlen(SHARED_MOD)), false));
        const char * loader = "options gen2\nrequire my_xmod\n";
        fAccess->setFileInfo("loader.das", make_unique<TextFileInfo>(loader, uint32_t(strlen(loader)), false));
        auto p = compileDaScript("loader.das", fAccess, tout, lib);
        REQUIRE(p);
        CHECK_FALSE(p->failed());
    }

    // 2) A requirer in a SUBDIRECTORY, with a FRESH FileAccess that does NOT
    //    carry my_xmod.das. It can only resolve via the promoted registry.
    //    Before the fix this failed with error[20605] file not found, because
    //    resolution recomputed a directory-relative "scripts/bug/my_xmod.das".
    {
        ModuleGroup lib;
        auto p = compileNamed("scripts/bug/bug.das", "options gen2\nrequire my_xmod\n", lib);
        REQUIRE(p);
        CHECK_FALSE(p->failed());
    }
}

TEST_CASE("bare `require fio` fails every time -- promoted daslib/fio is not name-matched") {
    // Promote daslib/fio (module fio, canonical require "daslib/fio").
    {
        ModuleGroup lib;
        auto p = compileNamed("load_fio.das", "options gen2\nrequire daslib/fio\n", lib);
        REQUIRE(p);
        CHECK_FALSE(p->failed());
    }
    // A bare `require fio` must NOT resolve the promoted module -- the canonical
    // require identity "daslib/fio" != "fio", so it falls through to a file
    // lookup that fails. Order-independent: holds whether or not fio was loaded.
    {
        ModuleGroup lib;
        auto p = compileNamed("bare_fio.das", "options gen2\nrequire fio\n", lib);
        REQUIRE(p);
        CHECK(p->failed());
    }
}

TEST_CASE("a compile that ignores shared modules builds its own builtin, never the promoted one") {
    {
        ModuleGroup lib;
        auto p = compileNamed("promote_builtin.das", "options gen2\n", lib);
        REQUIRE(p);
        REQUIRE_FALSE(p->failed());
    }
    auto promoted = Module::requireEx("builtin", true);
    REQUIRE(promoted);
    REQUIRE(bool(promoted->promoted));

    ModuleGroup lib;
    TextPrinter tout;
    auto fAccess = make_smart<FsFileAccess>();
    const char * src = "options gen2\n";
    fAccess->setFileInfo("own_builtin.das", make_unique<TextFileInfo>(src, uint32_t(strlen(src)), false));
    CodeOfPolicies policies;
    policies.ignore_shared_modules = true;
    auto p = compileDaScript("own_builtin.das", fAccess, tout, lib, policies);
    REQUIRE(p);
    REQUIRE_FALSE(p->failed());
    Module * own = nullptr;
    for ( auto m : lib.getModules() ) {
        if ( m->name == "builtin" ) own = m;
    }
    REQUIRE(own);
    CHECK(own != promoted);
    CHECK_FALSE(bool(own->promoted));
}
