#include <doctest/doctest.h>
#include "daScript/daScript.h"

using namespace das;

// The CodeOfPolicies ABI canary: the leading abi_stamp is written by the constructing binary's
// headers and checked by libDaScript at the compile entries. The fatal wrapper exits the
// process, so the tests cover the predicate; the wrapper is a 3-line print-and-exit over it.
static_assert(sizeof(CodeOfPolicies) < 65536, "sizeof would overflow into the version byte of the ABI stamp");

TEST_CASE("CodeOfPolicies ABI canary") {
    CodeOfPolicies good;
    CHECK(checkCodeOfPoliciesStamp(good));
    CHECK_EQ(good.abi_stamp, CodeOfPolicies::expected_abi_stamp());
    // pin the encoding, not just self-consistency: version in the top byte, sizeof in the
    // middle 16, low byte 0 (the format the mismatch messages print)
    CHECK_EQ(good.abi_stamp >> 24, uint32_t(DAS_POLICIES_VERSION));
    CHECK_EQ((good.abi_stamp >> 8) & 0xffffu, uint32_t(sizeof(CodeOfPolicies)));
    CHECK_EQ(good.abi_stamp & 0xffu, 0u);

    CodeOfPolicies bad;
    bad.abi_stamp = 0;  // what a pre-canary host's leading bools look like at offset 0
    CHECK_FALSE(checkCodeOfPoliciesStamp(bad));
    bad.abi_stamp = good.abi_stamp ^ (1u << 24);  // DAS_POLICIES_VERSION drift
    CHECK_FALSE(checkCodeOfPoliciesStamp(bad));
    bad.abi_stamp = good.abi_stamp ^ (4u << 8);   // sizeof drift (one field added/removed)
    CHECK_FALSE(checkCodeOfPoliciesStamp(bad));
}
