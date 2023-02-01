require dastest/testing_boost public
require daslib/match

enum Color
    black
    red
    green
    blue

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
