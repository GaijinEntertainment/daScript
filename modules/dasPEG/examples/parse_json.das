options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

require strings
require daslib/strings_boost
require daslib/json

require peg/peg

// Taken from daslib/json


// struct JsonValue
//     //! JSON value, wraps any JSON element.
//     value : JsValue



// variant JsValue
//     //! Single JSON element.
//     _object : table<string; JsonValue?>
//     _array  : array<JsonValue?>
//     _string : string
//     _number : double
//     _bool   : bool
//     _null   : void?



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


// Creation helpers


// def JV ( v : string )
//     //! Creates `JsonValue` out of value.
//     return new [[JsonValue value <- [[JsValue _string = v]]]]

// def JV ( v : double )
//     return new [[JsonValue value <- [[JsValue _number = v]]]]

// def JV ( v : bool )
//     return new [[JsonValue value <- [[JsValue _bool = v]]]]

// def JVNull
//     //! Creates `JsonValue` representing `null`.
//     return new [[JsonValue value <- [[JsValue _null = null]]]]

// def JV ( var v : table<string;JsonValue?> )
//     return new [[JsonValue value <- [[JsValue _object <- v]]]]

// def JV ( var v : array<JsonValue?> )
//     return new [[JsonValue value <- [[JsValue _array <- v]]]]


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
            print("Parsed element\n")
            return <- JV(a)

        // rule(Object as o) <|
        //     return <- JV(o)

        rule(Value as v) <|
            return <- v





        var Array: array<JsonValue?>

        // Optional element list;
        // If items are present (not []), then no trailing comma is allowed

        rule("[", WS, MB(ElementList) as list, "]", WS) <|
            print("Parsed Array\n")
            return <- [[array<JsonValue?>]] if list |> empty
            print("List non empty\n")
            return <- list[0] // Take the value from optional


        var ElementList: array<JsonValue?>

        // Multiple (possibly none) comma-separated elements followed by the final one
        // If the list is not empty, there must always be a final one

        rule(*CommaSeparatedElements as els, Element as last) <|
            print("Parsed element list\n")
            els |> push <| last
            return <- els


        var CommaSeparatedElements: JsonValue?

        // Helper to shave off commas

        rule(Element as e, ",", WS) <|
            print("Parsed comma sep\n")
            return <- e




        // var Object: table<string; JsonValue?>

        // rule("\{", WS, MB(MappingList) as list, "\}", WS) <|
        //     var empty: table<string; JsonValue?>
        //     return <- empty if list |> empty
        //     return <- list[0] |> into_table
        

        // var MappingList: array<tuple<string; JsonValue?>>

        // rule(*CommaSeparatedMappings as els, Mapping as last) <|
        //     els |> push <| last
        //     return <- els


        // var CommaSeparatedMappings: tuple<string; JsonValue?>

        // rule(Mapping as m, ",", WS) <|
        //     return <- m


        // var Mapping: tuple<string; JsonValue?>

        // rule(string_ as s, WS, ":", WS, Element as e) <|
        //     return <- [[tuple<string; JsonValue?> s, e]]






        var Value: JsonValue?

        rule("true", WS) <|
            print("Parsed true\n")
            return <- JV(true)

        rule("false", WS) <|
            return <- JV(false)

        rule("null", WS) <|
            return <- JVNull()
            
        // rule(double_ as d, WS) <|  // <- builtin double_ rule
        //     return <- JV(d)

        rule(string_ as s, WS) <|  // <- builtin string_ rule
            print("Parsed sring\n")
            return <- JV(s)



    var parser <- [[jsonParser]] 
    parser.input = "[\"123\", true,[], [[true]], \"123\"]"

    var result <- parser |> parse_JsonValue()

    var r <- parser |> parse_JsonValue()

    print ("{result.value |> write_json}\n")

    return