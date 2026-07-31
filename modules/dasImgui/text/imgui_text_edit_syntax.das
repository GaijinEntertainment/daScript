options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_text_edit_syntax shared

require imgui/imgui_text_source_edit public
require tree_sitter/tree_sitter public
require math

//! Debounced full-parse refinement for the editor's lexical syntax tier:
//! tree-sitter tokens replace the per-line lexical runs (types, functions,
//! operators) once the buffer sits idle. The editor stays usable without
//! this module — the lexical tier already styles comments/strings/keywords.

//! Idle seconds before a full re-parse runs.
let public TEXT_EDIT_SYNTAX_DEBOUNCE = 0.3f
//! Buffers above this size keep the lexical tier only (parse on demand).
let public TEXT_EDIT_SYNTAX_MAX_BYTES = 262144

def private role_style(role : SyntaxRole) : TextSourceSyntaxStyle { // nolint:STYLE037
    if (role == SyntaxRole.comment) return TextSourceSyntaxStyle.comment
    if (role == SyntaxRole.string_ || role == SyntaxRole.character
            || role == SyntaxRole.escape) return TextSourceSyntaxStyle.string_
    if (role == SyntaxRole.number || role == SyntaxRole.boolean_) return TextSourceSyntaxStyle.number
    if (role == SyntaxRole.keyword || role == SyntaxRole.attribute
            || role == SyntaxRole.module_) return TextSourceSyntaxStyle.keyword
    if (role == SyntaxRole.type_) return TextSourceSyntaxStyle.type_
    if (role == SyntaxRole.function_) return TextSourceSyntaxStyle.function_
    if (role == SyntaxRole.variable || role == SyntaxRole.parameter
            || role == SyntaxRole.label_) return TextSourceSyntaxStyle.variable
    if (role == SyntaxRole.property) return TextSourceSyntaxStyle.property
    if (role == SyntaxRole.constant) return TextSourceSyntaxStyle.constant
    if (role == SyntaxRole.operator_) return TextSourceSyntaxStyle.operator_
    if (role == SyntaxRole.punctuation) return TextSourceSyntaxStyle.punctuation
    return TextSourceSyntaxStyle.none
}

def public text_edit_apply_syntax(var ed : TextEditState;
                                  tokens : array<SyntaxToken>; revision : int) {
    //! Replace the per-line runs from full-parse tokens (byte offsets over
    //! the LF-joined `text_edit_text` domain). Any producer may feed this.
    while (length(ed.line_styles) < length(ed.lines)) {
        var blank = TextEditLineStyle()
        ed.line_styles |> emplace(blank)
    }
    while (length(ed.line_styles) > length(ed.lines)) {
        ed.line_styles |> erase(length(ed.line_styles) - 1)
    }
    // build into scratch: the parse must never DOWNGRADE the lexical tier
    var inscope scratch : array<TextEditLineStyle>
    scratch |> resize(length(ed.lines))
    var line = 0
    var line_start = 0
    for (token in tokens) {
        let style = role_style(token.role)
        continue if (style == TextSourceSyntaxStyle.none)
        // advance to the token's first line (tokens arrive ascending)
        while (line < length(ed.lines)
                && line_start + length(ed.lines[line]) < token.start_byte) {
            line_start += length(ed.lines[line]) + 1
            line++
        }
        break if (line >= length(ed.lines))
        // split multi-line tokens at each newline
        var at = max(token.start_byte, line_start)
        var span_line = line
        var span_start = line_start
        while (at < token.end_byte && span_line < length(ed.lines)) {
            let line_end = span_start + length(ed.lines[span_line])
            let piece_end = min(token.end_byte, line_end)
            if (piece_end > at) {
                scratch[span_line].runs |> push(TextEditStyleRun(
                    start_byte = at - span_start, end_byte = piece_end - span_start,
                    style = style))
            }
            break if (token.end_byte <= line_end)
            span_start = line_end + 1
            span_line++
            at = span_start
        }
    }
    // quality bar: a fragment that error-recovers to almost nothing (prose,
    // mid-edit breakage) keeps the lexical styling wholesale
    var parse_bytes = 0
    var lexical_bytes = 0
    for (entry in scratch) {
        for (run in entry.runs) {
            parse_bytes += run.end_byte - run.start_byte
        }
    }
    for (entry in ed.line_styles) {
        for (run in entry.runs) {
            lexical_bytes += run.end_byte - run.start_byte
        }
    }
    if (parse_bytes * 2 >= lexical_bytes) {
        // per line: the parse wins only where it produced runs
        for (index in range(length(ed.lines))) {
            if (!empty(scratch[index].runs)) {
                delete ed.line_styles[index].runs
                ed.line_styles[index].runs <- scratch[index].runs
            }
        }
    }
    ed.syntax_revision = revision
}

def public text_edit_syntax_sync(var ed : TextEditState) : bool {
    //! Frame tick: once the buffer sits idle past the debounce, run the full
    //! tree-sitter parse and refine the lexical styles. Returns true when a
    //! parse ran. No-op without a language, a grammar, or under the debounce.
    return false if (empty(ed.language) || ed.language == "plain"
        || ed.syntax_revision == ed.revision
        || ed.syntax_idle < TEXT_EDIT_SYNTAX_DEBOUNCE
        || !syntax_language_available(ed.language))
    let source = text_edit_text(ed)
    if (length(source) > TEXT_EDIT_SYNTAX_MAX_BYTES) {
        ed.syntax_revision = ed.revision // stay lexical; do not retry per frame
        return false
    }
    var tokens : array<SyntaxToken>
    let revision = ed.revision
    if (syntax_highlight(ed.language, source, tokens) == SyntaxHighlightStatus.ok) {
        text_edit_apply_syntax(ed, tokens, revision)
    } else {
        ed.syntax_revision = revision // grammar failed; lexical tier stands
    }
    delete tokens
    return ed.syntax_revision == revision
}
