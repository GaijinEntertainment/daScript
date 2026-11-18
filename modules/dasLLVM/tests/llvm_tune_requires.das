options gen2

require dastest/testing_boost
require llvm/daslib/llvm_tune

// The per-ISA fallback rail: `requires=` feature expressions and the `;`-chain that lets ONE
// annotation ship a different default per target. Nothing here asserts WHICH features this box
// has — only the grammar and the selection rule, so it holds on every CI lane.

[test(description = "an empty requires= expression passes on every box")]
def test_empty_requires_always_passes(t : T?) {
    t |> success(tune_requires_ok("", tune_native_target()))
}

[test(description = "a requires= naming a feature nothing recognizes never passes")]
def test_unknown_feature_never_passes(t : T?) {
    // fail-closed: a kernel gated on a name nothing recognizes must not be selected
    t |> success(!tune_requires_ok("no_such_feature_xyz", tune_native_target()))
}

[test(description = "a | in requires= passes exactly when either side's feature is present")]
def test_or_alternatives(t : T?) {
    // `|` is an OR — a real feature on either side carries the term
    t |> success(!tune_requires_ok("no_such_feature_xyz|also_bogus", tune_native_target()))
    t |> equal(tune_requires_ok("no_such_feature_xyz|neon", tune_native_target()),
        tune_requires_ok("neon", tune_native_target()))
}

[test(description = "a , in requires= fails the whole expression when any one term fails")]
def test_and_conjunction(t : T?) {
    // `,` is an AND — one failing term fails the whole expression
    t |> success(!tune_requires_ok("neon,no_such_feature_xyz", tune_native_target()))
    t |> success(!tune_requires_ok("no_such_feature_xyz,neon", tune_native_target()))
}

[test(description = "a ! ahead of a requires= name passes exactly when the feature is absent")]
def test_absent_feature(t : T?) {
    let box = tune_native_target()
    t |> success(tune_requires_ok("!no_such_feature_xyz", box))
    t |> equal(tune_requires_ok("!neon", box), !tune_requires_ok("neon", box))
    t |> success(!tune_requires_ok("neon,!neon", box),
        "a feature and its absence never hold together")
    t |> success(tune_requires_ok("neon|!neon", box),
        "one of a feature and its absence always holds")
    t |> success(!tune_requires_ok("!", box), "a ! with no name never passes")
    t |> equal(tune_pick_fallback("gated:!no_such_feature_xyz;generic", box), "gated")
}

[test(description = "a fallback chain with no gating picks its first entry")]
def test_plain_chain_is_the_first_entry(t : T?) {
    // no `:` anywhere means an ordered list with no gating — backward compatible with a bare name
    t |> equal(tune_pick_fallback("vec8_u2", tune_native_target()), "vec8_u2")
    t |> equal(tune_pick_fallback("vec16;vec8_u2", tune_native_target()), "vec16")
}

[test(description = "a fallback chain skips every entry whose requires= cannot pass")]
def test_gated_entry_is_skipped_when_it_cannot_pass(t : T?) {
    t |> equal(tune_pick_fallback("vec16:no_such_feature_xyz;vec8_u2", tune_native_target()),
        "vec8_u2")
    t |> equal(tune_pick_fallback("a:no_such_feature_xyz;b:also_bogus;c", tune_native_target()),
        "c")
}

[test(description = "a fallback chain with no eligible entry, and an empty chain, pick nothing")]
def test_chain_with_nothing_eligible_picks_nothing(t : T?) {
    // "" lets the caller fall through to its own default rather than inventing a perm
    t |> equal(tune_pick_fallback("a:no_such_feature_xyz", tune_native_target()), "")
    t |> equal(tune_pick_fallback("", tune_native_target()), "")
}

[test(description = "a gated fallback entry wins exactly when this box has its feature")]
def test_gated_entry_wins_when_its_feature_is_present(t : T?) {
    // neon is baseline on arm64 and absent on x64, so this asserts the SELECTION rule against
    // whatever the host answers rather than against a fixed architecture
    let expected = tune_requires_ok("neon", tune_native_target()) ? "armrow" : "generic"
    t |> equal(tune_pick_fallback("armrow:neon;generic", tune_native_target()), expected)
}

[test(description = "the earliest eligible entry of a fallback chain wins over every later one")]
def test_earlier_entry_wins_over_later(t : T?) {
    t |> equal(tune_pick_fallback("first;second", tune_native_target()), "first")
    t |> equal(tune_pick_fallback("first:no_such_feature_xyz;second;third", tune_native_target()),
        "second")
}
