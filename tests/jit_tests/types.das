options gen2

require dastest/testing_boost
require UnitTest

[safe_when_uninitialized]
struct Foo {
    a : int = 1234
    b : float = 1234.0
    c : string = "1234"
    d : int = 1234
}

[safe_when_uninitialized]
struct Bar {
    a : int = 13
    foo : Foo
    b : int
}

struct FooBar {
    a : array<int>
    b : int
}

def struct_field(var f : Foo) {
    f.a = 1
    f.b = 2.0
    f.c = "hello"
    return f.d
}

def struct_ptr_field(var f : Foo?) {
    f.a = 11
    f.b = 22.0
    f.c = "hhello"
    return f.d
}

def struct_ctor_call => Foo()

def make_struct_cmres => Foo(uninitialized a = 1, c = "bar")

// // def make_struct_with_block {
//     return Foo(a = 1, c = "bar") $(var f) { f.a = 2; }
// }

def make_struct_cmres_with_init => Foo(a = 1, c = "bar")

def make_struct_heap => new Foo(uninitialized a = 1, c = "bar")

def make_struct_heap_with_init => new Foo(a = 1, c = "bar")

def make_struct_with_substruct_cmres => Bar(a = 1, foo = Foo(uninitialized a = 2, b = 3.), b = 4)

def make_struct_with_substruct_cmres_with_init => Bar(b = 10, foo = Foo(a = 2, b = 3.))

def make_struct_with_substruct_heap => new Bar(a = 1, foo = Foo(uninitialized a = 2, b = 3.), b = 4)

def make_struct_with_substruct_heap_with_init => new Bar(b = 10, foo = Foo(a = 2, b = 3.))

def make_struct_with_field_call_to_cmres => Bar(a = 1, foo = Foo(), b = 4)

def make_struct_on_stack(which : bool) {
    let a = Foo(a = 1, b = 2.0, c = "3", d = 4)
    let b = Foo(a = 2, b = 3.0, c = "4", d = 5)
    return which ? a : b
}

def make_struct_with_substruct_on_stack(which : bool) {
    let a = Bar(a = 1, foo = Foo(uninitialized a = 2, b = 3.), b = 4)
    let b = Bar(a = 2, foo = Foo(uninitialized a = 3, b = 4.), b = 5)
    return which ? a : b
}

def new_struct_on_heap => new Foo

def new_struct_with_ctor_on_heap => new Foo()

def struct_field_move_init {
    var fb : FooBar
    var t <- [for (i in range(10)); i * i]
    fb <- FooBar(a <- t, b = 13)
    assert(empty(t))
    return <- fb
}

[test(description = "JIT-compiled structure fields, constructors, nested substructures and heap and stack creation return the interpreter's results")]
def test_struct(t : T?) {    // nolint:STYLE038 -- flat list of independent JIT arms, one per struct-construction shape
    t |> run("struct field") @(t : T?) {
        var f = Foo(d = 13)
        t |> success(!jit_enabled() || is_jit_function(@@struct_field))
        let res = struct_field(f)
        t |> equal(13, res)
        t |> equal(1, f.a)
        t |> equal(2.0, f.b)
        t |> equal("hello", f.c)
    }
    t |> run("struct ptr field") @(t : T?) {
        var f = Foo(d = 14)
        t |> success(!jit_enabled() || is_jit_function(@@struct_ptr_field))
        let res = struct_ptr_field(unsafe(addr(f)))
        t |> equal(14, res)
        t |> equal(11, f.a)
        t |> equal(22.0, f.b)
        t |> equal("hhello", f.c)
    }
    t |> run("struct ctor call") @(t : T?) {
        let f = struct_ctor_call()
        t |> success(!jit_enabled() || is_jit_function(@@struct_ctor_call))
        t |> equal(1234, f.a)
        t |> equal(1234.0, f.b)
        t |> equal("1234", f.c)
        t |> equal(1234, f.d)
    }
    t |> run("make struct cmres") @(t : T?) {
        let f = make_struct_cmres()
        t |> success(!jit_enabled() || is_jit_function(@@make_struct_cmres))
        t |> equal(1, f.a)
        t |> equal(0.0, f.b)
        t |> equal("bar", f.c)
        t |> equal(0, f.d)
    }
    t |> run("make struct cmres with init") @(t : T?) {
        let f = make_struct_cmres_with_init()
        t |> success(!jit_enabled() || is_jit_function(@@make_struct_cmres_with_init))
        t |> equal(1, f.a)
        t |> equal(1234.0, f.b)
        t |> equal("bar", f.c)
        t |> equal(1234, f.d)
    }
    t |> run("make struct with substruct cmres") @(t : T?) {
        let f = make_struct_with_substruct_cmres()
        t |> success(!jit_enabled() || is_jit_function(@@make_struct_with_substruct_cmres))
        t |> equal(1, f.a)
        t |> equal(2, f.foo.a)
        t |> equal(3., f.foo.b)
        t |> equal("", f.foo.c)
        t |> equal(0, f.foo.d)
        t |> equal(4, f.b)
    }
    t |> run("make struct with substruct cmres with init") @(t : T?) {
        let f = make_struct_with_substruct_cmres_with_init()
        t |> success(!jit_enabled()
                     || is_jit_function(@@make_struct_with_substruct_cmres_with_init))
        t |> equal(13, f.a)
        t |> equal(2, f.foo.a)
        t |> equal(3., f.foo.b)
        t |> equal("1234", f.foo.c)
        t |> equal(1234, f.foo.d)
        t |> equal(10, f.b)
    }
    t |> run("make struct with substruct equal to call to cmres") @(t : T?) {
        let f = make_struct_with_field_call_to_cmres()
        t |> success(!jit_enabled() || is_jit_function(@@make_struct_with_field_call_to_cmres))
        t |> equal(1, f.a)
        t |> equal(1234,  f.foo.a)
        t |> equal(1234., f.foo.b)
        t |> equal("1234", f.foo.c)
        t |> equal(1234,  f.foo.d)
        t |> equal(4, f.b)
    }
    t |> run("make struct on stack") @(t : T?) {
        let a = make_struct_on_stack(true)
        let b = make_struct_on_stack(false)
        t |> success(!jit_enabled() || is_jit_function(@@make_struct_on_stack))
        t |> equal(1,   a.a)
        t |> equal(2.,  a.b)
        t |> equal("3", a.c)
        t |> equal(4,   a.d)
        t |> equal(2,   b.a)
        t |> equal(3.,  b.b)
        t |> equal("4", b.c)
        t |> equal(5,   b.d)
    }
    t |> run("make struct with substruct on stack") @(t : T?) {
        let a = make_struct_with_substruct_on_stack(true)
        let b = make_struct_with_substruct_on_stack(false)
        t |> success(!jit_enabled() || is_jit_function(@@make_struct_with_substruct_on_stack))
        t |> equal(1,  a.a)
        t |> equal(2,  a.foo.a)
        t |> equal(3., a.foo.b)
        t |> equal("", a.foo.c)
        t |> equal(0,  a.foo.d)
        t |> equal(4,  a.b)
        t |> equal(2,  b.a)
        t |> equal(3,  b.foo.a)
        t |> equal(4., b.foo.b)
        t |> equal("", b.foo.c)
        t |> equal(0,  b.foo.d)
        t |> equal(5,  b.b)
    }
    t |> run("new struct on heap") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@new_struct_on_heap))
        var f = new_struct_on_heap()
        t |> equal(0, f.a)
        t |> equal(0., f.b)
        t |> equal("", f.c)
        t |> equal(0, f.d)
    }
    t |> run("new struct with ctor on heap") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@new_struct_with_ctor_on_heap))
        var f = new_struct_with_ctor_on_heap()
        t |> equal(1234, f.a)
        t |> equal(1234., f.b)
        t |> equal("1234", f.c)
        t |> equal(1234, f.d)
    }
    t |> run("make struct heap") @(t : T?) {
        var f = make_struct_heap()
        t |> success(!jit_enabled() || is_jit_function(@@make_struct_heap))
        t |> equal(1, f.a)
        t |> equal(0.0, f.b)
        t |> equal("bar", f.c)
        t |> equal(0, f.d)
    }
    // t |> run("make struct with block") @(t : T?) {
    //     var f = make_struct_with_block()
    // t |> success(!jit_enabled() ||
    // is_jit_function(@@make_struct_with_block)) t |> equal(2,     f.a) t
    // |> equal(0.0,   f.b) t |> equal("bar", f.c) t |> equal(0,     f.d)
    // }
    t |> run("make struct heap with init") @(t : T?) {
        var f = make_struct_heap_with_init()
        t |> success(!jit_enabled() || is_jit_function(@@make_struct_heap_with_init))
        t |> equal(1, f.a)
        t |> equal(1234.0, f.b)
        t |> equal("bar", f.c)
        t |> equal(1234, f.d)
    }
    t |> run("make struct with substruct heap") @(t : T?) {
        var f = make_struct_with_substruct_heap()
        t |> success(!jit_enabled() || is_jit_function(@@make_struct_with_substruct_heap))
        t |> equal(1, f.a)
        t |> equal(2, f.foo.a)
        t |> equal(3., f.foo.b)
        t |> equal("", f.foo.c)
        t |> equal(0, f.foo.d)
        t |> equal(4, f.b)
    }
    t |> run("make struct with substruct heap with init") @(t : T?) {
        var f = make_struct_with_substruct_heap_with_init()
        t |> success(!jit_enabled() || is_jit_function(@@make_struct_with_substruct_heap_with_init))
        t |> equal(13, f.a)
        t |> equal(2, f.foo.a)
        t |> equal(3., f.foo.b)
        t |> equal("1234", f.foo.c)
        t |> equal(1234, f.foo.d)
        t |> equal(10, f.b)
    }
    t |> run("move init") @(t : T?) {
        let f <- struct_field_move_init()
        t |> success(!jit_enabled() || is_jit_function(@@struct_field_move_init))
        for (i in range(10)) {
            t |> equal(f.a[i], i * i)
        }
        t |> equal(f.b, 13)
    }
}

typedef TupleFooT = tuple<a : int; b : float; c : string; d : int>
typedef TupleBarT = tuple<a : int; foo : TupleFooT; b : int>


def tuple_field(var f : TupleFooT) {
    f.a = 1
    f.b = 2.0
    f.c = "hello"
    return f.d
}

def make_tuple_cmres : TupleFooT => (a = 1, b = 0.0, c = "bar", d = 0)

def make_tuple_with_subtuple_cmres : TupleBarT =>
    (a = 1, foo = (a = 2, b = 3., c = "", d = 0), b = 4)

def TupleFooT : TupleFooT => (a = 1234, b = 1234.0, c = "1234", d = 1234)

def make_tuple_with_field_call_to_cmres : TupleBarT => (a = 1, foo = TupleFooT(), b = 4)

def make_tuple_on_stack(which : bool) : TupleFooT {
    let a : TupleFooT = (a = 1, b = 2.0, c = "3", d = 4)
    let b : TupleFooT = (a = 2, b = 3.0, c = "4", d = 5)
    return which ? a : b
}

def make_tuple_with_subtuple_on_stack(which : bool) : TupleBarT {
    let a : TupleBarT = (a = 1, foo = (a = 2, b = 3., c = "", d = 0), b = 4)
    let b : TupleBarT = (a = 2, foo = (a = 3, b = 4., c = "", d = 0), b = 5)
    return which ? a : b
}

[test(description = "JIT-compiled tuple fields, copy-or-move-result returns and stack creation return the interpreter's results")]
def test_tuple(t : T?) {
    t |> run("tuple field") @(t : T?) {
        var f : TupleFooT
        f.d = 13
        t |> success(!jit_enabled() || is_jit_function(@@tuple_field))
        let res = tuple_field(f)
        t |> equal(13, res)
        t |> equal(1, f.a)
        t |> equal(2.0, f.b)
        t |> equal("hello", f.c)
    }
    t |> run("make tuple cmres") @(t : T?) {
        let f = make_tuple_cmres()
        t |> success(!jit_enabled() || is_jit_function(@@make_tuple_cmres))
        t |> equal(1, f.a)
        t |> equal(0.0, f.b)
        t |> equal("bar", f.c)
        t |> equal(0, f.d)
    }
    t |> run("make struct with substruct cmres") @(t : T?) {
        let f = make_tuple_with_subtuple_cmres()
        t |> success(!jit_enabled() || is_jit_function(@@make_tuple_with_subtuple_cmres))
        t |> equal(1, f.a)
        t |> equal(2, f.foo.a)
        t |> equal(3., f.foo.b)
        t |> equal("", f.foo.c)
        t |> equal(0, f.foo.d)
        t |> equal(4, f.b)
    }
    t |> run("make struct with substruct equal to call to cmres") @(t : T?) {
        let f = make_tuple_with_field_call_to_cmres()
        t |> success(!jit_enabled() || is_jit_function(@@make_tuple_with_field_call_to_cmres))
        t |> equal(1, f.a)
        t |> equal(1234,  f.foo.a)
        t |> equal(1234., f.foo.b)
        t |> equal("1234", f.foo.c)
        t |> equal(1234,  f.foo.d)
        t |> equal(4, f.b)
    }
    t |> run("make struct on stack") @(t : T?) {
        let a = make_tuple_on_stack(true)
        let b = make_tuple_on_stack(false)
        t |> success(!jit_enabled() || is_jit_function(@@make_tuple_on_stack))
        t |> equal(1,   a.a)
        t |> equal(2.,  a.b)
        t |> equal("3", a.c)
        t |> equal(4,   a.d)
        t |> equal(2,   b.a)
        t |> equal(3.,  b.b)
        t |> equal("4", b.c)
        t |> equal(5,   b.d)
    }
    t |> run("make struct with substruct on stack") @(t : T?) {
        let a = make_tuple_with_subtuple_on_stack(true)
        let b = make_tuple_with_subtuple_on_stack(false)
        t |> success(!jit_enabled() || is_jit_function(@@make_tuple_with_subtuple_on_stack))
        t |> equal(1,  a.a)
        t |> equal(2,  a.foo.a)
        t |> equal(3., a.foo.b)
        t |> equal("", a.foo.c)
        t |> equal(0,  a.foo.d)
        t |> equal(4,  a.b)
        t |> equal(2,  b.a)
        t |> equal(3,  b.foo.a)
        t |> equal(4., b.foo.b)
        t |> equal("", b.foo.c)
        t |> equal(0,  b.foo.d)
        t |> equal(5,  b.b)
    }
}

variant FooT {
    a : int
    b : float
    c : string
}

variant BarT {
    a : int
    foo : FooT
}

def variant_field(var f : FooT) {
    unsafe {
        f.b = 1.
        return f.a
    }
}

def variant_field_move(var f : FooT) {
    let b = BarT(foo <- f)
    return b
}

def variant_is(var f : FooT) {
    f = FooT(b = 1.)
    return f is b
}

def variant_as(var f : FooT) {
    f = FooT(b = 1.)
    f as b = 2.
    return f as b
}

def variant_as_panic(var f : FooT) {
    f = FooT(b = 1.)
    return f as a
}

def make_variant_cmres : FooT => FooT(a = 13)

def make_variant_with_subvariant_cmres : BarT => BarT(foo = FooT(a = 13))

def FooT : FooT => FooT(c = "1234")

def make_variant_with_field_call_to_cmres : BarT => BarT(foo = FooT())

def make_variant_on_stack(which : bool) : FooT {
    let a = FooT(a = 1)
    let b = FooT(b = 2.)
    return which ? a : b
}

def make_variant_with_subvariant_on_stack(which : bool) : BarT {
    let a = BarT(foo = FooT(a = 1))
    let b = BarT(foo = FooT(b = 2.))
    return which ? a : b
}

[sideeffects]
def ask_variant_index {
    let f = FooT(b = 1.)
    return variant_index(f)
}

[sideeffects]
def change_variant_index {
    var f = FooT(b = 1.)
    unsafe(set_variant_index(f, 0))
    return f as a
}

[test(description = "JIT-compiled variant field access, is/as, index and construction return the interpreter's results, and as on the wrong case panics")]
def test_variant(t : T?) {    // nolint:STYLE038 -- flat list of independent JIT arms, one per variant operation
    t |> run("variant field") @(t : T?) {
        var f : FooT
        t |> success(!jit_enabled() || is_jit_function(@@variant_field))
        let res = variant_field(f)
        t |> equal(res, int(0x3f800000))
    }
    t |> run("variant field move") @(t : T?) {
        var f : FooT
        t |> success(!jit_enabled() || is_jit_function(@@variant_field_move))
        f = FooT(a = 1234)
        let res = variant_field_move(f)
        t |> equal(0, f as a)
        t |> equal(1234, (res as foo) as a)
    }
    t |> run("variant is") @(t : T?) {
        var f : FooT
        t |> success(!jit_enabled() || is_jit_function(@@variant_is))
        t |> success(variant_is(f))
        t |> equal(1., f as b)
    }
    t |> run("variant as") @(t : T?) {
        var f : FooT
        t |> success(!jit_enabled() || is_jit_function(@@variant_as))
        t |> equal(2., variant_as(f))
        t |> equal(2., f as b)
    }
    t |> run("variant as panic") @(t : T?) {
        var f : FooT
        t |> success(!jit_enabled() || is_jit_function(@@variant_as_panic))
        var failed = false
        try {
            t |> equal(2, variant_as_panic(f))
        } recover {
            failed = true
        }
        t |> success(failed)
    }

    t |> run("make variant cmres") @(t : T?) {
        let f = make_variant_cmres()
        t |> success(!jit_enabled() || is_jit_function(@@make_variant_cmres))
        t |> success(f is a)
        t |> equal(13, f as a)
    }
    t |> run("make struct with substruct cmres") @(t : T?) {
        let f = make_variant_with_subvariant_cmres()
        t |> success(!jit_enabled() || is_jit_function(@@make_variant_with_subvariant_cmres))
        t |> success(f is foo)
        t |> success(f as foo is a)
        t |> equal(13, f as foo as a)
    }
    t |> run("make struct with substruct equal to call to cmres") @(t : T?) {
        let f = make_variant_with_field_call_to_cmres()
        t |> success(!jit_enabled() || is_jit_function(@@make_variant_with_field_call_to_cmres))
        t |> success(f is foo)
        t |> success(f as foo is c)
        t |> equal("1234", f as foo as c)
    }
    t |> run("make struct on stack") @(t : T?) {
        let a = make_variant_on_stack(true)
        let b = make_variant_on_stack(false)
        t |> success(!jit_enabled() || is_jit_function(@@make_variant_on_stack))
        t |> success(a is a)
        t |> equal(1, a as a)
        t |> success(b is b)
        t |> equal(2., b as b)
    }
    t |> run("make struct with substruct on stack") @(t : T?) {
        let a = make_variant_with_subvariant_on_stack(true)
        let b = make_variant_with_subvariant_on_stack(false)
        t |> success(!jit_enabled() || is_jit_function(@@make_variant_with_subvariant_on_stack))
        t |> success(a is foo)
        t |> success(a as foo is a)
        t |> equal(1, a as foo as a)
        t |> success(b is foo)
        t |> success(b as foo is b)
        t |> equal(2., b as foo as b)
    }
    t |> run("variant_index") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@ask_variant_index))
        t |> equal(1, ask_variant_index())
    }
    t |> run("set_variant_index") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@change_variant_index))
        t |> equal(int(0x3f800000), change_variant_index())
    }
}

bitfield bits123 {
    one
    two
    three
}

def is_one(b : bits123) => b.one

def set_one(var b : bits123) {
    b |= bits123.one    // nolint:STYLE022 -- the |= codegen is the JIT shape under test
    return b
}

[test(description = "reading and setting a bitfield bit in JIT-compiled code returns the interpreter's value")]
def test_bitfield(t : T?) {
    t |> run("one") @(t : T?) {
        verify(!jit_enabled() || is_jit_function(@@is_one))
        t |> equal(true, is_one(bits123.one))
        t |> equal(false, is_one(bits123.two))
        t |> equal(false, is_one(bits123.three))
    }
    t |> run("set_one") @(t : T?) {
        verify(!jit_enabled() || is_jit_function(@@set_one))
        t |> equal(bits123.one, set_one(bits123.one))
        t |> equal(bits123.one | bits123.two, set_one(bits123.two))
        t |> equal(bits123.one | bits123.three, set_one(bits123.three))
    }
}

def vec_at_r2v(vec; idx : int) => vec[idx]

def check_vec2_at_r2v(t : T?; v : auto(vecT); a1, a2 : auto(argT)) {
    t |> success(!jit_enabled()
                 || is_jit_function(@@ < (vec : vecT; idx : int) : argT > vec_at_r2v))
    let f2 = vecT(a1, a2)
    let i1 = vec_at_r2v(f2, 0)
    let i2 = vec_at_r2v(f2, 1)
    t |> equal(a1, i1)
    t |> equal(a2, i2)
}

def check_vec3_at_r2v(t : T?; v : auto(vecT); a1, a2, a3 : auto(argT)) {
    t |> success(!jit_enabled()
                 || is_jit_function(@@ < (vec : vecT; idx : int) : argT > vec_at_r2v))
    let f2 = vecT(a1, a2, a3)
    let i1 = vec_at_r2v(f2, 0)
    let i2 = vec_at_r2v(f2, 1)
    let i3 = vec_at_r2v(f2, 2)
    t |> equal(a1, i1)
    t |> equal(a2, i2)
    t |> equal(a3, i3)
}

def check_vec4_at_r2v(t : T?; v : auto(vecT); a1, a2, a3, a4 : auto(argT)) {
    t |> success(!jit_enabled()
                 || is_jit_function(@@ < (vec : vecT; idx : int) : argT > vec_at_r2v))
    let f2 = vecT(a1, a2, a3, a4)
    let i1 = vec_at_r2v(f2, 0)
    let i2 = vec_at_r2v(f2, 1)
    let i3 = vec_at_r2v(f2, 2)
    let i4 = vec_at_r2v(f2, 3)
    t |> equal(a1, i1)
    t |> equal(a2, i2)
    t |> equal(a3, i3)
    t |> equal(a4, i4)
}

def set_vec(var vec &; idx : int; val : auto(argT)) {
    vec[idx] = val
}

def check_set_vec2(t : T?; var v : auto(vecT); a1, a2 : auto(argT)) {
    t |> success(!jit_enabled()
                 || is_jit_function(@@ < (var vec : vecT&; idx : int; val : argT) > set_vec))
    var f2 : vecT
    set_vec(f2, 0, a1)
    set_vec(f2, 1, a2)
    t |> equal(a1, f2[0])
    t |> equal(a2, f2[1])
}

def check_set_vec3(t : T?; var v : auto(vecT); a1, a2, a3 : auto(argT)) {
    t |> success(!jit_enabled()
                 || is_jit_function(@@ < (var vec : vecT&; idx : int; val : argT) > set_vec))
    var f2 : vecT
    set_vec(f2, 0, a1)
    set_vec(f2, 1, a2)
    set_vec(f2, 2, a3)
    t |> equal(a1, f2[0])
    t |> equal(a2, f2[1])
    t |> equal(a3, f2[2])
}

def check_set_vec4(t : T?; var v : auto(vecT); a1, a2, a3, a4 : auto(argT)) {
    t |> success(!jit_enabled()
                 || is_jit_function(@@ < (var vec : vecT&; idx : int; val : argT) > set_vec))
    var f2 : vecT
    set_vec(f2, 0, a1)
    set_vec(f2, 1, a2)
    set_vec(f2, 2, a3)
    set_vec(f2, 3, a4)
    t |> equal(a1, f2[0])
    t |> equal(a2, f2[1])
    t |> equal(a3, f2[2])
    t |> equal(a4, f2[3])
}

[test(description = "indexing and index-assigning float2, float3 and float4 components under JIT returns the interpreter's values")]
def test_vector_navigation(t : T?) {
    t |> run("vector2 [] r2v") @(t : T?) {
        check_vec2_at_r2v(t, type<float2>, 1.0, 2.0)
        check_vec2_at_r2v(t, type<int2>, 1, 2)
        check_vec2_at_r2v(t, type<uint2>, 1u, 2u)
        check_vec2_at_r2v(t, type<range>, 1, 2)
        check_vec2_at_r2v(t, type<urange>, 1u, 2u)
        check_vec2_at_r2v(t, type<range64>, 1l, 2l)
        check_vec2_at_r2v(t, type<urange64>, 1ul, 2ul)
    }
    t |> run("vector3 [] r2v") @(t : T?) {
        check_vec3_at_r2v(t, type<float3>, 1.0, 2.0, 3.0)
        check_vec3_at_r2v(t, type<int3>, 1, 2, 3)
        check_vec3_at_r2v(t, type<uint3>, 1u, 2u, 3u)
    }
    t |> run("vector4 [] r2v") @(t : T?) {
        check_vec4_at_r2v(t, type<float4>, 1.0, 2.0, 3.0, 4.0)
        check_vec4_at_r2v(t, type<int4>, 1, 2, 3, 4)
        check_vec4_at_r2v(t, type<uint4>, 1u, 2u, 3u, 4u)
    }
    t |> run("set vector2 []") @(t : T?) {
        check_set_vec2(t, type<float2>, 1.0, 2.0)
        check_set_vec2(t, type<int2>, 1, 2)
        check_set_vec2(t, type<uint2>, 1u, 2u)
        check_set_vec2(t, type<range>, 1, 2)
        check_set_vec2(t, type<urange>, 1u, 2u)
        check_set_vec2(t, type<range64>, 1l, 2l)
        check_set_vec2(t, type<urange64>, 1ul, 2ul)
    }
    t |> run("set vector3 []") @(t : T?) {
        check_set_vec3(t, type<float3>, 1.0, 2.0, 3.0)
        check_set_vec3(t, type<int3>, 1, 2, 3)
        check_set_vec3(t, type<uint3>, 1u, 2u, 3u)
    }
    t |> run("set vector4 []") @(t : T?) {
        check_set_vec4(t, type<float4>, 1.0, 2.0, 3.0, 4.0)
        check_set_vec4(t, type<int4>, 1, 2, 3, 4)
        check_set_vec4(t, type<uint4>, 1u, 2u, 3u, 4u)
    }
}

def swizzle_ref_xyz(var v : float4&; xyz : float3) {
    v.xyz = xyz
}

def swizzle_ref_xy(var v : float4&; xy : float2) {
    v.xy = xy
}

def swizzle_ref_x(var v : float4&; x : float) {
    v.x = x
}

def swizzle_ref_yzw(var v : float4&; xyz : float3) {
    v.yzw = xyz
}

def swizzle_ref_yz(var v : float4&; xy : float2) {
    v.yz = xy
}

def swizzle_ref_y(var v : float4&; x : float) {
    v.y = x
}


def swizzle_xyz(v : float4) => v.xyz

def swizzle_xy(v : float4) => v.xy
def swizzle_x(v : float4) => v.x

def swizzle_yzw(v : float4) => v.yzw

def swizzle_yz(v : float4) => v.yz

def swizzle_y(v : float4) => v.y

[sideeffects]
def get_screen_size(x, y : float) => float2(x, y)

def swizzle_1 => get_screen_size(1., 2.).y

[test(description = "JIT-compiled vector swizzles read and write the same components as the interpreter")]
def test_vector_swizzle(t : T?) {
    t |> run("swizzle 1") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@swizzle_1))
        let q = swizzle_1()
        t |> equal(2., q)
    }
    t |> run("swizzle ref") @(t : T?) {
        var v = float4(1, 2, 3, 4)
        t |> success(!jit_enabled() || is_jit_function(@@swizzle_ref_xyz))
        swizzle_ref_xyz(v, float3(5, 6, 7))
        t |> equal(v, float4(5, 6, 7, 4))
        t |> success(!jit_enabled() || is_jit_function(@@swizzle_ref_xy))
        swizzle_ref_xy(v, float2(8, 9))
        t |> equal(v, float4(8, 9, 7, 4))
        t |> success(!jit_enabled() || is_jit_function(@@swizzle_ref_x))
        swizzle_ref_x(v, 10.)
        t |> equal(v, float4(10, 9, 7, 4))
        v = float4(1, 2, 3, 4)
        t |> success(!jit_enabled() || is_jit_function(@@swizzle_ref_yzw))
        swizzle_ref_yzw(v, float3(5, 6, 7))
        t |> equal(v, float4(1, 5, 6, 7))
        t |> success(!jit_enabled() || is_jit_function(@@swizzle_ref_yz))
        swizzle_ref_yz(v, float2(8, 9))
        t |> equal(v, float4(1, 8, 9, 7))
        t |> success(!jit_enabled() || is_jit_function(@@swizzle_ref_y))
        swizzle_ref_y(v, 10.)
        t |> equal(v, float4(1, 10, 9, 7))
    }
    t |> run("swizzle imm") @(t : T?) {
        let v = float4(1, 2, 3, 4)
        t |> success(!jit_enabled() || is_jit_function(@@swizzle_xyz))
        t |> equal(float3(1, 2, 3), swizzle_xyz(v))
        t |> success(!jit_enabled() || is_jit_function(@@swizzle_xy))
        t |> equal(float2(1, 2), swizzle_xy(v))
        t |> success(!jit_enabled() || is_jit_function(@@swizzle_x))
        t |> equal(1., swizzle_x(v))
        t |> success(!jit_enabled() || is_jit_function(@@swizzle_yzw))
        t |> equal(float3(2, 3, 4), swizzle_yzw(v))
        t |> success(!jit_enabled() || is_jit_function(@@swizzle_yz))
        t |> equal(float2(2, 3), swizzle_yz(v))
        t |> success(!jit_enabled() || is_jit_function(@@swizzle_y))
        t |> equal(2., swizzle_y(v))
    }
}

def nullc(a, b) => a ?? b

def nullc_ref(var a : NavFoo?; var b : NavFoo; c : int) {
    (a ?? b).a = c
}

struct NavFoo {
    a, b : int
}

[test(description = "?? on a value and on a reference falls back to the default only when the left side is null")]
def test_null_coalescing(t : T?) {
    t |> run("null coalescing value") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@ < (a : int?; b : int) : void > nullc))
        var i = 13
        var pi = unsafe(addr(i))
        var pj : int?
        let n1 = nullc(pi, 1)
        let n2 = nullc(pj, 2)
        t |> equal(13, n1)
        t |> equal(2, n2)
    }
    t |> run("null coalescing ref") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@nullc_ref))
        var f = NavFoo(a = 13)
        var pf = unsafe(addr(f))
        var pn : NavFoo?
        var df = NavFoo(a = 1)
        nullc_ref(pf, df, 42)
        t |> equal(42, f.a)
        t |> equal(1, df.a)
        nullc_ref(pn, df, 33)
        t |> equal(33, df.a)
    }
}

def bOrD(f : NavFoo?; d : int) => f?.b ?? d

struct NavBar {
    f : NavFoo?
}

def bOrD2(b : NavBar?; d : int) => b?.f?.b ?? d

[test(description = "?. on a pointer to a structure and to a structure with a pointer field returns null for null and the field otherwise")]
def test_safe_field(t : T?) {
    t |> run("safe_field ptr to structure") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@bOrD))
        var f = NavFoo(a = 1, b = 2)
        var pF : NavFoo? = unsafe(addr(f))
        var pN : NavFoo?
        t |> equal(2, bOrD(pF, 3))
        t |> equal(4, bOrD(pN, 4))
    }
    t |> run("safe_field ptr to structure field with ptr") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@bOrD2))
        var f = NavFoo(a = 1, b = 2)
        var bF = NavBar(f = unsafe(addr(f)))
        var bN = NavBar()
        t |> equal(2, bOrD2(unsafe(addr(bF)), 3))
        t |> equal(4, bOrD2(unsafe(addr(bN)), 4))
    }
}

def jit_safe_at(a, b, c) => a?[b] ?? c

[test(description = "?[ ] on dims, arrays, tables and pointers to them returns the element or the default for a null container")]
def test_safe_at(t : T?) {
    t |> run("safe_at dim") @(t : T?) {
        t |> success(!jit_enabled()
                     || is_jit_function(@@ < (a : int const[3]; b, c : int) : int > jit_safe_at))
        let i = fixed_array(1, 2, 3)
        let v1 = jit_safe_at(i, 0, 42)
        let v2 = jit_safe_at(i, 3, 42)
        t |> equal(1, v1)
        t |> equal(42, v2)
    }
    t |> run("safe_at ptr to dim") @(t : T?) {
        t |> success(!jit_enabled()
                     || is_jit_function(@@ < (a : int const[3]?; b, c : int) : int > jit_safe_at))
        let i = fixed_array(1, 2, 3)
        let pi = unsafe(addr(i))
        let p0 : int[3]?
        let v1 = jit_safe_at(pi, 0, 42)
        let v2 = jit_safe_at(p0, 0, 42)
        let v3 = jit_safe_at(pi, 3, 42)
        let v4 = jit_safe_at(p0, 3, 42)
        t |> equal(1, v1)
        t |> equal(42, v2)
        t |> equal(42, v3)
        t |> equal(42, v4)
    }
    t |> run("safe_at array") @(t : T?) {
        t |> success(!jit_enabled()
                     || is_jit_function(@@ < (a : array<int>; b, c : int) : int > jit_safe_at))
        let i <- [ 1, 2, 3]
        let v1 = jit_safe_at(i, 0, 42)
        let v2 = jit_safe_at(i, 3, 42)
        t |> equal(1, v1)
        t |> equal(42, v2)
    }
    t |> run("safe_at ptr to array") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@ < (a : array<int> const?; b, c : int) : int > jit_safe_at))
        let i <- [ 1, 2, 3]
        let pi = unsafe(addr(i))
        let p0 : int[3]?
        let v1 = jit_safe_at(pi, 0, 42)
        let v2 = jit_safe_at(p0, 0, 42)
        let v3 = jit_safe_at(pi, 3, 42)
        let v4 = jit_safe_at(p0, 3, 42)
        t |> equal(1, v1)
        t |> equal(42, v2)
        t |> equal(42, v3)
        t |> equal(42, v4)
    }
    t |> run("safe_at table") @(t : T?) {
        t |> success(!jit_enabled()
                     || is_jit_function(@@ < (a : table<int; int>; b, c : int) : int > jit_safe_at))
        let i <- {0 => 1, 1 => 2, 2 => 3}
        let v1 = jit_safe_at(i, 0, 42)
        let v2 = jit_safe_at(i, 3, 42)
        t |> equal(1, v1)
        t |> equal(42, v2)
    }
    t |> run("safe_at ptr to table") @(t : T?) {
        t |> success(!jit_enabled()
                     || is_jit_function(
                                        @@ < (a : table<int; int> const?; b,
                                              c : int) : int > jit_safe_at))
        let i <- {0 => 1, 1 => 2, 2 => 3}
        let pi = unsafe(addr(i))
        let p0 : int[3]?
        let v1 = jit_safe_at(pi, 0, 42)
        let v2 = jit_safe_at(p0, 0, 42)
        let v3 = jit_safe_at(pi, 3, 42)
        let v4 = jit_safe_at(p0, 3, 42)
        t |> equal(1, v1)
        t |> equal(42, v2)
        t |> equal(42, v3)
        t |> equal(42, v4)
    }
}

variant IOrF {
    i : int
    f : float
}

def safe_as_i(a : IOrF; b : int) => (a ?as i) ?? b

def safe_as_pi(a : IOrF?; b : int) => (a ?as i) ?? b

[test(description = "?as on a variant value and a variant pointer returns the case value or null")]
def test_safe_variant_as(t : T?) {
    t |> run("safe variant as") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@safe_as_i))
        let i = IOrF(i = 1)
        let f = IOrF(f = 1.)
        t |> equal(1, safe_as_i(i, 42))
        t |> equal(42, safe_as_i(f, 42))
    }
    t |> run("safe variant ptr as") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@safe_as_pi))
        var i = IOrF(i = 1)
        var f = IOrF(f = 1.)
        var pi = unsafe(addr(i))
        var pf = unsafe(addr(f))
        var p0 : IOrF?
        t |> equal(1, safe_as_pi(pi, 42))
        t |> equal(42, safe_as_pi(pf, 42))
        t |> equal(42, safe_as_pi(p0, 42))
    }
}

[sideeffects]
def test_using(t : T?) {
    using() $(var f : FancyClass) {
        t |> equal(13, f.value)
    }
    using(1, 3) $(var f : FancyClass) {
        t |> equal(4, f.value)
    }
}

[sideeffects]
def test_placement_new(t : T?) {
    let fc0 = FancyClass()
    t |> equal(13, fc0.value)
    let fc = FancyClass(1, 2)
    t |> equal(3, fc.value)
}

class PlacementFoo {
    a : int
    b : int
    def PlacementFoo {
        a = 13
    }
}

[sideeffects]
def test_ascend_ctor(t : T?) {
    let foo = new PlacementFoo(b = 42)
    t |> equal(13, foo.a)
    t |> equal(42, foo.b)
}

[test(description = "using, placement new and an ascend constructor build and destroy the object under JIT")]
def test_using_and_placement_new(t : T?) {
    t |> run("using") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_using))
        test_using(t)
    }
    t |> run("placement new") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_placement_new))
        test_placement_new(t)
    }
    t |> run("ascend constructor") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_ascend_ctor))
        test_ascend_ctor(t)
    }
}
