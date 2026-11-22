options gen2
require dastest/testing_boost public

require daslib/random
require math

options no_aliasing = false // these tests make sense only with disabled aliasing

var seed : int4

let FULL_TEST = false

struct Foo {
    a : int
    b : float4
}

struct Bar {
    f : Foo
}

struct Pair {
    x : int
    y : int
}

def swap_pair(p : Pair) : Pair => Pair(x = p.y, y = p.x)

// potential alias - cmres vs argument a
def set(a : Foo) {
    var b = Foo(a = 13)
    b.a += a.a
    return b
}

// potential alias - cmres.a vs argument inc
def set2(inc : int&) {
    var b = Foo(a = 13)
    b.a += inc
    return b
}

// no aliasing ever
def set2_ok(inc : int) {
    var b = Foo(a = 13)
    b.a += inc
    return b
}

// potential alias - cmres.b.{xyzw} vs argument incf
def set3(incf : float&) {
    var b = Foo(b = float4(13.))
    b.b += float4(incf)
    return b
}

// potential alias - cmres.a vs argument inc via *inc
def set4(inc : int?) {
    var b = Foo(a = 13)
    b.a += *inc
    return b
}

var g_a, g_b : Foo

var g_p_a = new Foo()
var g_p_b = g_p_a

// potential alias - global variable cmres vs global variable g_a
def set5 {
    var b = Foo(a = 13)
    b.a += g_a.a
    return b
}

def get_a : Foo => g_a

// potential alias - global variable cmres vs global variable g_a via
// get_a function
def set6 {
    var b = Foo(a = 13)
    b.a += get_a().a
    return b
}

// this function ALWAYS aliases its result, becase it reads g_p_a
// pointer
def set7 {
    var b = Foo(a = 13)
    b.a += g_p_a?.a ?? 100500
    return b
}

def pass_ref_a(var a : Foo) : Foo& {
    unsafe {
        return a
    }
}

def set_with_extra(var b : Foo; extra : int) : Foo {
    var a = Foo(a = extra)
    a.a += b.a
    return a
}

[export]
def set_with_extra_which_touches_global_but_no_addr_taken(var b : Foo; extra : int) : Foo {
    b.a += g_a.a + extra
    return b
}

def set_with_2_extra(var b : Foo; e1, e2 : int) : Foo {
    var a = Foo(a = e1 + e2)
    a.a += b.a
    return a
}

[export]
def set_with_2_extra_which_touches_global(var b : Foo; e1, e2 : int) : Foo {
    b.a += g_a.a + e1 + e2
    return b
}

[test(description = "with no_aliasing off, every call, invoke and global-write shape whose result may alias an argument or global still computes the value a non-aliased copy would")]
def test_aliasing(t : T?) {     // nolint:STYLE038 - one flat arm per aliasing scenario
    t |> run("inlined make struct reads its own destination") @(t : T?) {
        var p = Pair(x = 1, y = 2)
        p = swap_pair(p)
        t |> equal(2, p.x)
        t |> equal(1, p.y)
        p <- swap_pair(p)
        t |> equal(1, p.x)
        t |> equal(2, p.y)
    }
    t |> run("straight up aliasing") @(t : T?) {
        var a = Foo(a = 1)
        var b : Foo
        a = set(a)                  // (1) 40211: function set result aliases argument a
        t |> equal(14, a.a)
        a = Foo(a = 1)
        b = set(a)                  // this one ok
        t |> equal(14, b.a)
    }
    t |> run("field aliasing") @(t : T?) {
        var a = Foo(a = 1)
        var b : Foo
        a = set2(a.a)               // (2) 40211: function set result aliases argument inc
        t |> equal(14, a.a)
        a = Foo(a = 1)
        a = set2_ok(a.a)            // this one ok
        t |> equal(14, a.a)
        a = Foo(a = 1)
        b = set2(a.a)               // this one ok
        t |> equal(14, b.a)
    }
    t |> run("swizzle + vector subtype aliasing") @(t : T?) {
        var a = Foo(b = float4(1.))
        var b : Foo
        a = set3(a.b.x)             // (3) 40211: function set result aliases argument incf
        t |> equal(14., a.b.x)
        a = Foo(b = float4(1.))
        b = set3(a.b.y)             // this one ok
        t |> equal(14., b.b.y)
    }
    t |> run("array [] aliasing") @(t : T?) {
        var a, b : Foo[2]
        a[1] = Foo(a = 1)
        a[0] = set(a[1])            // (4) 40211: function set result aliases argument a
                                        // note - false aliasing
        t |> equal(14, a[0].a)
        a[1] = Foo(a = 1)
        b[0] = set(a[1])            // this one ok
        t |> equal(14, b[0].a)
    }
    t |> run("array ?[] aliasing, ?? aliasing") @(t : T?) {
        var a, b : Foo[2]
        var defv = Foo(a = 2)
        a[0] = Foo(a = 1)
        a?[0] ?? defv = set(a[0])   // (5) 40211: function set result aliases argument a
        t |> equal(14, a[0].a)
        t |> equal(2, defv.a)
        a[0] = Foo(a = 1)
        a?[1] ?? defv = set(defv)   // (6) 40211: function set result aliases argument a
        t |> equal(15, a[1].a)
        t |> equal(2, defv.a)
        a[0] = Foo(a = 1)
        b?[0] ?? defv = set(a[0])   // this one ok
        t |> equal(14, b[0].a)
        t |> equal(2, defv.a)
        a[0] = Foo(a = 1)
        b?[3] ?? defv = set(a[0])   // this one ok
        t |> equal(14, defv.a)
    }
    t |> run("cast aliasing") @(t : T?) {
        var a = Foo(a = 1)
        let b = Foo(a = 2)
        // (7) 40211: function set result aliases argument inc
        a = set2(unsafe(reinterpret<int&>(a.b)))
        t |> equal(13, a.a)
        a = Foo(a = 1)
        a = set2(unsafe(reinterpret<int&>(b.b)))  // this one ok
        t |> equal(13, a.a)
    }
    t |> run("op3 aliasing") @(t : T?) {
        var a = Foo(a = 1)
        var b = Foo(a = 2)
        let cond = (random_int(seed) & 1) != 0
        cond ? a : b = set(a)       // (8) 40211: function set result aliases argument a
        t |> equal(14, cond ? a.a : b.a)
        a = Foo(a = 1)
        b = Foo(a = 2)
        a = set(cond ? a : b)       // (9) 40211: function set result aliases argument a
        t |> equal(cond ? 14 : 15, a.a)
        a = Foo(a = 1)
        b = Foo(a = 2)
        a = set(cond ? b : b)       // this one ok
        t |> equal(15, a.a)
    }
    t |> run("ref2ptr aliasing") @(t : T?) {
        var a = Foo(a = 1)
        var b = Foo(a = 2)
        a = set4(unsafe(addr(a.a))) // (10) 40211: function set result aliases argument a
        t |> equal(14, a.a)
        a = Foo(a = 1)
        b = Foo(a = 2)
        b = set4(unsafe(addr(a.a))) // this one ok
        t |> equal(14, b.a)
        a = Foo(a = 1)
        b = Foo(a = 2)
        a = set4(unsafe(addr(b.a))) // this one ok
        t |> equal(15, a.a)
    }
    t |> run("ptr2ref, ref2value aliasing") @(t : T?) {
        var a = Foo(a = 1)
        var pa = unsafe(addr(a))
        var paa = unsafe(addr(a.a))
        var b : Foo
        a = set2(*paa)      // (11) 40211: function set result aliases argument a
        t |> equal(14, a.a)
        a = Foo(a = 1)
        *pa = set2(a.a)     // (12) 40211: function set result aliases argument a
        t |> equal(14, a.a)
        a = Foo(a = 1)
        b = set2(*paa)      // (13) 40211: function set result aliases argument a
                                // note - false aliasing
        t |> equal(14, b.a)
        a = Foo(a = 1)
        set2(*paa)          // this oke ok
        t |> equal(1, a.a)
    }
    t |> run("global variable aliasing") @(t : T?) {
        g_a = Foo(a = 1)
        // (14) 40211: function set5 result aliases global variable ::g_a
        // S<::Foo> through function set5
        g_a = set5()
        t |> equal(14, g_a.a)
        g_a = Foo(a = 1)
        g_b = set5()        // this one ok
        t |> equal(14, g_b.a)
    }
    t |> run("global variable through function aliasing") @(t : T?) {
        g_a = Foo(a = 1)
        // (15) 40211: function set6 result aliases global variable ::g_a
        // S<::Foo> through function get_a
        g_a = set6()
        t |> equal(14, g_a.a)
        g_a = Foo(a = 1)
        g_b = set6()        // this one ok
        t |> equal(14, g_b.a)
    }
    t |> run("pointer argument false aliasing") @(t : T?) {
        var a = new Foo(a = 1)
        var b : Foo
        b = set(*a)         // nolint:STYLE011 - (16) 40211: function set result aliases argument a
                                // note - false aliasing
        t |> equal(14, b.a)
    }
    t |> run("pointer result false aliasing") @(t : T?) {
        let a = Foo(a = 1)
        var b = new Foo()
        *b = set(a)     // (17) 40211: function set result always aliases
                                // note - false aliasing
        t |> equal(14, b.a)
    }
    t |> run("global variable pointer aliasing") @(t : T?) {
        *g_p_a = Foo(a = 1)
        // (18) 40211: function set7 result aliases global variable ::g_p_a
        // 1<S<::Foo>>? through function set7
        g_b = set7()
                                // note - false aliasing
        t |> equal(14, g_b.a)
    }
    t |> run("pass_ref call aliasing") @(t : T?) {
        var a = Foo(a = 1)
        a = set(pass_ref_a(a))  // (19) 40211: function set result aliases argument a
        t |> equal(14, a.a)
        a = Foo(a = 1)
        a = set(move_to_local(a))      // this one ok
        t |> equal(14, a.a)
        a = Foo(a = 1)
        a = pass_ref_a(set(a))      // this one ok
        t |> equal(14, a.a)
    }
    t |> run("make struct aliasing") @(t : T?) {
        var b : Bar
        b.f = Foo(a = 1)
        b = Bar(f = set(b.f))    // (II) 40212: [[ExprMakeStruct ]] aliases
        t |> equal(14, b.f.a)
    }
    t |> run("make struct aliasing") @(t : T?) {
        var b2 : Bar[2]
        b2[0].f = Foo(a = 1)
        b2 = fixed_array(Bar(                      // (III) 40212: [[ExprMakeStruct ]] aliases
            f = Foo(a = 13)), Bar(
            f = Foo(a = b2[0].f.a + 13)
        ))
        t |> equal(13, b2[0].f.a)
        t |> equal(14, b2[1].f.a)
    }
    t |> run("straight up invoke function aliasing") @(t : T?) {
        let fn = @@set
        var a = Foo(a = 1)
        var b : Foo
        a = invoke(fn, a)                 // (20) 40211: function set result aliases argument a
        t |> equal(14, a.a)
        a = Foo(a = 1)
        b = invoke(fn, a)                 // this one ok
        t |> equal(14, b.a)
    }
    t |> run("global function without aliasing") @(t : T?) {
        let fn = @@set_with_extra
        g_a = Foo(a = 1)
        g_a = invoke(fn, Foo(a = 1), 13)              // this one ok
        t |> equal(14, g_a.a)
    }
    t |> run("global function with aliasing") @(t : T?) {
        var fn = @@set_with_2_extra
        // we take address of function (anywhere in the program)
        fn = @@set_with_2_extra_which_touches_global
        fn = @@set_with_2_extra
        g_a = Foo(a = 1)
        // (21) 40211: invoke result potentially aliases global g_a through
        // function set_with_2_extra_which_touches_global S<::Foo> Ci Ci
        g_a = invoke(fn, Foo(a = 1), 6, 7)
        t |> equal(14, g_a.a)
    }
    t |> run("straight up lambda aliasing") @(t : T?) {
        let fn <- @(a : Foo) {
            var b = Foo(a = 13)
            b.a += a.a
            return b
        }
        var a = Foo(a = 1)
        var b : Foo
        a = invoke(fn, a)                 // (22) 40211: function set result aliases argument a
        t |> equal(14, a.a)
        a = Foo(a = 1)
        b = invoke(fn, a)                 // this one ok
        t |> equal(14, b.a)
    }
    t |> run("global lambda without aliasing") @(t : T?) {
        let fn <- @(var b : Foo; extra : int) : Foo {
            var a = Foo(a = extra)
            a.a += b.a
            return a
        }
        g_a = Foo(a = 1)
        g_a = invoke(fn, Foo(a = 1), 13)              // this one ok
        t |> equal(14, g_a.a)
    }
    t |> run("global lambda with aliasing") @(t : T?) {
        var fn <- @(var b : Foo; e1, e2 : int) : Foo {
            b.a += g_a.a + e1 + e2
            return b
        }
        fn <- @(var b : Foo; e1, e2 : int) : Foo {     // nolint:PERF030 - re-pointing fn is the aliasing scenario
            var a = Foo(a = e1 + e2)
            a.a += b.a
            return a
        }
        g_a = Foo(a = 1)
        // (23) 40211: invoke result potentially aliases global g_a through
        // the lambda above
        g_a = invoke(fn, Foo(a = 1), 6, 7)
        t |> equal(14, g_a.a)
    }
}

struct Triple {
    a : int
    b : int
    c : int
}

[never_inline]
def sum_triple(src : Triple) : Triple => Triple(a = 1, b = 2, c = src.a + src.b)

[inline]
def rewrite_first_through_loop(var arr : array<Triple>) {
    for (p in arr) {
        arr[0] = Triple(a = 1, b = 2, c = p.a + p.b)
    }
}

[unsafe_outside_of_for]
def each_triple(var arr : array<Triple>) : iterator<Triple&> => <- unsafe(each(arr))

[test(description = "a value built into an array element that the right side reads through a loop variable over that array, or into the loop variable while the right side reads the array, reads the element as it was before the write")]
def test_loop_variable_aliasing(t : T?) {
    t |> run("literal reads the loop variable") @(t : T?) {
        var arr <- [Triple(a = 10, b = 20, c = 0)]
        for (p in arr) {
            arr[0] = Triple(a = 1, b = 2, c = p.a + p.b)
        }
        t |> equal(30, arr[0].c)
    }
    t |> run("literal is written through the loop variable") @(t : T?) {
        var arr <- [Triple(a = 10, b = 20, c = 0)]
        for (p in arr) {
            p = Triple(a = 1, b = 2, c = arr[0].a + arr[0].b)
        }
        t |> equal(30, arr[0].c)
    }
    t |> run("call reads the loop variable") @(t : T?) {
        var arr <- [Triple(a = 10, b = 20, c = 0)]
        for (p in arr) {
            arr[0] = sum_triple(p)
        }
        t |> equal(30, arr[0].c)
    }
    t |> run("loop over an iterator, literal reads the loop variable") @(t : T?) {
        var arr <- [Triple(a = 10, b = 20, c = 0)]
        for (p in each(arr)) {
            arr[0] = Triple(a = 1, b = 2, c = p.a + p.b)
        }
        t |> equal(30, arr[0].c)
    }
    t |> run("loop over an iterator, literal is written through the loop variable") @(t : T?) {
        var arr <- [Triple(a = 10, b = 20, c = 0)]
        for (p in each(arr)) {
            p = Triple(a = 1, b = 2, c = arr[0].a + arr[0].b)
        }
        t |> equal(30, arr[0].c)
    }
    t |> run("loop variable of a nested loop") @(t : T?) {
        var grid <- [[Triple(a = 10, b = 20, c = 0)]]
        for (row in grid) {
            for (p in row) {
                grid[0][0] = Triple(a = 1, b = 2, c = p.a + p.b)
            }
        }
        t |> equal(30, grid[0][0].c)
    }
    t |> run("loop in a function spliced into its caller") @(t : T?) {
        var arr <- [Triple(a = 10, b = 20, c = 0)]
        rewrite_first_through_loop(arr)
        t |> equal(30, arr[0].c)
    }
    t |> run("iterator from an invoke, literal written through the loop variable") @(t : T?) {
        var arr <- [Triple(a = 10, b = 20, c = 0)]
        let fn = @@each_triple
        for (p in (invoke(fn, arr))) {
            p = Triple(a = 1, b = 2, c = arr[0].a + arr[0].b)
        }
        t |> equal(30, arr[0].c)
    }
}

[test(description = "a value built into a variable that the right side reads through a reference local bound to it reads the variable as it was before the write")]
def test_ref_local_aliasing(t : T?) {
    t |> run("struct literal") @(t : T?) {
        var q = Triple(a = 10, b = 20, c = 0)
        let r & = q
        q = Triple(a = 1, b = 2, c = r.a + r.b)
        t |> equal(30, q.c)
    }
    t |> run("tuple literal") @(t : T?) {
        var tup = (10, 20, 0)
        let r & = tup
        tup = (1, 2, r._0 + r._1)
        t |> equal(30, tup._2)
    }
    t |> run("call") @(t : T?) {
        var q = Triple(a = 10, b = 20, c = 0)
        let r & = q
        q = sum_triple(r)
        t |> equal(30, q.c)
    }
}

var g_triple = Triple(a = 10, b = 20, c = 0)

[never_inline]
def sum_g_triple() : int => g_triple.a + g_triple.b

[never_inline]
def add_ints(x, y : int) : int => x + y

[never_inline]
def call_int_block(blk : block<() : int>) : int => invoke(blk)

[never_inline]
def rewrite_global_from(third : int& = g_triple.a) {
    g_triple = Triple(a = 1, b = 2, c = third)
}

[test(description = "a literal built into a target reads the target as it was before the write wherever its right side reads it - under a value call, an index, a condition, a block handed to a call, or a call that reads the global target")]
def test_literal_reads_its_target(t : T?) {
    t |> run("under a value call") @(t : T?) {
        var q = Triple(a = 10, b = 20, c = 0)
        q = Triple(a = 1, b = 2, c = add_ints(q.a, q.b))
        t |> equal(30, q.c)
    }
    t |> run("under an index") @(t : T?) {
        let lut <- [for (i in range(32)); i * 100]
        var q = Triple(a = 10, b = 20, c = 0)
        q = Triple(a = 1, b = 2, c = lut[q.a])
        t |> equal(1000, q.c)
    }
    t |> run("under a condition") @(t : T?) {
        var q = Triple(a = 10, b = 20, c = 0)
        q = Triple(a = 1, b = 2, c = q.a > 5 ? 111 : 222)
        t |> equal(111, q.c)
    }
    t |> run("under a safe index") @(t : T?) {
        let lut = fixed_array(0, 100, 200)
        var q = Triple(a = 2, b = 20, c = 0)
        q = Triple(a = 1, b = 2, c = lut?[q.a] ?? -1)
        t |> equal(200, q.c)
    }
    t |> run("under an invoke of a value function") @(t : T?) {
        let fn = @@add_ints
        var q = Triple(a = 10, b = 20, c = 0)
        q = Triple(a = 1, b = 2, c = invoke(fn, q.a, q.b))
        t |> equal(30, q.c)
    }
    t |> run("tuple literal under a value call") @(t : T?) {
        var tup = (10, 20, 0)
        tup = (1, 2, add_ints(tup._0, tup._1))
        t |> equal(30, tup._2)
    }
    t |> run("array literal under a value call") @(t : T?) {
        var fa = fixed_array(10, 20, 0)
        fa = fixed_array(1, 2, add_ints(fa[0], fa[1]))
        t |> equal(30, fa[2])
    }
    t |> run("in a block handed to a call") @(t : T?) {
        var q = Triple(a = 10, b = 20, c = 0)
        q = Triple(a = 1, b = 2, c = call_int_block($ => q.a + q.b))
        t |> equal(30, q.c)
    }
    t |> run("in a call that reads the global target") @(t : T?) {
        g_triple = Triple(a = 10, b = 20, c = 0)
        g_triple = Triple(a = 1, b = 2, c = sum_g_triple())
        t |> equal(30, g_triple.c)
    }
    t |> run("in an invoke of a function that reads the global target") @(t : T?) {
        var fn = @@sum_g_triple
        fn = @@sum_g_triple
        g_triple = Triple(a = 10, b = 20, c = 0)
        g_triple = Triple(a = 1, b = 2, c = invoke(fn))
        t |> equal(30, g_triple.c)
    }
}

[test(description = "an assignment whose right side cannot read the half-written target takes no temporary - under no_aliasing none of these is an error")]
def test_no_temporary_without_a_read_of_the_target(t : T?) {
    t |> run("value local copied from the target before the write") @(t : T?) {
        var q = Triple(a = 10, b = 20, c = 0)
        let sum = q.a + q.b
        q = Triple(a = 1, b = 2, c = sum)
        t |> equal(30, q.c)
    }
    t |> run("reference local bound through an index that read the target") @(t : T?) {
        var others <- [Triple(a = 10, b = 20, c = 0)]
        var q = Triple(a = 0, b = 20, c = 0)
        let r & = unsafe(others[q.a])
        q = Triple(a = 1, b = 2, c = r.a + r.b)
        t |> equal(30, q.c)
    }
    t |> run("reference argument whose default names the global target") @(t : T?) {
        g_triple = Triple(a = 10, b = 20, c = 0)
        let third = 30
        rewrite_global_from(third)
        t |> equal(30, g_triple.c)
    }
    t |> run("call argument holding a call that reads the global target") @(t : T?) {
        g_triple = Triple(a = 10, b = 20, c = 0)
        g_triple = sum_triple(Triple(a = sum_g_triple(), b = 5, c = 0))
        t |> equal(35, g_triple.c)
    }
    t |> run("loop source indexed through a dereferenced pointer") @(t : T?) {
        var rows <- [[Triple(a = 10, b = 20, c = 0)]]
        var row = 0
        let prow = unsafe(addr(row))
        for (p in rows[*prow]) {
            p = Triple(a = 1, b = 2, c = 3)
        }
        t |> equal(3, rows[0][0].c)
    }
}

struct IpP {
    a : int
    b : int
}

struct IpW {
    p : IpP
    c : int
}

[safe_when_uninitialized]
struct IpS {
    a : int = ip_s_b()
    b : int = ip_s_a()
}

var ip_g_s : IpS
var ip_g_p = IpP(a = 0, b = 0)
var ip_g_arr : array<IpP>

[never_inline]
def ip_s_a() : int => ip_g_s.a
[never_inline]
def ip_s_b() : int => ip_g_s.b
[never_inline]
def ip_p_a() : int => ip_g_p.a
[never_inline]
def ip_p_b() : int => ip_g_p.b
[never_inline]
def ip_get_a(q : IpP) : int => q.a
[never_inline]
def ip_get_b(q : IpP) : int => q.b

[never_inline]
def ip_init_s(var r : IpS&) {
    r = default<IpS>
}

[never_inline]
def ip_global_calls() {
    ip_g_p = IpP(a = ip_p_b(), b = ip_p_a())
}

[never_inline]
def ip_ref_calls(var r : IpP&) {
    r = IpP(a = ip_p_b(), b = ip_p_a())
}

[never_inline]
def ip_ref_globals(var r : IpP&) {
    r = IpP(a = ip_g_p.b, b = ip_g_p.a)
}

[never_inline]
def ip_ref_self_calls(var r : IpP&) {
    r = IpP(a = ip_get_b(r), b = ip_get_a(r))
}

[never_inline]
def ip_ref_other_ref(var r : IpP&; q : IpP) {
    r = IpP(a = q.b, b = q.a)
}

[never_inline]
def ip_global_ref_arg(q : IpP) {
    ip_g_p = IpP(a = q.b, b = q.a)
}

[never_inline]
def ip_ref_global_array(var r : IpP&) {
    r = IpP(a = ip_g_arr[0].b, b = ip_g_arr[0].a)
}

[never_inline]
def ip_ref_outer_arg(var r : IpP&; w : IpW) {
    r = IpP(a = w.p.b, b = w.p.a)
}

def ip_swapped(t : T?; p : IpP) {
    t |> equal(2, p.a)
    t |> equal(1, p.b)
}

[test(description = "a value built in place reads its destination only before it writes it: a read through a call, a global or another reference makes it build a copy")]
def test_make_in_place_reads(t : T?) {
    t |> run("field initializers read a global destination through calls") @(t : T?) {
        ip_g_s.a = 1
        ip_g_s.b = 2
        ip_g_s = default<IpS>
        t |> equal(2, ip_g_s.a)
        t |> equal(1, ip_g_s.b)
    }
    t |> run("field initializers read a reference destination through calls") @(t : T?) {
        ip_g_s.a = 1
        ip_g_s.b = 2
        ip_init_s(ip_g_s)
        t |> equal(2, ip_g_s.a)
        t |> equal(1, ip_g_s.b)
    }
    t |> run("calls read a global destination") @(t : T?) {
        ip_g_p = IpP(a = 1, b = 2)
        ip_global_calls()
        ip_swapped(t, ip_g_p)
    }
    t |> run("calls read the global a reference is bound to") @(t : T?) {
        ip_g_p = IpP(a = 1, b = 2)
        ip_ref_calls(ip_g_p)
        ip_swapped(t, ip_g_p)
    }
    t |> run("the global a reference is bound to") @(t : T?) {
        ip_g_p = IpP(a = 1, b = 2)
        ip_ref_globals(ip_g_p)
        ip_swapped(t, ip_g_p)
    }
    t |> run("calls take the reference destination") @(t : T?) {
        var x = IpP(a = 1, b = 2)
        ip_ref_self_calls(x)
        ip_swapped(t, x)
    }
    t |> run("calls take the local destination") @(t : T?) {
        var x = IpP(a = 1, b = 2)
        x = IpP(a = ip_get_b(x), b = ip_get_a(x))
        ip_swapped(t, x)
    }
    t |> run("builtin calls read the local destination") @(t : T?) {
        var x = IpP(a = 1, b = 2)
        x = IpP(a = abs(x.b), b = abs(x.a))
        ip_swapped(t, x)
    }
    t |> run("two references to one object") @(t : T?) {
        var x = IpP(a = 1, b = 2)
        ip_ref_other_ref(x, x)
        ip_swapped(t, x)
    }
    t |> run("a reference argument bound to the global destination") @(t : T?) {
        ip_g_p = IpP(a = 1, b = 2)
        ip_global_ref_arg(ip_g_p)
        ip_swapped(t, ip_g_p)
    }
    t |> run("a reference bound to an element of a global array") @(t : T?) {
        ip_g_arr |> clear()
        ip_g_arr |> push(IpP(a = 1, b = 2))
        ip_ref_global_array(ip_g_arr[0])
        ip_swapped(t, ip_g_arr[0])
    }
    t |> run("a reference argument holding the destination") @(t : T?) {
        var w = IpW(p = IpP(a = 1, b = 2), c = 0)
        ip_ref_outer_arg(w.p, w)
        ip_swapped(t, w.p)
    }
}
