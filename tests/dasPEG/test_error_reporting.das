options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require dastest/testing_boost public
require peg/peg

// Parser with commit: produces errors
def parse_with_commit(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule("val", WS, "=", commit, WS, number as n, ";", EOF) {
            return n
        }
    }
}

// Parser without commit: may not produce errors
def parse_no_commit(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule("val", WS, "=", WS, number as n, ";", EOF) {
            return n
        }
    }
}

// Parser with commit that has error position
def parse_keyword(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule("print", commit, WS, string_ as s, EOF) {
            return s
        }
    }
}

[test]
def test_commit_generates_errors(t : T?) {
    t |> run("missing value after commit produces errors") @(t : T?) {
        parse_with_commit("val = ;") $(val; err) {
            t |> success(!empty(err), "commit should produce errors on failure")
        }
    }
}

[test]
def test_commit_error_has_index(t : T?) {
    parse_with_commit("val = ;") $(val; err) {
        if (!empty(err)) {
            t |> success(err[0].index > 0, "error should have positive index")
        } else {
            t |> failure("expected errors")
        }
    }
}

[test]
def test_commit_error_has_text(t : T?) {
    parse_with_commit("val = ;") $(val; err) {
        if (!empty(err)) {
            t |> success(length(err[0].text) > 0, "error should have non-empty text")
        } else {
            t |> failure("expected errors")
        }
    }
}

[test]
def test_successful_parse_no_errors(t : T?) {
    parse_with_commit("val = 42;") $(val; err) {
        t |> success(err |> empty, "successful parse should have no errors")
        t |> equal(val, 42)
    }
}

[test]
def test_error_position_after_keyword(t : T?) {
    parse_keyword("print") $(val; err) {
        if (!empty(err)) {
            // Error should be at or after position 5 ("print" length)
            t |> success(err[0].index >= 5, "error position should be after 'print' keyword")
        } else {
            t |> failure("expected errors after committed keyword without value")
        }
    }
}

[test]
def test_multiple_error_sources(t : T?) {
    parse_with_commit("val =") $(val; err) {
        t |> success(!empty(err), "incomplete input should produce errors")
    }
}
