options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require dastest/testing_boost public
require peg/peg

// MB (optional): present or absent
def parse_optional_sign(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule(MB(minus), number as n, EOF) {
            return n
        }
        var minus : bool
        rule("-") {
            return true
        }
    }
}

// MB with binding
def parse_optional_suffix(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule("{+alpha}" as word, MB(suffix) as sfx, EOF) {
            return word
        }
        var alpha : void?
        rule(set('a'..'z', 'A'..'Z')) {
            return null
        }
        var suffix : bool
        rule("!") {
            return true
        }
    }
}

// Optional with typed result
def parse_optional_default(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule("val", WS, "=", WS, number as n, EOF) {
            return n
        }
        rule("val", EOF) {
            return 0
        }
    }
}

[test]
def test_optional_present(t : T?) {
    t |> run("minus present") @(t : T?) {
        parse_optional_sign("-42") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 42)
        }
    }
}

[test]
def test_optional_absent(t : T?) {
    t |> run("no minus") @(t : T?) {
        parse_optional_sign("42") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 42)
        }
    }
}

[test]
def test_optional_suffix_present(t : T?) {
    parse_optional_suffix("hello!") $(val; err) {
        t |> success(err |> empty)
        t |> equal(val, "hello")
    }
}

[test]
def test_optional_suffix_absent(t : T?) {
    parse_optional_suffix("hello") $(val; err) {
        t |> success(err |> empty)
        t |> equal(val, "hello")
    }
}

[test]
def test_optional_default_value(t : T?) {
    t |> run("with value") @(t : T?) {
        parse_optional_default("val = 42") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 42)
        }
    }
    t |> run("without value defaults to 0") @(t : T?) {
        parse_optional_default("val") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 0)
        }
    }
}
