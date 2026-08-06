options gen2
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_source_edit
require strings

//! Pure-logic coverage for the snippet engine: template parsing, one-unit
//! undo, tab-stop navigation, edit tracking, the watchdog, key precedence.

def private selection_of(ed : TextEditState const) : int4 {
    let sel = text_edit_selection(ed)
    return int4(sel.start_line, sel.start_column, sel.end_line, sel.end_column)
}

[test]
def test_snippet_expand_and_stops(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "")
    t |> success(text_edit_snippet_expand(ed, "[$\{SEL}]($\{1:url})"),
        "link template expands")
    t |> equal(text_edit_text(ed), "[](url)", "empty SEL splices nothing")
    t |> success(text_edit_snippet_active(ed), "session armed")
    t |> equal(selection_of(ed), int4(0, 3, 0, 6), "stop 1 placeholder selected")
    text_edit_insert(ed, "x")
    t |> equal(text_edit_text(ed), "[](x)", "typing replaces the placeholder")
    t |> success(text_edit_snippet_active(ed), "session survives the overtype")
    t |> success(text_edit_snippet_next(ed), "Tab past the last stop")
    t |> success(!text_edit_snippet_active(ed), "session ends at the final")
    t |> equal(ed.cursor_line, 0, "final caret line")
    t |> equal(ed.cursor_column, 5, "final caret lands at the tracked body end")
    text_edit_dispose(ed)
}

[test]
def test_snippet_wrap_keeps_selection_for_chaining(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "hello")
    text_edit_select(ed, 0, 0, 0, 5)
    t |> success(text_edit_snippet_expand(ed, "**$\{SEL}**"), "bold wrap expands")
    t |> equal(text_edit_text(ed), "**hello**", "wrap surrounds the selection")
    t |> equal(selection_of(ed), int4(0, 2, 0, 7), "the spliced text stays selected")
    t |> success(text_edit_snippet_expand(ed, "*$\{SEL}*"), "italic wrap chains")
    t |> equal(text_edit_text(ed), "***hello***", "chained wrap nests")
    t |> equal(selection_of(ed), int4(0, 3, 0, 8), "chained selection carries")
    let first_undo = text_edit_undo(ed)
    t |> success(first_undo, "undo accepted")
    t |> equal(text_edit_text(ed), "**hello**", "one undo peels one wrap")
    t |> success(!text_edit_snippet_active(ed), "undo kills the session")
    let second_undo = text_edit_undo(ed)
    t |> success(second_undo, "second undo accepted")
    t |> equal(text_edit_text(ed), "hello", "second undo peels the other wrap")
    text_edit_dispose(ed)
}

[test]
def test_snippet_navigation(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "")
    t |> success(text_edit_snippet_expand(ed, "f($\{1:a}, $\{2:b})"), "two stops")
    t |> equal(selection_of(ed), int4(0, 2, 0, 3), "stop 1 first")
    t |> success(text_edit_snippet_next(ed), "Tab")
    t |> equal(selection_of(ed), int4(0, 5, 0, 6), "stop 2 second")
    t |> success(text_edit_snippet_prev(ed), "Shift+Tab")
    t |> equal(selection_of(ed), int4(0, 2, 0, 3), "back at stop 1")
    t |> success(text_edit_snippet_prev(ed), "Shift+Tab clamps")
    t |> equal(selection_of(ed), int4(0, 2, 0, 3), "still at the first stop")
    let _ = text_edit_snippet_next(ed)
    t |> success(text_edit_snippet_next(ed), "Tab past the last")
    t |> success(!text_edit_snippet_active(ed), "session over")
    t |> equal(ed.cursor_column, 7, "caret at the default final (body end)")
    text_edit_dispose(ed)
}

[test]
def test_snippet_indent_and_final_marker(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "\t")
    text_edit_set_cursor(ed, 0, 1)
    t |> success(text_edit_snippet_expand(ed, "if ($\{1:c}) \{\n\t$\{0}\n\}"),
        "block template expands")
    t |> equal(ed.lines[0], "\tif (c) \{", "first body line continues the caret line")
    t |> equal(ed.lines[1], "\t\t", "later lines inherit the caret indentation")
    t |> equal(ed.lines[2], "\t\}", "closing line indented once")
    t |> equal(selection_of(ed), int4(0, 5, 0, 6), "condition stop selected")
    t |> success(text_edit_snippet_next(ed), "Tab to the final")
    t |> success(!text_edit_snippet_active(ed), "explicit $\{0} ends the session")
    t |> equal(ed.cursor_line, 1, "final caret on the body line")
    t |> equal(ed.cursor_column, 2, "final caret after the inner indent")
    text_edit_dispose(ed)
}

[test]
def test_snippet_one_undo_unit_and_fencing(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "")
    let depth_before = length(ed.undo_stack)
    t |> success(text_edit_snippet_expand(ed, "f($\{1:a})"), "expand")
    t |> equal(length(ed.undo_stack), depth_before + 1,
        "the whole expansion is ONE undo unit")
    text_edit_insert(ed, "xy")
    t |> equal(text_edit_text(ed), "f(xy)", "typed over the placeholder")
    t |> equal(length(ed.undo_stack), depth_before + 2,
        "typing after the expansion never coalesces into it")
    let undo_typing = text_edit_undo(ed)
    t |> success(undo_typing, "undo accepted")
    t |> equal(text_edit_text(ed), "f(a)", "undo #1 reverts only the typing")
    let undo_expand = text_edit_undo(ed)
    t |> success(undo_expand, "second undo accepted")
    t |> equal(text_edit_text(ed), "", "undo #2 reverts the whole expansion")
    let redo_expand = text_edit_redo(ed)
    t |> success(redo_expand, "redo accepted")
    t |> equal(text_edit_text(ed), "f(a)", "redo re-applies the expansion")
    t |> success(!text_edit_snippet_active(ed), "redo does not resurrect the session")
    // redo restores the CURSOR only (never the anchor) - the patched
    // after-cursor is the stop-1 selection's focus end
    t |> equal(selection_of(ed), int4(0, 3, 0, 3),
        "redo lands the caret at the patched stop-1 position")
    text_edit_dispose(ed)
}

[test]
def test_snippet_tracking_shifts_later_stops(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "")
    t |> success(text_edit_snippet_expand(ed, "f($\{1:a}, $\{2:b})"), "expand")
    text_edit_insert(ed, "long")
    t |> equal(text_edit_text(ed), "f(long, b)", "stop 1 grew")
    t |> success(text_edit_snippet_next(ed), "Tab to stop 2")
    t |> equal(selection_of(ed), int4(0, 8, 0, 9),
        "stop 2 shifted by the stop-1 growth")
    text_edit_insert(ed, "\n")
    t |> success(!text_edit_snippet_active(ed),
        "a stop growing multi-line kills the session cleanly")
    text_edit_dispose(ed)
}

[test]
def test_snippet_sync_watchdog(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "tail")
    text_edit_set_cursor(ed, 0, 0)
    t |> success(text_edit_snippet_expand(ed, "f($\{1:a})"), "expand at the head")
    text_edit_snippet_sync(ed)
    t |> success(text_edit_snippet_active(ed), "caret inside keeps the session")
    text_edit_set_cursor(ed, 0, length(ed.lines[0]))
    text_edit_snippet_sync(ed)
    t |> success(!text_edit_snippet_active(ed),
        "the caret leaving the region ends the session")
    text_edit_dispose(ed)
}

[test]
def test_snippet_tab_precedence(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "")
    t |> success(text_edit_snippet_expand(ed, "f($\{1:a}, $\{2:b})"), "expand")
    ed.ghost_text = "GHOST"
    text_edit_tab(ed)
    t |> success(find(text_edit_text(ed), "GHOST") < 0,
        "session wins Tab over the ghost completion")
    t |> equal(selection_of(ed), int4(0, 5, 0, 6), "Tab advanced to stop 2")
    text_edit_tab(ed)
    t |> success(!text_edit_snippet_active(ed), "Tab walks out of the session")
    t |> success(find(text_edit_text(ed), "\t") < 0,
        "no literal tab landed while the session ran")
    text_edit_dispose(ed)
}

[test]
def test_snippet_sessionless_and_literal_forms(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "")
    t |> success(text_edit_snippet_expand(ed, "plain body"), "no stops expands")
    t |> success(!text_edit_snippet_active(ed), "no tab stops = no session")
    t |> equal(text_edit_text(ed), "plain body", "body inserted")
    t |> equal(ed.cursor_column, 10, "caret at the body end")
    text_edit_load(ed, "")
    t |> success(text_edit_snippet_expand(ed, "a $\{weird} b"), "non-placeholder")
    t |> equal(text_edit_text(ed), "a $\{weird} b",
        "a non-numeric token emits verbatim")
    text_edit_dispose(ed)
}
