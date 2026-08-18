options gen2

require dastest/testing_boost public
require strings
require daslib/fio
require dastest/review_gate

[test]
def test_findings_accumulate_and_reset(t : T?) {
    gate_reset()
    gate_finding("a.das", "message one")
    gate_finding("b.das", 7, "line-anchored")
    gate_finding("c.das", 0, "zero line means no anchor")
    var inscope fs <- gate_findings()
    t |> equal(3, length(fs))
    t |> equal("a.das: message one", fs[0])
    t |> equal("b.das:7: line-anchored", fs[1])
    t |> equal("c.das: zero line means no anchor", fs[2])
    t |> equal(1, gate_verdict("fixture"))
    gate_reset()
    var inscope none <- gate_findings()
    t |> equal(0, length(none))
    t |> equal(0, gate_verdict("fixture"))
}

[test]
def test_kebab_case(t : T?) {
    t |> success(is_kebab_case("like-this"))
    t |> success(is_kebab_case("aot"))
    t |> success(is_kebab_case("gen1-to-gen2"))
    t |> success(!is_kebab_case("like_that"))
    t |> success(!is_kebab_case("likeThis"))
    t |> success(!is_kebab_case("-leading"))
    t |> success(!is_kebab_case("trailing-"))
    t |> success(!is_kebab_case("double--dash"))
    t |> success(!is_kebab_case(""))
}

[test]
def test_find_line(t : T?) {
    let text = "one\ntwo\nthree\n"
    t |> equal(1, find_line(text, "one"))
    t |> equal(2, find_line(text, "two"))
    t |> equal(3, find_line(text, "three"))
    t |> equal(2, find_line(text, "two\nthree"))
    t |> equal(0, find_line(text, "absent"))
    t |> equal(0, find_line("", "x"))
}

[test]
def test_das_requires(t : T?) {
    let fixture_r = create_temp_file_result("review_gate_req", ".das")
    t |> success(fixture_r is value, "temp fixture file created")
    if (!(fixture_r is value)) {
        return
    }
    let fixture = unsafe(fixture_r.value)
    let src = "options gen2\n\nrequire strings\nrequire daslib/fio public\nrequire utils_common // trailing comment\n// require commented_out\n    require indented/mod\nrequire tight//comment with no space\nrequire //nothing before the comment\n"
    t |> success(fwrite(fixture, src))
    var inscope reqs <- das_requires(fixture)
    t |> equal(5, length(reqs))
    t |> equal("strings", reqs[0])
    t |> equal("daslib/fio", reqs[1])
    t |> equal("utils_common", reqs[2])
    t |> equal("indented/mod", reqs[3])
    t |> equal("tight", reqs[4], "comment cut runs before the space split")
    t |> success(remove(fixture))
}

[test]
def test_cmake_command_blocks(t : T?) {
    let text = "# install( inside a comment stays closed\nset(FOO bar)\ninstall(FILES a.das\n    DESTINATION utils/aot) # trailing\nmyinstall(x)\ninstall(PROGRAMS \"weird)name.exe\" DESTINATION bin)\n"
    var inscope blocks <- cmake_command_blocks(text, "install")
    t |> equal(2, length(blocks))
    t |> success(find(blocks[0], "DESTINATION utils/aot") >= 0, "first block spans lines")
    t |> success(find(blocks[1], "weird)name.exe") >= 0, "paren inside quotes does not close the block")
    var inscope sets <- cmake_command_blocks(text, "set")
    t |> equal(1, length(sets))
    t |> equal("set(FOO bar)", sets[0])
}

[test]
def test_cmake_command_blocks_edges(t : T?) {
    // escaped quote inside a string, a paren inside an in-block comment,
    // whitespace before the paren, and an unterminated block at EOF
    let text = "install(FILES \"a\\\")b.das\" DESTINATION x)\ninstall(FILES a.das # comment with ) paren\n    DESTINATION y)\ninstall (PROGRAMS z)\ninstall(FILES never_closed"
    var inscope blocks <- cmake_command_blocks(text, "install")
    t |> equal(3, length(blocks), "the unterminated block yields nothing")
    t |> success(find(blocks[0], "DESTINATION x") >= 0, "escaped quote does not end the string")
    t |> success(find(blocks[1], "DESTINATION y") >= 0, "paren in an in-block comment does not close it")
    t |> success(find(blocks[2], "PROGRAMS z") >= 0, "whitespace before the paren is accepted")
}

[test]
def test_cmake_list_entries(t : T?) {
    let text = "set(DAS_UTILS\n    aot\n    dascov  # a comment\n    internal/jit\n)\nif(NOT X)\n    list(APPEND DAS_UTILS benchctl)   # same gate\nendif()\nset(OTHER_LIST nope)\n"
    var inscope entries <- cmake_list_entries(text, "DAS_UTILS")
    t |> equal(4, length(entries))
    t |> equal("aot", entries[0])
    t |> equal("dascov", entries[1])
    t |> equal("internal/jit", entries[2])
    t |> equal("benchctl", entries[3])
    var inscope other <- cmake_list_entries(text, "OTHER_LIST")
    t |> equal(1, length(other))
    t |> equal("nope", other[0])
    var inscope missing <- cmake_list_entries(text, "NOT_THERE")
    t |> equal(0, length(missing))

    var inscope bare <- cmake_list_entries("set()\nset(EMPTY_LIST)\n", "EMPTY_LIST")
    t |> equal(0, length(bare), "a set with no entries neither crashes nor yields")
}
