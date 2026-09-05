options gen2
options no_unused_function_arguments = false
require dastest/testing_boost public

// AOT emits structures in by-value dependency order, not source order: Outer embeds Inner,
// which is declared later, and Inner's function-typed field names Outer in its signature. A
// sort that reads the signature as a by-value dependency sees a cycle and falls back to source
// order, where the generated C++ embeds an incomplete type. The AOT build of this file is the
// gate; the assertions only keep the shapes alive. (An `array<Outer>` field inside Inner would
// be a real C++ cycle - TArray<T> takes sizeof(T) - so that shape stays out of the test.)

struct Outer {
    inner : Inner
    tag : int
}

struct Inner {
    on_outer : function<(o : Outer) : int>
    weight : float
}

def outer_tag(o : Outer) : int {
    return o.tag * 2
}

[test]
def test_struct_order(t : T?) {
    t |> run("an outer struct embeds a later-declared inner whose signature names the outer") @(t : T?) {
        var o = Outer(tag = 21, inner = Inner(on_outer = @@outer_tag, weight = 0.5))
        t |> equal(invoke(o.inner.on_outer, o), 42)
        t |> equal(o.inner.weight, 0.5)
    }
}
