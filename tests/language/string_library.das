options gen2
options no_unused_function_arguments = false
require strings
require UnitTest
require dastest/testing_boost public
require daslib/strings_boost
require _string_clone_init_helper

def so_failUint(val : string) {
    try {
        debug(uint(val), "=")
        assert(false, "can't convert non uint string without exception")
    } recover {}
}

def so_failInt(val : string) {
    try {
        debug(int(val), "=")
        assert(false, "can't convert non int string without exception")
    } recover {}
}

def so_failFloat(val : string) {
    try {
        debug(float(val), "=")
        assert(false, "can't convert non float string without exception")
    } recover {}
}

def so_failDouble(val : string) {
    try {
        debug(double(val), "=")
        assert(false, "can't convert non double string without exception")
    } recover {}
}

def so_failAll(val : string) {
    so_failUint(val)
    so_failInt(val)
    so_failFloat(val)
    so_failDouble(val)
}

[test(description = "string builtins - case conversion, reverse, find, starts/ends_with, strip, slice, numeric conversions and their failures, escapes, fmt and build_string - give the expected results")]
def test_string_ops(t : T?) {  // nolint:STYLE038 - flat list of independent test arms
    t |> run("case conversion and reverse") @(t : T?) {
        let s : string = "hello, world!"
        t |> equal(to_upper(s), "HELLO, WORLD!")
        t |> equal(to_lower(to_upper(s)), s)
        t |> equal(reverse(s), "!dlrow ,olleh")
        t |> equal(length(s), 13)
    }
    t |> run("find and starts/ends_with") @(t : T?) {
        let s : string = "hello, world!"
        t |> equal(find(s, "world!"), 7)
        t |> equal(find(s, "world!", 4), 7)
        t |> equal(find(s, "o"), 4)
        t |> equal(find(s, "o", 5), 8)
        t |> equal(find(s, "o", 9), -1)
        t |> equal(find(s, "??"), -1)
        t |> success(ends_with(s, "world!"))
        t |> success(starts_with(s, "hello"))
        t |> success(starts_with(s, ""))
        t |> success(ends_with(s, ""))
        t |> success(!starts_with(s, "a"))
        t |> success(!ends_with(s, "b"))
    }
    t |> run("strip and slice") @(t : T?) {
        let s : string = "hello, world!"
        let s2 : string = "   hello, world!   "
        t |> equal(strip(s2), s)
        t |> equal(strip_right(strip_left(s2)), s)
        t |> equal(strip_left(s2), "hello, world!   ")
        t |> equal(strip_right(s2), "   hello, world!")
        t |> equal(slice(s, 7), "world!")
        t |> equal(slice(s, -6), "world!")
        t |> equal(slice(s, 6, -6), " ")
        t |> equal(slice(s, 6, 7), " ")
    }
    t |> run("numeric conversions") @(t : T?) {
        t |> equal(int("11"), 11)
        t |> equal(int("-12"), -12)
        t |> equal(int("-12  "), -12)
        t |> equal(int("  -12"), -12)
        t |> equal(int("  -12  "), -12)
        t |> equal(uint("11"), 11u)
        t |> equal(uint("  12  "), 12u)
        t |> equal(uint("  12"), 12u)
        t |> equal(int64("11"), 11l)
        t |> equal(int64("-12"), -12l)
        t |> equal(int64("-12  "), -12l)
        t |> equal(int64("  -12"), -12l)
        t |> equal(int64("  -12  "), -12l)
        t |> equal(uint64("11"), 11ul)
        t |> equal(uint64("  12  "), 12ul)
        t |> equal(uint64("  12"), 12ul)
        t |> equal(float("-12"), -12.)
        t |> equal(float("1e1"), 10.)
        t |> equal(float("  12"), 12.)
        t |> equal(float("12   "), 12.)
        t |> equal(float("  12   "), 12.)
    }
    t |> run("fail conversions") @(t : T?) {
        so_failAll("")
        so_failAll("abc")
        t |> equal(to_int(""), 0)
        t |> equal(to_float(""), 0.)
        t |> equal(to_int("11abc"), 11)
        t |> equal(to_float("11.1abc"), 11.1)
    }
    t |> run("escape and multiline") @(t : T?) {
        let a_slash_slash = "ab\\xy"
        t |> equal(length(a_slash_slash), 5)
        let mls = "this is
multi line string"
        t |> equal(mls, "this is\nmulti line string")
    }
    t |> run("fmt formatting") @(t : T?) {
        t |> equal(fmt(":02x", 26), "1a")
        t |> equal(fmt(":02x", 27u), "1b")
        t |> equal(fmt(":02x", 28l), "1c")
        t |> equal(fmt(":02x", 29ul), "1d")
        t |> equal(fmt(":.1f", 1.23), "1.2")
        t |> equal(fmt(":.2f", 1.234), "1.23")
        var failed = false
        try {
            // the invalid format must reach RUNTIME fmt: a const-foldable format string moves
            // the throw into the compiler's const-folder, where the longjmp-out-of-catch panic
            // path leaks the exception object - CI's LeakSanitizer fails the AOT generator on it
            var ft : string
            ft = "blah"     // nolint:STYLE011 - init-at-decl (or let) makes the bad format foldable
            let x = fmt(ft, 13)
            assert(x == "13")
        } recover {
            failed = true
        }
        t |> success(failed)
    }
    t |> run("build_string and string iterator") @(t : T?) {
        let st = build_string() $(var writer : StringBuilderWriter) {
            write(writer, "255 hex is ")
            write(writer, fmt(":2x", 255))
        }
        t |> equal(st, "255 hex is ff")
        var count = 0
        var index = 0
        for (ch in st) {
            if (ch == '5') {
                count ++
            }
            let chAt = character_at(st, index)  // nolint:PERF003 - character_at is the tested construct
            t |> equal(ch, chAt)
            index ++
        }
        t |> equal(count, 2)
    }
    t |> run("build_string large and delete") @(t : T?) {
        var zzs = build_string() $(writer) {
            for (zzx in range(100500)) {
                write(writer, zzx)
            }
        }
        unsafe(delete_string(zzs))
        t |> success(true)
    }
}

[test(description = "the unicode escapes for code point zero decode to a null byte that ends the string, not to nothing")]
def test_unicode_null_escape(t : T?) {
    t |> run("\\u0000 and \\U00000000 decode to a null byte, not to nothing") @(t : T?) {
        t |> equal(length("a\u0000b"), 1)
        t |> equal(length("a\U00000000b"), 1)
    }
}

[test(description = "a bound das_string assigns from and compares with a string, interpolates, casts and peeks to its current text")]
def test_das_string(t : T?) {
    test_das_string() $(var s, s2 : das_string) {
        s2 := "test_das_string"
        t |> equal("{s}", "test_das_string")
        t |> equal(string(s), "test_das_string")
        peek(s) $(st) {
            t |> equal(st, "test_das_string")
        }
        t |> equal(s, "test_das_string")
        s := "out_of_it"
        t |> equal("{s}", "out_of_it")
        t |> equal(string(s), "out_of_it")
    }
}

// string by value (workhorse type, copies the pointer)
def take_string_by_value(s : string) : string => s

def take_string_by_const_value(s : string const) : string => s

def take_string_by_ref(var s : string&; new_val : string) {
    s = new_val
}

def take_string_by_const_ref(s : string const&) : string => s

// TestObjectBar by const (default for handled types)
def read_ss(bar : TestObjectBar) : string => bar.simple_string

def write_ss(var bar : TestObjectBar; val : string) {
    bar.simple_string = val
}

def read_ss_const(bar : TestObjectBar const) : string => bar.simple_string

[test(description = "a handled type's simple_string field starts empty and reads back each value written to it")]
def test_simple_string_read_write(t : T?) {
    t |> run("read and write simple_string field") @(t : T?) {
        var bar = new TestObjectBar
        t |> equal(bar.simple_string, "")
        bar.simple_string = "hello"
        t |> equal(bar.simple_string, "hello")
        bar.simple_string = "world"
        t |> equal(bar.simple_string, "world")
        unsafe {
            delete bar
        }
    }
}

[test(description = "a simple_string field passes to a string-by-value parameter intact")]
def test_simple_string_pass_value(t : T?) {
    t |> run("pass simple_string as string by value") @(t : T?) {
        var bar = new TestObjectBar
        bar.simple_string = "by_value"
        let result = take_string_by_value(bar.simple_string)
        t |> equal(result, "by_value")
        unsafe {
            delete bar
        }
    }
}

[test(description = "a simple_string field passes to a string const parameter intact")]
def test_simple_string_pass_const_value(t : T?) {
    t |> run("pass simple_string as string by const value") @(t : T?) {
        var bar = new TestObjectBar
        bar.simple_string = "by_const_value"
        let result = take_string_by_const_value(bar.simple_string)
        t |> equal(result, "by_const_value")
        unsafe {
            delete bar
        }
    }
}

[test(description = "a simple_string field passes to a string const& parameter intact")]
def test_simple_string_pass_const_ref(t : T?) {
    t |> run("pass simple_string as string by const reference") @(t : T?) {
        var bar = new TestObjectBar
        bar.simple_string = "by_const_ref"
        let result = take_string_by_const_ref(bar.simple_string)
        t |> equal(result, "by_const_ref")
        unsafe {
            delete bar
        }
    }
}

[test(description = "a string variable passed by mutable reference takes the callee's write")]
def test_simple_string_pass_ref(t : T?) {
    t |> run("pass string variable by mutable reference") @(t : T?) {
        var s = "before"
        take_string_by_ref(s, "after")
        t |> equal(s, "after")
    }
}

[test(description = "a simple_string field reads back through a TestObjectBar const parameter")]
def test_simple_string_via_bar_const(t : T?) {
    t |> run("read simple_string via TestObjectBar const") @(t : T?) {
        var bar = new TestObjectBar
        bar.simple_string = "via_const"
        let result = read_ss_const(deref(bar))
        t |> equal(result, "via_const")
        unsafe {
            delete bar
        }
    }
}

[test(description = "a simple_string field reads back through a plain TestObjectBar parameter")]
def test_simple_string_via_bar(t : T?) {
    t |> run("read simple_string via TestObjectBar") @(t : T?) {
        var bar = new TestObjectBar
        bar.simple_string = "via_bar"
        let result = read_ss(deref(bar))
        t |> equal(result, "via_bar")
        unsafe {
            delete bar
        }
    }
}

[test(description = "a write to simple_string through a var TestObjectBar parameter reaches the object")]
def test_simple_string_write_via_bar(t : T?) {
    t |> run("write simple_string via TestObjectBar mutable ref") @(t : T?) {
        var bar = new TestObjectBar
        write_ss(deref(bar), "written")
        t |> equal(bar.simple_string, "written")
        unsafe {
            delete bar
        }
    }
}

[test(description = "repeated writes to simple_string leave the last value")]
def test_simple_string_overwrite(t : T?) {
    t |> run("simple_string overwrite preserves latest value") @(t : T?) {
        var bar = new TestObjectBar
        bar.simple_string = "first"
        bar.simple_string = "second"
        bar.simple_string = "third"
        t |> equal(bar.simple_string, "third")
        unsafe {
            delete bar
        }
    }
}

def sb_test_arg(a : string) : string => "hello,{a}!"

[test(description = "string interpolation formats values and expressions, keeps escaped braces and quotes literal, and concatenates constants")]
def test_string_builder(t : T?) {
    t |> run("string interpolation") @(t : T?) {
        let empty : string = ""
        let q : string = "blah" + "blah"
        let s : string = "hello,world!"
        let s2 : string = "S = {s}"
        let s3 : string = "S2 = \{{s2}}"
        let s4 : string = "\"say\" 2 + 2 = {2+2}"
        let s5 : string = "\"say\" 2 + 2 = {2+2}{empty}"
        t |> equal(q, "blahblah")
        t |> equal(s2, "S = hello,world!")
        t |> equal(s3, "S2 = \{S = hello,world!}")
        t |> equal(s4, "\"say\" 2 + 2 = 4")
        t |> equal(s4, s5)
    }
}

def borrow_string(src : string) : string# {
    unsafe {
        return reinterpret<string#>(src)
    }
}

[test(description = "clone-initializing from a temporary string# makes a copy with its own storage")]
def test_temporary_string_clone_initialization(t : T?) {
    t |> run("string# := initialization retains a cloned string") @(t : T?) {
        let source = get_das_root()
        let retained := borrow_string(source)
        t |> equal(retained, source)
        t |> success(!same_string_storage(retained, source))
    }
}

def private byte_at(s : string; i : int) : int {
    unsafe {
        let p = reinterpret<uint8?>(s)
        return int(p[i])
    }
}

def private bytes_of(s : string) : int4 =>
    int4(length(s), byte_at(s, 1), byte_at(s, 2), byte_at(s, 3))

def private hex_escape() : int4 => bytes_of("a\x00b")

def private unicode_escape() : int4 => bytes_of("a\u0000b")

def private null_then_hex_digit() : int2 {
    let s = "\x00a"
    return int2(byte_at(s, 0), byte_at(s, 1))
}

def private returned_blob() : string => "q\x00r"

def private blob_in_builder() : string => "{"a\x00b"}tail"

let private g_blob = "g\x00h"

def private blob_key_lookups() : int3 {
    let tab <- { "k\x00a" => 7 }
    return int3(tab?["k\x00a"] ?? 0, tab?["k\x00b"] ?? 0, tab?["k"] ?? 0)
}

def private same_blob_twice() : int2 {
    let first = "d\x00e"
    let second = "d\x00e"
    return int2(byte_at(first, 2), byte_at(second, 2))
}

def private passed_blob(s : string) : int => byte_at(s, 2)

def private blobs_compare() : bool => "c\x00x" == "c\x00y"

[test(description = "a string constant with an embedded null keeps its bytes past the null as a global, an argument or a repeated constant, while table keys and comparisons read it only up to the null")]
def test_null_constant_handling(t : T?) {
    t |> run("a table key reads a blob up to its first null, the constant key and the lookup alike") @(t : T?) {
        t |> equal(blob_key_lookups(), int3(7, 7, 7))
    }
    t |> run("each copy of a repeated blob constant keeps its bytes") @(t : T?) {
        t |> equal(same_blob_twice(), int2('e', 'e'))
    }
    t |> run("a global blob keeps its bytes") @(t : T?) {
        t |> equal(bytes_of(g_blob).yz, int2(0, 'h'))
    }
    t |> run("a blob passed as an argument keeps its bytes") @(t : T?) {
        t |> equal(passed_blob("p\x00q"), 'q')
    }
    t |> run("blobs compare up to their first null") @(t : T?) {
        t |> success(blobs_compare())
    }
}

[test(description = "the hex and unicode null escapes each embed exactly one null byte, the hex escape stops after two digits, and a returned constant keeps the bytes after the null")]
def test_null_escapes(t : T?) {
    t |> run("\\x00 and \\u0000 each embed one null byte") @(t : T?) {
        let want = int4(1, 0, 'b', 0)
        t |> equal(hex_escape(), want)
        t |> equal(unicode_escape(), want)
    }
    t |> run("\\x00 reads two hex digits, a hex digit after it stays") @(t : T?) {
        t |> equal(null_then_hex_digit(), int2(0, 'a'))
    }
    t |> run("a returned constant keeps the bytes past the null") @(t : T?) {
        t |> equal(bytes_of(returned_blob()).yz, int2(0, 'r'))
    }
}

[test(description = "string interpolation reads an element with an embedded null only up to that null")]
def test_null_in_builder(t : T?) {
    t |> run("a builder reads a blob element up to its first null") @(t : T?) {
        t |> equal(blob_in_builder(), "atail")
    }
}

[test(description = "peek_data views a string's bytes in place and modify_data returns a copy with the block's byte writes applied")]
def test_peek_and_modify_string(t : T?) {
    t |> run("peek_data finds characters") @(t : T?) {
        let sample = " a + b * ( c << d ) blah blah b"
        peek_data(sample) $(foo) {
            for (x in range(length(foo))) {
                if (is_character_at(foo, x, 'b')) {
                    assert(x == 5 || x == 20 || x == 25 || x == 30)
                }
            }
        }
        t |> success(true)
    }
    t |> run("modify_data replaces characters") @(t : T?) {
        let sample = " a + b * ( c << d ) blah blah b"
        let new_sample = modify_data(sample) $(var foo) {
            for (x in range(length(foo))) {
                if (is_character_at(foo, x, 'b')) {
                    foo[x] = uint8('B')
                }
            }
        }
        t |> equal(new_sample, " a + B * ( c << d ) Blah Blah B")
    }
}

[test(description = "the common C character escapes - newline, nul, bell, vertical tab, hex and octal - lex to their code values")]
def test_c_character_escapes_lex(t : T?) {
    t |> equal(10, '\n')
    t |> equal(0, '\0')
    t |> equal(7, '\a')
    t |> equal(11, '\v')
    t |> equal(65, '\x41')
    t |> equal(65, '\101')
}

[test(description = "the remaining C character escapes, including one-digit hex and one- to three-digit octal, lex to their code values")]
def test_the_remaining_c_escapes_lex(t : T?) {
    t |> equal(8, '\b')
    t |> equal(9, '\t')
    t |> equal(12, '\f')
    t |> equal(13, '\r')
    t |> equal(92, '\\')
    t |> equal(39, '\'')
    t |> equal(34, '\"')
    t |> equal(63, '\?')
    t |> equal(15, '\xf')
    t |> equal(255, '\xFF')
    t |> equal(7, '\7')
    t |> equal(63, '\77')
    t |> equal(255, '\377')
}

[test(description = "an escaped character literal takes the u and u8 suffixes")]
def test_escapes_take_the_unsigned_suffixes(t : T?) {
    t |> equal(0u, '\0'u)
    t |> equal(11u, '\v'u)
    t |> equal(uint8(65), '\x41'u8)
    t |> equal(uint8(7), '\a'u8)
}

[test(description = "a string literal takes the same C escape set, with octal escapes stopping after three digits")]
def test_a_string_takes_the_c_escape_set(t : T?) {
    t |> equal("\x41\101", "AA")
    t |> equal("\101" + "2", "\1012")
    t |> equal("it\'s", "it's")
    peek_data("\a\v\?\377\1012") $(bytes) {
        t |> equal(length(bytes), 6)
        t |> equal(int(bytes[0]), 7)
        t |> equal(int(bytes[1]), 11)
        t |> equal(int(bytes[2]), 63)
        t |> equal(int(bytes[3]), 255)
        t |> equal(int(bytes[4]), 65)
        t |> equal(int(bytes[5]), '2')
    }
    peek_data("\4777") $(bytes) {
        t |> equal(length(bytes), 3)
        t |> equal(int(bytes[0]), 39)
        t |> equal(int(bytes[1]), '7')
    }
}

[test(description = "an embedded nul escape ends a string literal")]
def test_an_embedded_nul_ends_a_string(t : T?) {
    peek_data("a\0b") $(bytes) {
        t |> equal(length(bytes), 1)
        t |> equal(int(bytes[0]), 'a')
    }
}
