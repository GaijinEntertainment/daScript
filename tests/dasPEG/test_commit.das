options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require dastest/testing_boost public
require peg/peg

// commit prevents backtracking, produces errors
def parse_assignment(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : bool
        rule("{+alpha}" as name, WS, "=", commit, WS, number as n, ";", EOF) {
            return true
        }
        var alpha : void?
        rule(set('a'..'z', 'A'..'Z')) {
            return null
        }
    }
}

// commit after keyword for error messages
def parse_print(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule("print", commit, WS, number as n, EOF) {
            return n
        }
    }
}

// Multiple commits in a grammar
def parse_expr(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule(add as a, EOF) {
            return a
        }
        var add : int
        rule(add as a, WS, "+", commit, WS, number as n) {
            return a + n
        }
        rule(number as n) {
            return n
        }
    }
}

[test]
def test_commit_success(t : T?) {
    t |> run("valid assignment") @(t : T?) {
        parse_assignment("x = 42;") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, true)
        }
    }
}

[test]
def test_commit_error_on_missing_value(t : T?) {
    t |> run("missing value after =") @(t : T?) {
        parse_assignment("x = ;") $(val; err) {
            t |> success(!empty(err), "should error after commit when value missing")
        }
    }
}

[test]
def test_commit_error_on_missing_semicolon(t : T?) {
    t |> run("missing semicolon") @(t : T?) {
        parse_assignment("x = 42") $(val; err) {
            t |> success(!empty(err), "should error after commit when semicolon missing")
        }
    }
}

[test]
def test_commit_error_position(t : T?) {
    t |> run("error has position info") @(t : T?) {
        parse_print("print") $(val; err) {
            t |> success(!empty(err), "should error when value missing after print")
            if (!empty(err)) {
                t |> success(err[0].index > 0, "error should have non-zero position")
            }
        }
    }
}

[test]
def test_commit_keyword_success(t : T?) {
    parse_print("print 42") $(val; err) {
        t |> success(err |> empty)
        t |> equal(val, 42)
    }
}

[test]
def test_commit_in_expression(t : T?) {
    t |> run("valid addition") @(t : T?) {
        parse_expr("1 + 2") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 3)
        }
    }
    t |> run("trailing plus errors") @(t : T?) {
        parse_expr("1 +") $(val; err) {
            t |> success(!empty(err), "trailing + should error after commit")
        }
    }
}
