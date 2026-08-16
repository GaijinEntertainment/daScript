// Frame-position stamps must live on the sim-side LineInfo copies only: the AST outlives
// simulate, so a stamp left on Expression::at makes every at-keyed consumer - macro
// modules, lint, LSP - see 2^31-line functions.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/simulate/debug_info.h"

using namespace das;

#define SCRIPT_PATH "/tests-cpp/small/test_frame_pos_unstamp.das"

namespace {
    struct StampScan : Visitor {
        uint32_t tagged = 0;
        virtual void preVisitExpression ( Expression * expr ) override {
            Visitor::preVisitExpression(expr);
            if ( expr->at.last_line & LINEINFO_FRAME_POS_TAG ) tagged ++;
        }
    };
}

TEST_CASE("frame-position stamps live on sim nodes, not the AST") {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    auto program = compileDaScript(getDasRoot() + SCRIPT_PATH,
                                   fAccess, tout, dummyLibGroup);
    REQUIRE_FALSE(program->failed());

    Context ctx(program->getContextStackSize());
    REQUIRE(program->simulate(ctx, tout));
    REQUIRE(bool(ctx.gcEnabled));   // `options gc` armed the stamping path

    // frame_position() reads the call site's runtime LineInfo - 0 means the stamp
    // never reached the sim nodes
    auto fn = ctx.findFunction("stamped_position");
    REQUIRE(fn);
    int32_t pos = cast<int32_t>::to(ctx.evalWithCatch(fn, nullptr));
    CHECK_MESSAGE(ctx.getException() == nullptr, "stamped_position panicked");
    CHECK_MESSAGE(pos > 0, "sim-side call site lost its frame-position stamp");

    uint32_t tagged = 0;
    program->thisModule->functions.foreach([&](auto pfun){
        if ( !pfun->body ) return;
        StampScan scan;
        pfun->body->visit(scan);
        tagged += scan.tagged;
    });
    CHECK_MESSAGE(tagged == 0, "AST expressions still stamped after simulate");
}
