options gen2

require dastest/testing_boost
require daslib/toml
require daslib/json
require strings

def expect_ok(t : T?; what : string; input : string) : JsonValue? {
    var err : string
    var v : JsonValue? = read_toml(input, err)
    if (v == null) {
        t |> failure("{what}: expected OK but got error: {err}")
    }
    return v
}

def expect_fail(t : T?; what : string; input : string) {
    var err : string
    var v : JsonValue? = read_toml(input, err)
    if (v != null) {
        t |> failure("{what}: expected parse failure but parsed OK")
        return
    }
    if (empty(err)) {
        t |> failure("{what}: returned null but no error message")
    }
}

def get_object_field(var v : JsonValue?; key : string) : JsonValue? {
    if (v == null || !(v.value is _object)) return null
    var obj & = unsafe(v.value as _object)
    if (!key_exists(obj, key)) return null
    return obj[key]
}

[test]
def test_empty(t : T?) {
    t |> run("empty input parses to empty table") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "empty", "")
        t |> success(v != null && (v.value is _object))
        if (v != null) {
            t |> equal(length(unsafe(v.value as _object)), 0)
        }
    }
}

[test]
def test_comments_only(t : T?) {
    t |> run("comments and blank lines only") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "comments", "# top comment\n\n# another\n")
        t |> success(v != null)
        if (v != null) {
            t |> equal(length(unsafe(v.value as _object)), 0)
        }
    }
}

[test]
def test_string_value(t : T?) {
    t |> run("basic string with escapes") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "string", "name = \"a\\tb\\nc\"\n")
        var s : JsonValue? = get_object_field(v, "name")
        t |> success(s != null && (s.value is _string))
        if (s != null && (s.value is _string)) {
            t |> equal(s.value as _string, "a\tb\nc")
        }
    }
}

[test]
def test_literal_string(t : T?) {
    t |> run("literal string preserves backslashes") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "literal", "p = 'C:\\path'\n")
        var s : JsonValue? = get_object_field(v, "p")
        if (s != null && (s.value is _string)) {
            t |> equal(s.value as _string, "C:\\path")
        }
    }
}

[test]
def test_int_decimal(t : T?) {
    t |> run("decimal integer with sign and _ separator") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "int", "a = -1\nb = 1_000_000\n")
        var a : JsonValue? = get_object_field(v, "a")
        var b : JsonValue? = get_object_field(v, "b")
        if (a != null && (a.value is _longint)) {
            t |> equal(a.value as _longint, -1l)
        }
        if (b != null && (b.value is _longint)) {
            t |> equal(b.value as _longint, 1000000l)
        }
    }
}

[test]
def test_int_radix(t : T?) {
    t |> run("hex / octal / binary integers") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "radix", "h = 0xff\no = 0o755\nb = 0b1010\n")
        var h : JsonValue? = get_object_field(v, "h")
        var o : JsonValue? = get_object_field(v, "o")
        var b : JsonValue? = get_object_field(v, "b")
        if (h != null && (h.value is _longint)) t |> equal(h.value as _longint, 255l)
        if (o != null && (o.value is _longint)) t |> equal(o.value as _longint, 493l)
        if (b != null && (b.value is _longint)) t |> equal(b.value as _longint, 10l)
    }
}

[test]
def test_float(t : T?) {
    t |> run("float with decimal point and exponent") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "float", "pi = 3.14\nsci = 1e6\n")
        var pi : JsonValue? = get_object_field(v, "pi")
        var sci : JsonValue? = get_object_field(v, "sci")
        if (pi != null && (pi.value is _number)) {
            t |> equal(pi.value as _number, 3.14lf)
        }
        if (sci != null && (sci.value is _number)) {
            t |> equal(sci.value as _number, 1000000.0lf)
        }
    }
}

[test]
def test_bool(t : T?) {
    t |> run("boolean values") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "bool", "x = true\ny = false\n")
        var x : JsonValue? = get_object_field(v, "x")
        var y : JsonValue? = get_object_field(v, "y")
        if (x != null && (x.value is _bool)) t |> success(x.value as _bool)
        if (y != null && (y.value is _bool)) t |> success(!(y.value as _bool))
    }
}

[test]
def test_array(t : T?) {
    t |> run("inline and multi-line arrays both work") @@(t : T?) {
        var v1 : JsonValue? = expect_ok(t, "inline", "xs = [1, 2, 3]\n")
        var v2 : JsonValue? = expect_ok(t, "multiline", "xs = [\n  1,\n  2,\n  3,\n]\n")
        for (vv in [unsafe(reinterpret<JsonValue?>(v1)), unsafe(reinterpret<JsonValue?>(v2))]) {
            var xs : JsonValue? = get_object_field(vv, "xs")
            if (xs != null && (xs.value is _array)) {
                let arr & = unsafe(xs.value as _array)
                t |> equal(length(arr), 3)
            }
        }
    }
}

[test]
def test_inline_table(t : T?) {
    t |> run("inline table single-line") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "inline_table", "p = \{ x = 1, y = 2 \}\n")
        var p : JsonValue? = get_object_field(v, "p")
        var x : JsonValue? = get_object_field(p, "x")
        var y : JsonValue? = get_object_field(p, "y")
        if (x != null && (x.value is _longint)) t |> equal(x.value as _longint, 1l)
        if (y != null && (y.value is _longint)) t |> equal(y.value as _longint, 2l)
    }
}

[test]
def test_section(t : T?) {
    t |> run("[section] table header") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "section", "[server]\nhost = \"localhost\"\nport = 8080\n")
        var server : JsonValue? = get_object_field(v, "server")
        var host : JsonValue? = get_object_field(server, "host")
        var port : JsonValue? = get_object_field(server, "port")
        if (host != null && (host.value is _string)) {
            t |> equal(host.value as _string, "localhost")
        }
        if (port != null && (port.value is _longint)) {
            t |> equal(port.value as _longint, 8080l)
        }
    }
}

[test]
def test_dotted_section(t : T?) {
    t |> run("[a.b] dotted section creates nested tables") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "dotted", "[a.b]\nv = 1\n")
        var a : JsonValue? = get_object_field(v, "a")
        var b : JsonValue? = get_object_field(a, "b")
        var vv : JsonValue? = get_object_field(b, "v")
        if (vv != null && (vv.value is _longint)) {
            t |> equal(vv.value as _longint, 1l)
        }
    }
}

[test]
def test_dotted_key(t : T?) {
    t |> run("a.b.c = 1 creates nested tables") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "dotted_key", "a.b.c = 1\n")
        var a : JsonValue? = get_object_field(v, "a")
        var b : JsonValue? = get_object_field(a, "b")
        var c : JsonValue? = get_object_field(b, "c")
        if (c != null && (c.value is _longint)) {
            t |> equal(c.value as _longint, 1l)
        }
    }
}

[test]
def test_array_of_tables(t : T?) {
    t |> run("[[items]] appends entries to an array") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "aot", "[[items]]\nx = 1\n[[items]]\nx = 2\n")
        var items : JsonValue? = get_object_field(v, "items")
        if (items != null && (items.value is _array)) {
            let arr & = unsafe(items.value as _array)
            t |> equal(length(arr), 2)
        }
    }
}

[test]
def test_inf_nan(t : T?) {
    t |> run("inf, -inf, nan") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "inf_nan", "p = inf\nm = -inf\nn = nan\n")
        var p : JsonValue? = get_object_field(v, "p")
        var m : JsonValue? = get_object_field(v, "m")
        var n : JsonValue? = get_object_field(v, "n")
        if (p != null && (p.value is _number)) {
            t |> success((p.value as _number) > 1.0e300lf)
        }
        if (m != null && (m.value is _number)) {
            t |> success((m.value as _number) < -1.0e300lf)
        }
        if (n != null && (n.value is _number)) {
            let nv = n.value as _number
            t |> success(nv != nv)   // NaN != NaN
        }
    }
}

[test]
def test_datetime_preserved(t : T?) {
    t |> run("datetime values preserved as raw strings") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "datetime", "t = 2024-01-15T10:30:00Z\n")
        var dt : JsonValue? = get_object_field(v, "t")
        if (dt != null && (dt.value is _string)) {
            t |> equal(dt.value as _string, "2024-01-15T10:30:00Z")
        }
    }
}

[test]
def test_rules_shape(t : T?) {
    t |> run("[rules] table-of-bools (the .lint_config shape)") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "rules", "[rules]\nSTYLE005 = true\nPERF007 = false\n")
        var r : JsonValue? = get_object_field(v, "rules")
        var s5 : JsonValue? = get_object_field(r, "STYLE005")
        var p7 : JsonValue? = get_object_field(r, "PERF007")
        if (s5 != null && (s5.value is _bool)) t |> success(s5.value as _bool)
        if (p7 != null && (p7.value is _bool)) t |> success(!(p7.value as _bool))
    }
}

[test]
def test_date_shaped_bare_key(t : T?) {
    t |> run("date-shaped bare key is accepted (per TOML ABNF)") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "date_key", "2024-01-15 = 1\n")
        var dk : JsonValue? = get_object_field(v, "2024-01-15")
        if (dk != null && (dk.value is _longint)) {
            t |> equal(dk.value as _longint, 1l)
        }
    }
}

[test]
def test_inf_bare_key(t : T?) {
    t |> run("'inf' / 'nan' are valid bare keys (no leading sign)") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "inf_key", "inf = 1\nnan = 2\n")
        var ik : JsonValue? = get_object_field(v, "inf")
        var nk : JsonValue? = get_object_field(v, "nan")
        if (ik != null && (ik.value is _longint)) t |> equal(ik.value as _longint, 1l)
        if (nk != null && (nk.value is _longint)) t |> equal(nk.value as _longint, 2l)
    }
}

[test]
def test_inf_prefix_bare_key(t : T?) {
    t |> run("identifiers starting with 'inf' / 'nan' are not split (infinity_count)") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "inf_prefix", "infinity_count = 1\nnanoseconds = 2\n")
        var a : JsonValue? = get_object_field(v, "infinity_count")
        var b : JsonValue? = get_object_field(v, "nanoseconds")
        if (a != null && (a.value is _longint)) t |> equal(a.value as _longint, 1l)
        if (b != null && (b.value is _longint)) t |> equal(b.value as _longint, 2l)
    }
}

[test]
def test_dash_prefix_bare_key(t : T?) {
    t |> run("bare keys may start with '-' per TOML ABNF") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "dash_key", "-foo = 1\n")
        var dk : JsonValue? = get_object_field(v, "-foo")
        if (dk != null && (dk.value is _longint)) {
            t |> equal(dk.value as _longint, 1l)
        }
    }
}

[test]
def test_alphanum_bare_key(t : T?) {
    t |> run("bare keys may start with digits and contain alpha (123abc)") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "alphanum_key", "123abc = 1\n0xff_oops = 2\n")
        var a : JsonValue? = get_object_field(v, "123abc")
        var b : JsonValue? = get_object_field(v, "0xff_oops")
        if (a != null && (a.value is _longint)) t |> equal(a.value as _longint, 1l)
        if (b != null && (b.value is _longint)) t |> equal(b.value as _longint, 2l)
    }
}

[test]
def test_inf_nan_still_work_as_values(t : T?) {
    t |> run("'x = inf' / 'x = -inf' / 'x = nan' still interpret as float values") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "inf_val", "p = inf\nm = -inf\nq = nan\n")
        var p : JsonValue? = get_object_field(v, "p")
        var m : JsonValue? = get_object_field(v, "m")
        var q : JsonValue? = get_object_field(v, "q")
        if (p != null && (p.value is _number)) t |> success((p.value as _number) > 1.0e300lf)
        if (m != null && (m.value is _number)) t |> success((m.value as _number) < -1.0e300lf)
        if (q != null && (q.value is _number)) {
            let nv = q.value as _number
            t |> success(nv != nv)
        }
    }
}

[test]
def test_fail_underscore_trailing(t : T?) {
    t |> run("trailing underscore in number is rejected") @@(t : T?) {
        expect_fail(t, "trail_underscore", "x = 42_\n")
    }
}

[test]
def test_fail_underscore_double(t : T?) {
    t |> run("doubled underscore in number is rejected") @@(t : T?) {
        expect_fail(t, "double_underscore", "x = 4__2\n")
    }
}

[test]
def test_fail_underscore_hex_leading(t : T?) {
    t |> run("leading underscore after radix prefix is rejected") @@(t : T?) {
        expect_fail(t, "hex_underscore", "x = 0x_ff\n")
    }
}

[test]
def test_fail_underscore_float_fractional(t : T?) {
    t |> run("trailing underscore in fractional part is rejected") @@(t : T?) {
        expect_fail(t, "frac_underscore", "x = 3.14_\n")
    }
}

[test]
def test_valid_underscores(t : T?) {
    t |> run("'_' between digits parses correctly across decimal / hex / float / exp") @@(t : T?) {
        var v : JsonValue? = expect_ok(t, "valid_underscores",
            "a = 1_000\nb = 0xff_aa\nc = 3.141_592\nd = 1_0e1_0\n")
        var a : JsonValue? = get_object_field(v, "a")
        var b : JsonValue? = get_object_field(v, "b")
        if (a != null && (a.value is _longint)) t |> equal(a.value as _longint, 1000l)
        if (b != null && (b.value is _longint)) t |> equal(b.value as _longint, int64(0xffaa))
    }
}

[test]
def test_fail_exponent_no_digits(t : T?) {
    t |> run("exponent without digits ('1e') is rejected") @@(t : T?) {
        expect_fail(t, "exp_no_digits", "x = 1e\n")
    }
}

[test]
def test_fail_exponent_sign_no_digits(t : T?) {
    t |> run("exponent with sign but no digits ('1e+') is rejected") @@(t : T?) {
        expect_fail(t, "exp_sign_no_digits", "x = 1e+\n")
    }
}

def expect_fail_with_eof(t : T?; what : string; input : string) {
    var err : string
    var v : JsonValue? = read_toml(input, err)
    if (v != null) {
        t |> failure("{what}: expected parse failure but parsed OK")
        return
    }
    // EOF diagnostics must NOT report the default '0:0' position and SHOULD
    // mention end-of-input so the user knows the input was truncated.
    if (find(err, "0:0") >= 0) {
        t |> failure("{what}: EOF error reports stale '0:0' location: '{err}'")
    }
    if (find(err, "end of input") < 0) {
        t |> failure("{what}: EOF error doesn't say 'end of input': '{err}'")
    }
}

[test]
def test_fail_eof_after_key_no_equals(t : T?) {
    t |> run("EOF after a top-level key reports 'end of input' (not '0:0')") @@(t : T?) {
        expect_fail_with_eof(t, "eof_no_equals", "a")
    }
}

[test]
def test_fail_eof_in_section_header(t : T?) {
    t |> run("EOF inside section header reports 'end of input'") @@(t : T?) {
        expect_fail_with_eof(t, "eof_section", "[a")
    }
}

[test]
def test_fail_eof_in_aot_header(t : T?) {
    t |> run("EOF inside [[items header reports 'end of input'") @@(t : T?) {
        expect_fail_with_eof(t, "eof_aot", "[[a]")
    }
}

[test]
def test_fail_eof_in_inline_table(t : T?) {
    t |> run("EOF inside inline table reports 'end of input'") @@(t : T?) {
        expect_fail_with_eof(t, "eof_inline", "x = \{ a")
    }
}

[test]
def test_fail_unclosed_string(t : T?) {
    t |> run("unterminated basic string is rejected") @@(t : T?) {
        expect_fail(t, "unclosed", "name = \"oops\n")
    }
}

[test]
def test_fail_unclosed_array(t : T?) {
    t |> run("unterminated array is rejected") @@(t : T?) {
        expect_fail(t, "unclosed_array", "xs = [1, 2\n")
    }
}

[test]
def test_fail_duplicate_key(t : T?) {
    t |> run("duplicate key in same table is rejected") @@(t : T?) {
        expect_fail(t, "dup", "a = 1\na = 2\n")
    }
}

[test]
def test_fail_missing_equals(t : T?) {
    t |> run("key without = is rejected") @@(t : T?) {
        expect_fail(t, "no_equals", "key value\n")
    }
}
