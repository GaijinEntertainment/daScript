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

[test]
def test_line_operations(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "one\ntwo\nthree")
    text_edit_set_cursor(ed, 0, 1)
    text_edit_move_lines(ed, 1)
    t |> equal(text_edit_source(ed), "two\none\nthree", "move lines down swaps with the neighbor")
    t |> equal(ed.cursor_line, 1, "cursor follows the moved line")
    let _ = text_edit_undo(ed)
    t |> equal(text_edit_source(ed), "one\ntwo\nthree", "move undoes as one unit")
    text_edit_set_cursor(ed, 0, 0)
    text_edit_move_lines(ed, -1)
    t |> equal(text_edit_source(ed), "one\ntwo\nthree", "move up at the top is a no-op")
    text_edit_set_cursor(ed, 1, 2)
    text_edit_duplicate_lines(ed, true)
    t |> equal(text_edit_source(ed), "one\ntwo\ntwo\nthree", "duplicate copies the line")
    t |> equal(ed.cursor_line, 2, "caret lands on the lower copy")
    let _2 = text_edit_undo(ed)
    text_edit_set_cursor(ed, 1, 1)
    text_edit_delete_lines(ed)
    t |> equal(text_edit_source(ed), "one\nthree", "delete lines removes line and newline")
    t |> equal(ed.cursor_line, 1, "cursor stays at the removed slot")
    text_edit_set_cursor(ed, 1, 2)
    text_edit_delete_lines(ed)
    t |> equal(text_edit_source(ed), "one", "deleting the last line trims its newline")
    text_edit_dispose(ed)
}

[test]
def test_line_insert_indent_and_expand(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "\tindented\nplain")
    text_edit_set_cursor(ed, 0, 3)
    text_edit_insert_line_after(ed)
    t |> equal(text_edit_source(ed), "\tindented\n\t\nplain", "insert-below carries indentation")
    t |> equal(ed.cursor_line, 1, "caret on the new line")
    t |> equal(ed.cursor_column, 1, "caret after the carried indent")
    let _ = text_edit_undo(ed)
    text_edit_insert_line_before(ed)
    t |> equal(text_edit_source(ed), "\t\n\tindented\nplain", "insert-above carries indentation")
    t |> equal(ed.cursor_line, 0, "caret on the inserted line")
    text_edit_load(ed, "aa\n\nbb")
    text_edit_select(ed, 0, 0, 2, 2)
    text_edit_indent_lines(ed)
    t |> equal(text_edit_source(ed), "\taa\n\n\tbb", "block indent skips empty lines")
    t |> equal(text_edit_selected_text(ed), "\taa\n\n\tbb", "block stays selected")
    text_edit_outdent_lines(ed)
    t |> equal(text_edit_source(ed), "aa\n\nbb", "outdent reverses it")
    text_edit_load(ed, "    four\n\ttab")
    text_edit_select(ed, 0, 0, 1, 4)
    text_edit_outdent_lines(ed)
    t |> equal(text_edit_source(ed), "four\ntab", "outdent strips a tab or up to 4 spaces")
    text_edit_load(ed, "ab\ncd\nef")
    text_edit_set_cursor(ed, 0, 1)
    text_edit_expand_line_selection(ed)
    t |> equal(text_edit_selected_text(ed), "ab\n", "Ctrl+L selects the line with newline")
    text_edit_expand_line_selection(ed)
    t |> equal(text_edit_selected_text(ed), "ab\ncd\n", "repeat extends by a line")
    text_edit_dispose(ed)
}

[test]
def test_tab_smart_and_scroll_verb(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "aa\nbb")
    text_edit_select(ed, 0, 0, 1, 2)
    text_edit_tab(ed)
    t |> equal(text_edit_source(ed), "\taa\n\tbb", "Tab block-indents a multi-line selection")
    text_edit_load(ed, "hello")
    text_edit_select(ed, 0, 1, 0, 3)
    text_edit_tab(ed)
    t |> equal(text_edit_source(ed), "h\tlo", "Tab replaces a partial-line selection")
    text_edit_load(ed, "full")
    text_edit_select_all(ed)
    text_edit_tab(ed)
    t |> equal(text_edit_source(ed), "\tfull", "Tab indents a full-line selection")
    text_edit_scroll(ed, 3)
    text_edit_scroll(ed, -1)
    t |> equal(ed.pending_scroll_lines, 2, "scroll requests accumulate for the widget")
    text_edit_dispose(ed)
}

[test]
def test_vertical_moves_are_tab_visual(t : T?) {
    var ed = TextEditState()
    // tab_cells = 4: line 0 is [tab]X, so X starts at visual column 4.
    text_edit_load(ed, "\tX\nabcdeY")
    text_edit_set_cursor(ed, 0, 1)
    text_edit_move_vertical(ed, 1)
    t |> equal(ed.cursor_column, 4, "down from after-tab lands at visual column 4")
    text_edit_move_vertical(ed, -1)
    t |> equal(ed.cursor_column, 1, "up returns to the position after the tab")
    text_edit_set_cursor(ed, 1, 1)
    text_edit_move_vertical(ed, -1)
    t |> equal(ed.cursor_column, 0, "visual column 1 rounds to the tab start")
    text_edit_set_cursor(ed, 1, 3)
    text_edit_move_vertical(ed, -1)
    t |> equal(ed.cursor_column, 1, "visual column 3 rounds past the tab")
    text_edit_set_cursor(ed, 1, 5)
    text_edit_move_vertical(ed, -1, true)
    t |> equal(ed.cursor_column, 2, "shift-up selects to the tab-adjusted spot")
    t |> equal(text_edit_selected_text(ed), "\nabcde", "selection spans the visual landing")
    text_edit_dispose(ed)
}

[test]
def test_edit_goto_line(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "one\ntwo\nthree four\nfive")
    text_edit_goto_line(ed, 3, 7)
    t |> equal(ed.cursor_line, 2, "goto speaks 1-based lines")
    t |> equal(ed.cursor_column, 6, "and 1-based columns")
    text_edit_goto_line(ed, 99)
    t |> equal(ed.cursor_line, 3, "past-the-end clamps to the last line")
    text_edit_goto_line(ed, 2)
    t |> equal(ed.cursor_column, 0, "no column means column one")
    // the dialog's query parse
    t |> success(text_edit_goto_query(ed, "3:7"), "line:col parses")
    t |> equal(ed.cursor_line, 2, "query moved the line")
    t |> equal(ed.cursor_column, 6, "query moved the column")
    t |> success(text_edit_goto_query(ed, "1,2"), "comma works too")
    t |> equal(ed.cursor_line, 0, "comma query moved")
    t |> success(!text_edit_goto_query(ed, "banana"), "garbage declines")
    t |> equal(ed.cursor_line, 0, "and keeps the position")
    text_edit_open_goto(ed)
    t |> success(ed.goto_open && ed.goto_focus, "open arms the dialog + focus")
    text_edit_dispose(ed)
}

[test]
def test_edit_byte_mapping(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "ab\r\ncd\r\né")
    t |> equal(text_edit_text(ed), "ab\ncd\né", "text_edit_text joins with LF regardless of flavor")
    var at = text_edit_position_of_byte(ed, 0)
    t |> equal(at, int2(0, 0), "offset 0 is the buffer start")
    at = text_edit_position_of_byte(ed, 2)
    t |> equal(at, int2(0, 2), "offset at a line end stays on that line")
    at = text_edit_position_of_byte(ed, 3)
    t |> equal(at, int2(1, 0), "offset past the newline starts the next line")
    at = text_edit_position_of_byte(ed, 6)
    t |> equal(at, int2(2, 0), "offsets count LF newlines as one byte")
    at = text_edit_position_of_byte(ed, 100)
    t |> equal(at, int2(2, 2), "past-the-end offsets clamp to the buffer end")
    let span = text_edit_range_of_bytes(ed, 1, 4)
    t |> equal(span.start_line, 0, "range start maps by byte")
    t |> equal(span.start_column, 1, "range start column maps by byte")
    t |> equal(span.end_line, 1, "range end crosses the newline")
    t |> equal(span.end_column, 1, "range end column maps by byte")
    text_edit_dispose(ed)
}

[test]
def test_edit_replace_range_undo(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "hello world")
    text_edit_replace_range(ed, text_edit_range_of_bytes(ed, 6, 11), "das")
    t |> equal(text_edit_source(ed), "hello das", "replace swaps the range text")
    t |> equal(ed.cursor_column, 9, "cursor lands after the inserted text")
    t |> success(text_edit_undo(ed), "replace undoes")
    t |> equal(text_edit_source(ed), "hello world", "undo restores the original")
    t |> equal(ed.cursor_column, 0, "undo restores the pre-replace cursor")
    t |> success(text_edit_redo(ed), "replace redoes")
    t |> equal(text_edit_source(ed), "hello das", "redo reapplies the replacement")
    // multi-line span replaced by multi-line text
    text_edit_load(ed, "aa\nbb\ncc")
    text_edit_replace_range(ed, TextEditRange(start_line = 0, start_column = 1,
        end_line = 2, end_column = 1), "X\nY")
    t |> equal(text_edit_source(ed), "aX\nYc", "multi-line replace splices lines")
    t |> success(text_edit_undo(ed), "multi-line replace undoes")
    t |> equal(text_edit_source(ed), "aa\nbb\ncc", "undo restores the multi-line original")
    text_edit_dispose(ed)
}

[test]
def test_edit_replace_all_is_one_undo_unit(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "one two one\nthree one")
    var spans <- [text_edit_range_of_bytes(ed, 0, 3),
        text_edit_range_of_bytes(ed, 8, 11),
        text_edit_range_of_bytes(ed, 18, 21)]
    var texts <- ["first", "1", "1"]
    t |> equal(text_edit_replace_all(ed, spans, texts), 3, "replace-all reports the count")
    t |> equal(text_edit_source(ed), "first two 1\nthree 1",
        "replacements land back-to-front so earlier spans stay valid")
    t |> success(text_edit_undo(ed), "replace-all undoes")
    t |> equal(text_edit_source(ed), "one two one\nthree one",
        "ONE undo reverts the whole replace-all")
    t |> success(!text_edit_can_undo(ed), "replace-all was a single undo unit")
    t |> success(text_edit_redo(ed), "replace-all redoes")
    t |> equal(text_edit_source(ed), "first two 1\nthree 1",
        "ONE redo reapplies the whole replace-all")
    delete spans
    delete texts
    text_edit_dispose(ed)
}

[test]
def test_edit_find_sync_flow(t : T?) {
    var ed = TextEditState()
    var find = TextFindState()
    text_edit_load(ed, "alpha beta\ngamma alpha\nalpha")
    text_find_set_query(find, "alpha", false)
    text_edit_find_sync(ed, find)
    t |> equal(length(find.matches), 3, "sync rebuilds matches from the buffer")
    t |> equal(length(ed.highlight_ranges), 3, "every match paints a highlight")
    var sel = text_edit_selection(ed)
    t |> equal(sel.start_line, 0, "the current match is selected")
    t |> equal(sel.end_column, 5, "selection covers the match")
    text_find_step(find, 1)
    text_edit_find_sync(ed, find)
    sel = text_edit_selection(ed)
    t |> equal(sel.start_line, 1, "stepping jumps the selection to the next match")
    t |> equal(sel.start_column, 6, "the second match starts after 'gamma '")
    text_find_set_replace(find, "X")
    text_find_request_replace(find)
    text_edit_find_sync(ed, find)
    t |> equal(text_edit_source(ed), "alpha beta\ngamma X\nalpha",
        "replace-one rewrites only the current match")
    t |> equal(length(find.matches), 2, "matches recompute after the replace")
    sel = text_edit_selection(ed)
    t |> equal(sel.start_line, 2, "replace-one advances to the next match")
    text_find_request_replace_all(find)
    text_edit_find_sync(ed, find)
    t |> equal(text_edit_source(ed), "X beta\ngamma X\nX",
        "replace-all rewrites every remaining match")
    t |> success(text_edit_undo(ed), "replace-all undoes")
    t |> equal(text_edit_source(ed), "alpha beta\ngamma X\nalpha",
        "one undo reverts only the replace-all unit")
    text_find_close(find)
    text_edit_find_sync(ed, find)
    t |> equal(length(ed.highlight_ranges), 0, "closing find clears the highlights")
    text_find_dispose(find)
    text_edit_dispose(ed)
}

[test]
def test_edit_find_sync_no_replace_loop(t : T?) {
    var ed = TextEditState()
    var find = TextFindState()
    text_edit_load(ed, "a b a")
    text_find_set_query(find, "a", false)
    text_find_set_replace(find, "aa")
    text_edit_find_sync(ed, find)
    t |> equal(length(find.matches), 2, "two single-letter matches")
    text_find_request_replace(find)
    text_edit_find_sync(ed, find)
    t |> equal(text_edit_source(ed), "aa b a", "first match replaced")
    let current = text_find_current(find)
    t |> equal(current.start_byte, 5,
        "replace-one resumes PAST its own insertion, not on it")
    text_find_dispose(find)
    text_edit_dispose(ed)
}

[test]
def test_edit_lexical_syntax(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "def foo() \{ // note\n\tlet s = \"str\" + 42\n\}")
    text_edit_set_language(ed, "das")
    t |> equal(length(ed.line_styles), 3, "every line gets a style entry")
    t |> success(!empty(ed.line_styles[0].runs), "line 0 lexes")
    t |> equal(int(ed.line_styles[0].runs[0].style), int(TextSourceSyntaxStyle.keyword),
        "def is a keyword run")
    let l0_last = length(ed.line_styles[0].runs) - 1
    t |> equal(int(ed.line_styles[0].runs[l0_last].style),
        int(TextSourceSyntaxStyle.comment), "the trailing line comment styles")
    t |> equal(ed.line_styles[0].runs[l0_last].end_byte, length(ed.lines[0]),
        "the line comment runs to end of line")
    t |> equal(length(ed.line_styles[1].runs), 3, "line 1: let, string, number")
    t |> equal(int(ed.line_styles[1].runs[1].style), int(TextSourceSyntaxStyle.string_),
        "the string literal styles")
    t |> equal(int(ed.line_styles[1].runs[2].style), int(TextSourceSyntaxStyle.number),
        "the number styles")
    text_edit_set_language(ed, "")
    t |> success(empty(ed.line_styles), "clearing the language drops the styles")
    text_edit_dispose(ed)
}

[test]
def test_edit_block_comment_carry(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "aa\nbb\ncc")
    text_edit_set_language(ed, "das")
    t |> success(empty(ed.line_styles[1].runs), "plain identifiers get no runs")
    text_edit_set_cursor(ed, 0, 2)
    text_edit_insert(ed, " /*")
    t |> success(ed.line_styles[1].in_block_comment, "open comment carries into line 1")
    t |> success(ed.line_styles[2].in_block_comment, "and into line 2")
    t |> equal(int(ed.line_styles[2].runs[0].style), int(TextSourceSyntaxStyle.comment),
        "downstream lines restyle as comment")
    text_edit_set_cursor(ed, 1, length(ed.lines[1]))
    text_edit_insert(ed, "*/")
    t |> success(!ed.line_styles[2].in_block_comment, "closing it restyles downstream")
    t |> success(empty(ed.line_styles[2].runs), "line 2 is plain again")
    t |> success(text_edit_undo(ed), "undo the close")
    t |> success(ed.line_styles[2].in_block_comment, "undo restyles downstream too")
    text_edit_dispose(ed)
}

[test]
def test_edit_auto_indent_on_enter(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "def f() \{")
    text_edit_set_language(ed, "das")
    text_edit_move_end(ed)
    text_edit_newline(ed)
    t |> equal(ed.lines[1], "\t", "an unmatched opener indents the new line")
    text_edit_insert(ed, "x = 1")
    text_edit_newline(ed)
    t |> equal(ed.lines[2], "\t", "a plain line carries its indentation")
    // typing the closer on the fresh indented line dedents and lands it
    text_edit_type(ed, "}")
    t |> equal(ed.lines[2], "}", "the closer dedents its line")
    t |> success(text_edit_undo(ed), "the dedent undoes")
    t |> equal(ed.lines[2], "\t", "ONE undo removes the closer and restores indent")
    // openers inside strings and comments do not count
    text_edit_load(ed, "s = \"\{\" // \{")
    text_edit_set_language(ed, "das")
    text_edit_move_end(ed)
    text_edit_newline(ed)
    t |> equal(ed.lines[1], "", "braces in strings and comments never indent")
    // markdown has no indent pairs: Enter stays plain
    text_edit_load(ed, "\t- item \{")
    text_edit_set_language(ed, "md")
    text_edit_move_end(ed)
    text_edit_newline(ed)
    t |> equal(ed.lines[1], "", "markdown Enter does not auto-indent")
    // Enter BETWEEN a pair: closer gets its own line, caret indented between
    text_edit_load(ed, "\tfoo \{\}")
    text_edit_set_language(ed, "das")
    text_edit_set_cursor(ed, 0, 6)
    text_edit_newline(ed)
    t |> equal(length(ed.lines), 3, "the pair splits into three lines")
    t |> equal(ed.lines[0], "\tfoo \{", "the opener line keeps its text")
    t |> equal(ed.lines[1], "\t\t", "the middle line indents one deeper")
    t |> equal(ed.lines[2], "\t\}", "the closer keeps the original indent")
    t |> equal(ed.cursor_line, 1, "caret on the middle line")
    t |> equal(ed.cursor_column, 2, "caret at the middle indent")
    t |> success(text_edit_undo(ed), "the split undoes")
    t |> equal(text_edit_source(ed), "\tfoo \{\}", "one undo restores the pair")
    text_edit_dispose(ed)
}

[test]
def test_edit_auto_close_pairs(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "")
    text_edit_set_language(ed, "das")
    // opener at EOL closes itself, caret lands between
    text_edit_type(ed, "(")
    t |> equal(ed.lines[0], "()", "the opener auto-closes")
    t |> equal(ed.cursor_column, 1, "caret sits between the pair")
    // typing the closer steps over the twin instead of inserting
    text_edit_type(ed, ")")
    t |> equal(ed.lines[0], "()", "the closer skips its twin")
    t |> equal(ed.cursor_column, 2, "caret stepped over it")
    // an opener BEFORE a word does not pair
    text_edit_load(ed, "word")
    text_edit_set_language(ed, "das")
    text_edit_set_cursor(ed, 0, 0)
    text_edit_type(ed, "\{")
    t |> equal(ed.lines[0], "\{word", "no auto-close before an identifier")
    // backspace inside an empty pair eats both halves
    text_edit_load(ed, "")
    text_edit_set_language(ed, "das")
    text_edit_type(ed, "[")
    t |> equal(ed.lines[0], "[]", "bracket pairs too")
    text_edit_backspace(ed)
    t |> equal(ed.lines[0], "", "backspace deletes the empty pair whole")
    // quotes pair at word boundaries only
    text_edit_type(ed, "\"")
    t |> equal(ed.lines[0], "\"\"", "quote pairs at a boundary")
    text_edit_type(ed, "\"")
    t |> equal(ed.lines[0], "\"\"", "the second quote steps over")
    t |> equal(ed.cursor_column, 2, "caret after the pair")
    text_edit_type(ed, "s")
    text_edit_type(ed, "'")
    t |> equal(ed.lines[0], "\"\"s'", "no quote pair right after a word byte")
    // surround: a selection wraps and stays selected inside
    text_edit_load(ed, "abc")
    text_edit_set_language(ed, "das")
    text_edit_select(ed, 0, 0, 0, 3)
    text_edit_type(ed, "(")
    t |> equal(ed.lines[0], "(abc)", "the selection wraps in the pair")
    t |> equal(text_edit_selected_text(ed), "abc", "the content stays selected")
    t |> success(text_edit_undo(ed), "surround undoes")
    t |> equal(ed.lines[0], "abc", "one undo unwraps")
    // plain text pairs nothing (load keeps the language; clear it)
    text_edit_load(ed, "")
    text_edit_set_language(ed, "")
    text_edit_type(ed, "(")
    t |> equal(ed.lines[0], "(", "no language, no pairing")
    text_edit_dispose(ed)
}

[test]
def test_edit_bracket_match(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "def f(a : int) \{\n\tx = g(\"\}\")\n\}")
    text_edit_set_language(ed, "das")
    // caret ON the opener at end of line 0 matches the closer on line 2
    text_edit_set_cursor(ed, 0, 15)
    var matched = text_edit_match_bracket(ed)
    t |> success(matched.valid, "the opener matches")
    t |> equal(matched.close_line, 2, "its closer sits on the last line")
    t |> equal(matched.close_column, 0, "at column 0")
    // the } inside the string on line 1 was skipped
    text_edit_set_cursor(ed, 2, 1)
    matched = text_edit_match_bracket(ed)
    t |> success(matched.valid, "caret AFTER the closer matches back")
    t |> equal(matched.open_line, 0, "back to the line 0 opener")
    t |> equal(matched.open_column, 15, "at the brace")
    // nested parens: caret on ( of g(...) matches its own )
    text_edit_set_cursor(ed, 1, 6)
    matched = text_edit_match_bracket(ed)
    t |> success(matched.valid, "nested paren matches")
    t |> equal(matched.close_line, 1, "on the same line")
    // no bracket at the caret
    text_edit_set_cursor(ed, 1, 2)
    matched = text_edit_match_bracket(ed)
    t |> success(!matched.valid, "no bracket near the caret, no match")
    t |> success(!matched.mismatch, "and no mismatch either")
    // an unbalanced opener reports a mismatch at its own position
    text_edit_load(ed, "f(a\ng[b")
    text_edit_set_language(ed, "das")
    text_edit_set_cursor(ed, 0, 1)
    matched = text_edit_match_bracket(ed)
    t |> success(!matched.valid && matched.mismatch, "lone ( is a mismatch")
    t |> equal(matched.open_column, 1, "at its own column")
    text_edit_set_cursor(ed, 1, 2)
    matched = text_edit_match_bracket(ed)
    t |> success(matched.mismatch, "lone [ is a mismatch too")
    text_edit_dispose(ed)
}

[test]
def test_edit_syntax_tracks_line_edits(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "def a\ndef b")
    text_edit_set_language(ed, "das")
    text_edit_set_cursor(ed, 0, 5)
    text_edit_newline(ed)
    t |> equal(length(ed.line_styles), 3, "split keeps entries aligned")
    t |> equal(int(ed.line_styles[2].runs[0].style), int(TextSourceSyntaxStyle.keyword),
        "the shifted line keeps its keyword run")
    text_edit_backspace(ed)
    t |> equal(length(ed.line_styles), 2, "join keeps entries aligned")
    text_edit_dispose(ed)
}

[test]
def test_edit_find_sync_regex_groups(t : T?) {
    var ed = TextEditState()
    var find = TextFindState()
    text_edit_load(ed, "foo-bar baz-qux")
    text_find_set_query(find, "(\\w+)-(\\w+)", true)
    text_find_set_replace(find, "$2_$1")
    text_edit_find_sync(ed, find)
    t |> equal(length(find.matches), 2, "regex matches both pairs")
    text_find_request_replace_all(find)
    text_edit_find_sync(ed, find)
    t |> equal(text_edit_source(ed), "bar_foo qux_baz",
        "group templates expand per match")
    t |> success(text_edit_undo(ed), "regex replace-all undoes")
    t |> equal(text_edit_source(ed), "foo-bar baz-qux", "one undo restores it")
    text_find_dispose(find)
    text_edit_dispose(ed)
}
