// Annotation info is deep-copied into the context debug heap (AnnotationInfo /
// AnnotationArgumentInfo). The Context must be able to reflect struct / field
// annotations after the Program is gone, with no `options rtti` involved —
// the old representation pointed straight into the AST and required keeping
// the ProgramPtr alive for the Context's whole lifetime.

#include <doctest/doctest.h>

#include "daScript/daScript.h"

using namespace das;

#define SCRIPT_PATH "/tests-cpp/small/test_annotation_lifetime.das"

TEST_CASE("annotation reflection survives Program release") {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    auto program = compileDaScript(getDasRoot() + SCRIPT_PATH,
                                   fAccess, tout, dummyLibGroup);
    REQUIRE(program);
    REQUIRE_FALSE(program->failed());

    Context ctx(program->getContextStackSize());
    REQUIRE(program->simulate(ctx, tout));
    // no `options rtti` in the fixture — simulate drops thisProgram
    REQUIRE(ctx.thisProgram == nullptr);

    // release the Program before touching reflection — debug info is self-contained
    program.reset();

    auto fnCheck = ctx.findFunction("check");
    REQUIRE(fnCheck);
    bool ok = false;
    bool ranClean = ctx.runWithCatchAndClear([&](){
        vec4f r = ctx.callOrFastcall(fnCheck, nullptr, nullptr);
        ok = cast<bool>::to(r);
    });
    CHECK_MESSAGE(ranClean, "check() panicked after program release");
    CHECK_MESSAGE(ok, "annotation data missing or wrong after program release");
}
