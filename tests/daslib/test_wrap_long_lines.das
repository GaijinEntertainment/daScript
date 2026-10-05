options gen2
require dastest/testing_boost public
require daslib/das_source_formatter
require daslib/fio
require daslib/ast
require daslib/rtti
require strings

def fixture(name : string) : string => replace(fread("{get_das_root()}/tests/daslib/{name}"), "\r", "")

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

[test]
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
