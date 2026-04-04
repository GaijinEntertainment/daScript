// Tutorial PEG-06: Debugging and Options
//
// This tutorial covers:
//   - option(tracing): trace parser execution
//   - option(color): colored terminal output
//   - option(print_generated): inspect generated code
//   - log("message"): inline debug messages during parsing
//   - How commit enables meaningful error messages
//   - Reading and interpreting ParsingError results
//   - Performance tips: rule ordering, stack size
//
// Prerequisites: Tutorial PEG-02
//
// Run: daslang.exe tutorials/dasPEG/06_debugging.das

options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require peg/peg


// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Tracing parser execution
// ──────────────────────────────────────────────────────────────────────────
//
// option(tracing) shows which alternatives are tried and whether they succeed.
// This is invaluable when debugging why a grammar doesn't match.

def parse_traced(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule("hello", WS, "{+alpha}" as name, EOF) {
            return name
        }
        rule("bye", EOF) {
            return "goodbye"
        }
        var alpha : void?
        rule(set('a'..'z', 'A'..'Z')) {
            return null
        }
        option(tracing)
    }
}


// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Inline log messages
// ──────────────────────────────────────────────────────────────────────────
//
// log("message") prints during parsing.  Use string interpolation to
// include bound variables.  Log messages fire during the FIRST parse
// pass — they may fire for alternatives that later fail.

def parse_with_log(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule(number as n, log("parsed number: {n}"), WS, "+", WS, number as m, log("parsed second: {m}"), EOF) {
            return n + m
        }
        rule(number as n, log("single number: {n}"), EOF) {
            return n
        }
    }
}


// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Error reporting: commit vs no commit
// ──────────────────────────────────────────────────────────────────────────
//
// Without commit, PEG silently backtracks — errors may be empty.
// With commit, backtracking is cut off and errors are produced.

def parse_no_commit(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule("val", WS, "=", WS, number as n, ";", EOF) {
            return n
        }
    }
}

def parse_with_commit(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule("val", WS, "=", commit, WS, number as n, ";", EOF) {
            return n
        }
    }
}


// ──────────────────────────────────────────────────────────────────────────
// Section 4 — Interpreting ParsingError
// ──────────────────────────────────────────────────────────────────────────

def show_errors(errors : array<ParsingError>) {
    if (empty(errors)) {
        print("    (no errors — parser silently backtracked)\n")
    } else {
        for (e in errors) {
            print("    position {e.index}: {e.text}\n")
        }
    }
}


[export]
def main {
    // Section 1: Tracing
    print("=== Tracing (matching) ===\n")
    parse_traced("hello world") $(val; err) {
        print("  result: \"{val}\"\n")
    }
    print("\n=== Tracing (backtracking) ===\n")
    parse_traced("bye") $(val; err) {
        print("  result: \"{val}\"\n")
    }

    // Section 2: Log messages
    print("\n=== Log messages ===\n")
    parse_with_log("3 + 5") $(val; err) {
        print("  result: {val}\n")
    }
    parse_with_log("42") $(val; err) {
        print("  result: {val}\n")
    }

    // Section 3: Commit vs no commit
    print("\n=== Error reporting: without commit ===\n")
    print("  Parsing \"val = ;\"\n")
    parse_no_commit("val = ;") $(val; err) {
        show_errors(err)
    }

    print("\n=== Error reporting: with commit ===\n")
    print("  Parsing \"val = ;\"\n")
    parse_with_commit("val = ;") $(val; err) {
        show_errors(err)
    }

    // Section 4: Performance tips
    print("\n=== Performance tips ===\n")
    print("  1. Place most common alternatives first (PEG tries in order)\n")
    print("  2. Use PEEK to quickly reject impossible alternatives\n")
    print("  3. Use commit after unambiguous prefixes for better errors\n")
    print("  4. Set options stack = 1000000 (PEG needs more stack than default)\n")
    print("  5. PEG uses packrat memoization — cache results per rule per position\n")
}

// Expected output (tracing output abbreviated):
// === Tracing (matching) ===
//   (trace lines showing parser trying alternatives)
//   result: "world"
//
// === Tracing (backtracking) ===
//   (trace lines showing alternative 1 failing, alternative 2 matching)
//   result: "goodbye"
//
// === Log messages ===
//   parsed number: 3  parsed second: 5  result: 8
//   parsed number: 42  single number: 42  result: 42
//
// === Error reporting: without commit ===
//   Parsing "val = ;"
//     position 6: Error: Expected a decimal number at 6
//
// === Error reporting: with commit ===
//   Parsing "val = ;"
//     position 6: Error: Expected a decimal number at 6
//
// === Performance tips ===
//   1-5. (tips printed)
