options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require dastest/testing_boost public
require strings
require peg/peg

// Simple parser for edge case testing
def parse_word(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule("{+alpha}" as w, EOF) {
            return w
        }
        var alpha : void?
        rule(set('a'..'z', 'A'..'Z')) {
            return null
        }
    }
}

def parse_number(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule(number as n, EOF) {
            return n
        }
    }
}

def parse_eof_only(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : bool
        rule(EOF) {
            return true
        }
    }
}

// WS vs TS distinction
def parse_ws_test(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : bool
        rule("a", WS, "b", EOF) {
            return true
        }
    }
}

def parse_ts_test(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : bool
        rule("a", TS, "b", EOF) {
            return true
        }
    }
}

[test]
def test_empty_input(t : T?) {
    t |> run("empty input matches EOF") @(t : T?) {
        parse_eof_only("") $(val; err) {
            t |> equal(true, val)
        }
    }
    t |> run("empty input fails word parser") @(t : T?) {
        parse_word("") $(val; err) {
            t |> equal("", val)
        }
    }
}

[test]
def test_single_character(t : T?) {
    parse_word("a") $(val; err) {
        t |> equal("a", val)
    }
}

[test]
def test_long_input(t : T?) {
    let long_word = build_string() $(var writer) {
        for (_ in range(1000)) {
            writer |> write("a")
        }
    }
    parse_word(long_word) $(val; err) {
        t |> equal(long_word, val)
    }
}

[test]
def test_whitespace_only(t : T?) {
    parse_word("   ") $(val; err) {
        t |> equal("", val, "whitespace-only should fail word parser")
    }
}

[test]
def test_ws_newline(t : T?) {
    t |> run("WS accepts newlines") @(t : T?) {
        parse_ws_test("a\nb") $(val; err) {
            t |> equal(true, val)
        }
    }
    t |> run("WS accepts mixed whitespace") @(t : T?) {
        parse_ws_test("a \t\n b") $(val; err) {
            t |> equal(true, val)
        }
    }
}

[test]
def test_ts_no_newline(t : T?) {
    t |> run("TS accepts space") @(t : T?) {
        parse_ts_test("a b") $(val; err) {
            t |> equal(true, val)
        }
    }
    t |> run("TS accepts tab") @(t : T?) {
        parse_ts_test("a\tb") $(val; err) {
            t |> equal(true, val)
        }
    }
    t |> run("TS rejects newline") @(t : T?) {
        parse_ts_test("a\nb") $(val; err) {
            t |> equal(false, val, "TS should not accept newlines")
        }
    }
}

[test]
def test_large_number(t : T?) {
    parse_number("999999") $(val; err) {
        t |> equal(999999, val)
    }
}

[test]
def test_zero(t : T?) {
    parse_number("0") $(val; err) {
        t |> equal(0, val)
    }
}
