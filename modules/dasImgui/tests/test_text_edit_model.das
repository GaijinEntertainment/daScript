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
