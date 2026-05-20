options gen2
options persistent_heap = true
options gc

require daslib/decs_boost
require dastest/testing_boost public

// Phase 8c type-contract: Archetype.size is int64, long_entity_count() returns int64.
// Before PR-C: Archetype.size : int silently wraps past INT_MAX entities (per
// the original audit). Locking the field type with static_assert means a
// future revert flips this test red.
//
// Uses `stripped_typename` to stay independent of PR #2764 (typeinfo is_int64)
// landing first.

[test]
def test_archetype_size_is_int64(t : T?) {
    static_assert(typeinfo stripped_typename(default<Archetype> .size) == "int64",
                  "Archetype.size must be int64 (decs storage cap)")
    t |> equal(0, 0)
}

[test]
def test_long_entity_count_returns_int64(t : T?) {
    // PR-C surface contract: `entity_count() : int` (panics if > INT_MAX) and
    // the int64-safe accessor is `long_entity_count() : int64`. Mirrors the
    // `length()` / `long_length()` pattern.
    let n = long_entity_count()
    static_assert(typeinfo stripped_typename(n) == "int64",
                  "long_entity_count() must return int64")
    t |> equal(n, 0_l)
}

[test]
def test_entity_count_keeps_int_return(t : T?) {
    let n = entity_count()
    static_assert(typeinfo stripped_typename(n) == "int",
                  "entity_count() keeps int return (panics past INT_MAX)")
    t |> equal(n, 0)
}
