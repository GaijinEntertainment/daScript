options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/strings_boost
require daslib/faker
require daslib/fuzzer
require daslib/regex
require daslib/strings_convert

def test_conversions(t : T?; good_string : string; good_result : auto(TT); out_of_range_string : string) {
    var errC : ConversionResult
    var ofs : int

    var res = TT(good_string, errC, ofs)// as is
    t |> equal(good_result, res)
    t |> equal(ConversionResult.ok, errC)
    t |> equal(length(good_string), ofs)

    res = TT("  {good_string}  ", errC, ofs) // with spaces
    t |> equal(good_result, res)
    t |> equal(ConversionResult.ok, errC)
    t |> equal(length(good_string) + 2, ofs)

    res = TT("", errC, ofs) // empty string
    t |> equal(TT(), res)
    t |> equal(ConversionResult.invalid_argument, errC)
    t |> equal(0, ofs)

    res = TT("random goo", errC, ofs) // not a number
    t |> equal(TT(), res)
    t |> equal(ConversionResult.invalid_argument, errC)
    t |> equal(0, ofs)

    res = TT(out_of_range_string, errC, ofs) // out of range
    t |> equal(TT(), res)
    t |> equal(ConversionResult.out_of_range, errC)
    t |> equal(0, ofs)
}

def test_hex_conversions(t : T?; good_string : string; good_result : auto(TT); out_of_range_string : string) {
    var errC : ConversionResult
    var ofs : int

    var res = TT(good_string, errC, ofs, true)// as is
    t |> equal(good_result, res)
    t |> equal(ConversionResult.ok, errC)
    t |> equal(length(good_string), ofs)

    res = TT("  {good_string}  ", errC, ofs, true)// with spaces
    t |> equal(good_result, res)
    t |> equal(ConversionResult.ok, errC)
    t |> equal(length(good_string) + 2, ofs)

    res = TT("", errC, ofs, true)// empty string
    t |> equal(TT(), res)
    t |> equal(ConversionResult.invalid_argument, errC)
    t |> equal(0, ofs)

    res = TT("random goo", errC, ofs, true)// not a number
    t |> equal(TT(), res)
    t |> equal(ConversionResult.invalid_argument, errC)
    t |> equal(0, ofs)

    res = TT(out_of_range_string, errC, ofs, true)// out of range
    t |> equal(TT(), res)
    t |> equal(ConversionResult.out_of_range, errC)
    t |> equal(0, ofs)
}


[test(description = "string conversion of each integer width accepts decimal and hex text in range and reports an error for a value one past the range")]
def test_convert_with_errors(t : T?) {
    test_conversions(t, "13", int8(13), "128")
    test_hex_conversions(t, "13", int8(0x13), "0x100")

    test_conversions(t, "13", uint8(13), "256")
    test_hex_conversions(t, "13", uint8(0x13), "0x100")

    test_conversions(t, "13", int16(13), "32768")
    test_hex_conversions(t, "13", int16(0x13), "0x10000")

    test_conversions(t, "13", uint16(13), "65536")
    test_hex_conversions(t, "13", uint16(0x13), "0x10000")

    test_conversions(t, "13", int(13), "2147483648") // nolint:PERF020 the cast names the row's type in this conformance table
    test_hex_conversions(t, "13", int(0x13), "0x100000000")

    test_conversions(t, "13", uint(13), "4294967296")
    test_hex_conversions(t, "13", uint(0x13), "0x100000000") // nolint:PERF020 the cast names the row's type in this conformance table

    test_conversions(t, "13", int64(13), "9223372036854775808")
    test_hex_conversions(t, "13", int64(0x13), "0x10000000000000000")

    test_conversions(t, "13", uint64(13), "18446744073709551616")
    test_hex_conversions(t, "13", uint64(0x13), "0x10000000000000000")

    test_conversions(t, "13", float(13.), "1e39") // nolint:PERF020 the cast names the row's type in this conformance table
    test_conversions(t, "13", double(13.lf), "1e309") // nolint:PERF020 the cast names the row's type in this conformance table
}

[test(description = "string conversion functions survive fuzzed inputs for strings, chars, integers, floats and their parsing counterparts")]
def test_convert(t : T?) { // nolint:STYLE038 one flat pin+fuzz arm per conversion entry point under test

    let a8 <- array<uint8>(uint8('A'), uint8('B'), uint8('C'))
    t |> equal(string(a8), "ABC")
    t |> run("fuzz string from array") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let len = fake |> random_uint % 10u
            var arr <- [for (_q in urange(len)); uint8(fake |> random_int % 256)]
            t |> accept(string(arr))
            delete arr
        }
    }

    t |> equal(to_char('A'), "A")
    t |> run("fuzz to_char") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(to_char(fake |> random_int))
        }
    }

    t |> equal(int("13"), 13)
    t |> run("fuzz int") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(int(fake |> any_int))
        }
        fuzz() {
            t |> accept(int(fake |> any_string))
        }
    }

    t |> equal(int64("12345678999"), 12345678999l)
    t |> equal(int64("-123456789999"), -123456789999l)
    t |> run("fuzz int64") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(int64(fake |> any_int))
        }
        fuzz() {
            t |> accept(int64(fake |> any_string))
        }
    }

    t |> equal(uint("13"), 13u)
    t |> run("fuzz uint") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(uint(fake |> any_uint))
        }
        fuzz() {
            t |> accept(uint(fake |> any_string))
        }
    }

    t |> equal(uint64("12345678999"), 12345678999ul)
    t |> run("fuzz uint64") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(uint64(fake |> any_int))
        }
        fuzz() {
            t |> accept(uint64(fake |> any_string))
        }
    }

    t |> equal(float("13"), 13.)
    t |> equal(float("13."), 13.)
    t |> equal(float("13.54"), 13.54)
    t |> equal(float("-13"), -13.)
    t |> equal(float("-13."), -13.)
    t |> equal(float("-13.54"), -13.54)
    t |> run("fuzz float") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(float(fake |> any_float))
        }
        fuzz() {
            t |> accept(float(fake |> any_string))
        }
    }

    t |> equal(double("13"), 13.lf)
    t |> equal(double("13."), 13.lf)
    t |> equal(double("13.54"), 13.54lf)
    t |> equal(double("-13"), -13.lf)
    t |> equal(double("-13."), -13.lf)
    t |> equal(double("-13.54"), -13.54lf)
    t |> run("fuzz double") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(double(fake |> any_float))
        }
        fuzz() {
            t |> accept(double(fake |> any_string))
        }
    }

    t |> equal(to_int("13", true), 19)
    t |> run("fuzz to_int") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(to_int(fake |> any_hex, true))
        }
        fuzz() {
            t |> accept(to_int(fake |> any_string, true))
        }
    }

    t |> equal(to_uint("13", true), 0x13)
    t |> run("fuzz to_uint") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(to_uint(fake |> any_hex, true))
        }
        fuzz() {
            t |> accept(to_uint(fake |> any_string, true))
        }
    }

    t |> equal(to_int64("13", true), 19l)
    t |> run("fuzz to_int64") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(to_int64(fake |> any_hex, true))
        }
        fuzz() {
            t |> accept(to_int64(fake |> any_string, true))
        }
    }

    t |> equal(to_uint64("13", true), 0x13ul)
    t |> run("fuzz to_uint") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(to_uint64(fake |> any_hex, true))
        }
        fuzz() {
            t |> accept(to_uint64(fake |> any_string, true))
        }
    }

    t |> equal(to_float("13"), 13.)
    t |> equal(to_float("13."), 13.)
    t |> equal(to_float("13.54"), 13.54)
    t |> equal(to_float("-13"), -13.)
    t |> equal(to_float("-13."), -13.)
    t |> equal(to_float("-13.54"), -13.54)
    t |> run("fuzz float") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(to_float(fake |> any_float))
        }
        fuzz() {
            t |> accept(to_float(fake |> any_string))
        }
    }

    t |> equal(to_double("13"), 13.lf)
    t |> equal(to_double("13."), 13.lf)
    t |> equal(to_double("13.54"), 13.54lf)
    t |> equal(to_double("-13"), -13.lf)
    t |> equal(to_double("-13."), -13.lf)
    t |> equal(to_double("-13.54"), -13.54lf)
    t |> run("fuzz double") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(to_double(fake |> any_float))
        }
        fuzz() {
            t |> accept(to_double(fake |> any_string))
        }
    }
}

def expect_ok_value(t : T?; r : auto(R) const; expected : auto(V)) {
    t |> equal(true, is_ok(r))
    if (is_ok(r)) {
        t |> equal(expected, unwrap(r))
    }
}

def expect_err_kind(t : T?; r : auto(R) const; expected : ConversionError) {
    t |> equal(false, is_ok(r))
    if (is_err(r)) {
        t |> equal(expected, unwrap_err(r))
    }
}

[test(description = "try_to_int parses signed text with leading spaces and reports trailing garbage, invalid input and overflow as distinct errors")]
def test_try_to_int_basic(t : T?) {
    expect_ok_value(t, try_to_int("13"), 13)
    expect_ok_value(t, try_to_int("-13"), -13)
    expect_ok_value(t, try_to_int("0"), 0)
    expect_ok_value(t, try_to_int("  13"), 13)
    expect_err_kind(t, try_to_int("13  "), ConversionError.trailing_garbage)
    expect_err_kind(t, try_to_int("13abc"), ConversionError.trailing_garbage)
    expect_err_kind(t, try_to_int(""), ConversionError.invalid_argument)
    expect_err_kind(t, try_to_int("   "), ConversionError.invalid_argument)
    expect_err_kind(t, try_to_int("abc"), ConversionError.invalid_argument)
    expect_err_kind(t, try_to_int("1.5"), ConversionError.trailing_garbage)
    expect_err_kind(t, try_to_int("9999999999999999999"), ConversionError.out_of_range)
}

[test(description = "try_to_int in hex mode parses text with or without a 0x prefix")]
def test_try_to_int_hex(t : T?) {
    expect_ok_value(t, try_to_int("13", true), int(0x13))
    expect_ok_value(t, try_to_int("0x1F", true), int(0x1F))
    expect_ok_value(t, try_to_int("FF", true), int(0xFF))
    expect_err_kind(t, try_to_int("0x1F", false), ConversionError.trailing_garbage)
    expect_err_kind(t, try_to_int("xyz", true), ConversionError.invalid_argument)
}

[test(description = "try_to_int8 parses the full int8 range and reports out_of_range outside it")]
def test_try_to_int8(t : T?) {
    expect_ok_value(t, try_to_int8("13"), int8(13))
    expect_ok_value(t, try_to_int8("-128"), int8(-128))
    expect_ok_value(t, try_to_int8("127"), int8(127))
    expect_err_kind(t, try_to_int8("128"), ConversionError.out_of_range)
    expect_err_kind(t, try_to_int8(""), ConversionError.invalid_argument)
    expect_err_kind(t, try_to_int8("13x"), ConversionError.trailing_garbage)
}

[test(description = "try_to_uint8 parses up to 255 and reports out_of_range beyond it")]
def test_try_to_uint8(t : T?) {
    expect_ok_value(t, try_to_uint8("13"), uint8(13))
    expect_ok_value(t, try_to_uint8("255"), uint8(255))
    expect_err_kind(t, try_to_uint8("256"), ConversionError.out_of_range)
    expect_err_kind(t, try_to_uint8("-1"), ConversionError.invalid_argument)
}

[test(description = "try_to_int16 parses the full int16 range and reports out_of_range outside it")]
def test_try_to_int16(t : T?) {
    expect_ok_value(t, try_to_int16("13"), int16(13))
    expect_ok_value(t, try_to_int16("-32768"), int16(-32768))
    expect_ok_value(t, try_to_int16("32767"), int16(32767))
    expect_err_kind(t, try_to_int16("32768"), ConversionError.out_of_range)
}

[test(description = "try_to_uint16 parses up to 65535 and reports out_of_range beyond it")]
def test_try_to_uint16(t : T?) {
    expect_ok_value(t, try_to_uint16("13"), uint16(13))
    expect_ok_value(t, try_to_uint16("65535"), uint16(65535))
    expect_err_kind(t, try_to_uint16("65536"), ConversionError.out_of_range)
}

[test(description = "try_to_uint parses up to 4294967295 and reports out_of_range beyond it")]
def test_try_to_uint(t : T?) {
    expect_ok_value(t, try_to_uint("13"), 13u)
    expect_ok_value(t, try_to_uint("4294967295"), 4294967295u)
    expect_err_kind(t, try_to_uint("4294967296"), ConversionError.out_of_range)
    expect_err_kind(t, try_to_uint("-1"), ConversionError.invalid_argument)
}

[test(description = "try_to_int64 parses 64-bit values and reports out_of_range on overflow")]
def test_try_to_int64(t : T?) {
    expect_ok_value(t, try_to_int64("12345678999"), 12345678999l)
    expect_ok_value(t, try_to_int64("-12345678999"), -12345678999l)
    expect_err_kind(t, try_to_int64("99999999999999999999"), ConversionError.out_of_range)
}

[test(description = "try_to_uint64 parses up to the 64-bit maximum and reports out_of_range beyond it")]
def test_try_to_uint64(t : T?) {
    expect_ok_value(t, try_to_uint64("12345678999"), 12345678999ul)
    expect_ok_value(t, try_to_uint64("18446744073709551615"), 18446744073709551615ul)
    expect_err_kind(t, try_to_uint64("18446744073709551616"), ConversionError.out_of_range)
    expect_err_kind(t, try_to_uint64("-1"), ConversionError.invalid_argument)
}

[test(description = "try_to_float parses integer, fractional and negative text and reports invalid input as an error")]
def test_try_to_float_basic(t : T?) {
    expect_ok_value(t, try_to_float("13"), 13.f)
    expect_ok_value(t, try_to_float("13.5"), 13.5f)
    expect_ok_value(t, try_to_float("-13."), -13.f)
    expect_ok_value(t, try_to_float("  -13.54"), -13.54f)
    expect_err_kind(t, try_to_float(""), ConversionError.invalid_argument)
    expect_err_kind(t, try_to_float("abc"), ConversionError.invalid_argument)
    expect_err_kind(t, try_to_float("13x"), ConversionError.trailing_garbage)
    expect_err_kind(t, try_to_float("13.5  "), ConversionError.trailing_garbage)
    expect_err_kind(t, try_to_float("1e39"), ConversionError.out_of_range)
}

[test(description = "try_to_double parses integer, fractional and negative text and reports invalid input as an error")]
def test_try_to_double_basic(t : T?) {
    expect_ok_value(t, try_to_double("13"), 13.lf)
    expect_ok_value(t, try_to_double("13.54"), 13.54lf)
    expect_ok_value(t, try_to_double("-13.54"), -13.54lf)
    expect_err_kind(t, try_to_double(""), ConversionError.invalid_argument)
    expect_err_kind(t, try_to_double("13x"), ConversionError.trailing_garbage)
    expect_err_kind(t, try_to_double("1e309"), ConversionError.out_of_range)
}

[test(description = "formatting a value of each numeric type and parsing it back with try_to gives the same value")]
def test_try_to_round_trip(t : T?) {
    let i8 = int8(-42)
    expect_ok_value(t, try_to_int8(string(i8)), i8)
    let u8 = uint8(200)
    expect_ok_value(t, try_to_uint8(string(u8)), u8)
    let i16 = int16(-12345)
    expect_ok_value(t, try_to_int16(string(i16)), i16)
    let u16 = uint16(54321)
    expect_ok_value(t, try_to_uint16(string(u16)), u16)
    let i32 = -1234567
    expect_ok_value(t, try_to_int(string(i32)), i32)
    let u32 = 1234567u
    expect_ok_value(t, try_to_uint(string(u32)), u32)
    let i64 = -123456789012345l
    expect_ok_value(t, try_to_int64(string(i64)), i64)
    let u64 = 123456789012345ul
    expect_ok_value(t, try_to_uint64(string(u64)), u64)
    let f = 3.5f
    expect_ok_value(t, try_to_float(string(f)), f)
    let d = 3.5lf
    expect_ok_value(t, try_to_double(string(d)), d)
}
