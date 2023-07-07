options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

require strings
require daslib/strings_boost
require daslib/utf8_utils
require daslib/json

require peg/peg

// Grammar:
//
// # A JSON text is a sequence of whitespace followed by an element
// Json <- Spacing Element EOF
//
// # An element is an object, an array or a value
// Element <- Object / Array / Value
//
// # A JSON object is an unordered set of name/value pairs
// Object <- '{' Spacing Pair (',' Spacing Pair)* '}' Spacing
//
// Pair <- String ':' Spacing Element
//
// # A JSON array is an ordered collection of values
// Array <- '[' Spacing Element (',' Spacing Element)* ']' Spacing
//
// # A JSON value can be a string, number, "true", "false", or "null"
// Value <- String / Number / 'true' Spacing / 'false' Spacing / 'null' Spacing
//
// # JSON strings are surrounded by double quotes and support a backslash escaping syntax
// String <- '"' (!'"' Character)* '"' Spacing
// Character <- '\\' Escape / !'\\' .
//
// Escape <- ["\\/bfnrt] / 'u' [0-9A-Fa-f]{4}
//


def into_table(var src: array<tuple<auto(K); auto(V)>>): table<K; V>
    var result: table<K; V>
    for elem in src
        result[elem._0] <- elem._1
    return <- result


// Parsing specification


[export]
def main

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



    var parser <- [[jsonParser]] 


    // let input = "\{\"a\" : 123.53, \"b\": true \}"
    let input = "[\"123\", true,[], \{\}, \{\"hello\" : true, \"less\": \"more\"\}, [[true]], \"123\"]"

    if !is_utf8_string_valid(input)
        print("Invalid utf-8 in the parsed string")
        return

    for char in input
        parser.input |> push(char |> uint8())

    var result <- parser |> parse_JsonValue()

    print ("{result.value |> write_json}\n")

    return
