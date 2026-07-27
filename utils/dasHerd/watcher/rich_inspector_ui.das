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

var FOCUS_HINT_TOOLTIP : NarrativeState

var INSPECTOR_STATUS_TEXT : NarrativeState

var INSPECTOR_STATUS_SEPARATOR : EmptyMarkerState

var INSPECTOR_ERROR_POPUP : PopupContextItemState

var INSPECTOR_ERROR_COPY : ToggleState

var INSPECTOR_ERROR_SEND : ToggleState

var PREVIEW_ERROR_POPUP : PopupContextItemState

var PREVIEW_ERROR_COPY : ToggleState

var PREVIEW_ERROR_SEND : ToggleState

def draw_error_context(var popup_state : PopupContextItemState;
                       var copy_state : ToggleState;
                       var send_state : ToggleState;
                       popup_id, context, message : string) {
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
    text("Binary file ({g_inspector.preview_byte_count} bytes)")
    if (g_inspector.preview_texture == 0u) {
        text_disabled(INSPECTOR_BINARY_UNAVAILABLE,
            (text = empty(g_inspector.preview_error)
                ? "Preview unavailable"
                : "Preview unavailable: {g_inspector.preview_error}"))
        if (!empty(g_inspector.preview_error)) {
            draw_error_context(PREVIEW_ERROR_POPUP, PREVIEW_ERROR_COPY,
                PREVIEW_ERROR_SEND, "preview_error", "File Preview",
                g_inspector.preview_error)
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

def draw_side_by_side_diff() {
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
            let row = g_inspector.parsed.hunks[g_inspector.diff_current_hunk].row_start
            g_inspector.diff_scroll_y = float(row) * max(
                GetTextLineHeight(), g_inspector.new_view.key.line_height)
            g_inspector.diff_scroll_jump_pending = true
        }
    }
    same_line()
    if (small_button(DIFF_NEXT_CHANGE, (text = "Next change",
            enabled = g_inspector.diff_current_hunk + 1 < hunk_count))) {
        g_inspector.diff_current_hunk = min(hunk_count - 1,
            g_inspector.diff_current_hunk + 1)
        if (g_inspector.diff_current_hunk >= 0
                && g_inspector.diff_current_hunk < hunk_count) {
            let row = g_inspector.parsed.hunks[g_inspector.diff_current_hunk].row_start
            g_inspector.diff_scroll_y = float(row) * max(
                GetTextLineHeight(), g_inspector.new_view.key.line_height)
            g_inspector.diff_scroll_jump_pending = true
        }
    }
    same_line()
    text_disabled(DIFF_CHANGE_POSITION, (text = hunk_count > 0
        ? "Change {g_inspector.diff_current_hunk + 1} of {hunk_count}"
        : "No text changes"))

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
        let hovered = IsWindowHovered(ImGuiHoveredFlags.AllowWhenBlockedByActiveItem)
        if (g_inspector.diff_scroll_jump_pending || !hovered) {
            SetScrollY(g_inspector.diff_scroll_y)
        } else {
            g_inspector.diff_scroll_y = GetScrollY()
        }
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
        let hovered = IsWindowHovered(ImGuiHoveredFlags.AllowWhenBlockedByActiveItem)
        if (g_inspector.diff_scroll_jump_pending || !hovered) {
            SetScrollY(g_inspector.diff_scroll_y)
        } else {
            g_inspector.diff_scroll_y = GetScrollY()
        }
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

var g_search_matches : array<int>
var g_search_current : int
var g_search_identity : string

// Find matches (case-insensitive) in the view text; one peek pass assigns a
// 0-based line to each match offset.
def private rebuild_inspector_search(text, query : string) {
    delete g_search_matches
    g_search_current = 0
    return if (empty(query) || empty(text))
    let hay = to_lower(text)
    let needle = to_lower(query)
    var inscope offsets : array<int>
    var start = 0
    while (true) {
        let at = find(hay, needle, start)
        break if (at < 0)
        offsets |> push(at)
        start = at + max(1, length(needle))
    }
    return if (empty(offsets))
    peek_data(hay) $(arr) {
        var line = 0
        var next = 0
        for (index in range(length(arr))) {
            if (next < length(offsets) && index == offsets[next]) {
                g_search_matches |> push(line)
                next++
            }
            if (int(arr[index]) == '\n') line++
        }
    }
}

def private inspector_search_jump() {
    return if (empty(g_search_matches) || g_search_current < 0
        || g_search_current >= length(g_search_matches))
    let row = g_search_matches[g_search_current]
    g_focus_view_scroll_y = (float(row) * max(GetTextLineHeight(),
        g_inspector.view.key.line_height))
    g_focus_view_scroll_pending = true
}

// View-mode search (note 13). Diff panes align virtual rows, so a text-line
// jump would land wrong there — diff search waits on a line->row map.
def private draw_inspector_search() {
    SetNextItemWidth(220.0f)
    let _changed = input_text_growable(INSPECTOR_SEARCH, (text = "Find"))
    let query = strip(INSPECTOR_SEARCH.value)
    let identity = "{g_inspector.revision}:{g_inspector.installed_file_path}:{query}"
    if (identity != g_search_identity) {
        g_search_identity = identity
        rebuild_inspector_search(g_inspector.view_source, query)
        if (!empty(g_search_matches)) inspector_search_jump()
    }
    let matches = length(g_search_matches)
    same_line()
    if (small_button(INSPECTOR_SEARCH_PREV, (text = "<", enabled = matches > 1))) {
        g_search_current = (g_search_current + matches - 1) % matches
        inspector_search_jump()
    }
    same_line()
    if (small_button(INSPECTOR_SEARCH_NEXT, (text = ">", enabled = matches > 1))) {
        g_search_current = (g_search_current + 1) % matches
        inspector_search_jump()
    }
    if (!empty(query)) {
        same_line()
        if (matches > 0) {
            text_disabled(INSPECTOR_SEARCH_COUNT,
                (text = "{g_search_current + 1} of {matches}"))
        } else {
            text_colored(INSPECTOR_SEARCH_NONE,
                (color = token_warning_color(), text = "no matches"))
        }
    }
}

def draw_file_view() {
    if (!g_inspector.view_prepared) {
        draw_binary_preview()
        return
    }
    if (g_inspector.resolved_language != "markdown") {
        draw_inspector_search()
    }
    child(FILE_VIEW_PANE, (text = "file view", size = float2(0.0f, 0.0f),
            child_flags = ImGuiChildFlags.None,
            window_flags = ImGuiWindowFlags.HorizontalScrollbar)) {
        content_zoom_wheel(CONTENT_VIEW)
        if (g_inspector.resolved_language == "markdown"
                && !g_inspector.focus_document_valid) {
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
        } else {
            if (g_focus_view_scroll_pending) {
                SetScrollY(max(0.0f, g_focus_view_scroll_y))
                g_focus_view_scroll_pending = false
            }
            var style = source_view_style(CONTENT_VIEW)
            let result <- text_source_view("git file view",
                g_inspector.view_document, g_inspector.view, style)
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
        return GitWindowStatus(text = "{to_upper(g_inspector.installed_comparison)}  {g_inspector.installed_file_path}   HUNKS {length(g_inspector.parsed.hunks)}   PREP {g_inspector.prepare_elapsed_usec / 1000l} ms")
    }
    return GitWindowStatus(text = "Select a changed file")
}

def mouse_inside(p0, p1 : float2) : bool {
    let mouse = GetMousePos()
    return (mouse.x >= p0.x && mouse.x <= p1.x
        && mouse.y >= p0.y && mouse.y <= p1.y)
}

def draw_focus_target_overlay(target : WatcherFocusTarget const;
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

def draw_file_inspector_content() {
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
            status.text)
    }
}

def inspector_ui_init() {
    DIFF_SYNTAX.value = 0
    DIFF_CONTEXT.value = 0
    INSPECTOR_SEARCH.capacity = 256
}

def draw_file_inspector_dock() {
    dock_window(FILE_INSPECTOR_WIN.PUBLIC, (text = "File Inspector", closable = true,
            flags = ImGuiWindowFlags.None)) {
        draw_file_inspector_window()
    }
}
