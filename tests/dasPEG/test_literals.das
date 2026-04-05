options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require dastest/testing_boost public
require peg/peg

// Simple literal match: returns true if the literal is found
def parse_literal(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var lit : bool
        rule("hello", EOF) {
            return true
        }
    }
}

// Multi-word literal match
def parse_multi_literal(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var lit : bool
        rule("hello", " ", "world", EOF) {
            return true
        }
    }
}

// Escaped brace literals
def parse_braces(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var lit : bool
        rule("\{", "content", "\}", EOF) {
            return true
        }
    }
}

// Case sensitivity test
def parse_case(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var lit : bool
        rule("Hello", EOF) {
            return true
        }
    }
}

// Single character literal
def parse_single_char(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var lit : bool
        rule("x", EOF) {
            return true
        }
    }
}

[test]
def test_literal_match(t : T?) {
    t |> run("exact match") @(t : T?) {
        parse_literal("hello") $(val; err) {
            t |> success(err |> empty, "should have no errors")
            t |> equal(val, true)
        }
    }
}

[test]
def test_literal_no_match(t : T?) {
    t |> run("wrong input") @(t : T?) {
        parse_literal("world") $(val; err) {
            t |> success(!empty(err), "should have errors for non-matching input")
        }
    }
}

[test]
def test_literal_partial_match(t : T?) {
    t |> run("partial input fails") @(t : T?) {
        parse_literal("hell") $(val; err) {
            t |> success(!empty(err), "partial match should fail")
        }
    }
}

[test]
def test_literal_extra_input(t : T?) {
    t |> run("extra input after literal fails due to EOF") @(t : T?) {
        parse_literal("hello world") $(val; err) {
            t |> success(!empty(err), "extra input should fail because of EOF requirement")
        }
    }
}

[test]
def test_multi_literal(t : T?) {
    t |> run("multi-word literal") @(t : T?) {
        parse_multi_literal("hello world") $(val; err) {
            t |> success(err |> empty, "should match multi-word literal")
            t |> equal(val, true)
        }
    }
}

[test]
def test_escaped_braces(t : T?) {
    t |> run("escaped brace literals") @(t : T?) {
        parse_braces("\{content\}") $(val; err) {
            t |> success(err |> empty, "should match braces")
            t |> equal(val, true)
        }
    }
}

[test]
def test_case_sensitive(t : T?) {
    t |> run("case sensitive match") @(t : T?) {
        parse_case("Hello") $(val; err) {
            t |> success(err |> empty, "exact case should match")
            t |> equal(val, true)
        }
    }
    t |> run("wrong case fails") @(t : T?) {
        parse_case("hello") $(val; err) {
            t |> success(!empty(err), "wrong case should fail")
        }
    }
    t |> run("all caps fails") @(t : T?) {
        parse_case("HELLO") $(val; err) {
            t |> success(!empty(err), "all caps should fail")
        }
    }
}

[test]
def test_single_char_literal(t : T?) {
    t |> run("single char matches") @(t : T?) {
        parse_single_char("x") $(val; err) {
            t |> success(err |> empty, "single char should match")
            t |> equal(val, true)
        }
    }
    t |> run("wrong single char fails") @(t : T?) {
        parse_single_char("y") $(val; err) {
            t |> success(!empty(err), "wrong char should fail")
        }
    }
}

[test]
def test_empty_input(t : T?) {
    t |> run("empty input fails for non-empty literal") @(t : T?) {
        parse_literal("") $(val; err) {
            t |> success(!empty(err), "empty input should fail for 'hello' literal")
        }
    }
}
