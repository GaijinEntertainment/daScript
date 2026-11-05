options gen2
options gc_infer_collect_nodes = 0
options gc_infer_collect_pct = 0

require dastest/testing_boost public
require _overload_cache_gc_sweep_macro

struct Base {
    a : int
}

struct Other {
    c : int
}

var g_hit = 0

def operator =(var _a : void?&; _b : Base?) {
    g_hit ++
}

def probe_site(var t : void?&) {
    pass
}

[test]
def test_overload_cache_survives_gc_sweep(t : T?) {
    var p : void?
    probe_site(p)
    t |> equal(1, g_hit)
}
