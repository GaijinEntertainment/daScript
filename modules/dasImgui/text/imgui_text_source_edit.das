options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_text_source_edit shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_text_source_view public
require imgui/imgui_text_flow public
require clipboard/clipboard
require math
require strings
require daslib/strings_boost

//! Editing core + widget: an `array<string>` line buffer with a UTF-8-aware
//! (line, byte_column) cursor, rendered virtualised inside a scrolling child.
//! Buffer operations are pure (no ImGui) so tests drive them headless; the
//! widget owns focus, typed input, and scroll-follow. Selection, clipboard,
//! and undo arrive in the next arc steps.

enum public TextEditUndoKind {
    other
    insert_typing
    insert_space_first
    insert_space_run
    delete_back
    delete_forward
}

struct public TextEditUndoOp {
    //! One reversible edit: at (line, column) `removed` became `inserted`.
    line : int
    column : int
    removed : string
    inserted : string
}

struct public TextEditUndoUnit {
    ops : array<TextEditUndoOp>
    kind : TextEditUndoKind = TextEditUndoKind.other
    before_cursor_line : int
    before_cursor_column : int
    before_anchor_line : int = -1
    before_anchor_column : int
    after_cursor_line : int
    after_cursor_column : int
    //! Where the last edit left the cursor — the adjacency test (an edit
    //! elsewhere starts a new unit, so caret jumps break typing runs).
    end_line : int
    end_column : int
}

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
    //! Insert/overtype mode: typed characters replace the next grapheme
    //! within the line (never the newline); paste still inserts.
    overtype : bool = false
    undo_stack : array<TextEditUndoUnit>
    redo_stack : array<TextEditUndoUnit>
    //! True while the newest undo unit still accepts coalescing appends.
    undo_open : bool = false
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
    delete ed.undo_stack
    delete ed.redo_stack
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
    text_edit_clear_selection(ed)
    // Wholesale replacement clears history (spec: SetText resets, an
    // incremental programmatic edit participates).
    delete ed.undo_stack
    delete ed.redo_stack
    ed.undo_open = false
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

def public text_edit_range_text(ed : TextEditState const;
                                span : TextEditRange) : string {
    //! Text covered by an ordered range, with LF newlines.
    if (span.start_line == span.end_line) {
        return slice(ed.lines[span.start_line], span.start_column, span.end_column)
    }
    return build_string() $(var writer) {
        writer |> write(slice(ed.lines[span.start_line], span.start_column,
            length(ed.lines[span.start_line])))
        for (line in range(span.start_line + 1, span.end_line)) {
            writer |> write("\n")
            writer |> write(ed.lines[line])
        }
        writer |> write("\n")
        writer |> write(slice(ed.lines[span.end_line], 0, span.end_column))
    }
}

def public text_edit_selected_text(ed : TextEditState const) : string {
    //! Selected text with LF newlines ("" when no selection).
    return "" if (!text_edit_has_selection(ed))
    return text_edit_range_text(ed, text_edit_selection(ed))
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

// ===== Buffer primitives (no undo recording) =====

def private text_end_position(line, column : int; text : string) : int2 {
    // end of LF text laid down at (line, column)
    let breaks = count(text, "\n")
    return int2(line, column + length(text)) if (breaks == 0)
    let last_break = rfind(text, "\n")
    return int2(line + breaks, length(text) - last_break - 1)
}

def private remove_range_raw(var ed : TextEditState; span : TextEditRange) {
    ed.lines[span.start_line] = (slice(ed.lines[span.start_line], 0, span.start_column)
        + slice(ed.lines[span.end_line], span.end_column, length(ed.lines[span.end_line])))
    if (span.end_line > span.start_line) {
        ed.lines |> erase(span.start_line + 1, span.end_line - span.start_line)
    }
}

def private insert_raw(var ed : TextEditState; line, column : int;
                       text : string) : int2 {
    return int2(line, column) if (empty(text))
    let current = ed.lines[line]
    let left = slice(current, 0, column)
    let right = slice(current, column, length(current))
    var inscope parts <- split(text, "\n")
    if (length(parts) == 1) {
        ed.lines[line] = left + parts[0] + right
        return int2(line, column + length(parts[0]))
    }
    ed.lines |> reserve(length(ed.lines) + length(parts) - 1)
    ed.lines[line] = left + parts[0]
    for (index in range(1, length(parts) - 1)) {
        ed.lines |> push(parts[index], line + index)
    }
    let last = length(parts) - 1
    ed.lines |> push(parts[last] + right, line + last)
    return int2(line + last, length(parts[last]))
}

// ===== Undo journal =====

def public text_edit_push_undo_stop(var ed : TextEditState) {
    //! Fence: the next edit starts a new undo unit. Every compound command
    //! (paste, replace-all, line ops) brackets itself with this, so it undoes
    //! as ONE step — never by heuristics.
    ed.undo_open = false
}

def public text_edit_can_undo(ed : TextEditState const) : bool {
    return !empty(ed.undo_stack)
}

def public text_edit_can_redo(ed : TextEditState const) : bool {
    return !empty(ed.redo_stack)
}

def private space_kind_for(ed : TextEditState const; text : string) : TextEditUndoKind {
    return TextEditUndoKind.insert_typing if (text != " ")
    if (ed.undo_open && !empty(ed.undo_stack)) {
        let last_kind = ed.undo_stack[length(ed.undo_stack) - 1].kind
        if (last_kind == TextEditUndoKind.insert_space_first
                || last_kind == TextEditUndoKind.insert_space_run) {
            return TextEditUndoKind.insert_space_run
        }
    }
    return TextEditUndoKind.insert_space_first
}

def private kind_bucket(kind : TextEditUndoKind) : int {
    if (kind == TextEditUndoKind.insert_space_first
            || kind == TextEditUndoKind.insert_space_run) {
        return 100 // spaces share one bucket
    }
    return int(kind)
}

def private breaks_run(previous, current : TextEditUndoKind) : bool {
    // first space after a word does not break: undo units are word-sized
    return false if (previous == TextEditUndoKind.insert_space_first
        && current == TextEditUndoKind.insert_typing)
    return kind_bucket(previous) != kind_bucket(current)
}

def private commit_edit(var ed : TextEditState; kind : TextEditUndoKind;
                        line, column : int; removed, inserted : string) {
    // THE one recorded mutation: (removed -> inserted) at (line, column)
    delete ed.redo_stack
    var append_to_open = false
    if (ed.undo_open && !empty(ed.undo_stack)
            && kind != TextEditUndoKind.other) {
        let last & = unsafe(ed.undo_stack[length(ed.undo_stack) - 1])
        if (last.kind != TextEditUndoKind.other && !breaks_run(last.kind, kind)) {
            // delete-back removals END at the caret; everything else STARTS there
            let touch = (kind == TextEditUndoKind.delete_back
                ? text_end_position(line, column, removed) : int2(line, column))
            append_to_open = (touch.x == last.end_line && touch.y == last.end_column)
        }
    }
    if (!append_to_open) {
        var unit = TextEditUndoUnit(kind = kind,
            before_cursor_line = ed.cursor_line, before_cursor_column = ed.cursor_column,
            before_anchor_line = ed.anchor_line, before_anchor_column = ed.anchor_column)
        ed.undo_stack |> emplace(unit)
    }
    let last_index = length(ed.undo_stack) - 1
    ed.undo_stack[last_index].ops |> push(TextEditUndoOp(line = line, column = column,
        removed = removed, inserted = inserted))
    if (!empty(removed)) {
        let removed_end = text_end_position(line, column, removed)
        remove_range_raw(ed, TextEditRange(start_line = line, start_column = column,
            end_line = removed_end.x, end_column = removed_end.y))
    }
    let end_position = insert_raw(ed, line, column, inserted)
    ed.cursor_line = end_position.x
    ed.cursor_column = end_position.y
    text_edit_clear_selection(ed)
    var last & = unsafe(ed.undo_stack[last_index])
    last.kind = kind
    last.end_line = ed.cursor_line
    last.end_column = ed.cursor_column
    last.after_cursor_line = ed.cursor_line
    last.after_cursor_column = ed.cursor_column
    ed.undo_open = true
    touch_edit(ed)
}

def private restore_unit_cursor(var ed : TextEditState;
                                line, column, anchor_line, anchor_column : int) {
    ed.cursor_line = clamp(line, 0, length(ed.lines) - 1)
    ed.cursor_column = clamp(column, 0, length(ed.lines[ed.cursor_line]))
    ed.anchor_line = anchor_line
    ed.anchor_column = anchor_column
    ed.preferred_column = -1
}

def public text_edit_undo(var ed : TextEditState) : bool {
    //! Revert the newest undo unit; restores the pre-edit cursor + selection.
    ed.undo_open = false
    return false if (empty(ed.undo_stack))
    let unit_index = length(ed.undo_stack) - 1
    var unit <- ed.undo_stack[unit_index]
    ed.undo_stack |> erase(unit_index)
    var op_index = length(unit.ops) - 1
    while (op_index >= 0) {
        let op & = unsafe(unit.ops[op_index])
        if (!empty(op.inserted)) {
            let inserted_end = text_end_position(op.line, op.column, op.inserted)
            remove_range_raw(ed, TextEditRange(start_line = op.line,
                start_column = op.column,
                end_line = inserted_end.x, end_column = inserted_end.y))
        }
        if (!empty(op.removed)) {
            let _ = insert_raw(ed, op.line, op.column, op.removed)
        }
        op_index--
    }
    restore_unit_cursor(ed, unit.before_cursor_line, unit.before_cursor_column,
        unit.before_anchor_line, unit.before_anchor_column)
    ed.redo_stack |> emplace(unit)
    ed.dirty = true
    ed.revision++
    ed.follow_revision++
    ed.blink_time = 0.0f
    return true
}

def public text_edit_redo(var ed : TextEditState) : bool {
    ed.undo_open = false
    return false if (empty(ed.redo_stack))
    let unit_index = length(ed.redo_stack) - 1
    var unit <- ed.redo_stack[unit_index]
    ed.redo_stack |> erase(unit_index)
    for (op in unit.ops) {
        if (!empty(op.removed)) {
            let removed_end = text_end_position(op.line, op.column, op.removed)
            remove_range_raw(ed, TextEditRange(start_line = op.line,
                start_column = op.column,
                end_line = removed_end.x, end_column = removed_end.y))
        }
        let _ = insert_raw(ed, op.line, op.column, op.inserted)
    }
    restore_unit_cursor(ed, unit.after_cursor_line, unit.after_cursor_column, -1, 0)
    ed.undo_stack |> emplace(unit)
    ed.dirty = true
    ed.revision++
    ed.follow_revision++
    ed.blink_time = 0.0f
    return true
}

// ===== Clipboard =====

// Internal clipboard record: carries the line-mode flag the system clipboard
// cannot, and doubles as the paste source in headless runs. Paste compares the
// incoming text against it (the VS Code mechanism).
var private g_clip_text : string
var private g_clip_line_mode : bool = false

def public text_edit_clipboard_text() : string {
    //! Last text copied/cut by ANY editor in this context — the paste source
    //! when the system clipboard is unavailable (headless).
    return g_clip_text
}

def public text_edit_copy(var ed : TextEditState) : string {
    //! Copy verb: returns the text for the clipboard (the host writes the
    //! system clipboard). Empty selection copies the WHOLE line incl. its
    //! newline and arms line-mode paste.
    ensure_line(ed)
    var text : string
    if (text_edit_has_selection(ed)) {
        text = text_edit_selected_text(ed)
        g_clip_line_mode = false
    } else {
        text = ed.lines[ed.cursor_line] + "\n"
        g_clip_line_mode = true
    }
    g_clip_text = text
    return text
}

def public text_edit_cut(var ed : TextEditState) : string {
    //! Cut verb: copy + remove as one undo unit. Empty selection cuts the
    //! whole line including its newline.
    ensure_line(ed)
    var text : string
    if (text_edit_has_selection(ed)) {
        let sel = text_edit_selection(ed)
        text = text_edit_range_text(ed, sel)
        g_clip_line_mode = false
        g_clip_text = text
        commit_edit(ed, TextEditUndoKind.other, sel.start_line, sel.start_column,
            text, "")
        return text
    }
    let line = ed.cursor_line
    text = ed.lines[line] + "\n"
    g_clip_line_mode = true
    g_clip_text = text
    if (line < length(ed.lines) - 1) {
        commit_edit(ed, TextEditUndoKind.other, line, 0, text, "")
    } elif (line > 0) {
        // Last line: the removed newline sits BEFORE it.
        commit_edit(ed, TextEditUndoKind.other, line - 1,
            length(ed.lines[line - 1]), "\n" + ed.lines[line], "")
    } else {
        commit_edit(ed, TextEditUndoKind.other, 0, 0, ed.lines[0], "")
    }
    return text
}

def public text_edit_paste(var ed : TextEditState; text : string) {
    //! Paste verb; always ONE undo unit (kind `other` self-fences). A
    //! line-mode clip with no selection inserts at column 0 pushing the
    //! current line down, caret position preserved (the VS Code rule).
    return if (empty(text))
    ensure_line(ed)
    let normalized = replace(replace(text, "\r\n", "\n"), "\r", "\n")
    if (!text_edit_has_selection(ed) && g_clip_line_mode
            && normalized == g_clip_text) {
        let line = ed.cursor_line
        let column = ed.cursor_column
        commit_edit(ed, TextEditUndoKind.other, line, 0, "", normalized)
        ed.cursor_line = clamp(line + count(normalized, "\n"), 0,
            length(ed.lines) - 1)
        ed.cursor_column = clamp(column, 0, length(ed.lines[ed.cursor_line]))
    } elif (text_edit_has_selection(ed)) {
        let sel = text_edit_selection(ed)
        commit_edit(ed, TextEditUndoKind.other, sel.start_line, sel.start_column,
            text_edit_range_text(ed, sel), normalized)
    } else {
        commit_edit(ed, TextEditUndoKind.other, ed.cursor_line, ed.cursor_column,
            "", normalized)
    }
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
    let normalized = replace(replace(text, "\r\n", "\n"), "\r", "\n")
    if (text_edit_has_selection(ed)) {
        let sel = text_edit_selection(ed)
        commit_edit(ed, TextEditUndoKind.other, sel.start_line, sel.start_column,
            text_edit_range_text(ed, sel), normalized)
    } else {
        commit_edit(ed, space_kind_for(ed, normalized), ed.cursor_line,
            ed.cursor_column, "", normalized)
    }
}

def public text_edit_toggle_overtype(var ed : TextEditState) {
    ed.overtype = !ed.overtype
    ed.blink_time = 0.0f
}

def public text_edit_type(var ed : TextEditState; text : string) {
    //! One typed keystroke: like insert, but honoring overtype mode —
    //! replaces the next grapheme within the line, falls back to inserting
    //! at the line end. Selections are replaced normally either way.
    return if (empty(text))
    ensure_line(ed)
    if (ed.overtype && !text_edit_has_selection(ed) && find(text, "\n") < 0) {
        let line = ed.lines[ed.cursor_line]
        if (ed.cursor_column < length(line)) {
            let next = grapheme_next(line, ed.cursor_column)
            commit_edit(ed, space_kind_for(ed, text), ed.cursor_line,
                ed.cursor_column, slice(line, ed.cursor_column, next), text)
            return
        }
    }
    text_edit_insert(ed, text)
}

def public text_edit_newline(var ed : TextEditState) {
    // E4 grows auto-indent here. Enter fences the undo unit (spec: break
    // BEFORE, so the following typing run coalesces on the new line).
    text_edit_push_undo_stop(ed)
    text_edit_insert(ed, "\n")
}

def public text_edit_backspace(var ed : TextEditState) {
    ensure_line(ed)
    if (text_edit_has_selection(ed)) {
        let sel = text_edit_selection(ed)
        commit_edit(ed, TextEditUndoKind.delete_back, sel.start_line,
            sel.start_column, text_edit_range_text(ed, sel), "")
        return
    }
    if (ed.cursor_column > 0) {
        let line = ed.lines[ed.cursor_line]
        let previous = grapheme_prev(line, ed.cursor_column)
        commit_edit(ed, TextEditUndoKind.delete_back, ed.cursor_line, previous,
            slice(line, previous, ed.cursor_column), "")
    } elif (ed.cursor_line > 0) {
        commit_edit(ed, TextEditUndoKind.delete_back, ed.cursor_line - 1,
            length(ed.lines[ed.cursor_line - 1]), "\n", "")
    }
}

def public text_edit_delete(var ed : TextEditState) {
    ensure_line(ed)
    if (text_edit_has_selection(ed)) {
        let sel = text_edit_selection(ed)
        commit_edit(ed, TextEditUndoKind.delete_forward, sel.start_line,
            sel.start_column, text_edit_range_text(ed, sel), "")
        return
    }
    let line = ed.lines[ed.cursor_line]
    if (ed.cursor_column < length(line)) {
        let next = grapheme_next(line, ed.cursor_column)
        commit_edit(ed, TextEditUndoKind.delete_forward, ed.cursor_line,
            ed.cursor_column, slice(line, ed.cursor_column, next), "")
    } elif (ed.cursor_line < length(ed.lines) - 1) {
        commit_edit(ed, TextEditUndoKind.delete_forward, ed.cursor_line,
            ed.cursor_column, "\n", "")
    }
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
    if (text_edit_has_selection(ed) || ed.cursor_column == 0) {
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
    commit_edit(ed, TextEditUndoKind.other, ed.cursor_line, target,
        slice(line, target, ed.cursor_column), "")
}

def public text_edit_delete_word_right(var ed : TextEditState) {
    //! Ctrl+Delete — the forward mirror.
    ensure_line(ed)
    let line = ed.lines[ed.cursor_line]
    if (text_edit_has_selection(ed) || ed.cursor_column >= length(line)) {
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
    commit_edit(ed, TextEditUndoKind.other, ed.cursor_line, ed.cursor_column,
        slice(line, ed.cursor_column, target), "")
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

def private editor_copy_to_clipboard(var ed : TextEditState; use_system : bool) {
    let text = text_edit_copy(ed)
    if (use_system && !empty(text)) {
        let _ = clipboard_set_text(text)
    }
}

def private editor_cut_to_clipboard(var ed : TextEditState; use_system : bool) {
    let text = text_edit_cut(ed)
    if (use_system && !empty(text)) {
        let _ = clipboard_set_text(text)
    }
}

def private editor_paste_from_clipboard(var ed : TextEditState; use_system : bool) {
    var text = text_edit_clipboard_text()
    if (use_system) {
        var system_text = ""
        if (clipboard_get_text(system_text) == ClipboardStatus.ok
                && !empty(system_text)) {
            text = system_text
        }
    }
    text_edit_paste(ed, text)
}

def private nav_pressed(main_key, keypad_key : ImGuiKey; keypad_nav : bool) : bool {
    // every nav key answers for its numpad twin; twins mute while typing
    return (IsKeyPressed(main_key, true)
        || (keypad_nav && IsKeyPressed(keypad_key, true)))
}

def private route_editor_keys(var ed : TextEditState; page_rows : int; // nolint:STYLE037
                              use_system_clipboard : bool) {
    // interim hardwired keymap — migrates to imgui_commands in the registry step
    let io & = unsafe(GetIO())
    let select = io.KeyShift
    // chars drain FIRST: a char-delivering frame types, keypad nav mutes
    var typed_chars = false
    if (!io.KeyCtrl && !io.KeyAlt) {
        for (value in io.InputQueueCharacters) {
            let codepoint = int(value)
            if (codepoint >= 0x20 && codepoint != 0x7f && codepoint <= 0x10ffff) {
                text_edit_type(ed, codepoint_utf8(codepoint))
                ed.input_revision++
                typed_chars = true
            }
        }
    }
    let keypad_nav = !typed_chars
    if (io.KeyCtrl && !io.KeyAlt) {
        if (nav_pressed(ImGuiKey.LeftArrow, ImGuiKey.Keypad4, keypad_nav)) {
            text_edit_move_word_left(ed, select)
        }
        if (nav_pressed(ImGuiKey.RightArrow, ImGuiKey.Keypad6, keypad_nav)) {
            text_edit_move_word_right(ed, select)
        }
        if (nav_pressed(ImGuiKey.UpArrow, ImGuiKey.Keypad8, keypad_nav)) {
            text_edit_move_vertical(ed, -1, select)
        }
        if (nav_pressed(ImGuiKey.DownArrow, ImGuiKey.Keypad2, keypad_nav)) {
            text_edit_move_vertical(ed, 1, select)
        }
        if (nav_pressed(ImGuiKey.Home, ImGuiKey.Keypad7, keypad_nav)) {
            text_edit_move_document_start(ed, select)
        }
        if (nav_pressed(ImGuiKey.End, ImGuiKey.Keypad1, keypad_nav)) {
            text_edit_move_document_end(ed, select)
        }
        if (IsKeyPressed(ImGuiKey.Backspace, true)) {
            text_edit_delete_word_left(ed)
        }
        if (nav_pressed(ImGuiKey.Delete, ImGuiKey.KeypadDecimal, keypad_nav)) {
            text_edit_delete_word_right(ed)
        }
        if (IsKeyPressed(ImGuiKey.A, false)
                || (keypad_nav && IsKeyPressed(ImGuiKey.Keypad5, false))) {
            text_edit_select_all(ed)
        }
        if (IsKeyPressed(ImGuiKey.Z, true)) {
            let _ = io.KeyShift ? text_edit_redo(ed) : text_edit_undo(ed)
        }
        if (IsKeyPressed(ImGuiKey.Y, true)) {
            let _ = text_edit_redo(ed)
        }
        if (IsKeyPressed(ImGuiKey.C, false) || IsKeyPressed(ImGuiKey.Insert, false)
                || (keypad_nav && IsKeyPressed(ImGuiKey.Keypad0, false))) {
            editor_copy_to_clipboard(ed, use_system_clipboard)
        }
        if (IsKeyPressed(ImGuiKey.X, false)) {
            editor_cut_to_clipboard(ed, use_system_clipboard)
        }
        if (IsKeyPressed(ImGuiKey.V, true)) {
            editor_paste_from_clipboard(ed, use_system_clipboard)
        }
    } elif (io.KeyAlt && !io.KeyCtrl) {
        // Alt+Backspace undo, Shift+Alt+Backspace redo — the CUA legacy pair.
        if (IsKeyPressed(ImGuiKey.Backspace, true)) {
            let _ = io.KeyShift ? text_edit_redo(ed) : text_edit_undo(ed)
        }
    } elif (!io.KeyAlt) {
        if (nav_pressed(ImGuiKey.LeftArrow, ImGuiKey.Keypad4, keypad_nav)) {
            text_edit_move_left(ed, select)
        }
        if (nav_pressed(ImGuiKey.RightArrow, ImGuiKey.Keypad6, keypad_nav)) {
            text_edit_move_right(ed, select)
        }
        if (nav_pressed(ImGuiKey.UpArrow, ImGuiKey.Keypad8, keypad_nav)) {
            text_edit_move_vertical(ed, -1, select)
        }
        if (nav_pressed(ImGuiKey.DownArrow, ImGuiKey.Keypad2, keypad_nav)) {
            text_edit_move_vertical(ed, 1, select)
        }
        if (nav_pressed(ImGuiKey.PageUp, ImGuiKey.Keypad9, keypad_nav)) {
            text_edit_move_vertical(ed, -max(page_rows, 1), select)
        }
        if (nav_pressed(ImGuiKey.PageDown, ImGuiKey.Keypad3, keypad_nav)) {
            text_edit_move_vertical(ed, max(page_rows, 1), select)
        }
        if (nav_pressed(ImGuiKey.Home, ImGuiKey.Keypad7, keypad_nav)) {
            text_edit_move_home(ed, select)
        }
        if (nav_pressed(ImGuiKey.End, ImGuiKey.Keypad1, keypad_nav)) {
            text_edit_move_end(ed, select)
        }
        if (IsKeyPressed(ImGuiKey.Enter, true) || IsKeyPressed(ImGuiKey.KeypadEnter, true)) {
            text_edit_newline(ed)
        }
        if (IsKeyPressed(ImGuiKey.Backspace, true)) {
            text_edit_backspace(ed)
        }
        if (nav_pressed(ImGuiKey.Delete, ImGuiKey.KeypadDecimal, keypad_nav)) {
            if (select) {
                editor_cut_to_clipboard(ed, use_system_clipboard) // Shift+Del = cut (CUA)
            } else {
                text_edit_delete(ed)
            }
        }
        if (select && IsKeyPressed(ImGuiKey.Insert, false)) {
            editor_paste_from_clipboard(ed, use_system_clipboard) // Shift+Ins = paste (CUA)
        }
        if (!select && (IsKeyPressed(ImGuiKey.Insert, false)
                || (keypad_nav && IsKeyPressed(ImGuiKey.Keypad0, false)))) {
            text_edit_toggle_overtype(ed)
        }
        if (IsKeyPressed(ImGuiKey.Tab, true)) {
            text_edit_insert(ed, "\t")
        }
        if (IsKeyPressed(ImGuiKey.Escape, false) && text_edit_has_selection(ed)) {
            text_edit_clear_selection(ed)
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
        // overtype renders the block caret (translucent so the glyph reads)
        let caret_width = ed.overtype ? m.cell_width : max(1.0f, 1.5f * m.zoom)
        var caret_color = style.caret_color
        if (ed.overtype) {
            caret_color.w *= 0.55f
        }
        *dl |> AddRectFilled(float2(caret_x, caret_y),
            float2(caret_x + caret_width, caret_y + m.line_height),
            GetColorU32(caret_color), 0.0f, ImDrawFlags.None)
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
        route_editor_keys(ed, page_rows, style.write_system_clipboard)
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
