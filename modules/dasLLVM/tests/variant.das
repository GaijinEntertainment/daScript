require dastest/testing_boost

require llvm/llvm_jit

variant FooT
    a:int
    b:float
    c:string

variant BarT
    a:int
    foo:FooT

[jit]
def variant_field ( var f : FooT )
    unsafe
        f.b = 1.
        return f.a

[jit]
def variant_is ( var f : FooT )
    f = [[FooT b = 1.]]
    return f is b

[jit]
def variant_as ( var f : FooT )
    f = [[FooT b = 1.]]
    f as b = 2.
    return f as b

/*
[jit]
def make_variant_cmres : FooT
    return [[auto 1, 0.0, "bar", 0]]

[jit]
def make_variant_with_subvariant_cmres : BarT
    return [[auto 1, [[auto 2, 3., "", 0]], 4]]

def FooT : FooT
    return [[auto 1234, 1234.0, "1234", 1234]]

[jit]
def make_variant_with_field_call_to_cmres : BarT
    return [[auto 1, FooT(), 4]]

[jit]
def make_variant_on_stack(which:bool) : FooT
    var a = [[auto 1, 2.0, "3", 4]]
    var b = [[auto 2, 3.0, "4", 5]]
    return which ? a : b

[jit]
def make_variant_with_subvariant_on_stack(which:bool) : BarT
    var a = [[auto 1, [[auto 2, 3., "", 0]], 4]]
    var b = [[auto 2, [[auto 3, 4., "", 0]], 5]]
    return which ? a : b
*/

[test]
def test_tuple ( t:T? )
    t |> run("variant field") <| @ ( t : T? )
        var f : FooT
        t |> success ( is_jit_function(@@variant_field) )
        var res = variant_field(f)
        t |> equal ( res, int(0x3f800000) )
    t |> run("variant is") <| @ ( t : T? )
        var f : FooT
        t |> success ( is_jit_function(@@variant_is) )
        t |> success ( variant_is(f) )
        t |> equal(1., f as b)
    t |> run("variant as") <| @ ( t : T? )
        var f : FooT
        t |> success ( is_jit_function(@@variant_as) )
        t |> equal(2., variant_as(f) )
        t |> equal(2., f as b)
/*
    t |> run("make tuple cmres") <| @ ( t : T? )
        var f = make_variant_cmres()
        t |> success ( is_jit_function(@@make_variant_cmres) )
        t |> equal ( 1, f.a )
        t |> equal ( 0.0, f.b )
        t |> equal ( "bar", f.c )
        t |> equal ( 0, f.d )
    t |> run("make struct with substruct cmres") <| @ ( t : T? )
        var f = make_variant_with_subvariant_cmres()
        t |> success ( is_jit_function(@@make_variant_with_subvariant_cmres) )
        t |> equal ( 1, f.a )
        t |> equal ( 2, f.foo.a )
        t |> equal ( 3., f.foo.b )
        t |> equal ( "", f.foo.c )
        t |> equal ( 0, f.foo.d )
        t |> equal ( 4, f.b )
    t |> run("make struct with substruct equal to call to cmres") <| @ ( t : T? )
        var f = make_variant_with_field_call_to_cmres()
        t |> success ( is_jit_function(@@make_variant_with_field_call_to_cmres) )
        t |> equal ( 1, f.a )
        t |> equal ( 1234,  f.foo.a )
        t |> equal ( 1234., f.foo.b )
        t |> equal ( "1234",f.foo.c )
        t |> equal ( 1234,  f.foo.d )
        t |> equal ( 4, f.b )
    t |> run("make struct on stack") <| @ ( t : T? )
        var a = make_variant_on_stack(true)
        var b = make_variant_on_stack(false)
        t |> success ( is_jit_function(@@make_variant_on_stack) )
        t |> equal ( 1,   a.a )
        t |> equal ( 2.,  a.b )
        t |> equal ( "3", a.c )
        t |> equal ( 4,   a.d )
        t |> equal ( 2,   b.a )
        t |> equal ( 3.,  b.b )
        t |> equal ( "4", b.c )
        t |> equal ( 5,   b.d )
    t |> run("make struct with substruct on stack") <| @ ( t : T? )
        var a = make_variant_with_subvariant_on_stack(true)
        var b = make_variant_with_subvariant_on_stack(false)
        t |> success ( is_jit_function(@@make_variant_with_subvariant_on_stack) )
        t |> equal ( 1,  a.a )
        t |> equal ( 2,  a.foo.a )
        t |> equal ( 3., a.foo.b )
        t |> equal ( "", a.foo.c )
        t |> equal ( 0,  a.foo.d )
        t |> equal ( 4,  a.b )
        t |> equal ( 2,  b.a )
        t |> equal ( 3,  b.foo.a )
        t |> equal ( 4., b.foo.b )
        t |> equal ( "", b.foo.c )
        t |> equal ( 0,  b.foo.d )
        t |> equal ( 5,  b.b )
*/