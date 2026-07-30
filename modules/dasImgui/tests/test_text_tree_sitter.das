options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_tree_sitter public
require strings

def private style_at(document : TextSourceDocument const;
                     byte_offset : int) : TextSourceSyntaxStyle {
    for (span in document.spans) {
        if (byte_offset >= span.source.start_byte
                && byte_offset < span.source.end_byte) {
            return span.syntax_style
        }
    }
    return TextSourceSyntaxStyle.none
}

[test]
def test_das_fragment_recovers_lexical_syntax_after_parse_error(t : T?) {
    let source = "if (broken \{\n    value()\n\ndef public recovered(a : bool) : bool \{\n    var flag = true\n    return flag\n\}\n// def public comment_only\nlet text = \"def public string_only\"\n"
    var document <- tree_sitter_source_document(source, ".das", 17)

    t |> equal(style_at(document, find(source, "def public recovered")),
        TextSourceSyntaxStyle.keyword,
        "a declaration after an invalid fragment retains keyword color")
    t |> equal(style_at(document, find(source, "public recovered")),
        TextSourceSyntaxStyle.keyword,
        "declaration modifiers retain keyword color")
    t |> equal(style_at(document, find(source, "recovered")),
        TextSourceSyntaxStyle.function_,
        "the recovered declaration name retains function color")
    t |> equal(style_at(document, find(source, "bool")),
        TextSourceSyntaxStyle.type_,
        "built-in types retain type color")
    t |> equal(style_at(document, find(source, "var flag")),
        TextSourceSyntaxStyle.keyword,
        "local declaration keywords retain keyword color")

    let comment_def = find(source, "def public comment_only")
    t |> equal(style_at(document, comment_def),
        TextSourceSyntaxStyle.comment,
        "keywords inside comments remain comments")
    let string_def = find(source, "def public string_only")
    t |> equal(style_at(document, string_def),
        TextSourceSyntaxStyle.string_,
        "keywords inside strings remain strings")

    var covered_until = 0
    for (span in document.spans) {
        t |> equal(span.source.start_byte, covered_until,
            "fallback spans remain contiguous")
        covered_until = span.source.end_byte
    }
    t |> equal(covered_until, length(source),
        "fallback spans cover every source byte")
    text_source_document_dispose(document)
}

[test]
def test_json_lexical_syntax_handles_diff_fragments(t : T?) {
    let source = "\{\n  \"name\": \"sheep \\\"one\\\"\",\n  \"count\": -12.5e+2,\n  \"active\": true,\n  \"missing\": null,\n  \"partial\": \"unfinished"
    var document <- tree_sitter_source_document(source, "json", 23)

    t |> equal(style_at(document, find(source, "\"name\"")),
        TextSourceSyntaxStyle.property, "object keys are properties")
    t |> equal(style_at(document, find(source, "\"sheep")),
        TextSourceSyntaxStyle.string_, "quoted values are strings")
    t |> equal(style_at(document, find(source, "-12.5")),
        TextSourceSyntaxStyle.number, "JSON numbers retain number color")
    t |> equal(style_at(document, find(source, "true")),
        TextSourceSyntaxStyle.number, "JSON booleans retain literal color")
    t |> equal(style_at(document, find(source, "null")),
        TextSourceSyntaxStyle.constant, "JSON null is a constant")
    t |> equal(style_at(document, find(source, "\"unfinished")),
        TextSourceSyntaxStyle.string_,
        "an incomplete hunk still colors an unterminated string")

    var covered_until = 0
    for (span in document.spans) {
        t |> equal(span.source.start_byte, covered_until,
            "JSON spans remain contiguous")
        covered_until = span.source.end_byte
    }
    t |> equal(covered_until, length(source),
        "JSON spans cover every exact source byte")
    text_source_document_dispose(document)
}
