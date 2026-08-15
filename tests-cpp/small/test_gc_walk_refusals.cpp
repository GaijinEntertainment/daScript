// the heap_collect refusal halves only a C++ host can exercise:
//   - a das frame entered with at = nullptr makes the frame above it unattributable: refuse;
//   - a null line at the stack ROOT stays legal (the macro-context collect pattern).

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/aot.h"

#include <cstring>

using namespace das;

namespace {
    // re-enters the context on a das function pointer with NO line info - the shape a
    // C++ embedder produces with das_invoke_function<...>::invoke(ctx, nullptr, fn)
    void reenter_null_line ( TFunc<void> fn, Context * context ) {
        das_invoke_function<void>::invoke(context, nullptr, fn);
    }
}

class Module_GcWalkTest : public Module {
public:
    Module_GcWalkTest() : Module("gc_walk_test") {
        ModuleLibrary lib(this);
        lib.addBuiltInModule();
        addExtern<DAS_BIND_FUN(reenter_null_line)>(*this, lib, "reenter_null_line",
            SideEffects::invoke, "reenter_null_line")
                ->args({"fn","context"});
    }
};
REGISTER_MODULE(Module_GcWalkTest);

#define SCRIPT_PATH "/tests-cpp/small/test_gc_walk_refusals.das"

TEST_CASE("heap_collect refuses frames it can't attribute") {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    auto program = compileDaScript(getDasRoot() + SCRIPT_PATH,
                                   fAccess, tout, dummyLibGroup);
    if ( program->failed() ) {
        TextWriter tw;
        for ( auto & err : program->errors ) {
            tw << reportError(err.at, err.what, err.extra, err.fixme, err.cerr);
        }
        string serr = tw.str();
        MESSAGE(serr);
    }
    REQUIRE_FALSE(program->failed());

    Context ctx(program->getContextStackSize());
    REQUIRE(program->simulate(ctx, tout));

    SUBCASE("collect below a null-line re-entry refuses") {
        auto fn = ctx.findFunction("outer_reenter");
        REQUIRE(fn);
        bool refused = false;
        bool ok = ctx.runWithCatchAndClear([&](){
            vec4f r = ctx.callOrFastcall(fn, nullptr, nullptr);
            refused = cast<bool>::to(r);
        });
        CHECK_MESSAGE(ok, "the refusal must be recoverable inside das, not escape the call");
        CHECK_MESSAGE(refused, "collect under a null-line re-entry must refuse");
    }

    SUBCASE("collect with a null line at the stack ROOT stays legal") {
        auto fn = ctx.findFunction("collect_at_root");
        REQUIRE(fn);
        bool collected = false;
        bool ok = ctx.runWithCatchAndClear([&](){
            vec4f r = ctx.callOrFastcall(fn, nullptr, nullptr);
            collected = cast<bool>::to(r);
        });
        CHECK_MESSAGE(ok, "root-level collect must not throw");
        CHECK_MESSAGE(collected, "root-level collect with a null entry line must succeed");
    }
}
