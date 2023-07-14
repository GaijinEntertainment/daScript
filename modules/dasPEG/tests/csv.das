options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

options stack = 1000000000

require daslib/debug
options debugger

require dastest/testing_boost
require daslib/strings_boost
require daslib/utf8_utils
require daslib/fio
require peg/peg


options log

def get_parser_with(input: string): csvParser
    parse csv
        var Document: array<array<string>>

        rule(*Row as rows, EOF) <|
            return <- rows

        var Row: array<string>

        rule(*CommarSeparatedCell as cells, Cell as c, EOL) <|
            cells |> push <| c
            return <- cells

        var CommarSeparatedCell: string

        rule(Cell as c, ",") <|
            return <- c

        var Cell: string

        rule(QuotedCell as qc) <|
            return <- qc
        rule(NonQuotedCell as nqc) <|
            return <- nqc
        rule(PEEK(",")) <|  // allows empty cells
            return ""






        var QuotedCell: string

        rule("\"", "{EscapedChars}" as chars, "\"") <|
            return chars

        var EscapedChars: void?

        rule(*EscapedChar) <|
            return null

        var EscapedChar: void?

        rule("\"\"") <|
            return null
        rule(!"\"", any) <|
            return null




        var NonQuotedCell: string

        rule("{CellContents}" as contents) <|
            return contents

        var CellContents: void?

        rule(*CellSymbol) <|
            return null

        var CellSymbol: void?

        // Cell symbol is any symbol that is not NotCellContents
        rule(!NotCellContents, any) <|
            return null

        var NotCellContents: void?

        rule(EOL) <|
            return null
        rule(",") <|
            return null

        option(color)
        option(tracing)

        var EOL: void?

        rule("\r\n") <|
            return null
        rule("\n") <|
            return null
        rule(EOF) <|
            return null


    var parser <- [[csvParser]]

    for char in input
        parser.input |> push(char |> uint8())

    parser |> move_to_next_char
    parser.color_output = true

    return <- parser


// [test]
// def test_basic_csv(t: T?)
//     var input = "John,Doe,30"

//     var inscope parser <- get_parser_with(input)
//     var inscope res <- parser |> parse_Row
//     var inscope expected_res <-  [{string "John"; "Doe"; "30"}]

//     for a, b in expected_res, res.value
//         t |> equal(a, b)


// [test]
// def test_quoted_fields(t: T?)
//     var input = "\"John Doe\",\"123, Main Street, NY\",\"123-456-7890\""

//     var inscope parser <- get_parser_with(input)
//     var inscope res <- parser |> parse_Row
//     var inscope expected_res <-  [{string "John Doe"; "123, Main Street, NY"; "123-456-7890"}]

//     for a, b in expected_res, res.value
//         t |> equal(a, b)


// [test]
// def test_newline_fields(t: T?)
//     var input = "\"John Doe\",\"Hello, world!\nNice to meet you.\""

//     var inscope parser <- get_parser_with(input)
//     var inscope res <- parser |> parse_Row
//     var inscope expected_res <-  [{string "John Doe"; "Hello, world!\nNice to meet you."}]

//     for a, b in expected_res, res.value
//         t |> equal(a, b)


// [test]
// def test_special_chars(t: T?)
//     var input = "\"John Doe\",\"john.doe@example.com\""

//     var inscope parser <- get_parser_with(input)
//     var inscope res <- parser |> parse_Row
//     var inscope expected_res <-  [{string "John Doe"; "john.doe@example.com"}]

//     for a, b in expected_res, res.value
//         t |> equal(a, b)


// [test]
// def test_missing_fields(t: T?)
//     var input = "\"John Doe\",30,"

//     var inscope parser <- get_parser_with(input)
//     var inscope res <- parser |> parse_Row
//     var inscope expected_res <-  [{string "John Doe"; "30"; ""}]

//     for a, b in expected_res, res.value
//         t |> equal(a, b)


// [test]
// def test_different_types_of_data(t: T?)
//     var input = "1,\"John Doe\",True,100000,\"2023-06-01\""

//     var inscope parser <- get_parser_with(input)
//     var inscope res <- parser |> parse_Row
//     var inscope expected_res <-  [{string "1"; "John Doe"; "True"; "100000"; "2023-06-01"}]

//     for a, b in expected_res, res.value
//         t |> equal(a, b)


// [test]
// def test_escaped_quotes(t: T?)
//     var input = "1,\"She said, \"\"Hello, world!\"\"\""

//     var inscope parser <- get_parser_with(input)
//     var inscope res <- parser |> parse_Row
//     var inscope expected_res <-  [{string "1"; "She said, \"\"Hello, world!\"\""}]

//     for a, b in expected_res, res.value
//         t |> equal(a, b)


// [test]
// def test_leading_trailing_spaces(t: T?)
//     var input = " John Doe , 123 Main Street, NY , 123-456-7890 "

//     var inscope parser <- get_parser_with(input)
//     var inscope res <- parser |> parse_Row
//     var inscope expected_res <-  [{string " John Doe "; " 123 Main Street"; " NY "; " 123-456-7890 "}]

//     for a, b in expected_res, res.value
//         t |> equal(a, b)


[test]
def test_all_empty_fields(t: T?)
    var input = ","

    var inscope parser <- get_parser_with(input)
    var inscope res <- parser |> parse_Row
    var inscope expected_res <-  [{string ""; ""; ""}]

    for a, b in expected_res, res.value
        t |> equal(a, b)


// [test]
// def test_more(t: T?)
//     var input = "1,,\"\"\" A famous quote \"\"\""

//     var inscope parser <- get_parser_with(input)
//     var inscope res <- parser |> parse_Row
//     var inscope expected_res <-  [{string "1"; ""; "\"\" A famous quote \"\""}]

//     for a, b in expected_res, res.value
//         t |> equal(a, b)
