// Tutorial PEG-01: Hello Parser
//
// This tutorial covers:
//   - The parse macro and grammar structure
//   - Literal string matching
//   - EOF terminal
//   - Text extraction with "{rule}" as name
//   - Handling parse results and errors
//   - Character sets with set()
//
// Prerequisites: Basic daslang knowledge (functions, blocks, strings)
//
// Run: daslang.exe tutorials/dasPEG/01_hello_parser.das

options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require peg/peg


// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Your first parser: matching literal strings
// ──────────────────────────────────────────────────────────────────────────
//
// A PEG grammar is defined inside a `parse(input) { ... }` macro.
// The first `var` declaration is the entry rule.
// Each `rule(...)` defines an alternative the parser tries in order.

def parse_hello(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        // The entry rule — the parser starts here
        var greeting : bool

        // Try to match the literal "Hello" followed by end-of-input
        rule("Hello", EOF) {
            return true
        }
    }
}


// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Multiple alternatives (ordered choice)
// ──────────────────────────────────────────────────────────────────────────
//
// PEG parsers try alternatives in order.  The first matching alternative wins.
// This is called "ordered choice" — it eliminates ambiguity.

def parse_greeting(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var greeting : string

        rule("Hello", EOF) {
            return "hello"
        }
        rule("Hi", EOF) {
            return "hi"
        }
        rule("Hey", EOF) {
            return "hey"
        }
    }
}


// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Text extraction with "{rule}" as name
// ──────────────────────────────────────────────────────────────────────────
//
// To extract text matched by a rule, wrap it in "{...}" and bind with `as`.
// The `+rule` means "one or more" repetitions.

def parse_name(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var greeting : string

        // Match "Hello, " then extract one or more letters, then "!"
        rule("Hello, ", "{+letter}" as name, "!", EOF) {
            return name
        }

        // A helper rule that matches a single letter
        var letter : void?
        rule(set('a'..'z', 'A'..'Z')) {
            return null
        }
    }
}


// ──────────────────────────────────────────────────────────────────────────
// Section 4 — Multiple rules and WS (whitespace)
// ──────────────────────────────────────────────────────────────────────────
//
// Grammars can have multiple rules. WS matches zero or more whitespace chars.

def parse_key_value(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var entry : string

        // Match: key = "value"
        rule("{+alpha}" as key, WS, "=", WS, string_ as value, EOF) {
            return "{key}: {value}"
        }

        var alpha : void?
        rule(set('a'..'z', 'A'..'Z', '_')) {
            return null
        }
    }
}


// ──────────────────────────────────────────────────────────────────────────
// Section 5 — Error handling
// ──────────────────────────────────────────────────────────────────────────
//
// The parse callback receives both the result and an error array.
// On failure, the result is the default value for its type.

def try_parse(input : string) {
    parse_name(input) $(val; err) {
        if (!empty(err)) {
            print("  FAILED: ")
            for (e in err) {
                print("{e.text} (at position {e.index})\n")
            }
        } elif (val != "") {
            print("  name = \"{val}\"\n")
        } else {
            print("  no match\n")
        }
    }
}


[export]
def main {
    // Section 1: Simple literal matching
    print("=== Literal matching ===\n")
    parse_hello("Hello") $(val; err) {
        print("  \"Hello\" -> {val}\n")
    }
    parse_hello("Goodbye") $(val; err) {
        print("  \"Goodbye\" -> {val} (no match)\n")
    }

    // Section 2: Multiple alternatives
    print("\n=== Ordered choice ===\n")
    for (word in ["Hello", "Hi", "Hey", "Bye"]) {
        parse_greeting(word) $(val; err) {
            print("  \"{word}\" -> \"{val}\"\n")
        }
    }

    // Section 3: Text extraction
    print("\n=== Text extraction ===\n")
    parse_name("Hello, World!") $(val; err) {
        print("  name = \"{val}\"\n")
    }
    parse_name("Hello, daScript!") $(val; err) {
        print("  name = \"{val}\"\n")
    }

    // Section 4: Key-value parser
    print("\n=== Key-value ===\n")
    parse_key_value("name = \"Alice\"") $(val; err) {
        print("  {val}\n")
    }
    parse_key_value("age=\"30\"") $(val; err) {
        print("  {val}\n")
    }

    // Section 5: Error handling
    print("\n=== Error handling ===\n")
    print("  Parsing \"Hello, World!\":\n")
    try_parse("Hello, World!")
    print("  Parsing \"Hello, 123!\":\n")
    try_parse("Hello, 123!")
    print("  Parsing \"Goodbye\":\n")
    try_parse("Goodbye")
}

// Expected output:
// === Literal matching ===
//   "Hello" -> true
//   "Goodbye" -> false (no match)
//
// === Ordered choice ===
//   "Hello" -> "hello"
//   "Hi" -> "hi"
//   "Hey" -> "hey"
//   "Bye" -> ""
//
// === Text extraction ===
//   name = "World"
//   name = "daScript"
//
// === Key-value ===
//   name: Alice
//   age: 30
//
// === Error handling ===
//   Parsing "Hello, World!":
//   name = "World"
//   Parsing "Hello, 123!":
//   FAILED: (error messages with position info)
//   Parsing "Goodbye":
//   FAILED: (error messages with position info)
