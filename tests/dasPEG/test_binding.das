options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require dastest/testing_boost public
require peg/peg

// Bind number to variable
def parse_bound_number(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule(number as n, EOF) {
            return n
        }
    }
}

// Bind string_ to variable
def parse_bound_string(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule(string_ as s, EOF) {
            return s
        }
    }
}

// Bind double_ to variable
def parse_bound_double(input : string; blk : block<(val : double; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : double
        rule(double_ as d, EOF) {
            return d
        }
    }
}

// Bind rule to rule: nonterminal binding
def parse_add(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule(term as a, WS, "+", WS, term as b, EOF) {
            return a + b
        }
        rule(term as a, EOF) {
            return a
        }
        var term : int
        rule(number as n) {
            return n
        }
    }
}

// Text extraction with "{rule}" as name
def parse_text_extract(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule("{+letter}" as word, EOF) {
            return word
        }
        var letter : void?
        rule(set('a'..'z', 'A'..'Z')) {
            return null
        }
    }
}

// Text extraction of mixed content
def parse_ident(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule("{+ident_char}" as id, EOF) {
            return id
        }
        var ident_char : void?
        rule(set('a'..'z', 'A'..'Z', '0'..'9', '_')) {
            return null
        }
    }
}

// Multiple bindings in one rule
def parse_pair(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule("{+alpha}" as key, WS, "=", WS, "{+alpha}" as value, EOF) {
            return "{key}={value}"
        }
        var alpha : void?
        rule(set('a'..'z', 'A'..'Z')) {
            return null
        }
    }
}

[test(description = "a bound number rule returns the parsed integer value")]
def test_bind_number(t : T?) {
    parse_bound_number("42") $(val; err) {
        t |> success(err |> empty)
        t |> equal(val, 42)
    }
}

[test(description = "a bound string rule returns the string literal contents without quotes")]
def test_bind_string(t : T?) {
    parse_bound_string("\"hello\"") $(val; err) {
        t |> success(err |> empty)
        t |> equal(val, "hello")
    }
}

[test(description = "a bound double rule returns the parsed double value")]
def test_bind_double(t : T?) {
    parse_bound_double("3.14") $(val; err) {
        t |> success(err |> empty)
        t |> equal(val, 3.14lf)
    }
}

[test(description = "bindings to nonterminal results combine into an addition and fall back to a single term")]
def test_bind_nonterminal(t : T?) {
    t |> run("addition") @(t : T?) {
        parse_add("3 + 5") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 8)
        }
    }
    t |> run("single term") @(t : T?) {
        parse_add("42") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 42)
        }
    }
}

[test(description = "a text-capturing binding extracts the matched word, including a single letter")]
def test_text_extract(t : T?) {
    t |> run("extract word") @(t : T?) {
        parse_text_extract("Hello") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "Hello")
        }
    }
    t |> run("extract single letter") @(t : T?) {
        parse_text_extract("x") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "x")
        }
    }
}

[test(description = "an identifier binding captures alphanumeric names and names starting with an underscore")]
def test_ident_extract(t : T?) {
    t |> run("alphanumeric identifier") @(t : T?) {
        parse_ident("foo_bar123") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "foo_bar123")
        }
    }
    t |> run("underscore start") @(t : T?) {
        parse_ident("_private") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "_private")
        }
    }
}

[test(description = "several bindings in one rule are all visible to the rule's action")]
def test_multiple_bindings(t : T?) {
    parse_pair("key = value") $(val; err) {
        t |> success(err |> empty)
        t |> equal(val, "key=value")
    }
}
