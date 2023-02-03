require dastest/testing_boost public
require daslib/match

enum Color
    black
    red
    green
    blue

variant IF
    i : int
    f : float

struct Foo
    a : int
    v : IF

struct Bar
    a : int
    b : float

struct AB
    a, b : int

[sideeffects]
def enum_match ( color:Color )
    match(color) <|
        if Color black
            return 0
        if Color red
            return 1
        if $v(anything)
            return -1

[sideeffects]
def enum_static_match ( color, blah )
    static_match(color) <|
        if Color black
            return 0
        if blah
            return 1
        if $v(anything)
            return -1

[sideeffects]
def static_match_by_type ( what )
    static_match(what) <|
        if match_type($v(expr),type<int>)
            return "int {expr}"
        if $v(anything)
            return "anything"

[sideeffects]
def struct_match ( f : Foo )
    match(f) <|
        if [[Foo a=13]]
            return 0
        if [[Foo a=$v(anyA)]]
            return anyA

[sideeffects]
def struct_match ( b : Bar )
    match(b) <|
        if [[Bar a=13]]
            return 0
        if [[Bar a=$v(anyA), b=13.0]]
            return anyA
        if $v(anything)
            return -1

[sideeffects]
def guards_match ( ab : AB )
    match(ab) <|
        if [[AB a=$v(a), b=$v(b)]] && (b > a)
            return "{b} > {a}"
        if [[AB a=$v(a), b=$v(b)]]
            return "{b} <= {a}"

[sideeffects]
def variant_as_match ( v : IF )
    match(v) <|
        if $v(i) as i
            return "int"
        if $v(f) as f
            return "float"
        if $v(anything)
            return "anything"

[sideeffects]
def nested_struct_match ( v : Foo )
    match(v) <|
        if [[Foo a=1, v=$v(i) as i]]
            return 1
        if [[Foo v=$v(f) as f]]
            return 2
        if $v(anything)
            return 0

[sideeffects]
def variant_match ( v : IF )
    match(v) <|
        if [[IF i=$v(i)]]
            return 1
        if [[IF f=$v(f)]]
            return 2
        if $v(anything)
            return 0

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
    t |> run("guards match") <| @@ ( t : T? )
        t |> equal( "2 > 1", guards_match([[AB a=1, b=2]]) )
        t |> equal( "1 <= 2", guards_match([[AB a=2, b=1]]) )
    t |> run("variant match") <| @@ ( t : T? )
        t |> equal(1, variant_match([[IF i=1]]))
        t |> equal(2, variant_match([[IF f=1.0]]))
    t |> run("variant as match") <| @@ ( t : T? )
        t |> equal("int", variant_as_match([[IF i=1]]))
        t |> equal("float", variant_as_match([[IF f=1.0]]))
    t |> run("nested struct + variant as match") <| @@ ( t : T? )
        t |> equal(1, nested_struct_match([[Foo a=1, v=[[IF i=1]]]]))
        t |> equal(2, nested_struct_match([[Foo v=[[IF f=1.]]]]))
        t |> equal(0, nested_struct_match([[Foo a=13, v=[[IF i=1]]]]))



