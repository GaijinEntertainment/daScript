options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

// options log_infer_passes

require strings
require daslib/strings_boost

require peg/peg


[export]
def main
    var parser <- [[calcParser]]

    let input = "1+2*2+1---(3*2+1*2)-1-1-1*2"

    for char in input
        parser.input |> push(char |> uint8())

    parse calc
        var add: int

        rule(add as a, "+", mul as m) <|
            return a + m
        rule(add as a, "-", mul as m) <|
            return a - m
        rule(mul as m) <|
            return m

        var mul: int

        rule(mul as m, "*", unary as u) <|
            return m * u
        rule(mul as m, "/", unary as u) <|
            return m / u
        rule(unary as u) <|
            return u

        var unary: int

        rule("-", unary as u) <|
            return u
        rule(prim as p) <|
            return p

        var prim: int

        rule("(", add as a, ")") <|
            return a
        rule(number as n) <|
            return n

    var res <- parser |> parse_add

    print("Assert {res.value} == -6\n")
