require dastest/testing_boost public
require daslib/match

enum Color
    black
    red
    green
    blue

def enum_match ( color:Color ) : int
    match(color) <|
        to(Color black) <|
            return 0
        to(Color red) <|
            return 1
        to($v(anything)) <|
            return -1

def enum_static_match ( color ) : int
    static_match(color) <|
        to(Color black) <|
            return 0
        to(1234) <|
            return 1
        to($v(anything)) <|
            return -1

[test]
def test_all_match ( t:T? )
    t |> run("enum match") <| @@ ( t : T? )
        t |> equal ( 0, enum_match(Color black) )
        t |> equal ( 1, enum_match(Color red) )
        t |> equal ( -1, enum_match(Color green) )
        t |> equal ( -1, enum_match(Color blue) )
    t |> run("enum static match") <| @@ ( t : T? )
        t |> equal ( 0, enum_static_match(Color black) )
        t |> equal ( 1, enum_static_match(1234) )
        t |> equal ( -1, enum_static_match(5678) )
        t |> equal ( -1, enum_static_match("a") )
