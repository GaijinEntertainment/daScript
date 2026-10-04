#include <doctest/doctest.h>
#include "daScript/daScript.h"

using namespace das;

TEST_CASE("explicit string release cancels deferred temporary disposal") {
    Context ctx(16 * 1024, true);
    CodeOfPolicies policies;
    policies.persistent_heap = true;
    ctx.setup(0, 0, policies, AnnotationArgumentList());
    auto first = ctx.allocateString("queued allocation", 17, nullptr);
    ctx.freeTempString(first, nullptr);
    REQUIRE(ctx.stringDisposeQue == first);
    auto unrelated = ctx.allocateString("unrelated", 9, nullptr);
    REQUIRE(ctx.freeString(unrelated, 9, nullptr));
    CHECK(ctx.stringDisposeQue == first);
    REQUIRE(ctx.freeString(first, 17, nullptr));
    REQUIRE(ctx.stringDisposeQue == nullptr);
    auto second = ctx.allocateString("second allocation", 17, nullptr);
    ctx.freeTempString(second, nullptr);
    CHECK(strcmp(second, "second allocation") == 0);
}
