#include <doctest/doctest.h>
#include "daScript/daScript.h"

using namespace das;

TEST_CASE("the fastcall depth cap reaches a context through setup, a clone copies it, restart zeroes the counter") {
    Context ctx(16 * 1024, false);
    CodeOfPolicies policies;
    policies.max_fast_call_depth = 7;
    ctx.setup(0, 0, policies, AnnotationArgumentList());
    CHECK(ctx.maxFastCallDepth == 7u);
    ctx.fastCallDepth = 5;
    ctx.restart();
    CHECK(ctx.fastCallDepth == 0u);
    ctx.fastCallDepth = 3;
    Context clone(ctx, 0u);
    CHECK(clone.maxFastCallDepth == 7u);
    CHECK(clone.fastCallDepth == 0u);
}
