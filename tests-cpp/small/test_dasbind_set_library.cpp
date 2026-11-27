#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/aot_builtin_dasbind.h"

#include <cstring>

using namespace das;

TEST_CASE("a host names the library of an extern alias on the dasbind module") {
    const char * src =
        "options gen2\n"
        "options no_aot\n"
        "require dasbind\n"
        "let @extern_library probe_host = \"dasbind_no_such_probe\"\n"
        "[extern(cdecl, late, name=\"dasbind_probe_dbl_at45\", lib=probe_host)]\n"
        "def probe(a0, a1, a2, a3 : int64; a4, a5 : double) : double => 0.0lf\n"
        "[export]\n"
        "def run : double {\n"
        "    return probe(1l, 2l, 3l, 4l, 1.5lf, 2.5lf)\n"
        "}\n";
    auto dasbind = Module::require("dasbind");
    REQUIRE(dasbind);
    dasbind_set_library(dasbind, "probe_host", "dasbind_probe");
    TextPrinter tout;
    ModuleGroup group;
    auto access = make_smart<FsFileAccess>();
    access->setFileInfo("dasbind_set_library.das", make_unique<TextFileInfo>(src, uint32_t(strlen(src)), false));
    auto program = compileDaScript("dasbind_set_library.das", access, tout, group);
    REQUIRE_FALSE(program->failed());
    Context ctx(program->getContextStackSize());
    REQUIRE(program->simulate(ctx, tout));
    auto fn = ctx.findFunction("run");
    REQUIRE(fn);
    double result = 0.0;
    bool ok = ctx.runWithCatchAndClear([&]() {
        result = cast<double>::to(ctx.eval(fn, nullptr));
    });
    REQUIRE(ok);
    CHECK(result == 4331.5);
}
