options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

options stack = 1000000000


require dastest/testing_boost

require daslib/strings_boost
require daslib/utf8_utils
require daslib/defer
require daslib/json
require daslib/fio

require peg/peg


[export]
def main
    var file = fopen("canada.json", "rb")
    defer <|
        fclose(file)
    var input = file |> fread

    profile(10, "PEG json") <|
        get_parser_with(input) <| $(res_json; err)
            print("{intptr(unsafe(reinterpret<JsonValue?> addr(res_json)))}")

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
