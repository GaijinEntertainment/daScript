options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

require dastest/testing_boost
require peg/peg


def parse_expr( calc:string; blk: block<( val:int; err:array<ParsingError> ):void> )
    parse calc
        var expr: int // Place EOF to consume all input (for error reporting)

        rule(add as a, WS, EOF) <|
            return a

        var add: int

        rule(add as a, WS, "+", commit, WS, mul as m) <|
            return a + m
        rule(add as a, WS, "-", commit, WS, mul as m) <|
            return a - m
        rule(mul as m) <|
            return m

        var mul: int

        rule(mul as m, WS, "*", WS, unary as u) <|
            return m * u
        rule(mul as m, WS, "/", WS, unary as u) <|
            return m / u
        rule(unary as u) <|
            return u

        var unary: int

        rule("-", commit, WS, unary as u) <|
            return u
        rule(prim as p) <|
            return p

        var prim: int

        rule("(", commit, WS, add as a, WS, ")", WS) <|
            return a
        rule( PEEK(set('0'..'9')), commit,
                number as n ) <|
            return n


[test]
def test_calc_1(t: T?)
    let input = "1+2*2+1---(3*2 +1*2)-1-1-1*2 "
    parse_expr(input) <| $(res;err)
        t |> equal(-6, res)

[test]
def test_calc_2(t: T?)
    let input = "5-2*3+4/2"
    parse_expr(input) <| $(res;err)
        t |> equal(1, res)


[test]
def test_calc_3(t: T?)
    let input = "4*(2+3)-6"
    parse_expr(input) <| $(res;err)
        t |> equal(14, res)


[test]
def test_calc_4(t: T?)
    let input = "(2+3)*4-6"
    parse_expr(input) <| $(res;err)
        t |> equal(14, res)

// Parsing fails

[test]
def test_calc_error_1(t: T?)
    let input = "4-"
    parse_expr(input) <| $(res;err)
        if err |> empty
            t |> failure("Should fail")

[test]
def test_ctest_calc_error_2(t: T?)
    let input = "(2"
    parse_expr(input) <| $(res;err)
        if err |> empty
            t |> failure("Should fail")

[test]
def test_calc_error_3(t: T?)
    let input = "2+3)"
    parse_expr(input) <| $(res;err)
        if err |> empty
            t |> failure("Should fail")

[test]
def test_calc_error_4(t: T?)
    let input = "2+*3"
    parse_expr(input) <| $(res;err)
        if err |> empty
            t |> failure("Should fail")

[test]
def test_calc_error_5(t: T?)
    let input = "4*(2+3-6"
    parse_expr(input) <| $(res;err)
        if err |> empty
            t |> failure("Should fail")

[test]
def test_calc_error_6(t: T?)
    let input = "2*3*"
    parse_expr(input) <| $(res;err)
        if err |> empty
            t |> failure("Should fail")
