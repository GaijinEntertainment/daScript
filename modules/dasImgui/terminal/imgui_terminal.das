options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_terminal shared

require imgui public
require terminal/terminal public
require clipboard/clipboard
require imgui/imgui_boost_runtime
require imgui/imgui_text_flow
require math
require strings

//! First interactive terminal view. Terminal cells remain the layout authority;
//! glyphs are clipped to the one- or two-cell rectangle assigned by the emulator.

struct public ImGuiTerminalStyle {
    background : float4 = float4(0.035f, 0.043f, 0.055f, 1.0f)
    foreground : float4 = float4(0.83f, 0.85f, 0.88f, 1.0f)
    cursor : float4 = float4(0.82f, 0.84f, 0.90f, 0.72f)
    focus_border : float4 = float4(0.30f, 0.56f, 0.92f, 1.0f)
    selection : float4 = float4(0.20f, 0.42f, 0.72f, 0.72f)
    visual_header_color : float4 = float4(0.48f, 0.52f, 0.58f, 1.0f)
    visual_header : string
    cell_width : float = 0.0f
    line_height : float = 0.0f
    view_height : float = 0.0f
    cell_backgrounds : bool = true
    external_input : bool  // Return encoded input to the caller instead of writing this terminal's PTY.
}

struct public ImGuiTerminalState {
    focused : bool
    columns : int
    rows : int
    render_revision : int
    resize_revision : int
    input_revision : int
    external_input : bool  // Last rendered input-routing mode; exposed for live inspection.
    content_revision : int
    screen_text : string
    visual_header : string
    scroll_offset : int
    scrollback_rows : int
    cursor_row : int
    cursor_column : int
    cursor_visible : bool
    cursor_blinking : bool
    cursor_phase_visible : bool
    selection_active : bool
    selection_dragging : bool
    scrollbar_dragging : bool
    selection_anchor_row : int
    selection_anchor_column : int
    selection_focus_row : int
    selection_focus_column : int
    selection_revision : int
    paint_cells : int
    paint_special_cells : int
    paint_background_cells : int
    paint_background_runs : int
    paint_glyph_cells : int
    paint_text_runs : int
    paint_batched_cells : int
    perf_cache_refreshed : bool
    perf_snapshot_us : int
    perf_run_build_us : int
    perf_chrome_us : int
    perf_text_us : int
    perf_terminal_us : int
}

//! Cell projection cache kept separate from ImGuiTerminalState so live
//! inspection remains concise. Rebuilds only when terminal content, geometry,
//! active buffer, or scroll position changes.
struct public ImGuiTerminalCache {
    terminal_revision : int64 = -1l
    scroll_offset : int = -1
    snapshot : TerminalViewportSnapshot = TerminalViewportSnapshot()
    refreshed : bool
    snapshot_us : int
    run_build_us : int
}

struct public ImGuiTerminalResult {
    focused : bool
    resized : bool
    input_sent : bool
    paste_sent : bool
    selection_copied : bool
    selection_text : string
    columns : int
    rows : int
    input_text : string  // Encoded keys/paste for an external transport when external_input is set.
}

def private indexed_color(index : int) : float4 {
    if (index == 0) return float4(0.00f, 0.00f, 0.00f, 1.0f)
    if (index == 1) return float4(0.80f, 0.16f, 0.16f, 1.0f)
    if (index == 2) return float4(0.20f, 0.72f, 0.30f, 1.0f)
    if (index == 3) return float4(0.80f, 0.66f, 0.20f, 1.0f)
    if (index == 4) return float4(0.25f, 0.43f, 0.82f, 1.0f)
    if (index == 5) return float4(0.72f, 0.30f, 0.76f, 1.0f)
    if (index == 6) return float4(0.20f, 0.70f, 0.72f, 1.0f)
    if (index == 7) return float4(0.72f, 0.74f, 0.76f, 1.0f)
    if (index < 16) {
        let bright = indexed_color(index - 8)
        return float4(min(1.0f, bright.x + 0.22f), min(1.0f, bright.y + 0.22f),
            min(1.0f, bright.z + 0.22f), 1.0f)
    }
    if (index < 232) {
        let cube = index - 16
        let r = cube / 36
        let g = (cube / 6) % 6
        let b = cube % 6
        return float4(float(r == 0 ? 0 : 55 + r * 40) / 255.0f,
            float(g == 0 ? 0 : 55 + g * 40) / 255.0f,
            float(b == 0 ? 0 : 55 + b * 40) / 255.0f, 1.0f)
    }
    let gray = float(8 + (index - 232) * 10) / 255.0f
    return float4(gray, gray, gray, 1.0f)
}

def private resolve_color(color : TerminalColor const; fallback : float4) : float4 {
    if (color.kind == TerminalColorKind.indexed) return indexed_color(color.index)
    if (color.kind == TerminalColorKind.rgb) {
        return float4(float(color.red) / 255.0f, float(color.green) / 255.0f,
            float(color.blue) / 255.0f, 1.0f)
    }
    return fallback
}

def private faint_color(foreground, background : float4) : float4 {
    let strength = 0.44f
    return float4(
        background.x + (foreground.x - background.x) * strength,
        background.y + (foreground.y - background.y) * strength,
        background.z + (foreground.z - background.z) * strength,
        foreground.w)
}

def private effective_background(cell : TerminalCell const;
                                 style : ImGuiTerminalStyle const) : float4 {
    if ((cell.attributes & TERMINAL_ATTR_INVERSE) != 0) {
        return resolve_color(cell.foreground, style.foreground)
    }
    return resolve_color(cell.background, style.background)
}

def private draw_terminal_background_runs(var dl : ImDrawList?;
                                          cells : array<TerminalCell> const;
                                          row_base, visible_start_row, columns : int;
                                          origin : float2; cell_width, line_height : float;
                                          style : ImGuiTerminalStyle const;
                                          var state : ImGuiTerminalState) {
    if (!style.cell_backgrounds || columns <= 0 || empty(cells)) return
    let row_count = length(cells) / columns
    for (row in range(row_count)) {
        let display_row = row_base + row - visible_start_row
        if (display_row < 0 || display_row >= state.rows) continue
        var run_first = -1
        var run_color = style.background
        var wide_until = -1
        var wide_color = style.background
        for (column in range(columns + 1)) {
            var color = style.background
            if (column < columns) {
                let cell = cells[row * columns + column]
                if (cell.width > 1) {
                    color = effective_background(cell, style)
                    wide_until = min(columns, column + cell.width)
                    wide_color = color
                } elif (cell.width == 0 && column < wide_until) {
                    color = wide_color
                } else {
                    color = effective_background(cell, style)
                    wide_until = -1
                }
            }
            if (color != style.background) {
                state.paint_background_cells++
                if (run_first < 0) {
                    run_first = column
                    run_color = color
                } elif (color != run_color) {
                    let p0 = origin + float2(float(run_first) * cell_width,
                        float(display_row) * line_height)
                    let p1 = origin + float2(float(column) * cell_width,
                        float(display_row + 1) * line_height)
                    *dl |> AddRectFilled(p0, p1, GetColorU32(run_color),
                        0.0f, ImDrawFlags.None)
                    state.paint_background_runs++
                    run_first = column
                    run_color = color
                }
            } elif (run_first >= 0) {
                let p0 = origin + float2(float(run_first) * cell_width,
                    float(display_row) * line_height)
                let p1 = origin + float2(float(column) * cell_width,
                    float(display_row + 1) * line_height)
                *dl |> AddRectFilled(p0, p1, GetColorU32(run_color),
                    0.0f, ImDrawFlags.None)
                state.paint_background_runs++
                run_first = -1
            }
        }
    }
}

def private draw_terminal_cell_chrome(var dl : ImDrawList?; cell : TerminalCell const;
                                      display_row, rows : int;
                                      origin : float2; cell_width, line_height : float;
                                      style : ImGuiTerminalStyle const) {
    if (cell.width == 0 || display_row < 0 || display_row >= rows) return
    let inverse = (cell.attributes & 32) != 0
    let underline = (cell.attributes & 8) != 0
    let strike = (cell.attributes & 128) != 0
    if (!underline && !strike) return
    let p0 = origin + float2(float(cell.column) * cell_width,
        float(display_row) * line_height)
    let p1 = p0 + float2(cell_width * float(max(1, cell.width)), line_height)
    var fg = resolve_color(cell.foreground, style.foreground)
    var bg = resolve_color(cell.background, style.background)
    if (inverse && style.cell_backgrounds) {
        let swap = fg
        fg = bg
        bg = swap
    }
    if (underline) {
        *dl |> AddLine(float2(p0.x, p1.y - 1.0f), float2(p1.x, p1.y - 1.0f),
            GetColorU32(fg), 1.0f)
    }
    if (strike) {
        let y = p0.y + line_height * 0.55f
        *dl |> AddLine(float2(p0.x, y), float2(p1.x, y), GetColorU32(fg), 1.0f)
    }
}

def private draw_terminal_selection(var dl : ImDrawList?;
                                    state : ImGuiTerminalState const;
                                    columns, rows, visible_start_row : int;
                                    origin : float2; cell_width, line_height : float;
                                    style : ImGuiTerminalStyle const) {
    if (!state.selection_active) return
    let anchor = state.selection_anchor_row * columns + state.selection_anchor_column
    let focus = state.selection_focus_row * columns + state.selection_focus_column
    let first = min(anchor, focus)
    let last = max(anchor, focus) + 1
    for (display_row in range(0, rows)) {
        let row_first = (visible_start_row + display_row) * columns
        let overlap_first = max(first, row_first)
        let overlap_last = min(last, row_first + columns)
        if (overlap_last <= overlap_first) continue
        let first_column = overlap_first - row_first
        let last_column = overlap_last - row_first
        let p0 = origin + float2(float(first_column) * cell_width,
            float(display_row) * line_height)
        let p1 = origin + float2(float(last_column) * cell_width,
            float(display_row + 1) * line_height)
        *dl |> AddRectFilled(p0, p1, GetColorU32(style.selection), 0.0f, ImDrawFlags.None)
    }
}

def private batchable_ascii(cell : TerminalCell const) : bool {
    // Styled/linked ASCII stays on the sparse-cell path so its attributes and OSC 8 metadata survive.
    if (cell.width != 1 || cell.attributes != 0 || cell.hyperlink != ""
            || length(cell.grapheme) != 1) {
        return false
    }
    let value = cell.grapheme |> character_at(0) // nolint:PERF003 one byte per cached cell
    return value >= 32 && value <= 126
}

def private draw_terminal_text_runs(var dl : ImDrawList?;
                                    runs : array<TerminalTextRun> const;
                                    visible_start_row : int; origin : float2;
                                    cell_width, line_height : float;
                                    style : ImGuiTerminalStyle const;
                                    var state : ImGuiTerminalState) {
    for (run in runs) {
        let display_row = run.global_row - visible_start_row
        if (display_row < 0 || display_row >= state.rows) continue
        let fallback = run.default_is_background ? style.background : style.foreground
        let color = resolve_color(run.color, fallback)
        let p0 = origin + float2(float(run.column) * cell_width,
            float(display_row) * line_height)
        *dl |> AddText(p0, GetColorU32(color), run.text)
        state.paint_text_runs++
        state.paint_glyph_cells += run.glyph_cells
        state.paint_batched_cells += run.glyph_cells
    }
}

def private draw_terminal_fallback_cells(var dl : ImDrawList?;
                                         cells : array<TerminalCell> const; row_base : int;
                                         visible_start_row : int; origin : float2;
                                         cell_width, line_height : float;
                                         style : ImGuiTerminalStyle const;
                                         var state : ImGuiTerminalState) {
    for (cell in cells) {
        if (batchable_ascii(cell) || cell.width == 0 || cell.grapheme == ""
                || cell.grapheme == " "
                || (cell.attributes & TERMINAL_ATTR_HIDDEN) != 0) continue
        let global_row = row_base + cell.row
        let display_row = global_row - visible_start_row
        if (display_row < 0 || display_row >= state.rows) continue
        let inverse = (cell.attributes & 32) != 0
        var color = (inverse && style.cell_backgrounds
            ? resolve_color(cell.background, style.background)
            : resolve_color(cell.foreground, style.foreground))
        if ((cell.attributes & TERMINAL_ATTR_FAINT) != 0) {
            let background = (style.cell_backgrounds
                ? effective_background(cell, style) : style.background)
            color = faint_color(color, background)
        }
        let p0 = origin + float2(float(cell.column) * cell_width,
            float(display_row) * line_height)
        *dl |> AddText(p0, GetColorU32(color), cell.grapheme)
        state.paint_text_runs++
        state.paint_glyph_cells++
    }
}

def private selected_cell(cells : array<TerminalCell> const;
                          row, column, columns : int) : tuple<string; int> {
    // Defensive coordinate check: a malformed external snapshot yields a blank cell, not a wrong grapheme.
    let index = row * columns + column
    if (index < 0 || index >= length(cells)) return (" ", 1)
    let cell = cells[index]
    if (cell.row != row || cell.column != column || cell.width <= 0) return (" ", 1)
    return (cell.grapheme, max(1, cell.width))
}

def private selected_text(buffer : TerminalBufferSnapshot const;
                          state : ImGuiTerminalState const; columns : int) : string {
    if (!state.selection_active) return ""
    let anchor = state.selection_anchor_row * columns + state.selection_anchor_column
    let focus = state.selection_focus_row * columns + state.selection_focus_column
    let first = min(anchor, focus)
    let last = max(anchor, focus)
    let first_row = first / columns
    let last_row = last / columns
    return build_string() $(var result) {
        for (row in range(first_row, last_row + 1)) {
            let first_column = row == first_row ? first % columns : 0
            let last_column = row == last_row ? last % columns : columns - 1
            var continuation_until = -1
            let line = build_string() $(var line_writer) {
                for (column in range(first_column, last_column + 1)) {
                    if (column < continuation_until) continue
                    if (row < buffer.scrollback_rows) {
                        let (grapheme, width) = selected_cell(
                            buffer.scrollback, row, column, columns)
                        line_writer |> write(grapheme)
                        continuation_until = column + width
                    } else {
                        let (grapheme, width) = selected_cell(
                            buffer.cells, row - buffer.scrollback_rows, column, columns)
                        line_writer |> write(grapheme)
                        continuation_until = column + width
                    }
                }
            }
            result |> write(line |> rtrim(" "))
            if (row != last_row) {
                result |> write("\n")
            }
        }
    }
}

def public imgui_terminal_clear_selection(var state : ImGuiTerminalState) {
    if (!state.selection_active && !state.selection_dragging) return
    state.selection_active = false
    state.selection_dragging = false
    state.selection_revision++
}

def public imgui_terminal_select_all(terminal : Terminal const;
                                     var state : ImGuiTerminalState) {
    var snapshot = TerminalSnapshot()
    terminal_snapshot(terminal, snapshot)
    let buffer <- snapshot.alternate_active ? snapshot.alternate : snapshot.normal
    let last_row = max(0, buffer.scrollback_rows + snapshot.rows - 1)
    state.selection_anchor_row = 0
    state.selection_anchor_column = 0
    state.selection_focus_row = last_row
    state.selection_focus_column = max(0, snapshot.columns - 1)
    state.selection_active = true
    state.selection_dragging = false
    state.selection_revision++
}

def public imgui_terminal_copy_selection(terminal : Terminal const;
                                         state : ImGuiTerminalState const) : bool {
    if (!state.selection_active) return false
    var snapshot = TerminalSnapshot()
    terminal_snapshot(terminal, snapshot)
    let buffer <- snapshot.alternate_active ? snapshot.alternate : snapshot.normal
    return clipboard_set_text(selected_text(buffer, state, snapshot.columns)) == ClipboardStatus.ok
}

def public imgui_terminal_paste_clipboard(terminal : Terminal const) : bool {
    var text = ""
    if (clipboard_get_text(text) != ClipboardStatus.ok) return false
    return terminal_send_paste(terminal, text)
}

def private pressed(key : ImGuiKey) : bool {
    return IsKeyPressed(key, false)
}

def private dispatch_input(terminal : Terminal const; text : string;
                           external_input : bool; var output : string&) : bool {
    if (empty(text)) return false
    if (external_input) {
        output += text
        return true
    }
    return terminal_write(terminal, text)
}

def private send_special(terminal : Terminal const; key : ImGuiKey;
                         terminal_key : TerminalKey; io : ImGuiIO const;
                         external_input : bool; var output : string&) : bool {
    // nolint:STYLE015 — Printable keys repeat through InputQueueCharacters. Special terminal keys
    // need ImGui's repeat path explicitly or a held Backspace/Delete/arrow only
    // emits its initial press.
    if (!IsKeyPressed(key, true)) return false
    let encoded = terminal_encode_key(terminal, TerminalKeyEvent(
        key = terminal_key, shift = io.KeyShift, alt = io.KeyAlt,
        control = io.KeyCtrl))
    return dispatch_input(terminal, encoded, external_input, output)
}

def private route_input(terminal : Terminal const;
                        var state : ImGuiTerminalState;
                        external_input : bool) : tuple<bool; bool; string> {
    if (!state.focused) return (false, false, "")
    let io & = unsafe(GetIO())
    var sent = false
    var pasted = false
    var output : string
    sent = send_special(terminal, ImGuiKey.Enter, TerminalKey.enter, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.KeypadEnter, TerminalKey.enter, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.Tab, TerminalKey.tab, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.Backspace, TerminalKey.backspace, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.Escape, TerminalKey.escape, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.UpArrow, TerminalKey.up, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.DownArrow, TerminalKey.down, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.RightArrow, TerminalKey.right, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.LeftArrow, TerminalKey.left, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.Home, TerminalKey.home, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.End, TerminalKey.end, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.Insert, TerminalKey.insert, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.Delete, TerminalKey.delete_key, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.PageUp, TerminalKey.page_up, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.PageDown, TerminalKey.page_down, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.F1, TerminalKey.f1, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.F2, TerminalKey.f2, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.F3, TerminalKey.f3, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.F4, TerminalKey.f4, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.F5, TerminalKey.f5, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.F6, TerminalKey.f6, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.F7, TerminalKey.f7, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.F8, TerminalKey.f8, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.F9, TerminalKey.f9, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.F10, TerminalKey.f10, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.F11, TerminalKey.f11, io, external_input, output) || sent
    sent = send_special(terminal, ImGuiKey.F12, TerminalKey.f12, io, external_input, output) || sent

    if (io.KeyCtrl) {
        if (pressed(ImGuiKey.V)) {
            var text = ""
            if (clipboard_get_text(text) == ClipboardStatus.ok) {
                let encoded = terminal_encode_paste(terminal, text)
                pasted = dispatch_input(terminal, encoded, external_input, output)
                sent = pasted || sent
            }
        } else {
            for (letter in range(0, 26)) {
                if (letter == 2 && state.selection_active) continue
                let imgui_key = unsafe(reinterpret<ImGuiKey>(int(ImGuiKey.A) + letter))
                if (pressed(imgui_key)) {
                    let encoded = terminal_encode_key(terminal, TerminalKeyEvent(
                        key = TerminalKey.text, text = to_char(65 + letter),
                        shift = io.KeyShift, alt = io.KeyAlt, control = true))
                    sent = dispatch_input(terminal, encoded, external_input, output) || sent
                }
            }
        }
    } else {
        for (value in io.InputQueueCharacters) {
            let codepoint = int(value)
            if (codepoint > 0 && codepoint <= 0x10ffff) {
                let encoded = terminal_encode_key(terminal, TerminalKeyEvent(
                    key = TerminalKey.text, text = codepoint_utf8(codepoint),
                    shift = io.KeyShift, alt = io.KeyAlt,
                    control = io.KeyCtrl))
                sent = dispatch_input(terminal, encoded, external_input, output) || sent
            }
        }
    }
    if (sent) state.input_revision++
    return (sent, pasted, output)
}

def private refresh_terminal_cache(terminal : Terminal const; scroll_offset : int;
                                   var cache : ImGuiTerminalCache) : bool {
    let revision = terminal_revision(terminal)
    if (cache.terminal_revision == revision && cache.scroll_offset == scroll_offset) {
        return false
    }
    terminal_viewport_snapshot(terminal, scroll_offset, cache.snapshot)
    cache.snapshot_us += cache.snapshot.cell_projection_us
    cache.run_build_us += cache.snapshot.text_run_projection_us
    cache.refreshed = true
    cache.terminal_revision = cache.snapshot.revision
    cache.scroll_offset = clamp(scroll_offset, 0, cache.snapshot.buffer.scrollback_rows)
    return true
}

def public imgui_terminal(id : string; var terminal : Terminal; // nolint:STYLE037
                          var state : ImGuiTerminalState;
                          var cache : ImGuiTerminalCache;
                          style : ImGuiTerminalStyle = ImGuiTerminalStyle()) : ImGuiTerminalResult {
    let terminal_started = ref_time_ticks()
    cache.refreshed = false
    cache.snapshot_us = 0
    cache.run_build_us = 0
    var result = ImGuiTerminalResult()
    var snapshot_changed = refresh_terminal_cache(terminal, state.scroll_offset, cache)
    var snapshot & = unsafe(cache.snapshot)
    var cell_width = style.cell_width
    if (cell_width <= 0.0f) {
        cell_width = max(1.0f, CalcTextSize("M", false, -1.0f).x)
    }
    var line_height = style.line_height
    if (line_height <= 0.0f) {
        line_height = max(1.0f, GetTextLineHeight())
    }
    var available = GetContentRegionAvail()
    if (style.view_height > 0.0f) {
        available.y = min(available.y, style.view_height)
    }
    let columns = max(1, int(floor(available.x / cell_width)))
    let visual_header_rows = empty(style.visual_header) ? 0 : 1
    let rows = max(1,
        int(floor(available.y / line_height)) - visual_header_rows)
    if (columns != snapshot.columns || rows != snapshot.rows) {
        terminal_resize(terminal, columns, rows)
        if (refresh_terminal_cache(terminal, state.scroll_offset, cache)) {
            snapshot_changed = true
        }
        state.resize_revision++
        result.resized = true
    }
    state.columns = snapshot.columns
    state.rows = snapshot.rows
    result.columns = snapshot.columns
    result.rows = snapshot.rows

    let origin = GetCursorScreenPos()
    let content_origin = origin + float2(0.0f,
        line_height * float(visual_header_rows))
    let extent = float2(cell_width * float(snapshot.columns),
        line_height * float(snapshot.rows + visual_header_rows))
    PushID(id)
    InvisibleButton("##terminal", extent)
    let hovered = IsItemHovered(ImGuiHoveredFlags.None)
    unsafe {
        register_clickable_state_bbox(id, "##terminal", "terminal", addr(state),
            reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<ImGuiTerminalState>)))
    }
    if (IsItemFocused()) {
        state.focused = true
    }
    if (hovered && IsMouseClicked(ImGuiMouseButton.Left, false)) {
        state.focused = true
    }
    if (!hovered && IsMouseClicked(ImGuiMouseButton.Left, false)) {
        state.focused = false
    }
    PopID()

    var dl = GetWindowDrawList()
    *dl |> AddRectFilled(origin, origin + extent, GetColorU32(style.background),
        0.0f, ImDrawFlags.None)
    *dl |> PushClipRect(origin, origin + extent, true)
    state.visual_header = style.visual_header
    if (visual_header_rows > 0) {
        *dl |> AddText(origin, GetColorU32(style.visual_header_color),
            style.visual_header)
    }
    var buffer & = unsafe(snapshot.buffer)
    state.scrollback_rows = buffer.scrollback_rows
    if (snapshot_changed) {
        let current_screen_text = snapshot.screen_text
        if (current_screen_text != state.screen_text) {
            state.screen_text = current_screen_text
            state.content_revision++
        }
    }
    let io & = unsafe(GetIO())
    let local_mouse = io.MousePos - content_origin
    // nolint:STYLE014 — When the child asked for mouse tracking (alt-screen TUIs: Claude Code,
    // vim), plain wheel and clicks belong to the CHILD - it scrolls its own
    // transcript, exactly like a real terminal (note 42). Shift reserves the
    // local gestures (scrollback scroll, text selection); Ctrl+wheel stays
    // application zoom.
    let mouse_to_child = terminal_mouse_reporting_active(terminal) && !io.KeyShift
    var mouse_report : string
    if (hovered && !io.KeyCtrl && io.MouseWheel != 0.0f) {
        if (mouse_to_child && state.scroll_offset == 0) {
            let wheel_row = clamp(int(floor(local_mouse.y / line_height)),
                0, snapshot.rows - 1)
            let wheel_column = clamp(int(floor(local_mouse.x / cell_width)),
                0, snapshot.columns - 1)
            let notches = clamp(int(round(abs(io.MouseWheel))), 1, 8)
            for (_notch in range(notches)) {
                mouse_report += terminal_encode_mouse(terminal, // nolint:PERF001 — at most 8 wheel notches
                    io.MouseWheel > 0.0f ? 64 : 65, wheel_column, wheel_row, true)
            }
        } else {
            let old_scroll_offset = state.scroll_offset
            state.scroll_offset = clamp(state.scroll_offset + int(io.MouseWheel * 3.0f),
                0, buffer.scrollback_rows)
            if (state.scroll_offset != old_scroll_offset) {
                let _ = refresh_terminal_cache(terminal, state.scroll_offset, cache)
            }
        }
    } else {
        state.scroll_offset = clamp(state.scroll_offset, 0, buffer.scrollback_rows)
    }
    // nolint:STYLE014 — Scrollback scrollbar: a thin right-edge track, visible only when
    // history exists. Claims clicks in its band so text selection never
    // starts from a scrollbar drag.
    // Inset from the widget edge: the dock splitter's grab zone claims hover
    // over the outermost pixels, which would eat scrollbar clicks.
    let scrollbar_inner = extent.x - 20.0f
    let scrollbar_outer = extent.x - 6.0f
    let content_height = line_height * float(snapshot.rows)
    let over_scrollbar = (buffer.scrollback_rows > 0
        && local_mouse.x >= scrollbar_inner && local_mouse.x <= scrollbar_outer
        && local_mouse.y >= 0.0f && local_mouse.y < content_height)
    if (hovered && over_scrollbar && IsMouseClicked(ImGuiMouseButton.Left, false)) {
        state.scrollbar_dragging = true
    }
    if (state.scrollbar_dragging) {
        if (IsMouseDown(ImGuiMouseButton.Left)) {
            let total_rows = buffer.scrollback_rows + snapshot.rows
            let fraction = clamp(local_mouse.y / max(1.0f, content_height), 0.0f, 1.0f)
            let first_visible = int(fraction * float(total_rows - snapshot.rows) + 0.5f)
            let dragged_offset = clamp(buffer.scrollback_rows - first_visible,
                0, buffer.scrollback_rows)
            if (dragged_offset != state.scroll_offset) {
                state.scroll_offset = dragged_offset
                let _ = refresh_terminal_cache(terminal, state.scroll_offset, cache)
            }
        } else {
            state.scrollbar_dragging = false
        }
    }
    let visible_start_row = buffer.scrollback_rows - state.scroll_offset
    let over_content = (local_mouse.y >= 0.0f
        && local_mouse.y < content_height
        && !over_scrollbar && !state.scrollbar_dragging)
    if (mouse_to_child && hovered) {
        // Terminal buttons: 0 left, 1 middle, 2 right. Presses need the
        // cursor over content; releases forward from anywhere hovered so the
        // child never sees a stuck button.
        let report_row = clamp(int(floor(local_mouse.y / line_height)),
            0, snapshot.rows - 1)
        let report_column = clamp(int(floor(local_mouse.x / cell_width)),
            0, snapshot.columns - 1)
        if (over_content && IsMouseClicked(ImGuiMouseButton.Left, false)) {
            mouse_report += terminal_encode_mouse(terminal, 0, report_column, report_row, true)
        }
        if (IsMouseReleased(ImGuiMouseButton.Left)) {
            mouse_report += terminal_encode_mouse(terminal, 0, report_column, report_row, false)
        }
        if (over_content && IsMouseClicked(ImGuiMouseButton.Middle, false)) {
            mouse_report += terminal_encode_mouse(terminal, 1, report_column, report_row, true)
        }
        if (IsMouseReleased(ImGuiMouseButton.Middle)) {
            mouse_report += terminal_encode_mouse(terminal, 1, report_column, report_row, false)
        }
        if (over_content && IsMouseClicked(ImGuiMouseButton.Right, false)) {
            mouse_report += terminal_encode_mouse(terminal, 2, report_column, report_row, true)
        }
        if (IsMouseReleased(ImGuiMouseButton.Right)) {
            mouse_report += terminal_encode_mouse(terminal, 2, report_column, report_row, false)
        }
    }
    if (!mouse_to_child && hovered && over_content && IsMouseClicked(ImGuiMouseButton.Left, false)) {
        let selected_row = visible_start_row + clamp(
            int(floor(local_mouse.y / line_height)), 0, snapshot.rows - 1)
        let selected_column = clamp(
            int(floor(local_mouse.x / cell_width)), 0, snapshot.columns - 1)
        state.selection_active = false
        state.selection_dragging = true
        state.selection_anchor_row = selected_row
        state.selection_anchor_column = selected_column
        state.selection_focus_row = selected_row
        state.selection_focus_column = selected_column
        state.selection_revision++
    } elif (hovered && over_content && state.selection_dragging
            && IsMouseDown(ImGuiMouseButton.Left)) {
        let selected_row = visible_start_row + clamp(
            int(floor(local_mouse.y / line_height)), 0, snapshot.rows - 1)
        let selected_column = clamp(
            int(floor(local_mouse.x / cell_width)), 0, snapshot.columns - 1)
        if (selected_row != state.selection_focus_row
                || selected_column != state.selection_focus_column) {
            state.selection_active = true
            state.selection_focus_row = selected_row
            state.selection_focus_column = selected_column
            state.selection_revision++
        }
    }
    if (state.selection_dragging && IsMouseReleased(ImGuiMouseButton.Left)) {
        state.selection_dragging = false
    }
    state.paint_cells = snapshot.columns * snapshot.rows
    state.paint_special_cells = 0
    state.paint_background_cells = 0
    state.paint_background_runs = 0
    state.paint_glyph_cells = 0
    state.paint_text_runs = 0
    state.paint_batched_cells = 0
    let chrome_started = ref_time_ticks()
    draw_terminal_background_runs(dl, buffer.scrollback, 0, visible_start_row,
        snapshot.columns, content_origin, cell_width, line_height, style, state)
    draw_terminal_background_runs(dl, buffer.cells, buffer.scrollback_rows,
        visible_start_row, snapshot.columns, content_origin, cell_width, line_height,
        style, state)
    for (cell in buffer.scrollback) {
        let display_row = cell.row - visible_start_row
        if (cell.width > 0 && display_row >= 0 && display_row < state.rows) {
            state.paint_special_cells++
        }
        draw_terminal_cell_chrome(dl, cell, display_row, state.rows,
            content_origin, cell_width, line_height, style)
    }
    for (cell in buffer.cells) {
        let global_row = buffer.scrollback_rows + cell.row
        let display_row = global_row - visible_start_row
        if (cell.width > 0 && display_row >= 0 && display_row < state.rows) {
            state.paint_special_cells++
        }
        draw_terminal_cell_chrome(dl, cell, display_row, state.rows,
            content_origin, cell_width, line_height, style)
    }
    draw_terminal_selection(dl, state, snapshot.columns, snapshot.rows,
        visible_start_row, content_origin, cell_width, line_height, style)
    state.perf_chrome_us = get_time_usec(chrome_started)
    let text_started = ref_time_ticks()
    draw_terminal_text_runs(dl, snapshot.text_runs, visible_start_row,
        content_origin, cell_width, line_height, style, state)
    draw_terminal_fallback_cells(dl, buffer.scrollback, 0, visible_start_row,
        content_origin, cell_width, line_height, style, state)
    draw_terminal_fallback_cells(dl, buffer.cells, buffer.scrollback_rows, visible_start_row,
        content_origin, cell_width, line_height, style, state)
    state.perf_text_us = get_time_usec(text_started)
    state.cursor_row = buffer.cursor.row
    state.cursor_column = buffer.cursor.column
    state.cursor_visible = buffer.cursor.visible
    state.cursor_blinking = buffer.cursor.blinking
    state.cursor_phase_visible = (buffer.cursor.visible
        && (!buffer.cursor.blinking || !state.focused
            || fmod(float(GetTime()), 1.0f) < 0.5f))
    if (state.cursor_phase_visible && state.scroll_offset == 0) {
        let cp0 = content_origin + float2(float(buffer.cursor.column) * cell_width,
            float(buffer.cursor.row) * line_height)
        *dl |> AddRectFilled(cp0, cp0 + float2(max(1.0f, cell_width * 0.12f), line_height),
            GetColorU32(style.cursor), 0.0f, ImDrawFlags.None)
    }
    if (buffer.scrollback_rows > 0) {
        // Scrollback scrollbar: thin track at the right edge, thumb sized by
        // the visible proportion, brighter while hovered or dragged.
        let total_rows = float(buffer.scrollback_rows + snapshot.rows)
        let track_x1 = content_origin.x + extent.x - 8.0f
        let track_x0 = track_x1 - 6.0f
        let track_y0 = content_origin.y
        let track_h = content_height
        let thumb_h = max(24.0f, track_h * float(snapshot.rows) / total_rows)
        let scroll_span = max(1.0f, float(buffer.scrollback_rows))
        let scroll_fraction = float(buffer.scrollback_rows - state.scroll_offset) / scroll_span
        let thumb_y = track_y0 + (track_h - thumb_h) * scroll_fraction
        let active = over_scrollbar || state.scrollbar_dragging
        *dl |> AddRectFilled(float2(track_x0, track_y0),
            float2(track_x1, track_y0 + track_h),
            GetColorU32(float4(1.0f, 1.0f, 1.0f, 0.06f)), 3.0f, ImDrawFlags.None)
        *dl |> AddRectFilled(float2(track_x0, thumb_y),
            float2(track_x1, thumb_y + thumb_h),
            GetColorU32(float4(1.0f, 1.0f, 1.0f, active ? 0.45f : 0.22f)),
            3.0f, ImDrawFlags.None)
    }
    *dl |> PopClipRect()
    if (state.focused) {
        *dl |> AddRect(origin, origin + extent, GetColorU32(style.focus_border),
            0.0f, ImDrawFlags.None, 1.0f)
    }
    if (state.focused && state.selection_active && io.KeyCtrl && pressed(ImGuiKey.C)) {
        var copy_snapshot = TerminalSnapshot()
        terminal_snapshot(terminal, copy_snapshot)
        let copy_buffer <- copy_snapshot.alternate_active ? copy_snapshot.alternate : copy_snapshot.normal
        let copy_text = selected_text(copy_buffer, state, copy_snapshot.columns)
        result.selection_text = copy_text
        result.selection_copied = clipboard_set_text(copy_text) == ClipboardStatus.ok
    }
    var (sent, pasted, input_text) = route_input(terminal, state, style.external_input)
    if (!empty(mouse_report)
            && dispatch_input(terminal, mouse_report, style.external_input, input_text)) {
        sent = true
    }
    state.external_input = style.external_input
    result.input_sent = sent
    result.paste_sent = pasted
    result.input_text = input_text
    result.focused = state.focused
    state.perf_cache_refreshed = cache.refreshed
    state.perf_snapshot_us = cache.snapshot_us
    state.perf_run_build_us = cache.run_build_us
    state.perf_terminal_us = get_time_usec(terminal_started)
    state.render_revision++
    return <- result
}

//! Compatibility overload for callers which do not retain a cache. Interactive
//! terminal hosts should pass a persistent ImGuiTerminalCache.
def public imgui_terminal(id : string; var terminal : Terminal;
                          var state : ImGuiTerminalState;
                          style : ImGuiTerminalStyle = ImGuiTerminalStyle()) : ImGuiTerminalResult {
    var cache = ImGuiTerminalCache()
    return <- imgui_terminal(id, terminal, state, cache, style)
}
