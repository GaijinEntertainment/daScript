#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/simulate/aot.h"

using namespace das;

TEST_CASE("an AOT table move carries tombstones, so a moved table never rehashes on a count it never had") {
    TTable<int32_t,int32_t> a; das_zero(a);
    a.tombstones = 7;
    TTable<int32_t,int32_t> b; das_zero(b);
    b = a;
    CHECK(b.tombstones == 7u);
    CHECK(a.tombstones == 0u);
    TTable<int32_t,void> sa; das_zero(sa);
    sa.tombstones = 5;
    TTable<int32_t,void> sb; das_zero(sb);
    sb = sa;
    CHECK(sb.tombstones == 5u);
    CHECK(sa.tombstones == 0u);
}
