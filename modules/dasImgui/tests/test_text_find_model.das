options gen2
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_find

//! Pure-logic coverage for the find model: no UI, no live host.

[test]
def test_find_literal_case_insensitive(t : T?) {
    var state = TextFindState()
    state.query = "ab"
    let ran = text_find_rebuild(state, "xxAByyab", 1)
    t |> success(ran, "first rebuild runs")
    t |> equal(length(state.matches), 2, "case-insensitive literal finds both spellings")
    t |> equal(state.matches[0].start_byte, 2, "first match starts at AB")
    t |> equal(state.matches[0].end_byte, 4, "first match spans the needle length")
    t |> equal(state.matches[1].start_byte, 6, "second match starts at ab")
    t |> equal(state.current, 0, "current begins at the first match")
    let again = text_find_rebuild(state, "xxAByyab", 1)
    t |> success(!again, "identical query and revision short-circuits")
    text_find_dispose(state)
}

[test]
def test_find_step_wraps(t : T?) {
    var state = TextFindState()
    state.query = "a"
    let _ = text_find_rebuild(state, "a b a b a", 1)
    t |> equal(length(state.matches), 3, "three single-byte matches")
    let jump_before = state.jump_revision
    text_find_step(state, 1)
    t |> equal(state.current, 1, "step forward advances")
    text_find_step(state, 1)
    text_find_step(state, 1)
    t |> equal(state.current, 0, "step forward wraps to the first match")
    text_find_step(state, -1)
    t |> equal(state.current, 2, "step back wraps to the last match")
    t |> equal(state.jump_revision, jump_before + 4, "every step advances the jump revision")
    text_find_dispose(state)
}

[test]
def test_find_sticky_current_across_rebuild(t : T?) {
    var state = TextFindState()
    state.query = "aa"
    let _ = text_find_rebuild(state, "aa..aa..aa", 1)
    text_find_step(state, 1)
    t |> equal(state.matches[state.current].start_byte, 4, "current sits on the middle match")
    state.query = "aa."
    let _2 = text_find_rebuild(state, "aa..aa..aa", 1)
    t |> equal(length(state.matches), 2, "narrowed query drops the final match")
    t |> equal(state.matches[state.current].start_byte, 4,
               "current stays at the same position after the rebuild")
    text_find_dispose(state)
}

[test]
def test_find_regex_and_errors(t : T?) {
    var state = TextFindState()
    state.use_regex = true
    state.query = "requ[a-z]+e"
    let _ = text_find_rebuild(state, "require strings\nrequire math\n", 1)
    t |> equal(length(state.matches), 2, "regex matches every require")
    t |> equal(state.matches[0].start_byte, 0, "first regex match anchors at zero")
    t |> equal(state.matches[0].end_byte, length("require"), "regex match spans the word")
    t |> equal(state.error, "", "valid regex reports no error")
    state.query = "["
    let _2 = text_find_rebuild(state, "require strings\n", 1)
    t |> equal(length(state.matches), 0, "invalid regex yields no matches")
    t |> success(!empty(state.error), "invalid regex reports an error")
    state.query = "x*"
    let _3 = text_find_rebuild(state, "aaa", 2)
    t |> equal(length(state.matches), 0, "zero-width matches are filtered out")
    t |> equal(state.error, "", "zero-width filtering is not an error")
    text_find_dispose(state)
}
