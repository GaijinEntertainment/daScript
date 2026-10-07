options gen2
options indenting = 4
options strict_smart_pointers = true
options _cyclomatic_complexity = 50   // a port of the reference converter: its visitor and pattern lexer keep the reference's shape
options _function_length = 170

module json_schema shared public

require lr1/lr1
require lr1/gbnf
require daslib/strings_boost
require strings
require math

// A JSON schema compiled to a GBNF grammar whose sentences are exactly the documents the schema admits,
// then to the engine's tables. The emitted text follows llama.cpp's schema converter rule for rule, so its
// conversion corpus is this compiler's oracle: object properties in declaration order with the required
// ones first, optional properties as a chain of optional tails, `enum`/`const` as literals, integer
// `minimum`/`maximum` as digit-range alternatives, `minLength`/`maxLength` and `minItems`/`maxItems` as
// repetitions, `$ref` to a local definition, `anyOf`/`oneOf` as alternatives, `allOf` merged, string
// `pattern` as a regular expression over the string's characters, and the `date`/`time`/`uuid` formats.
// Object keys keep their declaration order, which daslib's JSON tables drop, so the schema is parsed here.

// ===== an order-keeping JSON tree for the schema =====

enum SKind {
    s_null
    s_bool
    s_number
    s_string
    s_array
    s_object
}

//! One node of the schema document. Numbers keep their source spelling (`num_text`) for exact dumps.
struct SNode {
    kind : SKind
    flag : bool
    num : double
    num_text : string
    str : string
    items : array<SNode?>
    keys : array<string>
    vals : array<SNode?>
}

//! A read-only view of a node: the document owns every node, readers borrow.
typedef SRef = SNode const?

def private snode_get(n : SRef; key : string) : SRef {
    if (n == null || n.kind != SKind.s_object) {
        return null
    }
    for (i, k in count(), n.keys) {
        if (k == key) {
            return n.vals[i]
        }
    }
    return null
}

def private snode_has(n : SRef; key : string) : bool => snode_get(n, key) != null

def private snode_str(n : SRef; key : string) : string {
    let v = snode_get(n, key)
    return v != null && v.kind == SKind.s_string ? v.str : ""
}

def private snode_int(n : SRef; key : string; dflt : int64) : int64 {
    let v = snode_get(n, key)
    return v != null && v.kind == SKind.s_number ? int64(v.num) : dflt
}

struct private SParser {
    text : array<uint8>
    pos : int
    err : string
}

def private sp_peek(p : SParser) : int => p.pos < length(p.text) ? int(p.text[p.pos]) : -1

def private sp_ws(var p : SParser) {
    while (p.pos < length(p.text)) {
        let c = sp_peek(p)
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            p.pos++
        } else {
            break
        }
    }
}

def private sp_fail(var p : SParser; what : string) {
    if (empty(p.err)) {
        p.err = "json schema: {what} at byte {p.pos}"
    }
}

def private sp_expect(var p : SParser; c : int) : bool {
    sp_ws(p)
    if (sp_peek(p) != c) {
        sp_fail(p, "expected '{to_char(c)}'")
        return false
    }
    p.pos++
    return true
}

def private hex4(var p : SParser) : int {
    var v = 0
    for (_ in range(4)) {
        let c = sp_peek(p)
        var h = -1
        if (is_number(c)) {
            h = c - '0'
        } elif (c >= 'a' && c <= 'f') {
            h = c - 'a' + 10
        } elif (c >= 'A' && c <= 'F') {
            h = c - 'A' + 10
        }
        if (h < 0) {
            sp_fail(p, "bad \\u escape")
            return 0
        }
        v = v * 16 + h
        p.pos++
    }
    return v
}

def private write_cp(var w : StringBuilderWriter; cp : int) {
    if (cp < 128) {
        w |> write(to_char(cp))
    } elif (cp < 2048) {
        w |> write(to_char(192 | (cp >> 6)))
        w |> write(to_char(128 | (cp & 63)))
    } elif (cp < 65536) {
        w |> write(to_char(224 | (cp >> 12)))
        w |> write(to_char(128 | ((cp >> 6) & 63)))
        w |> write(to_char(128 | (cp & 63)))
    } else {
        w |> write(to_char(240 | (cp >> 18)))
        w |> write(to_char(128 | ((cp >> 12) & 63)))
        w |> write(to_char(128 | ((cp >> 6) & 63)))
        w |> write(to_char(128 | (cp & 63)))
    }
}

// a JSON string with the opening quote consumed
def private sp_string(var p : SParser) : string {
    return build_string() $(var w) {
        while (true) {
            let c = sp_peek(p)
            if (c < 0) {
                sp_fail(p, "unterminated string")
                return
            }
            p.pos++
            if (c == '"') {
                return
            }
            if (c != '\\') {
                w |> write(to_char(c))
                continue
            }
            let e = sp_peek(p)
            p.pos++
            if (e == 'n') {
                w |> write("\n")
            } elif (e == 't') {
                w |> write("\t")
            } elif (e == 'r') {
                w |> write("\r")
            } elif (e == 'b') {
                w |> write(to_char(8))
            } elif (e == 'f') {
                w |> write(to_char(12))
            } elif (e == '/' || e == '"' || e == '\\') {
                w |> write(to_char(e))
            } elif (e == 'u') {
                var cp = hex4(p)
                if (cp >= 0xD800 && cp < 0xDC00 && sp_peek(p) == '\\') {
                    p.pos += 2
                    let lo = hex4(p)
                    cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00)
                }
                write_cp(w, cp)
            } else {
                sp_fail(p, "bad escape")
                return
            }
        }
    }
}

def private sp_value(var p : SParser) : SNode? {
    sp_ws(p)
    let c = sp_peek(p)
    var n = new SNode
    if (c == '{') {
        p.pos++
        n.kind = SKind.s_object
        sp_ws(p)
        if (sp_peek(p) == '}') {
            p.pos++
            return n
        }
        while (empty(p.err)) {
            if (!sp_expect(p, '"')) {
                break
            }
            let key = sp_string(p)
            break if (!sp_expect(p, ':'))
            var v = sp_value(p)
            n.keys |> push(key)
            n.vals |> emplace(v)
            sp_ws(p)
            if (sp_peek(p) == ',') {
                p.pos++
                continue
            }
            sp_expect(p, '}')
            break
        }
    } elif (c == '[') {
        p.pos++
        n.kind = SKind.s_array
        sp_ws(p)
        if (sp_peek(p) == ']') {
            p.pos++
            return n
        }
        while (empty(p.err)) {
            var v = sp_value(p)
            n.items |> emplace(v)
            sp_ws(p)
            if (sp_peek(p) == ',') {
                p.pos++
                continue
            }
            sp_expect(p, ']')
            break
        }
    } elif (c == '"') {
        p.pos++
        n.kind = SKind.s_string
        n.str = sp_string(p)
    } elif (c == 't' || c == 'f' || c == 'n') {
        let start = p.pos
        while (p.pos < length(p.text) && is_alpha(sp_peek(p))) {
            p.pos++
        }
        let word = build_string() $(var w) {
            for (i in range(start, p.pos)) {
                w |> write(to_char(int(p.text[i])))
            }
        }
        if (word == "true" || word == "false") {
            n.kind = SKind.s_bool
            n.flag = word == "true"
        } elif (word == "null") {
            n.kind = SKind.s_null
        } else {
            sp_fail(p, "unexpected word '{word}'")
        }
    } elif (c == '-' || is_number(c)) {
        let start = p.pos
        while (p.pos < length(p.text)) {
            let d = sp_peek(p)
            if (is_number(d) || d == '-' || d == '+' || d == '.' || d == 'e' || d == 'E') {
                p.pos++
            } else {
                break
            }
        }
        n.kind = SKind.s_number
        n.num_text = build_string() $(var w) {
            for (i in range(start, p.pos)) {
                w |> write(to_char(int(p.text[i])))
            }
        }
        n.num = to_double(n.num_text)
    } else {
        sp_fail(p, "unexpected character")
    }
    return n
}

//! Parse schema text into an order-keeping tree; null with `err` set on malformed JSON. The caller owns
//! the tree (`delete`).
def parse_schema(text : string; var err : string&) : SNode? {
    var p : SParser
    peek_data(text) $(d : array<uint8> const#) {
        p.text |> resize(length(d))
        for (i, b in count(), d) {
            p.text[i] = b
        }
    }
    var n = sp_value(p)
    sp_ws(p)
    if (empty(p.err) && p.pos < length(p.text)) {
        sp_fail(p, "trailing characters")
    }
    if (!empty(p.err)) {
        err = p.err
        unsafe {
            delete n
        }
        return null
    }
    return n
}

// the compact dump the converter's literals are built from: `{"a":1,"b":[true,null]}`, strings escaped
// the JSON way (control characters as \uXXXX), numbers as spelled
def private dump_compact(n : SRef; var w : StringBuilderWriter) {
    if (n == null) {
        w |> write("null")
        return
    }
    if (n.kind == SKind.s_null) {
        w |> write("null")
    } elif (n.kind == SKind.s_bool) {
        w |> write(n.flag ? "true" : "false")
    } elif (n.kind == SKind.s_number) {
        w |> write(n.num_text)
    } elif (n.kind == SKind.s_string) {
        dump_string(n.str, w)
    } elif (n.kind == SKind.s_array) {
        w |> write("[")
        for (i, it in count(), n.items) {
            if (i > 0) {
                w |> write(",")
            }
            dump_compact(it, w)
        }
        w |> write("]")
    } else {
        w |> write("\{")
        for (i, k in count(), n.keys) {
            if (i > 0) {
                w |> write(",")
            }
            dump_string(k, w)
            w |> write(":")
            dump_compact(n.vals[i], w)
        }
        w |> write("\}")
    }
}

def private dump_string(s : string; var w : StringBuilderWriter) {
    w |> write("\"")
    peek_data(s) $(d : array<uint8> const#) {
        for (b in d) {
            let c = int(b)
            if (c == '"') {
                w |> write("\\\"")
            } elif (c == '\\') {
                w |> write("\\\\")
            } elif (c == '\n') {
                w |> write("\\n")
            } elif (c == '\r') {
                w |> write("\\r")
            } elif (c == '\t') {
                w |> write("\\t")
            } elif (c == 8) {
                w |> write("\\b")
            } elif (c == 12) {
                w |> write("\\f")
            } elif (c < 32) {
                w |> write("\\u00")
                w |> write(to_char(c < 16 ? '0' : '1'))
                let lo = c & 15
                w |> write(to_char(lo < 10 ? '0' + lo : 'a' + lo - 10))
            } else {
                w |> write(to_char(c))
            }
        }
    }
    w |> write("\"")
}

def private dump(n : SRef) : string {
    return build_string() $(var w) {
        dump_compact(n, w)
    }
}

// ===== the converter =====

let private SPACE_RULE = "| \" \" | \"\\n\"\{1,2\} [ \\t]\{0,20\}"

struct private BuiltinRule {
    content : string
    deps : array<string>
}

def private builtin(content : string; deps : array<string>) : BuiltinRule {
    var r : BuiltinRule
    r.content = content
    r.deps := deps
    return <- r
}

// the primitive rule `name`, `found` false (and an empty rule) for a name that is none
def private primitive_rule(name : string; var found : bool&) : BuiltinRule {
    found = true
    if (name == "boolean") {
        return <- builtin("(\"true\" | \"false\")", [])
    } elif (name == "decimal-part") {
        return <- builtin("[0-9]\{1,16\}", [])
    } elif (name == "integral-part") {
        return <- builtin("[0] | [1-9] [0-9]\{0,15\}", [])
    } elif (name == "number") {
        return <- builtin("(\"-\"? integral-part) (\".\" decimal-part)? ([eE] [-+]? integral-part)?", ["integral-part", "decimal-part"])
    } elif (name == "integer") {
        return <- builtin("(\"-\"? integral-part)", ["integral-part"])
    } elif (name == "value") {
        return <- builtin("object | array | string | number | boolean | null", ["object", "array", "string", "number", "boolean", "null"])
    } elif (name == "object") {
        return <- builtin("\"\{\" space ( string \":\" space value (\",\" space string \":\" space value)* )? space \"\}\"", ["string", "value"])
    } elif (name == "array") {
        return <- builtin("\"[\" space ( value (\",\" space value)* )? space \"]\"", ["value"])
    } elif (name == "uuid") {
        return <- builtin("\"\\\"\" [0-9a-fA-F]\{8\} \"-\" [0-9a-fA-F]\{4\} \"-\" [0-9a-fA-F]\{4\} \"-\" [0-9a-fA-F]\{4\} \"-\" [0-9a-fA-F]\{12\} \"\\\"\"", [])
    } elif (name == "char") {
        return <- builtin("[^\"\\\\\\x7F\\x00-\\x1F] | [\\\\] ([\"\\\\bfnrt] | \"u\" [0-9a-fA-F]\{4\})", [])
    } elif (name == "string") {
        return <- builtin("\"\\\"\" char* \"\\\"\"", ["char"])
    } elif (name == "null") {
        return <- builtin("\"null\"", [])
    }
    found = false
    return <- builtin("", [])
}

// the string-format rule `name`, `found` false for a name that is none
def private format_rule(name : string; var found : bool&) : BuiltinRule {
    found = true
    if (name == "date") {
        return <- builtin("[0-9]\{4\} \"-\" ( \"0\" [1-9] | \"1\" [0-2] ) \"-\" ( \"0\" [1-9] | [1-2] [0-9] | \"3\" [0-1] )", [])
    } elif (name == "time") {
        return <- builtin("([01] [0-9] | \"2\" [0-3]) \":\" [0-5] [0-9] \":\" [0-5] [0-9] ( \".\" [0-9]\{3\} )? ( \"Z\" | ( \"+\" | \"-\" ) ( [01] [0-9] | \"2\" [0-3] ) \":\" [0-5] [0-9] )", [])
    } elif (name == "date-time") {
        return <- builtin("date \"T\" time", ["date", "time"])
    } elif (name == "date-string") {
        return <- builtin("\"\\\"\" date \"\\\"\"", ["date"])
    } elif (name == "time-string") {
        return <- builtin("\"\\\"\" time \"\\\"\"", ["time"])
    } elif (name == "date-time-string") {
        return <- builtin("\"\\\"\" date-time \"\\\"\"", ["date-time"])
    }
    found = false
    return <- builtin("", [])
}

def private is_reserved_name(name : string) : bool {
    var is_prim = false
    var is_fmt = false
    primitive_rule(name, is_prim)
    format_rule(name, is_fmt)
    return name == "root" || is_prim || is_fmt
}

// a GBNF literal: `"` and `\` escaped, newlines spelled
def private format_literal(s : string) : string {
    return build_string() $(var w) {
        w |> write("\"")
        peek_data(s) $(d : array<uint8> const#) {
            for (b in d) {
                let c = int(b)
                if (c == '\r') {
                    w |> write("\\r")
                } elif (c == '\n') {
                    w |> write("\\n")
                } elif (c == '"') {
                    w |> write("\\\"")
                } elif (c == '\\') {
                    w |> write("\\\\")
                } else {
                    w |> write(to_char(c))
                }
            }
        }
        w |> write("\"")
    }
}

// `x`, `x?`, `x+`, `x*`, `x{m,n}`, with an optional separator between repeats
def private build_repetition(item : string; min_items, max_items : int; separator : string = "") : string {
    let has_max = max_items != INT_MAX
    if (max_items == 0) {
        return ""
    }
    if (min_items == 0 && max_items == 1) {
        return "{item}?"
    }
    if (empty(separator)) {
        if (min_items == 1 && !has_max) {
            return "{item}+"
        }
        if (min_items == 0 && !has_max) {
            return "{item}*"
        }
        let hi = has_max ? "{max_items}" : ""
        return "{item}\{{min_items},{hi}\}"
    }
    let grouped = "({separator} {item})"
    let rest = build_repetition(grouped, min_items == 0 ? 0 : min_items - 1, has_max ? max_items - 1 : max_items)
    var result = "{item} {rest}"
    if (min_items == 0) {
        result = "({result})?"
    }
    return result
}

let private INT64_MIN = -9223372036854775807l - 1l
let private INT64_MAX = 9223372036854775807l

def private repeat_digit(d : int; n : int) : array<int> {
    return <- [for (_ in range(n)); d]
}

def private one_then_zeros(digits : int) : array<int> {
    return <- [for (k in range(digits)); k == 0 ? '1' : '0']
}

def private digits_of(v : int64) : array<int> {
    var out : array<int>
    peek_data("{v}") $(d : array<uint8> const#) {
        out |> reserve(length(d))
        for (b in d) {
            out |> push(int(b))
        }
    }
    return <- out
}

def private write_digits(var w : StringBuilderWriter; ds : array<int>; from, to : int) {
    for (i in range(from, to)) {
        w |> write(to_char(ds[i]))
    }
}

def private digit_range(var w : StringBuilderWriter; from, to : int) {
    w |> write("[")
    if (from == to) {
        w |> write(to_char(from))
    } else {
        w |> write(to_char(from))
        w |> write("-")
        w |> write(to_char(to))
    }
    w |> write("]")
}

def private more_digits(var w : StringBuilderWriter; min_digits, max_digits : int) {
    w |> write("[0-9]")
    if (min_digits == max_digits && min_digits == 1) {
        return
    }
    w |> write("\{")
    w |> write(min_digits)
    if (max_digits != min_digits) {
        w |> write(",")
        if (max_digits != INT_MAX) {
            w |> write(max_digits)
        }
    }
    w |> write("\}")
}

def private tail_of(ds : array<int>; from : int) : array<int> {
    return <- [for (i in range(from, length(ds))); ds[i]]
}

def private same_digits(a, b : array<int>) : bool {
    return false if (length(a) != length(b))
    for (x, y in a, b) {
        return false if (x != y)
    }
    return true
}

// the digit-by-digit alternatives of the integers from `from` to `to`, both digit strings of one
// length (the converter's uniform_range)
def private uniform_range(var w : StringBuilderWriter; from, to : array<int>) {
    var i = 0
    let n = length(from)
    while (i < n && i < length(to) && from[i] == to[i]) {
        i++
    }
    if (i > 0) {
        w |> write("\"")
        write_digits(w, from, 0, i)
        w |> write("\"")
    }
    if (i < n && i < length(to)) {
        if (i > 0) {
            w |> write(" ")
        }
        let sub_len = n - i - 1
        if (sub_len > 0) {
            let from_sub <- tail_of(from, i + 1)
            let to_sub <- tail_of(to, i + 1)
            let sub_zeros <- repeat_digit('0', sub_len)
            let sub_nines <- repeat_digit('9', sub_len)
            var to_reached = false
            w |> write("(")
            if (same_digits(from_sub, sub_zeros)) {
                digit_range(w, from[i], to[i] - 1)
                w |> write(" ")
                more_digits(w, sub_len, sub_len)
            } else {
                w |> write("[")
                w |> write(to_char(from[i]))
                w |> write("] ")
                w |> write("(")
                uniform_range(w, from_sub, sub_nines)
                w |> write(")")
                if (from[i] < to[i] - 1) {
                    w |> write(" | ")
                    if (same_digits(to_sub, sub_nines)) {
                        digit_range(w, from[i] + 1, to[i])
                        to_reached = true
                    } else {
                        digit_range(w, from[i] + 1, to[i] - 1)
                    }
                    w |> write(" ")
                    more_digits(w, sub_len, sub_len)
                }
            }
            if (!to_reached) {
                w |> write(" | ")
                digit_range(w, to[i], to[i])
                w |> write(" ")
                uniform_range(w, sub_zeros, to_sub)
            }
            w |> write(")")
        } else {
            w |> write("[")
            w |> write(to_char(from[i]))
            w |> write("-")
            w |> write(to_char(to[i]))
            w |> write("]")
        }
    }
}

// the alternatives of every integer in [min_value, max_value]; INT64_MIN / INT64_MAX = unbounded
def private build_min_max_int(min_value, max_value : int64; var w : StringBuilderWriter; decimals_left : int = 16; top_level : bool = true) {
    let has_min = min_value != INT64_MIN
    let has_max = max_value != INT64_MAX
    if (has_min && has_max) {
        if (min_value < 0l && max_value < 0l) {
            w |> write("\"-\" (")
            build_min_max_int(-max_value, -min_value, w, decimals_left, true)
            w |> write(")")
            return
        }
        var lo = min_value
        if (lo < 0l) {
            w |> write("\"-\" (")
            build_min_max_int(0l, -lo, w, decimals_left, true)
            w |> write(") | ")
            lo = 0l
        }
        let lo_s <- digits_of(lo)
        let max_s <- digits_of(max_value)
        let min_digits = length(lo_s)
        let max_digits = length(max_s)
        for (digits in range(min_digits, max_digits)) {
            // the first band starts at lo, every wider one at 1 followed by zeros
            let band_lo <- digits == min_digits ? digits_of(lo) : one_then_zeros(digits)
            let nines <- repeat_digit('9', digits)
            uniform_range(w, band_lo, nines)
            w |> write(" | ")
        }
        let last_lo <- max_digits == min_digits ? digits_of(lo) : one_then_zeros(max_digits)
        uniform_range(w, last_lo, max_s)
        return
    }
    let less_decimals = max(decimals_left - 1, 1)
    if (has_min) {
        if (min_value < 0l) {
            w |> write("\"-\" (")
            build_min_max_int(INT64_MIN, -min_value, w, decimals_left, false)
            w |> write(") | [0] | [1-9] ")
            more_digits(w, 0, decimals_left - 1)
        } elif (min_value == 0l) {
            if (top_level) {
                w |> write("[0] | [1-9] ")
                more_digits(w, 0, less_decimals)
            } else {
                more_digits(w, 1, decimals_left)
            }
        } elif (min_value <= 9l) {
            let c = '0' + int(min_value)
            let range_start = top_level ? '1' : '0'
            if (c > range_start) {
                digit_range(w, range_start, c - 1)
                w |> write(" ")
                more_digits(w, 1, less_decimals)
                w |> write(" | ")
            }
            digit_range(w, c, '9')
            w |> write(" ")
            more_digits(w, 0, less_decimals)
        } else {
            let min_s <- digits_of(min_value)
            let len = length(min_s)
            let c = min_s[0]
            if (c > '1') {
                digit_range(w, top_level ? '1' : '0', c - 1)
                w |> write(" ")
                more_digits(w, len, less_decimals)
                w |> write(" | ")
            }
            digit_range(w, c, c)
            w |> write(" (")
            // the value without its leading digit
            var pow = 1l
            for (_ in range(len - 1)) {
                pow *= 10l
            }
            build_min_max_int(min_value - int64(c - '0') * pow, INT64_MAX, w, less_decimals, false)
            w |> write(")")
            if (c < '9') {
                w |> write(" | ")
                digit_range(w, c + 1, '9')
                w |> write(" ")
                more_digits(w, len - 1, less_decimals)
            }
        }
        return
    }
    if (has_max) {
        if (max_value >= 0l) {
            if (top_level) {
                w |> write("\"-\" [1-9] ")
                more_digits(w, 0, less_decimals)
                w |> write(" | ")
            }
            build_min_max_int(0l, max_value, w, decimals_left, true)
        } else {
            w |> write("\"-\" (")
            build_min_max_int(-max_value, INT64_MAX, w, decimals_left, false)
            w |> write(")")
        }
    }
}

//! The compiler's state: the rules so far, the local definitions `$ref` names, the first error.
struct private Conv {
    rules : table<string; string>
    refs : table<string; SRef>      //!< `#/...` -> the subtree (borrowed from the document)
    resolving : table<string>
    err : string
}

def private conv_fail(var c : Conv; what : string) {
    if (empty(c.err)) {
        c.err = "json schema: {what}"
    }
}

// rule names are `[a-zA-Z0-9-]`; anything else collapses to one dash
def private sanitize_name(name : string) : string {
    return build_string() $(var w) {
        var dash = false
        peek_data(name) $(d : array<uint8> const#) {
            for (b in d) {
                let ch = int(b)
                if (is_alpha(ch) || is_number(ch) || ch == '-') {
                    w |> write(to_char(ch))
                    dash = false
                } elif (!dash) {
                    w |> write("-")
                    dash = true
                }
            }
        }
    }
}

// the rule under `name` (suffixed 0, 1, ... when the name is taken by a different body)
def private add_rule(var c : Conv; name : string; rule : string) : string {
    let esc = sanitize_name(name)
    if (!(c.rules |> key_exists(esc)) || c.rules[esc] == rule) {
        c.rules[esc] = rule
        return esc
    }
    var i = 0
    while ((c.rules |> key_exists("{esc}{i}")) && c.rules["{esc}{i}"] != rule) {
        i++
    }
    let key = "{esc}{i}"
    c.rules[key] = rule
    return key
}

def private add_primitive(var c : Conv; name : string; rule : BuiltinRule) : string {
    let n = add_rule(c, name, rule.content)
    for (dep in rule.deps) {
        var is_prim = false
        var is_fmt = false
        primitive_rule(dep, is_prim)
        format_rule(dep, is_fmt)
        if (!is_prim && !is_fmt) {
            conv_fail(c, "rule {dep} not known")
            continue
        }
        let dr <- is_prim ? primitive_rule(dep, is_prim) : format_rule(dep, is_fmt)
        if (!(c.rules |> key_exists(dep))) {
            add_primitive(c, dep, dr)
        }
    }
    return n
}

def private add_primitive_named(var c : Conv; name : string) : string {
    var found = false
    let r <- primitive_rule(name, found)
    return add_primitive(c, name, r)
}

def private join_name(name, tail : string) : string => empty(name) ? tail : "{name}-{tail}"

def private generate_union_rule(var c : Conv; name : string; alts : array<SNode?>) : string {
    var rules : array<string>
    rules |> reserve(length(alts))
    for (i, a in count(), alts) {
        rules |> push(visit(c, a, empty(name) ? "alternative-{i}" : "{name}-{i}"))
    }
    return join(rules, " | ")
}

// the same schema under each listed type: one alternative per type
def private generate_type_union(var c : Conv; name : string; schema : SRef; types : array<SNode?>) : string {
    var rules : array<string>
    rules |> reserve(length(types))
    for (i, t in count(), types) {
        rules |> push(visit(c, schema, empty(name) ? "alternative-{i}" : "{name}-{i}", t.kind == SKind.s_string ? t.str : "?"))
    }
    return join(rules, " | ")
}

// a JSON string that is none of `strings`: a trie of the forbidden spellings, each branch escaping
// through a differing character
struct private TrieNode {
    chars : array<int>
    children : array<TrieNode?>
    is_end : bool
}

def private trie_insert(var node : TrieNode?; s : string) {
    var cur = node
    peek_data(s) $(d : array<uint8> const#) {
        for (b in d) {
            let ch = int(b)
            var idx = -1
            for (i, k in count(), cur.chars) {
                if (k == ch) {
                    idx = i
                }
            }
            if (idx < 0) {
                // children stay sorted by character, as an ordered map iterates
                var at = length(cur.chars)
                for (i, k in count(), cur.chars) {
                    if (k > ch) {
                        at = i
                        break
                    }
                }
                cur.chars |> push(ch)
                cur.children |> push(new TrieNode)
                var j = length(cur.chars) - 1
                while (j > at) {
                    cur.chars[j] = cur.chars[j - 1]
                    swap(cur.children[j], cur.children[j - 1])
                    j--
                }
                cur.chars[at] = ch
                idx = at
            }
            cur = cur.children[idx]
        }
    }
    cur.is_end = true
}

def private trie_visit(node : TrieNode const?; char_rule : string; var w : StringBuilderWriter) {
    let rejects = bytes_to_string(node.chars)
    var first = true
    for (i, ch in count(), node.chars) {
        if (first) {
            first = false
        } else {
            w |> write(" | ")
        }
        w |> write("[")
        w |> write(to_char(ch))
        w |> write("]")
        let child = node.children[i]
        if (!empty(child.chars)) {
            w |> write(" (")
            trie_visit(child, char_rule, w)
            w |> write(")")
        } elif (child.is_end) {
            w |> write(" ")
            w |> write(char_rule)
            w |> write("+")
        }
    }
    if (!empty(node.chars)) {
        if (!first) {
            w |> write(" | ")
        }
        w |> write("[^\"")
        w |> write(rejects)
        w |> write("] ")
        w |> write(char_rule)
        w |> write("*")
    }
}

def private not_strings(var c : Conv; strings : array<string>) : string {
    var trie = new TrieNode
    for (s in strings) {
        trie_insert(trie, s)
    }
    let char_rule = add_primitive_named(c, "char")
    let out = build_string() $(var w) {
        w |> write("[\"] ( ")
        trie_visit(trie, char_rule, w)
        w |> write(" )")
        if (!trie.is_end) {
            w |> write("?")
        }
        w |> write(" [\"]")
    }
    unsafe {
        delete trie
    }
    return out
}

def private resolve_ref(var c : Conv; ref : string) : string {
    let hash = find(ref, "#")
    let fragment = hash >= 0 ? slice(ref, hash + 1) : ref
    var ref_name = "ref{sanitize_name(fragment)}"
    if (!(c.rules |> key_exists(ref_name)) && !(c.resolving |> key_exists(ref))) {
        c.resolving |> insert(ref)
        let target : SRef = c.refs?[ref] ?? null
        if (target == null) {
            conv_fail(c, "unresolved $ref {ref}")
        } else {
            ref_name = visit(c, target, ref_name)
        }
        c.resolving |> erase(ref)
    }
    return ref_name
}

// every `$ref` of the document resolved to its subtree: local `#/a/b` pointers only
def private collect_refs(var c : Conv; root : SRef; n : SRef) {
    return if (n == null)
    if (n.kind == SKind.s_array) {
        for (it in n.items) {
            collect_refs(c, root, it)
        }
        return
    }
    return if (n.kind != SKind.s_object)
    let ref = snode_get(n, "$ref")
    if (ref != null && ref.kind == SKind.s_string) {
        let r = ref.str
        if (!(c.refs |> key_exists(r))) {
            if (!(r |> starts_with("#/"))) {
                conv_fail(c, "unsupported $ref {r}")
                return
            }
            var target : SRef = root
            var parts <- split(slice(r, 2), "/")
            for (sel in parts) {
                if (target != null && target.kind == SKind.s_object && snode_has(target, sel)) {
                    target = snode_get(target, sel)
                } elif (target != null && target.kind == SKind.s_array) {
                    let idx = to_int(sel)
                    if (idx < 0 || idx >= length(target.items)) {
                        conv_fail(c, "error resolving $ref {r}: {sel} not in {dump(target)}")
                        return
                    }
                    target = target.items[idx]
                } else {
                    conv_fail(c, "error resolving $ref {r}: {sel} not in {dump(target)}")
                    return
                }
            }
            c.refs[r] = target
        }
        return
    }
    for (v in n.vals) {
        collect_refs(c, root, v)
    }
}

def private build_object_rule(var c : Conv; prop_keys : array<string>; prop_vals : array<SRef>; required : table<string>; name : string; additional : SRef; has_additional : bool) : string {
    var required_props : array<string>
    var optional_props : array<string>
    var prop_kv : table<string; string>
    var prop_names : array<string>
    for (i, pname in count(), prop_keys) {
        let prop_rule = visit(c, prop_vals[i], join_name(name, pname))
        prop_kv[pname] = add_rule(c, "{join_name(name, pname)}-kv", "{format_literal(dump_str(pname))} space \":\" space {prop_rule}")
        if (required |> key_exists(pname)) {
            required_props |> push(pname)
        } else {
            optional_props |> push(pname)
        }
        prop_names |> push(pname)
    }
    let add_is_true = has_additional && additional != null && additional.kind == SKind.s_bool && additional.flag
    let add_is_object = has_additional && additional != null && additional.kind == SKind.s_object
    if (add_is_true || add_is_object) {
        let sub_name = join_name(name, "additional")
        let value_rule = add_is_object ? visit(c, additional, "{sub_name}-value") : add_primitive_named(c, "value")
        let key_rule = empty(prop_names) ? add_primitive_named(c, "string") : add_rule(c, "{sub_name}-k", not_strings(c, prop_names))
        let kv_rule = add_rule(c, "{sub_name}-kv", "{key_rule} \":\" space {value_rule}")
        prop_kv["*"] = kv_rule
        optional_props |> push("*")
    }
    return build_string() $(var w) {
        w |> write("\"\{\" space ")
        for (i, rp in count(), required_props) {
            if (i > 0) {
                w |> write(" \",\" space ")
            }
            w |> write(prop_kv?[rp] ?? "")
        }
        if (!empty(optional_props)) {
            w |> write(" (")
            if (!empty(required_props)) {
                w |> write(" \",\" space ( ")
            }
            for (i in range(length(optional_props))) {
                if (i > 0) {
                    w |> write(" | ")
                }
                w |> write(recursive_refs(c, optional_props, i, false, prop_kv, name))
            }
            if (!empty(required_props)) {
                w |> write(" )")
            }
            w |> write(" )?")
        }
        w |> write(" space \"\}\"")
    }
}

// the optional properties from index `from` on: each one, then the rest as an optional tail rule
def private recursive_refs(var c : Conv; ks : array<string>; from : int; first_is_optional : bool; prop_kv : table<string; string>; name : string) : string {
    if (from >= length(ks)) {
        return ""
    }
    let k = ks[from]
    let kv_rule_name : string = prop_kv?[k] ?? ""
    let comma_ref = "( \",\" space {kv_rule_name} )"
    var res = ""
    if (first_is_optional) {
        res = comma_ref + (k == "*" ? "*" : "?")
    } else {
        res = kv_rule_name + (k == "*" ? " {comma_ref}*" : "")
    }
    if (from + 1 < length(ks)) {
        let rest = recursive_refs(c, ks, from + 1, true, prop_kv, name)
        res += " " + add_rule(c, "{join_name(name, k)}-rest", rest)
    }
    return res
}

def private dump_str(s : string) : string {
    return build_string() $(var w) {
        dump_string(s, w)
    }
}

// ----- the pattern (regular expression) visitor -----

def private is_non_literal(ch : int) : bool => ch == '|' || ch == '.' || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '*' || ch == '+' || ch == '?'
def private escaped_in_regex_only(ch : int) : bool => ch == '^' || ch == '$' || is_non_literal(ch)

struct private PatPiece {
    text : string
    is_literal : bool
}

def private piece_rule(p : PatPiece) : string => p.is_literal ? "\"{p.text}\"" : p.text

struct private PatState {
    pat : array<int>            //!< the pattern's bytes between the anchors
    i : int
    name : string
    sub_rule_ids : table<string; string>
}

def private pat_at(ps : PatState; i : int) : int => i < length(ps.pat) ? ps.pat[i] : -1

def private bytes_to_string(bs : array<int>) : string {
    return build_string() $(var w) {
        for (b in bs) {
            w |> write(to_char(b))
        }
    }
}

// `[...]` with the bracket at ps.i: the class text verbatim, escapes kept
def private pat_class(var c : Conv; var ps : PatState) : string {
    var sq : array<int>
    sq |> push('[')
    ps.i++
    while (ps.i < length(ps.pat) && pat_at(ps, ps.i) != ']') {
        if (pat_at(ps, ps.i) == '\\') {
            sq |> push(pat_at(ps, ps.i))
            sq |> push(pat_at(ps, ps.i + 1))
            ps.i += 2
        } else {
            sq |> push(pat_at(ps, ps.i))
            ps.i++
        }
    }
    if (ps.i >= length(ps.pat)) {
        conv_fail(c, "unbalanced square brackets in pattern")
    }
    sq |> push(']')
    ps.i++
    return bytes_to_string(sq)
}

// `{m,n}` with the brace at ps.i, applied to the sequence's last piece
def private pat_curly(var c : Conv; var ps : PatState; var seq : array<PatPiece>) {
    var curly : array<int>
    ps.i++
    while (ps.i < length(ps.pat) && pat_at(ps, ps.i) != '}') {
        curly |> push(pat_at(ps, ps.i))
        ps.i++
    }
    if (ps.i >= length(ps.pat)) {
        conv_fail(c, "unbalanced curly brackets in pattern")
    }
    ps.i++
    let nums <- split(bytes_to_string(curly), ",")
    var min_times = 0
    var max_times = INT_MAX
    if (length(nums) == 1) {
        min_times = to_int(nums[0])
        max_times = min_times
    } elif (length(nums) != 2) {
        conv_fail(c, "wrong number of values in curly brackets")
    } else {
        if (!empty(nums[0])) {
            min_times = to_int(nums[0])
        }
        if (!empty(nums[1])) {
            max_times = to_int(nums[1])
        }
    }
    if (empty(seq)) {
        conv_fail(c, "a repetition with nothing before it")
        return
    }
    let last = seq[length(seq) - 1]
    var sub = last.text
    if (!last.is_literal) {
        if (!(ps.sub_rule_ids |> key_exists(sub))) {
            let id = add_rule(c, "{ps.name}-{length(ps.sub_rule_ids) + 1}", sub)
            ps.sub_rule_ids[sub] = id
        }
        sub = ps.sub_rule_ids[sub]
    }
    seq[length(seq) - 1] = PatPiece(text = build_repetition(last.is_literal ? "\"{sub}\"" : sub, min_times, max_times, ""), is_literal = false)
}

// a run of literal characters from ps.i: escapes the grammar does not need are dropped, a quote is escaped
def private pat_literal(var ps : PatState) : string {
    var lit : array<int>
    let n = length(ps.pat)
    while (ps.i < n) {
        let cur = pat_at(ps, ps.i)
        if (cur == '\\' && ps.i < n - 1) {
            let next = pat_at(ps, ps.i + 1)
            if (escaped_in_regex_only(next)) {
                lit |> push(next)
            } else {
                lit |> push(cur)
                lit |> push(next)
            }
            ps.i += 2
        } elif (cur == '"') {
            lit |> push('\\')
            lit |> push('"')
            ps.i++
        } elif (!is_non_literal(cur) && (ps.i == n - 1 || empty(lit) || pat_at(ps, ps.i + 1) == '.' || !is_non_literal(pat_at(ps, ps.i + 1)))) {
            lit |> push(cur)
            ps.i++
        } else {
            break
        }
    }
    return bytes_to_string(lit)
}

// a sequence of the pattern up to `)` or its end, consecutive literals merged
def private pat_transform(var c : Conv; var ps : PatState) : PatPiece {
    let start = ps.i
    var seq : array<PatPiece>
    let n = length(ps.pat)
    while (ps.i < n && empty(c.err)) {
        let ch = pat_at(ps, ps.i)
        if (ch == '.') {
            seq |> push(PatPiece(text = add_rule(c, "dot", "[^\\x0A\\x0D]"), is_literal = false))
            ps.i++
        } elif (ch == '(') {
            ps.i++
            if (pat_at(ps, ps.i) == '?') {
                if (pat_at(ps, ps.i + 1) == ':') {
                    ps.i += 2
                } else {
                    conv_fail(c, "unsupported pattern syntax (lookaround)")
                    return PatPiece()
                }
            }
            let inner = pat_transform(c, ps)
            seq |> push(PatPiece(text = "({piece_rule(inner)})", is_literal = false))
        } elif (ch == ')') {
            ps.i++
            if (start > 0 && pat_at(ps, start - 1) != '(' && (start < 2 || pat_at(ps, start - 2) != '?' || pat_at(ps, start - 1) != ':')) {
                conv_fail(c, "unbalanced parentheses in pattern")
            }
            return join_seq(seq)
        } elif (ch == '[') {
            let cls = pat_class(c, ps)
            seq |> push(PatPiece(text = cls, is_literal = false))
        } elif (ch == '|') {
            seq |> push(PatPiece(text = "|", is_literal = false))
            ps.i++
        } elif (ch == '*' || ch == '+' || ch == '?') {
            if (empty(seq)) {
                conv_fail(c, "a quantifier with nothing before it")
                return PatPiece()
            }
            let last = seq[length(seq) - 1]
            seq[length(seq) - 1] = PatPiece(text = "{piece_rule(last)}{to_char(ch)}", is_literal = false)
            ps.i++
        } elif (ch == '{') {
            pat_curly(c, ps, seq)
        } else {
            let literal = pat_literal(ps)
            if (!empty(literal)) {
                seq |> push(PatPiece(text = literal, is_literal = true))
            }
        }
    }
    return join_seq(seq)
}

def private join_seq(seq : array<PatPiece>) : PatPiece {
    var ret : array<PatPiece>
    var literal : array<string>
    for (item in seq) {
        if (item.is_literal) {
            literal |> push(item.text)
        } else {
            if (!empty(literal)) {
                ret |> push(PatPiece(text = join(literal, ""), is_literal = true))
                literal |> clear()
            }
            ret |> push(item)
        }
    }
    if (!empty(literal)) {
        ret |> push(PatPiece(text = join(literal, ""), is_literal = true))
    }
    let results <- [for (item in ret); piece_rule(item)]
    return PatPiece(text = join(results, " "), is_literal = false)
}

def private visit_pattern(var c : Conv; pattern : string; name : string) : string {
    var bytes : array<int>
    peek_data(pattern) $(d : array<uint8> const#) {
        bytes |> reserve(length(d))
        for (b in d) {
            bytes |> push(int(b))
        }
    }
    let n = length(bytes)
    if (!(n >= 2 && bytes[0] == '^' && bytes[n - 1] == '$')) {
        conv_fail(c, "pattern must start with '^' and end with '$'")
        return ""
    }
    var ps = PatState(name = name)
    ps.pat <- [for (i in range(1, n - 1)); bytes[i]]
    let body = pat_transform(c, ps)
    return add_rule(c, name, "\"\\\"\" ({piece_rule(body)}) \"\\\"\"")
}

// ----- the schema visitor -----

def private type_is(t : SRef; what : string) : bool => t != null && t.kind == SKind.s_string && t.str == what

// the schema's `type` as a string ("" = none), `type_override` standing in for a listed type
def private type_name(schema_type : SRef; type_override : string) : string {
    if (!empty(type_override)) {
        return type_override
    }
    return schema_type != null && schema_type.kind == SKind.s_string ? schema_type.str : ""
}

def private visit(var c : Conv; schema : SRef; name : string; type_override : string = "") : string {
    return "" if (!empty(c.err))
    if (schema == null) {
        conv_fail(c, "a schema is missing")
        return ""
    }
    let schema_type_node = snode_get(schema, "type")
    let tname = type_name(schema_type_node, type_override)
    let has_type = !empty(tname)
    let schema_format = snode_str(schema, "format")
    let rule_name = is_reserved_name(name) ? "{name}-" : (empty(name) ? "root" : name)
    if (schema.kind != SKind.s_object) {
        conv_fail(c, "unrecognized schema: {dump(schema)}")
        return ""
    }
    if (snode_has(schema, "$ref")) {
        return add_rule(c, rule_name, resolve_ref(c, snode_str(schema, "$ref")))
    }
    let one_of = snode_get(schema, "oneOf")
    let any_of = snode_get(schema, "anyOf")
    if (one_of != null || any_of != null) {
        let alts = one_of != null ? one_of : any_of
        return add_rule(c, rule_name, generate_union_rule(c, name, alts.items))
    }
    if (empty(type_override) && schema_type_node != null && schema_type_node.kind == SKind.s_array) {
        // one alternative per listed type: the same schema read under each
        return add_rule(c, rule_name, generate_type_union(c, name, schema, schema_type_node.items))
    }
    if (snode_has(schema, "const")) {
        return add_rule(c, rule_name, format_literal(dump(snode_get(schema, "const"))))
    }
    let enum_v = snode_get(schema, "enum")
    if (enum_v != null) {
        let vals <- [for (v in enum_v.items); format_literal(dump(v))]
        return add_rule(c, rule_name, "({join(vals, " | ")})")
    }
    let additional = snode_get(schema, "additionalProperties")
    let additional_restricts = additional != null && !(additional.kind == SKind.s_bool && additional.flag)
    if ((!has_type || tname == "object") && (snode_has(schema, "properties") || additional_restricts)) {
        var required : table<string>
        let req = snode_get(schema, "required")
        if (req != null && req.kind == SKind.s_array) {
            for (it in req.items) {
                if (it.kind == SKind.s_string) {
                    required |> insert(it.str)
                }
            }
        }
        var prop_keys : array<string>
        var prop_vals : array<SRef>
        let props = snode_get(schema, "properties")
        if (props != null) {
            prop_keys := props.keys
            prop_vals |> reserve(length(props.vals))
            for (v in props.vals) {   // nolint:PERF022 - the element type widens to the read-only view
                prop_vals |> push(v)
            }
        }
        return add_rule(c, rule_name, build_object_rule(c, prop_keys, prop_vals, required, name, additional, additional != null))
    }
    let all_of = snode_get(schema, "allOf")
    if ((!has_type || tname == "object" || tname == "string") && all_of != null) {
        var required : table<string>
        var prop_keys : array<string>
        var prop_vals : array<SRef>
        var enum_counts : table<string; int>
        var enum_order : array<string>
        for (comp in all_of.items) {
            let comps_any = snode_get(comp, "anyOf")
            if (comps_any != null) {
                for (cc in comps_any.items) {
                    all_of_component(c, cc, false, prop_keys, prop_vals, required, enum_counts, enum_order)
                }
            } else {
                all_of_component(c, comp, true, prop_keys, prop_vals, required, enum_counts, enum_order)
            }
        }
        var result = ""
        if (!empty(enum_order)) {
            let inter <- [for (k in enum_order); k; where enum_counts[k] == length(all_of.items)]
            if (!empty(inter)) {
                result = add_rule(c, rule_name, "({join(inter, " | ")})")
            }
        }
        if (empty(result)) {
            result = add_rule(c, rule_name, build_object_rule(c, prop_keys, prop_vals, required, name, null, false))
        }
        return result
    }
    let items = snode_has(schema, "items") ? snode_get(schema, "items") : snode_get(schema, "prefixItems")
    if ((!has_type || tname == "array") && items != null) {
        if (items.kind == SKind.s_array) {
            let tuple_rules <- [for (i, it in count(), items.items); visit(c, it, join_name(name, "tuple-{i}"))]
            return add_rule(c, rule_name, "\"[\" space {join(tuple_rules, " \",\" space ")} space \"]\"")
        }
        let item_rule = visit(c, items, join_name(name, "item"))
        let min_items = int(snode_int(schema, "minItems", 0l))
        let mx = snode_get(schema, "maxItems")
        let max_items = mx != null && mx.kind == SKind.s_number ? int(mx.num) : INT_MAX
        return add_rule(c, rule_name, "\"[\" space {build_repetition(item_rule, min_items, max_items, "\",\" space")} space \"]\"")
    }
    if ((!has_type || tname == "string") && snode_has(schema, "pattern")) {
        return visit_pattern(c, snode_str(schema, "pattern"), rule_name)
    }
    if ((!has_type || tname == "string") && is_uuid_format(schema_format)) {
        var found = false
        let ur <- primitive_rule("uuid", found)
        return add_primitive(c, rule_name == "root" ? "root" : schema_format, ur)
    }
    var is_fmt = false
    let fr <- format_rule("{schema_format}-string", is_fmt)
    if ((!has_type || tname == "string") && is_fmt) {
        return add_rule(c, rule_name, add_primitive(c, "{schema_format}-string", fr))
    }
    if (tname == "string" && (snode_has(schema, "minLength") || snode_has(schema, "maxLength"))) {
        let char_rule = add_primitive_named(c, "char")
        let min_len = int(snode_int(schema, "minLength", 0l))
        let mx = snode_get(schema, "maxLength")
        let max_len = mx != null && mx.kind == SKind.s_number ? int(mx.num) : INT_MAX
        return add_rule(c, rule_name, "\"\\\"\" {build_repetition(char_rule, min_len, max_len)} \"\\\"\"")
    }
    if (tname == "integer" && (snode_has(schema, "minimum") || snode_has(schema, "exclusiveMinimum") || snode_has(schema, "maximum") || snode_has(schema, "exclusiveMaximum"))) {
        var min_value = INT64_MIN
        var max_value = INT64_MAX
        if (snode_has(schema, "minimum")) {
            min_value = snode_int(schema, "minimum", 0l)
        } elif (snode_has(schema, "exclusiveMinimum")) {
            min_value = snode_int(schema, "exclusiveMinimum", 0l) + 1l
        }
        if (snode_has(schema, "maximum")) {
            max_value = snode_int(schema, "maximum", 0l)
        } elif (snode_has(schema, "exclusiveMaximum")) {
            max_value = snode_int(schema, "exclusiveMaximum", 0l) - 1l
        }
        let body = build_string() $(var w) {
            w |> write("(")
            build_min_max_int(min_value, max_value, w)
            w |> write(")")
        }
        return add_rule(c, rule_name, body)
    }
    if (empty(schema.keys) || tname == "object") {
        return add_rule(c, rule_name, add_primitive_named(c, "object"))
    }
    if (!has_type) {
        // no type and no structural keyword (a bare description): any value
        return add_rule(c, rule_name, add_primitive_named(c, "value"))
    }
    var is_prim = false
    let pr <- primitive_rule(tname, is_prim)
    if (!is_prim) {
        conv_fail(c, "unrecognized schema: {dump(schema)}")
        return ""
    }
    return add_primitive(c, rule_name == "root" ? "root" : tname, pr)
}

def private is_uuid_format(f : string) : bool => f == "uuid" || f == "uuid1" || f == "uuid2" || f == "uuid3" || f == "uuid4" || f == "uuid5"

def private all_of_component(var c : Conv; comp : SRef; is_required : bool; var prop_keys : array<string>; var prop_vals : array<SRef>; var required : table<string>; var enum_counts : table<string; int>; var enum_order : array<string>) {
    if (snode_has(comp, "$ref")) {
        let target : SRef = c.refs?[snode_str(comp, "$ref")] ?? null
        if (target != null) {
            all_of_component(c, target, is_required, prop_keys, prop_vals, required, enum_counts, enum_order)
        }
    } elif (snode_has(comp, "properties")) {
        let ps = snode_get(comp, "properties")
        for (i, k in count(), ps.keys) {
            prop_keys |> push(k)
            prop_vals |> push(ps.vals[i])
            if (is_required) {
                required |> insert(k)
            }
        }
    } elif (snode_has(comp, "enum")) {
        for (v in snode_get(comp, "enum").items) {
            let rule = format_literal(dump(v))
            if (!(enum_counts |> key_exists(rule))) {
                enum_counts[rule] = 0
                enum_order |> push(rule)
            }
            enum_counts[rule]++
        }
    }
}

// the rules in name order, one per line - the converter's own text shape
def private format_grammar(c : Conv) : string {
    var names <- [for (k in keys(c.rules)); k]
    sort(names)
    return build_string() $(var w) {
        for (n in names) {
            w |> write(n)
            w |> write(" ::= ")
            w |> write(c.rules?[n] ?? "")
            w |> write("\n")
        }
    }
}

//! The GBNF grammar text a schema compiles to, "" with `err` set when the schema cannot be compiled.
def json_schema_to_gbnf(schema_text : string; var err : string&) : string {
    var doc = parse_schema(schema_text, err)
    if (doc == null) {
        return ""
    }
    var c : Conv
    c.rules["space"] = SPACE_RULE
    collect_refs(c, doc, doc)
    if (empty(c.err)) {
        visit(c, doc, "")
    }
    var out = ""
    if (empty(c.err)) {
        out = format_grammar(c)
    }
    err = c.err
    unsafe {
        delete doc
    }
    return out
}

//! The schema compiled all the way to the engine's tables; `err` names the first problem of either stage.
def json_schema_grammar(schema_text : string; var err : string&) : Grammar {
    let text = json_schema_to_gbnf(schema_text, err)
    if (!empty(err)) {
        var g : Grammar
        g.err = err
        return <- g
    }
    return <- gbnf_grammar(text, err)
}
