options gen2
options indenting = 4
options strict_smart_pointers = true

module gbnf shared public

require lr1/lr1
require daslib/strings_boost
require math

// A reader for grammars in GBNF, the BNF dialect llama.cpp's grammar files use: `name ::= alternatives`,
// sequences of `"literals"`, `[character classes]`, rule names, `( groups )` and `.`, with the repetition
// suffixes `* + ? {m} {m,} {m,n}`, `#` comments, and `|` continuing a rule across a newline. Terminals are
// code points; a class that reaches past ASCII compiles into the UTF-8 byte sequences of its ranges, so the
// grammar it builds stays a byte grammar. Token references (`<[id]>`, `<name>`, `!<...>`) are refused: the
// acceptor sees bytes, not the tokenizer's ids.

struct private Reader {
    text : array<uint8>
    pos : int
    gb : GrammarBuilder
    err : string
    fresh : int
    defined : table<string>
    referenced : table<string>
}

def private peek(r : Reader) : int => r.pos < length(r.text) ? int(r.text[r.pos]) : -1
def private peek_at(r : Reader; off : int) : int => r.pos + off < length(r.text) ? int(r.text[r.pos + off]) : -1

def private fail(var r : Reader; what : string) {
    if (empty(r.err)) {
        r.err = "gbnf: {what} at byte {r.pos}"
    }
}

def private is_name_byte(c : int) : bool => is_alpha(c) || is_number(c) || c == '-' || c == '_'

def private is_space(c : int) : bool => c == ' ' || c == '\t' || c == '\r'

// spaces, tabs and comments; a newline is skipped only when `newlines` (inside parentheses, or when the rule
// continues with `|`)
def private skip_ws(var r : Reader; newlines : bool) {
    while (r.pos < length(r.text)) {
        let c = peek(r)
        if (is_space(c)) {
            r.pos++
        } elif (c == '#') {
            while (r.pos < length(r.text) && peek(r) != '\n') {
                r.pos++
            }
        } elif (c == '\n' && (newlines || next_line_continues(r))) {
            r.pos++
        } else {
            break
        }
    }
}

// after a newline: does the next non-blank line start with `|`? (the rule continues)
def private next_line_continues(r : Reader) : bool {
    var p = r.pos + 1
    while (p < length(r.text)) {
        let c = int(r.text[p])
        if (is_space(c) || c == '\n') {
            p++
        } elif (c == '#') {
            while (p < length(r.text) && int(r.text[p]) != '\n') {
                p++
            }
        } else {
            return c == '|'
        }
    }
    return false
}

def private read_name(var r : Reader) : string {
    let start = r.pos
    while (r.pos < length(r.text) && is_name_byte(peek(r))) {
        r.pos++
    }
    return build_string() $(var w) {
        for (i in range(start, r.pos)) {
            w |> write(to_char(int(r.text[i])))
        }
    }
}

def private hex_val(c : int) : int {
    if (is_number(c)) {
        return c - '0'
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10
    }
    return -1
}

def private read_hex(var r : Reader; digits : int) : int {
    var v = 0
    for (_ in range(digits)) {
        let h = hex_val(peek(r))
        if (h < 0) {
            fail(r, "bad hex escape")
            return 0
        }
        v = v * 16 + h
        r.pos++
    }
    return v
}

// one code point of the grammar text, escapes decoded (`\n \t \r \" \\ \[ \] \xXX \uXXXX \UXXXXXXXX`)
def private read_codepoint(var r : Reader) : int {
    let c = peek(r)
    if (c < 0) {
        fail(r, "unexpected end of grammar")
        return 0
    }
    if (c == '\\') {
        r.pos++
        let e = peek(r)
        r.pos++
        if (e == 'n') {
            return '\n'
        } elif (e == 't') {
            return '\t'
        } elif (e == 'r') {
            return '\r'
        } elif (e == 'x') {
            return read_hex(r, 2)
        } elif (e == 'u') {
            return read_hex(r, 4)
        } elif (e == 'U') {
            return read_hex(r, 8)
        } elif (e == '"' || e == '[' || e == ']' || e == '\\' || e == '-' || e == '^') {
            return e
        }
        fail(r, "unknown escape")
        return 0
    }
    // UTF-8 decode of a literal character
    if (c < 128) {
        r.pos++
        return c
    }
    let len = c >= 240 ? 4 : (c >= 224 ? 3 : 2)
    var v = c & (len == 2 ? 31 : (len == 3 ? 15 : 7))
    r.pos++
    for (_ in range(len - 1)) {
        let cc = peek(r)
        if (cc < 0 || (cc & 192) != 128) {
            fail(r, "malformed UTF-8 in the grammar text")
            return 0
        }
        v = (v << 6) | (cc & 63)
        r.pos++
    }
    return v
}

// ----- code point ranges -> byte grammar -----

struct private CpRange {
    lo : int
    hi : int
}

def private utf8_len(cp : int) : int => cp < 128 ? 1 : (cp < 2048 ? 2 : (cp < 65536 ? 3 : 4))

def private utf8_bytes(cp : int) : array<int> {
    var out : array<int>
    if (cp < 128) {
        out |> push(cp)
    } elif (cp < 2048) {
        out |> push(192 | (cp >> 6))
        out |> push(128 | (cp & 63))
    } elif (cp < 65536) {
        out |> push(224 | (cp >> 12))
        out |> push(128 | ((cp >> 6) & 63))
        out |> push(128 | (cp & 63))
    } else {
        out |> push(240 | (cp >> 18))
        out |> push(128 | ((cp >> 12) & 63))
        out |> push(128 | ((cp >> 6) & 63))
        out |> push(128 | (cp & 63))
    }
    return <- out
}

// the byte-sequence alternatives of one code point range as productions of rule `rule`: split at the
// UTF-8 length boundaries, then recursively so every alternative is a sequence of independent byte ranges
def private add_cp_range(var r : Reader; rule : int; lo, hi : int) {
    if (lo > hi) {
        return
    }
    // one encoding length per alternative
    for (bound in [127, 2047, 65535]) {
        if (lo <= bound && hi > bound) {
            add_cp_range(r, rule, lo, bound)
            add_cp_range(r, rule, bound + 1, hi)
            return
        }
    }
    let n = utf8_len(lo)
    if (n == 1) {
        var bs : ByteSet
        for (c in range(lo, hi + 1)) {
            bs[c] = true
        }
        var rhs : array<GrammarSym>
        rhs |> push(sym_bytes(bs))
        add_production(r.gb, rule, rhs)
        return
    }
    // split so that every byte but the last is fixed across the range, or the whole range is a full
    // continuation span at this position: the classic per-position subdivision
    for (i in range(1, n)) {
        let shift = 6 * (n - i)
        let mask = (1 << shift) - 1
        if ((lo & mask) != 0 && (lo | mask) < hi) {
            add_cp_range(r, rule, lo, lo | mask)
            add_cp_range(r, rule, (lo | mask) + 1, hi)
            return
        }
        if ((hi & mask) != mask && lo < (hi & ~mask)) {
            add_cp_range(r, rule, lo, (hi & ~mask) - 1)
            add_cp_range(r, rule, hi & ~mask, hi)
            return
        }
    }
    // now lo and hi share the same leading bytes until a position where lo runs 128.. and hi ..191
    let lb <- utf8_bytes(lo)
    let hb <- utf8_bytes(hi)
    var rhs : array<GrammarSym>
    rhs |> reserve(n)
    for (i in range(n)) {
        var bs : ByteSet
        for (c in range(lb[i], hb[i] + 1)) {
            bs[c] = true
        }
        rhs |> push(sym_bytes(bs))
    }
    add_production(r.gb, rule, rhs)
}

def private fresh_rule(var r : Reader; what : string) : int {
    r.fresh++
    return rule_id(r.gb, "<{what}{r.fresh}>")
}

// a class as a symbol: ASCII-only and plain -> one byte set; otherwise a rule over the UTF-8 sequences
def private class_symbol(var r : Reader; var ranges : array<CpRange>; negated : bool) : GrammarSym {
    var ascii_only = !negated
    if (ascii_only) {
        for (rg in ranges) {
            if (rg.hi >= 128) {
                ascii_only = false
            }
        }
    }
    if (ascii_only) {
        var bs : ByteSet
        for (rg in ranges) {
            for (c in range(rg.lo, rg.hi + 1)) {
                bs[c] = true
            }
        }
        return sym_bytes(bs)
    }
    sort(ranges) $(a, b : CpRange) : bool { return a.lo < b.lo }
    let rule = fresh_rule(r, "class")
    if (!negated) {
        for (rg in ranges) {
            add_cp_range(r, rule, rg.lo, rg.hi)
        }
    } else {
        var next = 0
        for (rg in ranges) {
            if (rg.lo > next) {
                add_cp_range(r, rule, next, rg.lo - 1)
            }
            next = max(next, rg.hi + 1)
        }
        if (next <= 1114111) {
            add_cp_range(r, rule, next, 1114111)
        }
    }
    return sym_rule(rule)
}

// `[...]` with the opening bracket consumed
def private read_class(var r : Reader) : GrammarSym {
    var negated = false
    if (peek(r) == '^') {
        negated = true
        r.pos++
    }
    var ranges : array<CpRange>
    while (peek(r) != ']') {
        if (peek(r) < 0) {
            fail(r, "unterminated character class")
            break
        }
        let lo = read_codepoint(r)
        var hi = lo
        if (peek(r) == '-' && peek_at(r, 1) != ']' && peek_at(r, 1) >= 0) {
            r.pos++
            hi = read_codepoint(r)
        }
        ranges |> push(CpRange(lo = lo, hi = hi))
        break if (!empty(r.err))
    }
    r.pos++   // ']'
    return class_symbol(r, ranges, negated)
}

// `"..."` with the opening quote consumed: a sequence of single-byte terminals
def private read_literal(var r : Reader; var seq : array<GrammarSym>) {
    while (peek(r) != '"') {
        if (peek(r) < 0) {
            fail(r, "unterminated literal")
            return
        }
        let cp = read_codepoint(r)
        return if (!empty(r.err))
        let bytes <- utf8_bytes(cp)
        seq |> reserve(length(seq) + length(bytes))
        for (b in bytes) {
            seq |> push(sym_byte(b))
        }
    }
    r.pos++   // closing quote
}

// the rule for `x` repeated: x* / x+ / x? / x{m,n}; `x` is a one-symbol sequence or a group rule
def private repeat_symbol(var r : Reader; x : GrammarSym; lo, hi : int) : GrammarSym {
    // x{lo,hi}: lo copies, then a chain of up to (hi - lo) optional copies; hi < 0 = unbounded
    let rule = fresh_rule(r, "rep")
    var head <- [for (_ in range(lo)); x]
    if (hi < 0) {
        // tail = x*  as a left-recursive star rule
        let star = fresh_rule(r, "star")
        var e : array<GrammarSym>
        add_production(r.gb, star, e)
        var more <- [sym_rule(star), x]
        add_production(r.gb, star, more)
        head |> push(sym_rule(star))
        add_production(r.gb, rule, head)
        return sym_rule(rule)
    }
    if (hi > lo) {
        // opt_k ::= eps | x opt_{k-1}
        var inner = -1
        for (_ in range(hi - lo)) {
            let opt = fresh_rule(r, "opt")
            var e : array<GrammarSym>
            add_production(r.gb, opt, e)
            var some : array<GrammarSym>
            some |> push(x)
            if (inner >= 0) {
                some |> push(sym_rule(inner))
            }
            add_production(r.gb, opt, some)
            inner = opt
        }
        head |> push(sym_rule(inner))
    }
    add_production(r.gb, rule, head)
    return sym_rule(rule)
}

def private read_int(var r : Reader) : int {
    var v = 0
    var any = false
    while (is_number(peek(r))) {
        v = v * 10 + (peek(r) - '0')
        r.pos++
        any = true
    }
    if (!any) {
        fail(r, "expected a number in a repetition")
    }
    return v
}

// a repetition suffix after the symbol `x`, if any
def private read_repetition(var r : Reader; x : GrammarSym) : GrammarSym {
    let c = peek(r)
    if (c == '*') {
        r.pos++
        return repeat_symbol(r, x, 0, -1)
    } elif (c == '+') {
        r.pos++
        return repeat_symbol(r, x, 1, -1)
    } elif (c == '?') {
        r.pos++
        return repeat_symbol(r, x, 0, 1)
    } elif (c == '{') {
        r.pos++
        skip_ws(r, true)
        let lo = read_int(r)
        var hi = lo
        skip_ws(r, true)
        if (peek(r) == ',') {
            r.pos++
            skip_ws(r, true)
            hi = peek(r) == '}' ? -1 : read_int(r)
            skip_ws(r, true)
        }
        if (peek(r) != '}') {
            fail(r, "expected } after a repetition")
            return x
        }
        r.pos++
        return repeat_symbol(r, x, lo, hi)
    }
    return x
}

// a sequence of alternatives into `rule`
def private read_alternatives(var r : Reader; rule : int; nested : bool) {
    while (true) {
        var seq : array<GrammarSym>
        read_sequence(r, seq, nested)
        return if (!empty(r.err))
        add_production(r.gb, rule, seq)
        skip_ws(r, nested)
        if (peek(r) == '|') {
            r.pos++
            skip_ws(r, true)
            continue
        }
        break
    }
}

// a literal of several bytes carries its own repetition as a group; a one-symbol element repeats in place
def private read_sequence(var r : Reader; var seq : array<GrammarSym>; nested : bool) {
    while (true) {
        skip_ws(r, nested)
        let c = peek(r)
        if (c < 0 || c == '|' || c == ')' || c == '\n') {
            return
        }
        if (c == '"') {
            r.pos++
            var lit : array<GrammarSym>
            read_literal(r, lit)
            return if (!empty(r.err))
            if (length(lit) == 1) {
                seq |> push(read_repetition(r, lit[0]))
            } else {
                let rep = peek(r)
                if (rep == '*' || rep == '+' || rep == '?' || rep == '{') {
                    let grp = fresh_rule(r, "lit")
                    add_production(r.gb, grp, lit)
                    seq |> push(read_repetition(r, sym_rule(grp)))
                } else {
                    seq |> push_from(lit)
                }
            }
        } elif (c == '[') {
            r.pos++
            let cls = read_class(r)
            return if (!empty(r.err))
            seq |> push(read_repetition(r, cls))
        } elif (c == '(') {
            r.pos++
            let grp = fresh_rule(r, "group")
            skip_ws(r, true)
            read_alternatives(r, grp, true)
            return if (!empty(r.err))
            if (peek(r) != ')') {
                fail(r, "expected )")
                return
            }
            r.pos++
            seq |> push(read_repetition(r, sym_rule(grp)))
        } elif (c == '.') {
            r.pos++
            var any : array<CpRange>
            any |> push(CpRange(lo = 0, hi = 1114111))
            seq |> push(read_repetition(r, class_symbol(r, any, false)))
        } elif (c == '<' || c == '!') {
            fail(r, "token references are not supported (the acceptor reads bytes)")
            return
        } elif (is_name_byte(c)) {
            let name = read_name(r)
            if (!empty(r.err)) {
                return
            }
            // `name ::=` here means the previous rule ended without a newline
            let save = r.pos
            skip_ws(r, false)
            if (peek(r) == ':' && peek_at(r, 1) == ':' && peek_at(r, 2) == '=') {
                r.pos = save - length(name)
                return
            }
            r.pos = save
            r.referenced |> insert(name)
            seq |> push(read_repetition(r, sym_rule(rule_id(r.gb, name))))
        } else {
            fail(r, "unexpected character '{to_char(c)}'")
            return
        }
        return if (!empty(r.err))
    }
}

//! Build the grammar `text` declares, rooted at its `root` rule. `err` names the first problem - a syntax
//! error in the text, a rule referenced but never defined, no `root`, or the table builder's verdict.
def gbnf_grammar(text : string; var err : string&) : Grammar {
    var r : Reader
    peek_data(text) $(d : array<uint8> const#) {
        r.text |> resize(length(d))
        for (i, b in count(), d) {
            r.text[i] = b
        }
    }
    while (empty(r.err)) {
        skip_ws(r, true)
        break if (peek(r) < 0)
        if (!is_name_byte(peek(r))) {
            fail(r, "expected a rule name")
            break
        }
        let name = read_name(r)
        skip_ws(r, false)
        if (!(peek(r) == ':' && peek_at(r, 1) == ':' && peek_at(r, 2) == '=')) {
            fail(r, "expected ::= after '{name}'")
            break
        }
        r.pos += 3
        skip_ws(r, true)
        if (r.defined |> key_exists(name)) {
            fail(r, "rule '{name}' is defined twice")
            break
        }
        r.defined |> insert(name)
        read_alternatives(r, rule_id(r.gb, name), false)
    }
    var g : Grammar
    if (!empty(r.err)) {
        err = r.err
        g.err = r.err
        return <- g
    }
    for (name in keys(r.referenced)) {
        if (!(r.defined |> key_exists(name))) {
            err = "gbnf: rule '{name}' is referenced but never defined"
            g.err = err
            return <- g
        }
    }
    if (!(r.defined |> key_exists("root"))) {
        err = "gbnf: no root rule"
        g.err = err
        return <- g
    }
    g <- build_grammar(r.gb, r.gb.rule_index["root"])
    err = g.err
    return <- g
}
