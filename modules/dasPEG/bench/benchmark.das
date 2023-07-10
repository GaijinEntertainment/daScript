//require daslib/profiler
require daslib/json
require daslib/strings_boost
require daslib/fio
require dastest/testing_boost
require daslib/utf8_utils
require peg/peg

options stack = 1000000000


[export]
def main
    var input <- fopen("canada.json", "r") |> fread

    profile(10, "PEG json") <|
        var parser <- get_parser_with(input)
        var res_json <- parse_JsonValue(parser).value
        print("{intptr(unsafe(addr(res_json)))}")

    profile(10, "std json") <|
        var discard_error: string
        var json <- read_json(input, discard_error)
        print("{intptr(unsafe(addr(json)))}")


[skip_lock_check]
def into_table(var src: array<tuple<auto(K); auto(V)>>): table<K; V>
    var result: table<K; V>
    for elem in src
        result[elem._0] <- elem._1
    return <- result


def get_parser_with(input: string): jsonParser
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

        rule("[", WS, MB(ElementList) as list, "]", WS) <|
            return <- [[array<JsonValue?>]] if list |> empty
            return <- list[0] // Take the value from optional


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

        rule("\{", WS, MB(MappingList) as list, "\}", WS) <|
            var empty: table<string; JsonValue?>
            return <- empty if list |> empty
            return <- list[0] |> into_table


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

        rule(double_ as d, WS) <|  // <- builtin double_ rule
            return <- JV(d)

        rule("true", WS) <|
            return <- JV(true)

        rule("false", WS) <|
            return <- JV(false)

        rule("null", WS) <|
            return <- JVNull()

        rule(string_ as s, WS) <|  // <- builtin string_ rule
            return <- JV(s)



    var parser <- [[jsonParser]]

    // TODO: move into set_input parser method

    //verify(input |> is_utf8_string_valid, "Invalid utf-8 in the parsed string")

    for char in input
        parser.input |> push(char |> uint8())

    return <- parser

