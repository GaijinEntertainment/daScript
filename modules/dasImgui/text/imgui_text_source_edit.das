options gen2
options _comment_hygiene = true
options indenting = 4
// custom-widget layer: raw drawlist/ID-stack by design (same as the view);
// the find-bar require pulls imgui_lint into this chain, hence the opt-out
options _allow_imgui_legacy = true

module imgui_text_source_edit shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_text_source_view public
require imgui/imgui_text_flow public
require imgui/imgui_text_find public
require imgui/imgui_text_language public
require imgui/imgui_commands public
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

struct public TextEditRange {
    //! Ordered (start <= end) line/byte-column selection range.
    start_line : int
    start_column : int
    end_line : int
    end_column : int
}

struct public TextEditStyleRun {
    //! Byte range within one line painted with a syntax style.
    start_byte : int
    end_byte : int
    style : TextSourceSyntaxStyle
}

struct public TextEditLineStyle {
    runs : array<TextEditStyleRun>
    //! Whether the line BEGINS inside a block comment (the lexical carry).
    in_block_comment : bool
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
    //! VISUAL column remembered across consecutive vertical moves (tabs count
    //! as `tab_cells`); -1 = none.
    preferred_column : int = -1
    //! Visual width of a tab, synced from the style by the widget each frame
    //! so pure verbs and the renderer agree.
    tab_cells : int = 4
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
    //! Viewport scroll request in lines (Ctrl+Up/Down); caret stays put.
    pending_scroll_lines : int = 0
    //! Host-fed ranges painted under the text (find matches); keep ascending.
    highlight_ranges : array<TextEditRange>
    //! Canonical language for styling ("" = plain); text_edit_set_language.
    language : string
    //! Per-line style runs, kept aligned with `lines` through every edit.
    line_styles : array<TextEditLineStyle>
    //! Seconds since the last edit — drives the full-parse debounce.
    syntax_idle : float = 0.0f
    //! Buffer revision the last FULL parse styled (-1 = lexical only).
    syntax_revision : int = -1
    //! Ghost completion drawn after the caret; Tab accepts it. Cleared by
    //! every edit/move — the completion sync re-offers next frame.
    ghost_text : string
    //! Completion LIST under the caret (top candidates). list_index -1 =
    //! visible but the keyboard is still in the editor (Down enters).
    list_items : array<string>
    list_open : bool = false
    list_index : int = -1
    //! Byte length of the request prefix — accepting inserts the remainder.
    list_prefix_bytes : int = 0
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

def public text_edit_dispose(var ed : TextEditState) {
    delete ed.lines
    delete ed.undo_stack
    delete ed.redo_stack
    delete ed.highlight_ranges
    delete ed.line_styles
    delete ed.list_items
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
    delete ed.highlight_ranges
    delete ed.line_styles
    text_edit_close_list(ed)
    ed.undo_open = false
    ed.dirty = false
    ed.revision++
    ed.follow_revision++
    ed.syntax_revision = -1
    ed.syntax_idle = 0.0f
    restyle_lines(ed, 0, length(ed.lines) - 1)
}

def public text_edit_source(ed : TextEditState const) : string {
    return join(ed.lines, ed.crlf ? "\r\n" : "\n")
}

def public text_edit_text(ed : TextEditState const) : string {
    //! LF-joined buffer text — the search / byte-mapping domain
    //! (`text_edit_source` keeps the file's CRLF flavor for saving).
    return join(ed.lines, "\n")
}

def public text_edit_position_of_byte(ed : TextEditState const;
                                      byte_offset : int) : int2 {
    //! Map a byte offset in the LF-joined text (`text_edit_text` domain) to
    //! (line, column); out-of-range offsets clamp to the buffer ends.
    return int2(0, 0) if (empty(ed.lines))
    var remaining = max(byte_offset, 0)
    for (index in range(length(ed.lines))) {
        let line_length = length(ed.lines[index])
        return int2(index, remaining) if (remaining <= line_length)
        remaining -= line_length + 1
    }
    let last = length(ed.lines) - 1
    return int2(last, length(ed.lines[last]))
}

def public text_edit_range_of_bytes(ed : TextEditState const;
                                    start_byte, end_byte : int) : TextEditRange {
    let head = text_edit_position_of_byte(ed, start_byte)
    let tail = text_edit_position_of_byte(ed, max(end_byte, start_byte))
    return TextEditRange(start_line = head.x, start_column = head.y,
        end_line = tail.x, end_column = tail.y)
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

// ===== Syntax (lexical tier) =====

def private lex_push(var runs : array<TextEditStyleRun>; start_byte, end_byte : int;
                     syntax : TextSourceSyntaxStyle) {
    return if (end_byte <= start_byte)
    runs |> push(TextEditStyleRun(start_byte = start_byte, end_byte = end_byte,
        style = syntax))
}

def private bytes_match(line : string; at : int; needle : string) : bool {
    let needle_length = length(needle)
    return false if (needle_length == 0 || at + needle_length > length(line))
    return slice(line, at, at + needle_length) == needle
}

def private byte_in(set_bytes : string; b : int) : bool {
    var hit = false
    peek_data(set_bytes) $(bytes) {
        for (value in bytes) {
            if (int(value) == b) {
                hit = true
                break
            }
        }
    }
    return hit
}

def private lex_line(profile : TextLanguageProfile const; line : string; // nolint:STYLE037
                     entry_comment : bool;
                     var runs : array<TextEditStyleRun>) : bool {
    // returns whether the NEXT line starts inside a block comment
    runs |> clear()
    return entry_comment if (empty(line))
    var in_comment = entry_comment
    let n = length(line)
    let has_block = !empty(profile.block_comment_open)
    let lc_first = empty(profile.line_comment) ? -1 : first_character(profile.line_comment)
    let bo_first = has_block ? first_character(profile.block_comment_open) : -1
    peek_data(line) $(bytes) {
        var at = 0
        if (in_comment) {
            let close_at = has_block ? find(line, profile.block_comment_close) : -1
            if (close_at < 0) {
                lex_push(runs, 0, n, TextSourceSyntaxStyle.comment)
                at = n
            } else {
                at = close_at + length(profile.block_comment_close)
                lex_push(runs, 0, at, TextSourceSyntaxStyle.comment)
                in_comment = false
            }
        }
        while (at < n) {
            let b = int(bytes[at])
            if (b == lc_first && bytes_match(line, at, profile.line_comment)) {
                lex_push(runs, at, n, TextSourceSyntaxStyle.comment)
                at = n
            } elif (b == bo_first && bytes_match(line, at, profile.block_comment_open)) {
                let close_at = find(line, profile.block_comment_close,
                    at + length(profile.block_comment_open))
                if (close_at < 0) {
                    lex_push(runs, at, n, TextSourceSyntaxStyle.comment)
                    in_comment = true
                    at = n
                } else {
                    let after = close_at + length(profile.block_comment_close)
                    lex_push(runs, at, after, TextSourceSyntaxStyle.comment)
                    at = after
                }
            } elif (byte_in(profile.string_delimiters, b)) {
                var end_ = at + 1
                while (end_ < n) {
                    let c = int(bytes[end_])
                    if (c == '\\') {
                        end_ += 2
                    } elif (c == b) {
                        end_++
                        break
                    } else {
                        end_++
                    }
                }
                lex_push(runs, at, min(end_, n), TextSourceSyntaxStyle.string_)
                at = min(end_, n)
            } elif (is_alpha(b) || b == '_' || b >= 0x80) {
                let start = at
                while (at < n && byte_word_class(int(bytes[at])) == 1) {
                    at++
                }
                if (find_index(profile.keywords, slice(line, start, at)) >= 0) {
                    lex_push(runs, start, at, TextSourceSyntaxStyle.keyword)
                }
            } elif (is_number(b)) {
                let start = at
                while (at < n && (is_number(int(bytes[at])) || is_alpha(int(bytes[at]))
                        || int(bytes[at]) == '.')) {
                    at++
                }
                lex_push(runs, start, at, TextSourceSyntaxStyle.number)
            } else {
                at++
            }
        }
    }
    return in_comment
}

def private restyle_lines(var ed : TextEditState; first_line, last_forced : int) {
    return if (empty(ed.language))
    // align the per-line entries with the buffer at the edit point
    while (length(ed.line_styles) < length(ed.lines)) {
        var blank = TextEditLineStyle()
        ed.line_styles |> emplace(blank, clamp(first_line, 0, length(ed.line_styles)))
    }
    while (length(ed.line_styles) > length(ed.lines)) {
        ed.line_styles |> erase(clamp(first_line, 0, length(ed.line_styles) - 1))
    }
    return if (empty(ed.lines))
    text_language_profile(ed.language) $(profile) {
        // scratch-lex the line above for its carry - never replace its runs
        var at = clamp(first_line, 0, length(ed.lines) - 1)
        var carry = false
        if (at > 0 && at - 1 < length(ed.line_styles)) {
            var scratch : array<TextEditStyleRun>
            carry = lex_line(profile, ed.lines[at - 1],
                ed.line_styles[at - 1].in_block_comment, scratch)
            delete scratch
        }
        while (at < length(ed.lines)) {
            // converged: an untouched line already starts in this state
            break if (at > last_forced && ed.line_styles[at].in_block_comment == carry)
            ed.line_styles[at].in_block_comment = carry
            carry = lex_line(profile, ed.lines[at], carry, ed.line_styles[at].runs)
            at++
        }
    }
}

struct public TextEditBracketMatch {
    valid : bool = false
    //! A bracket sits at the caret but its counterpart is missing; the lone
    //! bracket's position is in open_line/open_column.
    mismatch : bool = false
    open_line : int
    open_column : int
    close_line : int
    close_column : int
}

def private code_byte(ed : TextEditState const; line, column : int) : int {
    // out of range, or inside a comment/string run = not code
    return -1 if (line < 0 || line >= length(ed.lines) || column < 0
        || column >= length(ed.lines[line])
        || (line < length(ed.line_styles)
            && run_covers(ed.line_styles[line].runs, column)))
    var value = -1
    peek_data(ed.lines[line]) $(bytes) {
        value = int(bytes[column])
    }
    return value
}

def private scan_bracket(ed : TextEditState const; line, column : int; // nolint:STYLE037
                         open_byte, close_byte : int; forward : bool) : int2 {
    var depth = 1
    var at_line = line
    var at = column
    var scanned = 0
    while (scanned < 2000) {
        if (forward) {
            at++
            if (at >= length(ed.lines[at_line])) {
                at_line++
                scanned++
                return int2(-1, -1) if (at_line >= length(ed.lines))
                at = -1
                continue
            }
        } else {
            at--
            if (at < 0) {
                at_line--
                scanned++
                return int2(-1, -1) if (at_line < 0)
                at = length(ed.lines[at_line])
                continue
            }
        }
        let b = code_byte(ed, at_line, at)
        continue if (b < 0)
        if (b == (forward ? open_byte : close_byte)) {
            depth++
        } elif (b == (forward ? close_byte : open_byte)) {
            depth--
            return int2(at_line, at) if (depth == 0)
        }
    }
    return int2(-1, -1)
}

def public text_edit_match_bracket(ed : TextEditState const) : TextEditBracketMatch {
    //! The bracket pair at the caret (the byte AT the caret, else the byte
    //! BEFORE it) — comments/strings skipped, pairs from the language
    //! profile. Scan bounded to 2000 lines each way.
    var result = TextEditBracketMatch()
    return result if (empty(ed.language) || empty(ed.lines))
    text_language_profile(ed.language) $(profile) {
        let line = ed.cursor_line
        var column = ed.cursor_column
        var b = code_byte(ed, line, column)
        var pair_index = bracket_pair_index(profile, b)
        if (pair_index < 0 && column > 0) {
            column--
            b = code_byte(ed, line, column)
            pair_index = bracket_pair_index(profile, b)
        }
        if (pair_index >= 0) {
            let pair = profile.bracket_pairs[pair_index]
            let opener = b == pair.x
            let other = scan_bracket(ed, line, column, pair.x, pair.y, opener)
            if (other.x >= 0) {
                result.valid = true
                result.open_line = opener ? line : other.x
                result.open_column = opener ? column : other.y
                result.close_line = opener ? other.x : line
                result.close_column = opener ? other.y : column
            } else {
                result.mismatch = true
                result.open_line = line
                result.open_column = column
            }
        }
    }
    return result
}

def private bracket_pair_index(profile : TextLanguageProfile const; b : int) : int {
    return -1 if (b < 0)
    for (index in range(length(profile.bracket_pairs))) {
        let pair = profile.bracket_pairs[index]
        return index if (b == pair.x || b == pair.y)
    }
    return -1
}

def private parse_only_style(syntax : TextSourceSyntaxStyle) : bool {
    // styles only the FULL parse produces; the lexical pass never emits them
    return (syntax == TextSourceSyntaxStyle.type_
        || syntax == TextSourceSyntaxStyle.function_
        || syntax == TextSourceSyntaxStyle.variable
        || syntax == TextSourceSyntaxStyle.property
        || syntax == TextSourceSyntaxStyle.constant
        || syntax == TextSourceSyntaxStyle.operator_
        || syntax == TextSourceSyntaxStyle.punctuation)
}

def private run_overlaps(runs : array<TextEditStyleRun>; start, end_ : int) : bool {
    for (run in runs) {
        return true if (run.start_byte < end_ && run.end_byte > start)
    }
    return false
}

def private merge_parse_styles(var ed : TextEditState; line : int; // nolint:STYLE037
                               old_runs : array<TextEditStyleRun>;
                               edit_column, removed_bytes, inserted_bytes : int) {
    // anti-flicker: shift old parse-only runs across the edit into lexical gaps
    return if (line >= length(ed.line_styles) || line >= length(ed.lines))
    let delta = inserted_bytes - removed_bytes
    let line_length = length(ed.lines[line])
    for (old in old_runs) {
        continue if (!parse_only_style(old.style))
        var start = old.start_byte
        var end_ = old.end_byte
        if (start >= edit_column + removed_bytes) {
            start += delta
            end_ += delta
        } elif (end_ > edit_column) {
            continue // the edit cut through it - the reparse will restyle
        }
        continue if (start < 0 || end_ > line_length || end_ <= start
            || run_overlaps(ed.line_styles[line].runs, start, end_))
        // keep the runs sorted by start byte - the painter fills gaps in order
        var insert_at = length(ed.line_styles[line].runs)
        for (index in range(length(ed.line_styles[line].runs))) {
            if (ed.line_styles[line].runs[index].start_byte > start) {
                insert_at = index
                break
            }
        }
        ed.line_styles[line].runs |> push(TextEditStyleRun(start_byte = start,
            end_byte = end_, style = old.style), insert_at)
    }
}

def public text_edit_set_language(var ed : TextEditState; language : string) {
    //! Language for syntax styling (and E4 indent/brackets): a name or file
    //! extension, canonicalized per `syntax_canonical_language`; "" clears.
    ed.language = empty(language) ? "" : syntax_canonical_language(language)
    ed.syntax_revision = -1
    delete ed.line_styles
    restyle_lines(ed, 0, length(ed.lines) - 1)
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
    let same_line = find(removed, "\n") < 0 && find(inserted, "\n") < 0
    var preserved : array<TextEditStyleRun>
    if (same_line && line < length(ed.line_styles)) {
        preserved := ed.line_styles[line].runs
    }
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
    restyle_lines(ed, min(line, ed.cursor_line), max(line, ed.cursor_line))
    if (same_line) {
        merge_parse_styles(ed, line, preserved, column, length(removed),
            length(inserted))
    }
    delete preserved
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
    restyle_lines(ed, 0, length(ed.lines) - 1)
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
    restyle_lines(ed, 0, length(ed.lines) - 1)
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

def private visual_column_of(line : string; column, tab_cells : int) : int {
    // tab advances its full visual width; every other grapheme counts 1
    var visual = 0
    var inscope graphemes <- text_graphemes(line)
    for (grapheme in graphemes) {
        break if (grapheme.source.start_byte >= column)
        visual += grapheme.first_codepoint == 0x0009u ? max(tab_cells, 1) : 1
    }
    return visual
}

def private column_at_visual(line : string; visual_target, tab_cells : int) : int {
    // nearest grapheme boundary to the target visual column
    var visual = 0
    var result = length(line)
    var inscope graphemes <- text_graphemes(line)
    for (grapheme in graphemes) {
        let width = grapheme.first_codepoint == 0x0009u ? max(tab_cells, 1) : 1
        if (visual_target < visual + width) {
            result = ((visual_target - visual) * 2 < width
                ? grapheme.source.start_byte : grapheme.source.end_byte)
            break
        }
        visual += width
    }
    return result
}

// ===== Edits =====

def private touch_edit(var ed : TextEditState) {
    ed.dirty = true
    ed.revision++
    ed.preferred_column = -1
    ed.follow_revision++
    ed.blink_time = 0.0f
    ed.syntax_idle = 0.0f
    ed.ghost_text = ""
}

def private touch_move(var ed : TextEditState) {
    ed.follow_revision++
    ed.blink_time = 0.0f
    ed.ghost_text = ""
    text_edit_close_list(ed)
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

def private closer_dedents(var ed : TextEditState; text : string) : bool {
    // a closer typed at a whitespace-only line start dedents + lands, ONE unit
    return false if (empty(ed.language) || length(text) != 1
        || text_edit_has_selection(ed) || ed.cursor_column == 0)
    let b = first_character(text)
    let line = ed.lines[ed.cursor_line]
    var handled = false
    text_language_profile(ed.language) $(profile) {
        var is_closer = false
        for (pair in profile.indent_pairs) {
            if (b == pair.y) {
                is_closer = true
            }
        }
        if (is_closer) {
            let prefix = slice(line, 0, ed.cursor_column)
            if (prefix == leading_whitespace(line)) {
                let dedented = outdent_line(prefix)
                if (dedented != prefix) {
                    commit_edit(ed, TextEditUndoKind.other, ed.cursor_line, 0,
                        prefix, dedented + text)
                    handled = true
                }
            }
        }
    }
    return handled
}

def private line_byte(line : string; at : int) : int {
    return -1 if (at < 0 || at >= length(line))
    var value = -1
    peek_data(line) $(bytes) {
        value = int(bytes[at])
    }
    return value
}

def private pair_close_for(profile : TextLanguageProfile const; b : int) : int {
    for (pair in profile.bracket_pairs) {
        return pair.y if (b == pair.x)
    }
    return -1
}

def private is_pair_closer(profile : TextLanguageProfile const; b : int) : bool {
    for (pair in profile.bracket_pairs) {
        return true if (b == pair.y)
    }
    return false
}

def private surround_selection(var ed : TextEditState;
                               close_b : int; text : string) : bool {
    return false if (close_b < 0)
    let sel = text_edit_selection(ed)
    let inner = text_edit_range_text(ed, sel)
    commit_edit(ed, TextEditUndoKind.other, sel.start_line, sel.start_column,
        inner, text + inner + to_char(close_b))
    // the content stays selected inside the fresh pair
    let end_column = sel.end_line == sel.start_line ? sel.end_column + 1 : sel.end_column
    text_edit_select(ed, sel.start_line, sel.start_column + 1,
        sel.end_line, end_column)
    patch_unit_after(ed)
    return true
}

def private auto_close_pairs(var ed : TextEditState; text : string) : bool { // nolint:STYLE037
    // openers close themselves, closers skip twins, selections surround
    return false if (empty(ed.language) || length(text) != 1 || ed.overtype)
    let b = first_character(text)
    var handled = false
    text_language_profile(ed.language) $(profile) {
        let close_b = pair_close_for(profile, b)
        if (text_edit_has_selection(ed)) {
            handled = surround_selection(ed, close_b, text)
        } else {
            let line = ed.lines[ed.cursor_line]
            let next_b = line_byte(line, ed.cursor_column)
            let prev_b = line_byte(line, ed.cursor_column - 1)
            let quote = byte_in(profile.string_delimiters, b)
            let before_open = (next_b < 0 || next_b == ' ' || next_b == '\t'
                || is_pair_closer(profile, next_b))
            if (next_b == b && (is_pair_closer(profile, b) || quote)) {
                // typing the closer that is already there: step over it
                text_edit_move_right(ed)
                handled = true
            } elif (close_b >= 0 && before_open) {
                commit_edit(ed, TextEditUndoKind.other, ed.cursor_line,
                    ed.cursor_column, "", text + to_char(close_b))
                ed.cursor_column--
                patch_unit_after(ed)
                handled = true
            } elif (quote && before_open && byte_word_class(prev_b) != 1
                    && prev_b != b) {
                commit_edit(ed, TextEditUndoKind.other, ed.cursor_line,
                    ed.cursor_column, "", text + text)
                ed.cursor_column--
                patch_unit_after(ed)
                handled = true
            }
        }
    }
    return handled
}

def public text_edit_type(var ed : TextEditState; text : string) {
    //! One typed keystroke: like insert, but honoring overtype mode —
    //! replaces the next grapheme within the line, falls back to inserting
    //! at the line end. Selections are replaced normally either way.
    return if (empty(text))
    ensure_line(ed)
    ed.input_revision++
    return if (closer_dedents(ed, text) || auto_close_pairs(ed, text))
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

def private run_covers(runs : array<TextEditStyleRun>; at : int) : bool {
    // inside a comment/string run = not code (auto-indent must ignore it)
    for (run in runs) {
        continue if (run.style != TextSourceSyntaxStyle.comment
            && run.style != TextSourceSyntaxStyle.string_)
        return true if (at >= run.start_byte && at < run.end_byte)
    }
    return false
}

def private code_brace_delta(ed : TextEditState const;
                             profile : TextLanguageProfile const;
                             line_index, limit_column : int) : int {
    var delta = 0
    let has_runs = line_index < length(ed.line_styles)
    peek_data(ed.lines[line_index]) $(bytes) {
        for (at in range(min(limit_column, length(bytes)))) {
            continue if (has_runs && run_covers(ed.line_styles[line_index].runs, at))
            let b = int(bytes[at])
            for (pair in profile.indent_pairs) {
                if (b == pair.x) {
                    delta++
                } elif (b == pair.y) {
                    delta = max(delta - 1, 0) // a closer never dedents PAST its line
                }
            }
        }
    }
    return delta
}

def public text_edit_newline(var ed : TextEditState) {
    //! Enter: split the line, carrying the indentation; an unmatched REAL
    //! opener before the caret (comments/strings ignored) indents one deeper.
    //! Fences the undo unit BEFORE, so typing coalesces on the new line.
    text_edit_push_undo_stop(ed)
    ensure_line(ed)
    var inserted = "\n"
    if (!empty(ed.language)) {
        let reference = text_edit_selection(ed)
        text_language_profile(ed.language) $(profile) {
            if (!empty(profile.indent_pairs)) {
                var indent = leading_whitespace(ed.lines[reference.start_line])
                if (code_brace_delta(ed, profile, reference.start_line,
                        reference.start_column) > 0) {
                    indent += "\t"
                }
                inserted = "\n" + indent
            }
        }
    }
    text_edit_insert(ed, inserted)
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
        if (!empty(ed.language)) {
            // inside an empty auto-closed pair, Backspace eats both halves
            let prev_b = line_byte(line, ed.cursor_column - 1)
            let next_b = line_byte(line, ed.cursor_column)
            var pair_delete = false
            text_language_profile(ed.language) $(profile) {
                pair_delete = ((next_b >= 0 && pair_close_for(profile, prev_b) == next_b)
                    || (prev_b >= 0 && prev_b == next_b
                        && byte_in(profile.string_delimiters, prev_b)))
            }
            if (pair_delete) {
                commit_edit(ed, TextEditUndoKind.delete_back, ed.cursor_line,
                    ed.cursor_column - 1,
                    slice(line, ed.cursor_column - 1, ed.cursor_column + 1), "")
                return
            }
        }
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
            : visual_column_of(ed.lines[ed.cursor_line], ed.cursor_column,
                ed.tab_cells))
        ed.cursor_line = target
        ed.cursor_column = column_at_visual(ed.lines[target], preferred,
            ed.tab_cells)
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

// ===== Line operations =====

def private line_block(ed : TextEditState const) : int2 {
    // touched lines; selection ending at column 0 excludes that line
    let sel = text_edit_selection(ed)
    var last = sel.end_line
    if (last > sel.start_line && sel.end_column == 0) {
        last--
    }
    return int2(sel.start_line, last)
}

def private block_text(ed : TextEditState const; first, last : int) : string {
    return text_edit_range_text(ed, TextEditRange(start_line = first,
        start_column = 0, end_line = last,
        end_column = length(ed.lines[last])))
}

def private patch_unit_after(var ed : TextEditState) {
    // line-op verbs place the cursor AFTER committing; keep redo in sync
    return if (empty(ed.undo_stack))
    var last & = unsafe(ed.undo_stack[length(ed.undo_stack) - 1])
    last.after_cursor_line = ed.cursor_line
    last.after_cursor_column = ed.cursor_column
}

def private leading_whitespace(line : string) : string {
    var at = 0
    peek_data(line) $(bytes) {
        while (at < length(bytes)
                && (int(bytes[at]) == ' ' || int(bytes[at]) == '\t')) {
            at++
        }
    }
    return slice(line, 0, at)
}

def public text_edit_scroll(var ed : TextEditState; delta_lines : int) {
    //! Scroll the viewport WITHOUT moving the caret (Ctrl+Up/Down).
    ed.pending_scroll_lines += delta_lines
}

def public text_edit_move_lines(var ed : TextEditState; delta : int) {
    //! Move the lines touched by the selection one line up (-1) or down (+1);
    //! no-op at buffer edges; cursor/selection follow the block; ONE undo unit.
    ensure_line(ed)
    let touched = line_block(ed)
    return if ((delta < 0 && touched.x == 0)
        || (delta > 0 && touched.y >= length(ed.lines) - 1))
    let region_first = delta < 0 ? touched.x - 1 : touched.x
    let region_last = delta < 0 ? touched.y : touched.y + 1
    let neighbor = delta < 0 ? ed.lines[touched.x - 1] : ed.lines[touched.y + 1]
    let moved = block_text(ed, touched.x, touched.y)
    let old_text = block_text(ed, region_first, region_last)
    let new_text = delta < 0 ? moved + "\n" + neighbor : neighbor + "\n" + moved
    let cursor_line = ed.cursor_line
    let cursor_column = ed.cursor_column
    let anchor_line = ed.anchor_line
    let anchor_column = ed.anchor_column
    commit_edit(ed, TextEditUndoKind.other, region_first, 0, old_text, new_text)
    ed.cursor_line = cursor_line + delta
    ed.cursor_column = min(cursor_column, length(ed.lines[ed.cursor_line]))
    if (anchor_line >= 0) {
        ed.anchor_line = anchor_line + delta
        ed.anchor_column = min(anchor_column, length(ed.lines[ed.anchor_line]))
    }
    patch_unit_after(ed)
    ed.follow_revision++
}

def public text_edit_duplicate_lines(var ed : TextEditState; down : bool = true) {
    //! Duplicate the touched lines; `down` puts the caret on the lower copy
    //! (VS Code copyLinesDown), otherwise it stays on the upper one.
    ensure_line(ed)
    let touched = line_block(ed)
    let copied = block_text(ed, touched.x, touched.y)
    let cursor_line = ed.cursor_line
    let cursor_column = ed.cursor_column
    let anchor_line = ed.anchor_line
    let anchor_column = ed.anchor_column
    commit_edit(ed, TextEditUndoKind.other, touched.x, 0, "", copied + "\n")
    let shift = down ? touched.y - touched.x + 1 : 0
    ed.cursor_line = cursor_line + shift
    ed.cursor_column = cursor_column
    if (anchor_line >= 0) {
        ed.anchor_line = anchor_line + shift
        ed.anchor_column = anchor_column
    }
    patch_unit_after(ed)
    ed.follow_revision++
}

def public text_edit_delete_lines(var ed : TextEditState) {
    //! Delete every line touched by the selection, including its newline.
    ensure_line(ed)
    let touched = line_block(ed)
    let column = text_edit_selection(ed).start_column
    let removed = block_text(ed, touched.x, touched.y)
    if (touched.y < length(ed.lines) - 1) {
        commit_edit(ed, TextEditUndoKind.other, touched.x, 0, removed + "\n", "")
    } elif (touched.x > 0) {
        commit_edit(ed, TextEditUndoKind.other, touched.x - 1,
            length(ed.lines[touched.x - 1]), "\n" + removed, "")
        ed.cursor_line = min(touched.x, length(ed.lines) - 1)
        ed.cursor_column = 0
    } else {
        commit_edit(ed, TextEditUndoKind.other, 0, 0, removed, "")
    }
    ed.cursor_column = min(column, length(ed.lines[ed.cursor_line]))
    patch_unit_after(ed)
}

def public text_edit_insert_line_after(var ed : TextEditState) {
    //! Open a blank line below (Ctrl+Enter) carrying the current indentation,
    //! without splitting the current line.
    ensure_line(ed)
    let indent = leading_whitespace(ed.lines[ed.cursor_line])
    text_edit_push_undo_stop(ed)
    commit_edit(ed, TextEditUndoKind.insert_typing, ed.cursor_line,
        length(ed.lines[ed.cursor_line]), "", "\n" + indent)
}

def public text_edit_insert_line_before(var ed : TextEditState) {
    //! Open a blank line above (Ctrl+Shift+Enter) carrying the indentation.
    ensure_line(ed)
    let line = ed.cursor_line
    let indent = leading_whitespace(ed.lines[line])
    text_edit_push_undo_stop(ed)
    commit_edit(ed, TextEditUndoKind.insert_typing, line, 0, "", indent + "\n")
    ed.cursor_line = line
    ed.cursor_column = length(indent)
    patch_unit_after(ed)
}

def private reindent_block(var ed : TextEditState; outdent : bool) {
    ensure_line(ed)
    let touched = line_block(ed)
    let had_selection = text_edit_has_selection(ed)
    let old_text = block_text(ed, touched.x, touched.y)
    let new_text = build_string() $(var writer) {
        for (index in range(touched.x, touched.y + 1)) {
            if (index > touched.x) {
                writer |> write("\n")
            }
            let line = ed.lines[index]
            if (outdent) {
                writer |> write(outdent_line(line))
            } elif (empty(line)) {
                writer |> write(line) // empty lines are skipped on indent
            } else {
                writer |> write("\t")
                writer |> write(line)
            }
        }
    }
    return if (new_text == old_text)
    let cursor_line = ed.cursor_line
    commit_edit(ed, TextEditUndoKind.other, touched.x, 0, old_text, new_text)
    if (had_selection) {
        // the block stays selected whole after a re-indent
        ed.anchor_line = touched.x
        ed.anchor_column = 0
        ed.cursor_line = touched.y
        ed.cursor_column = length(ed.lines[touched.y])
    } else {
        ed.cursor_line = cursor_line
        ed.cursor_column = min(ed.cursor_column, length(ed.lines[cursor_line]))
    }
    patch_unit_after(ed)
    ed.follow_revision++
}

def private outdent_line(line : string) : string {
    return slice(line, 1, length(line)) if (starts_with(line, "\t"))
    var spaces = 0
    peek_data(line) $(bytes) {
        while (spaces < 4 && spaces < length(bytes) && int(bytes[spaces]) == ' ') {
            spaces++
        }
    }
    return slice(line, spaces, length(line))
}

def public text_edit_indent_lines(var ed : TextEditState) {
    reindent_block(ed, false)
}

def public text_edit_outdent_lines(var ed : TextEditState) {
    reindent_block(ed, true)
}

def public text_edit_set_ghost(var ed : TextEditState; text : string) {
    //! Offer a ghost completion at the caret (single line; "" clears). The
    //! completion sync owns this; Tab accepts, any edit or move clears.
    ed.ghost_text = text
}

def public text_edit_close_list(var ed : TextEditState) {
    ed.list_open = false
    ed.list_index = -1
    delete ed.list_items
}

def public text_edit_list_next(var ed : TextEditState) : bool {
    //! Down while the list shows: enter it (from -1) or step down. Returns
    //! false when the list is closed — the caller moves the caret instead.
    return false if (!ed.list_open || empty(ed.list_items))
    ed.list_index = min(ed.list_index + 1, length(ed.list_items) - 1)
    return true
}

def public text_edit_list_prev(var ed : TextEditState) : bool {
    //! Up inside the list; stepping past the top (-1) returns the keyboard
    //! to the editor, the list stays visible.
    return false if (!ed.list_open || ed.list_index < 0)
    ed.list_index--
    return true
}

def public text_edit_list_accept(var ed : TextEditState) : bool {
    //! Enter on a selected list row: insert the remainder of that candidate.
    return false if (!ed.list_open || ed.list_index < 0
        || ed.list_index >= length(ed.list_items))
    let item = ed.list_items[ed.list_index]
    let tail = slice(item, min(ed.list_prefix_bytes, length(item)), length(item))
    text_edit_close_list(ed)
    text_edit_insert(ed, tail)
    return true
}

def public text_edit_tab(var ed : TextEditState) {
    //! Tab accepts a visible ghost completion first (the VS Code rule); else
    //! per spec fork 2: multi-line or full-line selection block indents; a
    //! partial-line selection is replaced; else insert a tab.
    ensure_line(ed)
    if (!empty(ed.ghost_text)) {
        let accepted = ed.ghost_text
        text_edit_insert(ed, accepted)
        return
    }
    if (text_edit_has_selection(ed)) {
        let sel = text_edit_selection(ed)
        if (sel.end_line > sel.start_line
                || (sel.start_column == 0
                    && sel.end_column == length(ed.lines[sel.start_line]))) {
            text_edit_indent_lines(ed)
            return
        }
    }
    text_edit_insert(ed, "\t")
}

// ===== Find / replace =====

def public text_edit_replace_range(var ed : TextEditState; span : TextEditRange;
                                   text : string) {
    //! Replace an ordered range with `text` as ONE undo unit (kind `other`
    //! self-fences); the cursor lands after the inserted text.
    ensure_line(ed)
    let normalized = replace(replace(text, "\r\n", "\n"), "\r", "\n")
    commit_edit(ed, TextEditUndoKind.other, span.start_line, span.start_column,
        text_edit_range_text(ed, span), normalized)
}

def public text_edit_replace_all(var ed : TextEditState;
                                 spans : array<TextEditRange>;
                                 texts : array<string>) : int {
    //! Replace every span (ascending, non-overlapping) with its text as ONE
    //! undo unit — one undo reverts the whole replace-all. Returns the count.
    ensure_line(ed)
    let count_ = min(length(spans), length(texts))
    return 0 if (count_ == 0)
    delete ed.redo_stack
    var unit = TextEditUndoUnit(kind = TextEditUndoKind.other,
        before_cursor_line = ed.cursor_line, before_cursor_column = ed.cursor_column,
        before_anchor_line = ed.anchor_line, before_anchor_column = ed.anchor_column)
    ed.undo_stack |> emplace(unit)
    let unit_index = length(ed.undo_stack) - 1
    // back to front, so earlier spans stay valid while later ones change
    var index = count_ - 1
    while (index >= 0) {
        let span = spans[index]
        let inserted = replace(replace(texts[index], "\r\n", "\n"), "\r", "\n")
        ed.undo_stack[unit_index].ops |> push(TextEditUndoOp(
            line = span.start_line, column = span.start_column,
            removed = text_edit_range_text(ed, span), inserted = inserted))
        remove_range_raw(ed, span)
        let _ = insert_raw(ed, span.start_line, span.start_column, inserted)
        index--
    }
    text_edit_clear_selection(ed)
    ed.cursor_line = clamp(ed.cursor_line, 0, length(ed.lines) - 1)
    ed.cursor_column = clamp(ed.cursor_column, 0, length(ed.lines[ed.cursor_line]))
    var last & = unsafe(ed.undo_stack[unit_index])
    last.after_cursor_line = ed.cursor_line
    last.after_cursor_column = ed.cursor_column
    last.end_line = ed.cursor_line
    last.end_column = ed.cursor_column
    ed.undo_open = false
    touch_edit(ed)
    restyle_lines(ed, 0, length(ed.lines) - 1)
    return count_
}

def public text_edit_set_highlights(var ed : TextEditState;
                                    spans : array<TextEditRange>) {
    //! Ranges painted under the text (find matches); keep them ascending.
    delete ed.highlight_ranges
    ed.highlight_ranges := spans
}

def public text_edit_clear_highlights(var ed : TextEditState) {
    delete ed.highlight_ranges
}

def private refresh_find_highlights(var ed : TextEditState;
                                    find : TextFindState const) {
    var spans : array<TextEditRange>
    spans |> reserve(length(find.matches))
    for (matched in find.matches) {
        spans |> push(text_edit_range_of_bytes(ed, matched.start_byte,
            matched.end_byte))
    }
    delete ed.highlight_ranges
    ed.highlight_ranges <- spans
}

def private apply_replace_current(var ed : TextEditState;
                                  var find : TextFindState) : bool {
    let found = text_find_current(find)
    return false if (found.start_byte < 0)
    var inscope texts : array<string>
    text_find_replacements(find, text_edit_text(ed), texts)
    let inserted = (find.current >= 0 && find.current < length(texts)
        ? texts[find.current] : find.replace_text)
    text_edit_replace_range(ed,
        text_edit_range_of_bytes(ed, found.start_byte, found.end_byte), inserted)
    // resume PAST the insertion so a self-matching replacement cannot pin here
    let resume = found.start_byte + length(inserted)
    let _ = text_find_rebuild(find, text_edit_text(ed), ed.revision)
    if (!empty(find.matches)) {
        var pick = 0
        for (index in range(length(find.matches))) {
            if (find.matches[index].start_byte >= resume) {
                pick = index
                break
            }
        }
        find.current = pick
        find.jump_revision++
    }
    return true
}

def private apply_replace_all(var ed : TextEditState;
                              var find : TextFindState) : bool {
    return false if (empty(find.matches))
    let source = text_edit_text(ed)
    var inscope texts : array<string>
    text_find_replacements(find, source, texts)
    var inscope spans : array<TextEditRange>
    spans |> reserve(length(find.matches))
    for (matched in find.matches) {
        spans |> push(text_edit_range_of_bytes(ed, matched.start_byte,
            matched.end_byte))
    }
    let _ = text_edit_replace_all(ed, spans, texts)
    let _rebuilt = text_find_rebuild(find, text_edit_text(ed), ed.revision)
    return true
}

def public text_edit_find_sync(var ed : TextEditState; var find : TextFindState) {
    //! Per-frame glue between a buffer and its find/replace bar: rebuild on
    //! change, apply replace requests through the undo journal (replace-all =
    //! ONE undo unit), select the current match, refresh highlights. Pure state.
    if (!find.open) {
        text_edit_clear_highlights(ed)
        return
    }
    var changed = false
    if (text_find_needs_rebuild(find, ed.revision)) {
        let _ = text_find_rebuild(find, text_edit_text(ed), ed.revision)
        changed = true
    }
    if (find.replace_revision != find.applied_replace_revision) {
        find.applied_replace_revision = find.replace_revision
        changed = apply_replace_current(ed, find) || changed
    }
    if (find.replace_all_revision != find.applied_replace_all_revision) {
        find.applied_replace_all_revision = find.replace_all_revision
        changed = apply_replace_all(ed, find) || changed
    }
    if (find.jump_revision != find.applied_jump_revision) {
        find.applied_jump_revision = find.jump_revision
        let found = text_find_current(find)
        if (found.start_byte >= 0) {
            let span = text_edit_range_of_bytes(ed, found.start_byte,
                found.end_byte)
            text_edit_select(ed, span.start_line, span.start_column,
                span.end_line, span.end_column)
        }
    }
    if (changed || (empty(ed.highlight_ranges) && !empty(find.matches))) {
        refresh_find_highlights(ed, find)
    }
}

def private seed_find_from_selection(ed : TextEditState const;
                                     var find : TextFindState) {
    // VS Code seeds the query from a single-line selection
    return if (!text_edit_has_selection(ed))
    let sel = text_edit_selection(ed)
    return if (sel.start_line != sel.end_line)
    let text = text_edit_range_text(ed, sel)
    return if (empty(text))
    text_find_set_query(find, text, find.use_regex)
}

def private dispatch_find_command(ed : TextEditState;
                                  var find : TextFindState; command_id : string) {
    if (command_id == "find.open") {
        seed_find_from_selection(ed, find)
        text_find_open(find)
    } elif (command_id == "find.replace") {
        seed_find_from_selection(ed, find)
        text_find_open_replace(find)
    } elif (command_id == "find.next") {
        text_find_step(find, 1)
    } elif (command_id == "find.prev") {
        text_find_step(find, -1)
    } elif (command_id == "find.close") {
        text_find_close(find)
    }
}

def public text_edit_find_sync(var ed : TextEditState; var find : TextFindState;
                               var registry : CommandRegistry) {
    //! Registry variant: first drains the `find.` invocations queued by the
    //! editor's component-scope poll (open / replace / next / prev / close),
    //! then runs the pure sync.
    var invocation = CommandInvocation()
    while (command_take_prefix(registry, "find.", invocation)) {
        dispatch_find_command(ed, find, invocation.command_id)
    }
    command_set_enabled(registry, "find.close", find.open)
    text_edit_find_sync(ed, find)
}

def public text_edit_expand_line_selection(var ed : TextEditState) {
    //! Ctrl+L: select the current line incl. newline; repeat extends a line.
    ensure_line(ed)
    let sel = text_edit_selection(ed)
    ed.anchor_line = sel.start_line
    ed.anchor_column = 0
    if (sel.end_line < length(ed.lines) - 1) {
        ed.cursor_line = sel.end_line + 1
        ed.cursor_column = 0
    } else {
        ed.cursor_line = sel.end_line
        ed.cursor_column = length(ed.lines[sel.end_line])
    }
    ed.preferred_column = -1
    touch_move(ed)
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

def private drain_typed_chars(var ed : TextEditState) : bool {
    // typed characters NEVER route through bindings (IME/paste-as-typing safe)
    let io & = unsafe(GetIO())
    var typed_chars = false
    if (!io.KeyCtrl && !io.KeyAlt) {
        for (value in io.InputQueueCharacters) {
            let codepoint = int(value)
            if (codepoint >= 0x20 && codepoint != 0x7f && codepoint <= 0x10ffff) {
                text_edit_type(ed, codepoint_utf8(codepoint))
                typed_chars = true
            }
        }
    }
    return typed_chars
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
    let keypad_nav = !drain_typed_chars(ed)
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
            if (select || !text_edit_list_prev(ed)) {
                text_edit_move_vertical(ed, -1, select)
            }
        }
        if (nav_pressed(ImGuiKey.DownArrow, ImGuiKey.Keypad2, keypad_nav)) {
            if (select || !text_edit_list_next(ed)) {
                text_edit_move_vertical(ed, 1, select)
            }
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
            if (!text_edit_list_accept(ed)) {
                text_edit_newline(ed)
            }
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
        if (IsKeyPressed(ImGuiKey.Escape, false)) {
            if (ed.list_open) {
                text_edit_close_list(ed)
            } elif (text_edit_has_selection(ed)) {
                text_edit_clear_selection(ed)
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
    ed.syntax_idle += io.DeltaTime
    draw_edit_highlights(ed, m, style, font, dl, first_visible, last_visible)
    if (ed.focused) {
        draw_bracket_highlight(ed, m, style, font, dl, first_visible, last_visible)
    }
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
            draw_edit_line(ed, row, line, m, style, font, dl, row_origin)
        }
    }
    if (ed.focused && !empty(ed.ghost_text) && !has_selection
            && ed.cursor_line >= first_visible && ed.cursor_line <= last_visible) {
        draw_edit_ghost(ed, m, style, font, dl)
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
    if (ed.focused && ed.list_open && !empty(ed.list_items)) {
        draw_edit_list(ed, m, style, font, dl)
    }
}

def private edit_syntax_color(style : TextSourceViewStyle const; // nolint:STYLE037
                              syntax : TextSourceSyntaxStyle) : float4 {
    if (syntax == TextSourceSyntaxStyle.comment) return style.syntax_comment_color
    if (syntax == TextSourceSyntaxStyle.string_) return style.syntax_string_color
    if (syntax == TextSourceSyntaxStyle.number) return style.syntax_number_color
    if (syntax == TextSourceSyntaxStyle.keyword) return style.syntax_keyword_color
    if (syntax == TextSourceSyntaxStyle.type_) return style.syntax_type_color
    if (syntax == TextSourceSyntaxStyle.function_) return style.syntax_function_color
    if (syntax == TextSourceSyntaxStyle.variable) return style.syntax_variable_color
    if (syntax == TextSourceSyntaxStyle.property) return style.syntax_property_color
    if (syntax == TextSourceSyntaxStyle.constant) return style.syntax_constant_color
    if (syntax == TextSourceSyntaxStyle.operator_) return style.syntax_operator_color
    if (syntax == TextSourceSyntaxStyle.punctuation) return style.syntax_punctuation_color
    return style.text_color
}

def private draw_edit_line(ed : TextEditState const; row : int; line : string;
                           m : EditMetrics; style : TextSourceViewStyle const;
                           var font : ImFont?; var dl : ImDrawList?;
                           row_origin : float2) {
    var x = row_origin.x + m.gutter_space
    let position_y = row_origin.y
    if (row >= length(ed.line_styles) || empty(ed.line_styles[row].runs)) {
        let _ = text_source_draw_range(dl, font, m.font_size, float2(x, position_y),
            style.text_color, line, 0, length(line), m.cell_width, style.tab_cells)
        return
    }
    var covered = 0
    for (run in ed.line_styles[row].runs) {
        if (run.start_byte > covered) {
            x += text_source_draw_range(dl, font, m.font_size, float2(x, position_y),
                style.text_color, line, covered, run.start_byte,
                m.cell_width, style.tab_cells)
        }
        let run_end = min(run.end_byte, length(line))
        x += text_source_draw_range(dl, font, m.font_size, float2(x, position_y),
            edit_syntax_color(style, run.style), line, run.start_byte, run_end,
            m.cell_width, style.tab_cells)
        covered = run_end
    }
    if (covered < length(line)) {
        let _ = text_source_draw_range(dl, font, m.font_size, float2(x, position_y),
            style.text_color, line, covered, length(line),
            m.cell_width, style.tab_cells)
    }
}

def private draw_edit_list(ed : TextEditState const; m : EditMetrics;
                           style : TextSourceViewStyle const;
                           var font : ImFont?; var dl : ImDrawList?) {
    // suggest rows under the caret; selection_color marks the entered row
    let anchor_x = (m.origin.x + m.gutter_space
        + text_source_range_width(ed.lines[ed.cursor_line], 0,
            ed.cursor_column - min(ed.list_prefix_bytes, ed.cursor_column),
            font, m.font_size, m.cell_width, style.tab_cells))
    var width = m.cell_width * 8.0f
    for (item in ed.list_items) {
        width = max(width, CalcTextSizeForFontRange(font, m.font_size,
            item, length(item), 0, length(item)).x + m.cell_width * 2.0f)
    }
    let top = m.origin.y + float(ed.cursor_line + 1) * m.line_height
    let bottom = top + float(length(ed.list_items)) * m.line_height
    *dl |> AddRectFilled(float2(anchor_x, top), float2(anchor_x + width, bottom),
        GetColorU32(style.list_bg_color), 0.0f, ImDrawFlags.None)
    for (index in range(length(ed.list_items))) {
        let row_top = top + float(index) * m.line_height
        if (index == ed.list_index) {
            *dl |> AddRectFilled(float2(anchor_x, row_top),
                float2(anchor_x + width, row_top + m.line_height),
                GetColorU32(style.selection_color), 0.0f, ImDrawFlags.None)
        }
        let item = ed.list_items[index]
        let _ = text_source_draw_range(dl, font, m.font_size,
            float2(anchor_x + m.cell_width, row_top), style.text_color,
            item, 0, length(item), m.cell_width, style.tab_cells)
    }
}

def private draw_edit_ghost(ed : TextEditState const; m : EditMetrics;
                            style : TextSourceViewStyle const;
                            var font : ImFont?; var dl : ImDrawList?) {
    let ghost_x = (m.origin.x + m.gutter_space
        + text_source_range_width(ed.lines[ed.cursor_line], 0, ed.cursor_column,
            font, m.font_size, m.cell_width, style.tab_cells))
    var ghost_color = style.text_color
    ghost_color.w *= 0.45f
    let _ = text_source_draw_range(dl, font, m.font_size,
        float2(ghost_x, m.origin.y + float(ed.cursor_line) * m.line_height),
        ghost_color, ed.ghost_text, 0, length(ed.ghost_text),
        m.cell_width, style.tab_cells)
}

def private draw_bracket_highlight(ed : TextEditState const; m : EditMetrics;
                                   style : TextSourceViewStyle const;
                                   var font : ImFont?; var dl : ImDrawList?;
                                   first_visible, last_visible : int) {
    let matched = text_edit_match_bracket(ed)
    if (matched.valid) {
        draw_bracket_cell(ed, m, style, font, dl, matched.open_line,
            matched.open_column, first_visible, last_visible,
            style.bracket_match_color)
        draw_bracket_cell(ed, m, style, font, dl, matched.close_line,
            matched.close_column, first_visible, last_visible,
            style.bracket_match_color)
    } elif (matched.mismatch) {
        draw_bracket_cell(ed, m, style, font, dl, matched.open_line,
            matched.open_column, first_visible, last_visible,
            style.bracket_mismatch_color)
    }
}

def private draw_bracket_cell(ed : TextEditState const; m : EditMetrics;
                              style : TextSourceViewStyle const;
                              var font : ImFont?; var dl : ImDrawList?;
                              line, column, first_visible, last_visible : int;
                              color : float4) {
    return if (line < first_visible || line > last_visible)
    let text = ed.lines[line]
    let x0 = (m.gutter_space + text_source_range_width(text, 0, column,
        font, m.font_size, m.cell_width, style.tab_cells))
    let x1 = (m.gutter_space + text_source_range_width(text, 0,
        min(column + 1, length(text)), font, m.font_size, m.cell_width,
        style.tab_cells))
    let top = m.origin.y + float(line) * m.line_height
    *dl |> AddRectFilled(float2(m.origin.x + x0, top),
        float2(m.origin.x + max(x1, x0 + 2.0f), top + m.line_height),
        GetColorU32(color), 0.0f, ImDrawFlags.None)
}

def private draw_edit_highlights(ed : TextEditState const; m : EditMetrics;
                                 style : TextSourceViewStyle const;
                                 var font : ImFont?; var dl : ImDrawList?;
                                 first_visible, last_visible : int) {
    return if (empty(ed.highlight_ranges))
    let color = GetColorU32(style.match_color)
    for (span in ed.highlight_ranges) {
        continue if (span.end_line < first_visible)
        break if (span.start_line > last_visible)
        for (row in range(max(span.start_line, first_visible),
                min(span.end_line, last_visible) + 1)) {
            let row_line = ed.lines[row]
            let from_column = row == span.start_line ? span.start_column : 0
            let to_column = row == span.end_line ? span.end_column : length(row_line)
            let x0 = (m.gutter_space + text_source_range_width(row_line, 0,
                from_column, font, m.font_size, m.cell_width, style.tab_cells))
            let x1 = (m.gutter_space + text_source_range_width(row_line, 0,
                to_column, font, m.font_size, m.cell_width, style.tab_cells))
            let row_top = m.origin.y + float(row) * m.line_height
            *dl |> AddRectFilled(float2(m.origin.x + x0, row_top),
                float2(m.origin.x + max(x1, x0 + 2.0f), row_top + m.line_height),
                color, 0.0f, ImDrawFlags.None)
        }
    }
}

def private text_source_edit_impl(id : string; var ed : TextEditState;
                                  var style : TextSourceViewStyle;
                                  var registry : CommandRegistry;
                                  use_registry : bool) : TextEditResult {
    var result = TextEditResult()
    ensure_line(ed)
    ed.tab_cells = max(style.tab_cells, 1)
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
    // keyboard arbitration: focused WINDOW picks the editor; WantTextInput mutes
    let io & = unsafe(GetIO())
    let window_focused = IsWindowFocused(ImGuiFocusedFlags.RootAndChildWindows)
    if (IsItemFocused()) {
        ed.focused = true
    } elif (ed.focused && window_focused && !io.WantTextInput) {
        // Shortcut routing follows item focus; the find bar keeps it on close — reclaim
        FocusItem()
    }
    handle_edit_mouse(ed, m, style, hovered, font)
    let page_rows = max(1, int(floor(m.view_height / m.line_height)) - 1)
    ed.visible_rows = page_rows
    if (ed.focused && window_focused && !io.WantTextInput) {
        if (use_registry) {
            // chars first (never binding-routed); char frames mute keypad chords
            let typed = drain_typed_chars(ed)
            // belt for Escape: closing the list never depends on chord routing
            if (ed.list_open && IsKeyPressed(ImGuiKey.Escape, false)) {
                text_edit_close_list(ed)
            }
            command_set_enabled(registry, "edit.cancel",
                text_edit_has_selection(ed) || ed.list_open)
            command_poll_scope(registry, CommandScope.component, id, typed)
        } else {
            route_editor_keys(ed, page_rows, style.write_system_clipboard)
        }
    }
    if (use_registry) {
        var invocation = CommandInvocation()
        while (command_take_prefix(registry, "edit.", invocation)) {
            dispatch_edit_command(ed, invocation.command_id, page_rows,
                style.write_system_clipboard)
        }
    }
    follow_edit_cursor(ed, m, style.tab_cells, font)
    if (ed.pending_scroll_lines != 0) {
        SetScrollY(max(GetScrollY()
            + float(ed.pending_scroll_lines) * m.line_height, 0.0f))
        ed.pending_scroll_lines = 0
    }
    // recount after input: a line-join edit shrinks the buffer mid-frame
    refresh_layout_cache(ed)
    draw_edit_content(ed, m, style, font)
    PopID()
    result.focused = ed.focused
    result.edited = ed.revision != revision_before
    result.cursor_moved = ed.follow_revision != follow_before
    return result
}

// ===== Command registry keymap =====

var private g_no_registry = CommandRegistry()

def public text_source_edit(id : string; var ed : TextEditState;
                            var style : TextSourceViewStyle) : TextEditResult {
    //! Draw + drive the editor as the sole content of a scrolling child window
    //! (the host owns the child, like `text_source_view`). This overload uses
    //! the built-in keymap; pass a CommandRegistry for rebindable keys.
    return text_source_edit_impl(id, ed, style, g_no_registry, false)
}

def public text_source_edit(id : string; var ed : TextEditState;
                            var style : TextSourceViewStyle;
                            var registry : CommandRegistry) : TextEditResult {
    //! Registry-driven variant: every key routes through rebindable
    //! imgui_commands bindings (call `text_edit_register_commands` once at
    //! init; the bindings editor + JSON persistence come with the registry).
    return text_source_edit_impl(id, ed, style, registry, true)
}

def private edit_define(var registry : CommandRegistry; id, title : string;
                        binding : CommandBinding) {
    command_define(registry, CommandDefinition(id = id, title = title,
        category = "Editor", scope = CommandScope.component), binding)
}

def private edit_binding(id : string; key, keypad : ImGuiKey;
                         primary, shift, rep : bool) : CommandBinding {
    var binding = command_binding(id, key, primary, shift, false, false,
        CommandScope.component, rep)
    if (keypad != ImGuiKey.None) {
        binding = with_alt_chord(binding, keypad, primary, shift)
    }
    return binding
}

def private edit_define_nav(var registry : CommandRegistry; id, title : string;
                            key, keypad : ImGuiKey; primary : bool = false) {
    // base + Shift select twin, numpad twins in the second chord slot
    edit_define(registry, id, title,
        edit_binding(id, key, keypad, primary, false, true))
    edit_define(registry, id + ".select", title + " (select)",
        edit_binding(id + ".select", key, keypad, primary, true, true))
}

def public text_edit_register_commands(var registry : CommandRegistry) { // nolint:STYLE038
    //! Register the editor verb surface as rebindable commands with the spec
    //! defaults (VS Code chords, CUA alternates, numpad twins). Idempotent.
    // find.close registers BEFORE edit.cancel: the Escape chain closes the bar first
    edit_define(registry, "find.close", "Close find",
        edit_binding("find.close", ImGuiKey.Escape, ImGuiKey.None,
            false, false, false))
    command_set_enabled(registry, "find.close", false)
    edit_define(registry, "find.open", "Find",
        edit_binding("find.open", ImGuiKey.F, ImGuiKey.None, true, false, false))
    edit_define(registry, "find.replace", "Replace",
        edit_binding("find.replace", ImGuiKey.H, ImGuiKey.None, true, false, false))
    edit_define(registry, "find.next", "Find next",
        edit_binding("find.next", ImGuiKey.F3, ImGuiKey.None, false, false, true))
    edit_define(registry, "find.prev", "Find previous",
        edit_binding("find.prev", ImGuiKey.F3, ImGuiKey.None, false, true, true))
    edit_define_nav(registry, "edit.cursor_left", "Cursor left",
        ImGuiKey.LeftArrow, ImGuiKey.Keypad4)
    edit_define_nav(registry, "edit.cursor_right", "Cursor right",
        ImGuiKey.RightArrow, ImGuiKey.Keypad6)
    edit_define_nav(registry, "edit.cursor_up", "Cursor up",
        ImGuiKey.UpArrow, ImGuiKey.Keypad8)
    edit_define_nav(registry, "edit.cursor_down", "Cursor down",
        ImGuiKey.DownArrow, ImGuiKey.Keypad2)
    edit_define_nav(registry, "edit.page_up", "Page up",
        ImGuiKey.PageUp, ImGuiKey.Keypad9)
    edit_define_nav(registry, "edit.page_down", "Page down",
        ImGuiKey.PageDown, ImGuiKey.Keypad3)
    edit_define_nav(registry, "edit.home", "Line start",
        ImGuiKey.Home, ImGuiKey.Keypad7)
    edit_define_nav(registry, "edit.end", "Line end",
        ImGuiKey.End, ImGuiKey.Keypad1)
    edit_define_nav(registry, "edit.word_left", "Word left",
        ImGuiKey.LeftArrow, ImGuiKey.Keypad4, true)
    edit_define_nav(registry, "edit.word_right", "Word right",
        ImGuiKey.RightArrow, ImGuiKey.Keypad6, true)
    edit_define_nav(registry, "edit.document_start", "Document start",
        ImGuiKey.Home, ImGuiKey.Keypad7, true)
    edit_define_nav(registry, "edit.document_end", "Document end",
        ImGuiKey.End, ImGuiKey.Keypad1, true)
    edit_define(registry, "edit.newline", "New line",
        edit_binding("edit.newline", ImGuiKey.Enter, ImGuiKey.KeypadEnter,
            false, false, true))
    edit_define(registry, "edit.backspace", "Delete left",
        edit_binding("edit.backspace", ImGuiKey.Backspace, ImGuiKey.None,
            false, false, true))
    edit_define(registry, "edit.delete", "Delete right",
        edit_binding("edit.delete", ImGuiKey.Delete, ImGuiKey.KeypadDecimal,
            false, false, true))
    edit_define(registry, "edit.delete_word_left", "Delete word left",
        edit_binding("edit.delete_word_left", ImGuiKey.Backspace, ImGuiKey.None,
            true, false, true))
    edit_define(registry, "edit.delete_word_right", "Delete word right",
        edit_binding("edit.delete_word_right", ImGuiKey.Delete,
            ImGuiKey.KeypadDecimal, true, false, true))
    edit_define(registry, "edit.tab", "Insert tab",
        edit_binding("edit.tab", ImGuiKey.Tab, ImGuiKey.None, false, false, true))
    edit_define(registry, "edit.select_all", "Select all",
        edit_binding("edit.select_all", ImGuiKey.A, ImGuiKey.Keypad5,
            true, false, false))
    edit_define(registry, "edit.undo", "Undo",
        with_alt_chord(command_binding("edit.undo", ImGuiKey.Z, true, false,
            false, false, CommandScope.component, true),
            ImGuiKey.Backspace, false, false, true))
    edit_define(registry, "edit.redo", "Redo",
        with_alt_chord(command_binding("edit.redo", ImGuiKey.Y, true, false,
            false, false, CommandScope.component, true),
            ImGuiKey.Z, true, true))
    edit_define(registry, "edit.copy", "Copy",
        with_alt_chord(command_binding("edit.copy", ImGuiKey.C, true, false,
            false, false, CommandScope.component), ImGuiKey.Insert, true))
    edit_define(registry, "edit.cut", "Cut",
        with_alt_chord(command_binding("edit.cut", ImGuiKey.X, true, false,
            false, false, CommandScope.component), ImGuiKey.Delete, false, true))
    edit_define(registry, "edit.paste", "Paste",
        with_alt_chord(command_binding("edit.paste", ImGuiKey.V, true, false,
            false, false, CommandScope.component, true),
            ImGuiKey.Insert, false, true))
    edit_define(registry, "edit.toggle_overtype", "Toggle overtype",
        edit_binding("edit.toggle_overtype", ImGuiKey.Insert, ImGuiKey.Keypad0,
            false, false, false))
    edit_define(registry, "edit.cancel", "Cancel selection",
        edit_binding("edit.cancel", ImGuiKey.Escape, ImGuiKey.None,
            false, false, false))
    // VS Code double-binds vertical selects on Ctrl+Shift too
    edit_define_ctrl_select(registry, "edit.cursor_up", "Cursor up",
        ImGuiKey.UpArrow, ImGuiKey.Keypad8)
    edit_define_ctrl_select(registry, "edit.cursor_down", "Cursor down",
        ImGuiKey.DownArrow, ImGuiKey.Keypad2)
    edit_define_ctrl_select(registry, "edit.page_up", "Page up",
        ImGuiKey.PageUp, ImGuiKey.Keypad9)
    edit_define_ctrl_select(registry, "edit.page_down", "Page down",
        ImGuiKey.PageDown, ImGuiKey.Keypad3)
    // Tier-2 line operations (VS Code chords)
    edit_define(registry, "edit.scroll_up", "Scroll up",
        edit_binding("edit.scroll_up", ImGuiKey.UpArrow, ImGuiKey.Keypad8,
            true, false, true))
    edit_define(registry, "edit.scroll_down", "Scroll down",
        edit_binding("edit.scroll_down", ImGuiKey.DownArrow, ImGuiKey.Keypad2,
            true, false, true))
    edit_define(registry, "edit.move_lines_up", "Move lines up",
        with_alt_chord(command_binding("edit.move_lines_up", ImGuiKey.UpArrow,
            false, false, true, false, CommandScope.component, true),
            ImGuiKey.Keypad8, false, false, true))
    edit_define(registry, "edit.move_lines_down", "Move lines down",
        with_alt_chord(command_binding("edit.move_lines_down", ImGuiKey.DownArrow,
            false, false, true, false, CommandScope.component, true),
            ImGuiKey.Keypad2, false, false, true))
    edit_define(registry, "edit.duplicate_lines_up", "Duplicate lines up",
        with_alt_chord(command_binding("edit.duplicate_lines_up", ImGuiKey.UpArrow,
            false, true, true, false, CommandScope.component),
            ImGuiKey.Keypad8, false, true, true))
    edit_define(registry, "edit.duplicate_lines_down", "Duplicate lines down",
        with_alt_chord(command_binding("edit.duplicate_lines_down", ImGuiKey.DownArrow,
            false, true, true, false, CommandScope.component),
            ImGuiKey.Keypad2, false, true, true))
    edit_define(registry, "edit.delete_lines", "Delete lines",
        command_binding("edit.delete_lines", ImGuiKey.K, true, true, false,
            false, CommandScope.component, true))
    edit_define(registry, "edit.insert_line_after", "Insert line below",
        with_alt_chord(command_binding("edit.insert_line_after", ImGuiKey.Enter,
            true, false, false, false, CommandScope.component),
            ImGuiKey.KeypadEnter, true))
    edit_define(registry, "edit.insert_line_before", "Insert line above",
        with_alt_chord(command_binding("edit.insert_line_before", ImGuiKey.Enter,
            true, true, false, false, CommandScope.component),
            ImGuiKey.KeypadEnter, true, true))
    edit_define(registry, "edit.indent_lines", "Indent lines",
        command_binding("edit.indent_lines", ImGuiKey.RightBracket, true,
            false, false, false, CommandScope.component, true))
    edit_define(registry, "edit.outdent_lines", "Outdent lines",
        with_alt_chord(command_binding("edit.outdent_lines", ImGuiKey.LeftBracket,
            true, false, false, false, CommandScope.component, true),
            ImGuiKey.Tab, false, true))
    edit_define(registry, "edit.expand_line_selection", "Select line",
        command_binding("edit.expand_line_selection", ImGuiKey.L, true, false,
            false, false, CommandScope.component, true))
}

def private edit_define_ctrl_select(var registry : CommandRegistry;
                                    id, title : string; key, keypad : ImGuiKey) {
    let row_id = id + ".select.2"
    edit_define(registry, row_id, title + " (select, Ctrl)",
        with_alt_chord(command_binding(row_id, key, true, true, false, false,
            CommandScope.component, true), keypad, true, true))
}

def private dispatch_edit_command(var ed : TextEditState; command_id : string; // nolint:STYLE037
                                  page_rows : int; use_system_clipboard : bool) {
    var select = false
    var verb = command_id
    if (ends_with(verb, ".2")) {
        verb = slice(verb, 0, length(verb) - 2)
    }
    if (ends_with(verb, ".select")) {
        select = true
        verb = slice(verb, 0, length(verb) - 7)
    }
    if (verb == "edit.cursor_left") {
        text_edit_move_left(ed, select)
    } elif (verb == "edit.cursor_right") {
        text_edit_move_right(ed, select)
    } elif (verb == "edit.cursor_up") {
        if (select || !text_edit_list_prev(ed)) {
            text_edit_move_vertical(ed, -1, select)
        }
    } elif (verb == "edit.cursor_down") {
        if (select || !text_edit_list_next(ed)) {
            text_edit_move_vertical(ed, 1, select)
        }
    } elif (verb == "edit.page_up") {
        text_edit_move_vertical(ed, -max(page_rows, 1), select)
    } elif (verb == "edit.page_down") {
        text_edit_move_vertical(ed, max(page_rows, 1), select)
    } elif (verb == "edit.home") {
        text_edit_move_home(ed, select)
    } elif (verb == "edit.end") {
        text_edit_move_end(ed, select)
    } elif (verb == "edit.word_left") {
        text_edit_move_word_left(ed, select)
    } elif (verb == "edit.word_right") {
        text_edit_move_word_right(ed, select)
    } elif (verb == "edit.document_start") {
        text_edit_move_document_start(ed, select)
    } elif (verb == "edit.document_end") {
        text_edit_move_document_end(ed, select)
    } elif (verb == "edit.newline") {
        if (!text_edit_list_accept(ed)) {
            text_edit_newline(ed)
        }
    } elif (verb == "edit.backspace") {
        text_edit_backspace(ed)
    } elif (verb == "edit.delete") {
        text_edit_delete(ed)
    } elif (verb == "edit.delete_word_left") {
        text_edit_delete_word_left(ed)
    } elif (verb == "edit.delete_word_right") {
        text_edit_delete_word_right(ed)
    } elif (verb == "edit.tab") {
        text_edit_tab(ed)
    } elif (verb == "edit.select_all") {
        text_edit_select_all(ed)
    } elif (verb == "edit.undo") {
        let _ = text_edit_undo(ed)
    } elif (verb == "edit.redo") {
        let _ = text_edit_redo(ed)
    } elif (verb == "edit.copy") {
        editor_copy_to_clipboard(ed, use_system_clipboard)
    } elif (verb == "edit.cut") {
        editor_cut_to_clipboard(ed, use_system_clipboard)
    } elif (verb == "edit.paste") {
        editor_paste_from_clipboard(ed, use_system_clipboard)
    } elif (verb == "edit.toggle_overtype") {
        text_edit_toggle_overtype(ed)
    } elif (verb == "edit.cancel") {
        if (ed.list_open) {
            text_edit_close_list(ed)
        } else {
            text_edit_clear_selection(ed)
        }
    } else {
        dispatch_edit_line_command(ed, verb)
    }
}

def private dispatch_edit_line_command(var ed : TextEditState; verb : string) {
    if (verb == "edit.scroll_up") {
        text_edit_scroll(ed, -1)
    } elif (verb == "edit.scroll_down") {
        text_edit_scroll(ed, 1)
    } elif (verb == "edit.move_lines_up") {
        text_edit_move_lines(ed, -1)
    } elif (verb == "edit.move_lines_down") {
        text_edit_move_lines(ed, 1)
    } elif (verb == "edit.duplicate_lines_up") {
        text_edit_duplicate_lines(ed, false)
    } elif (verb == "edit.duplicate_lines_down") {
        text_edit_duplicate_lines(ed, true)
    } elif (verb == "edit.delete_lines") {
        text_edit_delete_lines(ed)
    } elif (verb == "edit.insert_line_after") {
        text_edit_insert_line_after(ed)
    } elif (verb == "edit.insert_line_before") {
        text_edit_insert_line_before(ed)
    } elif (verb == "edit.indent_lines") {
        text_edit_indent_lines(ed)
    } elif (verb == "edit.outdent_lines") {
        text_edit_outdent_lines(ed)
    } elif (verb == "edit.expand_line_selection") {
        text_edit_expand_line_selection(ed)
    }
}
