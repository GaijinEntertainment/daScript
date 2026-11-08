options gen2

require dastest/testing_boost
require strings

// Byte views are binary-safe by design: every view extern is entry
// glue over a length-bounded core, so an interior NUL is data and the
// byte after the view is never read. This file covers the two things
// the parity harness further down cannot: the interior-NUL behaviour
// where view and string MUST diverge, and the edges of the three
// NUL-dependence rewrites (bounded find/rfind, len-taking strip/trim
// walkers, bounded integer parse).
//
// The remaining bound - view_size's INT_MAX guard in
// module_builtin_string.cpp - has no test here: tripping it takes a
// >2GB array. It holds by inspection instead: every view entry sizes
// itself through view_size, which throws before the uint32 truncation
// can happen.

// a NUL cannot arrive through to_bytes - the source string ends there
// - so the hole is punched between two conversions
def private with_nul(head, tail : string) : array<uint8> {
    var b <- to_bytes(head)
    b |> push(uint8(0))
    b |> push_from(to_bytes(tail))
    return <- b
}

// bytes of `text` with the view shortened to `n` - the dropped bytes
// stay in the buffer, so a core that scans for a terminator instead
// of honouring `size` would walk straight into them
def private truncated(text : string; n : int) : array<uint8> {
    var b <- to_bytes(text)
    b |> resize(n)
    return <- b
}

def private str_byte(s : string; idx : int) : int {
    let p = unsafe(reinterpret<uint8 const?>(s))
    return int(unsafe(p[idx]))
}

def private is_null_str(s : string) : bool => empty(s)

// ===== interior NUL =====

[test(description = "find, rfind and find-char on a view see past an interior NUL, including the NUL itself, where the string twin stops at it")]
def test_nul_find(t : T?) {
    var inscope d <- with_nul("a", "cd 42")
    let s = string(d)
    t |> equal(length(d), 7, "the view carries all seven bytes")
    t |> equal(length(s), 1, "the materialized string stops at the interior NUL")
    t |> equal(find(d, "cd"), 2, "find locates a substring past the interior NUL")
    t |> equal(find(s, "cd"), -1, "the string counterpart cannot see past the NUL")
    t |> equal(find(d, "42"), 5, "find reaches the far end of the view")
    t |> equal(find(d, "cd", 3), -1, "find-from past the hit still misses")
    t |> equal(rfind(d, "d"), 3, "rfind locates a substring past the interior NUL")
    t |> equal(rfind(s, "d"), -1, "the string counterpart cannot rfind past the NUL")
    t |> equal(rfind(d, "a"), 0, "rfind still reaches offset 0")
    t |> equal(find(d, 'c'), 2, "find char locates a byte past the interior NUL")
    t |> equal(find(d, 'c', 3), -1, "find char from past the hit misses")
    t |> equal(find(d, 0), 1, "find char locates the NUL itself - it is data to a view")
    t |> equal(find(s, 0), -1, "the string counterpart never finds a NUL")
}

[test(description = "slice and chop on a view across an interior NUL read as truncated like the string twin")]
def test_nul_slice_and_chop(t : T?) {
    var inscope d <- with_nul("a", "cd")
    let win = slice(d, 0, 3)
    // the window materializes all three bytes, but every downstream op is
    // strlen-based, so it reads as "a" - exactly what string(d) already
    // does today
    t |> equal(length(win), 1, "a window spanning the NUL reads as truncated")
    t |> equal(win, "a", "a window spanning the NUL compares as its leading run")
    t |> equal(str_byte(win, 1), 0, "the NUL is still in the materialized buffer")
    t |> equal(str_byte(win, 2), 'c', "and so is the byte after it")
    let chopped = chop(d, 0, 3)
    t |> equal(length(chopped), 1, "chop spanning the NUL truncates the same way")
    t |> equal(str_byte(chopped, 2), 'c', "chop copies the whole window regardless")
    let tail = slice(d, 2)
    t |> equal(tail, "cd", "a window entirely past the NUL is an ordinary string")
    t |> equal(length(tail), 2, "and has its full length")
    t |> success(is_null_str(slice(d, 1, 1)), "an empty window is still null")
}

[test(description = "starts_with and ends_with on a view read the leading and trailing runs around an interior NUL")]
def test_nul_prefix_suffix(t : T?) {
    var inscope d <- with_nul("a", "cd 42")
    let s = string(d)
    t |> success(starts_with(d, "a"), "starts_with reads the leading run")
    t |> success(ends_with(d, "42"), "ends_with reads the tail past the NUL")
    t |> success(!ends_with(s, "42"), "the string counterpart ends at the NUL")
    t |> success(ends_with(d, "2"), "ends_with of a single trailing byte")
    t |> success(starts_with(d, 2, "cd"), "starts_with at an offset past the NUL")
    t |> success(!starts_with(d, 1, "c"), "offset 1 is the NUL byte itself, not 'c'")
    t |> success(starts_with(d, 1, "", 0u), "an in-range offset with cmpLen 0 is still true")
    t |> success(!starts_with(d, 7, ""), "offset == size stays false")
    t |> success(starts_with(d, 5, "42"), "starts_with at the last two bytes")
}

[test(description = "strip and trim on a view keep the run that contains an interior NUL")]
def test_nul_strip_and_trim(t : T?) {
    // a NUL is not whitespace, so the walkers step over it like any other
    // byte
    var inscope inner <- with_nul("a", "b")
    let kept = strip(inner)
    t |> equal(length(kept), 1, "strip keeps the NUL-containing run, which reads truncated")
    t |> equal(str_byte(kept, 1), 0, "the NUL survives the copy")
    t |> equal(str_byte(kept, 2), 'b', "and so does the byte after it")
    t |> equal(length(trim(inner)), 1, "trim behaves the same")
    var inscope padded <- with_nul(" ", " ")
    let stripped = strip(padded)
    t |> equal(length(stripped), 0, "a lone NUL between spaces reads as the empty string")
    t |> equal(length(trim(padded)), 0, "trim agrees")
    // the char-set form only trims the right, so the leading space
    // survives and the NUL stops it
    let rt = rtrim(padded, " ")
    t |> equal(length(rt), 1, "the rtrim char-set walker stops at the NUL")
    t |> equal(str_byte(rt, 1), 0, "with the NUL kept as content")
}

[test(description = "skip_white_space on a view stops at an interior NUL, as its string twin stops at the truncated end")]
def test_nul_skip_white_space(t : T?) {
    // a NUL is content, so the cursor stops on it from either surface
    var inscope lead <- with_nul(" ", " a")
    t |> equal(skip_white_space(lead, 0), 1, "the view cursor stops at the NUL")
    t |> equal(skip_white_space(string(lead), 0), 1,
               "and so does the string twin, at its truncated length")
    // past the NUL the surfaces MUST diverge: the view still carries the
    // tail, the string does not
    t |> equal(skip_white_space(lead, 2), 3, "the view crosses the whitespace after the NUL")
    t |> equal(skip_white_space(string(lead), 2), 1,
               "the string twin clamps to the length the NUL leaves it")
    t |> equal(length(lead), 4, "the view length counts every byte")
    t |> equal(length(string(lead)), 1, "while the materialized string reads truncated")
}

[test(description = "the view number parser stops its cursor at an interior NUL")]
def test_nul_parse_cursor(t : T?) {
    var inscope d <- with_nul("1", " 42")
    var cr = ConversionResult.ok
    var off = 0
    let first = int(d, cr, off)
    t |> equal(first, 1, "the cursor parses the leading number")
    t |> equal(cr, ConversionResult.ok, "leading parse succeeds")
    t |> equal(off, 1, "and stops before the NUL")
    off ++     // step over the NUL, which is a separator like any other byte here
    let second = int(d, cr, off)
    t |> equal(second, 42, "the cursor parses past an interior NUL")
    t |> equal(cr, ConversionResult.ok, "the parse after the NUL succeeds")
    t |> equal(off, 5, "and the cursor lands at the end of the view")
    // the string counterpart sees a one-character string and nothing else
    var scr = ConversionResult.ok
    var soff = 0
    t |> equal(int(string(d), scr, soff), 1, "the string counterpart parses only the leading run")
    t |> equal(soff, 1, "and its offset stops at the NUL")
}

[test(description = "write_string of a view writes all its bytes, interior NUL included")]
def test_nul_write_string(t : T?) {
    var inscope d <- with_nul("a", "c")
    let written = build_string() $(var w) {
        w |> write_string(d)
        w |> write("TAIL")
    }
    // the writer is binary-safe: all seven bytes land in the builder
    // verbatim, and the allocated string carries them - only the
    // strlen-based surface reads it as "a"
    t |> equal(length(written), 1, "the result reads as its leading run")
    t |> equal(written, "a", "and compares as it")
    t |> equal(str_byte(written, 1), 0, "the NUL is written through")
    t |> equal(str_byte(written, 2), 'c', "so is the byte after it")
    t |> equal(str_byte(written, 3), 'T', "and so is everything written afterwards")
    t |> equal(str_byte(written, 6), 'L', "right through to the last byte")
    let ranged = build_string() $(var w) {
        w |> write_string(d, 0, 3)
    }
    t |> equal(length(ranged), 1, "the range form truncates the same way")
    t |> equal(str_byte(ranged, 2), 'c', "and copies the whole range regardless")
}

// ===== bounded cores =====

[test(description = "find on a shortened view never reads past its size")]
def test_bounded_find(t : T?) {
    var inscope cut <- truncated("abcdefgh", 3)
    t |> equal(find(cut, "cd"), -1, "find never reads past size")
    t |> equal(find(cut, "cd", 0), -1, "find-from never reads past size")
    t |> equal(find(cut, 'd'), -1, "find char never reads past size")
    t |> equal(rfind(cut, "cd"), -1, "rfind never reads past size")
    t |> equal(find(cut, "bc"), 1, "a match fully inside the view still hits")
    var inscope edge <- to_bytes("abcdef")
    t |> equal(find(edge, "ef"), 4, "a needle ending exactly at the boundary hits")
    t |> equal(rfind(edge, "ef"), 4, "rfind agrees")
    t |> equal(find(edge, "f"), 5, "a one-byte needle at the last offset hits")
    t |> equal(find(edge, "abcdefg"), -1, "a needle longer than the view misses")
    t |> equal(rfind(edge, "abcdefg"), -1, "rfind agrees")
    var inscope almost <- truncated("abcdef", 5)
    t |> equal(find(almost, "ef"), -1, "a needle that only matches past size misses")
    t |> equal(rfind(almost, "ef"), -1, "rfind agrees")
    t |> equal(find(almost, "de"), 3, "the same needle one byte earlier still hits")
}

[test(description = "rfind on a view returns the last offset and clamps a start at the size")]
def test_bounded_rfind_clamp(t : T?) {
    var inscope d <- to_bytes("abcabc")
    t |> equal(rfind(d, "bc"), 4, "rfind returns the last offset")
    t |> equal(rfind(d, "bc", 6), 4, "start == size clamps to the last fitting offset")
    t |> equal(rfind(d, "bc", 5), 4, "start above the last fitting offset clamps down")
    t |> equal(rfind(d, "bc", 4), 4, "start exactly at the hit")
    t |> equal(rfind(d, "bc", 3), 1, "start below the last hit finds the earlier one")
    t |> equal(rfind(d, "bc", 1), 1, "start exactly at the earlier hit")
    t |> equal(rfind(d, "bc", 0), -1, "start 0 only tests offset 0")
    t |> equal(rfind(d, "bc", -1), -1, "a negative start is -1")
    t |> equal(rfind(d, "abc", -1), -1, "a negative start is -1 even where offset 0 matches")
    t |> equal(rfind(d, "bc", 100), 4, "a start past size clamps to the last fitting offset")
    t |> equal(rfind(d, "abcabc", 0), 0, "a whole-view needle still matches at 0")
    t |> equal(rfind(d, "", 3), -1, "an empty needle is -1, not the start")
}

[test(description = "strip and trim of an all-whitespace view return null without reading past the size")]
def test_bounded_strip_and_trim(t : T?) {
    var inscope ws <- to_bytes(" \t\n\r\f\v ")
    t |> success(is_null_str(strip(ws)), "an all-whitespace view strips to null")
    t |> success(is_null_str(strip_left(ws)), "strip_left agrees")
    t |> success(is_null_str(strip_right(ws)), "strip_right agrees")
    t |> success(is_null_str(trim(ws)), "trim agrees")
    t |> success(is_null_str(ltrim(ws)), "ltrim agrees")
    t |> success(is_null_str(rtrim(ws)), "rtrim agrees")
    var inscope both <- to_bytes("  a  ")
    t |> equal(strip(both), "a", "a whitespace run on both boundaries strips to the middle")
    t |> equal(trim(both), "a", "trim agrees")
    t |> equal(strip_left(both), "a  ", "strip_left leaves the trailing run")
    t |> equal(strip_right(both), "  a", "strip_right leaves the leading run")
    var inscope cut <- truncated("ab  cd", 4)
    t |> equal(rtrim(cut), "ab", "the rtrim walker stops at size")
    t |> equal(strip_right(cut), "ab", "the strip_right walker stops at size")
    t |> equal(strip(cut), "ab", "so does strip")
    t |> equal(trim(cut), "ab", "and trim")
    var inscope chars <- truncated("abxxcd", 4)
    t |> equal(rtrim(chars, "x"), "ab", "the rtrim char-set walker stops at size")
    var inscope lead <- truncated(" abc", 1)
    t |> success(is_null_str(strip(lead)), "a one-byte all-whitespace view strips to null")
}

[test(description = "integer, real and hex parses of a shortened view stop at its size with the cursor exactly there")]
def test_bounded_parse(t : T?) {
    var inscope digits <- truncated("12345678", 3)
    var cr = ConversionResult.ok
    var off = 0
    t |> equal(int(digits, cr, off), 123, "the parse stops at size, not at a terminator")
    t |> equal(cr, ConversionResult.ok, "and succeeds")
    t |> equal(off, 3, "with the cursor exactly at size")
    var inscope real <- truncated("1.25xyz", 4)
    off = 0
    t |> equal(double(real, cr, off), 1.25lf, "the real parse stops at size too")
    t |> equal(off, 4, "with the cursor exactly at size")
    var inscope hex <- truncated("0x1Fzz", 4)
    off = 0
    t |> equal(int(hex, cr, off, true), 31, "the hex parse stops at size")
    t |> equal(off, 4, "with the cursor exactly at size")
}

[test(description = "repeated parses walk the numbers of a comma-separated view with the cursor advancing correctly")]
def test_bounded_parse_cursor_walk(t : T?) {
    var inscope csv <- to_bytes("10,20,30")
    var cr = ConversionResult.ok
    var off = 0
    var inscope seen : array<int>
    while (off < length(csv)) {
        let v = int(csv, cr, off)
        break if (cr != ConversionResult.ok)
        seen |> push(v)
        off ++     // step over the separator
    }
    t |> equal(length(seen), 3, "the cursor walks every number in the view")
    t |> equal(seen[0], 10, "first number")
    t |> equal(seen[1], 20, "second number")
    t |> equal(seen[2], 30, "third number")
    t |> equal(off, 9, "the last step leaves the cursor one past the view")
}

[test(description = "a parse cursor that is negative, past the size or at the size parses nothing, reports invalid_argument and stays untouched")]
def test_parse_offset_clamp(t : T?) {
    var inscope d <- to_bytes("42")
    var cr = ConversionResult.ok
    var off = 0
    t |> equal(int(d, cr, off), 42, "offset 0 parses the whole view")
    t |> equal(off, 2, "and consumes it")
    // out-of-range cursors are rejected without touching the cursor
    off = -1
    t |> equal(int(d, cr, off), 0, "a negative cursor parses nothing")
    t |> equal(cr, ConversionResult.invalid_argument, "and reports invalid_argument")
    t |> equal(off, -1, "and leaves the cursor untouched")
    off = 3
    t |> equal(int(d, cr, off), 0, "a cursor past size parses nothing")
    t |> equal(cr, ConversionResult.invalid_argument, "and reports invalid_argument")
    t |> equal(off, 3, "and leaves the cursor untouched")
    off = 2
    t |> equal(int(d, cr, off), 0, "a cursor exactly at size parses nothing")
    t |> equal(cr, ConversionResult.invalid_argument, "and reports invalid_argument")
    t |> equal(off, 2, "and leaves the cursor untouched")
    off = -1
    t |> equal(double(d, cr, off), 0.0lf, "the real form rejects a negative cursor too")
    t |> equal(off, -1, "leaving it untouched")
    off = 3
    t |> equal(double(d, cr, off), 0.0lf, "and a cursor past size")
    t |> equal(off, 3, "leaving it untouched")
    off = 2
    t |> equal(double(d, cr, off), 0.0lf, "and a cursor exactly at size")
    t |> equal(cr, ConversionResult.invalid_argument, "reporting invalid_argument")
    t |> equal(off, 2, "leaving it untouched")
}

// Parity property harness for the byte-view string surface.
// Every view extern that has a string counterpart is checked as
//     twin(d, args) == counterpart(string(d), args)
// over NUL-free buffers x an index / needle / offset / cmpLen
// lattice.
//
// Interior-NUL behaviour is deliberately NOT here: that is where the
// two surfaces must differ by design, and it is pinned in the
// interior-NUL tests above.
//
// Every buffer is assembled at runtime. With all-constant arguments
// the optimizer folds the whole call away, and the property would
// compare the const-folder against itself.
//
// The view_size INT_MAX guard (view_size in
// module_builtin_string.cpp) has no runtime test here - tripping it
// needs a >2GB array. It holds by inspection: every view entry sizes
// itself through view_size, which throws before the uint32 truncation
// can happen.

struct private Case {
    name : string
    bytes : array<uint8>
}

def private add_text(var cases : array<Case>; name, text : string) {
    var b <- to_bytes(text)
    cases |> emplace(Case(name = name, bytes <- b))
}

// high bytes cannot be written as das literals, so they arrive as raw
// byte values
def private add_raw(var cases : array<Case>; name, head : string; mid : array<int>; tail : string) {
    var b <- to_bytes(head)
    b |> reserve(length(b) + length(mid))
    for (v in mid) {
        b |> push(uint8(v))
    }
    b |> push_from(to_bytes(tail))
    cases |> emplace(Case(name = name, bytes <- b))
}

def private text_cases() : array<Case> {
    var cases : array<Case>
    add_text(cases, "empty", "")
    add_text(cases, "one", "a")
    add_text(cases, "plain", "abcdef")
    add_text(cases, "repeat", "abcabc")
    add_text(cases, "padded", "  ab x ")
    add_text(cases, "spaces", "   ")
    add_text(cases, "all ws", " \t\n\r\f\v")
    add_text(cases, "edge x", "xxabcxx")
    add_text(cases, "ws inside", "a \t b")
    add_raw(cases, "high mid", "ab", [195, 169], "cd")
    add_raw(cases, "high only", "", [255, 254, 128], "")
    add_raw(cases, "high and ws", " ", [200, 9], " ")
    return <- cases
}

def private number_cases() : array<Case> {
    var cases : array<Case>
    add_text(cases, "empty", "")
    add_text(cases, "spaces", "   ")
    add_text(cases, "ws 6set", " \t\n\r\f\v")
    add_text(cases, "int", "123")
    add_text(cases, "lead ws int", "  42xyz")
    add_text(cases, "neg", "-7")
    add_text(cases, "plus", "+5")
    add_text(cases, "garbage", "abc")
    add_text(cases, "lead ws garbage", "  xyz")
    add_text(cases, "overflow", "99999999999999")
    add_text(cases, "byte overflow", "200")
    add_text(cases, "u16 max", "65535")
    add_text(cases, "i64 max", "9223372036854775807")
    add_text(cases, "u64 max", "18446744073709551615")
    add_text(cases, "hex prefixed", "0x1F")
    add_text(cases, "hex bare", "1F")
    add_text(cases, "hex prefix only", "0x")
    add_text(cases, "real", " 2.5e1zz")
    add_text(cases, "real dot", ".5")
    add_text(cases, "real trailing dot", "1.")
    add_text(cases, "real overflow", "1e400")
    add_text(cases, "csv", "1,2,3,4")
    add_raw(cases, "high prefix", "", [200], "42")
    return <- cases
}

// negative / in-range / at-the-boundary / overshooting, for every
// index-like argument
def private idx_lattice(n : int) : array<int> => <- [-100, -2, -1, 0, 1, 3, n - 1, n, n + 1, 100]

def private high_needle() : string {
    return build_string() $(var w) {
        w |> write_char(195)
    }
}

// generic needles plus needles cut from the case itself, so every
// case gets a needle that sits exactly at its start boundary, exactly
// at its end boundary, and one byte too long
def private needles_for(s : string) : array<string> {
    return <- [
        "", "a", "bc", "zz",
        s, s + "d",
        slice(s, 0, 1), slice(s, -1), slice(s, -2),
        high_needle()
    ]
}

def private check_slice(t : T?; name : string; d : array<uint8>) {
    let s = string(d)
    var inscope idx <- idx_lattice(length(d))
    for (a in idx) {
        t |> equal(slice(d, a), slice(s, a), "slice({name},{a})")  // nolint:PERF031 - the parity oracle calls the string counterpart per iteration by design
        for (b in idx) {
            t |> equal(slice(d, a, b), slice(s, a, b), "slice({name},{a},{b})")  // nolint:PERF031 - the parity oracle calls the string counterpart per iteration by design
        }
    }
}

def private check_chop(t : T?; name : string; d : array<uint8>) {
    let s = string(d)
    let n = length(d)
    var inscope idx <- idx_lattice(n)
    var inscope lens <- [-1, 0, 1, 3, n, n + 1, 100]
    for (a in idx) {
        for (l in lens) {
            t |> equal(chop(d, a, l), chop(s, a, l), "chop({name},{a},{l})")  // nolint:PERF031 - the parity oracle calls the string counterpart per iteration by design
        }
    }
}

def private check_find(t : T?; name : string; d : array<uint8>) {
    let s = string(d)
    var inscope needles <- needles_for(s)
    var inscope starts <- idx_lattice(length(d))
    for (nd in needles) {
        t |> equal(find(d, nd), find(s, nd), "find({name},[{nd}])")
        t |> equal(rfind(d, nd), rfind(s, nd), "rfind({name},[{nd}])")
        for (st in starts) {
            t |> equal(find(d, nd, st), find(s, nd, st), "find({name},[{nd}],{st})")
            t |> equal(rfind(d, nd, st), rfind(s, nd, st), "rfind({name},[{nd}],{st})")
        }
    }
}

def private check_find_char(t : T?; name : string; d : array<uint8>) {
    let s = string(d)
    // -61 is byte 0xC3 read as a signed char, 195 is the same byte
    // unsigned: both sides match the unsigned reading, so -61 must miss
    // and 195 must hit
    var inscope chars <- [-61, -1, 0, 9, 32, 'a', 'c', 'x', 'z', 128, 195, 200, 255, 256, 1000]
    var inscope starts <- idx_lattice(length(d))
    for (ch in chars) {
        t |> equal(find(d, ch), find(s, ch), "find_char({name},{ch})")
        for (st in starts) {
            t |> equal(find(d, ch, st), find(s, ch, st), "find_char({name},{ch},{st})")
        }
    }
}

def private check_starts_with(t : T?; name : string; d : array<uint8>) {
    let s = string(d)
    var inscope needles <- needles_for(s)
    var inscope lens <- [0u, 1u, 3u, 100u]
    for (cmp in needles) {
        t |> equal(starts_with(d, cmp), starts_with(s, cmp), "starts_with({name},[{cmp}])")
        t |> equal(ends_with(d, cmp), ends_with(s, cmp), "ends_with({name},[{cmp}])")
        for (cl in lens) {
            t |> equal(starts_with(d, cmp, cl), starts_with(s, cmp, cl),
                       "starts_with({name},[{cmp}],{cl})")
        }
    }
}

def private check_starts_with_at(t : T?; name : string; d : array<uint8>) {
    let s = string(d)
    var inscope needles <- needles_for(s)
    var inscope offs <- idx_lattice(length(d))
    var inscope lens <- [0u, 1u, 3u, 100u]
    for (cmp in needles) {
        for (off in offs) {
            t |> equal(starts_with(d, off, cmp), starts_with(s, off, cmp),
                       "starts_with({name},{off},[{cmp}])")
            for (cl in lens) {
                t |> equal(starts_with(d, off, cmp, cl), starts_with(s, off, cmp, cl),
                    "starts_with({name},{off},[{cmp}],{cl})")
            }
        }
    }
}

def private check_strip_trim(t : T?; name : string; d : array<uint8>) {
    let s = string(d)
    t |> equal(strip(d), strip(s), "strip({name})")
    t |> equal(strip_left(d), strip_left(s), "strip_left({name})")
    t |> equal(strip_right(d), strip_right(s), "strip_right({name})")
    t |> equal(trim(d), trim(s), "trim({name})")
    t |> equal(ltrim(d), ltrim(s), "ltrim({name})")
    t |> equal(rtrim(d), rtrim(s), "rtrim({name})")
    var inscope sets <- ["", "x", "xy", " ", "abc", " \t\n\r\f\v"]
    for (cs in sets) {
        t |> equal(rtrim(d, cs), rtrim(s, cs), "rtrim({name},[{cs}])")
    }
}

def private check_skip_white_space(t : T?; name : string; d : array<uint8>) {
    let s = string(d)
    var inscope offs <- idx_lattice(length(d))
    for (off in offs) {
        t |> equal(skip_white_space(d, off), skip_white_space(s, off),
                   "skip_white_space({name},{off})")
    }
}

// write_string has no string counterpart to compare against; its
// oracle is slice, which the checks above have already tied to the
// string surface
def private check_write(t : T?; name : string; d : array<uint8>) {
    let whole = build_string() $(var w) {
        w |> write_string(d)
    }
    t |> equal(whole, string(d), "write_string({name}) == string(d)")
    var inscope idx <- idx_lattice(length(d))
    for (a in idx) {
        for (b in idx) {
            let part = build_string() $(var w) {
                w |> write_string(d, a, b)
            }
            t |> equal(part, slice(d, a, b), "write_string({name},{a},{b}) == slice")
        }
    }
}

// to_bytes has no string counterpart either - it is the materializing
// inverse of string(d), so its oracle is the round trip: over
// NUL-free bytes it must reproduce the view exactly
def private check_to_bytes(t : T?; name : string; d : array<uint8>) {
    let s = string(d)
    var inscope b <- to_bytes(s)
    t |> equal(length(b), length(d), "to_bytes({name}) length")
    var same = length(b) == length(d)
    for (x, y in b, d) {
        same = same && x == y
    }
    t |> success(same, "to_bytes({name}) reproduces the view byte for byte")
    t |> equal(string(b), s, "string(to_bytes({name})) round-trips")
}

[test(description = "view slice and chop equal the string versions over the text-case lattice")]
def test_parity_slice_and_chop(t : T?) {
    var inscope cases <- text_cases()
    for (c in cases) {
        check_slice(t, c.name, c.bytes)
        check_chop(t, c.name, c.bytes)
    }
}

[test(description = "view find equals the string find over the text-case lattice")]
def test_parity_find(t : T?) {
    var inscope cases <- text_cases()
    for (c in cases) {
        check_find(t, c.name, c.bytes)
    }
}

[test(description = "view find-char equals the string find-char over the text-case lattice")]
def test_parity_find_char(t : T?) {
    var inscope cases <- text_cases()
    for (c in cases) {
        check_find_char(t, c.name, c.bytes)
    }
}

[test(description = "view starts_with and ends_with equal the string versions over the text-case lattice")]
def test_parity_starts_and_ends_with(t : T?) {
    var inscope cases <- text_cases()
    for (c in cases) {
        check_starts_with(t, c.name, c.bytes)
        check_starts_with_at(t, c.name, c.bytes)
    }
}

[test(description = "view strip and trim equal the string versions over the text-case lattice")]
def test_parity_strip_and_trim(t : T?) {
    var inscope cases <- text_cases()
    for (c in cases) {
        check_strip_trim(t, c.name, c.bytes)
    }
}

[test(description = "view skip_white_space equals the string version over the text-case lattice")]
def test_parity_skip_white_space(t : T?) {
    var inscope cases <- text_cases()
    for (c in cases) {
        check_skip_white_space(t, c.name, c.bytes)
    }
}

[test(description = "writing a view equals writing the string over the text-case lattice")]
def test_parity_write_string(t : T?) {
    var inscope cases <- text_cases()
    for (c in cases) {
        check_write(t, c.name, c.bytes)
    }
}

[test(description = "to_bytes of the string form reproduces the original view byte for byte over the text-case lattice")]
def test_parity_to_bytes(t : T?) {
    var inscope cases <- text_cases()
    for (c in cases) {
        check_to_bytes(t, c.name, c.bytes)
    }
}

// ===== parse family =====
// The view forms take offset in AND out; the string forms only write
// it. Parity is therefore at cursor 0: preset the view offset to 0
// and compare the value / result / offset triple. The value is
// compared stringified so that one comparison shape serves all ten
// widths.

def private cmp_parse(t : T?; what, av, bv : string; acr, bcr : ConversionResult; aoff,
                      boff : int) {
    t |> equal(av, bv, "{what} value")
    t |> equal(acr, bcr, "{what} result")
    t |> equal(aoff, boff, "{what} offset")
}

def private par_int8(t : T?; name : string; d : array<uint8>; s : string; hex : bool) {
    var acr = ConversionResult.ok
    var aoff = 0
    let av = int8(d, acr, aoff, hex)
    var bcr = ConversionResult.ok
    var boff = 0
    let bv = int8(s, bcr, boff, hex)
    cmp_parse(t, "int8({name},hex={hex})", "{av}", "{bv}", acr, bcr, aoff, boff)
}

def private par_uint8(t : T?; name : string; d : array<uint8>; s : string; hex : bool) {
    var acr = ConversionResult.ok
    var aoff = 0
    let av = uint8(d, acr, aoff, hex)
    var bcr = ConversionResult.ok
    var boff = 0
    let bv = uint8(s, bcr, boff, hex)
    cmp_parse(t, "uint8({name},hex={hex})", "{av}", "{bv}", acr, bcr, aoff, boff)
}

def private par_int16(t : T?; name : string; d : array<uint8>; s : string; hex : bool) {
    var acr = ConversionResult.ok
    var aoff = 0
    let av = int16(d, acr, aoff, hex)
    var bcr = ConversionResult.ok
    var boff = 0
    let bv = int16(s, bcr, boff, hex)
    cmp_parse(t, "int16({name},hex={hex})", "{av}", "{bv}", acr, bcr, aoff, boff)
}

def private par_uint16(t : T?; name : string; d : array<uint8>; s : string; hex : bool) {
    var acr = ConversionResult.ok
    var aoff = 0
    let av = uint16(d, acr, aoff, hex)
    var bcr = ConversionResult.ok
    var boff = 0
    let bv = uint16(s, bcr, boff, hex)
    cmp_parse(t, "uint16({name},hex={hex})", "{av}", "{bv}", acr, bcr, aoff, boff)
}

def private par_int(t : T?; name : string; d : array<uint8>; s : string; hex : bool) {
    var acr = ConversionResult.ok
    var aoff = 0
    let av = int(d, acr, aoff, hex)
    var bcr = ConversionResult.ok
    var boff = 0
    let bv = int(s, bcr, boff, hex)
    cmp_parse(t, "int({name},hex={hex})", "{av}", "{bv}", acr, bcr, aoff, boff)
}

def private par_uint(t : T?; name : string; d : array<uint8>; s : string; hex : bool) {
    var acr = ConversionResult.ok
    var aoff = 0
    let av = uint(d, acr, aoff, hex)
    var bcr = ConversionResult.ok
    var boff = 0
    let bv = uint(s, bcr, boff, hex)
    cmp_parse(t, "uint({name},hex={hex})", "{av}", "{bv}", acr, bcr, aoff, boff)
}

def private par_int64(t : T?; name : string; d : array<uint8>; s : string; hex : bool) {
    var acr = ConversionResult.ok
    var aoff = 0
    let av = int64(d, acr, aoff, hex)
    var bcr = ConversionResult.ok
    var boff = 0
    let bv = int64(s, bcr, boff, hex)
    cmp_parse(t, "int64({name},hex={hex})", "{av}", "{bv}", acr, bcr, aoff, boff)
}

def private par_uint64(t : T?; name : string; d : array<uint8>; s : string; hex : bool) {
    var acr = ConversionResult.ok
    var aoff = 0
    let av = uint64(d, acr, aoff, hex)
    var bcr = ConversionResult.ok
    var boff = 0
    let bv = uint64(s, bcr, boff, hex)
    cmp_parse(t, "uint64({name},hex={hex})", "{av}", "{bv}", acr, bcr, aoff, boff)
}

def private par_float(t : T?; name : string; d : array<uint8>; s : string) {
    var acr = ConversionResult.ok
    var aoff = 0
    let av = float(d, acr, aoff)
    var bcr = ConversionResult.ok
    var boff = 0
    let bv = float(s, bcr, boff)
    cmp_parse(t, "float({name})", "{av}", "{bv}", acr, bcr, aoff, boff)
}

def private par_double(t : T?; name : string; d : array<uint8>; s : string) {
    var acr = ConversionResult.ok
    var aoff = 0
    let av = double(d, acr, aoff)
    var bcr = ConversionResult.ok
    var boff = 0
    let bv = double(s, bcr, boff)
    cmp_parse(t, "double({name})", "{av}", "{bv}", acr, bcr, aoff, boff)
}

def private check_parse_ints(t : T?; name : string; d : array<uint8>; s : string; hex : bool) {
    par_int8(t, name, d, s, hex)
    par_uint8(t, name, d, s, hex)
    par_int16(t, name, d, s, hex)
    par_uint16(t, name, d, s, hex)
    par_int(t, name, d, s, hex)
    par_uint(t, name, d, s, hex)
    par_int64(t, name, d, s, hex)
    par_uint64(t, name, d, s, hex)
}

[test(description = "view integer parses, decimal and hex, equal the string parses over the number cases")]
def test_parity_parse_ints(t : T?) {
    var inscope cases <- number_cases()
    var inscope hexes <- [false, true]
    for (c in cases) {
        let s = string(c.bytes)
        for (hex in hexes) {
            check_parse_ints(t, c.name, c.bytes, s, hex)
        }
    }
}

[test(description = "view real parses equal the string parses over the number cases")]
def test_parity_parse_reals(t : T?) {
    var inscope cases <- number_cases()
    for (c in cases) {
        let s = string(c.bytes)
        par_float(t, c.name, c.bytes, s)
        par_double(t, c.name, c.bytes, s)
    }
}
