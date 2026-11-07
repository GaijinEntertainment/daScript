options gen2
require dastest/testing_boost public
require daslib/das_source_formatter
require strings

[test]
def test_the_formatter_keeps_a_pointer_return_type_of_a_parameterless_function(t : T?) {
    let source = ("options gen2\n\ndef f() : int? \{\n    return null\n\}\n\n"
        + "def g : int? \{\n    return null\n\}\n")
    let formatted = format_source_string(source)
    t |> success(find(formatted, "def f() : int? \{") >= 0, "with parentheses: {formatted}")
    t |> success(find(formatted, "def g : int? \{") >= 0, "without parentheses: {formatted}")
    t |> success(find(formatted, "int ?") < 0, "no space before the ?: {formatted}")
}

[test]
def test_the_formatter_keeps_a_pointer_return_type_after_a_visibility_keyword(t : T?) {
    let source = "options gen2\n\ndef private h : int? \{\n    return null\n\}\n"
    let formatted = format_source_string(source)
    t |> success(find(formatted, "def private h : int? \{") >= 0, "{formatted}")
}

[test]
def test_the_formatter_spaces_a_ternary_in_an_expression_body(t : T?) {
    let source = "options gen2\n\ndef k : int => g_flag?1:2\n"
    let formatted = format_source_string(source)
    t |> success(find(formatted, "def k : int => g_flag ? 1 : 2") >= 0, "{formatted}")
}
