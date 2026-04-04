options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require dastest/testing_boost public
require peg/peg

// number built-in
def parse_number(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var num : int
        rule(number as n, EOF) {
            return n
        }
    }
}

// double_ built-in
def parse_double(input : string; blk : block<(val : double; err : array<ParsingError>) : void>) {
    parse(input) {
        var dbl : double
        rule(double_ as d, EOF) {
            return d
        }
    }
}

// string_ built-in
def parse_string(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var str : string
        rule(string_ as s, EOF) {
            return s
        }
    }
}

// WS built-in
def parse_ws(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var ws : bool
        rule("a", WS, "b", EOF) {
            return true
        }
    }
}

// TS built-in (tab or space only)
def parse_ts(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var ts : bool
        rule("a", TS, "b", EOF) {
            return true
        }
    }
}

// EOF built-in
def parse_eof(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var eof : bool
        rule(EOF) {
            return true
        }
    }
}

// EOL built-in
def parse_eol(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var eol : bool
        rule("a", EOL, "b", EOF) {
            return true
        }
    }
}

// any built-in
def parse_any(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var a : bool
        rule(any, any, any, EOF) {
            return true
        }
    }
}

[test]
def test_number_basic(t : T?) {
    t |> run("simple integer") @(t : T?) {
        parse_number("42") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 42)
        }
    }
    t |> run("zero") @(t : T?) {
        parse_number("0") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 0)
        }
    }
    t |> run("large number") @(t : T?) {
        parse_number("123456") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 123456)
        }
    }
    t |> run("letter fails") @(t : T?) {
        parse_number("abc") $(val; err) {
            t |> success(!empty(err))
        }
    }
}

[test]
def test_double_basic(t : T?) {
    t |> run("integer as double") @(t : T?) {
        parse_double("42") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 42.0lf)
        }
    }
    t |> run("decimal") @(t : T?) {
        parse_double("3.14") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 3.14lf)
        }
    }
    t |> run("negative") @(t : T?) {
        parse_double("-1.5") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, -1.5lf)
        }
    }
    t |> run("scientific notation") @(t : T?) {
        parse_double("1.5e3") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 1500.0lf)
        }
    }
    t |> run("scientific with negative exponent") @(t : T?) {
        parse_double("2.5e-2") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 0.025lf)
        }
    }
}

[test]
def test_string_literal(t : T?) {
    t |> run("simple string") @(t : T?) {
        parse_string("\"hello\"") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "hello")
        }
    }
    t |> run("empty string") @(t : T?) {
        parse_string("\"\"") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "")
        }
    }
    t |> run("string with spaces") @(t : T?) {
        parse_string("\"hello world\"") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "hello world")
        }
    }
    t |> run("unclosed string fails") @(t : T?) {
        parse_string("\"hello") $(val; err) {
            t |> success(!empty(err), "unclosed string should fail")
        }
    }
}

[test]
def test_ws_whitespace(t : T?) {
    t |> run("space between tokens") @(t : T?) {
        parse_ws("a b") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("tab between tokens") @(t : T?) {
        parse_ws("a\tb") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("multiple spaces") @(t : T?) {
        parse_ws("a   b") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("no space") @(t : T?) {
        parse_ws("ab") $(val; err) {
            t |> success(err |> empty, "WS matches zero whitespace too")
        }
    }
}

[test]
def test_ts_tab_or_space(t : T?) {
    t |> run("space") @(t : T?) {
        parse_ts("a b") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("tab") @(t : T?) {
        parse_ts("a\tb") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("no space") @(t : T?) {
        parse_ts("ab") $(val; err) {
            t |> success(err |> empty, "TS matches zero tabs/spaces too")
        }
    }
}

[test]
def test_eof(t : T?) {
    t |> run("empty input matches EOF") @(t : T?) {
        parse_eof("") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, true)
        }
    }
    t |> run("non-empty input fails EOF") @(t : T?) {
        parse_eof("x") $(val; err) {
            t |> success(!empty(err))
        }
    }
}

[test]
def test_eol(t : T?) {
    t |> run("LF between tokens") @(t : T?) {
        parse_eol("a\nb") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("CRLF between tokens") @(t : T?) {
        parse_eol("a\r\nb") $(val; err) {
            t |> success(err |> empty)
        }
    }
}

[test]
def test_any(t : T?) {
    t |> run("three chars match") @(t : T?) {
        parse_any("abc") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("three digits match") @(t : T?) {
        parse_any("123") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("two chars fail (need 3)") @(t : T?) {
        parse_any("ab") $(val; err) {
            t |> success(!empty(err))
        }
    }
    t |> run("four chars fail (EOF after 3)") @(t : T?) {
        parse_any("abcd") $(val; err) {
            t |> success(!empty(err))
        }
    }
}
