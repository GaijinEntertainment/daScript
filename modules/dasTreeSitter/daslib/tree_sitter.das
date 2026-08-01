options gen2
options indenting = 4
options no_aot   // no AOT gen exists for this module (native grammar lib is per-box) — test_aot interprets it

module tree_sitter shared

require tree_sitter_core public
require daslib/utf8_utils
require strings

//! View-independent syntax highlighting backed by one embedded Tree-sitter
//! runtime. All source positions are half-open UTF-8 byte ranges.

enum public SyntaxHighlightStatus {
    ok
    unknown_language
    invalid_source
    incompatible_language
    query_error
    parse_error
}

enum public SyntaxKind {
    unknown
    comment
    literal
    identifier
    keyword
    type_name
    function_name
    property_name
    punctuation
    operator_
    markup
}

enum public SyntaxRole {
    none
    comment
    string_
    character
    number
    boolean_
    keyword
    type_
    function_
    variable
    parameter
    property
    constant
    label_
    attribute
    module_
    operator_
    punctuation
    escape
    title
    emphasis
    strong
    literal
    uri
    reference
    other
}

struct public SyntaxToken {
    start_byte : int
    end_byte : int
    kind : SyntaxKind = SyntaxKind.unknown
    role : SyntaxRole = SyntaxRole.other
    //! Original query capture, such as `function.method.call`. Themes may use
    //! dotted fallback while `role` remains stable across language packs.
    capture_name : string
    language : string
}

struct public SyntaxNodeSpan {
    //! One named multi-line parse node from `syntax_structure` — the outline
    //! granularity used for folding and structural decorations.
    start_byte : int
    end_byte : int
    start_row : int
    end_row : int
    node_type : string
}

def public syntax_canonical_language(language : string) : string {
    // Hosts pass `extension()` straight through and it keeps the leading dot;
    // strip it once here so every language matches by bare name.
    let lowered = to_lower(language)
    let value = starts_with(lowered, ".") ? slice(lowered, 1) : lowered
    if (value == "das" || value == "daslang" || value == "dascript") return "daslang"
    if (value == "c") return "c"
    if (value == "cpp" || value == "c++" || value == "cc" || value == "cxx"
            || value == "h" || value == "hh" || value == "hpp"
            || value == "hxx") return "cpp"
    if (value == "md" || value == "markdown") return "markdown"
    return value
}

def public syntax_language_available(language : string) : bool {
    return _tree_sitter_language_available(language)
}

def public syntax_role_for_capture(capture_name : string) : SyntaxRole { // nolint:STYLE037 — ordered prefix ladder; a split hides that `string.escape` must be tested before `string`
    if (capture_name == "none") return SyntaxRole.none
    if (starts_with(capture_name, "comment")) return SyntaxRole.comment
    if (starts_with(capture_name, "string.escape") || capture_name == "escape"
            || capture_name == "string.special") return SyntaxRole.escape
    if (starts_with(capture_name, "string")) return SyntaxRole.string_
    if (starts_with(capture_name, "character")) return SyntaxRole.character
    if (starts_with(capture_name, "number")) return SyntaxRole.number
    if (starts_with(capture_name, "boolean")) return SyntaxRole.boolean_
    if (starts_with(capture_name, "keyword")) return SyntaxRole.keyword
    if (starts_with(capture_name, "type")) return SyntaxRole.type_
    if (starts_with(capture_name, "function")) return SyntaxRole.function_
    if (capture_name == "variable.parameter") return SyntaxRole.parameter
    if (starts_with(capture_name, "variable")) return SyntaxRole.variable
    if (starts_with(capture_name, "property")) return SyntaxRole.property
    if (starts_with(capture_name, "constant")) return SyntaxRole.constant
    if (starts_with(capture_name, "label")) return SyntaxRole.label_
    if (starts_with(capture_name, "attribute")) return SyntaxRole.attribute
    if (starts_with(capture_name, "module")) return SyntaxRole.module_
    if (capture_name == "operator") return SyntaxRole.operator_
    if (starts_with(capture_name, "punctuation") || capture_name == "delimiter") {
        return SyntaxRole.punctuation
    }
    if (capture_name == "text.title") return SyntaxRole.title
    if (capture_name == "text.emphasis") return SyntaxRole.emphasis
    if (capture_name == "text.strong") return SyntaxRole.strong
    if (capture_name == "text.literal") return SyntaxRole.literal
    if (capture_name == "text.uri") return SyntaxRole.uri
    if (capture_name == "text.reference") return SyntaxRole.reference
    return SyntaxRole.other
}

def public syntax_kind_for_capture(capture_name : string) : SyntaxKind {
    let role = syntax_role_for_capture(capture_name)
    if (role == SyntaxRole.comment) return SyntaxKind.comment
    if (role == SyntaxRole.string_ || role == SyntaxRole.character
            || role == SyntaxRole.number || role == SyntaxRole.boolean_
            || role == SyntaxRole.constant || role == SyntaxRole.escape) {
        return SyntaxKind.literal
    }
    if (role == SyntaxRole.keyword) return SyntaxKind.keyword
    if (role == SyntaxRole.type_) return SyntaxKind.type_name
    if (role == SyntaxRole.function_) return SyntaxKind.function_name
    if (role == SyntaxRole.property) return SyntaxKind.property_name
    if (role == SyntaxRole.variable || role == SyntaxRole.parameter
            || role == SyntaxRole.label_ || role == SyntaxRole.attribute
            || role == SyntaxRole.module_) return SyntaxKind.identifier
    if (role == SyntaxRole.operator_) return SyntaxKind.operator_
    if (role == SyntaxRole.punctuation) return SyntaxKind.punctuation
    if (role == SyntaxRole.title || role == SyntaxRole.emphasis
            || role == SyntaxRole.strong || role == SyntaxRole.literal
            || role == SyntaxRole.uri || role == SyntaxRole.reference) {
        return SyntaxKind.markup
    }
    return SyntaxKind.unknown
}

def public syntax_structure(language, source : string;
                            var spans : array<SyntaxNodeSpan>) : SyntaxHighlightStatus {
    //! Collect every named MULTI-LINE parse node (type + byte range + rows).
    //! Consumers filter by `node_type` — e.g. the fold tables in
    //! `imgui/imgui_text_tree_sitter`.
    delete spans
    if (!is_utf8_string_valid(source)) return SyntaxHighlightStatus.invalid_source
    let canonical = syntax_canonical_language(language)
    let raw_status = _tree_sitter_structure(canonical, source, length(source)) $(node_type : string const#; start_byte, end_byte, start_row, end_row : int) {
        spans |> push(SyntaxNodeSpan(
            start_byte = start_byte,
            end_byte = end_byte,
            start_row = start_row,
            end_row = end_row,
            node_type = "{node_type}"))
    }
    return unsafe(reinterpret<SyntaxHighlightStatus>(raw_status))
}

def public syntax_highlight(language, source : string;
                            var tokens : array<SyntaxToken>) : SyntaxHighlightStatus {
    delete tokens
    if (!is_utf8_string_valid(source)) return SyntaxHighlightStatus.invalid_source
    let canonical = syntax_canonical_language(language)
    let raw_status = _tree_sitter_highlight(canonical, source, length(source)) $(var start_byte, end_byte : int; capture_name : string const#) {
        let capture_value = "{capture_name}"
        tokens |> push(SyntaxToken(
            start_byte = start_byte,
            end_byte = end_byte,
            kind = syntax_kind_for_capture(capture_value),
            role = syntax_role_for_capture(capture_value),
            capture_name = capture_value,
            language = canonical))
    }
    return unsafe(reinterpret<SyntaxHighlightStatus>(raw_status))
}

def public syntax_highlight_and_structure(language, source : string;
                                          var tokens : array<SyntaxToken>;
                                          var spans : array<SyntaxNodeSpan>) : SyntaxHighlightStatus {
    //! `syntax_highlight` + `syntax_structure` from ONE parse of the source —
    //! same results as the two split calls, minus the second full parse.
    delete tokens
    delete spans
    if (!is_utf8_string_valid(source)) return SyntaxHighlightStatus.invalid_source
    let canonical = syntax_canonical_language(language)
    let raw_status = _tree_sitter_highlight_and_structure(canonical, source, length(source),
        $(var start_byte, end_byte : int; capture_name : string const#) {
            let capture_value = "{capture_name}"
            tokens |> push(SyntaxToken(
                start_byte = start_byte,
                end_byte = end_byte,
                kind = syntax_kind_for_capture(capture_value),
                role = syntax_role_for_capture(capture_value),
                capture_name = capture_value,
                language = canonical))
        }) $(node_type : string const#; start_byte, end_byte, start_row, end_row : int) {
        spans |> push(SyntaxNodeSpan(
            start_byte = start_byte,
            end_byte = end_byte,
            start_row = start_row,
            end_row = end_row,
            node_type = "{node_type}"))
    }
    return unsafe(reinterpret<SyntaxHighlightStatus>(raw_status))
}
