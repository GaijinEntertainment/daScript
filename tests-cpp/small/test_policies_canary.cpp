#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/daScriptC.h"

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

TEST_CASE("the loop unroll and forward policies reach CodeOfPolicies through the C API") {
    das_policies * pol = das_policies_make();
    auto policies = (CodeOfPolicies *) pol;
    CHECK_EQ(policies->loop_unroll_cost, 8);
    REQUIRE(das_policies_set_int(pol, DAS_POLICY_LOOP_UNROLL_COST, 0) == 1);
    CHECK_EQ(policies->loop_unroll_cost, 0);
    CHECK_FALSE(policies->disable_forward);
    REQUIRE(das_policies_set_bool(pol, DAS_POLICY_DISABLE_FORWARD, 1) == 1);
    CHECK(policies->disable_forward);
    das_policies_release(pol);
}
