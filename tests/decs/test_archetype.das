options gen2
options persistent_heap = true
options gc
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require daslib/decs_boost
require dastest/testing_boost public
require daslib/fio

// === Archetype creation and lookup ===

[test(description = "entities with the same components share one archetype with the right size and component set")]
def test_single_archetype(t : T?) {
    restart()
    for (i in range(5)) {
        create_entity() @(eid, cmp) {
            cmp.pos := float3(i)
        }
    }
    commit()
    t |> run("one archetype") @(t : T?) {
        t |> equal(length(decsState.allArchetypes), 1)
    }
    t |> run("correct size") @(t : T?) {
        t |> equal(decsState.allArchetypes[0].size, 5_l)
    }
    t |> run("has eid component") @(t : T?) {
        t |> success(decsState.allArchetypes[0] |> has("eid"))
    }
    t |> run("has pos component") @(t : T?) {
        t |> success(decsState.allArchetypes[0] |> has("pos"))
    }
    t |> run("no vel component") @(t : T?) {
        t |> success(!(decsState.allArchetypes[0] |> has("vel")))
    }
}

[test(description = "entities with different component sets land in distinct archetypes")]
def test_multiple_archetypes(t : T?) {
    restart()
    create_entity() @(eid, cmp) {
        cmp.a := 1
    }
    create_entity() @(eid, cmp) {
        cmp.b := 2.0
    }
    create_entity() @(eid, cmp) {
        cmp.a := 3
        cmp.b := 4.0
    }
    commit()
    t |> run("three distinct archetypes") @(t : T?) {
        t |> equal(length(decsState.allArchetypes), 3)
    }
}

[test(description = "two entities with the same components land in the same archetype")]
def test_same_components_same_archetype(t : T?) {
    restart()
    create_entity() @(eid, cmp) {
        cmp.x := 1
        cmp.y := 2
    }
    create_entity() @(eid, cmp) {
        cmp.x := 10
        cmp.y := 20
    }
    commit()
    t |> run("same archetype") @(t : T?) {
        t |> equal(length(decsState.allArchetypes), 1)
    }
    t |> run("two entities in it") @(t : T?) {
        t |> equal(decsState.allArchetypes[0].size, 2_l)
    }
}

[test(description = "the archetype lookup table has an entry for each created archetype")]
def test_archetype_lookup(t : T?) {
    restart()
    create_entity() @(eid, cmp) {
        cmp.hp := 100
    }
    create_entity() @(eid, cmp) {
        cmp.hp := 50
        cmp.armor := 10
    }
    commit()
    t |> run("lookup table has entries") @(t : T?) {
        var count = 0
        for (_k, _v in keys(decsState.archetypeLookup), values(decsState.archetypeLookup)) {
            count++
        }
        t |> equal(count, 2)
    }
}

// === Entity removal shrinks archetype ===

[test(description = "deleting entities shrinks their archetype's size")]
def test_archetype_shrinks_on_delete(t : T?) {
    restart()
    var eids : array<EntityId>
    eids |> reserve(4)
    for (i in range(4)) {
        let eid = create_entity() @(eid, cmp) {
            cmp.val := i
        }
        eids |> push(eid)
    }
    commit()
    t |> equal(decsState.allArchetypes[0].size, 4_l)
    delete_entity(eids[1])
    delete_entity(eids[2])
    commit()
    t |> equal(decsState.allArchetypes[0].size, 2_l)
    // remaining entities still have correct data
    var vals : array<int>
    query() $(val : int) {
        vals |> push(val)
    }
    sort(vals)
    t |> equal(length(vals), 2)
    t |> equal(vals[0], 0)
    t |> equal(vals[1], 3)
}

// === Archetype changes on update ===

[test(description = "updating an entity's components moves it to another archetype and empties the old one")]
def test_update_entity_changes_archetype(t : T?) {
    restart()
    let eid = create_entity() @(eid, cmp) {
        cmp.x := 1
    }
    commit()
    t |> run("initially 1 archetype") @(t : T?) {
        t |> equal(length(decsState.allArchetypes), 1)
    }
    update_entity(eid) @(eid, cmp) {
        cmp.y := 2
    }
    commit()
    t |> run("now 2 archetypes") @(t : T?) {
        t |> equal(length(decsState.allArchetypes), 2)
    }
    t |> run("old archetype is empty") @(t : T?) {
        // one of them has size 0
        var emptyCount = 0
        for (arch in decsState.allArchetypes) {
            if (arch.size == 0_l) {
                emptyCount++
            }
        }
        t |> equal(emptyCount, 1)
    }
    // entity is queryable in new archetype
    var found = false
    query(eid) $(x : int; y : int) {
        t |> equal(x, 1)
        t |> equal(y, 2)
        found = true
    }
    t |> success(found)
}

// === Empty world queries ===

[test(description = "a query over an empty world visits nothing")]
def test_query_empty_world(t : T?) {
    restart()
    var count = 0
    query() $(pos : float3) {
        count++
    }
    t |> equal(count, 0)
}

[test(description = "find_query over an empty world finds nothing")]
def test_find_query_empty_world(t : T?) {
    restart()
    let found = find_query() $(pos : float3) {
        return true
    }
    t |> equal(found, false)
}

// === Entity lookup table ===

[test(description = "the entity lookup table is sized for the entities and carries their generation")]
def test_entity_lookup(t : T?) {
    restart()
    let eid = create_entity() @(eid, cmp) {
        cmp.tag := 42
    }
    commit()
    t |> run("lookup table sized correctly") @(t : T?) {
        t |> success(int(eid.id) < length(decsState.entityLookup))
    }
    t |> run("generation matches") @(t : T?) {
        t |> equal(decsState.entityLookup[eid.id].generation, eid.generation)
    }
}

// Phase 8c type-contract: Archetype.size is int64,
// long_entity_count() returns int64. Before PR-C: Archetype.size :
// int silently wraps past INT_MAX entities (per the original audit).
// Asserting the field types at test time means a future revert flips
// this test red.
//
// Uses runtime `equal` rather than `static_assert` because dasfmt
// inserts a space between `default<X>` and `.field` that the typer
// rejects as non-constexpr in the static_assert path. typeinfo
// strings are still compile-time-evaluated, so the runtime check is
// effectively a const compare.

[test(description = "an archetype's size is an int64")]
def test_archetype_size_is_int64(t : T?) {
    var _a : Archetype                                  // referenced only by typeinfo below
    t |> equal(typeinfo stripped_typename(_a.size), "int64")
}

[test(description = "long_entity_count returns an int64 count of zero in an empty world")]
def test_long_entity_count_returns_int64(t : T?) {
    // PR-C surface contract: `entity_count() : int` (panics if > INT_MAX)
    // and the int64-safe accessor is `long_entity_count() : int64`.
    // Mirrors the `length()` / `long_length()` pattern.
    restart()
    let n = long_entity_count()
    t |> equal(typeinfo stripped_typename(n), "int64")
    t |> equal(n, 0_l)
}

[test(description = "entity_count returns an int count of zero in an empty world")]
def test_entity_count_keeps_int_return(t : T?) {
    restart()
    let n = entity_count()
    t |> equal(typeinfo stripped_typename(n), "int")
    t |> equal(n, 0)
}

// Phase 8c memory-gated probe: push component byte storage (c.data)
// past INT_MAX bytes to exercise the long_length / int64 resize
// cascade widened in part 3 of this branch. Before PR-C:
// `length(c.data) + c.stride * count` in create_entities (line 986)
// panicked at > INT_MAX c.data bytes per the long-array surface
// contract. With PR-C, the resize routes through int64.
//
// One [decs_template] field of type float4[4] = 64 bytes stride (the
// max per-component payload, bounded by ComponentValue.data :
// float4[4]). 35 M entities * 64 = 2.24 GB c.data > INT_MAX (2.147
// GB).
//
// Entity count stays well under INT_MAX so we isolate the
// BYTE-storage overflow (the entity-count overflow probe would
// require 2 G+ entities for a memory-prohibitive 40+ GB).
//
// Memory: ~3 GB (2.24 GB c.data + 700 MB entityLookup at 35M
// entries). Gated on DASLANG_HUGE_HEAP_TESTS=1:
// $env:DASLANG_HUGE_HEAP_TESTS = "1" bin/Release/daslang.exe
// dastest/dastest.das -- --test tests/decs/test_archetype.das

def huge_enabled() : bool {
    static_if (typeinfo sizeof(type<int?>) < 8) {
        return false
    }
    return has_env_variable("DASLANG_HUGE_HEAP_TESTS") && get_env_variable("DASLANG_HUGE_HEAP_TESTS") == "1"
}

// 64-byte component payload — the maximum per ComponentValue.data :
// float4[4].
[decs_template]
struct HugePayload {
    payload : float4[4]
}

let HUGE_N = 35_000_000        // 35 M entities (fits in int comfortably)
// At sizeof(HugePayload.payload) = 64 bytes, c.data = 2.24 GB >
// INT_MAX.

[test(description = "a huge entity batch grows component storage past the int range through the int64 resize path")]
def test_long_length_resize_cascade(t : T?) {
    if (!huge_enabled()) return
    restart()

    create_entities(HUGE_N) $(_eid : EntityId; _i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, default<HugePayload>)
    }

    // long_entity_count fits in int here, but the field is int64.
    t |> equal(long_entity_count(), int64(HUGE_N))
    t |> equal(decsState.allArchetypes[0].size, int64(HUGE_N))

    // The point of the probe: at 2.24 GB c.data the WIDENED resize
    // cascade in create_entities ran without hitting length()'s INT_MAX
    // panic. If part 3's widening regressed, create_entities above would
    // have panicked before we got here.

    restart()
}
