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

[test]
def test_find_reopen_rearms_jump(t : T?) {
    var state = TextFindState()
    text_find_open(state)
    t |> equal(state.jump_revision, 0, "opening with no matches does not arm a jump")
    state.query = "aa"
    let _ = text_find_rebuild(state, "aa..aa", 1)
    text_find_step(state, 1)
    let jump_before = state.jump_revision
    text_find_close(state)
    text_find_open(state)
    t |> equal(state.jump_revision, jump_before + 1, "reopening re-arms the jump")
    t |> equal(state.current, 1, "reopening keeps the current match")
    text_find_open(state)
    t |> equal(state.jump_revision, jump_before + 1, "open while already open does not re-arm")
    text_find_dispose(state)
}

[test]
def test_find_replace_requests_and_open(t : T?) {
    var state = TextFindState()
    text_find_open_replace(state)
    t |> success(state.open && state.replace_open, "Ctrl+H opens both rows")
    t |> success(state.replace_focus_pending, "focus heads for the replace input")
    t |> success(!state.focus_pending, "the find input does not steal it")
    text_find_request_replace(state)
    text_find_request_replace_all(state)
    t |> equal(state.replace_revision, 1, "replace-one request counts")
    t |> equal(state.replace_all_revision, 1, "replace-all request counts")
    text_find_set_replace(state, "next")
    t |> equal(state.replace_text, "next", "programmatic replace text lands")
    t |> success(state.pending_replace_valid, "widget sync is armed")
    text_find_dispose(state)
}

[test]
def test_find_replacements_literal(t : T?) {
    var state = TextFindState()
    state.query = "aa"
    state.replace_text = "$1"
    let _ = text_find_rebuild(state, "aa..aa", 1)
    var texts : array<string>
    text_find_replacements(state, "aa..aa", texts)
    t |> equal(length(texts), 2, "one replacement per match")
    t |> equal(texts[0], "$1", "literal mode never expands templates")
    delete texts
    text_find_dispose(state)
}

[test]
def test_find_replacements_regex_templates(t : T?) {
    var state = TextFindState()
    state.use_regex = true
    state.query = "(\\w+)-(\\w+)"
    let source = "foo-bar baz-qux"
    let _ = text_find_rebuild(state, source, 1)
    t |> equal(length(state.matches), 2, "regex matches both pairs")
    var texts : array<string>
    state.replace_text = "$2_$1"
    text_find_replacements(state, source, texts)
    t |> equal(texts[0], "bar_foo", "numbered groups expand per match")
    t |> equal(texts[1], "qux_baz", "second match expands with ITS groups")
    state.replace_text = "[$&] costs $$5 $\{2\}"
    text_find_replacements(state, source, texts)
    t |> equal(texts[0], "[foo-bar] costs $5 bar",
        "whole match, literal dollar, and brace groups all expand")
    state.replace_text = "$5x"
    text_find_replacements(state, source, texts)
    t |> equal(texts[0], "x", "out-of-range groups expand to nothing")
    delete texts
    text_find_dispose(state)
}

[test]
def test_find_replacements_named_groups(t : T?) {
    var state = TextFindState()
    state.use_regex = true
    state.query = "(?P<key>\\w+)=(?P<val>\\w+)"
    let source = "a=1 b=2"
    let _ = text_find_rebuild(state, source, 1)
    t |> equal(length(state.matches), 2, "regex matches both assignments")
    var texts : array<string>
    state.replace_text = "$\{val\}:$\{key\}"
    text_find_replacements(state, source, texts)
    t |> equal(texts[0], "1:a", "named groups expand")
    t |> equal(texts[1], "2:b", "named groups track each match")
    delete texts
    text_find_dispose(state)
}
