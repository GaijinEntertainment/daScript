options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/json_boost
require strings

[test(description = "read_json_bounded decodes unicode text and rejects the listed malformed documents with an error")]
def bounded_json_preserves_unicode_and_rejects_invalid_tokens(t : T?) {
    let limits = JsonReadLimits(max_bytes = 1024, max_depth = 8, max_tokens = 128, max_string_bytes = 64)
    var error : string
    var value = read_json_bounded("[\"Привет\",\"\\uD83D\\uDE00\"]", limits, error)
    t |> equal(error, "")
    t |> success(value != null)
    if (value != null) {
        t |> equal(value?[0].value as _string, "Привет")
        t |> equal(value?[1].value as _string, "😀")
        delete_json(value)
    }
    for (text in ["[1] trailing", "[01]", "[1.]", "[1e]", "[1e9999]", "[\"\\q\"]", "[\"\\uD800\"]", "[\"\\uDC00\"]", "[\"\\uZZZZ\"]", "[\"\\u0000\"]", "[\"raw\nnewline\"]", "\{\"x\":1,\"x\":2\}"]) {
        value = read_json_bounded(text, limits, error)
        t |> success(value == null, "reject malformed input")
        t |> success(!empty(error))
        delete_json(value)
    }
    let invalid_utf8 = [34u8, 192u8, 128u8, 34u8]
    value = read_json_bounded(invalid_utf8, limits, error)
    t |> success(value == null)
    let nul = [49u8, 0u8, 50u8]
    value = read_json_bounded(nul, limits, error)
    t |> success(value == null, "raw NUL cannot hide a suffix")
}

[test(description = "read_json_bounded accepts input at each limit and fails with no tree one past it")]
def bounded_json_limits_are_inclusive_and_fail_without_a_tree(t : T?) {
    var error : string
    let limits = JsonReadLimits(max_bytes = 32, max_depth = 2, max_tokens = 5, max_string_bytes = 2)
    var value = read_json_bounded("[[0]]", limits, error)
    t |> success(value != null)
    delete_json(value)
    value = read_json_bounded("[[[0]]]", limits, error)
    t |> success(value == null)
    value = read_json_bounded("[0,1,2]", limits, error)
    t |> success(value == null)
    value = read_json_bounded("\"é\"", limits, error)
    t |> success(value != null, "two-byte string fits exactly")
    delete_json(value)
    value = read_json_bounded("\"éx\"", limits, error)
    t |> success(value == null)
    value = read_json_bounded("[0]", JsonReadLimits(max_bytes = 2), error)
    t |> success(value == null)
    value = read_json_bounded("[0]", JsonReadLimits(max_depth = 0), error)
    t |> success(value == null)
}

[test(description = "read_json_bounded rejects duplicate keys even when the global allow-duplicates setting is on")]
def bounded_json_does_not_inherit_permissive_duplicate_setting(t : T?) {
    let prior = set_allow_duplicate_keys(true)
    var error : string
    var value = read_json_bounded("\{\"x\":1,\"x\":2\}", JsonReadLimits(), error)
    t |> success(value == null)
    value = read_json("\{\"x\":1,\"x\":2\}", error)
    t |> success(value != null, "ordinary reader's configured behavior is restored")
    delete_json(value)
    set_allow_duplicate_keys(prior)
}

def parse(t : T?; text : string) : JsonValue? {
    var error : string
    var js = read_json(text, error)
    t |> success(js != null, "parsed {text}")
    t |> equal(error, "")
    return js
}

def repeat_digit(digit : int; count : int) : string {
    return build_string() $(var w) {
        for (_ in range(count)) {
            write_char(w, digit)
        }
    }
}

[test(description = "integers within the int64 range parse as integers")]
def test_integers_within_range(t : T?) {
    t |> run("int64 max") @(t : T?) {
        var js = parse(t, "9223372036854775807")
        t |> equal(js is _longint, true)
        t |> equal(js as _longint, 9223372036854775807l)
    }
    t |> run("int64 min") @(t : T?) {
        var js = parse(t, "-9223372036854775808")
        t |> equal(js is _longint, true)
        t |> equal(js as _longint, -9223372036854775807l - 1l)
    }
    t |> run("nineteen digits below the limit") @(t : T?) {
        var js = parse(t, "9223372036854775806")
        t |> equal(js is _longint, true)
        t |> equal(js as _longint, 9223372036854775806l)
    }
    t |> run("leading zeros do not push past the limit") @(t : T?) {
        var js = parse(t, "000000000000000000000042")
        t |> equal(js is _longint, true)
        t |> equal(js as _longint, 42l)
    }
    t |> run("zero") @(t : T?) {
        var js = parse(t, "0")
        t |> equal(js is _longint, true)
        t |> equal(js as _longint, 0l)
    }
    t |> run("negative zero") @(t : T?) {
        var js = parse(t, "-0")
        t |> equal(js is _longint, true)
        t |> equal(js as _longint, 0l)
    }
}

[test(description = "integers outside the int64 range parse as numbers instead of failing")]
def test_integers_out_of_range_become_numbers(t : T?) {
    t |> run("int64 max plus one") @(t : T?) {
        var js = parse(t, "9223372036854775808")
        t |> equal(js is _number, true)
        t |> success(js as _number > 9.223e18lf, "magnitude and sign are preserved")
    }
    t |> run("uint64 max") @(t : T?) {
        var js = parse(t, "18446744073709551615")
        t |> equal(js is _number, true)
        t |> success(js as _number > 1.8e19lf, "magnitude and sign are preserved")
    }
    t |> run("int64 min minus one") @(t : T?) {
        var js = parse(t, "-9223372036854775809")
        t |> equal(js is _number, true)
        t |> success(js as _number < -9.223e18lf, "magnitude and sign are preserved")
    }
    t |> run("far past uint64") @(t : T?) {
        var js = parse(t, "99999999999999999999999999")
        t |> equal(js is _number, true)
        t |> success(js as _number > 9.9e25lf, "magnitude and sign are preserved")
    }
    t |> run("out of range inside a document") @(t : T?) {
        var js = parse(t, "\{ \"id\" : 18446744073709551615 \}")
        t |> equal(js?["id"] is _number, true)
    }
}

[test(description = "an integer too long for a double is a parse error, not a panic")]
def test_integers_past_double_range_error(t : T?) {
    t |> run("four hundred digits is a parse error, not a panic") @(t : T?) {
        var error : string
        var js = read_json(repeat_digit('9', 400), error)
        t |> equal(js == null, true)
        t |> success(!empty(error), "the lexer reports the out-of-range number")
    }
    t |> run("negative four hundred digits") @(t : T?) {
        var error : string
        var js = read_json("-" + repeat_digit('9', 400), error)
        t |> equal(js == null, true)
        t |> success(!empty(error), "the lexer reports the out-of-range number")
    }
}

def expect_number_error(t : T?; text : string) {
    var error : string
    let js = read_json(text, error)
    t |> success(js == null, "{text} is rejected")
    t |> success(!empty(error), "{text} reports an error instead of panicking")
}

def expect_number_error_matching(t : T?; text, fragment : string) {
    var error : string
    let js = read_json(text, error)
    t |> success(js == null, "{text} is rejected")
    t |> success(error |> find(fragment) >= 0, "{text}: got <{error}>")
}

[test(description = "a float whose exponent overflows or underflows a double is a parse error")]
def test_floats_past_double_range_error(t : T?) {
    t |> run("exponent overflow") @(t : T?) {
        expect_number_error(t, "1e400")
        expect_number_error(t, "-1e999")
        expect_number_error(t, "2.5e308")
    }
    t |> run("exponent underflow") @(t : T?) {
        expect_number_error(t, "1e-400")
    }
    t |> run("inside a document") @(t : T?) {
        expect_number_error(t, "\{ \"ratio\" : 1e400 \}")
        expect_number_error(t, "[1, 1e400]")
    }
}

[test(description = "ordinary and extreme representable floats still parse")]
def test_floats_within_range_still_parse(t : T?) {
    t |> run("ordinary and extreme representable values") @(t : T?) {
        for (text in ["3.14", "-0.5", "1.5e10", "2.5E-3", "1.7e308", "-1.7e308", "1e-320", "0.0"]) {
            var js = parse(t, text)
            t |> success(js is _number, "{text} is a number")
        }
    }
}

[test(description = "a truncated exponent reports an invalid number while true overflow reports out of range")]
def test_number_error_messages_distinguish_invalid_from_range(t : T?) {
    t |> run("truncated exponents are invalid, not out of range") @(t : T?) {
        for (text in ["1e", "1e+", "1.5e-", "[1e]"]) {
            expect_number_error_matching(t, text, "is invalid")
        }
    }
    t |> run("true overflow stays out of range") @(t : T?) {
        for (text in ["1e400", "1e-400", "2.5e308"]) {
            expect_number_error_matching(t, text, "is out of range")
        }
    }
}

[test(description = "a number with a leading plus is rejected as RFC 8259 requires")]
def test_leading_plus_is_not_a_number(t : T?) {
    t |> run("RFC 8259 has no leading plus") @(t : T?) {
        expect_number_error(t, "+5")
        expect_number_error(t, "+3.14")
        expect_number_error(t, "[+1]")
    }
}

def utf8_bytes(s : string) : string {
    return build_string() $(var w) {
        peek_data(s) $(raw) {
            var first = true
            for (b in raw) {
                if (!first) {
                    write_char(w, ' ')
                }
                first = false
                write(w, int(b))
            }
        }
    }
}

def decode(t : T?; text : string) : string {
    var error : string
    let js = read_json(text, error)
    t |> success(js != null, "parsed {text}")
    if (js == null) {
        t |> failure("{text}: {error}")
        return ""
    }
    t |> success(js is _string, "decoded a string")
    return js as _string
}

[test(description = "a valid surrogate pair escape decodes to one 4-byte code point")]
def test_surrogate_pairs(t : T?) {
    t |> run("emoji pair decodes to one 4-byte code point") @(t : T?) {
        let s = decode(t, "\"\\ud83d\\ude00\"")
        t |> equal(utf8_bytes(s), "240 159 152 128")
        t |> equal(length(s), 4)
    }
    t |> run("musical symbol pair") @(t : T?) {
        let s = decode(t, "\"\\ud834\\udd1e\"")
        t |> equal(utf8_bytes(s), "240 157 132 158")
    }
    t |> run("last code point") @(t : T?) {
        let s = decode(t, "\"\\udbff\\udfff\"")
        t |> equal(utf8_bytes(s), "244 143 191 191")
    }
    t |> run("pair surrounded by ASCII") @(t : T?) {
        let s = decode(t, "\"a\\ud83d\\ude00b\"")
        t |> equal(utf8_bytes(s), "97 240 159 152 128 98")
    }
    t |> run("two pairs in a row") @(t : T?) {
        let s = decode(t, "\"\\ud83d\\ude00\\ud83d\\ude00\"")
        t |> equal(utf8_bytes(s), "240 159 152 128 240 159 152 128")
    }
}

[test(description = "a lone surrogate escape decodes to U+FFFD")]
def test_lone_surrogates(t : T?) {
    t |> run("lone high surrogate becomes U+FFFD") @(t : T?) {
        let s = decode(t, "\"\\ud83d\"")
        t |> equal(utf8_bytes(s), "239 191 189")
    }
    t |> run("lone low surrogate becomes U+FFFD") @(t : T?) {
        let s = decode(t, "\"\\ude00\"")
        t |> equal(utf8_bytes(s), "239 191 189")
    }
    t |> run("high surrogate followed by a literal char") @(t : T?) {
        let s = decode(t, "\"\\ud83dx\"")
        t |> equal(utf8_bytes(s), "239 191 189 120")
    }
    t |> run("high surrogate followed by a non-u escape") @(t : T?) {
        let s = decode(t, "\"\\ud83d\\n\"")
        t |> equal(utf8_bytes(s), "239 191 189 10")
    }
    t |> run("high surrogate followed by a BMP escape") @(t : T?) {
        let s = decode(t, "\"\\ud83d\\u0041\"")
        t |> equal(utf8_bytes(s), "239 191 189 65")
    }
    t |> run("two high surrogates then a low one") @(t : T?) {
        let s = decode(t, "\"\\ud83d\\ud83d\\ude00\"")
        t |> equal(utf8_bytes(s), "239 191 189 240 159 152 128")
    }
    t |> run("pair split by a literal char") @(t : T?) {
        let s = decode(t, "\"\\ud83dx\\ude00\"")
        t |> equal(utf8_bytes(s), "239 191 189 120 239 191 189")
    }
}

[test(description = "escapes below and above the surrogate block decode as before and a malformed escape is kept verbatim")]
def test_bmp_escapes_unchanged(t : T?) {
    t |> run("ASCII") @(t : T?) {
        let s = decode(t, "\"\\u0041\"")
        t |> equal(utf8_bytes(s), "65")
    }
    t |> run("two-byte") @(t : T?) {
        let s = decode(t, "\"\\u00e9\"")
        t |> equal(utf8_bytes(s), "195 169")
    }
    t |> run("three-byte") @(t : T?) {
        let s = decode(t, "\"\\u20ac\"")
        t |> equal(utf8_bytes(s), "226 130 172")
    }
    t |> run("just below the surrogate block") @(t : T?) {
        let s = decode(t, "\"\\ud7ff\"")
        t |> equal(utf8_bytes(s), "237 159 191")
    }
    t |> run("just above the surrogate block") @(t : T?) {
        let s = decode(t, "\"\\ue000\"")
        t |> equal(utf8_bytes(s), "238 128 128")
    }
    t |> run("malformed escape is preserved verbatim") @(t : T?) {
        let s = decode(t, "\"\\u12zz\"")
        t |> equal(s, "\\u12zz")
    }
}
