#include <doctest/doctest.h>
#include "daScript/daScript.h"

using namespace das;

// The CodeOfPolicies ABI canary: the leading abi_stamp is written by the constructing binary's
// headers and checked by libDaScript at the compile entries. The fatal wrapper exits the
// process, so the tests cover the predicate; the wrapper is a 3-line print-and-exit over it.
TEST_CASE("CodeOfPolicies ABI canary") {
    CodeOfPolicies good;
    CHECK(checkCodeOfPoliciesStamp(good));
    CHECK_EQ(good.abi_stamp, CodeOfPolicies::expected_abi_stamp());
    // the stamp's low byte is 0 by construction, so a pre-canary libDaScript reading this
    // word as its leading bools still sees aot == false
    CHECK_EQ(good.abi_stamp & 0xffu, 0u);

    CodeOfPolicies bad;
    bad.abi_stamp = 0;  // what a pre-canary host's leading bools look like at offset 0
    CHECK_FALSE(checkCodeOfPoliciesStamp(bad));
    bad.abi_stamp = good.abi_stamp ^ (1u << 24);  // DAS_POLICIES_VERSION drift
    CHECK_FALSE(checkCodeOfPoliciesStamp(bad));
    bad.abi_stamp = good.abi_stamp ^ (4u << 8);   // sizeof drift (one field added/removed)
    CHECK_FALSE(checkCodeOfPoliciesStamp(bad));
}
