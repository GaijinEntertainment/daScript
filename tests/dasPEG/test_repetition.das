options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require dastest/testing_boost public
require peg/peg

// Zero-or-more (*) with array binding using space-separated pattern
def parse_star_array(input : string; blk : block<(val : array<int>; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : array<int>
        rule(*spaced_number as items, number as last, EOF) {
            items |> push(last)
            return <- items
        }
        rule(EOF) {
            var empty_arr : array<int>
            return <- empty_arr
        }
        var spaced_number : int
        rule(number as n, " ") {
            return n
        }
    }
}

// Zero-or-more (*) without binding (just repetition, discard results)
def parse_star_skip(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : int
        rule(*sp, number as n, EOF) {
            return n
        }
        var sp : void?
        rule(" ") {
            return null
        }
    }
}

// One-or-more (+) with array binding
def parse_plus(input : string; blk : block<(val : array<int>; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : array<int>
        rule(+spaced_number as items, number as last, EOF) {
            items |> push(last)
            return <- items
        }
        var spaced_number : int
        rule(number as n, " ") {
            return n
        }
    }
}

// Comma-separated list pattern (the canonical PEG idiom)
def parse_csv_nums(input : string; blk : block<(val : array<int>; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : array<int>
        rule(*comma_item as items, number as last, EOF) {
            items |> push(last)
            return <- items
        }
        rule(EOF) {
            var empty_arr : array<int>
            return <- empty_arr
        }
        var comma_item : int
        rule(number as n, WS, ",", WS) {
            return n
        }
    }
}

// Lines of text
def parse_lines(input : string; blk : block<(val : array<string>; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : array<string>
        rule(*newline_line as lines, last_line as last) {
            lines |> push(last)
            return <- lines
        }
        var newline_line : string
        rule("{+not_newline}" as text, EOL) {
            return text
        }
        var last_line : string
        rule("{+not_newline}" as text, EOF) {
            return text
        }
        var not_newline : void?
        rule(not_set('\n', '\r')) {
            return null
        }
    }
}

[test]
def test_star_empty(t : T?) {
    parse_star_array("") $(val; err) {
        t |> success(err |> empty)
        t |> equal(val |> length, 0)
    }
}

[test]
def test_star_one(t : T?) {
    parse_star_array("42") $(val; err) {
        t |> success(err |> empty, "single number should match")
        t |> equal(val |> length, 1)
        t |> equal(val[0], 42)
    }
}

[test]
def test_star_many(t : T?) {
    parse_star_array("1 2 3") $(val; err) {
        t |> success(err |> empty, "space-separated numbers")
        t |> equal(val |> length, 3)
        t |> equal(val[0], 1)
        t |> equal(val[1], 2)
        t |> equal(val[2], 3)
    }
}

[test]
def test_star_skip(t : T?) {
    t |> run("skip leading spaces") @(t : T?) {
        parse_star_skip("   42") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 42)
        }
    }
    t |> run("no leading spaces") @(t : T?) {
        parse_star_skip("42") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, 42)
        }
    }
}

[test]
def test_plus_empty_fails(t : T?) {
    parse_plus("") $(val; err) {
        t |> success(!empty(err), "one-or-more should require at least one match")
    }
}

[test]
def test_plus_one(t : T?) {
    // + requires at least one match of spaced_number (number + " ")
    // so "7 8" has spaced_number="7 " and last=8
    parse_plus("7 8") $(val; err) {
        t |> success(err |> empty, "one spaced + one last")
        t |> equal(val |> length, 2)
        t |> equal(val[0], 7)
        t |> equal(val[1], 8)
    }
}

[test]
def test_plus_many(t : T?) {
    parse_plus("10 20 30") $(val; err) {
        t |> success(err |> empty, "multiple elements")
        t |> equal(val |> length, 3)
        t |> equal(val[0], 10)
        t |> equal(val[1], 20)
        t |> equal(val[2], 30)
    }
}

[test]
def test_comma_separated(t : T?) {
    t |> run("empty list") @(t : T?) {
        parse_csv_nums("") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val |> length, 0)
        }
    }
    t |> run("single item") @(t : T?) {
        parse_csv_nums("42") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val |> length, 1)
            t |> equal(val[0], 42)
        }
    }
    t |> run("three items") @(t : T?) {
        parse_csv_nums("1, 2, 3") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val |> length, 3)
            t |> equal(val[0], 1)
            t |> equal(val[1], 2)
            t |> equal(val[2], 3)
        }
    }
}

[test]
def test_lines(t : T?) {
    t |> run("two lines") @(t : T?) {
        parse_lines("hello\nworld") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val |> length, 2)
            t |> equal(val[0], "hello")
            t |> equal(val[1], "world")
        }
    }
    t |> run("single line no newline") @(t : T?) {
        parse_lines("hello") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val |> length, 1)
            t |> equal(val[0], "hello")
        }
    }
}
