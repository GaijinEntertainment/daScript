options gen2
options persistent_heap

require daslib/json_boost
require dastest/testing_boost

// Serialize-path apply benchmark: JV(struct) walks the struct fields via `apply` to build a JsonValue
// map. After dropping the function-escaping `return` from to_JV(struct) (so the block inlines instead of
// falling to PR #3008's per-field invoke fallback), this lane is the regression guard for that win.
// Measured on the rewrite (200k iters, INTERP / JIT ns/op):
//   invoke fallback (return-based) 2083.5 / 1424.5, 2672 B/op, 13 allocs/op
//   inline        (return-free)    1161.5 /  579.5, 1136 B/op, 11 allocs/op  = 1.79x / 2.46x, -2 allocs.
// (to_XML(struct) got the same rewrite; it is build-gated behind DAS_PUGIXML so it is not benched here.)

let N = 200000

enum Color {
    red
    green
    blue
}

struct SerRec {
    a : int
    b : float
    @optional c : string
    @enum_as_int col : Color
    d : int
    e : bool
    f : string
}

[benchmark]
def probe_to_jv_struct(b : B?) {
    let r = SerRec(a = 1, b = 2.5, c = "hi", col = Color.green, d = 7, e = true, f = "world")
    b |> run("to_jv_struct/{N}", N) {
        var acc = 0
        for (_i in range(N)) {
            var jv = JV(r)
            if (jv != null) { acc++ }
            unsafe { delete jv }
        }
        if (acc == 0) { b->failNow() }
    }
}
