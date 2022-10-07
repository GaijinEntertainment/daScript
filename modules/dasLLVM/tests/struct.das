require dastest/testing_boost

require llvm/llvm_jit

struct Foo
    a : int = 1234
    b : float = 1234.0
    c : string = "1234"
    d : int = 1234

[jit]
def struct_field ( var f : Foo )
    f.a = 1
    f.b = 2.0
    f.c = "hello"
    return f.d

[jit]
def struct_ptr_field ( var f : Foo? )
    f.a = 11
    f.b = 22.0
    f.c = "hhello"
    return f.d

[jit]
def struct_ctor_call
    return Foo()

[jit]
def make_struct_cmres
    return [[Foo a=1, c="bar"]]

[jit]
def make_struct_cmres_with_init
    return [[Foo() a=1, c="bar"]]

[test]
def test_struct ( t:T? )
    t |> run("struct field") <| @ ( t : T? )
        var f = [[Foo d=13]]
        t |> success ( is_jit_function(@@struct_field) )
        var res = struct_field(f)
        t |> equal ( 13, res )
        t |> equal ( 1, f.a )
        t |> equal ( 2.0, f.b )
        t |> equal ( "hello", f.c )
    t |> run("struct ptr field") <| @ ( t : T? )
        var f = [[Foo d=14]]
        t |> success ( is_jit_function(@@struct_ptr_field) )
        var res = struct_ptr_field(unsafe(addr(f)))
        t |> equal ( 14, res )
        t |> equal ( 11, f.a )
        t |> equal ( 22.0, f.b )
        t |> equal ( "hhello", f.c )
    t |> run("struct ctor call") <| @ ( t : T? )
        var f = struct_ctor_call()
        t |> success ( is_jit_function(@@struct_ctor_call) )
        t |> equal ( 1234, f.a )
        t |> equal ( 1234.0, f.b )
        t |> equal ( "1234", f.c )
        t |> equal ( 1234, f.d )
    t |> run("make struct cmres") <| @ ( t : T? )
        var f = make_struct_cmres()
        t |> success ( is_jit_function(@@make_struct_cmres) )
        t |> equal ( 1, f.a )
        t |> equal ( 0.0, f.b )
        t |> equal ( "bar", f.c )
        t |> equal ( 0, f.d )
    t |> run("make struct cmres with init") <| @ ( t : T? )
        var f = make_struct_cmres_with_init()
        t |> success ( is_jit_function(@@make_struct_cmres_with_init) )
        t |> equal ( 1, f.a )
        t |> equal ( 1234.0, f.b )
        t |> equal ( "bar", f.c )
        t |> equal ( 1234, f.d )


