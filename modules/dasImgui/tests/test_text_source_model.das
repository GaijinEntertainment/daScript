options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_source_model public
require strings

def private marker_text(document : TextSourceDocument const) : string {
    var result = ""
    for (span in document.spans) {
        if (!span.styles.marker) continue
        if (!empty(result)) {
            result = result + "|"
        }
        result = result + slice(document.source,
            span.source.start_byte, span.source.end_byte)
    }
    return result
}

[test]
def test_plain_source_document(t : T?) {
    let source = "plain 👩‍💻 text"
    var document <- plain_source_document(source, 11)
    t |> equal(document.source, source, "plain source retains exact UTF-8 text")
    t |> equal(document.revision, 11, "caller document revision is retained")
    t |> equal(length(document.spans), 1, "non-empty source has one span")
    t |> equal(document.spans[0].source.start_byte, 0,
               "plain span begins at the first byte")
    t |> equal(document.spans[0].source.end_byte, length(source),
               "plain span covers every source byte")
    t |> success(!document.spans[0].styles.marker && !document.spans[0].styles.bold,
                 "plain span has no Markdown styles")
    t |> equal(document.spans[0].syntax_style, TextSourceSyntaxStyle.none,
               "plain span has no syntax style")
    document.line_numbers |> push(41)
    text_source_document_dispose(document)
    t |> equal(length(document.line_numbers), 0,
               "disposing a source document releases optional line numbers")

    var empty_document <- plain_source_document("", 12)
    t |> equal(empty_document.source, "", "empty source is retained")
    t |> equal(empty_document.revision, 12, "empty document retains its revision")
    t |> equal(length(empty_document.spans), 0, "empty source has no spans")
    text_source_document_dispose(empty_document)
}

[test]
def test_markdown_source_markers_preserve_original_bytes(t : T?) {
    let source = "# Heading\n**bold** and `code` [link](target) 👩‍💻\n- item\n"
    var document <- markdown_source_document(source, 7)
    t |> equal(document.source, source, "source model retains exact UTF-8 text")
    t |> equal(document.revision, 7, "caller document revision is retained")
    let markers = marker_text(document)
    t |> success(find(markers, "#|**|**|`|`|[|]|(") == 0,
                 "Markdown delimiter spans are recognized in source order")
    t |> success(ends_with(markers, ")|-"),
                 "link close and list marker complete the delimiter sequence")
    for (i in range(length(document.spans))) {
        let span = document.spans[i]
        var expected_start = 0
        if (i > 0) {
            expected_start = document.spans[i - 1].source.end_byte
        }
        t |> equal(span.source.start_byte, expected_start,
                   "source style spans are contiguous")
        t |> equal(span.styles.marker, span.styles.bold,
                   "every Markdown marker requests bold source weight")
    }
    t |> equal(document.spans[length(document.spans) - 1].source.end_byte,
               length(source), "style spans cover every source byte")
    text_source_document_dispose(document)
}

[test]
def test_markdown_source_escaped_marker_stays_regular(t : T?) {
    let source = "escaped \\* regular; ~~strike~~; > quote"
    var document <- markdown_source_document(source)
    let markers = marker_text(document)
    t |> equal(markers, "\\|~~|~~",
               "escape and paired strike delimiters are markers without styling escaped '*'")
    text_source_document_dispose(document)
}
