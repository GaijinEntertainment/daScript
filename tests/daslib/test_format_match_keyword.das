options gen2
require dastest/testing_boost public
require daslib/das_source_formatter
require daslib/fio
require strings

def fixture(name : string) : string => replace(fread("{get_das_root()}/tests/daslib/{name}"), "\r", "")

[test]
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
