options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_text_tree_sitter shared

require imgui/imgui_text_source_model public
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

def private das_keyword_style(word : string) : TextSourceSyntaxStyle {
    if (word == "true" || word == "false") return TextSourceSyntaxStyle.number
    if (word == "null") return TextSourceSyntaxStyle.constant
    if (word == "bool" || word == "void" || word == "string"
            || word == "auto" || word == "int" || word == "int8"
            || word == "int16" || word == "int64" || word == "uint"
            || word == "uint8" || word == "uint16" || word == "uint64"
            || word == "float" || word == "double" || word == "range"
            || word == "urange" || word == "block" || word == "function"
            || word == "lambda" || word == "array" || word == "table"
            || word == "tuple" || word == "variant" || word == "iterator"
            || word == "smart_ptr" || word == "typedecl") {
        return TextSourceSyntaxStyle.type_
    }
    if (word == "if" || word == "elif" || word == "else"
            || word == "static_if" || word == "static_elif"
            || word == "for" || word == "while" || word == "break"
            || word == "continue" || word == "def" || word == "return"
            || word == "yield" || word == "try" || word == "recover"
            || word == "struct" || word == "class" || word == "enum"
            || word == "typedef" || word == "bitfield" || word == "let"
            || word == "var" || word == "const" || word == "override"
            || word == "sealed" || word == "abstract" || word == "static"
            || word == "public" || word == "private" || word == "shared"
            || word == "inscope" || word == "implicit" || word == "explicit"
            || word == "module" || word == "require" || word == "include"
            || word == "options" || word == "expect" || word == "assume"
            || word == "with" || word == "where" || word == "label"
            || word == "goto" || word == "pass" || word == "delete"
            || word == "new" || word == "deref" || word == "addr"
            || word == "cast" || word == "upcast" || word == "reinterpret"
            || word == "typeinfo" || word == "type" || word == "unsafe"
            || word == "finally" || word == "in" || word == "is"
            || word == "as" || word == "aka" || word == "operator"
            || word == "template" || word == "default"
            || word == "uninitialized" || word == "generator"
            || word == "capture") {
        return TextSourceSyntaxStyle.keyword
    }
    return TextSourceSyntaxStyle.none
}

def private push_das_fallback(var spans : array<TextSourceSpan>;
                              start_byte, end_byte : int;
                              style : TextSourceSyntaxStyle) {
    if (end_byte <= start_byte || style == TextSourceSyntaxStyle.none) return
    spans |> push(TextSourceSpan(
        source = TextSourceRange(start_byte = start_byte, end_byte = end_byte),
        syntax_style = style))
}

def private das_lexical_fallback(source : string) : array<TextSourceSpan> {
    // Fills query gaps from incomplete snippets / recovery nodes (common in compact Git hunks); tree-sitter stays authoritative.
    var result : array<TextSourceSpan>
    var expect_function = false
    var expect_type = false
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
                let word = slice(source, start, at)
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

def public tree_sitter_source_document(source, language : string;
                                       revision : int = 0) : TextSourceDocument {
    let canonical = syntax_canonical_language(language)
    if (canonical == "json") {
        return <- json_lexical_source_document(source, revision)
    }
    var document = TextSourceDocument(source = source, revision = revision)
    var tokens : array<SyntaxToken>
    if (syntax_highlight(language, source, tokens) != SyntaxHighlightStatus.ok) {
        delete tokens
        return <- plain_source_document(source, revision)
    }

    var covered_until = 0
    for (token in tokens) {
        let syntax_style = source_syntax_style(token.role)
        if (syntax_style == TextSourceSyntaxStyle.none) continue
        let start_byte = clamp(token.start_byte, covered_until, length(source))
        let end_byte = clamp(token.end_byte, start_byte, length(source))
        push_source_syntax_span(document, covered_until, start_byte,
                                TextSourceSyntaxStyle.none)
        push_source_syntax_span(document, start_byte, end_byte, syntax_style)
        covered_until = end_byte
    }
    push_source_syntax_span(document, covered_until, length(source),
                            TextSourceSyntaxStyle.none)
    if (canonical == "daslang") {
        var fallback <- das_lexical_fallback(source)
        apply_syntax_fallback(document, fallback)
        delete fallback
    }
    delete tokens
    return <- document
}
