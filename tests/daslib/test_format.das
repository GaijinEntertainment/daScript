options gen2

require dastest/testing_boost public
require daslib/das_source_formatter
require daslib/fio
require strings
require daslib/ast
require daslib/rtti

def fixture(name : string) : string => replace(fread("{get_das_root()}/tests/daslib/{name}"), "\r", "")

[test(description = "the formatter keeps the space after match, and restores it where it was glued")]
def test_match_keyword_keeps_its_space(t : T?) {
    let formatted = fixture("_fixture_match_keyword_space.das")
    t |> run("the fixture is already formatted") @(t : T?) {
        t |> equal(format_source_string(formatted), formatted)
    }
    t |> run("a glued match keyword with arms gets its space back, in value position too") @(t : T?) {
        let glued = replace(formatted, "match (", "match(")
        t |> success(glued != formatted)
        t |> equal(format_source_string(glued), formatted)
    }
}

[test(description = "the formatter leaves a pointer return type of a parameterless function unspaced")]
def test_the_formatter_keeps_a_pointer_return_type_of_a_parameterless_function(t : T?) {
    let source = ("options gen2\n\ndef f() : int? \{\n    return null\n\}\n\n"
        + "def g : int? \{\n    return null\n\}\n")
    let formatted = format_source_string(source)
    t |> success(find(formatted, "def f() : int? \{") >= 0, "with parentheses: {formatted}")
    t |> success(find(formatted, "def g : int? \{") >= 0, "without parentheses: {formatted}")
    t |> success(find(formatted, "int ?") < 0, "no space before the ?: {formatted}")
}

[test(description = "the formatter leaves a pointer return type unspaced after a visibility keyword")]
def test_the_formatter_keeps_a_pointer_return_type_after_a_visibility_keyword(t : T?) {
    let source = "options gen2\n\ndef private h : int? \{\n    return null\n\}\n"
    let formatted = format_source_string(source)
    t |> success(find(formatted, "def private h : int? \{") >= 0, "{formatted}")
}

[test(description = "the formatter spaces a ternary in an expression-bodied function")]
def test_the_formatter_spaces_a_ternary_in_an_expression_body(t : T?) {
    let source = "options gen2\n\ndef k : int => g_flag?1:2\n"
    let formatted = format_source_string(source)
    t |> success(find(formatted, "def k : int => g_flag ? 1 : 2") >= 0, "{formatted}")
}

[test(description = "the formatter keeps the raw assignment operators as single tokens")]
def test_raw_assignment_tokens_survive_formatting(t : T?) {
    t |> run("the formatter keeps !== !<- and !:= as one token each") @(t : T?) {
        let source = "options gen2\ndef raw(var a : int; b : int) \{\n    a !== b\n    a !<- b\n    a !:= b\n\}\n"
        let formatted = format_source_string(source)
        t |> success(find(formatted, "a !== b") >= 0, "raw copy kept: {formatted}")
        t |> success(find(formatted, "a !<- b") >= 0, "raw move kept: {formatted}")
        t |> success(find(formatted, "a !:= b") >= 0, "raw clone kept: {formatted}")
        t |> success(find(formatted, "! ") < 0, "no raw token split after the bang: {formatted}")
    }
}

def tokens_only(text : string) : string =>
    replace(replace(replace(replace(text, " ", ""), "\t", ""), "\r", ""), "\n", "")

def compiles(t : T?; text : string) : bool {
    var ok = false
    var inscope access <- make_file_access("")
    let path = "{get_das_root()}/tests/daslib/_wrapped_in_memory.das"
    set_file_source(access, path, text)
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            compile_file(path, access, unsafe(addr(mg)), cop) $(compiled; _program; issues) {
                ok = compiled
                if (!compiled) {
                    t |> failure("wrapped source does not compile: {issues}")
                }
            }
        }
    }
    return ok
}

[test(description = "wrap_long_lines breaks long lines as the after fixture shows, changes only whitespace, stays compilable and idempotent")]
def test_wrap_long_lines(t : T?) {
    let before = fixture("_fixture_wrap_long_lines_before.das")
    let after = fixture("_fixture_wrap_long_lines_after.das")
    let wrapped = wrap_long_lines(before, 100)
    t |> run("each rule in the fixture breaks its line exactly as the after fixture shows") @(t : T?) {
        t |> equal(wrapped, after)
    }
    t |> run("only line breaks and indentation change") @(t : T?) {
        t |> equal(tokens_only(wrapped), tokens_only(before))
    }
    t |> run("the wrapped source compiles") @(t : T?) {
        t |> success(compiles(t, wrapped))
    }
    t |> run("wrapping wrapped text changes nothing") @(t : T?) {
        t |> equal(wrap_long_lines(wrapped, 100), wrapped)
    }
    t |> run("a limit of 0 keeps the source") @(t : T?) {
        t |> equal(wrap_long_lines(before, 0), before)
    }
    t |> run("CRLF sources keep CRLF on the new lines") @(t : T?) {
        t |> equal(wrap_long_lines(replace(before, "\n", "\r\n"), 100), replace(after, "\n", "\r\n"))
    }
    t |> run("format_source with a limit formats, then wraps the same way") @(t : T?) {
        var bytes <- [for (c in before); uint8(c)]
        t |> equal(format_source(bytes, false, 100), after)
        delete bytes
    }
}
