options gen2
options persistent_heap = true   // entityLookup ~ 44 GB at 2.2 G entities
options gc

require daslib/decs_boost
require daslib/fio
require dastest/testing_boost public

// Phase 8c memory-gated probe: actually push the entity count past INT_MAX
// and verify the int64 surface holds. Before PR-C: Archetype.size : int
// silently wraps + entityLookup resize cascade panics, so this scenario was
// unreachable. After PR-C: storage is int64-safe; the cascade widening
// (commit 2) allows entityLookup growth up to UINT_MAX entries.
//
// EntityId.id widening (uint -> uint64) is a separate architectural followup,
// so the ceiling here is UINT_MAX entities, not UINT64_MAX. That's still
// enough to push past INT_MAX (~2.14 G), which is the whole point.
//
// Memory: ~46 GB at 2.2 G entities (20-byte entityLookup entries + 1-byte
// payload per entity). CI runners would OOM, so gated on
// DASLANG_HUGE_HEAP_TESTS=1. Run manually on a 64 GB+ workstation:
//   $env:DASLANG_HUGE_HEAP_TESTS = "1"
//   bin/Release/daslang.exe dastest/dastest.das -- --test tests/decs/test_huge_entity_count_int64.das

def huge_enabled() : bool {
    static_if (typeinfo sizeof(type<int?>) < 8) {
        return false
    }
    return has_env_variable("DASLANG_HUGE_HEAP_TESTS") && get_env_variable("DASLANG_HUGE_HEAP_TESTS") == "1"
}

[decs_template]
struct HugePayload {
    payload : uint8
}

// Two chunks summing past INT_MAX. INT_MAX = 2,147,483,647 — each chunk fits
// in int (the create_entities count param) without overflow, and together
// they cross the boundary so arch.size goes past INT_MAX.
let CHUNK_A = 1_100_000_000     // 1.1 G entities (fits in int)
let CHUNK_B = 1_100_000_000     // 1.1 G entities (fits in int)
// Total: 2.2 G entities — past INT_MAX (2.147 G), under UINT_MAX (4.29 G).

[test]
def test_long_entity_count_past_int_max(t : T?) {
    if (!huge_enabled()) return
    restart()

    create_entities(CHUNK_A) $(eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, HugePayload(payload = uint8(i & int(0xFF))))
    }
    // The first chunk fits in int, so entity_count() (the int-returning
    // surface) still works here.
    t |> equal(long_entity_count(), int64(CHUNK_A))

    create_entities(CHUNK_B) $(eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, HugePayload(payload = uint8(i & int(0xFF))))
    }

    // After the second chunk: total entities > INT_MAX. long_entity_count()
    // returns the int64-safe total; arch.size carries it without wrap.
    let total = int64(CHUNK_A) + int64(CHUNK_B)
    t |> equal(long_entity_count(), total)
    t |> equal(decsState.allArchetypes[0].size, total)

    // entity_count() (int return) would panic here per the surface contract
    // (mirrors length() / long_length()). We can't assert that with the
    // dastest harness (try/recover is banned for panic UX testing per the
    // soft-fail rule), so the panic side is documented in the source rather
    // than runtime-verified.

    restart()
}
