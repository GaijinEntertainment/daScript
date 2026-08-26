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
def test_cmake_words(t : T?) {
    let text = "add_test(NAME a # comment ) here\n    COMMAND \"c:/p a t h/x.exe\")\n"
    var inscope words <- cmake_words(text)
    t |> equal(7, length(words))
    t |> equal("add_test", words[0])
    t |> equal("(", words[1], "parens are words of their own")
    t |> equal("NAME", words[2])
    t |> equal("a", words[3])
    t |> equal("COMMAND", words[4], "the comment runs to end of line only")
    t |> equal("c:/p a t h/x.exe", words[5], "a quoted run is one word, quotes dropped")
    t |> equal(")", words[6])

    var inscope args <- cmake_args(text)
    t |> equal(4, length(args), "cmake_args drops the command name and the parens")
    t |> equal("NAME", args[0])
    t |> equal("c:/p a t h/x.exe", args[3])

    var inscope trailing <- cmake_words("set(A \"unterminated\\")
    t |> equal(4, length(trailing), "an escape at EOF terminates the scan")
    t |> equal("unterminated\\", trailing[3], "the unterminated run ends at EOF, not past it")

    var inscope escaped <- cmake_words("\"esc\\\"aped\" x")
    t |> equal(2, length(escaped))
    t |> equal("esc\\\"aped", escaped[0], "an escaped quote does not end the run")

    var inscope eof_comment <- cmake_words("# trailing comment, no newline")
    t |> equal(0, length(eof_comment))

    var inscope tight <- cmake_words("a\"b\" c")
    t |> equal(3, length(tight), "a quote breaks a word even with no space before it")
    t |> equal("a", tight[0])
    var inscope hashed <- cmake_words("a#b\nd")
    t |> equal(2, length(hashed), "a # breaks a word and opens a comment")
    t |> equal("a", hashed[0])
    t |> equal("d", hashed[1])
}

[test]
def test_cmake_test_labels(t : T?) {
    let text = "add_test(NAME one COMMAND x)\nset_tests_properties(one PROPERTIES\n    LABELS \"big;style_lint\"\n    PASS_REGULAR_EXPRESSION \"STYLE014\")\ndoctest_discover_tests(exe PROPERTIES LABELS \"small\")\nset_tests_properties(two PROPERTIES LABELS big)\n"
    var inscope labels <- cmake_test_labels(text)
    t |> equal(3, length(labels))
    t |> equal("big", labels[0])
    t |> equal("style_lint", labels[1], "a semicolon list yields every label")
    t |> equal("small", labels[2], "any command's LABELS property counts")

    var inscope none <- cmake_test_labels("add_test(NAME x COMMAND y)\n# LABELS \"commented\"\n")
    t |> equal(0, length(none))

    var inscope dangling <- cmake_test_labels("set_tests_properties(x PROPERTIES LABELS")
    t |> equal(0, length(dangling), "a LABELS with no value reads past nothing")
    var inscope trailing_semi <- cmake_test_labels("set_tests_properties(x PROPERTIES LABELS \"big;\")")
    t |> equal(1, length(trailing_semi), "a trailing semicolon yields no empty label")
    t |> equal("big", trailing_semi[0])

    // an unquoted list variable expands to one argument per label - the shape the vendored
    // doctestAddTests.cmake writes as `LABELS ${labels}`
    var inscope expanded <- cmake_test_labels(
        "set_tests_properties(x PROPERTIES LABELS big style_lint PASS_REGULAR_EXPRESSION \"STYLE014\")\nadd_dependencies(test-big y)\n")
    t |> equal(2, length(expanded), "every label of an expanded list is read")
    t |> equal("big", expanded[0])
    t |> equal("style_lint", expanded[1])

    var inscope bounded <- cmake_test_labels("set_tests_properties(x PROPERTIES LABELS big)\nadd_dependencies(test-big y)\n")
    t |> equal(1, length(bounded), "the run stops at the end of its own command")
    t |> equal("big", bounded[0])
}

[test]
def test_cmake_command_targets(t : T?) {
    let text = "add_dependencies(test-big test_a test_b)\n# add_dependencies(test-small ghost)\nadd_dependencies(\n    test-small test_c)\nadd_executable(other x.cpp)\n"
    var inscope deps <- cmake_command_targets(text, "add_dependencies")
    t |> equal(2, length(deps))
    t |> equal("test-big", deps[0])
    t |> equal("test-small", deps[1], "the target may sit on the next line")
    var inscope exes <- cmake_command_targets(text, "add_executable")
    t |> equal(1, length(exes))
    t |> equal("other", exes[0])

    var inscope argless <- cmake_command_targets("add_dependencies()\nadd_dependencies(x y)", "add_dependencies")
    t |> equal(1, length(argless), "an empty block names no target and does not index past it")
    t |> equal("x", argless[0])
    var inscope absent <- cmake_command_targets(text, "add_library")
    t |> equal(0, length(absent))
}

[test]
def test_cmake_commands_are_case_insensitive(t : T?) {
    // CMake itself accepts any casing; a gate that only reads one spelling reports
    // a wiring defect that is not there, or misses one that is
    let text = "ADD_DEPENDENCIES(test-big test_a)\nAdd_Test(NAME m COMMAND test_a)\nset_tests_properties(m PROPERTIES LABELS \"big\")\n"
    var inscope deps <- cmake_command_targets(text, "add_dependencies")
    t |> equal(1, length(deps))
    t |> equal("test-big", deps[0], "an all-caps command opens a block")
    var inscope cmds <- cmake_test_commands(text)
    t |> equal(1, length(cmds))
    t |> equal("test_a", cmds[0], "mixed case opens a block too")
    var inscope labels <- cmake_test_labels(text)
    t |> equal(1, length(labels))
    t |> equal("big", labels[0])
}

[test]
def test_cmake_test_commands(t : T?) {
    let text = "add_test(NAME a COMMAND test_a WORKING_DIRECTORY x)\nadd_test(\n    NAME b\n    COMMAND $<TARGET_FILE:daslang> script.das)\nadd_test(NAME c)\n"
    var inscope cmds <- cmake_test_commands(text)
    t |> equal(2, length(cmds), "a block with no COMMAND contributes nothing")
    t |> equal("test_a", cmds[0])
    t |> equal("$<TARGET_FILE:daslang>", cmds[1], "a generator expression comes back verbatim")
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

    var inscope shouty <- cmake_list_entries("SET(CAPS a)\nLIST(APPEND CAPS b)\n", "CAPS")
    t |> equal(2, length(shouty), "an uppercase command is read once, not twice")
}
