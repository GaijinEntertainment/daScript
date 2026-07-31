options gen2
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_source_edit

//! Pure-logic coverage for the E1 editing core: no UI, no live host.

[test]
def test_edit_load_and_source_round_trip(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "a\r\nb\r\n")
    t |> success(ed.crlf, "CRLF flavor detected on load")
    t |> equal(length(ed.lines), 3, "trailing newline yields a final empty line")
    t |> equal(ed.lines[0], "a", "first line split")
    t |> equal(ed.lines[2], "", "final line empty")
    t |> equal(text_edit_source(ed), "a\r\nb\r\n", "CRLF source round-trips exactly")
    t |> success(!ed.dirty, "freshly loaded buffer is clean")
    text_edit_load(ed, "x\ny")
    t |> success(!ed.crlf, "LF source stays LF")
    t |> equal(text_edit_source(ed), "x\ny", "LF source round-trips")
    text_edit_load(ed, "")
    t |> equal(length(ed.lines), 1, "empty source is one empty line")
    t |> equal(text_edit_source(ed), "", "empty source round-trips")
    text_edit_dispose(ed)
}

[test]
def test_edit_insert_and_line_splits(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "")
    let clean_revision = ed.revision
    text_edit_insert(ed, "hi")
    t |> equal(ed.lines[0], "hi", "typing appends into the line")
    t |> equal(ed.cursor_column, 2, "cursor lands after the inserted text")
    t |> success(ed.dirty, "insert marks the buffer dirty")
    t |> success(ed.revision > clean_revision, "insert advances the revision")
    text_edit_newline(ed)
    t |> equal(length(ed.lines), 2, "newline splits the buffer")
    t |> equal(ed.cursor_line, 1, "cursor moves to the new line")
    t |> equal(ed.cursor_column, 0, "cursor starts the new line")
    text_edit_load(ed, "headtail")
    text_edit_set_cursor(ed, 0, 4)
    text_edit_newline(ed)
    t |> equal(ed.lines[0], "head", "mid-line split keeps the left half")
    t |> equal(ed.lines[1], "tail", "mid-line split carries the right half")
    text_edit_load(ed, "xy")
    text_edit_set_cursor(ed, 0, 1)
    text_edit_insert(ed, "a\r\nb\nc")
    t |> equal(length(ed.lines), 3, "multi-line insert adds interior lines")
    t |> equal(ed.lines[0], "xa", "first inserted segment joins the left half")
    t |> equal(ed.lines[1], "b", "interior segment becomes its own line")
    t |> equal(ed.lines[2], "cy", "last segment joins the right half")
    t |> equal(ed.cursor_line, 2, "cursor lands on the last inserted line")
    t |> equal(ed.cursor_column, 1, "cursor lands after the last segment")
    text_edit_dispose(ed)
}

[test]
def test_edit_backspace_delete_and_joins(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "ab\ncd")
    text_edit_set_cursor(ed, 1, 0)
    text_edit_backspace(ed)
    t |> equal(length(ed.lines), 1, "backspace at column zero joins lines")
    t |> equal(ed.lines[0], "abcd", "joined line keeps both halves")
    t |> equal(ed.cursor_column, 2, "cursor sits at the join point")
    text_edit_backspace(ed)
    t |> equal(ed.lines[0], "acd", "backspace removes the previous character")
    text_edit_load(ed, "ab\ncd")
    text_edit_set_cursor(ed, 0, 2)
    text_edit_delete(ed)
    t |> equal(length(ed.lines), 1, "delete at line end joins the next line up")
    t |> equal(ed.lines[0], "abcd", "forward join keeps both halves")
    t |> equal(ed.cursor_column, 2, "forward join leaves the cursor in place")
    text_edit_delete(ed)
    t |> equal(ed.lines[0], "abd", "delete removes the next character")
    text_edit_set_cursor(ed, 0, 0)
    let revision_before = ed.revision
    text_edit_backspace(ed)
    t |> equal(ed.revision, revision_before, "backspace at buffer start is a no-op")
    text_edit_set_cursor(ed, 0, 3)
    text_edit_delete(ed)
    t |> equal(ed.revision, revision_before, "delete at buffer end is a no-op")
    text_edit_dispose(ed)
}

[test]
def test_edit_utf8_grapheme_movement(t : T?) {
    var ed = TextEditState()
    // a(1) é(2) 你(3) 🌍(4) b(1) — byte boundaries 0,1,3,6,10,11.
    text_edit_load(ed, "aé你🌍b")
    text_edit_move_end(ed)
    t |> equal(ed.cursor_column, 11, "End lands on the byte length")
    text_edit_move_left(ed)
    t |> equal(ed.cursor_column, 10, "left steps over the single-byte tail")
    text_edit_move_left(ed)
    t |> equal(ed.cursor_column, 6, "left steps over the 4-byte emoji")
    text_edit_move_left(ed)
    t |> equal(ed.cursor_column, 3, "left steps over the 3-byte CJK")
    text_edit_move_left(ed)
    t |> equal(ed.cursor_column, 1, "left steps over the 2-byte accent")
    text_edit_move_right(ed)
    text_edit_move_right(ed)
    t |> equal(ed.cursor_column, 6, "right re-crosses the same boundaries")
    text_edit_move_end(ed)
    text_edit_backspace(ed)
    text_edit_backspace(ed)
    t |> equal(ed.lines[0], "aé你", "backspace removes whole graphemes")
    let combined = "x" + codepoint_utf8(int(0x65)) + codepoint_utf8(int(0x301))
    text_edit_load(ed, combined)
    text_edit_move_end(ed)
    text_edit_move_left(ed)
    t |> equal(ed.cursor_column, 1, "combining mark clusters as one grapheme")
    text_edit_move_end(ed)
    text_edit_backspace(ed)
    t |> equal(ed.lines[0], "x", "backspace removes the whole cluster")
    text_edit_dispose(ed)
}

[test]
def test_edit_vertical_preferred_column(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "longer line\nab\nabcdef")
    text_edit_set_cursor(ed, 0, 8)
    text_edit_move_vertical(ed, 1)
    t |> equal(ed.cursor_line, 1, "down moves one line")
    t |> equal(ed.cursor_column, 2, "short line clamps the column")
    text_edit_move_vertical(ed, 1)
    t |> equal(ed.cursor_column, 6, "preferred column survives the short line")
    text_edit_move_vertical(ed, -2)
    t |> equal(ed.cursor_column, 8, "preferred column restores on the way back")
    text_edit_move_vertical(ed, -1)
    t |> equal(ed.cursor_column, 0, "up on the first line goes to line start")
    text_edit_set_cursor(ed, 2, 2)
    text_edit_move_vertical(ed, 1)
    t |> equal(ed.cursor_column, 6, "down on the last line goes to line end")
    text_edit_move_vertical(ed, -10)
    t |> equal(ed.cursor_line, 0, "page up clamps to the first line")
    text_edit_dispose(ed)
}

[test]
def test_edit_selection_core(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "alpha\nbeta\ngamma")
    t |> success(!text_edit_has_selection(ed), "fresh buffer has no selection")
    text_edit_select(ed, 0, 2, 2, 3)
    t |> success(text_edit_has_selection(ed), "programmatic selection is active")
    t |> equal(text_edit_selected_text(ed), "pha\nbeta\ngam", "multi-line selected text")
    text_edit_select(ed, 1, 3, 1, 1)
    t |> equal(text_edit_selected_text(ed), "et", "backwards selection is ordered")
    let sel = text_edit_selection(ed)
    t |> equal(sel.start_column, 1, "ordered start")
    t |> equal(sel.end_column, 3, "ordered end")
    text_edit_move_left(ed)
    t |> success(!text_edit_has_selection(ed), "plain left collapses the selection")
    t |> equal(ed.cursor_column, 1, "collapse lands at selection start")
    text_edit_select(ed, 1, 1, 1, 3)
    text_edit_move_right(ed)
    t |> equal(ed.cursor_column, 3, "plain right collapses to selection end")
    text_edit_select_all(ed)
    t |> equal(text_edit_selected_text(ed), "alpha\nbeta\ngamma", "select all covers the buffer")
    text_edit_clear_selection(ed)
    t |> success(!text_edit_has_selection(ed), "clear drops the selection")
    text_edit_dispose(ed)
}

[test]
def test_edit_selection_edits(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "alpha\nbeta\ngamma")
    text_edit_select(ed, 0, 2, 2, 3)
    text_edit_insert(ed, "X")
    t |> equal(text_edit_source(ed), "alXma", "insert replaces the selection")
    t |> equal(ed.cursor_column, 3, "cursor lands after the replacement")
    text_edit_load(ed, "alpha\nbeta")
    text_edit_select(ed, 0, 3, 1, 2)
    text_edit_backspace(ed)
    t |> equal(text_edit_source(ed), "alpta", "backspace deletes only the selection")
    t |> success(!text_edit_has_selection(ed), "selection is gone after the delete")
    text_edit_load(ed, "alpha")
    text_edit_select(ed, 0, 1, 0, 4)
    text_edit_delete(ed)
    t |> equal(text_edit_source(ed), "aa", "delete removes only the selection")
    text_edit_dispose(ed)
}

[test]
def test_edit_shift_movement(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "one two\nthree")
    text_edit_move_right(ed, true)
    text_edit_move_right(ed, true)
    t |> equal(text_edit_selected_text(ed), "on", "shift-right extends by graphemes")
    text_edit_move_left(ed, true)
    t |> equal(text_edit_selected_text(ed), "o", "shift-left shrinks back")
    text_edit_move_end(ed, true)
    t |> equal(text_edit_selected_text(ed), "one two", "shift-end extends to line end")
    text_edit_move_vertical(ed, 1, true)
    t |> equal(text_edit_selected_text(ed), "one two\nthree", "shift-down crosses the line")
    text_edit_move_document_start(ed, true)
    t |> success(!text_edit_has_selection(ed),
        "selection collapses when the cursor returns onto the anchor")
    text_edit_set_cursor(ed, 0, 0)
    t |> success(!text_edit_has_selection(ed), "set_cursor clears the selection")
    text_edit_dispose(ed)
}

[test]
def test_edit_word_motion(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "foo.bar baz")
    text_edit_move_document_end(ed)
    text_edit_move_word_left(ed)
    t |> equal(ed.cursor_column, 8, "word-left lands at baz start")
    text_edit_move_word_left(ed)
    t |> equal(ed.cursor_column, 4, "word-left lands at bar start")
    text_edit_move_word_left(ed)
    t |> equal(ed.cursor_column, 0, "lone dot is skipped with the word (fast rule)")
    text_edit_move_word_right(ed)
    t |> equal(ed.cursor_column, 3, "word-right lands at foo END (VS Code)")
    text_edit_move_word_right(ed)
    t |> equal(ed.cursor_column, 7, "word-right skips the lone dot to bar end")
    text_edit_move_word_right(ed)
    t |> equal(ed.cursor_column, 11, "word-right lands at baz end")
    text_edit_load(ed, "a->b")
    text_edit_move_document_end(ed)
    text_edit_move_word_left(ed)
    text_edit_move_word_left(ed)
    t |> equal(ed.cursor_column, 1, "multi-char separator run keeps its own stop")
    text_edit_load(ed, "one\ntwo")
    text_edit_set_cursor(ed, 1, 0)
    text_edit_move_word_left(ed)
    t |> equal(ed.cursor_line, 0, "word-left at column zero wraps to the previous line")
    t |> equal(ed.cursor_column, 3, "and lands at its end")
    text_edit_dispose(ed)
}

[test]
def test_edit_word_delete(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "foo bar baz")
    text_edit_set_cursor(ed, 0, 7)
    text_edit_delete_word_left(ed)
    t |> equal(text_edit_source(ed), "foo  baz", "delete-word-left removes bar, keeps its space")
    t |> equal(ed.cursor_column, 4, "cursor sits where the word began")
    text_edit_load(ed, "foo   ")
    text_edit_move_end(ed)
    text_edit_delete_word_left(ed)
    t |> equal(text_edit_source(ed), "foo", "2+ spaces delete as their own run")
    text_edit_load(ed, "foo ")
    text_edit_move_end(ed)
    text_edit_delete_word_left(ed)
    t |> equal(text_edit_source(ed), "", "a single space goes with the word")
    text_edit_load(ed, "foo bar")
    text_edit_set_cursor(ed, 0, 3)
    text_edit_delete_word_right(ed)
    t |> equal(text_edit_source(ed), "foo", "delete-word-right eats the space and the word")
    text_edit_load(ed, "ab\ncd")
    text_edit_set_cursor(ed, 0, 2)
    text_edit_delete_word_right(ed)
    t |> equal(text_edit_source(ed), "abcd", "delete-word-right at line end joins")
    text_edit_dispose(ed)
}

[test]
def test_edit_smart_home(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "    code")
    text_edit_move_end(ed)
    text_edit_move_home(ed)
    t |> equal(ed.cursor_column, 4, "home goes to first non-blank")
    text_edit_move_home(ed)
    t |> equal(ed.cursor_column, 0, "home again toggles to column zero")
    text_edit_move_home(ed)
    t |> equal(ed.cursor_column, 4, "and back")
    text_edit_dispose(ed)
}

[test]
def test_undo_typing_coalesces(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "")
    text_edit_insert(ed, "a")
    text_edit_insert(ed, "b")
    text_edit_insert(ed, "c")
    t |> equal(length(ed.undo_stack), 1, "a typing run is one undo unit")
    t |> success(text_edit_undo(ed), "undo runs")
    t |> equal(text_edit_source(ed), "", "one undo reverts the whole run")
    t |> success(!text_edit_can_undo(ed), "stack is empty")
    t |> success(text_edit_can_redo(ed), "redo is armed")
    t |> success(text_edit_redo(ed), "redo runs")
    t |> equal(text_edit_source(ed), "abc", "redo restores the run")
    t |> equal(ed.cursor_column, 3, "redo restores the after-cursor")
    text_edit_dispose(ed)
}

[test]
def test_undo_word_sized_units(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "")
    text_edit_insert(ed, "a")
    text_edit_insert(ed, "b")
    text_edit_insert(ed, " ")
    text_edit_insert(ed, "c")
    t |> equal(length(ed.undo_stack), 2, "space breaks before, not after (word units)")
    let _ = text_edit_undo(ed)
    t |> equal(text_edit_source(ed), "ab", "undo drops the space and what followed")
    text_edit_insert(ed, "!")
    t |> success(!text_edit_can_redo(ed), "a new edit clears the redo stack")
    text_edit_dispose(ed)
}

[test]
def test_undo_breaks_on_kind_and_adjacency(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "")
    text_edit_insert(ed, "a")
    text_edit_backspace(ed)
    t |> equal(length(ed.undo_stack), 2, "typing then backspace is two units")
    text_edit_load(ed, "hello world")
    text_edit_set_cursor(ed, 0, 5)
    text_edit_insert(ed, "x")
    text_edit_set_cursor(ed, 0, 0)
    text_edit_insert(ed, "y")
    t |> equal(length(ed.undo_stack), 2, "a caret jump breaks the typing run")
    let _ = text_edit_undo(ed)
    t |> equal(text_edit_source(ed), "hellox world", "first undo reverts only the last unit")
    text_edit_dispose(ed)
}

[test]
def test_undo_backspace_run_and_joins(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "abcd")
    text_edit_move_end(ed)
    text_edit_backspace(ed)
    text_edit_backspace(ed)
    t |> equal(length(ed.undo_stack), 1, "a backspace run coalesces")
    let _ = text_edit_undo(ed)
    t |> equal(text_edit_source(ed), "abcd", "undo restores both characters")
    text_edit_load(ed, "ab\ncd")
    text_edit_set_cursor(ed, 1, 0)
    text_edit_backspace(ed)
    t |> equal(text_edit_source(ed), "abcd", "join happened")
    let _2 = text_edit_undo(ed)
    t |> equal(text_edit_source(ed), "ab\ncd", "undo restores the newline")
    t |> equal(ed.cursor_line, 1, "and the pre-join cursor line")
    text_edit_dispose(ed)
}

[test]
def test_undo_enter_fence_and_selection_restore(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "")
    text_edit_insert(ed, "a")
    text_edit_newline(ed)
    text_edit_insert(ed, "b")
    t |> equal(length(ed.undo_stack), 2, "Enter fences before itself")
    let _ = text_edit_undo(ed)
    t |> equal(text_edit_source(ed), "a", "newline and following typing undo together")
    text_edit_load(ed, "alpha beta")
    text_edit_select(ed, 0, 0, 0, 5)
    text_edit_insert(ed, "X")
    t |> equal(text_edit_source(ed), "X beta", "selection replaced")
    let _2 = text_edit_undo(ed)
    t |> equal(text_edit_source(ed), "alpha beta", "undo restores the replaced text")
    t |> equal(text_edit_selected_text(ed), "alpha", "and the selection that was replaced")
    text_edit_dispose(ed)
}

[test]
def test_undo_push_stop_and_word_delete(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "")
    text_edit_insert(ed, "a")
    text_edit_push_undo_stop(ed)
    text_edit_insert(ed, "b")
    t |> equal(length(ed.undo_stack), 2, "an explicit fence splits same-kind typing")
    text_edit_load(ed, "one two")
    text_edit_move_end(ed)
    text_edit_delete_word_left(ed)
    t |> equal(text_edit_source(ed), "one ", "word delete applied")
    text_edit_delete_word_left(ed)
    t |> equal(text_edit_source(ed), "", "second word delete applied")
    t |> equal(length(ed.undo_stack), 2, "word deletes never coalesce")
    let _ = text_edit_undo(ed)
    t |> equal(text_edit_source(ed), "one ", "each word delete undoes alone")
    text_edit_dispose(ed)
}

[test]
def test_undo_multiline_insert(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "headtail")
    text_edit_set_cursor(ed, 0, 4)
    text_edit_insert(ed, "A\nB\nC")
    t |> equal(text_edit_source(ed), "headA\nB\nCtail", "multi-line insert applied")
    let _ = text_edit_undo(ed)
    t |> equal(text_edit_source(ed), "headtail", "multi-line insert undoes cleanly")
    t |> equal(ed.cursor_column, 4, "cursor back at the insert point")
    let _2 = text_edit_redo(ed)
    t |> equal(text_edit_source(ed), "headA\nB\nCtail", "and redoes cleanly")
    text_edit_dispose(ed)
}

[test]
def test_clipboard_selection_round_trip(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "alpha beta")
    text_edit_select(ed, 0, 0, 0, 5)
    let copied = text_edit_copy(ed)
    t |> equal(copied, "alpha", "copy returns the selected text")
    t |> equal(text_edit_clipboard_text(), "alpha", "internal clipboard records it")
    text_edit_set_cursor(ed, 0, 10)
    text_edit_paste(ed, copied)
    t |> equal(text_edit_source(ed), "alpha betaalpha", "paste inserts at the caret")
    let _ = text_edit_undo(ed)
    t |> equal(text_edit_source(ed), "alpha beta", "paste is one undo unit")
    text_edit_select(ed, 0, 0, 0, 6)
    let cut = text_edit_cut(ed)
    t |> equal(cut, "alpha ", "cut returns the removed text")
    t |> equal(text_edit_source(ed), "beta", "cut removes the selection")
    let _2 = text_edit_undo(ed)
    t |> equal(text_edit_source(ed), "alpha beta", "cut is one undo unit")
    t |> equal(text_edit_selected_text(ed), "alpha ", "undo of cut restores the selection")
    text_edit_dispose(ed)
}

[test]
def test_clipboard_line_mode(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "one\ntwo\nthree")
    text_edit_set_cursor(ed, 1, 2)
    let copied = text_edit_copy(ed)
    t |> equal(copied, "two\n", "empty-selection copy takes the whole line")
    text_edit_set_cursor(ed, 2, 3)
    text_edit_paste(ed, copied)
    t |> equal(text_edit_source(ed), "one\ntwo\ntwo\nthree",
        "line-mode paste inserts ABOVE, pushing the line down")
    t |> equal(ed.cursor_line, 3, "caret follows its text line down")
    t |> equal(ed.cursor_column, 3, "caret column preserved")
    let _ = text_edit_undo(ed)
    t |> equal(text_edit_source(ed), "one\ntwo\nthree", "line-mode paste undoes as one unit")
    text_edit_set_cursor(ed, 1, 1)
    let cut = text_edit_cut(ed)
    t |> equal(cut, "two\n", "empty-selection cut takes the line and newline")
    t |> equal(text_edit_source(ed), "one\nthree", "cut line removed entirely")
    let _2 = text_edit_undo(ed)
    t |> equal(text_edit_source(ed), "one\ntwo\nthree", "line cut undoes")
    text_edit_set_cursor(ed, 2, 0)
    let last_cut = text_edit_cut(ed)
    t |> equal(last_cut, "three\n", "cutting the last line still reports its newline")
    t |> equal(text_edit_source(ed), "one\ntwo", "last-line cut removes the trailing newline")
    text_edit_dispose(ed)
}

[test]
def test_clipboard_external_text_is_not_line_mode(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "one\ntwo")
    text_edit_set_cursor(ed, 0, 1)
    let _ = text_edit_copy(ed) // arms line mode with "one\n"
    text_edit_paste(ed, "different\n")
    t |> equal(text_edit_source(ed), "odifferent\nne\ntwo",
        "unrecognized text pastes at the caret, not as a line block")
    text_edit_dispose(ed)
}

[test]
def test_overtype_mode(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "abcd")
    text_edit_toggle_overtype(ed)
    t |> success(ed.overtype, "overtype toggles on")
    text_edit_type(ed, "X")
    t |> equal(text_edit_source(ed), "Xbcd", "overtype replaces the next character")
    t |> equal(ed.cursor_column, 1, "cursor advances past the replacement")
    text_edit_type(ed, "Y")
    t |> equal(text_edit_source(ed), "XYcd", "consecutive overtypes continue")
    t |> equal(length(ed.undo_stack), 1, "overtype run coalesces into one unit")
    let _ = text_edit_undo(ed)
    t |> equal(text_edit_source(ed), "abcd", "overtype run undoes together")
    text_edit_move_end(ed)
    text_edit_type(ed, "Z")
    t |> equal(text_edit_source(ed), "abcdZ", "overtype at line end falls back to insert")
    text_edit_load(ed, "ab\ncd")
    ed.overtype = true
    text_edit_set_cursor(ed, 0, 2)
    text_edit_type(ed, "Q")
    t |> equal(text_edit_source(ed), "abQ\ncd", "overtype never eats the newline")
    text_edit_toggle_overtype(ed)
    t |> success(!ed.overtype, "overtype toggles off")
    text_edit_dispose(ed)
}
