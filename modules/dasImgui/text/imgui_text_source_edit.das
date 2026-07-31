options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_text_source_edit shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_text_source_view public
require imgui/imgui_text_flow public
require math
require strings
require daslib/strings_boost

//! Editing core + widget: an `array<string>` line buffer with a UTF-8-aware
//! (line, byte_column) cursor, rendered virtualised inside a scrolling child.
//! Buffer operations are pure (no ImGui) so tests drive them headless; the
//! widget owns focus, typed input, and scroll-follow. Selection, clipboard,
//! and undo arrive in the next arc steps.

struct public TextEditState {
    lines : array<string>
    crlf : bool = false
    cursor_line : int = 0
    //! Byte offset inside `lines[cursor_line]`; always a grapheme boundary.
    cursor_column : int = 0
    //! Selection anchor; -1 line = no anchor. Selection = anchor..cursor.
    anchor_line : int = -1
    anchor_column : int = 0
    selection_dragging : bool = false
    //! Grapheme index remembered across consecutive vertical moves; -1 = none.
    preferred_column : int = -1
    revision : int = 0
    dirty : bool = false
    focused : bool = false
    blink_time : float = 0.0f
    //! Bumped by edits/moves that must scroll the caret into view.
    follow_revision : int = 0
    followed_revision : int = 0
    input_revision : int = 0
    // widget layout cache + per-draw stamps for live inspection
    layout_revision : int = -1
    max_line_bytes : int = 0
    line_count : int = 1
    visible_rows : int = 0
}

struct public TextEditResult {
    focused : bool
    edited : bool
    cursor_moved : bool
}

struct public TextEditRange {
    //! Ordered (start <= end) line/byte-column selection range.
    start_line : int
    start_column : int
    end_line : int
    end_column : int
}

def public text_edit_dispose(var ed : TextEditState) {
    delete ed.lines
    ed = TextEditState()
}

def public text_edit_load(var ed : TextEditState; source : string) {
    //! Reset the buffer from `source`. CRLF endings are normalized to LF and
    //! remembered, so `text_edit_source` reproduces the original flavor.
    delete ed.lines
    ed.crlf = find(source, "\r\n") >= 0
    let normalized = replace(replace(source, "\r\n", "\n"), "\r", "\n")
    ed.lines <- split(normalized, "\n")
    if (empty(ed.lines)) {
        ed.lines |> push("")
    }
    ed.cursor_line = 0
    ed.cursor_column = 0
    ed.preferred_column = -1
    ed.dirty = false
    ed.revision++
    ed.follow_revision++
}

def public text_edit_source(ed : TextEditState const) : string {
    return join(ed.lines, ed.crlf ? "\r\n" : "\n")
}

def public text_edit_line_count(ed : TextEditState const) : int {
    return max(1, length(ed.lines))
}

def private ensure_line(var ed : TextEditState) {
    if (empty(ed.lines)) {
        ed.lines |> push("")
    }
    ed.cursor_line = clamp(ed.cursor_line, 0, length(ed.lines) - 1)
    ed.cursor_column = clamp(ed.cursor_column, 0, length(ed.lines[ed.cursor_line]))
}

// ===== Selection =====

def public text_edit_has_selection(ed : TextEditState const) : bool {
    return (ed.anchor_line >= 0
        && (ed.anchor_line != ed.cursor_line || ed.anchor_column != ed.cursor_column))
}

def public text_edit_selection(ed : TextEditState const) : TextEditRange {
    //! Ordered selection range; collapses to the cursor when no selection.
    if (!text_edit_has_selection(ed)) {
        return TextEditRange(start_line = ed.cursor_line, start_column = ed.cursor_column,
            end_line = ed.cursor_line, end_column = ed.cursor_column)
    }
    if (ed.anchor_line < ed.cursor_line
            || (ed.anchor_line == ed.cursor_line && ed.anchor_column < ed.cursor_column)) {
        return TextEditRange(start_line = ed.anchor_line, start_column = ed.anchor_column,
            end_line = ed.cursor_line, end_column = ed.cursor_column)
    }
    return TextEditRange(start_line = ed.cursor_line, start_column = ed.cursor_column,
        end_line = ed.anchor_line, end_column = ed.anchor_column)
}

def public text_edit_clear_selection(var ed : TextEditState) {
    ed.anchor_line = -1
    ed.anchor_column = 0
}

def public text_edit_select(var ed : TextEditState;
                            anchor_line, anchor_column, line, column : int) {
    //! Programmatic selection: anchor at (anchor_line, anchor_column), cursor
    //! at (line, column); both clamped.
    ensure_line(ed)
    let last = length(ed.lines) - 1
    ed.anchor_line = clamp(anchor_line, 0, last)
    ed.anchor_column = clamp(anchor_column, 0, length(ed.lines[ed.anchor_line]))
    ed.cursor_line = clamp(line, 0, last)
    ed.cursor_column = clamp(column, 0, length(ed.lines[ed.cursor_line]))
    ed.preferred_column = -1
    touch_move(ed)
}

def public text_edit_select_all(var ed : TextEditState) {
    ensure_line(ed)
    let last = length(ed.lines) - 1
    text_edit_select(ed, 0, 0, last, length(ed.lines[last]))
}

def public text_edit_selected_text(ed : TextEditState const) : string {
    //! Selected text with LF newlines ("" when no selection).
    return "" if (!text_edit_has_selection(ed))
    let sel = text_edit_selection(ed)
    if (sel.start_line == sel.end_line) {
        return slice(ed.lines[sel.start_line], sel.start_column, sel.end_column)
    }
    return build_string() $(var writer) {
        writer |> write(slice(ed.lines[sel.start_line], sel.start_column,
            length(ed.lines[sel.start_line])))
        for (line in range(sel.start_line + 1, sel.end_line)) {
            writer |> write("\n")
            writer |> write(ed.lines[line])
        }
        writer |> write("\n")
        writer |> write(slice(ed.lines[sel.end_line], 0, sel.end_column))
    }
}

def private begin_move(var ed : TextEditState; select : bool) {
    //! Shift-move arms the anchor at the pre-move cursor; a plain move drops it.
    if (select) {
        if (ed.anchor_line < 0) {
            ed.anchor_line = ed.cursor_line
            ed.anchor_column = ed.cursor_column
        }
    } else {
        text_edit_clear_selection(ed)
    }
}

def private delete_selection(var ed : TextEditState) : bool {
    return false if (!text_edit_has_selection(ed))
    let sel = text_edit_selection(ed)
    ed.lines[sel.start_line] = (slice(ed.lines[sel.start_line], 0, sel.start_column)
        + slice(ed.lines[sel.end_line], sel.end_column, length(ed.lines[sel.end_line])))
    if (sel.end_line > sel.start_line) {
        ed.lines |> erase(sel.start_line + 1, sel.end_line - sel.start_line)
    }
    ed.cursor_line = sel.start_line
    ed.cursor_column = sel.start_column
    text_edit_clear_selection(ed)
    return true
}

// ===== Grapheme boundaries =====

def private grapheme_prev(line : string; column : int) : int {
    return 0 if (column <= 0)
    var result = 0
    var inscope graphemes <- text_graphemes(line)
    for (grapheme in graphemes) {
        break if (grapheme.source.start_byte >= column)
        result = grapheme.source.start_byte
    }
    return result
}

def private grapheme_next(line : string; column : int) : int {
    let line_length = length(line)
    return line_length if (column >= line_length)
    var inscope graphemes <- text_graphemes(line)
    for (grapheme in graphemes) {
        return grapheme.source.end_byte if (grapheme.source.end_byte > column)
    }
    return line_length
}

def private grapheme_index_of(line : string; column : int) : int {
    var index = 0
    var inscope graphemes <- text_graphemes(line)
    for (grapheme in graphemes) {
        break if (grapheme.source.end_byte > column)
        index++
    }
    return index
}

def private grapheme_column_at(line : string; index : int) : int {
    var at = 0
    var inscope graphemes <- text_graphemes(line)
    for (grapheme in graphemes) {
        return grapheme.source.start_byte if (at == index)
        at++
    }
    return length(line)
}

// ===== Edits =====

def private touch_edit(var ed : TextEditState) {
    ed.dirty = true
    ed.revision++
    ed.preferred_column = -1
    ed.follow_revision++
    ed.blink_time = 0.0f
}

def private touch_move(var ed : TextEditState) {
    ed.follow_revision++
    ed.blink_time = 0.0f
}

def public text_edit_insert(var ed : TextEditState; text : string) {
    //! Insert `text` at the cursor, replacing any selection. Embedded newlines
    //! (any flavor) split lines; the cursor lands after the inserted text.
    return if (empty(text))
    ensure_line(ed)
    if (delete_selection(ed)) {
        ed.dirty = true
        ed.revision++
    }
    let normalized = replace(replace(text, "\r\n", "\n"), "\r", "\n")
    let line = ed.lines[ed.cursor_line]
    let left = slice(line, 0, ed.cursor_column)
    let right = slice(line, ed.cursor_column, length(line))
    var inscope parts <- split(normalized, "\n")
    if (length(parts) == 1) {
        ed.lines[ed.cursor_line] = left + parts[0] + right
        ed.cursor_column = length(left) + length(parts[0])
    } else {
        ed.lines |> reserve(length(ed.lines) + length(parts) - 1)
        ed.lines[ed.cursor_line] = left + parts[0]
        for (index in range(1, length(parts) - 1)) {
            ed.lines |> push(parts[index], ed.cursor_line + index)
        }
        let last = length(parts) - 1
        ed.cursor_line += last
        ed.cursor_column = length(parts[last])
        ed.lines |> push(parts[last] + right, ed.cursor_line)
    }
    touch_edit(ed)
}

def public text_edit_newline(var ed : TextEditState) {
    // E4 grows auto-indent here; E1 splits the line verbatim.
    text_edit_insert(ed, "\n")
}

def public text_edit_backspace(var ed : TextEditState) {
    ensure_line(ed)
    if (delete_selection(ed)) {
        touch_edit(ed)
        return
    }
    if (ed.cursor_column > 0) {
        let line = ed.lines[ed.cursor_line]
        let previous = grapheme_prev(line, ed.cursor_column)
        ed.lines[ed.cursor_line] = (slice(line, 0, previous)
            + slice(line, ed.cursor_column, length(line)))
        ed.cursor_column = previous
    } elif (ed.cursor_line > 0) {
        let previous_length = length(ed.lines[ed.cursor_line - 1])
        ed.lines[ed.cursor_line - 1] = (ed.lines[ed.cursor_line - 1]
            + ed.lines[ed.cursor_line])
        ed.lines |> erase(ed.cursor_line)
        ed.cursor_line--
        ed.cursor_column = previous_length
    } else {
        return
    }
    touch_edit(ed)
}

def public text_edit_delete(var ed : TextEditState) {
    ensure_line(ed)
    if (delete_selection(ed)) {
        touch_edit(ed)
        return
    }
    let line = ed.lines[ed.cursor_line]
    if (ed.cursor_column < length(line)) {
        let next = grapheme_next(line, ed.cursor_column)
        ed.lines[ed.cursor_line] = (slice(line, 0, ed.cursor_column)
            + slice(line, next, length(line)))
    } elif (ed.cursor_line < length(ed.lines) - 1) {
        ed.lines[ed.cursor_line] = line + ed.lines[ed.cursor_line + 1]
        ed.lines |> erase(ed.cursor_line + 1)
    } else {
        return
    }
    touch_edit(ed)
}

// ===== Cursor movement =====

def public text_edit_move_left(var ed : TextEditState; select : bool = false) {
    ensure_line(ed)
    if (!select && text_edit_has_selection(ed)) {
        // Plain arrow with a selection collapses to its edge without moving.
        let sel = text_edit_selection(ed)
        ed.cursor_line = sel.start_line
        ed.cursor_column = sel.start_column
        text_edit_clear_selection(ed)
    } else {
        begin_move(ed, select)
        if (ed.cursor_column > 0) {
            ed.cursor_column = grapheme_prev(ed.lines[ed.cursor_line], ed.cursor_column)
        } elif (ed.cursor_line > 0) {
            ed.cursor_line--
            ed.cursor_column = length(ed.lines[ed.cursor_line])
        }
    }
    ed.preferred_column = -1
    touch_move(ed)
}

def public text_edit_move_right(var ed : TextEditState; select : bool = false) {
    ensure_line(ed)
    if (!select && text_edit_has_selection(ed)) {
        let sel = text_edit_selection(ed)
        ed.cursor_line = sel.end_line
        ed.cursor_column = sel.end_column
        text_edit_clear_selection(ed)
    } else {
        begin_move(ed, select)
        if (ed.cursor_column < length(ed.lines[ed.cursor_line])) {
            ed.cursor_column = grapheme_next(ed.lines[ed.cursor_line], ed.cursor_column)
        } elif (ed.cursor_line < length(ed.lines) - 1) {
            ed.cursor_line++
            ed.cursor_column = 0
        }
    }
    ed.preferred_column = -1
    touch_move(ed)
}

def public text_edit_move_vertical(var ed : TextEditState; delta_lines : int;
                                   select : bool = false) {
    //! Up/down/page moves. The grapheme column is remembered across
    //! consecutive vertical moves so short lines don't erode the target.
    ensure_line(ed)
    if (!select && text_edit_has_selection(ed)) {
        // Collapse to the direction's edge, then move from there.
        let sel = text_edit_selection(ed)
        ed.cursor_line = delta_lines < 0 ? sel.start_line : sel.end_line
        ed.cursor_column = delta_lines < 0 ? sel.start_column : sel.end_column
        ed.preferred_column = -1
        text_edit_clear_selection(ed)
    }
    begin_move(ed, select)
    let target = clamp(ed.cursor_line + delta_lines, 0, length(ed.lines) - 1)
    if (target == ed.cursor_line) {
        // First/last line: clamp to the line boundary like every editor.
        ed.cursor_column = delta_lines < 0 ? 0 : length(ed.lines[ed.cursor_line])
        ed.preferred_column = -1
    } else {
        let preferred = (ed.preferred_column >= 0 ? ed.preferred_column
            : grapheme_index_of(ed.lines[ed.cursor_line], ed.cursor_column))
        ed.cursor_line = target
        ed.cursor_column = grapheme_column_at(ed.lines[target], preferred)
        ed.preferred_column = preferred
    }
    touch_move(ed)
}

def public text_edit_move_home(var ed : TextEditState; select : bool = false) {
    //! Smart home: to the first non-blank column; if already there, column 0.
    ensure_line(ed)
    begin_move(ed, select)
    let line = ed.lines[ed.cursor_line]
    var first_non_blank = 0
    peek_data(line) $(bytes) {
        while (first_non_blank < length(bytes)
                && (int(bytes[first_non_blank]) == ' '
                    || int(bytes[first_non_blank]) == '\t')) {
            first_non_blank++
        }
    }
    if (first_non_blank >= length(line)) {
        first_non_blank = 0
    }
    ed.cursor_column = ed.cursor_column == first_non_blank ? 0 : first_non_blank
    ed.preferred_column = -1
    touch_move(ed)
}

def public text_edit_move_end(var ed : TextEditState; select : bool = false) {
    ensure_line(ed)
    begin_move(ed, select)
    ed.cursor_column = length(ed.lines[ed.cursor_line])
    ed.preferred_column = -1
    touch_move(ed)
}

def public text_edit_move_document_start(var ed : TextEditState; select : bool = false) {
    ensure_line(ed)
    begin_move(ed, select)
    ed.cursor_line = 0
    ed.cursor_column = 0
    ed.preferred_column = -1
    touch_move(ed)
}

def public text_edit_move_document_end(var ed : TextEditState; select : bool = false) {
    ensure_line(ed)
    begin_move(ed, select)
    ed.cursor_line = length(ed.lines) - 1
    ed.cursor_column = length(ed.lines[ed.cursor_line])
    ed.preferred_column = -1
    touch_move(ed)
}

def public text_edit_set_cursor(var ed : TextEditState; line, column : int) {
    ensure_line(ed)
    text_edit_clear_selection(ed)
    ed.cursor_line = clamp(line, 0, length(ed.lines) - 1)
    ed.cursor_column = clamp(column, 0, length(ed.lines[ed.cursor_line]))
    ed.preferred_column = -1
    touch_move(ed)
}

// ===== Word boundaries =====

def private byte_word_class(b : int) : int {
    // ONE predicate for all four word ops: 0 ws, 1 word, 2 separator
    return 0 if (b == ' ' || b == '\t')
    return 1 if (is_alpha(b) || is_number(b) || b == '_' || b >= 0x80)
    return 2
}

def private word_start_left(line : string; column : int; fast : bool) : int {
    // fast = VS Code Ctrl+Left: a LONE separator between words gets no stop
    var at = column
    peek_data(line) $(bytes) {
        while (at > 0 && byte_word_class(int(bytes[at - 1])) == 0) {
            at--
        }
        if (at > 0) {
            let cls = byte_word_class(int(bytes[at - 1]))
            let run_end = at
            while (at > 0 && byte_word_class(int(bytes[at - 1])) == cls) {
                at--
            }
            if (fast && cls == 2 && run_end - at == 1
                    && run_end < length(bytes)
                    && byte_word_class(int(bytes[run_end])) == 1
                    && at > 0 && byte_word_class(int(bytes[at - 1])) == 1) {
                while (at > 0 && byte_word_class(int(bytes[at - 1])) == 1) {
                    at--
                }
            }
        }
    }
    return at
}

def private word_end_right(line : string; column : int; fast : bool) : int {
    // VS Code Ctrl+Right lands on word ENDS (spec fork 1); mirrored fast skip
    var at = column
    peek_data(line) $(bytes) {
        let n = length(bytes)
        while (at < n && byte_word_class(int(bytes[at])) == 0) {
            at++
        }
        if (at < n) {
            let cls = byte_word_class(int(bytes[at]))
            let run_start = at
            while (at < n && byte_word_class(int(bytes[at])) == cls) {
                at++
            }
            if (fast && cls == 2 && at - run_start == 1
                    && at < n && byte_word_class(int(bytes[at])) == 1
                    && run_start > 0
                    && byte_word_class(int(bytes[run_start - 1])) == 1) {
                while (at < n && byte_word_class(int(bytes[at])) == 1) {
                    at++
                }
            }
        }
    }
    return at
}

def private word_run_at(line : string; column : int) : int2 {
    // same-class run containing column — double-click word selection
    let line_length = length(line)
    return int2(0, 0) if (line_length == 0)
    var run = int2(0, line_length)
    peek_data(line) $(bytes) {
        let sample = clamp(column, 0, line_length - 1)
        let cls = byte_word_class(int(bytes[sample]))
        var start = sample
        while (start > 0 && byte_word_class(int(bytes[start - 1])) == cls) {
            start--
        }
        var end_ = sample
        while (end_ < line_length && byte_word_class(int(bytes[end_])) == cls) {
            end_++
        }
        run = int2(start, end_)
    }
    return run
}

def public text_edit_move_word_left(var ed : TextEditState; select : bool = false) {
    ensure_line(ed)
    begin_move(ed, select)
    if (ed.cursor_column == 0) {
        if (ed.cursor_line > 0) {
            ed.cursor_line--
            ed.cursor_column = length(ed.lines[ed.cursor_line])
        }
    } else {
        ed.cursor_column = word_start_left(ed.lines[ed.cursor_line],
            ed.cursor_column, true)
    }
    ed.preferred_column = -1
    touch_move(ed)
}

def public text_edit_move_word_right(var ed : TextEditState; select : bool = false) {
    ensure_line(ed)
    begin_move(ed, select)
    if (ed.cursor_column >= length(ed.lines[ed.cursor_line])) {
        if (ed.cursor_line < length(ed.lines) - 1) {
            ed.cursor_line++
            ed.cursor_column = 0
        }
    } else {
        ed.cursor_column = word_end_right(ed.lines[ed.cursor_line],
            ed.cursor_column, true)
    }
    ed.preferred_column = -1
    touch_move(ed)
}

def public text_edit_delete_word_left(var ed : TextEditState) {
    //! Ctrl+Backspace. Whitespace heuristic: a run of 2+ spaces left of the
    //! caret deletes alone; a single space goes with the word before it.
    ensure_line(ed)
    if (delete_selection(ed)) {
        touch_edit(ed)
        return
    }
    if (ed.cursor_column == 0) {
        text_edit_backspace(ed)
        return
    }
    let line = ed.lines[ed.cursor_line]
    var ws_start = ed.cursor_column
    peek_data(line) $(bytes) {
        while (ws_start > 0 && byte_word_class(int(bytes[ws_start - 1])) == 0) {
            ws_start--
        }
    }
    let target = (ed.cursor_column - ws_start >= 2 ? ws_start
        : word_start_left(line, ed.cursor_column, false))
    ed.lines[ed.cursor_line] = (slice(line, 0, target)
        + slice(line, ed.cursor_column, length(line)))
    ed.cursor_column = target
    touch_edit(ed)
}

def public text_edit_delete_word_right(var ed : TextEditState) {
    //! Ctrl+Delete — the forward mirror.
    ensure_line(ed)
    if (delete_selection(ed)) {
        touch_edit(ed)
        return
    }
    let line = ed.lines[ed.cursor_line]
    if (ed.cursor_column >= length(line)) {
        text_edit_delete(ed)
        return
    }
    var ws_end = ed.cursor_column
    peek_data(line) $(bytes) {
        while (ws_end < length(bytes) && byte_word_class(int(bytes[ws_end])) == 0) {
            ws_end++
        }
    }
    let target = (ws_end - ed.cursor_column >= 2 ? ws_end
        : word_end_right(line, ed.cursor_column, false))
    ed.lines[ed.cursor_line] = (slice(line, 0, ed.cursor_column)
        + slice(line, target, length(line)))
    touch_edit(ed)
}

// ===== The widget =====

def private edit_hit_column(line : string; local_x : float; var font : ImFont?;
                            font_size, cell_width : float; tab_cells : int) : int {
    return 0 if (local_x <= 0.0f || empty(line))
    var x = 0.0f
    var result = length(line)
    var inscope graphemes <- text_graphemes(line)
    for (grapheme in graphemes) {
        let width = (grapheme.first_codepoint == 0x0009u
            ? float(max(tab_cells, 1)) * cell_width
            : CalcTextSizeForFontRange(font, font_size, line, length(line),
                grapheme.source.start_byte, grapheme.source.end_byte).x)
        if (local_x < x + width * 0.5f) {
            result = grapheme.source.start_byte
            break
        }
        if (local_x < x + width) {
            result = grapheme.source.end_byte
            break
        }
        x += width
    }
    return result
}

def private refresh_layout_cache(var ed : TextEditState) {
    return if (ed.layout_revision == ed.revision)
    ed.layout_revision = ed.revision
    ed.line_count = length(ed.lines)
    var longest = 0
    for (line in ed.lines) {
        longest = max(longest, length(line))
    }
    ed.max_line_bytes = longest
}

def private route_editor_keys(var ed : TextEditState; page_rows : int) { // nolint:STYLE037
    // interim hardwired keymap — migrates to imgui_commands in the registry step
    let io & = unsafe(GetIO())
    let select = io.KeyShift
    if (io.KeyCtrl && !io.KeyAlt) {
        if (IsKeyPressed(ImGuiKey.LeftArrow, true)) {
            text_edit_move_word_left(ed, select)
        }
        if (IsKeyPressed(ImGuiKey.RightArrow, true)) {
            text_edit_move_word_right(ed, select)
        }
        if (IsKeyPressed(ImGuiKey.Home, true)) {
            text_edit_move_document_start(ed, select)
        }
        if (IsKeyPressed(ImGuiKey.End, true)) {
            text_edit_move_document_end(ed, select)
        }
        if (IsKeyPressed(ImGuiKey.Backspace, true)) {
            text_edit_delete_word_left(ed)
        }
        if (IsKeyPressed(ImGuiKey.Delete, true)) {
            text_edit_delete_word_right(ed)
        }
        if (IsKeyPressed(ImGuiKey.A, false)) {
            text_edit_select_all(ed)
        }
    } elif (!io.KeyAlt) {
        if (IsKeyPressed(ImGuiKey.LeftArrow, true)) {
            text_edit_move_left(ed, select)
        }
        if (IsKeyPressed(ImGuiKey.RightArrow, true)) {
            text_edit_move_right(ed, select)
        }
        if (IsKeyPressed(ImGuiKey.UpArrow, true)) {
            text_edit_move_vertical(ed, -1, select)
        }
        if (IsKeyPressed(ImGuiKey.DownArrow, true)) {
            text_edit_move_vertical(ed, 1, select)
        }
        if (IsKeyPressed(ImGuiKey.PageUp, true)) {
            text_edit_move_vertical(ed, -max(page_rows, 1), select)
        }
        if (IsKeyPressed(ImGuiKey.PageDown, true)) {
            text_edit_move_vertical(ed, max(page_rows, 1), select)
        }
        if (IsKeyPressed(ImGuiKey.Home, true)) {
            text_edit_move_home(ed, select)
        }
        if (IsKeyPressed(ImGuiKey.End, true)) {
            text_edit_move_end(ed, select)
        }
        if (IsKeyPressed(ImGuiKey.Enter, true) || IsKeyPressed(ImGuiKey.KeypadEnter, true)) {
            text_edit_newline(ed)
        }
        if (IsKeyPressed(ImGuiKey.Backspace, true)) {
            text_edit_backspace(ed)
        }
        if (IsKeyPressed(ImGuiKey.Delete, true)) {
            text_edit_delete(ed)
        }
        if (IsKeyPressed(ImGuiKey.Tab, true)) {
            text_edit_insert(ed, "\t")
        }
        if (IsKeyPressed(ImGuiKey.Escape, false) && text_edit_has_selection(ed)) {
            text_edit_clear_selection(ed)
        }
    }
    if (!io.KeyCtrl && !io.KeyAlt) {
        for (value in io.InputQueueCharacters) {
            let codepoint = int(value)
            if (codepoint >= 0x20 && codepoint != 0x7f && codepoint <= 0x10ffff) {
                text_edit_insert(ed, codepoint_utf8(codepoint))
                ed.input_revision++
            }
        }
    }
}

struct private EditMetrics {
    zoom : float = 1.0f
    font_size : float = 0.0f
    line_height : float = 1.0f
    cell_width : float = 1.0f
    number_width : float = 0.0f
    gutter_space : float = 0.0f
    origin : float2
    extent : float2
    view_height : float = 0.0f
}

def private compute_edit_metrics(ed : TextEditState const;
                                 style : TextSourceViewStyle const;
                                 var font : ImFont?) : EditMetrics {
    let zoom = clamp(style.zoom, 0.5f, 3.0f)
    let font_size = (style.fonts.size_px > 0.0f
        ? style.fonts.size_px : GetFontSize()) * zoom
    let line_height = max(font_size,
        GetTextLineHeight() * font_size / max(GetFontSize(), 1.0f))
    let cell_width = max(1.0f,
        CalcTextSizeForFontRange(font, font_size, "M", 1, 0, 1).x)
    let line_count = length(ed.lines)
    let number_text = "{line_count}"
    let number_width = CalcTextSizeForFontRange(font, font_size,
        number_text, length(number_text), 0, length(number_text)).x
    let gutter_space = number_width + cell_width * 2.0f
    let text_width = max(GetContentRegionAvail().x - gutter_space,
        float(ed.max_line_bytes) * cell_width + cell_width)
    return EditMetrics(zoom = zoom, font_size = font_size,
        line_height = line_height, cell_width = cell_width,
        number_width = number_width, gutter_space = gutter_space,
        origin = GetCursorScreenPos(),
        extent = float2(gutter_space + text_width, float(line_count) * line_height),
        view_height = GetWindowHeight())
}

def private edit_hit_position(ed : TextEditState const; m : EditMetrics;
                              style : TextSourceViewStyle const;
                              var font : ImFont?) : int2 {
    let local = GetMousePos() - m.origin
    let row = clamp(int(floor(local.y / m.line_height)), 0, length(ed.lines) - 1)
    let column = edit_hit_column(ed.lines[row], local.x - m.gutter_space,
        font, m.font_size, m.cell_width, style.tab_cells)
    return int2(row, column)
}

def private handle_edit_mouse(var ed : TextEditState; m : EditMetrics;
                              style : TextSourceViewStyle const; hovered : bool;
                              var font : ImFont?) {
    let io & = unsafe(GetIO())
    if (hovered && IsMouseClicked(ImGuiMouseButton.Left, false)) {
        ed.focused = true
        let hit = edit_hit_position(ed, m, style, font)
        let clicks = GetMouseClickedCount(ImGuiMouseButton.Left)
        if (io.KeyShift) {
            // Shift+click extends from the current caret (or existing anchor).
            if (ed.anchor_line < 0) {
                ed.anchor_line = ed.cursor_line
                ed.anchor_column = ed.cursor_column
            }
            ed.cursor_line = hit.x
            ed.cursor_column = hit.y
        } elif (clicks >= 3) {
            // Triple-click: whole line including its newline.
            ed.anchor_line = hit.x
            ed.anchor_column = 0
            if (hit.x < length(ed.lines) - 1) {
                ed.cursor_line = hit.x + 1
                ed.cursor_column = 0
            } else {
                ed.cursor_line = hit.x
                ed.cursor_column = length(ed.lines[hit.x])
            }
        } elif (clicks == 2) {
            let run = word_run_at(ed.lines[hit.x], hit.y)
            ed.anchor_line = hit.x
            ed.anchor_column = run.x
            ed.cursor_line = hit.x
            ed.cursor_column = run.y
        } else {
            ed.cursor_line = hit.x
            ed.cursor_column = hit.y
            // Anchor arms at the click point; dragging grows the selection.
            ed.anchor_line = hit.x
            ed.anchor_column = hit.y
            ed.selection_dragging = true
        }
        ed.preferred_column = -1
        ed.blink_time = 0.0f
    } elif (ed.selection_dragging && IsMouseDown(ImGuiMouseButton.Left)) {
        let hit = edit_hit_position(ed, m, style, font)
        ed.cursor_line = hit.x
        ed.cursor_column = hit.y
    } elif (!hovered && IsMouseClicked(ImGuiMouseButton.Left, false)) {
        ed.focused = false
    }
    if (ed.selection_dragging && IsMouseReleased(ImGuiMouseButton.Left)) {
        ed.selection_dragging = false
        if (!text_edit_has_selection(ed)) {
            text_edit_clear_selection(ed)
        }
    }
}

def private follow_edit_cursor(var ed : TextEditState; m : EditMetrics;
                               tab_cells : int; var font : ImFont?) {
    //! Scroll-follow before drawing so the caret never renders off-view.
    return if (ed.follow_revision == ed.followed_revision)
    ed.followed_revision = ed.follow_revision
    let scroll_y = GetScrollY()
    let widget_top = m.origin.y - GetWindowPos().y + scroll_y
    let cursor_top = widget_top + float(ed.cursor_line) * m.line_height
    if (cursor_top < scroll_y) {
        SetScrollY(max(cursor_top, 0.0f))
    } elif (cursor_top + m.line_height > scroll_y + m.view_height) {
        SetScrollY(cursor_top + m.line_height - m.view_height)
    }
    let caret_x = text_source_range_width(ed.lines[ed.cursor_line], 0,
        ed.cursor_column, font, m.font_size, m.cell_width, tab_cells)
    let scroll_x = GetScrollX()
    let view_width = GetWindowWidth()
    if (caret_x < scroll_x) {
        SetScrollX(max(caret_x - m.cell_width * 4.0f, 0.0f))
    } elif (m.gutter_space + caret_x > scroll_x + view_width - m.cell_width * 2.0f) {
        SetScrollX(m.gutter_space + caret_x - view_width + m.cell_width * 8.0f)
    }
}

def private draw_edit_content(var ed : TextEditState; m : EditMetrics;
                              style : TextSourceViewStyle const;
                              var font : ImFont?) {
    var dl = GetWindowDrawList()
    let draw_count = length(ed.lines)
    let first_visible = clamp(
        int(floor((GetWindowPos().y - m.origin.y) / m.line_height)),
        0, draw_count - 1)
    let last_visible = clamp(
        first_visible + int(ceil(m.view_height / m.line_height)) + 1,
        0, draw_count - 1)
    let io & = unsafe(GetIO())
    ed.blink_time += io.DeltaTime
    let has_selection = text_edit_has_selection(ed)
    let sel = text_edit_selection(ed)
    for (row in range(first_visible, last_visible + 1)) {
        let row_origin = float2(m.origin.x, m.origin.y + float(row) * m.line_height)
        if (row == ed.cursor_line && ed.focused && !has_selection) {
            *dl |> AddRectFilled(row_origin,
                row_origin + float2(m.extent.x, m.line_height),
                GetColorU32(style.current_line_color), 0.0f, ImDrawFlags.None)
        }
        if (has_selection && row >= sel.start_line && row <= sel.end_line) {
            let row_line = ed.lines[row]
            let from_column = row == sel.start_line ? sel.start_column : 0
            let to_column = row == sel.end_line ? sel.end_column : length(row_line)
            let x0 = (m.gutter_space + text_source_range_width(row_line, 0,
                from_column, font, m.font_size, m.cell_width, style.tab_cells))
            var x1 = (m.gutter_space + text_source_range_width(row_line, 0,
                to_column, font, m.font_size, m.cell_width, style.tab_cells))
            if (row < sel.end_line) {
                x1 += m.cell_width * 0.5f // the selected newline shows as a nub
            }
            *dl |> AddRectFilled(
                float2(row_origin.x + x0, row_origin.y),
                float2(row_origin.x + max(x1, x0 + 1.0f), row_origin.y + m.line_height),
                GetColorU32(style.selection_color), 0.0f, ImDrawFlags.None)
        }
        let row_number = "{row + 1}"
        let row_number_width = CalcTextSizeForFontRange(font, m.font_size,
            row_number, length(row_number), 0, length(row_number)).x
        *dl |> AddTextRange(font, m.font_size,
            float2(row_origin.x + m.number_width - row_number_width, row_origin.y),
            GetColorU32(style.line_number_color),
            row_number, length(row_number), 0, length(row_number))
        let line = ed.lines[row]
        if (!empty(line)) {
            let _ = text_source_draw_range(dl, font, m.font_size,
                float2(row_origin.x + m.gutter_space, row_origin.y),
                style.text_color, line, 0, length(line),
                m.cell_width, style.tab_cells)
        }
    }
    if (ed.focused && ed.cursor_line >= first_visible && ed.cursor_line <= last_visible
            && fmod(ed.blink_time, 1.06f) < 0.53f) {
        let caret_x = (m.origin.x + m.gutter_space
            + text_source_range_width(ed.lines[ed.cursor_line], 0, ed.cursor_column,
                font, m.font_size, m.cell_width, style.tab_cells))
        let caret_y = m.origin.y + float(ed.cursor_line) * m.line_height
        *dl |> AddRectFilled(float2(caret_x, caret_y),
            float2(caret_x + max(1.0f, 1.5f * m.zoom), caret_y + m.line_height),
            GetColorU32(style.caret_color), 0.0f, ImDrawFlags.None)
    }
}

def public text_source_edit(id : string; var ed : TextEditState;
                            var style : TextSourceViewStyle) : TextEditResult {
    //! Draw + drive the editor as the sole content of a scrolling child window
    //! (the host owns the child, like `text_source_view`). Returns what changed
    //! this frame; the durable state lives in `ed`.
    var result = TextEditResult()
    ensure_line(ed)
    refresh_layout_cache(ed)
    let revision_before = ed.revision
    let follow_before = ed.follow_revision
    var fallback = GetFont()
    var font = style.fonts.monospace != null ? style.fonts.monospace : fallback
    let m = compute_edit_metrics(ed, style, font)
    PushID(id)
    InvisibleButton("##edit", m.extent)
    unsafe {
        register_clickable_state_bbox(id, "##edit", "text_edit", addr(ed),
            reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<TextEditState>)))
    }
    let hovered = IsItemHovered(ImGuiHoveredFlags.None)
    if (IsItemFocused()) {
        ed.focused = true
    }
    handle_edit_mouse(ed, m, style, hovered, font)
    let page_rows = max(1, int(floor(m.view_height / m.line_height)) - 1)
    ed.visible_rows = page_rows
    // Keyboard arbitration: several editors may each remember focused=true
    // (a click elsewhere is the only clearer), so the focused WINDOW picks
    // which one hears keys; an active text input (find bar) mutes them all.
    let io & = unsafe(GetIO())
    let window_focused = IsWindowFocused(ImGuiFocusedFlags.RootAndChildWindows)
    if (ed.focused && window_focused && !io.WantTextInput) {
        route_editor_keys(ed, page_rows)
    }
    follow_edit_cursor(ed, m, style.tab_cells, font)
    // Recount after input: an edit above may have grown or shrunk the buffer,
    // and drawing against the pre-edit count indexes past a line join.
    refresh_layout_cache(ed)
    draw_edit_content(ed, m, style, font)
    PopID()
    result.focused = ed.focused
    result.edited = ed.revision != revision_before
    result.cursor_moved = ed.follow_revision != follow_before
    return result
}
