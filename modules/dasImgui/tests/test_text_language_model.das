options gen2
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_language
require imgui/imgui_text_tree_sitter

//! Pure-logic coverage for the language profile registry: no UI, no parser.

[test]
def test_language_lookup_canonicalizes(t : T?) {
    text_language_profile(".hpp") $(profile) {
        t |> equal(profile.name, "cpp", "extensions canonicalize to the language")
    }
    text_language_profile("das") $(profile) {
        t |> equal(profile.name, "daslang", "aliases canonicalize too")
    }
    text_language_profile("xyz") $(profile) {
        t |> equal(profile.name, "plain", "unknown languages fall back to plain")
    }
    // hosts pass `extension()` straight in and it keeps the leading dot
    text_language_profile(".json") $(profile) {
        t |> equal(profile.name, "json", "dotted extensions reach the json profile")
    }
    t |> equal(syntax_canonical_language(".json"), "json",
               "the canonicalizer strips the leading dot")
    t |> success(text_language_known("cpp"), "cpp has a first-class profile")
    t |> success(text_language_known(".md"), "known-check canonicalizes extensions")
    t |> success(text_language_known("json") && text_language_known(".json"),
                 "json is known by name and by extension")
    t |> success(!text_language_known("xyz"), "unknown language is not known")
}

[test]
def test_language_fold_kinds_migrated(t : T?) {
    // the fold tables moved here verbatim from imgui_text_tree_sitter
    text_language_profile("daslang") $(profile) {
        t |> success(key_exists(profile.fold_kinds, "structure_declaration"),
            "das folds structures")
        t |> success(key_exists(profile.fold_kinds, "block_comment"),
            "das folds block comments")
    }
    text_language_profile("c") $(profile) {
        t |> success(key_exists(profile.fold_kinds, "function_definition"),
            "c folds functions")
        t |> success(!key_exists(profile.fold_kinds, "class_specifier"),
            "c does not fold classes")
    }
    text_language_profile("cpp") $(profile) {
        t |> success(key_exists(profile.fold_kinds, "class_specifier"),
            "cpp folds classes")
        t |> success(key_exists(profile.fold_kinds, "namespace_definition"),
            "cpp folds namespaces")
    }
    text_language_profile("markdown") $(profile) {
        t |> success(key_exists(profile.fold_kinds, "fenced_code_block"),
            "markdown folds code fences")
    }
    text_language_profile("plain") $(profile) {
        t |> success(empty(profile.fold_kinds), "plain folds nothing")
    }
}

[test]
def test_language_brackets_and_indent(t : T?) {
    text_language_profile("daslang") $(profile) {
        t |> equal(length(profile.bracket_pairs), 3, "das pairs parens braces brackets")
        t |> equal(length(profile.indent_pairs), 1, "only braces drive das indent")
        t |> equal(profile.indent_pairs[0], int2('{', '}'), "the indent pair is the brace")
        t |> equal(profile.line_comment, "//", "das line comment")
        t |> equal(profile.block_comment_close, "*/", "das block comment close")
    }
    text_language_profile("markdown") $(profile) {
        t |> success(empty(profile.indent_pairs), "markdown never auto-indents")
        t |> equal(length(profile.bracket_pairs), 3, "markdown still pairs brackets")
        t |> success(empty(profile.line_comment), "markdown has no line comment")
    }
    text_language_profile(".json") $(profile) {
        t |> equal(length(profile.indent_pairs), 2,
            "json indents inside braces and brackets, path-derived name included")
        t |> equal(profile.string_delimiters, "\"", "json strings are double-quoted only")
    }
}

[test]
def test_language_keywords_and_completion(t : T?) {
    text_language_profile("daslang") $(profile) {
        t |> success(find_index(profile.keywords, "def") >= 0, "das knows def")
        t |> success(find_index(profile.keywords, "typeinfo") >= 0, "das knows typeinfo")
        t |> success(profile.ghost_completion, "das ghost completion defaults on")
    }
    text_language_profile("cpp") $(profile) {
        t |> success(find_index(profile.keywords, "constexpr") >= 0, "cpp knows constexpr")
    }
    text_language_profile("markdown") $(profile) {
        t |> success(empty(profile.keywords), "markdown offers no keywords")
        t |> success(profile.ghost_completion, "markdown ghost stays on (doc words)")
    }
    text_language_profile("plain") $(profile) {
        t |> success(!profile.ghost_completion, "plain text has ghost off")
    }
}

[test]
def test_language_keywords_are_the_lexer_keywords(t : T?) {
    // the profile owns the keyword set; the fallback lexer only styles it
    var unstyled = 0
    text_language_profile("daslang") $(profile) {
        for (word in profile.keywords) {
            if (text_source_das_keyword_style(word) == TextSourceSyntaxStyle.none) {
                unstyled++
            }
        }
        t |> success(find_index(profile.keywords, "inscope") >= 0,
                     "keywords the lexer knew are offered for completion too")
        t |> success(find_index(profile.keywords, "uninitialized") >= 0,
                     "and the longest of them survived the merge")
        t |> success(find_index(profile.keywords, "panic") >= 0,
                     "keywords only completion knew are styled now")
    }
    t |> equal(unstyled, 0, "every profile keyword has a fallback lexer style")
    t |> equal(text_source_das_keyword_style("int"), TextSourceSyntaxStyle.type_,
               "type names style as types")
    t |> equal(text_source_das_keyword_style("float4"), TextSourceSyntaxStyle.type_,
               "vector type names style as types too")
    t |> equal(text_source_das_keyword_style("true"), TextSourceSyntaxStyle.number,
               "boolean literals style as numbers")
    t |> equal(text_source_das_keyword_style("null"), TextSourceSyntaxStyle.constant,
               "null styles as a constant")
    t |> equal(text_source_das_keyword_style("def"), TextSourceSyntaxStyle.keyword,
               "everything else styles as a keyword")
    t |> equal(text_source_das_keyword_style("not_a_keyword"),
               TextSourceSyntaxStyle.none, "a plain identifier has no keyword style")
}
