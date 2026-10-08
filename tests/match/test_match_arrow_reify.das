options gen2
options rtti
options no_aot

require dastest/testing_boost public
require daslib/match
require daslib/templates_boost
require daslib/ast_boost

variant IntOrStr {
    i : int
    s : string
}

[sideeffects]
def reify_bound(v : IntOrStr) : string {
    return match (v) {
        IntOrStr(i = n) => describe(qmacro($v(n) + 1))
        _ => "string"
    }
}

[test]
def test_bound_name_inside_reification_tag(t : T?) {
    ast_gc_guard() {
        t |> equal(reify_bound(IntOrStr(i = 41)), "(41 + 1)")
        t |> equal(reify_bound(IntOrStr(s = "x")), "string")
    }
}
