options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_text_tree_sitter shared

require imgui/imgui_text_source_model public
require imgui/imgui_text_language public
require tree_sitter/tree_sitter public
require math
require strings

//! Optional Tree-sitter producer for the parser-neutral source-view model.
//! The source model and renderer remain usable without a parser provider.

def private source_syntax_style(role : SyntaxRole) : TextSourceSyntaxStyle {
    if (role == SyntaxRole.comment) {
        return TextSourceSyntaxStyle.comment
    } elif (role == SyntaxRole.string_ || role == SyntaxRole.character
            || role == SyntaxRole.escape) {
        return TextSourceSyntaxStyle.string_
    } elif (role == SyntaxRole.number || role == SyntaxRole.boolean_) {
        return TextSourceSyntaxStyle.number
    } elif (role == SyntaxRole.keyword || role == SyntaxRole.attribute
            || role == SyntaxRole.module_) {
        return TextSourceSyntaxStyle.keyword
    } elif (role == SyntaxRole.type_) {
        return TextSourceSyntaxStyle.type_
    } elif (role == SyntaxRole.function_) {
        return TextSourceSyntaxStyle.function_
    } elif (role == SyntaxRole.variable || role == SyntaxRole.parameter
            || role == SyntaxRole.label_) {
        return TextSourceSyntaxStyle.variable
    } elif (role == SyntaxRole.property) {
        return TextSourceSyntaxStyle.property
    } elif (role == SyntaxRole.constant) {
        return TextSourceSyntaxStyle.constant
    } elif (role == SyntaxRole.operator_) {
        return TextSourceSyntaxStyle.operator_
    } elif (role == SyntaxRole.punctuation) {
        return TextSourceSyntaxStyle.punctuation
    }
    return TextSourceSyntaxStyle.none
}

def private push_source_syntax_span(var document : TextSourceDocument;
                                    start_byte, end_byte : int;
                                    style : TextSourceSyntaxStyle) {
    if (end_byte <= start_byte) return
    document.spans |> push(TextSourceSpan(
        source = TextSourceRange(start_byte = start_byte, end_byte = end_byte),
        syntax_style = style))
}

def private ascii_upper(value : int) : bool {
    return value >= 'A' && value <= 'Z' // nolint:PERF014
}

def private ascii_lower(value : int) : bool {
    return value >= 'a' && value <= 'z' // nolint:PERF014
}

def private das_identifier_start(value : int) : bool {
    return value == '_' || ascii_upper(value) || ascii_lower(value)
}

def private das_identifier_continue(value : int) : bool {
    return das_identifier_start(value) || is_number(value)
}

let private DAS_TYPE_WORDS : table<string> <- {
    "array", "auto", "block", "bool", "double", "float", "float2", "float3",
    "float4", "function", "int", "int16", "int2", "int3", "int4", "int64",
    "int8", "iterator", "lambda", "range", "smart_ptr", "string", "table",
    "tuple", "typedecl", "uint", "uint16", "uint2", "uint3", "uint4",
    "uint64", "uint8", "urange", "variant", "void"
}

var private g_das_keyword_styles : table<string; TextSourceSyntaxStyle>
// One hash lookup instead of ~100 string compares per identifier; longer
// identifiers than the longest keyword skip the lookup AND the slice.
var private g_das_keyword_max_bytes = 0

def private das_word_style(word : string) : TextSourceSyntaxStyle {
    if (word == "true" || word == "false") return TextSourceSyntaxStyle.number
    if (word == "null") return TextSourceSyntaxStyle.constant
    return (key_exists(DAS_TYPE_WORDS, word) ? TextSourceSyntaxStyle.type_
        : TextSourceSyntaxStyle.keyword)
}

def private ensure_das_keyword_styles { // nolint:STYLE038
    if (!empty(g_das_keyword_styles)) return
    // imgui_text_language owns the keyword SET; only the styling lives here.
    text_language_profile("daslang") $(profile) {
        for (word in profile.keywords) {
            g_das_keyword_styles[word] = das_word_style(word)
            g_das_keyword_max_bytes = max(g_das_keyword_max_bytes, length(word))
        }
    }
}

def private das_keyword_style(word : string) : TextSourceSyntaxStyle {
    return g_das_keyword_styles?[word] ?? TextSourceSyntaxStyle.none
}

def public text_source_das_keyword_style(word : string) : TextSourceSyntaxStyle {
    //! Fallback-lexer style for a daslang word; `none` when it is not one of
    //! the daslang profile's keywords.
    ensure_das_keyword_styles()
    return das_keyword_style(word)
}

def private push_das_fallback(var spans : array<TextSourceSpan>;
                              start_byte, end_byte : int;
                              style : TextSourceSyntaxStyle) {
    if (end_byte <= start_byte || style == TextSourceSyntaxStyle.none) return
    spans |> push(TextSourceSpan(
        source = TextSourceRange(start_byte = start_byte, end_byte = end_byte),
        syntax_style = style))
}

def private das_lexical_fallback(source : string) : array<TextSourceSpan> { // nolint:STYLE038
    // Fills query gaps from incomplete snippets / recovery nodes (common in compact Git hunks); tree-sitter stays authoritative.
    var result : array<TextSourceSpan>
    var expect_function = false
    var expect_type = false
    ensure_das_keyword_styles()
    peek_data(source) $(bytes) {
        var at = 0
        while (at < length(bytes)) {
            let value = int(bytes[at])
            if (value == '/' && at + 1 < length(bytes)
                    && int(bytes[at + 1]) == '/') {
                let start = at
                at += 2
                while (at < length(bytes) && int(bytes[at]) != '\n') {
                    at++
                }
                push_das_fallback(result, start, at,
                    TextSourceSyntaxStyle.comment)
                continue
            }
            if (value == '/' && at + 1 < length(bytes)
                    && int(bytes[at + 1]) == '*') {
                let start = at
                at += 2
                while (at + 1 < length(bytes)
                        && !(int(bytes[at]) == '*'
                            && int(bytes[at + 1]) == '/')) {
                    at++
                }
                at = min(length(bytes), at + 2)
                push_das_fallback(result, start, at,
                    TextSourceSyntaxStyle.comment)
                continue
            }
            if (value == '"' || value == '\'') {
                let start = at
                let quote = value
                at++
                while (at < length(bytes)) {
                    if (int(bytes[at]) == '\\' && at + 1 < length(bytes)) {
                        at += 2
                        continue
                    }
                    let current = int(bytes[at])
                    at++
                    if (current == quote) break
                }
                push_das_fallback(result, start, at,
                    TextSourceSyntaxStyle.string_)
                continue
            }
            if (is_number(value)) {
                let start = at
                at++
                while (at < length(bytes)) {
                    let current = int(bytes[at])
                    if (!das_identifier_continue(current) && current != '.') break
                    at++
                }
                push_das_fallback(result, start, at,
                    TextSourceSyntaxStyle.number)
                continue
            }
            if (das_identifier_start(value)) {
                let start = at
                at++
                while (at < length(bytes)
                        && das_identifier_continue(int(bytes[at]))) {
                    at++
                }
                // Expect flags below still style a long name after `def`.
                var word = ""
                if (at - start <= g_das_keyword_max_bytes) {
                    word = slice(source, start, at)
                }
                var style = das_keyword_style(word)
                if (style == TextSourceSyntaxStyle.none && expect_function) {
                    style = TextSourceSyntaxStyle.function_
                    expect_function = false
                } elif (style == TextSourceSyntaxStyle.none && expect_type) {
                    style = TextSourceSyntaxStyle.type_
                    expect_type = false
                }
                push_das_fallback(result, start, at, style)
                if (word == "def") {
                    expect_function = true
                }
                if (word == "struct" || word == "class" || word == "enum"
                        || word == "typedef" || word == "bitfield") {
                    expect_type = true
                }
                continue
            }
            at++
        }
    }
    return <- result
}

def private apply_syntax_fallback(var document : TextSourceDocument;
                                  overlays : array<TextSourceSpan> const) {
    if (empty(overlays)) return
    var merged : array<TextSourceSpan>
    var overlay_index = 0
    for (base in document.spans) {
        while (overlay_index < length(overlays)
                && overlays[overlay_index].source.end_byte
                    <= base.source.start_byte) {
            overlay_index++
        }
        if (base.syntax_style != TextSourceSyntaxStyle.none) {
            merged |> push(base)
            continue
        }
        var cursor = base.source.start_byte
        var scan = overlay_index
        while (scan < length(overlays)
                && overlays[scan].source.start_byte < base.source.end_byte) {
            let overlay = overlays[scan]
            let overlay_start = max(cursor, overlay.source.start_byte)
            let overlay_end = min(base.source.end_byte, overlay.source.end_byte)
            if (overlay_start > cursor) {
                merged |> push(TextSourceSpan(
                    source = TextSourceRange(
                        start_byte = cursor, end_byte = overlay_start),
                    styles = base.styles))
            }
            if (overlay_end > overlay_start) {
                merged |> push(TextSourceSpan(
                    source = TextSourceRange(
                        start_byte = overlay_start, end_byte = overlay_end),
                    styles = base.styles,
                    syntax_style = overlay.syntax_style))
                cursor = overlay_end
            }
            scan++
        }
        if (cursor < base.source.end_byte) {
            merged |> push(TextSourceSpan(
                source = TextSourceRange(
                    start_byte = cursor, end_byte = base.source.end_byte),
                styles = base.styles))
        }
    }
    delete document.spans
    document.spans <- merged
}

def private json_whitespace(value : int) : bool {
    return value == ' ' || value == '\t' || value == '\r' || value == '\n'
}

def private json_number_byte(value : int) : bool {
    return is_number(value) || value == '-' || value == '+' || value == '.' || value == 'e' || value == 'E'
}

def private push_json_span(var document : TextSourceDocument;
                           var covered_until : int&;
                           start_byte, end_byte : int;
                           style : TextSourceSyntaxStyle) {
    push_source_syntax_span(document, covered_until, start_byte,
        TextSourceSyntaxStyle.none)
    push_source_syntax_span(document, start_byte, end_byte, style)
    covered_until = end_byte
}

def private json_lexical_source_document(source : string;
                                         revision : int) : TextSourceDocument {
    // Diffs are often incomplete JSON, so this lexer preserves exact byte coords and colors every token it can, even an unterminated string at a hunk edge.
    var document = TextSourceDocument(source = source, revision = revision)
    var covered_until = 0
    peek_data(source) $(bytes) {
        var at = 0
        while (at < length(bytes)) {
            let value = int(bytes[at])
            if (value == '"') {
                let start = at
                var terminated = false
                at++
                while (at < length(bytes)) {
                    let current = int(bytes[at])
                    if (current == '\\' && at + 1 < length(bytes)) {
                        at += 2
                        continue
                    }
                    at++
                    if (current == '"') {
                        terminated = true
                        break
                    }
                }
                var lookahead = at
                while (lookahead < length(bytes)
                        && json_whitespace(int(bytes[lookahead]))) {
                    lookahead++
                }
                var style = TextSourceSyntaxStyle.string_
                if (terminated && lookahead < length(bytes)
                        && int(bytes[lookahead]) == ':') {
                    style = TextSourceSyntaxStyle.property
                }
                push_json_span(document, covered_until, start, at, style)
                continue
            }
            if (is_number(value) || value == '-') {
                let start = at
                at++
                while (at < length(bytes)
                        && json_number_byte(int(bytes[at]))) {
                    at++
                }
                push_json_span(document, covered_until, start, at,
                    TextSourceSyntaxStyle.number)
                continue
            }
            if (ascii_lower(value)) {
                let start = at
                at++
                while (at < length(bytes) && ascii_lower(int(bytes[at]))) {
                    at++
                }
                let word = slice(source, start, at)
                if (word == "true" || word == "false") {
                    push_json_span(document, covered_until, start, at,
                        TextSourceSyntaxStyle.number)
                } elif (word == "null") {
                    push_json_span(document, covered_until, start, at,
                        TextSourceSyntaxStyle.constant)
                }
                continue
            }
            if (value == '{' || value == '}' || value == '[' || value == ']'
                    || value == ':' || value == ',') {
                push_json_span(document, covered_until, at, at + 1,
                    TextSourceSyntaxStyle.punctuation)
            }
            at++
        }
    }
    push_source_syntax_span(document, covered_until, length(source),
        TextSourceSyntaxStyle.none)
    return <- document
}

def private push_profile_folds(var document : TextSourceDocument;
                               profile : TextLanguageProfile const;
                               spans : array<SyntaxNodeSpan>) {
    for (span in spans) {
        continue if (!key_exists(profile.fold_kinds, span.node_type))
        document.folds |> push(TextSourceFoldRegion(
            start_byte = span.start_byte,
            end_byte = span.end_byte,
            header_row = span.start_row,
            end_row = span.end_row,
            kind = span.node_type))
    }
}

def private collect_profile_folds(var document : TextSourceDocument;
                                  profile : TextLanguageProfile const;
                                  canonical, source : string) {
    var spans : array<SyntaxNodeSpan>
    if (syntax_structure(canonical, source, spans) == SyntaxHighlightStatus.ok) {
        push_profile_folds(document, profile, spans)
    }
    delete spans
}

def public tree_sitter_collect_folds(var document : TextSourceDocument;
                                     canonical, source : string) {
    //! Fill `document.folds` from the parse-tree outline. No-op for
    //! languages whose profile folds nothing, or when the walk fails.
    text_language_profile(canonical) $(profile) {
        if (!empty(profile.fold_kinds)) {
            collect_profile_folds(document, profile, canonical, source)
        }
    }
}

def public tree_sitter_source_document(source, language : string;
                                       revision : int = 0) : TextSourceDocument {
    let canonical = syntax_canonical_language(language)
    if (canonical == "json") {
        return <- json_lexical_source_document(source, revision)
    }
    var document = TextSourceDocument(source = source, revision = revision)
    // folds ride the SAME parse as highlighting when the profile wants them;
    // a second full syntax_structure parse measured as pure loss on big files
    var wants_folds = false
    text_language_profile(canonical) $(profile) {
        wants_folds = !empty(profile.fold_kinds)
    }
    var tokens : array<SyntaxToken>
    var structure_spans : array<SyntaxNodeSpan>
    let highlight_status = (wants_folds
        ? syntax_highlight_and_structure(language, source, tokens, structure_spans)
        : syntax_highlight(language, source, tokens))
    if (highlight_status != SyntaxHighlightStatus.ok) {
        delete tokens
        delete structure_spans
        return <- plain_source_document(source, revision)
    }

    // length(string) is a strlen: per-token calls made token_count x
    // file_size byte scans — 7.6 s on a 716 KB file, 11 ms hoisted.
    let source_length = length(source)
    var covered_until = 0
    document.spans |> reserve(2 * length(tokens) + 1)
    for (token in tokens) {
        let syntax_style = source_syntax_style(token.role)
        if (syntax_style == TextSourceSyntaxStyle.none) continue
        let start_byte = clamp(token.start_byte, covered_until, source_length)
        let end_byte = clamp(token.end_byte, start_byte, source_length)
        push_source_syntax_span(document, covered_until, start_byte,
                                TextSourceSyntaxStyle.none)
        push_source_syntax_span(document, start_byte, end_byte, syntax_style)
        covered_until = end_byte
    }
    push_source_syntax_span(document, covered_until, source_length,
                            TextSourceSyntaxStyle.none)
    // The fallback fills gaps in INCOMPLETE snippets (compact hunks); on a
    // big full document tree-sitter is authoritative and this scan cost
    // a measured 2.4 s of a 711 KB file's 2.7 s total.
    if (canonical == "daslang" && source_length <= 128 * 1024) {
        var fallback <- das_lexical_fallback(source)
        apply_syntax_fallback(document, fallback)
        delete fallback
    }
    delete tokens
    if (wants_folds) {
        text_language_profile(canonical) $(profile) {
            push_profile_folds(document, profile, structure_spans)
        }
    }
    delete structure_spans
    return <- document
}
