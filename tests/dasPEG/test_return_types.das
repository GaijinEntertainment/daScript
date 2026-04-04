options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require dastest/testing_boost public
require peg/peg

// Return int
def parse_int(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule(number as n, EOF) {
            return n
        }
    }
}

// Return double
def parse_dbl(input : string; blk : block<(val : double; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : double
        rule(double_ as d, EOF) {
            return d
        }
    }
}

// Return string
def parse_str(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule("{+letter}" as w, EOF) {
            return w
        }
        var letter : void?
        rule(set('a'..'z', 'A'..'Z')) {
            return null
        }
    }
}

// Return bool
def parse_bool(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : bool
        rule("true", EOF) {
            return true
        }
        rule("false", EOF) {
            return false
        }
    }
}

// Return struct
struct KeyValue {
    key : string
    value : int
}

def parse_kv(input : string; blk : block<(val : KeyValue; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : KeyValue
        rule("{+alpha}" as k, WS, "=", WS, number as v, EOF) {
            return KeyValue(key = k, value = v)
        }
        var alpha : void?
        rule(set('a'..'z', 'A'..'Z')) {
            return null
        }
    }
}

// Return tuple
def parse_pair_tuple(input : string; blk : block<(val : tuple<string; int>; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : tuple<string; int>
        rule("{+alpha}" as k, WS, ":", WS, number as v, EOF) {
            return (k, v)
        }
        var alpha : void?
        rule(set('a'..'z', 'A'..'Z')) {
            return null
        }
    }
}

// Return void? (pattern rules)
def parse_void(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : bool
        rule(digit, digit, digit, EOF) {
            return true
        }
        var digit : void?
        rule(set('0'..'9')) {
            return null
        }
    }
}

[test]
def test_return_int(t : T?) {
    parse_int("42") $(val; err) {
        t |> success(err |> empty)
        t |> equal(val, 42)
    }
}

[test]
def test_return_double(t : T?) {
    parse_dbl("3.14") $(val; err) {
        t |> success(err |> empty)
        t |> equal(val, 3.14lf)
    }
}

[test]
def test_return_string(t : T?) {
    parse_str("hello") $(val; err) {
        t |> success(err |> empty)
        t |> equal(val, "hello")
    }
}

[test]
def test_return_bool(t : T?) {
    t |> run("true") @(t : T?) {
        parse_bool("true") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, true)
        }
    }
    t |> run("false") @(t : T?) {
        parse_bool("false") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, false)
        }
    }
}

[test]
def test_return_struct(t : T?) {
    parse_kv("name = 42") $(val; err) {
        t |> success(err |> empty)
        t |> equal(val.key, "name")
        t |> equal(val.value, 42)
    }
}

[test]
def test_return_tuple(t : T?) {
    parse_pair_tuple("age : 30") $(val; err) {
        t |> success(err |> empty)
        t |> equal(val._0, "age")
        t |> equal(val._1, 30)
    }
}

[test]
def test_void_pattern_rule(t : T?) {
    t |> run("three digits") @(t : T?) {
        parse_void("123") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, true)
        }
    }
    t |> run("two digits fails") @(t : T?) {
        parse_void("12") $(val; err) {
            t |> equal(val, false, "two digits should not match three-digit pattern")
        }
    }
}
