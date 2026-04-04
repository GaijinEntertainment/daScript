options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require dastest/testing_boost public
require peg/peg

// Ordered choice: first match wins
def parse_token_type(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule("true", EOF) {
            return "bool"
        }
        rule("false", EOF) {
            return "bool"
        }
        rule(number as n, EOF) {
            return "number"
        }
        rule(string_ as s, EOF) {
            return "string"
        }
    }
}

// Fallthrough to later alternatives
def parse_value(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule("null", EOF) {
            return "null"
        }
        rule(number as n, EOF) {
            return "{n}"
        }
        rule("{+alpha}" as word, EOF) {
            return word
        }
        var alpha : void?
        rule(set('a'..'z', 'A'..'Z')) {
            return null
        }
    }
}

// Many alternatives on one rule
def parse_operator(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule("==", EOF) {
            return "eq"
        }
        rule("!=", EOF) {
            return "ne"
        }
        rule("<=", EOF) {
            return "le"
        }
        rule(">=", EOF) {
            return "ge"
        }
        rule("<", EOF) {
            return "lt"
        }
        rule(">", EOF) {
            return "gt"
        }
    }
}

// Ordered choice: prefix ambiguity resolved by ordering
def parse_prefix(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule("foobar", EOF) {
            return "foobar"
        }
        rule("foo", EOF) {
            return "foo"
        }
    }
}

[test]
def test_ordered_choice(t : T?) {
    t |> run("true is bool") @(t : T?) {
        parse_token_type("true") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "bool")
        }
    }
    t |> run("false is bool") @(t : T?) {
        parse_token_type("false") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "bool")
        }
    }
    t |> run("42 is number") @(t : T?) {
        parse_token_type("42") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "number")
        }
    }
    t |> run("string") @(t : T?) {
        parse_token_type("\"hi\"") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "string")
        }
    }
}

[test]
def test_fallthrough(t : T?) {
    t |> run("null matches first") @(t : T?) {
        parse_value("null") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "null")
        }
    }
    t |> run("number matches second") @(t : T?) {
        parse_value("123") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "123")
        }
    }
    t |> run("word matches third") @(t : T?) {
        parse_value("hello") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "hello")
        }
    }
    t |> run("nothing matches") @(t : T?) {
        parse_value("!@#") $(val; err) {
            t |> success(!empty(err))
        }
    }
}

[test]
def test_many_alternatives(t : T?) {
    t |> run("==") @(t : T?) {
        parse_operator("==") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "eq")
        }
    }
    t |> run("!=") @(t : T?) {
        parse_operator("!=") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "ne")
        }
    }
    t |> run("<=") @(t : T?) {
        parse_operator("<=") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "le")
        }
    }
    t |> run("<") @(t : T?) {
        parse_operator("<") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "lt")
        }
    }
    t |> run(">") @(t : T?) {
        parse_operator(">") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "gt")
        }
    }
}

[test]
def test_prefix_ordering(t : T?) {
    t |> run("longer prefix matches first") @(t : T?) {
        parse_prefix("foobar") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "foobar")
        }
    }
    t |> run("shorter prefix matches second") @(t : T?) {
        parse_prefix("foo") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "foo")
        }
    }
}
