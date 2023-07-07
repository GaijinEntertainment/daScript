options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

// options log_infer_passes

require strings
require daslib/strings_boost

require peg/peg


// def match_string_literal(var parser: Parser): tuple<success:bool; string>
//     //! Tries to match everything inside ""

//     // Define buffer to store string literal characters
//     var inscope buffer: das_string

//     var current_char = parser |> get_current_char

//     // If the current character is not a double quote, the rule is not a string
//     return [[auto false, ""]] if current_char != '"'

//     parser |> move(1)
//     current_char = parser |> get_current_char

//     while current_char != '"' && current_char != -1
//         buffer |> append(current_char)
//         parser |> move(1)
//         current_char = parser |> get_current_char

//     // If we've reached EOF file without finding a closing quote
//     return [[auto false, ""]] if current_char == -1

//     parser |> move(1)
//     return [[auto true, buffer |> string()]]


// def match_double_literal(var parser: Parser): tuple<success:bool; double>
//     //! Matches doubles in the form of [-+]? [0-9]* .? [0-9]+ ([eE] [-+]? [0-9]+)?
//     //! The number is not checked to be representable as defined in IEEE-754

//     var inscope buffer: array<uint8>
//     var current_char = parser |> get_current_char

//     if current_char == '-' || current_char == '+'
//         buffer |> append(current_char)
//         parser |> move(1)
//         current_char = parser |> get_current_char

//     // Add everything up to '.' to the buffer
//     while current_char |> is_number || current_char == '.'
//         buffer |> append(current_char)
//         parser |> move(1)
//         current_char = parser |> get_current_char


//     // Match exponent part
//     if current_char == 'e' || current_char == 'E'
//         buffer |> append(current_char)
//         parser |> move(1)
//         current_char = parser |> get_current_char

//         // Check for '-' or '+' after 'e' or 'E'
//         if current_char == '-' || current_char == '+'
//             buffer |> append(current_char)
//             parser |> move(1)
//             current_char = parser |> get_current_char

//         // Continue appending digits after 'e' or 'E'
//         while current_char |> is_number
//             buffer |> append(current_char)
//             parser |> move(1)
//             current_char = parser |> get_current_char

//     return [[auto true, buffer |> string() |> double()]]

[export]
def main
    var parser <- [[calcParser]]
    parser.input = "1+2*2+1---(3*2+1*2)-1-1-1*2"

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
