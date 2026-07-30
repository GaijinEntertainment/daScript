options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_function_arguments = false

module imgui_text_flow shared

require math
require daslib/strings_boost

//! View-independent UTF-8 inline-flow primitives used by dasImgui's Markdown,
//! editor, and terminal-facing components. Source positions are always UTF-8
//! byte offsets; screen-space output is a flat list of line fragments.
//!
//! Grapheme grouping deliberately lives here rather than in a renderer. The
//! current implementation covers the clusters that matter most for UI input:
//! combining marks, variation selectors, emoji modifiers, ZWJ sequences,
//! regional-indicator flags, keycaps, and Hangul syllable composition. The API
//! is intentionally independent of the implementation so a generated Unicode
//! property table can replace these predicates without changing consumers.

struct public TextSourceRange {
    start_byte : int
    end_byte : int
}

struct public TextGrapheme {
    source : TextSourceRange
    first_codepoint : uint
    last_codepoint : uint
}

struct public TextFlowSpan {
    //! UTF-8 source bytes belonging to this semantic/style run.
    source : TextSourceRange
    style_id : int
    semantic_id : int = -1
    //! Optional syntax/theme role supplied by a language provider. The flow
    //! core preserves this opaque value without depending on any parser.
    syntax_style_id : int = -1

    //! Adds URI-friendly opportunities after '/', '?', '&', '#', '=', '-',
    //! '_', '.', ':', and '%'. Natural whitespace wrapping remains enabled.
    uri_wrap : bool = false

    //! Optional inline control placed immediately before this span. Control
    //! kinds are consumer-defined; the flow core only preserves the value.
    leading_control_id : int = -1
    leading_control_kind : int = 0
    leading_control_size : float2

    //! Optional inline control placed immediately after this span. Its space is
    //! always part of layout even when the renderer chooses not to draw it.
    trailing_control_id : int = -1
    trailing_control_kind : int = 0
    trailing_control_size : float2
}

struct public TextFlowFragment {
    source : TextSourceRange
    style_id : int
    semantic_id : int
    syntax_style_id : int = -1
    line_index : int
    rect : float4                 //!< local (x0, y0, x1, y1)
    control_id : int = -1
    control_kind : int = 0
    is_control : bool = false
}

struct public TextFlowLine {
    first_fragment : int
    fragment_count : int
    source : TextSourceRange
    y : float
    width : float
    height : float
}

struct public TextFlowLayout {
    lines : array<TextFlowLine>
    fragments : array<TextFlowFragment>
    size : float2
}

struct public TextFlowHit {
    //! A grapheme-boundary caret produced by hit-testing local layout space.
    valid : bool = false
    byte_offset : int = -1
    line_index : int = -1
    fragment_index : int = -1
    caret : float2
    height : float = 0.0f
    trailing : bool = false
}

enum public TextUriKind {
    absolute
    www
}

struct public TextUri {
    source : TextSourceRange
    kind : TextUriKind = TextUriKind.absolute
}

struct private TextScalar {
    source : TextSourceRange
    codepoint : uint
}

struct private TextFlowUnit {
    source : TextSourceRange
    style_id : int
    semantic_id : int
    syntax_style_id : int
    width : float
    height : float
    break_after : bool
    hard_break : bool
    keep_with_next : bool = false
    control_id : int = -1
    control_kind : int = 0
    is_control : bool = false
}

def private continuation_byte(b : uint) : bool {
    return (b & 0xC0u) == 0x80u
}

def private ascii_alpha(b : int) : bool {
    return ((b >= 'A' && b <= 'Z') || (b >= 'a' && b <= 'z')) // nolint:PERF014
}

def private ascii_digit(b : int) : bool {
    return b >= '0' && b <= '9' // nolint:PERF014
}

def private uri_boundary_before(bytes : array<uint8> const#; at : int) : bool {
    if (at <= 0) return true
    let b = int(bytes[at - 1])
    return b < 0x80 && !ascii_alpha(b) && !ascii_digit(b) && b != '_'
}

def private starts_ascii_ci(bytes : array<uint8> const#; at : int; value : string) : bool {
    if (at < 0 || at + length(value) > length(bytes)) return false
    var matches = true
    peek_data(value) $(expected) {
        for (i in range(length(expected))) {
            var a = int(bytes[at + i])
            var b = int(expected[i])
            if (a >= 'A' && a <= 'Z') { // nolint:PERF014
                a += 32
            }
            if (b >= 'A' && b <= 'Z') { // nolint:PERF014
                b += 32
            }
            if (a != b) {
                matches = false
            }
        }
    }
    return matches
}

def private known_uri_scheme(value : string) : bool {
    let lower = to_lower(value)
    return (lower == "http" || lower == "https" || lower == "ftp"
        || lower == "file" || lower == "mailto" || lower == "tel"
        || lower == "urn" || lower == "ssh" || lower == "git"
        || lower == "vscode")
}

def private uri_end(bytes : array<uint8> const#; start : int) : int {
    var at = start
    var parens = 0
    while (at < length(bytes)) {
        let b = int(bytes[at])
        if (b <= 0x20 || b == '"' || b == '\''
                || b == '<' || b == '>' || b == '`'
                || b == '[' || b == ']'
                || b == '{' || b == '}') break
        if (b == '(') {
            parens++
        } elif (b == ')') {
            if (parens == 0) break
            parens--
        }
        at++
    }
    while (at > start) {
        let b = int(bytes[at - 1])
        if (b == '.' || b == ',' || b == ';'
                || b == '!' || b == '?' || b == ':') {
            at--
        } else {
            break
        }
    }
    return at
}

def public text_uris(text : string) : array<TextUri> {
    //! Find URI-shaped spans in arbitrary UTF-8 text. Lives below Markdown because terminals,
    //! editors, logs, and Markdown leaves all need identical recognition. Unicode bytes are
    //! accepted in the URI body; surrounding punctuation is excluded conservatively.
    var out : array<TextUri>
    peek_data(text) $(bytes) {
        var at = 0
        while (at < length(bytes)) {
            if (!uri_boundary_before(bytes, at)) {
                at++
                continue
            }
            var body = -1
            var kind = TextUriKind.absolute
            if (starts_ascii_ci(bytes, at, "www.")) {
                body = at + 4
                kind = TextUriKind.www
            } elif (ascii_alpha(int(bytes[at]))) {
                var colon = at + 1
                while (colon < length(bytes)) {
                    let b = int(bytes[colon])
                    if (!ascii_alpha(b) && !ascii_digit(b)
                            && b != '+' && b != '-' && b != '.') break
                    colon++
                }
                if (colon < length(bytes) && int(bytes[colon]) == ':') {
                    let slash_form = (colon + 2 < length(bytes)
                        && int(bytes[colon + 1]) == '/'
                        && int(bytes[colon + 2]) == '/')
                    if (slash_form || known_uri_scheme(slice(text, at, colon))) {
                        body = slash_form ? colon + 3 : colon + 1
                    }
                }
            }
            if (body >= 0) {
                let end_ = uri_end(bytes, body)
                if (end_ > body) {
                    out |> push(TextUri(
                        source = TextSourceRange(start_byte = at, end_byte = end_),
                        kind = kind))
                    at = end_
                    continue
                }
            }
            at++
        }
    }
    return <- out
}

def private decode_scalars(text : string; var out : array<TextScalar>) {
    peek_data(text) $(bytes) {
        let n = length(bytes)
        var i = 0
        while (i < n) {
            let start = i
            let b0 = uint(bytes[i])
            var cp = 0xFFFDu
            var count = 1
            if (b0 < 0x80u) {
                cp = b0
            } elif (b0 >= 0xC2u && b0 <= 0xDFu && i + 1 < n
                    && continuation_byte(uint(bytes[i + 1]))) {
                cp = ((b0 & 0x1Fu) << 6u) | (uint(bytes[i + 1]) & 0x3Fu)
                count = 2
            } elif (b0 >= 0xE0u && b0 <= 0xEFu && i + 2 < n
                    && continuation_byte(uint(bytes[i + 1]))
                    && continuation_byte(uint(bytes[i + 2]))) {
                let b1 = uint(bytes[i + 1])
                let valid = ((b0 != 0xE0u || b1 >= 0xA0u)
                         && (b0 != 0xEDu || b1 < 0xA0u))
                if (valid) {
                    cp = (((b0 & 0x0Fu) << 12u) | ((b1 & 0x3Fu) << 6u)
                       | (uint(bytes[i + 2]) & 0x3Fu))
                    count = 3
                }
            } elif (b0 >= 0xF0u && b0 <= 0xF4u && i + 3 < n
                    && continuation_byte(uint(bytes[i + 1]))
                    && continuation_byte(uint(bytes[i + 2]))
                    && continuation_byte(uint(bytes[i + 3]))) {
                let b1 = uint(bytes[i + 1])
                let valid = ((b0 != 0xF0u || b1 >= 0x90u)
                         && (b0 != 0xF4u || b1 < 0x90u))
                if (valid) {
                    cp = (((b0 & 0x07u) << 18u) | ((b1 & 0x3Fu) << 12u)
                       | ((uint(bytes[i + 2]) & 0x3Fu) << 6u)
                       | (uint(bytes[i + 3]) & 0x3Fu))
                    count = 4
                }
            }
            i += count
            out |> push(TextScalar(
                source = TextSourceRange(start_byte = start, end_byte = i),
                codepoint = cp))
        }
    }
}

def private in_range(cp, lo, hi : uint) : bool {
    return cp >= lo && cp <= hi
}

def private is_control(cp : uint) : bool {
    return (cp == 0x000Au || cp == 0x000Du
        || in_range(cp, 0x0000u, 0x001Fu)
        || in_range(cp, 0x007Fu, 0x009Fu))
}

def private is_extend(cp : uint) : bool {
    // Combining ranges plus variation selectors, emoji modifiers/tags, and combining keycaps.
    return (in_range(cp, 0x0300u, 0x036Fu)
        || in_range(cp, 0x0483u, 0x0489u)
        || in_range(cp, 0x0591u, 0x05BDu) || cp == 0x05BFu
        || in_range(cp, 0x05C1u, 0x05C2u) || in_range(cp, 0x05C4u, 0x05C5u)
        || in_range(cp, 0x0610u, 0x061Au) || in_range(cp, 0x064Bu, 0x065Fu)
        || cp == 0x0670u || in_range(cp, 0x06D6u, 0x06EDu)
        || in_range(cp, 0x0711u, 0x0711u) || in_range(cp, 0x0730u, 0x074Au)
        || in_range(cp, 0x07A6u, 0x07B0u) || in_range(cp, 0x07EBu, 0x07F3u)
        || in_range(cp, 0x0816u, 0x082Du) || in_range(cp, 0x0859u, 0x085Bu)
        || in_range(cp, 0x08D3u, 0x0902u)
        || in_range(cp, 0x093Au, 0x093Cu) || in_range(cp, 0x0941u, 0x0948u)
        || in_range(cp, 0x094Du, 0x094Du) || in_range(cp, 0x0951u, 0x0957u)
        || in_range(cp, 0x0962u, 0x0963u)
        || in_range(cp, 0x1AB0u, 0x1AFFu)
        || in_range(cp, 0x1DC0u, 0x1DFFu)
        || in_range(cp, 0x20D0u, 0x20FFu)
        || in_range(cp, 0xFE00u, 0xFE0Fu)
        || in_range(cp, 0xFE20u, 0xFE2Fu)
        || in_range(cp, 0x1F3FBu, 0x1F3FFu)
        || in_range(cp, 0xE0020u, 0xE007Fu)
        || in_range(cp, 0xE0100u, 0xE01EFu))
}

def private is_spacing_mark(cp : uint) : bool {
    // High-value Mc ranges: prevent caret stops inside the visible consonant/vowel cluster.
    return (cp == 0x0903u || in_range(cp, 0x093Bu, 0x0940u)
        || in_range(cp, 0x0949u, 0x094Cu) || in_range(cp, 0x0982u, 0x0983u)
        || in_range(cp, 0x09BEu, 0x09C0u) || in_range(cp, 0x09C7u, 0x09C8u)
        || in_range(cp, 0x09CBu, 0x09CCu) || in_range(cp, 0x0A3Eu, 0x0A40u)
        || in_range(cp, 0x0ABEu, 0x0AC0u) || in_range(cp, 0x0B3Eu, 0x0B40u)
        || in_range(cp, 0x0BBEu, 0x0BC2u) || in_range(cp, 0x0BC6u, 0x0BC8u)
        || in_range(cp, 0x0BCAu, 0x0BCCu) || in_range(cp, 0x0C01u, 0x0C03u)
        || in_range(cp, 0x0C41u, 0x0C44u) || in_range(cp, 0x0D3Eu, 0x0D40u)
        || in_range(cp, 0x0D46u, 0x0D48u) || in_range(cp, 0x0D4Au, 0x0D4Cu))
}

def private is_prepend(cp : uint) : bool {
    return (in_range(cp, 0x0600u, 0x0605u) || cp == 0x06DDu || cp == 0x070Fu
        || cp == 0x08E2u || cp == 0x110BDu || in_range(cp, 0x111C2u, 0x111C3u))
}

def private is_regional_indicator(cp : uint) : bool {
    return in_range(cp, 0x1F1E6u, 0x1F1FFu)
}

def private is_hangul_l(cp : uint) : bool {
    return in_range(cp, 0x1100u, 0x115Fu) || in_range(cp, 0xA960u, 0xA97Cu)
}

def private is_hangul_v(cp : uint) : bool {
    return in_range(cp, 0x1160u, 0x11A7u) || in_range(cp, 0xD7B0u, 0xD7C6u)
}

def private is_hangul_t(cp : uint) : bool {
    return in_range(cp, 0x11A8u, 0x11FFu) || in_range(cp, 0xD7CBu, 0xD7FBu)
}

def private is_hangul_lv(cp : uint) : bool {
    return in_range(cp, 0xAC00u, 0xD7A3u) && ((cp - 0xAC00u) % 28u) == 0u
}

def private is_hangul_lvt(cp : uint) : bool {
    return in_range(cp, 0xAC00u, 0xD7A3u) && ((cp - 0xAC00u) % 28u) != 0u
}

def private should_break_grapheme(prev, cur : uint; regional_run : int) : bool {
    if (prev == 0x000Du && cur == 0x000Au) return false
    if (is_control(prev) || is_control(cur)) return true

    let joins_hangul = (
        (is_hangul_l(prev) && (is_hangul_l(cur) || is_hangul_v(cur)
            || is_hangul_lv(cur) || is_hangul_lvt(cur)))
        || ((is_hangul_lv(prev) || is_hangul_v(prev))
            && (is_hangul_v(cur) || is_hangul_t(cur)))
        || ((is_hangul_lvt(prev) || is_hangul_t(prev)) && is_hangul_t(cur)))
    // ZWJ-join is intentionally broad — keeps emoji families/professions atomic (narrow to Extended_Pictographic later).
    if (joins_hangul || is_prepend(prev) || is_extend(cur)
            || is_spacing_mark(cur) || cur == 0x200Du || prev == 0x200Du) return false

    if (is_regional_indicator(prev) && is_regional_indicator(cur)) {
        return (regional_run % 2) == 0
    }
    return true
}

def public text_graphemes(text : string) : array<TextGrapheme> {
    var scalars : array<TextScalar>
    decode_scalars(text, scalars)
    var out : array<TextGrapheme>
    if (empty(scalars)) {
        delete scalars
        return <- out
    }

    var cluster_start = 0
    var regional_run = is_regional_indicator(scalars[0].codepoint) ? 1 : 0
    for (i in range(1, length(scalars))) {
        let prev = scalars[i - 1].codepoint
        let cur = scalars[i].codepoint
        let split = should_break_grapheme(prev, cur, regional_run)
        if (split) {
            out |> push(TextGrapheme(
                source = TextSourceRange(
                    start_byte = scalars[cluster_start].source.start_byte,
                    end_byte = scalars[i - 1].source.end_byte),
                first_codepoint = scalars[cluster_start].codepoint,
                last_codepoint = prev))
            cluster_start = i
            regional_run = is_regional_indicator(cur) ? 1 : 0
        } elif (is_regional_indicator(cur)) {
            regional_run++
        } elif (!is_extend(cur) && cur != 0x200Du) {
            regional_run = 0
        }
    }
    out |> push(TextGrapheme(
        source = TextSourceRange(
            start_byte = scalars[cluster_start].source.start_byte,
            end_byte = scalars[length(scalars) - 1].source.end_byte),
        first_codepoint = scalars[cluster_start].codepoint,
        last_codepoint = scalars[length(scalars) - 1].codepoint))
    delete scalars
    return <- out
}

def private is_wide_text_codepoint(cp : uint) : bool {
    // Compact East-Asian-wide + emoji subset for terminal/editor cells (isolated so generated width tables can replace it).
    return (in_range(cp, 0x1100u, 0x115Fu)
        || in_range(cp, 0x231Au, 0x231Bu)
        || in_range(cp, 0x2329u, 0x232Au)
        || in_range(cp, 0x23E9u, 0x23ECu) || cp == 0x23F0u || cp == 0x23F3u
        || in_range(cp, 0x25FDu, 0x25FEu)
        || in_range(cp, 0x2614u, 0x2615u)
        || in_range(cp, 0x2648u, 0x2653u)
        || cp == 0x267Fu || cp == 0x2693u || cp == 0x26A1u
        || in_range(cp, 0x26AAu, 0x26ABu)
        || in_range(cp, 0x26BDu, 0x26BEu)
        || in_range(cp, 0x26C4u, 0x26C5u) || cp == 0x26CEu
        || cp == 0x26D4u || cp == 0x26EAu
        || in_range(cp, 0x26F2u, 0x26F3u) || cp == 0x26F5u
        || cp == 0x26FAu || cp == 0x26FDu
        || cp == 0x2705u || in_range(cp, 0x270Au, 0x270Bu)
        || cp == 0x2728u || cp == 0x274Cu || cp == 0x274Eu
        || in_range(cp, 0x2753u, 0x2755u) || cp == 0x2757u
        || in_range(cp, 0x2795u, 0x2797u) || cp == 0x27B0u || cp == 0x27BFu
        || in_range(cp, 0x2B1Bu, 0x2B1Cu) || cp == 0x2B50u || cp == 0x2B55u
        || in_range(cp, 0x2E80u, 0xA4CFu)
        || in_range(cp, 0xAC00u, 0xD7A3u)
        || in_range(cp, 0xF900u, 0xFAFFu)
        || in_range(cp, 0xFE10u, 0xFE19u)
        || in_range(cp, 0xFE30u, 0xFE6Fu)
        || in_range(cp, 0xFF01u, 0xFF60u)
        || in_range(cp, 0xFFE0u, 0xFFE6u)
        || in_range(cp, 0x1F1E6u, 0x1F1FFu)
        || in_range(cp, 0x1F300u, 0x1FAFFu)
        || in_range(cp, 0x20000u, 0x3FFFDu))
}

def public text_grapheme_cell_width(grapheme : TextGrapheme const) : int {
    //! Terminal/editor display cells for one already-grouped grapheme. Newline
    //! controls occupy no cell; CJK, emoji, flags, keycaps, and emoji-presentation
    //! sequences occupy two. Combining/ZWJ bytes never add cells independently.
    let cp = grapheme.first_codepoint
    if (cp == 0x000Au || cp == 0x000Du) return 0
    if (cp == 0x0009u) return 4
    if (is_wide_text_codepoint(cp)
            || grapheme.last_codepoint == 0xFE0Fu
            || grapheme.last_codepoint == 0x20E3u) return 2
    return 1
}

def public text_cell_width(text : string) : int {
    //! Sum display cells across UTF-8 graphemes. Intended for source/editor
    //! layout and tests; line breaking remains the caller's responsibility.
    var width = 0
    var graphemes <- text_graphemes(text)
    for (grapheme in graphemes) {
        width += text_grapheme_cell_width(grapheme)
    }
    delete graphemes
    return width
}

def public text_previous_grapheme_boundary(text : string; at : int) : int {
    //! Move one caret stop left without entering a UTF-8 grapheme cluster.
    let caret = clamp(at, 0, length(text))
    var result = 0
    var graphemes <- text_graphemes(text)
    for (g in graphemes) {
        if (g.source.start_byte >= caret) break
        result = g.source.start_byte
    }
    delete graphemes
    return result
}

def public text_next_grapheme_boundary(text : string; at : int) : int {
    //! Move one caret stop right without entering a UTF-8 grapheme cluster.
    let caret = clamp(at, 0, length(text))
    var result = length(text)
    var graphemes <- text_graphemes(text)
    for (g in graphemes) {
        if (g.source.end_byte > caret) {
            result = g.source.end_byte
            break
        }
    }
    delete graphemes
    return result
}

def private natural_break_after(cp : uint) : bool {
    return cp == 0x0020u || cp == 0x0009u || cp == 0x200Bu
}

def private uri_break_after(cp : int) : bool {
    return (cp == '/' || cp == '?' || cp == '&'
        || cp == '#' || cp == '=' || cp == '-'
        || cp == '_' || cp == '.' || cp == ':'
        || cp == '%')
}

def private make_units(source : string; spans : array<TextFlowSpan> const;
                       line_height : float;
                       measure : block<(text : string; start_byte : int;
                                        end_byte : int; style_id : int) : float>) : array<TextFlowUnit> {
    var units : array<TextFlowUnit>
    let source_len = length(source)
    units |> reserve(source_len + length(spans) * 2)
    for (span in spans) {
        let a = clamp(span.source.start_byte, 0, source_len)
        let b = clamp(span.source.end_byte, a, source_len)
        if (span.leading_control_id >= 0 && span.leading_control_size.x > 0.0f) {
            units |> push(TextFlowUnit(
                source = TextSourceRange(start_byte = a, end_byte = a),
                style_id = span.style_id,
                semantic_id = span.semantic_id,
                syntax_style_id = span.syntax_style_id,
                width = span.leading_control_size.x,
                height = max(line_height, span.leading_control_size.y),
                break_after = false,
                hard_break = false,
                keep_with_next = true,
                control_id = span.leading_control_id,
                control_kind = span.leading_control_kind,
                is_control = true))
        }
        let span_text = slice(source, a, b)
        var graphemes <- text_graphemes(span_text)
        for (g in graphemes) {
            let ga = a + g.source.start_byte
            let gb = a + g.source.end_byte
            let hard = g.first_codepoint == 0x000Au || g.first_codepoint == 0x000Du
            let measured_width = (hard ? 0.0f
                : invoke(measure, source, ga, gb, span.style_id))
            units |> push(TextFlowUnit(
                source = TextSourceRange(start_byte = ga, end_byte = gb),
                style_id = span.style_id,
                semantic_id = span.semantic_id,
                syntax_style_id = span.syntax_style_id,
                width = measured_width,
                height = line_height,
                break_after = natural_break_after(g.last_codepoint)
                    || (span.uri_wrap && uri_break_after(int(g.last_codepoint))),
                hard_break = hard))
        }
        delete graphemes
        if (span.trailing_control_id >= 0 && span.trailing_control_size.x > 0.0f) {
            units |> push(TextFlowUnit(
                source = TextSourceRange(start_byte = b, end_byte = b),
                style_id = span.style_id,
                semantic_id = span.semantic_id,
                syntax_style_id = span.syntax_style_id,
                width = span.trailing_control_size.x,
                height = max(line_height, span.trailing_control_size.y),
                break_after = true,
                hard_break = false,
                control_id = span.trailing_control_id,
                control_kind = span.trailing_control_kind,
                is_control = true))
        }
    }
    return <- units
}

def private push_fragment(var layout : TextFlowLayout; units : array<TextFlowUnit> const;
                          first, end_, line_index : int; x, y, line_height : float) : float {
    let u0 = units[first]
    var width = 0.0f
    for (i in range(first, end_)) {
        width += units[i].width
    }

    var rect_y = y
    var rect_h = line_height
    if (u0.is_control) {
        rect_h = u0.height
        rect_y = y + (line_height - rect_h) * 0.5f
    }
    layout.fragments |> push(TextFlowFragment(
        source = TextSourceRange(
            start_byte = u0.source.start_byte,
            end_byte = units[end_ - 1].source.end_byte),
        style_id = u0.style_id,
        semantic_id = u0.semantic_id,
        syntax_style_id = u0.syntax_style_id,
        line_index = line_index,
        rect = float4(x, rect_y, x + width, rect_y + rect_h),
        control_id = u0.control_id,
        control_kind = u0.control_kind,
        is_control = u0.is_control))
    return width
}

def private emit_line(var layout : TextFlowLayout; units : array<TextFlowUnit> const;
                      first, end_ : int; y, fallback_height : float) : float {
    var line_height = fallback_height
    var line_width = 0.0f
    for (i in range(first, end_)) {
        line_height = max(line_height, units[i].height)
        line_width += units[i].width
    }

    let first_fragment = length(layout.fragments)
    let line_index = length(layout.lines)
    var x = 0.0f
    var fragment_start = first
    var i = first
    while (i < end_) {
        let a = units[fragment_start]
        let b = units[i]
        var same = false
        if (i > fragment_start) {
            same = (!a.is_control && !b.is_control
                && a.style_id == b.style_id && a.semantic_id == b.semantic_id
                && a.syntax_style_id == b.syntax_style_id
                && units[i - 1].source.end_byte == b.source.start_byte)
        }
        if (i > fragment_start && !same) {
            x += push_fragment(layout, units, fragment_start, i,
                               line_index, x, y, line_height)
            fragment_start = i
        }
        if (b.is_control) {
            if (i > fragment_start) {
                x += push_fragment(layout, units, fragment_start, i,
                                   line_index, x, y, line_height)
            }
            x += push_fragment(layout, units, i, i + 1,
                               line_index, x, y, line_height)
            fragment_start = i + 1
        }
        i++
    }
    if (fragment_start < end_) {
        push_fragment(layout, units, fragment_start, end_,
                      line_index, x, y, line_height)
    }

    var source_start = 0
    if (first < end_) {
        source_start = units[first].source.start_byte
    } elif (first < length(units)) {
        source_start = units[first].source.start_byte
    } elif (!empty(units)) {
        source_start = units[length(units) - 1].source.end_byte
    }
    let source_end = first < end_ ? units[end_ - 1].source.end_byte : source_start
    layout.lines |> push(TextFlowLine(
        first_fragment = first_fragment,
        fragment_count = length(layout.fragments) - first_fragment,
        source = TextSourceRange(start_byte = source_start, end_byte = source_end),
        y = y,
        width = line_width,
        height = line_height))
    return line_height
}

def private layout_units(units : array<TextFlowUnit> const;
                         max_width, line_height : float) : TextFlowLayout {
    var layout : TextFlowLayout
    let width_limit = max(max_width, 1.0f)
    var start = 0
    var y = 0.0f
    var widest = 0.0f

    while (start < length(units)) {
        if (units[start].hard_break) {
            let h = emit_line(layout, units, start, start, y, line_height)
            y += h
            start++
            continue
        }

        var i = start
        var width = 0.0f
        var last_break = -1
        var end_ = start
        var consumed_hard_break = false
        while (i < length(units)) {
            if (units[i].hard_break) {
                end_ = i
                consumed_hard_break = true
                break
            }
            let next_width = width + units[i].width
            if (next_width > width_limit && i > start) {
                if (units[i - 1].keep_with_next) {
                    if (i - 1 > start) {
                        end_ = i - 1
                        break
                    }
                    // Leading adornment + first grapheme are the smallest legal unit, even if they exceed the width.
                    width = next_width
                    i++
                    end_ = i
                    break
                } else {
                    end_ = last_break > start ? last_break : i
                    break
                }
            }
            width = next_width
            if (units[i].break_after) {
                last_break = i + 1
            }
            i++
            end_ = i
            if (width > width_limit) break // one oversized grapheme/control
        }

        let h = emit_line(layout, units, start, end_, y, line_height)
        widest = max(widest, layout.lines[length(layout.lines) - 1].width)
        y += h
        start = end_
        if (consumed_hard_break && start < length(units) && units[start].hard_break) {
            start++
        }
    }

    // A terminal newline needs a caret line: the hard-break unit adds one final empty visual line mapped to the source-end byte.
    if (!empty(units) && units[length(units) - 1].hard_break) {
        let h = emit_line(layout, units, length(units), length(units),
                          y, line_height)
        y += h
    }

    layout.size = float2(min(widest, width_limit), y)
    return <- layout
}

def public text_flow_hit_test_ranges(source : string;
                                     layout : TextFlowLayout const;
                                     point : float2;
                                     measure : block<(text : string;
                                         start_byte : int; end_byte : int;
                                         style_id : int) : float>) : TextFlowHit {
    //! Map a local screen point to the nearest caret within the text fragment
    //! under it. Carets are emitted only at grapheme boundaries. Inline
    //! controls deliberately do not participate in text selection.
    for (fragment_index in range(length(layout.fragments))) {
        let fragment = layout.fragments[fragment_index]
        if (fragment.is_control || point.x < fragment.rect.x || point.x > fragment.rect.z
                || point.y < fragment.rect.y || point.y > fragment.rect.w) continue

        let a = clamp(fragment.source.start_byte, 0, length(source))
        let b = clamp(fragment.source.end_byte, a, length(source))
        let value = slice(source, a, b)
        var graphemes <- text_graphemes(value)
        var x = fragment.rect.x
        var hit = TextFlowHit()
        for (g in graphemes) {
            let ga = a + g.source.start_byte
            let gb = a + g.source.end_byte
            let width = invoke(measure, source, ga, gb, fragment.style_id)
            if (point.x < x + width * 0.5f) {
                hit = TextFlowHit(
                    valid = true,
                    byte_offset = ga,
                    line_index = fragment.line_index,
                    fragment_index = fragment_index,
                    caret = float2(x, fragment.rect.y),
                    height = fragment.rect.w - fragment.rect.y,
                    trailing = false)
                break
            }
            x += width
        }
        if (!hit.valid) {
            hit = TextFlowHit(
                valid = true,
                byte_offset = b,
                line_index = fragment.line_index,
                fragment_index = fragment_index,
                caret = float2(x, fragment.rect.y),
                height = fragment.rect.w - fragment.rect.y,
                trailing = true)
        }
        delete graphemes
        return hit
    }
    return TextFlowHit()
}

def public layout_text_flow_styled_ranges(source : string;
                                          spans : array<TextFlowSpan> const;
                                          max_width, line_height : float;
                                          measure : block<(text : string;
                                              start_byte : int; end_byte : int;
                                              style_id : int) : float>) : TextFlowLayout {
    //! Greedy inline layout with style-aware measurement. Text is never split
    //! inside a grapheme cluster; the measured style is carried unchanged into
    //! each emitted fragment so rendering can use the identical font role.
    var units <- make_units(source, spans, line_height, measure)
    var layout <- layout_units(units, max_width, line_height)
    delete units
    return <- layout
}

def public layout_text_flow_styled(source : string; spans : array<TextFlowSpan> const;
                                   max_width, line_height : float;
                                   measure : block<(text : string; style_id : int) : float>) : TextFlowLayout {
    return <- layout_text_flow_styled_ranges(source, spans, max_width, line_height,
        $(text : string; start_byte : int; end_byte : int; style_id : int) : float {
            let value = slice(text, start_byte, end_byte)
            let width = invoke(measure, value, style_id)
            return width
        })
}

def public layout_text_flow(source : string; spans : array<TextFlowSpan> const;
                            max_width, line_height : float;
                            measure : block<(text : string) : float>) : TextFlowLayout {
    //! Compatibility overload for consumers whose styles share one font.
    return <- layout_text_flow_styled(source, spans, max_width, line_height,
        $(text : string; _style_id : int) : float {
            return invoke(measure, text)
        })
}
