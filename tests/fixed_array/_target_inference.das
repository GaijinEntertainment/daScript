// TARGET SPEC for the tFixedArray rework (FIXED_ARRAY_REWORK.md) — Part B.
// Leading underscore: dastest SKIPS this file. Stage 1 renames it to
// test_target_inference.das when the new inference semantics land. Until then this
// file is the reviewed acceptance criteria; it does NOT compile/pass on master.
//
// New semantics (agreed, no compat shim):
//   auto(TT)   <- int[4]       =>  TT = int[4]      (whole array, like array<int>)
//   auto(TT)[] <- float[4][4]  =>  TT = float[4]    (peel one level)
//   auto(TT)[] <- M4[10]       =>  TT = M4          (alias preserved)
//   default<TT> after FA binding => zeroed FA
// Exact const decoration in the TT strings ("int[4]" vs "int const[4]") to be
// settled at the Stage 1 review — adjust the expected strings then.
//
// The `expect` line below documents how master FAILS this spec today (and keeps
// lint/CI off the file until then; dastest already skips it via the `_` prefix):
//   30192 — auto(TT) <- int[4] binds TT=int, so `for (x in default<TT>)` iterates an int
//   30341 x2 — auto(TT)[] can't take float[4][4]; get_key(table<K;V[N]>) can't resolve
// Stage 1: delete the `expect` line and rename the file to test_target_inference.das.
options gen2
expect 30192, 30341:2
require dastest/testing_boost public

typedef M4 = float[4][4]

def plain_name(a : auto(TT)) : string {
    return typeinfo typename(type<TT>)
}

def dim_name(a : auto(TT)[]) : string {
    return typeinfo typename(type<TT>)
}

def zeroed_count(a : auto(TT)) : int {
    var z = default<TT>
    var n = 0
    for (x in z) {
        n ++
    }
    return n
}

def pick(a : auto(TT)) : string {
    return "plain"
}

def pick(a : auto(TT)[]) : string {
    return "dim"
}

[test]
def test_whole_array_binding(t : T?) {
    t |> run("auto(TT) binds the whole fixed array") @(t : T?) {
        var i4 : int[4]
        t |> equal(plain_name(i4), "int const[4]")
    }
    t |> run("default<TT> is a zeroed FA") @(t : T?) {
        var i4 : int[4]
        t |> equal(zeroed_count(i4), 4)
    }
}

[test]
def test_peel_one_level(t : T?) {
    t |> run("auto(TT)[] peels one level off multi-dim") @(t : T?) {
        var m : float[4][4]
        t |> equal(dim_name(m), "float const[4]")
    }
    t |> run("multi-dim now prefers the dim overload") @(t : T?) {
        // characterized on master as "plain" (test_generics_current.das documents
        // the old behavior); the rework makes auto[] match and win
        var m : float[2][3]
        t |> equal(pick(m), "dim")
    }
    t |> run("1D keeps preferring the dim overload") @(t : T?) {
        var i4 : int[4]
        t |> equal(pick(i4), "dim")
    }
}

// NOTE: unlike the rest of this file (enabled at Stage 1), this subtest is gated on
// STAGE 4 — the broken builtin.das:1407 overload is deleted (not fixed) there, and only
// then does the base get_key generic cover FA values. If Stage 1 enablement trips on it,
// split it out and keep it disabled until Stage 4.
[test]
def test_get_key_fixed(t : T?) {
    t |> run("get_key on table<K;V[N]> resolves") @(t : T?) {
        // broken on master: builtin.das:1407 fails to resolve valT const[-2]
        var tab : table<string; int[4]>
        var v : int[4]
        for (i in range(4)) {
            v[i] = i + 1
        }
        tab |> insert("a", v)
        t |> equal(get_key(tab, v), "a")
        delete tab
    }
}
