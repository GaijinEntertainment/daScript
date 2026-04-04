options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require dastest/testing_boost public
require strings
require peg/peg

// Direct recursion: nested parentheses counting depth
def parse_nested_parens(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule(parens as p, EOF) {
            return p
        }
        var parens : int
        rule("(", parens as inner, ")") {
            return inner + 1
        }
        rule("(", ")") {
            return 1
        }
    }
}

// Mutual recursion: top -> expr -> atom -> expr (expression with parens)
def parse_mutual(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var top : int
        rule(expr as e, EOF) {
            return e
        }
        var expr : int
        rule(atom as a) {
            return a
        }
        var atom : int
        rule("(", expr as e, ")") {
            return e
        }
        rule(number as n) {
            return n
        }
    }
}

// Deep nesting stress test
def parse_deep(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule("(", expr as inner, ")") {
            return inner + 1
        }
        rule(number as n) {
            return n
        }
    }
}

[test]
def test_one_level(t : T?) {
    parse_nested_parens("()") $(val; err) {
        t |> equal(1, val)
    }
}

[test]
def test_two_levels(t : T?) {
    parse_nested_parens("(())") $(val; err) {
        t |> equal(2, val)
    }
}

[test]
def test_three_levels(t : T?) {
    parse_nested_parens("((()))") $(val; err) {
        t |> equal(3, val)
    }
}

[test]
def test_mutual_number(t : T?) {
    parse_mutual("42") $(val; err) {
        t |> equal(42, val)
    }
}

[test]
def test_mutual_parens(t : T?) {
    parse_mutual("(42)") $(val; err) {
        t |> equal(42, val)
    }
}

[test]
def test_mutual_nested(t : T?) {
    parse_mutual("((42))") $(val; err) {
        t |> equal(42, val)
    }
}

[test]
def test_deep_nesting(t : T?) {
    let depth = 20
    let input = build_string() $(var writer) {
        for (_ in range(depth)) {
            writer |> write("(")
        }
        writer |> write("42")
        for (_ in range(depth)) {
            writer |> write(")")
        }
    }
    parse_deep(input) $(val; err) {
        t |> equal(42 + depth, val)
    }
}
