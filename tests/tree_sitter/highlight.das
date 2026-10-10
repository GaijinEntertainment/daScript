options gen2
options indenting = 4
options no_aot   // tests/tree_sitter has no AOT registration (module grammar is per-box) — runs interpreted in test_aot

require dastest/testing_boost public
require tree_sitter/tree_sitter
require strings

def private has_role_at(tokens : array<SyntaxToken> const;
                        at : int; role : SyntaxRole) : bool {
    for (token in tokens) {
        if (token.start_byte <= at && at < token.end_byte && token.role == role) {
            return true
        }
    }
    return false
}

def private ranges_are_ordered(tokens : array<SyntaxToken> const) : bool {
    var previous_end = 0
    for (token in tokens) {
        if (token.start_byte < previous_end || token.end_byte <= token.start_byte) return false
        previous_end = token.end_byte
    }
    return true
}

[test]
def test_tree_sitter_language_registry(t : T?) {
    t |> success(syntax_language_available("das"), "daslang alias is available")
    t |> success(syntax_language_available("c"), "C is available")
    t |> success(syntax_language_available("c++"), "C++ alias is available")
    t |> success(syntax_language_available(".hpp"), "C++ extension alias is available")
    t |> success(syntax_language_available("markdown"), "Markdown is available")
    t |> success(!syntax_language_available("glsl"), "unregistered language is explicit")
}

[test]
def test_tree_sitter_daslang_highlights_utf8_and_incomplete_source(t : T?) {
    let lb = to_char(123)
    let source = "// привет\ndef greet(name : string) {lb}\n    let value = \"мир\"\n    return name\n"
    var tokens : array<SyntaxToken>
    t |> equal(syntax_highlight("das", source, tokens), SyntaxHighlightStatus.ok,
               "incomplete daslang still highlights")
    t |> success(ranges_are_ordered(tokens), "tokens are flattened and ordered")
    t |> success(has_role_at(tokens, find(source, "greet"), SyntaxRole.function_),
                 "function declaration is semantic")
    t |> success(has_role_at(tokens, find(source, "привет"), SyntaxRole.comment),
                 "UTF-8 comment range uses source bytes")
    t |> success(has_role_at(tokens, find(source, "мир"), SyntaxRole.string_),
                 "UTF-8 string range uses source bytes")
}

[test]
def test_tree_sitter_daslang_fields_and_interpolations(t : T?) {
    let lb = to_char(123)
    let rb = to_char(125)
    let source = "def show(p : Point) {lb}\n    let w = p.width\n    return \"{lb}describe(p.x){rb}\"\n{rb}\n"
    var tokens : array<SyntaxToken>
    t |> equal(syntax_highlight("das", source, tokens), SyntaxHighlightStatus.ok)
    t |> success(has_role_at(tokens, find(source, "width"), SyntaxRole.property), "a field name is a property")
    t |> success(has_role_at(tokens, find(source, "describe"), SyntaxRole.function_),
                 "a call inside an interpolation keeps its own role, not the string's")
    t |> success(has_role_at(tokens, find(source, "x)"), SyntaxRole.property),
                 "a field name inside an interpolation is a property")
}

[test]
def test_tree_sitter_c_predicates_and_functions(t : T?) {
    let lb = to_char(123)
    let rb = to_char(125)
    let source = "#define ANSWER 42\nint main(void) {lb} return ANSWER; {rb}\n"
    var tokens : array<SyntaxToken>
    t |> equal(syntax_highlight("c", source, tokens), SyntaxHighlightStatus.ok)
    t |> success(has_role_at(tokens, find(source, "main"), SyntaxRole.function_),
                 "C function overrides generic identifier")
    t |> success(has_role_at(tokens, find(source, "ANSWER"), SyntaxRole.constant),
                 "C #match predicate recognizes screaming-snake constants")
    t |> success(ranges_are_ordered(tokens), "C captures do not overlap")
}

[test]
def test_tree_sitter_cpp_composes_c_and_cpp_queries(t : T?) {
    let lb = to_char(123)
    let rb = to_char(125)
    let source = "class Widget {lb} public: int value() const {lb} return 42; {rb} {rb};\n"
    var tokens : array<SyntaxToken>
    t |> equal(syntax_highlight("hpp", source, tokens), SyntaxHighlightStatus.ok)
    t |> success(has_role_at(tokens, find(source, "class"), SyntaxRole.keyword),
                 "C++-only keyword is highlighted")
    t |> success(has_role_at(tokens, find(source, "value"), SyntaxRole.function_),
                 "C base query still supplies method highlighting")
    t |> success(has_role_at(tokens, find(source, "return"), SyntaxRole.keyword),
                 "C base keywords remain active in C++")
}

[test]
def test_tree_sitter_markdown_combines_block_and_inline_grammars(t : T?) {
    let source = "# **Hello**\n\n[site](https://example.com)\n\n```cpp\nint x;\n```\n"
    var tokens : array<SyntaxToken>
    t |> equal(syntax_highlight("md", source, tokens), SyntaxHighlightStatus.ok)
    t |> success(has_role_at(tokens, find(source, "Hello"), SyntaxRole.strong),
                 "Markdown inline grammar highlights strong text")
    t |> success(has_role_at(tokens, find(source, "https://"), SyntaxRole.uri),
                 "Markdown inline grammar highlights link destinations")
    t |> success(!has_role_at(tokens, find(source, "int x"), SyntaxRole.literal),
                 "code-fence content is left for its injected language")
    t |> success(ranges_are_ordered(tokens), "block and inline captures are flattened")
}

[test]
def test_tree_sitter_unknown_language_is_non_destructive(t : T?) {
    var tokens = [SyntaxToken(start_byte = 1, end_byte = 2)]
    t |> equal(syntax_highlight("not-a-language", "text", tokens),
               SyntaxHighlightStatus.unknown_language)
    t |> equal(length(tokens), 0, "output is cleared before every request")
}
