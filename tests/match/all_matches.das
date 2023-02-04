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
        if _
            return -1

[sideeffects]
def enum_static_match ( color, blah )
    static_match(color) <|
        if Color black
            return 0
        if match_expr(blah)
            return 1
        if _
            return -1

[sideeffects]
def static_match_by_type ( what )
    static_match(what) <|
        if match_type($v(expr),type<int>)
            return "int {expr}"
        if _
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
        if _
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
        if _
            return "anything"

[sideeffects]
def nested_struct_match ( v : Foo )
    match(v) <|
        if [[Foo a=1, v=$v(i) as i]]
            return 1
        if [[Foo v=$v(f) as f]]
            return 2
        if _
            return 0

[sideeffects]
def variant_match ( v : IF )
    match(v) <|
        if [[IF i=$v(i)]]
            return 1
        if [[IF f=$v(f)]]
            return 2
        if _
            return 0

[sideeffects]
def static_array_match ( A : int[3] )
    match(A) <|
        if [[auto $v(a);$v(b);$v(c)]] && (a+b+c)==6 // total of 3 elements, sum is 6
            return 1
        if [[int 0;...]]    // starts with 0
            return 0
        if [[int ...;13]]   // ends with 13
            return 2
        if [[int 12;...;12]]    // starts and ends with 12
            return 3
        if _
            return -1

[sideeffects]
def dynamic_array_match ( A : array<int> )
    match(A) <|
        if [{auto $v(a);$v(b);$v(c)}] && (a+b+c)==6 // total of 3 elements, sum is 6
            return 1
        if [{int 0;0;0;...}]    // first 3 are 0
            return 0
        if [{int ...;1;2}]      // ends with 1,2
            return 2
        if [{int 0;1;...;2;3}]    // starts with 0,1, ends with 2,3
            return 3
        if _
            return -1


[sideeffects]
def ascending_array_match ( A : int[3] )
    match(A) <|
        if [[int $v(x);match_expr(x+1);match_expr(x+2)]]
            return true
        if _
            return false


[export,sideeffects]
def tuple_match ( A : tuple<int;float;string> )
    match(A) <|
        if [[auto 1,_,"3"]]
            return 1
        if [[auto 13,...]]      // starts with 13
            return 2
        if [[auto ...,"13"]]    // ends with 13
            return 3
        if [[auto 2,...,"2"]]   // starts with 2, ends with "2"
            return 4
        if _
            return 0

[sideeffects]
def or_match ( i :int )
    match(i) <|
        if 1 || 2
            return true
        if _
            return false

[sideeffects]
def or_match_v ( B:Bar )
    match(B) <|
        if [[Bar a=1, b=$v(b)]] || [[Bar a=2, b=$v(b)]]
            return b
        if _
            return 0.0

[sideeffects]
def struct_ptr_match ( f : Foo? )
    match(f) <|
        if null
            return -1
        if [[Foo a=13]]
            return 0
        if [[Foo a=$v(anyA)]]
            return anyA
        if _
            return -2

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
    t |> run("static array match") <| @@ ( t : T? )
        t |> equal( 1, static_array_match([[int 1;2;3]]))
        t |> equal( 0, static_array_match([[int 0;1;2]]))   // strts with 0
        t |> equal( 0, static_array_match([[int 0;0;0]]))   // strts with 0
        t |> equal(-1, static_array_match([[int 3;3;3]]))
        t |> equal(2, static_array_match([[int 3;3;13]]))  // ends with 12
        t |> equal(3, static_array_match([[int 12;3;12]]))  // starts and ends with 12
    t |> run("dynamic array match") <| @@ ( t : T? )
        t |> equal( 1, dynamic_array_match([{int 1;2;3}]))          // summ is 6, 3 total
        t |> equal( 0, dynamic_array_match([{int 0;0;0}]))          // first 3 are 0
        t |> equal( 0, dynamic_array_match([{int 0;0;0;1;2;3}]))    // first 3 are 0
        t |> equal(-1, dynamic_array_match([{int 3;3;3}]))
        t |> equal(-1, dynamic_array_match([{int 1;2;3;4}]))
        t |> equal(-1, dynamic_array_match([{int 2;2}]))
        t |> equal(2, dynamic_array_match([{int 1;2}]))     // ends with 1,2
        t |> equal(2, dynamic_array_match([{int 2;1;2}]))   // ends with 1,2
        t |> equal(3, dynamic_array_match([{int 0;1;2;3}]))   // starts with 0,1, ends with 2,3
        t |> equal(3, dynamic_array_match([{int 0;1;12;11;10;2;3}]))   // starts with 0,1, ends with 2,3
    t |> run("match_expr") <| @@ ( t : T? )
        t |> equal( true, ascending_array_match([[int 1;2;3]]))
        t |> equal( true, ascending_array_match([[int 0;1;2]]))
        t |> equal( false, ascending_array_match([[int 1;1;2]]))
    t |> run("match tuple") <| @@ ( t : T? )
        t |> equal(1, tuple_match([[auto 1,2.0,"3"]]))
        t |> equal(0, tuple_match([[auto 1,2.0,"4"]]))
        t |> equal(2, tuple_match([[auto 13,2.0,"4"]]))
        t |> equal(3, tuple_match([[auto 11,2.0,"13"]]))
        t |> equal(4, tuple_match([[auto 2,2.0,"2"]]))
    t |> run("or match") <| @@ ( t : T? )
        t |> equal(true, or_match(1))
        t |> equal(true, or_match(2))
        t |> equal(false, or_match(3))
    t |> run("or match with variable") <| @@ ( t : T? )
        t |> equal(1., or_match_v([[Bar a=1, b=1.]]))
        t |> equal(1., or_match_v([[Bar a=2, b=1.]]))
        t |> equal(0., or_match_v([[Bar a=3, b=1.]]))
    t |> run("struct ptr match") <| @@ ( t : T? )
        t |> equal( -1, struct_ptr_match(null) )
        t |> equal( 0, struct_ptr_match(new [[Foo a=13]]) )
        t |> equal( 23, struct_ptr_match(new [[Foo a=23]]) )

