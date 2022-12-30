require dastest/testing_boost

require llvm/llvm_jit

[jit]
def nullc ( a, b )
    return a ?? b

[jit]
def nullc_ref ( var a : Foo?; var b : Foo; c : int )
    (a ?? b).a = c

struct Foo
    a,b : int

[test]
def test_null_coalescing ( t:T? )
    t |> run("null coalescing value") <| @ ( t : T? )
        t |> success(is_jit_function(@@<(a:int?;b:int):void>nullc))
        var i = 13
        var pi = unsafe(addr(i))
        var pj : int?
        var n1 = nullc(pi,1)
        var n2 = nullc(pj,2)
        t |> equal(13, n1)
        t |> equal(2, n2)
    t |> run("null coalescing ref") <| @ ( t : T? )
        t |> success(is_jit_function(@@nullc_ref))
        var f = [[Foo a=13]]
        var pf = unsafe(addr(f))
        var pn : Foo?
        var df = [[Foo a=1]]
        nullc_ref(pf,df,42)
        t |> equal(42, f.a)
        t |> equal(1, df.a)
        nullc_ref(pn,df,33)
        t |> equal(33, df.a)

[jit]
def bOrD ( f : Foo?; d:int )
    return f?.b ?? d

struct Bar
    f : Foo?

[jit]
def bOrD2 ( b : Bar?; d: int )
    return b?.f?.b ?? d

[test]
def test_safe_field ( t:T? )
    t |> run("safe_field ptr to structure") <| @ ( t : T? )
        t |> success(is_jit_function(@@bOrD))
        var f = [[Foo a=1,b=2]]
        var pF : Foo? = unsafe(addr(f))
        var pN : Foo?
        t |> equal ( 2, bOrD(pF, 3) )
        t |> equal ( 4, bOrD(pN, 4) )
    t |> run("safe_field ptr to structure field with ptr") <| @ ( t : T? )
        t |> success(is_jit_function(@@bOrD2))
        var f = [[Foo a=1,b=2]]
        var bF = [[Bar f=unsafe(addr(f))]]
        var bN = [[Bar]]
        t |> equal ( 2, bOrD2(unsafe(addr(bF)), 3) )
        t |> equal ( 4, bOrD2(unsafe(addr(bN)), 4) )

