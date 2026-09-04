options gen2
require UnitTest
require dastest/testing_boost public

var g_tab : table<EntityId; int>

// One table reached by both tiers in one program: [no_aot] keeps the write interpreted inside
// the AOT binary, while the read is AOT. The index is written directly here rather than through
// a daslib generic, so the hashing happens in this function and not in an AOT'd callee. A value
// type is hashed and compared by its workhorse in every tier, so the read finds what the write
// put there; hashing the C++ object in AOT instead puts the key in another bucket.
[no_aot, no_jit]
def write_interpreted {
    g_tab[EntityId(7)] = 42
}

def read_aot : int {
    return g_tab ?[EntityId(7)] ?? -1
}

[test]
def test_cross_tier_table_hash(t : T?) {
    write_interpreted()
    t |> equal(length(g_tab), 1)
    t |> equal(read_aot(), 42)
}
