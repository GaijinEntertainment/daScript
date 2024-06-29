require dastest/testing_boost public

require daslib/strings_boost

require _operators_derived

struct Foo
    a : int

struct Bar
    a : float

def operator as Bar ( a : Foo )
    return [[Bar a = float(a.a)]]

def operator ?as Bar ( a : Foo? ) : Bar?
    if a == null
        return null
    return new [[Bar a = float(a.a)]]

def operator is Bar ( anything )
    return false

def operator is Bar ( a : Bar )
    return true

struct Goo
    a : string

def operator [] ( t:Goo; cmp:string )
    return "{t . . a}-{cmp}"

def operator ?[] ( t:Goo; cmp:string ) : string const?#
    if cmp==t . . a
        unsafe
            return reinterpret<string const?#> addr(t . . a)
    else
        unsafe
            return reinterpret<string const?#> null

def operator . ( t:Goo; name:string )
    return [[Goo a="{t . . a}.{name}"]]

def operator ?. ( t:Goo; name:string ) : string const?#
    if name=="yes"
        unsafe
            return reinterpret<string const?#> addr(t . . a)
    else
        unsafe
            return reinterpret<string const?#> null

def operator . length ( t:Goo )
    return length(t . . a)

def operator ?. length ( t:Goo )
    return length(t . . a) + 1

[test]
def test_operators ( t : T? )
    t |> run("as, ?as, is") <| @ ( t : T? )
        var f = [[Foo a= 13]]
        var b = f as Bar
        t |> equal(13., b.a)
        t |> success(b is Bar)
        t |> success(!(f is Bar))
        var pf = unsafe(addr(f))
        var pb = pf ?as Bar ?? [[Bar a = 0.0]]
        t |> equal(13., pb.a)
        var nf : Foo?
        var nb = nf ?as Bar ?? [[Bar a = 1.0]]
        t |> equal(1., nb.a)
    t |> run("[], ?[], ., ?.") <| @ ( t : T? )
        var T = [[Goo a="hello"]]
        var z = T["blah"]
        var w = T?["blah"] ?? "not_blah"
        var W = T?["hello"] ?? "this???"
        var EZ = T.GooBar
        var HARD_YES = T?.yes ?? "how??"
        var HARD_NO = T?.no ?? "hard no"
        t |> equal("hello-blah",z)
        t |> equal("not_blah",w)
        t |> equal("hello",W)
        t |> equal("hello.GooBar",EZ . . a)
        t |> equal("hello",HARD_YES)
        t |> equal("hard no",HARD_NO)
        t |> equal(5,T.length)
        t |> equal(6,T?.length)

        var bar : BarOp? <- new [[BarOp() a=42]]
        t |> equal(42, bar.Prop)

