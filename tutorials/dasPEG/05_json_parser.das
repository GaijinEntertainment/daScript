// Tutorial PEG-05: JSON Parser
//
// This tutorial covers:
//   - MB() optional matching
//   - string_ and double_ built-in terminals
//   - Complex recursive grammars (arrays in arrays, objects in objects)
//   - tuple and table return types
//   - Helper functions outside the grammar
//   - Escaped characters in literals ("\{", "\}")
//   - The comma-separated list pattern (the canonical PEG idiom)
//
// Prerequisites: Tutorial PEG-03
//
// Run: daslang.exe tutorials/dasPEG/05_json_parser.das

options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 10000000

require peg/peg
require daslib/json
require daslib/stringify


// ──────────────────────────────────────────────────────────────────────────
// Helper: convert array of key-value tuples to a table
// ──────────────────────────────────────────────────────────────────────────

def into_table(var src : array<tuple<string; JsonValue?>>) : table<string; JsonValue?> {
    var result : table<string; JsonValue?>
    for (elem in src) {
        result |> insert(elem._0, elem._1)
    }
    return <- result
}


// ──────────────────────────────────────────────────────────────────────────
// The JSON Grammar
// ──────────────────────────────────────────────────────────────────────────
//
// This is a complete JSON parser producing daslib/json JsonValue? trees.
// It demonstrates deep recursive grammars and all PEG features together.

def parse_json(input : string; blk : block<(var val : JsonValue?# implicit; err : array<ParsingError>) : void>) {
    parse(input) {
        var json_value : JsonValue?

        rule(WS, element as e, EOF) {
            return <- e
        }

        // An element is a value, array, or object
        var element : JsonValue?

        rule(array_ as a) {
            return <- JV(a)
        }
        rule(object_ as o) {
            return <- JV(o)
        }
        rule(value as v) {
            return <- v
        }

        // Array: [ elements ]
        var array_ : array<JsonValue?>

        rule("[", WS, "]", WS) {
            return <- array<JsonValue?>()
        }
        rule("[", WS, element_list as list, "]", WS) {
            return <- list
        }

        // Comma-separated element list (canonical PEG idiom)
        var element_list : array<JsonValue?>

        rule(*comma_element as els, element as last) {
            els |> push <| last
            return <- els
        }

        var comma_element : JsonValue?
        rule(element as e, ",", WS) {
            return <- e
        }

        // Object: { mappings }
        var object_ : table<string; JsonValue?>

        rule("\{", WS, "\}", WS) {
            var empty : table<string; JsonValue?>
            return <- empty
        }
        rule("\{", WS, mapping_list as list, "\}", WS) {
            return <- list |> into_table
        }

        var mapping_list : array<tuple<string; JsonValue?>>

        rule(*comma_mapping as els, mapping as last) {
            els |> push <| last
            return <- els
        }

        var comma_mapping : tuple<string; JsonValue?>
        rule(mapping as m, ",", WS) {
            return <- m
        }

        var mapping : tuple<string; JsonValue?>
        rule(string_ as s, WS, ":", WS, element as e) {
            return <- (s, e)
        }

        // Primitive values
        var value : JsonValue?

        rule("true", WS) {
            return <- JV(true)
        }
        rule("false", WS) {
            return <- JV(false)
        }
        rule("null", WS) {
            return <- JVNull()
        }
        rule(double_ as d, WS) {
            return <- JV(d)
        }
        rule(string_ as s, WS) {
            return <- JV(s)
        }
    }
}


[export]
def main {
    // Simple values
    print("=== Simple values ===\n")
    parse_json("42") $(var val; err) {
        print("  42 -> {write_json(val)}\n")
    }
    parse_json("\"hello\"") $(var val; err) {
        print("  \"hello\" -> {write_json(val)}\n")
    }
    parse_json("true") $(var val; err) {
        print("  true -> {write_json(val)}\n")
    }

    // Arrays
    print("\n=== Arrays ===\n")
    parse_json("[1, 2, 3]") $(var val; err) {
        print("  [1,2,3] -> {write_json(val)}\n")
    }

    // Objects
    print("\n=== Objects ===\n")
    let obj_input = %stringify~{"name": "Alice", "age": 30, "active": true}%%
    parse_json(obj_input) $(var val; err) {
        print("  -> {write_json(val)}\n")
    }

    // Nested structure
    print("\n=== Nested ===\n")
    let nested = %stringify~{"users": [{"name": "Alice"}, {"name": "Bob"}], "count": 2}%%
    parse_json(nested) $(var val; err) {
        print("  -> {write_json(val)}\n")
    }

    // Verify against stdlib JSON parser
    print("\n=== Verification ===\n")
    let test_input = %stringify~[1, "hello", true, null, {"key": [2, 3]}]%%
    var discard_error : string
    var stdlib_json <- read_json(test_input, discard_error)
    parse_json(test_input) $(var peg_json; err) {
        let match_ = write_json(stdlib_json) == write_json(peg_json)
        print("  stdlib vs PEG match: {match_}\n")
    }
}

// Expected output (JSON pretty-printed by write_json):
// === Simple values ===
//   42 -> 42
//   "hello" -> "hello"
//   true -> true
//
// === Arrays ===
//   [1,2,3] -> [
// 	1,
// 	2,
// 	3
// ]
//
// === Objects ===
//   -> {
// 	"active" : true,
// 	"age" : 30,
// 	"name" : "Alice"
// }
//
// === Nested ===
//   -> {
// 	"count" : 2,
// 	"users" : [{"name":"Alice"}, {"name":"Bob"}]
// }
//
// === Verification ===
//   stdlib vs PEG match: true
