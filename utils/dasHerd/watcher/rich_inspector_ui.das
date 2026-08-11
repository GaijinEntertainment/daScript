options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

module rich_inspector_ui

require imgui/imgui_harness
require imgui/imgui_icons
require imgui/imgui_drawlist_builtin
require imgui/imgui_terminal
require imgui/imgui_fonts
require imgui/imgui_docking_builtin
require imgui/imgui_table_builtin
require imgui/imgui_markdown_tree_sitter
require imgui/imgui_text_source_view
require imgui/imgui_text_tree_sitter
require imgui/imgui_text_find
require clipboard/clipboard
require daslib/safe_addr
require daslib/json_boost
require daslib/jobque_boost
require daslib/strings_boost
require live/live_commands
require ./watcher_core.das
require ./repository_core.das
require ./git_graph_model.das
require ./file_inspection.das
require ./image_preview_gl.das
require math
require strings
require ./rich_state.das
require ./rich_net.das
require ./rich_focus.das
// For the shared "start a session to fix this" escape on every error surface.
// The launcher does not depend on the inspector, so this stays acyclic.
require ./rich_launcher_ui.das

var FOCUS_HINT_TOOLTIP : NarrativeState

var INSPECTOR_STATUS_TEXT : NarrativeState

var INSPECTOR_STATUS_SEPARATOR : EmptyMarkerState

var INSPECTOR_ERROR_POPUP : PopupContextItemState

var INSPECTOR_ERROR_COPY : ToggleState

var INSPECTOR_ERROR_SEND : ToggleState

var PREVIEW_ERROR_POPUP : PopupContextItemState

var PREVIEW_ERROR_COPY : ToggleState

var PREVIEW_ERROR_SEND : ToggleState

var PREVIEW_ERROR_FIX : ToggleState

var INSPECTOR_ERROR_FIX : ToggleState

// put the current hunk's first row at the top on the next frame
def private scroll_to_current_hunk() : void {
    let row = g_inspector.parsed.hunks[g_inspector.diff_current_hunk].row_start
    g_inspector.diff_scroll_y = float(row) * max(
        GetTextLineHeight(), g_inspector.new_view.key.line_height)
    g_inspector.diff_scroll_jump_pending = true
}

// drive the pane's scroll from our own value while a jump is pending or the mouse is elsewhere,
// otherwise let the user's wheel win and read the position back
def private sync_diff_scroll() : void {
    let hovered = IsWindowHovered(ImGuiHoveredFlags.AllowWhenBlockedByActiveItem)
    if (g_inspector.diff_scroll_jump_pending || !hovered) {
        SetScrollY(g_inspector.diff_scroll_y)
    } else {
        g_inspector.diff_scroll_y = GetScrollY()
    }
}

def draw_error_context(var popup_state : PopupContextItemState;
                       var copy_state : ToggleState;
                       var send_state : ToggleState;
                       popup_id, context, message : string;
                       var fix_state : ToggleState;
                       worktree_path : string = "") {
    popup_context_item(popup_state,
            (str_id = popup_id, flags = ImGuiPopupFlags.MouseButtonRight)) {
        if (menu_item(copy_state, (text = "Copy error"))) {
            let copied = clipboard_set_text(message)
            g_status = (copied == ClipboardStatus.ok
                ? "Error copied" : "Clipboard unavailable")
        }
        if (menu_item(send_state, (text = "Send to session agent",
                enabled = g_client != null && g_client.connected
                    && !empty(g_client.selected_id)))) {
            g_client->post_error_request(context, message)
            g_status = "Error sent to session agent"
        }
        // When all else fails, on EVERY error surface, not just the ones that
        // happened to think of it: hand it to a fresh agent where it happened.
        if (menu_item(fix_state, (text = "Start a session to fix this...",
                enabled = g_client != null && g_client.connected))) {
            start_error_session(context, message, worktree_path)
        }
    }
}

def git_status_color(status : GitWindowStatus const) : float4 {
    return (status.error
        ? float4(1.0f, 0.35f, 0.30f, 1.0f)
        : (status.busy
            ? float4(0.56f, 0.59f, 0.64f, 1.0f)
            : float4(0.69f, 0.73f, 0.80f, 1.0f)))
}

// Content renderers size text from an explicit zoom against a FIXED base
// (fonts.size_px), not from the pushed font, so each names the content scale
// it belongs to: View text and Diff text are separately sized, and the UI
// scale reaches both once through FontScaleMain (note 55).
def source_view_style(kind : string;
                      diff_background : bool = false) : TextSourceViewStyle {
    return TextSourceViewStyle(
        fonts = g_fonts,
        zoom = content_zoom_scale(kind),
        wrap = false,
        line_mark_background_alpha = diff_background ? 0.11f : 0.0f,
        line_mark_width = 2.0f,
        write_system_clipboard = !harness_is_headless())
}

def syntax_cell_count(state : TextSourceViewState const) : int {
    var count = 0
    for (cell in state.cells) {
        if (cell.style_id > 1) count++
    }
    return count
}

def syntax_style_cell_count(state : TextSourceViewState const;
                            style : TextSourceSyntaxStyle) : int {
    var count = 0
    let style_id = 1 + int(style)
    for (cell in state.cells) {
        if (cell.style_id == style_id) count++
    }
    return count
}

def inspector_request_state_name(state : InspectorRequestState) : string {
    if (state == InspectorRequestState.queued) return "queued"
    if (state == InspectorRequestState.running) return "running"
    return "idle"
}

def inspector_content_state_name(state : InspectorContentState) : string {
    if (state == InspectorContentState.ready) return "ready"
    if (state == InspectorContentState.failed) return "failed"
    return "absent"
}

def draw_binary_preview() {
    if (g_inspector.preview_texture == 0u && g_inspector.view_prepared) {
        // git reports a binary diff (NUL content, or .gitattributes marking —
        // ds_parser.cpp is deliberately `binary` there), but the bytes decode
        // as text and the View payload is already fetched. Raw text is always
        // available; "Preview unavailable" over a readable file was the
        // live-review bug. Say why there is no diff, then show the file.
        text_disabled(INSPECTOR_BINARY_TEXT_NOTE, (text =
            "git treats this file as binary (content or .gitattributes), " +
            "so there is no diff - showing the current file text"))
        draw_file_view()
        return
    }
    text("Binary file ({g_inspector.preview_byte_count} bytes)")
    if (g_inspector.preview_texture == 0u) {
        text_disabled(INSPECTOR_BINARY_UNAVAILABLE,
            (text = empty(g_inspector.preview_error)
                ? "Preview unavailable"
                : "Preview unavailable: {g_inspector.preview_error}"))
        if (!empty(g_inspector.preview_error)) {
            draw_error_context(PREVIEW_ERROR_POPUP, PREVIEW_ERROR_COPY,
                PREVIEW_ERROR_SEND, "preview_error", "File Preview",
                g_inspector.preview_error, PREVIEW_ERROR_FIX)
        }
        return
    }
    text_disabled(INSPECTOR_BINARY_DIMENSIONS,
        (text = "Decoded by the bundled stb_image: " +
            "{g_inspector.preview_width} x {g_inspector.preview_height}"))
    let available = GetContentRegionAvail()
    let scale = min(1.0f, min(
        available.x / max(1.0f, float(g_inspector.preview_width)),
        available.y / max(1.0f, float(g_inspector.preview_height))))
    unsafe {
        image(INSPECTOR_BINARY_PREVIEW,
            (user_texture_id = inspector_preview_texture_ref(
                g_inspector.preview_texture),
             size = float2(max(1.0f, float(g_inspector.preview_width) * scale),
                           max(1.0f, float(g_inspector.preview_height) * scale)),
             uv0 = float2(0.0f, 0.0f), uv1 = float2(1.0f, 1.0f),
             tint_col = float4(1.0f, 1.0f, 1.0f, 1.0f),
             border_col = float4(0.35f, 0.35f, 0.35f, 1.0f)))
    }
}

def draw_side_by_side_diff() { // nolint:STYLE038
    if (g_inspector.parsed.binary) {
        draw_binary_preview()
        return
    }
    let hunk_count = length(g_inspector.parsed.hunks)
    g_inspector.diff_current_hunk = hunk_count > 0 ? clamp(g_inspector.diff_current_hunk, 0, hunk_count - 1) : 0
    let replacement_busy = (g_inspector.loading
        || g_inspector.prepare.request != InspectorRequestState.idle)
    with_disabled(replacement_busy) {
        SetNextItemWidth(150.0f)
        combo(DIFF_CONTEXT.PUBLIC,
            (text = "Context", items <- INSPECTOR_CONTEXT_LABELS))
        same_line()
        SetNextItemWidth(150.0f)
        combo(DIFF_SYNTAX.PUBLIC, (text = "Syntax", items <- INSPECTOR_SYNTAX_LABELS))
    }
    let selected_context = clamp(DIFF_CONTEXT.value, 0,
        length(INSPECTOR_CONTEXT_VALUES) - 1)
    if (!replacement_busy
            && selected_context != g_inspector.installed_diff_context_choice
            && g_client != null && g_client.connected) {
        g_client->inspect_file(g_inspector.installed_repository_id,
            g_inspector.installed_worktree_path, g_inspector.installed_file_path,
            g_inspector.installed_comparison, INSPECTOR_CONTEXT_VALUES[selected_context],
            g_inspector.installed_git_revision)
    }
    let selected_syntax = clamp(DIFF_SYNTAX.value, 0,
        length(INSPECTOR_SYNTAX_VALUES) - 1)
    if (!replacement_busy
            && selected_syntax != g_inspector.installed_syntax_choice) {
        g_inspector.repository_id = g_inspector.installed_repository_id
        g_inspector.worktree_path = g_inspector.installed_worktree_path
        g_inspector.file_path = g_inspector.installed_file_path
        g_inspector.comparison = g_inspector.installed_comparison
        g_inspector.git_revision = g_inspector.installed_git_revision
        g_inspector.diff_context = g_inspector.installed_diff_context
        g_inspector.diff_context_choice = g_inspector.installed_diff_context_choice
        g_inspector.syntax_choice = selected_syntax
        g_inspector.pending_select_diff = false
        if (!empty(g_inspector.source_patch)) {
            queue_inspector_prepare("Applying syntax...")
        }
    }
    same_line()
    if (small_button(DIFF_PREVIOUS_CHANGE, (text = "Previous change",
            enabled = g_inspector.diff_current_hunk > 0))) {
        g_inspector.diff_current_hunk = max(0,
            g_inspector.diff_current_hunk - 1)
        if (g_inspector.diff_current_hunk < hunk_count) {
            scroll_to_current_hunk()
        }
    }
    same_line()
    if (small_button(DIFF_NEXT_CHANGE, (text = "Next change",
            enabled = g_inspector.diff_current_hunk + 1 < hunk_count))) {
        g_inspector.diff_current_hunk = min(hunk_count - 1,
            g_inspector.diff_current_hunk + 1)
        if (g_inspector.diff_current_hunk >= 0
                && g_inspector.diff_current_hunk < hunk_count) {
            scroll_to_current_hunk()
        }
    }
    same_line()
    text_disabled(DIFF_CHANGE_POSITION, (text = hunk_count > 0
        ? "Change {g_inspector.diff_current_hunk + 1} of {hunk_count}"
        : "No text changes"))
    draw_inspector_find(true)

    let available = GetContentRegionAvail()
    let splitter_width = 8.0f
    let usable_width = max(2.0f, available.x - splitter_width)
    var left_width = usable_width * clamp(
        g_inspector.diff_split_fraction, 0.12f, 0.88f)
    var right_width = usable_width - left_width
    let origin = GetCursorScreenPos()
    let split_min = min(120.0f, usable_width * 0.4f)
    let split_box = float4(origin.x + left_width, origin.y,
        origin.x + left_width + splitter_width, origin.y + available.y)
    let splitting = SplitterBehavior(split_box, GetID("git_diff_splitter"),
        ImGuiAxis.X, left_width, right_width, split_min, split_min)
    g_inspector.diff_split_fraction = left_width / max(1.0f, left_width + right_width)

    child(DIFF_LEFT_PANE, (text = "before", size = float2(left_width, available.y),
            child_flags = ImGuiChildFlags.Borders,
            window_flags = ImGuiWindowFlags.NoScrollbar)) {
        content_zoom_wheel(CONTENT_DIFF)
        let window_pos = GetWindowPos()
        let window_size = GetWindowSize()
        g_inspector.diff_left_rect = float4(window_pos.x, window_pos.y,
            window_pos.x + window_size.x, window_pos.y + window_size.y)
        sync_diff_scroll()
        text_disabled("BEFORE")
        separator()
        var style = source_view_style(CONTENT_DIFF, true)
        let _ = text_source_view("git diff before", g_inspector.old_document,
            g_inspector.old_view, style)
    }
    same_line((spacing = 0.0f))
    dummy(DIFF_SPLITTER_SPACE, (size = float2(splitter_width, available.y)))
    let splitter_hot = IsItemHovered() || splitting
    if (splitter_hot) SetMouseCursor(ImGuiMouseCursor.ResizeEW)
    let splitter_color = GetColorU32(splitter_hot
        ? ImGuiCol.SeparatorActive : ImGuiCol.Separator)
    with_window_drawlist() $(var dl) {
        dl |> add_line(
            float2(origin.x + left_width + splitter_width * 0.5f, origin.y),
            float2(origin.x + left_width + splitter_width * 0.5f,
                origin.y + available.y), splitter_color, 1.0f)
    }
    same_line((spacing = 0.0f))
    child(DIFF_RIGHT_PANE, (text = "after", size = float2(right_width, available.y),
            child_flags = ImGuiChildFlags.Borders,
            window_flags = ImGuiWindowFlags.HorizontalScrollbar)) {
        content_zoom_wheel(CONTENT_DIFF)
        let window_pos = GetWindowPos()
        let window_size = GetWindowSize()
        let scrollbar_width = GetStyle().ScrollbarSize
        g_inspector.diff_right_rect = float4(window_pos.x, window_pos.y,
            window_pos.x + window_size.x, window_pos.y + window_size.y)
        g_inspector.diff_scrollbar_rect = float4(
            window_pos.x + window_size.x - scrollbar_width, window_pos.y,
            window_pos.x + window_size.x, window_pos.y + window_size.y)
        sync_diff_scroll()
        text_disabled("AFTER")
        separator()
        var style = source_view_style(CONTENT_DIFF, true)
        let result <- text_source_view("git diff after", g_inspector.new_document,
            g_inspector.new_view, style)
        draw_active_focus_overlay(g_inspector.new_view,
            g_inspector.new_document, true)
        if (result.selection_context_requested) {
            open_popup(DIFF_SELECTION_POPUP)
        }
        popup(DIFF_SELECTION_POPUP,
                (text = "DiffSelection", flags = ImGuiWindowFlags.None)) {
            let selection = text_source_view_selection(g_inspector.new_view)
            if (menu_item(DIFF_LOOK_AT_THAT,
                    (text = "Look at that", enabled = g_client != null
                        && g_client.connected && !empty(g_client.selected_id)
                        && selection.end_byte > selection.start_byte))) {
                let canonical_start = diff_byte_to_view_byte(
                    selection.start_byte, false)
                let canonical_end = diff_byte_to_view_byte(
                    selection.end_byte, true)
                g_client->post_selection_request(canonical_start, canonical_end)
                close_current_popup(DIFF_SELECTION_POPUP)
            }
        }
    }
    g_inspector.diff_scroll_jump_pending = false
}

def private inspector_markdown_shown() : bool {
    // Must match draw_file_view's presentation choice: an active focus target
    // renders markdown as source, and find jumps must land in the same view.
    return (g_inspector.resolved_language == "markdown"
        && !g_inspector.focus_document_valid)
}

def public inspector_find_source : string {
    // Whole-file search in EVERY mode: the compact diff's aligned text hides
    // most of the file, so searching it answers "not found" for text plainly
    // in the file (the VSCode diff-search trap). The AFTER pane text is the
    // fallback when the full View text is unavailable (binary transport).
    return (empty(g_inspector.view_source)
        ? g_inspector.new_document.source : g_inspector.view_source)
}

def private file_line_for_byte(byte_offset : int) : int {
    // Greatest view_line_starts entry <= byte_offset; index i is 1-based
    // file line i+1 (the focus overlay's convention).
    var low = 0
    var high = length(g_inspector.view_line_starts) - 1
    var line = 1
    while (low <= high) {
        let mid = (low + high) / 2
        if (g_inspector.view_line_starts[mid] <= byte_offset) {
            line = mid + 1
            low = mid + 1
        } else {
            high = mid - 1
        }
    }
    return line
}

def private diff_pane_match(found : TextSourceRange) : tuple<row : int; pane : TextSourceRange> {
    // Map a whole-file match onto the AFTER pane: file line, then the aligned
    // row showing that line, then the byte offset within the row (clamped to
    // the pane's line end). row -1 = the line is not in the visible diff.
    let file_line = file_line_for_byte(found.start_byte)
    if (file_line >= 1 && file_line <= length(g_inspector.view_line_starts)) {
        for (row in range(length(g_inspector.new_document.line_numbers))) {
            continue if (g_inspector.new_document.line_numbers[row] != file_line)
            break if (row >= length(g_inspector.new_line_starts))
            let line_start = g_inspector.view_line_starts[file_line - 1]
            let pane_line_start = g_inspector.new_line_starts[row]
            let pane_line_end = (row + 1 < length(g_inspector.new_line_starts)
                ? max(pane_line_start, g_inspector.new_line_starts[row + 1] - 1)
                : length(g_inspector.new_document.source))
            let pane_start = min(pane_line_end,
                pane_line_start + (found.start_byte - line_start))
            let pane_end = min(pane_line_end,
                pane_line_start + (found.end_byte - line_start))
            return (row = row, pane = TextSourceRange(
                start_byte = pane_start, end_byte = pane_end))
        }
    }
    return (row = -1, pane = TextSourceRange(start_byte = -1, end_byte = -1))
}

let FIND_CONTEXT_LINES = 3

def private old_row_for_byte(byte_offset : int) : int {
    // Greatest old_line_starts entry <= byte_offset = the BEFORE pane row.
    var low = 0
    var high = length(g_inspector.old_line_starts) - 1
    var row = 0
    while (low <= high) {
        let mid = (low + high) / 2
        if (g_inspector.old_line_starts[mid] <= byte_offset) {
            row = mid
            low = mid + 1
        } else {
            high = mid - 1
        }
    }
    return row
}

def private removed_row_for_match(found : TextSourceRange) : int {
    // The BEFORE-pane row this match sits on - only when the whole match is
    // inside ONE row whose old side is a removed line. Context rows duplicate
    // whole-file matches; multi-row regex spans have no single home.
    let row = old_row_for_byte(found.start_byte)
    return -1 if (row < 0 || row >= length(g_inspector.parsed.rows)
        || g_inspector.parsed.rows[row].old_kind != GitDiffLineKind.removed)
    let row_end = (row + 1 < length(g_inspector.old_line_starts)
        ? g_inspector.old_line_starts[row + 1] - 1
        : length(g_inspector.old_document.source))
    return found.end_byte <= row_end ? row : -1
}

def private removed_row_anchor_line(row : int) : int {
    // The new-file line the removed row visually follows: the nearest row at
    // or above it that shows a new-file line.
    var walk = min(row, length(g_inspector.new_document.line_numbers) - 1)
    while (walk >= 0) {
        let line = g_inspector.new_document.line_numbers[walk]
        return line if (line > 0)
        walk--
    }
    return 0
}

def private merge_find_matches(new_side, old_side : array<TextSourceRange>) {
    // Both lists are ascending in their own domain, so ascending in their
    // visual keys too - a two-pointer merge keeps diff order without a sort.
    // Removed rows key just after the new-file line they follow.
    let total = length(new_side) + length(old_side)
    g_inspector.find.matches |> reserve(total)
    g_inspector.find_match_old_side |> reserve(total)
    var new_at = 0
    var old_at = 0
    while (new_at < length(new_side) || old_at < length(old_side)) {
        var take_old = new_at >= length(new_side)
        if (!take_old && old_at < length(old_side)) {
            let new_key = file_line_for_byte(new_side[new_at].start_byte) * 2
            let old_key = removed_row_anchor_line(
                old_row_for_byte(old_side[old_at].start_byte)) * 2 + 1
            take_old = old_key < new_key
        }
        if (take_old) {
            g_inspector.find.matches |> push(old_side[old_at])
            g_inspector.find_match_old_side |> push(true)
            old_at++
        } else {
            g_inspector.find.matches |> push(new_side[new_at])
            g_inspector.find_match_old_side |> push(false)
            new_at++
        }
    }
}

def public inspector_find_rebuild(diff_mode : bool) {
    //! The herder's dual-domain rebuild: the whole new file always, plus the
    //! BEFORE pane's removed rows in diff mode (deleted text exists nowhere
    //! else). Both lists merge in visual diff order; `find_match_old_side`
    //! runs parallel to `find.matches` and names each match's pane.
    let revision = g_inspector.revision * 2 + (diff_mode ? 1 : 0)
    if (revision == g_inspector.find.matched_revision
            && g_inspector.find.query == g_inspector.find.matched_query
            && g_inspector.find.use_regex == g_inspector.find.matched_regex) {
        return
    }
    var previous_start = -1
    var previous_old = false
    if (g_inspector.find.current >= 0
            && g_inspector.find.current < length(g_inspector.find.matches)) {
        previous_start = g_inspector.find.matches[g_inspector.find.current].start_byte
        previous_old = (g_inspector.find.current < length(g_inspector.find_match_old_side)
            && g_inspector.find_match_old_side[g_inspector.find.current])
    }
    g_inspector.find.matched_revision = revision
    g_inspector.find.matched_query = g_inspector.find.query
    g_inspector.find.matched_regex = g_inspector.find.use_regex
    delete g_inspector.find.matches
    delete g_inspector.find_match_old_side
    g_inspector.find.current = -1

    var inscope new_side : array<TextSourceRange>
    let new_error = text_find_collect(g_inspector.find.query,
        g_inspector.find.use_regex, inspector_find_source(), new_side)
    var inscope old_side : array<TextSourceRange>
    var old_error = ""
    if (diff_mode && !empty(g_inspector.view_source)) {
        var inscope raw_old : array<TextSourceRange>
        old_error = text_find_collect(g_inspector.find.query,
            g_inspector.find.use_regex, g_inspector.old_document.source, raw_old)
        for (candidate in raw_old) {
            if (removed_row_for_match(candidate) >= 0) {
                old_side |> push(candidate)
            }
        }
    }
    g_inspector.find.error = empty(new_error) ? old_error : new_error
    merge_find_matches(new_side, old_side)
    if (!empty(g_inspector.find.matches)) {
        g_inspector.find.current = 0
        if (previous_start >= 0) {
            for (index in range(length(g_inspector.find.matches))) {
                if (g_inspector.find_match_old_side[index] == previous_old
                        && g_inspector.find.matches[index].start_byte >= previous_start) {
                    g_inspector.find.current = index
                    break
                }
            }
        }
    }
    g_inspector.find.jump_revision++
}

def private request_find_context_window(found : TextSourceRange) {
    // The match is real but its region is not in the visible diff: ask the
    // prepare to display a context window around it - the diff analog of
    // fold auto-expand. The jump stays pending (not consumed) and lands once
    // the re-prepared rows install. Windows accumulate, and every push
    // re-queues, so when the prepare is idle every listed window is already
    // in the installed rows.
    inspector_reset_find_windows_if_stale()
    let line = file_line_for_byte(found.start_byte)
    for (existing in g_inspector.find_windows) {
        if (line >= existing.x && line <= existing.y) {
            // Already requested. Still in flight: keep the jump pending.
            // Idle: the installed rows include this window yet the match
            // still cannot map - consume the jump so it stops retrying.
            if (!g_inspector.loading
                    && g_inspector.prepare.request == InspectorRequestState.idle) {
                g_inspector.find_applied_jump = g_inspector.find.jump_revision
            }
            return
        }
    }
    g_inspector.find_windows |> push(int2(
        max(1, line - FIND_CONTEXT_LINES), line + FIND_CONTEXT_LINES))
    queue_inspector_prepare("Expanding diff for find...")
}

def public inspector_apply_find_jump(diff_mode : bool) {
    return if (g_inspector.find.jump_revision == g_inspector.find_applied_jump)
    let found = text_find_current(g_inspector.find)
    if (found.start_byte < 0) {
        g_inspector.find_applied_jump = g_inspector.find.jump_revision
        return
    }
    if (diff_mode) {
        let old_side = (g_inspector.find.current >= 0
            && g_inspector.find.current < length(g_inspector.find_match_old_side)
            && g_inspector.find_match_old_side[g_inspector.find.current])
        if (old_side) {
            // Deleted text: the match bytes are BEFORE-pane-local already;
            // removed rows are always displayed, so no window is ever needed.
            g_inspector.find_applied_jump = g_inspector.find.jump_revision
            text_source_view_clear_selection(g_inspector.new_view)
            text_source_view_select_range(g_inspector.old_document,
                g_inspector.old_view, found.start_byte, found.end_byte)
            let row = old_row_for_byte(found.start_byte)
            g_inspector.diff_scroll_y = max(0.0f, (float(row) - 6.0f) * max(
                GetTextLineHeight(), g_inspector.old_view.key.line_height))
            g_inspector.diff_scroll_jump_pending = true
            return
        }
        if (empty(g_inspector.view_source)) {
            // Fallback search domain IS the aligned pane text - the match's
            // bytes are already pane-local.
            g_inspector.find_applied_jump = g_inspector.find.jump_revision
            text_source_view_select_range(g_inspector.new_document,
                g_inspector.new_view, found.start_byte, found.end_byte)
            return
        }
        let mapped = diff_pane_match(found)
        if (mapped.row >= 0) {
            // Both panes share diff_scroll_y, so the hunk-jump scroll
            // machinery lands the aligned row in view.
            g_inspector.find_applied_jump = g_inspector.find.jump_revision
            text_source_view_clear_selection(g_inspector.old_view)
            text_source_view_select_range(g_inspector.new_document,
                g_inspector.new_view, mapped.pane.start_byte, mapped.pane.end_byte)
            g_inspector.diff_scroll_y = max(0.0f, (float(mapped.row) - 6.0f) * max(
                GetTextLineHeight(), g_inspector.new_view.key.line_height))
            g_inspector.diff_scroll_jump_pending = true
            return
        }
        request_find_context_window(found)
        return
    }
    g_inspector.find_applied_jump = g_inspector.find.jump_revision
    if (inspector_markdown_shown()) {
        g_inspector.find_scroll_markdown = markdown_view_select_source_range(
            g_inspector.markdown_view, found.start_byte, found.end_byte)
    } else {
        let _expanded = text_source_view_expand_for_byte(
            g_inspector.view_document, g_inspector.view, found.start_byte)
        text_source_view_select_range(g_inspector.view_document,
            g_inspector.view, found.start_byte, found.end_byte)
        g_inspector.find_scroll_byte = found.start_byte
    }
}

def private draw_inspector_find(diff_mode : bool) {
    return if (!g_inspector.find.open)
    text_find_bar(1, g_inspector.find)
    inspector_find_rebuild(diff_mode)
    inspector_apply_find_jump(diff_mode)
    if (g_inspector.find.open
            && IsWindowFocused(ImGuiFocusedFlags.RootAndChildWindows)
            && IsKeyPressed(ImGuiKey.Escape, false)) {
        text_find_close(g_inspector.find)
    }
}

def private apply_inspector_find_scroll() {
    // Inside the view child. No GetScrollMaxY clamp: it reports LAST frame's
    // range; ImGui clamps SetScrollY against the real height at frame end.
    // A jump that arrives before the View tab first laid out stays pending
    // (line_top < 0) and lands on the next frame - the diff-to-View reveal
    // path switches tabs and jumps in the same act.
    return if (g_inspector.find_scroll_byte < 0)
    let line_top = text_source_view_line_top(g_inspector.view,
        g_inspector.find_scroll_byte)
    return if (line_top < 0.0f)
    let height = GetWindowSize().y
    let local_top = line_top - GetScrollY()
    if (local_top < height * 0.1f || local_top > height * 0.9f) {
        SetScrollY(max(0.0f, line_top - height * 0.35f))
    }
    g_inspector.find_scroll_byte = -1
}

def private apply_inspector_markdown_find_scroll() {
    // The view's caret-visible pass edge-scrolls at best; a find jump places
    // the selection focus in the comfortable band instead.
    return if (!g_inspector.find_scroll_markdown)
    g_inspector.find_scroll_markdown = false
    let focus = markdown_view_selection_focus_screen_y(g_inspector.markdown_view)
    return if (!focus.valid)
    let view_top = GetWindowPos().y
    let height = GetWindowSize().y
    if (focus.y < view_top + height * 0.1f
            || focus.y > view_top + height * 0.9f) {
        SetScrollY(max(0.0f, GetScrollY() + focus.y
            - (view_top + height * 0.35f)))
    }
}

def draw_file_view() {
    if (!g_inspector.view_prepared) {
        draw_binary_preview()
        return
    }
    draw_inspector_find(false)
    child(FILE_VIEW_PANE, (text = "file view", size = float2(0.0f, 0.0f),
            child_flags = ImGuiChildFlags.None,
            window_flags = ImGuiWindowFlags.HorizontalScrollbar)) {
        content_zoom_wheel(CONTENT_VIEW)
        if (inspector_markdown_shown()) {
            var style = MarkdownViewStyle(
                fonts = g_fonts,
                typography = MarkdownTypography(
                    inline_code_scale = 0.84f, code_block_scale = 0.90f),
                zoom = content_zoom_scale(CONTENT_VIEW),
                write_system_clipboard = !harness_is_headless())
            let _ <- markdown_view_with_change_gutter("git markdown view",
                g_inspector.markdown_document, g_inspector.markdown_view,
                style, g_inspector.revision, g_inspector.changed_source_ranges,
                g_inspector.markdown_gutter_segments,
                max(1.0f, 2.0f * content_zoom_scale(CONTENT_VIEW)))
            apply_inspector_markdown_find_scroll()
        } else {
            if (g_focus_view_scroll_pending) {
                SetScrollY(max(0.0f, g_focus_view_scroll_y))
                g_focus_view_scroll_pending = false
            }
            var style = source_view_style(CONTENT_VIEW)
            let result <- text_source_view("git file view",
                g_inspector.view_document, g_inspector.view, style)
            apply_inspector_find_scroll()
            draw_active_focus_overlay(g_inspector.view,
                g_inspector.view_document, false)
            if (result.selection_context_requested) {
                open_popup(INSPECTOR_SELECTION_POPUP)
            }
            popup(INSPECTOR_SELECTION_POPUP,
                    (text = "InspectorSelection", flags = ImGuiWindowFlags.None)) {
                let selection = text_source_view_selection(g_inspector.view)
                if (menu_item(INSPECTOR_LOOK_AT_THAT,
                        (text = "Look at that", enabled = g_client != null
                            && g_client.connected && !empty(g_client.selected_id)
                            && selection.end_byte > selection.start_byte))) {
                    g_client->post_selection_request(
                        selection.start_byte, selection.end_byte)
                    close_current_popup(INSPECTOR_SELECTION_POPUP)
                }
            }
        }
    }
}

def file_inspector_status() : GitWindowStatus {
    if (g_client == null || !g_client.connected) {
        return GitWindowStatus(text = "WATCHER DISCONNECTED - reconnecting...", error = true)
    }
    let retained = g_inspector.prepare.has_retained_content
    let showing = retained ? " - showing {g_inspector.installed_file_path}" : ""
    if (g_inspector.loading) {
        let phase = empty(g_inspector.status) ? "Reading Git diff..." : g_inspector.status
        return GitWindowStatus(
            text = "LOADING {g_inspector.file_path} - {phase}{showing}", busy = true)
    }
    if (g_inspector.prepare.request != InspectorRequestState.idle) {
        let phase = (empty(g_inspector.prepare.status)
            ? "Preparing view..." : g_inspector.prepare.status)
        return GitWindowStatus(
            text = "PREPARING {g_inspector.file_path} - {phase}{showing}", busy = true)
    }
    if (!empty(g_inspector.error)) {
        let task = (empty(g_inspector.task_session_id)
            ? "" : "   TASK {g_inspector.task_session_id}")
        return GitWindowStatus(
            text = "FAILED {g_inspector.file_path} - {g_inspector.error}{showing}{task}", error = true)
    }
    if (retained) {
        let capped = (g_inspector.syntax_capped
            ? "   SYNTAX OFF (file too large or lines too long)" : "")
        return GitWindowStatus(text = "{to_upper(g_inspector.installed_comparison)}  {g_inspector.installed_file_path}   HUNKS {length(g_inspector.parsed.hunks)}   PREP {g_inspector.prepare_elapsed_usec / 1000l} ms{capped}")
    }
    return GitWindowStatus(text = "Select a changed file")
}

def mouse_inside(p0, p1 : float2) : bool {
    let mouse = GetMousePos()
    return (mouse.x >= p0.x && mouse.x <= p1.x
        && mouse.y >= p0.y && mouse.y <= p1.y)
}

def draw_focus_target_overlay(target : WatcherFocusTarget const; // nolint:STYLE038
                              state : TextSourceViewState const;
                              document : TextSourceDocument const;
                              diff_mapping : bool) {
    let dim_color = GetColorU32(float4(0.015f, 0.018f, 0.024f, 0.48f))
    let focus_fill = GetColorU32(float4(1.0f, 0.72f, 0.18f, 0.13f))
    let focus_outline = GetColorU32(float4(1.0f, 0.76f, 0.24f, 0.92f))
    let clip_min = GetWindowPos()
    let clip_max = clip_min + GetWindowSize()
    var hover_caption = ""
    with_window_drawlist() $(var dl) {
        dl |> with_drawlist_clip_rect(clip_min, clip_max, true) {
            if (state.virtual_mode && !empty(state.virtual_line_starts)) {
                let line_height = (state.screen_size.y
                    / float(length(state.virtual_line_starts)))
                for (line_index in range(length(state.virtual_line_starts))) {
                    let display_start = state.virtual_line_starts[line_index]
                    let display_end = (line_index + 1 < length(state.virtual_line_starts)
                        ? max(display_start,
                            state.virtual_line_starts[line_index + 1] - 1)
                        : length(document.source))
                    var source_start = display_start
                    var source_end = display_end
                    var mapped = !diff_mapping
                    if (diff_mapping && line_index < length(document.line_numbers)) {
                        let source_line = document.line_numbers[line_index]
                        if (source_line > 0
                                && source_line <= length(g_inspector.view_line_starts)) {
                            source_start = g_inspector.view_line_starts[source_line - 1]
                            source_end = (source_line < length(g_inspector.view_line_starts)
                                ? max(source_start,
                                    g_inspector.view_line_starts[source_line] - 1)
                                : length(g_inspector.view_document.source))
                            mapped = true
                        }
                    }
                    let y0 = state.screen_origin.y + float(line_index) * line_height
                    let p0 = float2(state.screen_origin.x, y0)
                    let p1 = float2(state.screen_origin.x + state.screen_size.x,
                        y0 + line_height)
                    if (p1.y < clip_min.y || p0.y > clip_max.y) continue
                    let focused = mapped && focus_intersects(target,
                        source_start, max(source_start + 1, source_end))
                    if (!focused) {
                        dl |> add_rect_filled(p0, p1, dim_color)
                    } else {
                        dl |> add_rect_filled(p0, p1, focus_fill)
                        let marker_right = (state.screen_origin.x
                            - max(2.0f, GetStyle().ItemSpacing.x * 0.35f))
                        dl |> add_rect_filled(
                            float2(marker_right - 3.0f, p0.y),
                            float2(marker_right, p1.y), focus_outline)
                        let caption = focus_caption(target,
                            source_start, max(source_start + 1, source_end))
                        if (empty(hover_caption) && !empty(caption)
                                && mouse_inside(p0, p1)) {
                            hover_caption = caption
                        }
                    }
                }
                return
            }
            var last_focused_line = -1
            for (cell in state.cells) {
                var source_start = cell.source.start_byte
                var source_end = cell.source.end_byte
                var mapped = !diff_mapping
                if (diff_mapping && cell.line_index >= 0
                        && cell.line_index < length(document.line_numbers)
                        && cell.line_index < length(g_inspector.new_line_starts)) {
                    let source_line = document.line_numbers[cell.line_index]
                    if (source_line > 0
                            && source_line <= length(g_inspector.view_line_starts)) {
                        let canonical_line_start = g_inspector.view_line_starts[source_line - 1]
                        let canonical_line_end = (source_line
                                < length(g_inspector.view_line_starts)
                            ? max(canonical_line_start,
                                g_inspector.view_line_starts[source_line] - 1)
                            : length(g_inspector.view_document.source))
                        source_start = min(canonical_line_end,
                            canonical_line_start + max(0,
                                cell.source.start_byte
                                    - g_inspector.new_line_starts[cell.line_index]))
                        source_end = min(canonical_line_end,
                            canonical_line_start + max(0,
                                cell.source.end_byte
                                    - g_inspector.new_line_starts[cell.line_index]))
                        mapped = true
                    }
                }
                let p0 = state.screen_origin + float2(cell.rect.x, cell.rect.y)
                let p1 = state.screen_origin + float2(cell.rect.z, cell.rect.w)
                let focused = mapped && focus_intersects(target,
                    source_start, max(source_start + 1, source_end))
                if (!focused) {
                    dl |> add_rect_filled(p0, p1, dim_color)
                } else {
                    dl |> add_rect_filled(p0, p1, focus_fill)
                    if (cell.line_index != last_focused_line) {
                        let marker_right = (state.screen_origin.x
                            - max(2.0f, GetStyle().ItemSpacing.x * 0.35f))
                        dl |> add_rect_filled(
                            float2(marker_right - 3.0f, p0.y),
                            float2(marker_right, p1.y), focus_outline)
                        last_focused_line = cell.line_index
                    }
                    let caption = focus_caption(target,
                        source_start, max(source_start + 1, source_end))
                    let hover_p0 = float2(state.screen_origin.x, p0.y)
                    let hover_p1 = float2(state.screen_origin.x
                        + state.screen_size.x, p1.y)
                    if (empty(hover_caption) && !empty(caption)
                            && mouse_inside(hover_p0, hover_p1)) {
                        hover_caption = caption
                    }
                }
            }
        }
    }
    if (!empty(hover_caption)) {
        set_tooltip(FOCUS_HINT_TOOLTIP, hover_caption)
    }
}

def draw_active_focus_overlay(state : TextSourceViewState const;
                              document : TextSourceDocument const;
                              diff_mapping : bool) {
    if (!g_inspector.focus_document_valid || g_client == null) return
    for (message in g_client.mailbox) {
        if (message.id != g_active_focus_message
                || g_active_focus_target < 0
                || g_active_focus_target >= length(message.focus.targets)) continue
        let target & = unsafe(message.focus.targets[g_active_focus_target])
        if (watcher_focus_repository(message.focus, target)
                    == g_inspector.installed_repository_id
                && watcher_focus_worktree(message.focus, target)
                    == g_inspector.installed_worktree_path
                && target.file_path == g_inspector.installed_file_path) {
            draw_focus_target_overlay(target, state, document, diff_mapping)
        }
        return
    }
}

def apply_active_focus() {
    if (g_client == null) return
    let identity = ("{g_active_focus_message}:{g_active_focus_target}:" +
        "{g_active_focus_range}:" +
        "{g_focus_visible}:{g_inspector.revision}:{g_inspector.installed_file_path}")
    if (identity == g_applied_focus_identity && !g_pending_focus_scroll) return
    g_inspector.focus_document_valid = false
    g_inspector.focus_start_byte = -1
    var matched_target = false
    if (g_focus_visible && !empty(g_active_focus_message)) {
        for (message in g_client.mailbox) {
            if (message.id != g_active_focus_message
                    || g_active_focus_target < 0
                    || g_active_focus_target >= length(message.focus.targets)) continue
            let target & = unsafe(message.focus.targets[g_active_focus_target])
            if (watcher_focus_repository(message.focus, target)
                        != g_inspector.installed_repository_id
                    || watcher_focus_worktree(message.focus, target)
                        != g_inspector.installed_worktree_path
                    || target.file_path != g_inspector.installed_file_path) continue
            matched_target = true
            g_inspector.focus_document_valid = true
            if (!empty(target.ranges)) {
                if (g_active_focus_range < 0
                        || g_active_focus_range >= length(target.ranges)) {
                    g_active_focus_range = first_focus_range(target)
                }
                g_inspector.focus_start_byte = target.ranges[
                    g_active_focus_range].start_byte
            } else {
                g_inspector.focus_start_byte = 0
            }
            if (g_pending_focus_scroll) {
                var line_index = 0
                peek_data(g_inspector.view_document.source) $(bytes) {
                    for (index in range(clamp(g_inspector.focus_start_byte,
                            0, length(bytes)))) {
                        if (int(bytes[index]) == '\n') {
                            line_index++
                        }
                    }
                }
                var diff_row = line_index
                for (row_index in range(length(g_inspector.parsed.rows))) {
                    if (g_inspector.parsed.rows[row_index].new_line
                            == line_index + 1) {
                        diff_row = row_index
                        break
                    }
                }
                let diff_line_height = max(GetTextLineHeight(),
                    g_inspector.new_view.key.line_height)
                let view_line_height = max(GetTextLineHeight(),
                    g_inspector.view.key.line_height)
                g_inspector.diff_scroll_y = (float(max(0, diff_row - 1))
                    * diff_line_height)
                g_focus_view_scroll_y = (float(max(0, line_index - 1))
                    * view_line_height)
                g_inspector.diff_scroll_jump_pending = true
                g_focus_view_scroll_pending = true
            }
            break
        }
    }
    if (!g_pending_focus_scroll || matched_target) {
        g_pending_focus_scroll = false
        g_applied_focus_identity = identity
    } else {
        // Replacement inspection keeps the old document alive. Do not consume
        // the one-shot navigation until the requested Focus target installs.
        g_applied_focus_identity = ""
    }
}

def draw_file_inspector_content() { // nolint:STYLE038
    if (g_inspector.pending_widget_sync) {
        DIFF_CONTEXT.value = g_inspector.installed_diff_context_choice
        DIFF_SYNTAX.value = g_inspector.installed_syntax_choice
        if (g_inspector.pending_select_diff_tab) {
            INSPECTOR_DIFF_TAB.pending_select = true
        }
        if (g_inspector.pending_select_view_tab) {
            INSPECTOR_VIEW_TAB.pending_select = true
        }
        g_inspector.pending_select_diff_tab = false
        g_inspector.pending_select_view_tab = false
        g_inspector.pending_widget_sync = false
    }
    if (!g_inspector.prepare.has_retained_content) {
        if (empty(g_inspector.file_path)) {
            text_disabled("Select a changed file")
        }
        return
    }
    apply_active_focus()
    text(INSPECTOR_PATH, (text = g_inspector.installed_file_path))
    same_line()
    text_disabled(INSPECTOR_SCOPE,
        (text = to_upper(g_inspector.installed_comparison)))
    // The inspector does NOT watch the disk: content is captured at inspect
    // time. Until auto-reload lands (backlog), the refresh is one click.
    if (!empty(g_inspector.installed_file_path)) {
        same_line()
        let inspect_busy = (g_inspector.loading
            || g_inspector.prepare.request != InspectorRequestState.idle)
        if (icon_button(INSPECTOR_REFRESH, (glyph = "refresh",
                tip = "Re-read this file (the view does not follow disk changes)",
                size = 16.0f))
                && !inspect_busy && g_client != null && g_client.connected) {
            g_client->inspect_file(g_inspector.installed_repository_id,
                g_inspector.installed_worktree_path,
                g_inspector.installed_file_path,
                g_inspector.installed_comparison,
                g_inspector.installed_diff_context,
                g_inspector.installed_git_revision)
        }
    }
    if (!empty(g_active_focus_message)) {
        same_line()
        if (arrow_button(FOCUS_PREVIOUS,
                (text = "##previous-focused-file", dir = ImGuiDir.Left))) {
            navigate_active_focus(-1)
        }
        set_item_tooltip(FOCUS_PREVIOUS_TIP,
            (text = "Previous focused file"))
        same_line()
        if (arrow_button(FOCUS_NEXT,
                (text = "##next-focused-file", dir = ImGuiDir.Right))) {
            navigate_active_focus(1)
        }
        set_item_tooltip(FOCUS_NEXT_TIP, (text = "Next focused file"))
        if (active_focus_range_count() > 1) {
            same_line()
            if (arrow_button(FOCUS_RANGE_PREVIOUS,
                    (text = "##previous-highlight", dir = ImGuiDir.Up))) {
                navigate_active_focus_range(-1)
            }
            set_item_tooltip(FOCUS_RANGE_PREVIOUS_TIP,
                (text = "Previous highlight in file"))
            same_line()
            if (arrow_button(FOCUS_RANGE_NEXT,
                    (text = "##next-highlight", dir = ImGuiDir.Down))) {
                navigate_active_focus_range(1)
            }
            set_item_tooltip(FOCUS_RANGE_NEXT_TIP,
                (text = "Next highlight in file"))
        }
        same_line()
        if (icon_button(FOCUS_TOGGLE,
                (glyph = g_focus_visible ? "visible" : "hidden",
                 tip = g_focus_visible ? "Hide focus" : "Show focus",
                 size = 16.0f))) {
            g_focus_visible = !g_focus_visible
            g_applied_focus_identity = ""
        }
    }
    tab_bar(INSPECTOR_MODE_TABS,
            (text = "InspectorModes", flags = ImGuiTabBarFlags.None)) {
        tab_item(INSPECTOR_DIFF_TAB.PUBLIC, (text = "Diff", closable = false,
                flags = ImGuiTabItemFlags.None)) {
            // A mode lag here means the TAB changed first — a user click.
            // Programmatic switches set mode before selecting the tab.
            if (g_inspector.mode != GitInspectorMode.diff) {
                g_inspector.mode = GitInspectorMode.diff
                g_inspector.sticky_mode = GitInspectorMode.diff
            }
            draw_side_by_side_diff()
        }
        tab_item(INSPECTOR_VIEW_TAB.PUBLIC, (text = "View", closable = false,
                flags = ImGuiTabItemFlags.None)) {
            if (g_inspector.mode != GitInspectorMode.view) {
                g_inspector.mode = GitInspectorMode.view
                g_inspector.sticky_mode = GitInspectorMode.view
            }
            draw_file_view()
        }
    }
}

def draw_file_inspector_window() {
    if (IsWindowFocused(ImGuiFocusedFlags.RootAndChildWindows)) {
        let io & = unsafe(GetIO())
        // io.KeyCtrl is logical — ImGui maps Cmd onto it under ConfigMacOSXBehaviors.
        if (io.KeyCtrl && IsKeyPressed(ImGuiKey.F, false)) {
            text_find_open(g_inspector.find)
        }
    }
    child(FILE_INSPECTOR_CONTENT, (text = "inspector content",
            size = float2(0.0f, -GetFrameHeightWithSpacing()),
            child_flags = ImGuiChildFlags.None,
            window_flags = ImGuiWindowFlags.None)) {
        draw_file_inspector_content()
    }
    let status = file_inspector_status()
    separator(INSPECTOR_STATUS_SEPARATOR)
    if (status.error || status.busy) {
        icon(status.error ? "warning" : "clock", GetTextLineHeight())
        same_line()
    }
    text_colored(INSPECTOR_STATUS_TEXT,
        (color = git_status_color(status), text = status.text))
    if (status.error) {
        draw_error_context(INSPECTOR_ERROR_POPUP, INSPECTOR_ERROR_COPY,
            INSPECTOR_ERROR_SEND, "inspector_error", "File Inspector",
            status.text, INSPECTOR_ERROR_FIX)
    }
}

def inspector_ui_init() {
    DIFF_SYNTAX.value = 0
    DIFF_CONTEXT.value = 0
}

def draw_file_inspector_dock() {
    dock_window(FILE_INSPECTOR_WIN.PUBLIC, (text = "File Inspector", closable = true,
            flags = ImGuiWindowFlags.None)) {
        draw_file_inspector_window()
    }
}
