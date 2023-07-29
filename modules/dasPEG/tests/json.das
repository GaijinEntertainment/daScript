options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

require dastest/testing_boost
require daslib/stringify
require daslib/utf8_utils
require daslib/json
require peg/peg

options stack = 10000000

def into_table(var src: array<tuple<auto(K); auto(V)>>): table<K; V>
    var result: table<K; V>
    for elem in src
        result[elem._0] <- elem._1
    return <- result


def get_parser_with( json:string; blk:block<(var res_json: JsonValue? # implicit; errors:array<ParsingError>):void> )
    parse json
        var JsonValue: JsonValue?

        rule(WS, Element as e, EOF) <|
            return <- e



        var Element: JsonValue?

        rule(Array as a) <|
            return <- JV(a)

        rule(Object as o) <|
            return <- JV(o)

        rule(Value as v) <|
            return <- v





        var Array: array<JsonValue?>

        // Optional element list;
        // If items are present (not []), then no trailing comma is allowed

        rule("[", WS, "]", WS) <|
            return <- [[array<JsonValue?>]]

        rule("[", WS, ElementList as list, "]", WS) <|
            return <- list


        var ElementList: array<JsonValue?>

        // Multiple (possibly none) comma-separated elements followed by the final one
        // If the list is not empty, there must always be a final one

        rule(*CommaSeparatedElements as els, Element as last) <|
            els |> push <| last
            return <- els


        var CommaSeparatedElements: JsonValue?

        // Helper to shave off commas

        rule(Element as e, ",", WS) <|
            return <- e




        var Object: table<string; JsonValue?>

        rule("\{", WS, "\}", WS) <|
            var empty: table<string; JsonValue?>
            return <- empty

        rule("\{", WS, MappingList as list, "\}", WS) <|
            return <- list |> into_table

        var MappingList: array<tuple<string; JsonValue?>>

        rule(*CommaSeparatedMappings as els, Mapping as last) <|
            els |> push <| last
            return <- els


        var CommaSeparatedMappings: tuple<string; JsonValue?>

        rule(Mapping as m, ",", WS) <|
            return <- m


        var Mapping: tuple<string; JsonValue?>

        rule(string_ as s, WS, ":", WS, Element as e) <|
            return <- [[auto s, e]]






        var Value: JsonValue?

        rule("true", WS) <|
            return <- JV(true)

        rule("false", WS) <|
            return <- JV(false)

        rule("null", WS) <|
            return <- JVNull()

        rule(double_ as d, WS) <|  // <- builtin double_ rule
            return <- JV(d)

        rule(string_ as s, WS) <|  // <- builtin string_ rule
            return <- JV(s)



[test]
def test1(t: T?)
    let input = "[[[[[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]]]]]]"

    var discard_error: string
    var json <- read_json(input, discard_error)

    get_parser_with(input) <| $(var res_json: JsonValue? # implicit; errors: array<ParsingError>)
        t |> equal(json |> write_json(), res_json |> write_json)


[test]
def test2(t: T?)
    let input = "\{\"a\" : 123.53, \"b\": true \}"

    var discard_error: string
    var json <- read_json(input, discard_error)

    get_parser_with(input) <| $(var res_json; errors)
        t |> equal(json |> write_json(), res_json |> write_json)


[test]
def test3(t: T?)
    let input = "[\"123\", true,[], \{\}, \{\"hello\" : true, \"less\": \"more\"\}, [[[true]]], \"123\"]"

    var discard_error: string
    var json <- read_json(input, discard_error)

    get_parser_with(input) <| $(var res_json; errors)
        t |> equal(json |> write_json(), res_json |> write_json)


[test]
def test4(t: T?)
    let input = "\{\"message\": \"Hello, world!\", \"nested\": \{\"nested_again\":
    \{\"is_it_deep\": true, \"numbers\": [1, 2, 3, 4]\}\}, \"array\": [false, null, \"mixed array\"]\}"

    var discard_error: string
    var json <- read_json(input, discard_error)

    get_parser_with(input) <| $(var res_json; errors)
        t |> equal(json |> write_json(), res_json |> write_json)


[test]
def test5(t: T?)
    let input = "[\{\"name\": \"John\", \"age\": 30, \"city\": \"New York\", \"siblings\":
    [\{\"name\": \"Jane\", \"age\": 25, \"city\": \"Chicago\"\},
    \{\"name\": \"Joe\", \"age\": 35, \"city\": \"San Francisco\"\}]\}, true, 123, null, [1, 2, 3]]"

    var discard_error: string
    var json <- read_json(input, discard_error)

    get_parser_with(input) <| $(var res_json; errors)
        t |> equal(json |> write_json(), res_json |> write_json)


[test]
def test6(t: T?)
    let input = "\{\"colors\": [\{\"colorName\": \"red\", \"hexValue\": \"#f00\"\},
    \{\"colorName\": \"green\", \"hexValue\": \"#0f0\"\},
    \{\"colorName\": \"blue\", \"hexValue\": \"#00f\"\},
    \{\"colorName\": \"cyan\", \"hexValue\": \"#0ff\"\},
    \{\"colorName\": \"magenta\", \"hexValue\": \"#f0f\"\},
    \{\"colorName\": \"yellow\", \"hexValue\": \"#ff0\"\},
    \{\"colorName\": \"black\", \"hexValue\": \"#000\"\}]\}"

    var discard_error: string
    var json <- read_json(input, discard_error)

    get_parser_with(input) <| $(var res_json; errors)
        t |> equal(json |> write_json(), res_json |> write_json)


[test]
def test_json_error_1(t: T?)
    let input = %stringify~{
"name": "John",
"city": "New York"
%%
    // Error: Expected the terminal `;` ...
    get_parser_with(input) <| $(res; err)
        if err |> empty
            t |> failure <| "Should not parse missing right curly bracket"


[test]
def test_json_error_2(t: T?)
    let input = %stringify~{
  "name": "John",
  "age" 30,
  "city": "New York"
}%%
    // Error: Expected the terminal `:` at 30, around; 30
    get_parser_with(input) <| $(res; err)
        if err |> empty
            t |> failure <| "Should not parse missing semilcolon"

[test]
def test_json_error_3(t: T?)
    let input = %stringify~"city": "New York"
}%%
    // Error: Expected EOF at 6
    get_parser_with(input) <| $(res; err)
        if err |> empty
            t |> failure <| "Should not parse missing semilcolon"


[test]
def test_json_error_4(t: T?)
    let input = %stringify~{
  "name": "John,
  "age": 30,
  "city": "New York"
}%%
    // Unhelpful error, though

    // Error: Expected the terminal `,` at 24, around; 24
    // Error: Expected the terminal `}` at 24, around; 24
    get_parser_with(input) <| $(res; err)
        if err |> empty
            t |> failure <| "Should not parse missing semilcolon"


[test]
def test_json_error_5(t: T?)
    let input = %stringify~
{
  "name": "John",
  "age": 30,
  "hobbies": ["reading",
  "cooking"
}
}%%
    // Error: Expected the terminal `]` at 77, around; 77
    // Error: Expected the terminal `,` at 77, around; 77
    get_parser_with(input) <| $(res; err)
        if err |> empty
            t |> failure <| "Should not parse missing semilcolon"
