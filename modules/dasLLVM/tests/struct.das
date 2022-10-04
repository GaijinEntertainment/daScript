require dastest/testing_boost

require llvm/llvm_jit

struct Foo
    a : int
    b : float
    c : string
    d : int

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



