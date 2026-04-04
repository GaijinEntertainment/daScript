options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require dastest/testing_boost public
require peg/peg

// PEEK: positive lookahead (matches without consuming)
def parse_peek_digit(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule(PEEK(set('0'..'9')), commit, number as n, EOF) {
            return n
        }
    }
}

// Negative lookahead (!): asserts rule fails
def parse_not_keyword(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule(!"if", !"then", "{+alpha}" as word, EOF) {
            return word
        }
        var alpha : void?
        rule(set('a'..'z', 'A'..'Z')) {
            return null
        }
    }
}

// PEEK combined with alternatives
def parse_token(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule(PEEK(set('0'..'9')), number as n, EOF) {
            return "num:{n}"
        }
        rule(PEEK(set('a'..'z', 'A'..'Z')), "{+alpha}" as w, EOF) {
            return "word:{w}"
        }
        var alpha : void?
        rule(set('a'..'z', 'A'..'Z')) {
            return null
        }
    }
}

// Negative lookahead doesn't consume input — must pair with 'any' to advance
def parse_not_eof(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule("{+not_end}" as text, ".", EOF) {
            return text
        }
        var not_end : void?
        rule(!".", any) {
            return null
        }
    }
}

[test]
def test_peek_matches(t : T?) {
    t |> run("digit passes peek") @(t : T?) {
        parse_peek_digit("42") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 42)
        }
    }
}

[test]
def test_peek_fails(t : T?) {
    t |> run("letter fails peek") @(t : T?) {
        parse_peek_digit("abc") $(val; err) {
            t |> success(!empty(err))
        }
    }
}

[test]
def test_negative_lookahead_accepts(t : T?) {
    t |> run("non-keyword accepted") @(t : T?) {
        parse_not_keyword("hello") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "hello")
        }
    }
}

[test]
def test_negative_lookahead_rejects(t : T?) {
    t |> run("keyword 'if' rejected") @(t : T?) {
        parse_not_keyword("if") $(val; err) {
            t |> equal(val, "", "keyword 'if' should not parse as identifier")
        }
    }
    t |> run("keyword 'then' rejected") @(t : T?) {
        parse_not_keyword("then") $(val; err) {
            t |> equal(val, "", "keyword 'then' should not parse as identifier")
        }
    }
}

[test]
def test_peek_with_alternatives(t : T?) {
    t |> run("number token") @(t : T?) {
        parse_token("42") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "num:42")
        }
    }
    t |> run("word token") @(t : T?) {
        parse_token("hello") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "word:hello")
        }
    }
}

[test]
def test_negative_lookahead_no_consume(t : T?) {
    t |> run("text before dot") @(t : T?) {
        parse_not_eof("abc.") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "abc")
        }
    }
}
