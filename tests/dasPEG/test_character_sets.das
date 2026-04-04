options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require dastest/testing_boost public
require peg/peg

// Single range: digits
def parse_digit(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var digit : bool
        rule(set('0'..'9'), EOF) {
            return true
        }
    }
}

// Multiple ranges: hex digits
def parse_hex(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var hex : bool
        rule(set('0'..'9', 'a'..'f', 'A'..'F'), EOF) {
            return true
        }
    }
}

// Single char in set (not a range)
def parse_sign(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var sign : bool
        rule(set('+', '-'), EOF) {
            return true
        }
    }
}

// not_set: match anything except specified chars
def parse_not_digit(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var notdigit : bool
        rule(not_set('0'..'9'), EOF) {
            return true
        }
    }
}

// Repeated set usage with text extraction
def parse_word(input : string; blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var word : string
        rule("{+letter}" as w, EOF) {
            return w
        }
        var letter : void?
        rule(set('a'..'z', 'A'..'Z')) {
            return null
        }
    }
}

// Multiple chars as set with boundary testing
def parse_vowel(input : string; blk : block<(val : bool; err : array<ParsingError>) : void>) {
    parse(input) {
        var vowel : bool
        rule(set('a', 'e', 'i', 'o', 'u'), EOF) {
            return true
        }
    }
}

[test]
def test_digit_range(t : T?) {
    t |> run("digit 0 matches") @(t : T?) {
        parse_digit("0") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("digit 5 matches") @(t : T?) {
        parse_digit("5") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("digit 9 matches") @(t : T?) {
        parse_digit("9") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("letter fails digit range") @(t : T?) {
        parse_digit("a") $(val; err) {
            t |> success(!empty(err))
        }
    }
}

[test]
def test_hex_multiple_ranges(t : T?) {
    t |> run("hex digit 0") @(t : T?) {
        parse_hex("0") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("hex letter a") @(t : T?) {
        parse_hex("a") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("hex letter F") @(t : T?) {
        parse_hex("F") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("hex letter g fails") @(t : T?) {
        parse_hex("g") $(val; err) {
            t |> success(!empty(err))
        }
    }
    t |> run("hex letter G fails") @(t : T?) {
        parse_hex("G") $(val; err) {
            t |> success(!empty(err))
        }
    }
}

[test]
def test_single_chars_in_set(t : T?) {
    t |> run("plus matches") @(t : T?) {
        parse_sign("+") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("minus matches") @(t : T?) {
        parse_sign("-") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("star fails") @(t : T?) {
        parse_sign("*") $(val; err) {
            t |> success(!empty(err))
        }
    }
}

[test]
def test_not_set(t : T?) {
    t |> run("letter passes not_set(digits)") @(t : T?) {
        parse_not_digit("a") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("symbol passes not_set(digits)") @(t : T?) {
        parse_not_digit("!") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("digit fails not_set(digits)") @(t : T?) {
        parse_not_digit("5") $(val; err) {
            t |> success(!empty(err))
        }
    }
}

[test]
def test_word_from_letter_set(t : T?) {
    t |> run("word extraction") @(t : T?) {
        parse_word("Hello") $(val; err) {
            t |> success(err |> empty, "should parse word")
            t |> equal(val, "Hello")
        }
    }
    t |> run("single letter word") @(t : T?) {
        parse_word("x") $(val; err) {
            t |> success(err |> empty)
            t |> equal(val, "x")
        }
    }
    t |> run("digits fail letter set") @(t : T?) {
        parse_word("123") $(val; err) {
            t |> success(!empty(err))
        }
    }
}

[test]
def test_vowels(t : T?) {
    t |> run("vowel a") @(t : T?) {
        parse_vowel("a") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("vowel e") @(t : T?) {
        parse_vowel("e") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("vowel u") @(t : T?) {
        parse_vowel("u") $(val; err) {
            t |> success(err |> empty)
        }
    }
    t |> run("consonant fails") @(t : T?) {
        parse_vowel("b") $(val; err) {
            t |> success(!empty(err))
        }
    }
}
