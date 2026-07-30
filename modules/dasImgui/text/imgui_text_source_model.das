options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_text_source_model shared

require imgui/imgui_text_flow public
require strings

//! View-independent styled source text. The source string is never rewritten:
//! every style span refers to exact UTF-8 bytes, so selection, clipboard, and
//! future edits use the same coordinates regardless of presentation.

bitfield public TextSourceStyles {
    marker
    bold
}

enum public TextSourceSyntaxStyle {
    none
    comment
    string_
    number
    keyword
    type_
    function_
    variable
    property
    constant
    operator_
    punctuation
}

enum public TextSourceLineMarkKind {
    added
    removed
    modified
}

struct public TextSourceLineMark {
    line_index : int
    kind : TextSourceLineMarkKind = TextSourceLineMarkKind.modified
}

struct public TextSourceSpan {
    source : TextSourceRange
    styles : TextSourceStyles
    //! Optional parser-neutral syntax role. Providers such as Tree-sitter
    //! populate this without coupling the source model to a parser runtime.
    syntax_style : TextSourceSyntaxStyle = TextSourceSyntaxStyle.none
}

struct public TextSourceDocument {
    source : string
    spans : array<TextSourceSpan>
    line_marks : array<TextSourceLineMark>
    //! Optional display numbers per unwrapped visual line. Negative entries
    //! reserve an aligned gap without drawing a number (side-by-side diffs).
    line_numbers : array<int>
    revision : int = 0
    marker_count : int = 0
}

def public text_source_document_dispose(var document : TextSourceDocument) {
    delete document.spans
    delete document.line_marks
    delete document.line_numbers
    document = TextSourceDocument()
}

def public plain_source_document(source : string;
                                 revision : int = 0) : TextSourceDocument {
    var document = TextSourceDocument(source = source, revision = revision)
    if (!empty(source)) {
        document.spans |> push(TextSourceSpan(
            source = TextSourceRange(start_byte = 0, end_byte = length(source))))
    }
    return <- document
}

def private ascii_markdown_delimiter(b : int) : bool {
    return (b == '*' || b == '_' || b == '~'
        || b == '`' || b == '[' || b == ']'
        || b == '(' || b == ')' || b == '!')
}

def private ascii_digit_byte(b : int) : bool {
    return b >= '0' && b <= '9' // nolint:PERF014
}

def private push_source_span(var document : TextSourceDocument;
                             start_byte, end_byte : int; marker : bool) {
    if (end_byte <= start_byte) return
    var styles : TextSourceStyles
    styles.marker = marker
    styles.bold = marker
    document.spans |> push(TextSourceSpan(
        source = TextSourceRange(start_byte = start_byte, end_byte = end_byte),
        styles = styles))
    if (marker) {
        document.marker_count++
    }
}

def private push_marker(var document : TextSourceDocument; var covered_until : int&;
                        start_byte, end_byte : int) {
    if (start_byte < covered_until || end_byte <= start_byte) return
    push_source_span(document, covered_until, start_byte, false)
    push_source_span(document, start_byte, end_byte, true)
    covered_until = end_byte
}

def public markdown_source_document(source : string;
                                    revision : int = 0) : TextSourceDocument {
    //! Lightweight Markdown lexical styling for source presentation. Not a second Markdown parser:
    //! it recognizes delimiter tokens while leaving all bytes visible. Structural MD4C parsing
    //! remains the authority for rendered Markdown.
    var document = TextSourceDocument(source = source, revision = revision)
    peek_data(source) $(bytes) {
        let n = length(bytes)
        var at = 0
        var line_start = 0
        var covered_until = 0
        var link_paren_depth = 0
        while (at < n) {
            if (at == line_start) {
                var content = at
                while (content < n && content - at < 3
                        && int(bytes[content]) == ' ') {
                    content++
                }
                if (content < n) {
                    let first = int(bytes[content])
                    var marker_end = content
                    if (first == '#') {
                        while (marker_end < n && marker_end - content < 6
                                && int(bytes[marker_end]) == '#') {
                            marker_end++
                        }
                        if (marker_end < n && int(bytes[marker_end]) == ' ') {
                            push_marker(document, covered_until, content, marker_end)
                            at = marker_end
                            continue
                        }
                    } elif (first == '>') {
                        push_marker(document, covered_until, content, content + 1)
                        at = content + 1
                        continue
                    } elif ((first == '-' || first == '+' || first == '*')
                            && content + 1 < n
                            && (int(bytes[content + 1]) == ' '
                                || int(bytes[content + 1]) == '\t')) {
                        push_marker(document, covered_until, content, content + 1)
                        at = content + 1
                        continue
                    } elif (ascii_digit_byte(first)) {
                        marker_end = content + 1
                        while (marker_end < n && ascii_digit_byte(int(bytes[marker_end]))) {
                            marker_end++
                        }
                        if (marker_end + 1 < n
                                && (int(bytes[marker_end]) == '.'
                                    || int(bytes[marker_end]) == ')')
                                && (int(bytes[marker_end + 1]) == ' '
                                    || int(bytes[marker_end + 1]) == '\t')) {
                            push_marker(document, covered_until, content, marker_end + 1)
                            at = marker_end + 1
                            continue
                        }
                    }
                }
            }

            let b = int(bytes[at])
            if (b == '\r' || b == '\n') {
                if (b == '\r' && at + 1 < n
                        && int(bytes[at + 1]) == '\n') {
                    at += 2
                } else {
                    at++
                }
                line_start = at
                continue
            }
            if (b == '\\' && at + 1 < n
                    && ascii_markdown_delimiter(int(bytes[at + 1]))) {
                push_marker(document, covered_until, at, at + 1)
                at += 2 // escaped byte remains regular
                continue
            }
            if (b == '*' || b == '_' || b == '`' || b == '~') {
                var end_ = at + 1
                while (end_ < n && int(bytes[end_]) == b) {
                    end_++
                }
                if (b != '~' || end_ - at >= 2) {
                    push_marker(document, covered_until, at, end_)
                }
                at = end_
                continue
            }
            if (b == '[' || b == ']' || b == '!') {
                push_marker(document, covered_until, at, at + 1)
            } elif (b == '(' && at > 0 && int(bytes[at - 1]) == ']') {
                push_marker(document, covered_until, at, at + 1)
                link_paren_depth = 1
            } elif (b == '(' && link_paren_depth > 0) {
                link_paren_depth++
                push_marker(document, covered_until, at, at + 1)
            } elif (b == ')' && link_paren_depth > 0) {
                push_marker(document, covered_until, at, at + 1)
                link_paren_depth--
            }
            at++
        }
        push_source_span(document, covered_until, n, false)
    }
    if (empty(document.spans) && empty(source)) {
        push_source_span(document, 0, 0, false)
    }
    return <- document
}
