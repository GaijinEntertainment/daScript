options gen2
options auto_inline_functions = true

require dastest/testing_boost public
require daslib/math_boost

enum Color {
    red
    green
}

struct Defaults {
    a : int = 7
    b : int = 11
}

struct Built {
    v : int
}

def Built(n : int) => Built(v = n * 2)

[inline]
def swizzle_sum(v : float3) : float {
    var r = v.x
    if (v.z > 100.0) {
        return 0.0
    }
    r += v.yz.x * 10.0
    return r
}

[inline]
def built_value(n : int) : int {
    var p = new Built(n)
    let r = p.v
    unsafe {
        delete p
    }
    return r
}

[inline]
def defaults_sum() : int {
    let d = Defaults()
    return d.a + d.b
}

def take_witness(v : auto(VT)) : int => typeinfo sizeof(v) + (v == default<VT> ? 100 : 0)

[inline]
def pass_witness(t : auto(TT)) : int => take_witness(t)

[never_inline]
def other_array() : array<int> => <- [7, 8, 9]

[inline]
def made_array(n : int) : array<int> {
    var a <- [for (i in range(n)); i]
    if (n > 100) {
        return <- a
    }
    return <- a
}

[inline]
def falsy(x : int) : bool {
    var r = false
    if (x > 100) {
        r = true
        return r
    }
    return r
}

[inline]
def loop_sum(a : array<int>) : int {
    var s = 0
    for (x in a) {
        s += x
    }
    if (s > 100000) {
        return 0
    }
    return s
}

var g_bumped = 0

[inline]
def bump(a : int) {
    var r = a
    if (a > 100) {
        return
    }
    r += a
    g_bumped += r
}

def bump_in_labeled_block() : int {
    g_bumped = 0
    var i = 0
    label 0:
    bump(i)
    i ++
    if (i < 3) {
        goto label 0
    }
    return g_bumped
}

[test]
def test_cloned_body_keeps_inferred_state(t : T?) {
    t |> run("a swizzle in a spliced body reads the fields it named") @(t : T?) {
        t |> equal(swizzle_sum(float3(1.0, 2.0, 3.0)), 21.0)
    }
    t |> run("new with a constructor in a spliced body calls the constructor") @(t : T?) {
        t |> equal(built_value(5), 10)
    }
    t |> run("a struct literal in a spliced body runs its field initializers") @(t : T?) {
        t |> equal(defaults_sum(), 18)
    }
    t |> run("a type witness passed on by value becomes the type's zero") @(t : T?) {
        t |> equal(pass_witness(type<int>), 104)
        t |> equal(pass_witness(type<Color>), 104)
        t |> equal(pass_witness(type<float>), 104)
    }
}

[test]
def test_splice_lowering(t : T?) {
    t |> run("a conditional array result moves into its declaration") @(t : T?) {
        let c = !empty(other_array())
        let arr <- c ? made_array(4) : other_array()
        t |> equal(length(arr), 4)
        t |> equal(arr[3], 3)
        let other <- !c ? made_array(4) : other_array()
        t |> equal(other[0], 7)
    }
    t |> run("&&= with an [inline] right side stores the result") @(t : T?) {
        var b = true
        b &&= falsy(1)
        t |> equal(b, false)
        var skipped = false
        skipped &&= falsy(200)
        t |> equal(skipped, false)
    }
    t |> run("a loop in a spliced body iterates its own source") @(t : T?) {
        var a <- [1, 2, 3, 4]
        t |> equal(loop_sum(a), 10)
    }
    t |> run("a compiled callee from another module splices with its reads unfolded") @(t : T?) {
        let x = 0.25
        t |> success(abs(linear_to_SRGB(x) - (1.055 * pow(x, 1.0 / 2.4) - 0.055)) < 0.001)
    }
}

[test]
def test_splice_in_labeled_block(t : T?) {
    t |> equal(bump_in_labeled_block(), 6)
}
