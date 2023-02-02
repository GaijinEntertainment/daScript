require dastest/testing_boost public
require daslib/match

enum Color
    black
    red
    green
    blue

struct Foo
    a : int

struct Bar
    a : int
    b : float

[sideeffects]
def enum_match ( color:Color ) : int
    match(color) <|
        if Color black
            return 0
        if Color red
            return 1
        if $v(anything)
            return -1

[sideeffects]
def enum_static_match ( color, blah ) : int
    static_match(color) <|
        if Color black
            return 0
        if blah
            return 1
        if $v(anything)
            return -1

[sideeffects]
def static_match_by_type ( what ) : string
    static_match(what) <|
        if match_type($v(expr),type<int>)
            return "int {expr}"
        if $v(anything)
            return "anything"

[sideeffects]
def struct_match ( f : Foo ) : int
    match(f) <|
        if [[Foo a=13]]
            return 0
        if [[Foo a=$v(anyA)]]
            return anyA

[sideeffects]
def struct_match ( b : Bar ) : int
    match(b) <|
        if [[Bar a=13]]
            return 0
        if [[Bar a=$v(anyA), b=13.0]]
            return anyA
        if $v(anything)
            return -1

[test]
def test_all_match ( t:T? )
    t |> run("enum match") <| @@ ( t : T? )
        t |> equal ( 0, enum_match(Color black) )
        t |> equal ( 1, enum_match(Color red) )
        t |> equal ( -1, enum_match(Color green) )
        t |> equal ( -1, enum_match(Color blue) )
    t |> run("enum static match") <| @@ ( t : T? )
        t |> equal ( 0, enum_static_match(Color black,null) )
        t |> equal ( 1, enum_static_match(1234,1234) )
        t |> equal ( -1, enum_static_match(5678,1234) )
        t |> equal ( -1, enum_static_match("a",1234) )
    t |> run("struct match") <| @@ ( t : T? )
        t |> equal( 0, struct_match([[Foo a=13]]) )
        t |> equal( 23, struct_match([[Foo a=23]]) )
        t |> equal ( 0, struct_match([[Bar a=13]]) )
        t |> equal ( 23, struct_match([[Bar a=23, b=13.0]]) )
        t |> equal ( -1, struct_match([[Bar a=23, b=14.0]]) )

