options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_function_arguments = false

module imgui_text_source_view shared

require imgui public
require imgui/imgui_text_source_model public
require imgui/imgui_fonts public
require clipboard/clipboard
require math
require strings

//! Selectable read-only source presentation shared by text viewers and the
//! future editor. Layout is cell-based but source positions remain UTF-8 byte
//! offsets. Color emoji/CJK reserve two cells; combining and ZWJ sequences are
//! atomic graphemes. Glyph drawing is centered and clipped to its cell box.

bitfield public TextSourceSelectionStyles {
    all_regular
    has_markers
    has_mixed_styles
}

struct public TextSourceSelection {
    active : bool = false
    source : TextSourceRange = TextSourceRange(start_byte = -1, end_byte = -1)
    styles : TextSourceSelectionStyles
    screen_valid : bool = false
    screen_bounds : float4
    screen_anchor : float2
}

struct public TextSourceViewResult {
    selection : TextSourceSelection = TextSourceSelection()
    selection_changed : bool = false
    selection_copied : bool = false
    selection_copy_requested : bool = false
    selection_copy_text : string
    selection_context_requested : bool = false
    selection_context_pos : float2
}

struct public TextSourceViewStyle {
    fonts : FontRoleSet = FontRoleSet()
    zoom : float = 1.0f
    text_color : float4 = float4(0.86f, 0.87f, 0.89f, 1.0f)
    marker_color : float4 = float4(0.97f, 0.76f, 0.34f, 1.0f)
    syntax_comment_color : float4 = float4(0.48f, 0.62f, 0.50f, 1.0f)
    syntax_string_color : float4 = float4(0.78f, 0.67f, 0.45f, 1.0f)
    syntax_number_color : float4 = float4(0.70f, 0.58f, 0.86f, 1.0f)
    syntax_keyword_color : float4 = float4(0.78f, 0.48f, 0.68f, 1.0f)
    syntax_type_color : float4 = float4(0.42f, 0.72f, 0.74f, 1.0f)
    syntax_function_color : float4 = float4(0.48f, 0.68f, 0.92f, 1.0f)
    syntax_variable_color : float4 = float4(0.86f, 0.87f, 0.89f, 1.0f)
    syntax_property_color : float4 = float4(0.60f, 0.76f, 0.90f, 1.0f)
    syntax_constant_color : float4 = float4(0.72f, 0.61f, 0.88f, 1.0f)
    syntax_operator_color : float4 = float4(0.76f, 0.78f, 0.82f, 1.0f)
    syntax_punctuation_color : float4 = float4(0.62f, 0.65f, 0.70f, 1.0f)
    selection_color : float4 = float4(0.24f, 0.48f, 0.78f, 0.55f)
    caret_color : float4 = float4(0.92f, 0.92f, 0.94f, 1.0f)
    added_line_color : float4 = float4(0.20f, 0.72f, 0.38f, 0.92f)
    removed_line_color : float4 = float4(0.88f, 0.28f, 0.32f, 0.92f)
    modified_line_color : float4 = float4(0.92f, 0.70f, 0.22f, 0.92f)
    line_number_color : float4 = float4(0.48f, 0.50f, 0.55f, 1.0f)
    line_mark_background_alpha : float = 0.0f
    line_mark_width : float = 2.0f
    wrap : bool = false
    tab_cells : int = 4
    write_system_clipboard : bool = true
}

def private line_mark_color(kind : TextSourceLineMarkKind;
                            style : TextSourceViewStyle const) : float4 {
    if (kind == TextSourceLineMarkKind.added) return style.added_line_color
    if (kind == TextSourceLineMarkKind.removed) return style.removed_line_color
    return style.modified_line_color
}

struct public TextSourceCell {
    source : TextSourceRange
    style_id : int = 0
    line_index : int = -1
    rect : float4
    columns : int = 1
    natural_width : float = 0.0f
}

struct public TextSourceLayoutKey {
    document_revision : int = -1
    max_width : float = 0.0f
    font_size : float = 0.0f
    line_height : float = 0.0f
    cell_width : float = 0.0f
    font_id : uint64 = 0uL
    wrap : bool = false
    tab_cells : int = 4
}

struct public TextSourceViewState {
    key : TextSourceLayoutKey = TextSourceLayoutKey()
    layout_valid : bool = false
    layout : TextFlowLayout
    cells : array<TextSourceCell>
    line_first_cell : array<int>
    line_cell_count : array<int>
    cells_first_line : int = -1
    cells_last_line : int = -1
    virtual_document_revision : int = -1
    virtual_line_starts : array<int>
    virtual_max_line_bytes : int
    virtual_visible_syntax_runs : int
    virtual_mode : bool
    screen_origin : float2
    screen_size : float2

    anchor_byte : int = -1
    focus_byte : int = -1
    selection_dragging : bool = false
    selection_changed : bool = false
    selection_screen_valid : bool = false
    selection_screen_bounds : float4
    selection_screen_anchor : float2
    caret_screen_valid : bool = false
    caret_screen_pos : float2
    caret_screen_height : float = 0.0f

    render_revision : int = 0
    selection_revision : int = 0
    selection_commit_revision : int = 0
    selection_copy_revision : int = 0
    selection_copy_pending : bool = false
    selection_copy_text : string
    selection_context_revision : int = 0
    selection_context_pos : float2
    navigation_revision : int = 0
    layout_hits : int = 0
    layout_misses : int = 0
}

let private SOURCE_STYLE_REGULAR = 0
let private SOURCE_STYLE_MARKER = 1
let private SOURCE_STYLE_SYNTAX_OFFSET = 1

def private source_zoom(style : TextSourceViewStyle const) : float {
    return clamp(style.zoom, 0.5f, 3.0f)
}

def private source_font(var style : TextSourceViewStyle; var fallback : ImFont?) : ImFont? {
    return style.fonts.monospace != null ? style.fonts.monospace : fallback
}

def private same_layout_key(a, b : TextSourceLayoutKey const) : bool {
    return (a.document_revision == b.document_revision
        && a.max_width == b.max_width && a.font_size == b.font_size
        && a.line_height == b.line_height && a.cell_width == b.cell_width
        && a.font_id == b.font_id && a.wrap == b.wrap
        && a.tab_cells == b.tab_cells)
}

def private selected_source(state : TextSourceViewState const) : TextSourceRange {
    if (state.anchor_byte < 0 || state.focus_byte < 0) {
        return TextSourceRange(start_byte = -1, end_byte = -1)
    }
    return TextSourceRange(start_byte = min(state.anchor_byte, state.focus_byte),
                           end_byte = max(state.anchor_byte, state.focus_byte))
}

def public text_source_view_selection(state : TextSourceViewState const) : TextSourceRange {
    return selected_source(state)
}

def public text_source_view_clear_selection(var state : TextSourceViewState) {
    let had_selection = state.anchor_byte >= 0 || state.focus_byte >= 0
    state.anchor_byte = -1
    state.focus_byte = -1
    state.selection_dragging = false
    if (had_selection) {
        state.selection_changed = true
        state.selection_revision++
    }
}

def public text_source_view_request_copy(var state : TextSourceViewState) {
    //! Request semantic copy; the host decides which input invokes it.
    state.selection_copy_pending = true
}

def public text_source_view_select_all(document : TextSourceDocument const;
                                       var state : TextSourceViewState) {
    state.anchor_byte = 0
    state.focus_byte = length(document.source)
    state.selection_changed = true
    state.selection_revision++
    state.selection_commit_revision++
}

def public text_source_view_invalidate(var state : TextSourceViewState) {
    delete state.layout.lines
    delete state.layout.fragments
    delete state.cells
    delete state.line_first_cell
    delete state.line_cell_count
    delete state.virtual_line_starts
    state.cells_first_line = -1
    state.cells_last_line = -1
    state.virtual_document_revision = -1
    state.virtual_max_line_bytes = 0
    state.virtual_visible_syntax_runs = 0
    state.virtual_mode = false
    state.layout_valid = false
    state.key = TextSourceLayoutKey()
    text_source_view_clear_selection(state)
}

def public text_source_view_dispose(var state : TextSourceViewState) {
    text_source_view_invalidate(state)
    state = TextSourceViewState()
}

def private style_id(span : TextSourceSpan const) : int {
    if (span.styles.marker) return SOURCE_STYLE_MARKER
    if (span.syntax_style != TextSourceSyntaxStyle.none) {
        return SOURCE_STYLE_SYNTAX_OFFSET + int(span.syntax_style)
    }
    return SOURCE_STYLE_REGULAR
}

def private source_color(style_id : int;
                         style : TextSourceViewStyle const) : float4 {
    let syntax_style = style_id - SOURCE_STYLE_SYNTAX_OFFSET
    if (syntax_style == int(TextSourceSyntaxStyle.comment)) return style.syntax_comment_color
    if (syntax_style == int(TextSourceSyntaxStyle.string_)) return style.syntax_string_color
    if (syntax_style == int(TextSourceSyntaxStyle.number)) return style.syntax_number_color
    if (syntax_style == int(TextSourceSyntaxStyle.keyword)) return style.syntax_keyword_color
    if (syntax_style == int(TextSourceSyntaxStyle.type_)) return style.syntax_type_color
    if (syntax_style == int(TextSourceSyntaxStyle.function_)) return style.syntax_function_color
    if (syntax_style == int(TextSourceSyntaxStyle.variable)) return style.syntax_variable_color
    if (syntax_style == int(TextSourceSyntaxStyle.property)) return style.syntax_property_color
    if (syntax_style == int(TextSourceSyntaxStyle.constant)) return style.syntax_constant_color
    if (syntax_style == int(TextSourceSyntaxStyle.operator_)) return style.syntax_operator_color
    if (syntax_style == int(TextSourceSyntaxStyle.punctuation)) return style.syntax_punctuation_color
    return style_id == SOURCE_STYLE_MARKER ? style.marker_color : style.text_color
}

def private range_columns(source : string; start_byte, end_byte, tab_cells : int) : int {
    if (end_byte <= start_byte) return 0
    var value := slice(source, start_byte, end_byte)
    var graphemes <- text_graphemes(value)
    var columns = 0
    for (grapheme in graphemes) {
        columns += (grapheme.first_codepoint == 0x0009u
            ? max(tab_cells, 1) : text_grapheme_cell_width(grapheme))
    }
    delete graphemes
    return columns
}

def private range_width(source : string; start_byte, end_byte, tab_cells : int;
                        cell_width : float) : float {
    return float(range_columns(source, start_byte, end_byte, tab_cells)) * cell_width
}

def private rebuild_cells(document : TextSourceDocument const;
                          var state : TextSourceViewState;
                          var font : ImFont?;
                          first_line, last_line : int) {
    if (state.cells_first_line == first_line
            && state.cells_last_line == last_line) return
    delete state.cells
    delete state.line_first_cell
    delete state.line_cell_count
    state.line_first_cell |> resize(length(state.layout.lines))
    state.line_cell_count |> resize(length(state.layout.lines))
    state.cells |> reserve(max(0, last_line - first_line + 1) * 128)
    for (line_index in range(length(state.layout.lines))) {
        state.line_first_cell[line_index] = -1
        state.line_cell_count[line_index] = 0
    }
    if (first_line >= 0 && last_line >= first_line) {
        for (line_index in range(first_line, min(last_line + 1,
                                                length(state.layout.lines)))) {
            let line = state.layout.lines[line_index]
            for (fragment_index in range(line.first_fragment,
                                         line.first_fragment + line.fragment_count)) {
                let fragment = state.layout.fragments[fragment_index]
                if (fragment.is_control) continue
                var value := slice(document.source,
                    fragment.source.start_byte, fragment.source.end_byte)
                var graphemes <- text_graphemes(value)
                var x = fragment.rect.x
                for (grapheme in graphemes) {
                    let start_byte = fragment.source.start_byte + grapheme.source.start_byte
                    let end_byte = fragment.source.start_byte + grapheme.source.end_byte
                    let columns = (grapheme.first_codepoint == 0x0009u
                        ? max(state.key.tab_cells, 1) : text_grapheme_cell_width(grapheme))
                    let width = float(columns) * state.key.cell_width
                    let natural_width = CalcTextSizeForFontRange(font, state.key.font_size,
                        document.source, length(document.source), start_byte, end_byte).x
                    let cell_index = length(state.cells)
                    state.cells |> push(TextSourceCell(
                        source = TextSourceRange(start_byte = start_byte, end_byte = end_byte),
                        style_id = fragment.style_id,
                        line_index = fragment.line_index,
                        rect = float4(x, fragment.rect.y, x + width, fragment.rect.w),
                        columns = columns,
                        natural_width = natural_width))
                    if (state.line_first_cell[line_index] < 0) {
                        state.line_first_cell[line_index] = cell_index
                    }
                    state.line_cell_count[line_index]++
                    x += width
                }
                delete graphemes
            }
        }
    }
    state.cells_first_line = first_line
    state.cells_last_line = last_line
}

def private ensure_layout(document : TextSourceDocument const;
                          var state : TextSourceViewState;
                          var style : TextSourceViewStyle;
                          available_width : float;
                          var font : ImFont?) {
    let zoom = source_zoom(style)
    let font_size = (style.fonts.size_px > 0.0f
        ? style.fonts.size_px : GetFontSize()) * zoom
    let line_height = max(font_size, GetTextLineHeight() * font_size / max(GetFontSize(), 1.0f))
    let cell_width = max(1.0f,
        CalcTextSizeForFontRange(font, font_size, "M", 1, 0, 1).x)
    let max_width = style.wrap ? max(available_width, cell_width) : 100000000.0f
    var key = TextSourceLayoutKey(
        document_revision = document.revision,
        max_width = max_width,
        font_size = font_size,
        line_height = line_height,
        cell_width = cell_width,
        font_id = unsafe(reinterpret<uint64>(font)),
        wrap = style.wrap,
        tab_cells = max(style.tab_cells, 1))
    if (state.layout_valid && same_layout_key(state.key, key)) {
        state.layout_hits++
        return
    }
    delete state.layout.lines
    delete state.layout.fragments
    var spans : array<TextFlowSpan>
    spans |> reserve(length(document.spans))
    for (span in document.spans) {
        spans |> push(TextFlowSpan(source = span.source, style_id = style_id(span)))
    }
    state.layout <- layout_text_flow_styled_ranges(document.source, spans,
        key.max_width, key.line_height) $(text : string;
                                          start_byte, end_byte, measured_style : int) : float {
            return range_width(text, start_byte, end_byte,
                               key.tab_cells, key.cell_width)
        }
    delete spans
    state.key = key
    state.layout_valid = true
    state.cells_first_line = -1
    state.cells_last_line = -1
    state.layout_misses++
}

def private line_number_gutter_width(document : TextSourceDocument const;
                                     style : TextSourceViewStyle const;
                                     font : ImFont?; font_size : float) : float {
    if (style.wrap || empty(document.line_numbers)) return 0.0f
    var largest = 0
    for (number in document.line_numbers) {
        largest = max(largest, number)
    }
    if (largest <= 0) return 0.0f
    let number_text = "{largest}"
    let text_width = CalcTextSizeForFontRange(
        font, font_size, number_text, length(number_text), 0, length(number_text)).x
    return text_width + max(8.0f, GetStyle().ItemSpacing.x)
}

def private draw_line_number(var dl : ImDrawList?; origin : float2;
                             line_y, gutter_width : float; number : int;
                             font : ImFont?; font_size : float;
                             color : float4) {
    if (number < 0 || gutter_width <= 0.0f) return
    let number_text = "{number}"
    let text_width = CalcTextSizeForFontRange(
        font, font_size, number_text, length(number_text), 0, length(number_text)).x
    let right_padding = max(4.0f, GetStyle().ItemSpacing.x * 0.5f)
    *dl |> AddTextRange(font, font_size,
        float2(origin.x + gutter_width - right_padding - text_width,
            origin.y + line_y),
        GetColorU32(color), number_text, length(number_text), 0, length(number_text))
}

def private line_for_y(state : TextSourceViewState const; y : float) : int {
    var lo = 0
    var hi = length(state.layout.lines)
    while (lo < hi) {
        let mid = (lo + hi) / 2
        let line = state.layout.lines[mid]
        if (line.y + line.height < y) {
            lo = mid + 1
        } else {
            hi = mid
        }
    }
    if (lo >= length(state.layout.lines)) return -1
    let line = state.layout.lines[lo]
    return y >= line.y && y <= line.y + line.height ? lo : -1
}

def private caret_for_byte(state : TextSourceViewState const; byte_offset : int) : TextFlowHit {
    for (cell_index in range(length(state.cells))) {
        let cell = state.cells[cell_index]
        if (byte_offset == cell.source.start_byte) {
            return TextFlowHit(valid = true, byte_offset = byte_offset,
                line_index = cell.line_index, fragment_index = cell_index,
                caret = float2(cell.rect.x, cell.rect.y),
                height = cell.rect.w - cell.rect.y)
        }
        if (byte_offset == cell.source.end_byte) {
            return TextFlowHit(valid = true, byte_offset = byte_offset,
                line_index = cell.line_index, fragment_index = cell_index,
                caret = float2(cell.rect.z, cell.rect.y),
                height = cell.rect.w - cell.rect.y, trailing = true)
        }
    }
    for (line_index in range(length(state.layout.lines))) {
        let line = state.layout.lines[line_index]
        if (byte_offset == line.source.start_byte || byte_offset == line.source.end_byte) {
            return TextFlowHit(valid = true, byte_offset = byte_offset,
                line_index = line_index, caret = float2(0.0f, line.y),
                height = line.height)
        }
    }
    return TextFlowHit()
}

def private hit_test(state : TextSourceViewState const; point : float2) : TextFlowHit {
    let line_index = line_for_y(state, point.y)
    if (line_index < 0) return TextFlowHit()
    let line = state.layout.lines[line_index]
    let first_cell = (line_index < length(state.line_first_cell)
        ? state.line_first_cell[line_index] : -1)
    let cell_count = (line_index < length(state.line_cell_count)
        ? state.line_cell_count[line_index] : 0)
    for (offset in range(cell_count)) {
        let cell_index = first_cell + offset
        let cell = state.cells[cell_index]
        if (point.x < cell.rect.x || point.x > cell.rect.z
                || point.y < cell.rect.y || point.y > cell.rect.w) continue
        let trailing = point.x >= (cell.rect.x + cell.rect.z) * 0.5f
        return TextFlowHit(valid = true,
            byte_offset = trailing ? cell.source.end_byte : cell.source.start_byte,
            line_index = cell.line_index, fragment_index = cell_index,
            caret = float2(trailing ? cell.rect.z : cell.rect.x, cell.rect.y),
            height = cell.rect.w - cell.rect.y, trailing = trailing)
    }
    if (first_cell < 0) {
        return TextFlowHit(valid = true, byte_offset = line.source.start_byte,
            line_index = line_index, caret = float2(0.0f, line.y), height = line.height)
    }
    let last_cell = first_cell + cell_count - 1
    let before = point.x <= state.cells[first_cell].rect.x
    let cell = before ? state.cells[first_cell] : state.cells[last_cell]
    return TextFlowHit(valid = true,
        byte_offset = before ? cell.source.start_byte : cell.source.end_byte,
        line_index = line_index, fragment_index = before ? first_cell : last_cell,
        caret = float2(before ? cell.rect.x : cell.rect.z, cell.rect.y),
        height = cell.rect.w - cell.rect.y, trailing = !before)
}

def private line_boundary(state : TextSourceViewState const;
                          byte_offset : int; end_ : bool) : int {
    let caret = caret_for_byte(state, byte_offset)
    if (!caret.valid || caret.line_index < 0
            || caret.line_index >= length(state.layout.lines)) return byte_offset
    let line = state.layout.lines[caret.line_index]
    return end_ ? line.source.end_byte : line.source.start_byte
}

def private vertical_byte(state : TextSourceViewState const;
                          byte_offset, direction : int) : int {
    let caret = caret_for_byte(state, byte_offset)
    if (!caret.valid) return byte_offset
    let target_line = caret.line_index + direction
    if (target_line < 0 || target_line >= length(state.layout.lines)) return byte_offset
    let line = state.layout.lines[target_line]
    let hit = hit_test(state, float2(caret.caret.x, line.y + line.height * 0.5f))
    return hit.valid ? hit.byte_offset : byte_offset
}

def private navigate(document : TextSourceDocument const;
                     var state : TextSourceViewState) {
    if (state.focus_byte < 0 || !IsWindowFocused(ImGuiFocusedFlags.RootAndChildWindows)
            || IsAnyItemActive()) return
    var action = 0
    if (IsKeyPressed(ImGuiKey.LeftArrow, false)) {
        action = 1
    } elif (IsKeyPressed(ImGuiKey.RightArrow, false)) {
        action = 2
    } elif (IsKeyPressed(ImGuiKey.UpArrow, false)) {
        action = 3
    } elif (IsKeyPressed(ImGuiKey.DownArrow, false)) {
        action = 4
    } elif (IsKeyPressed(ImGuiKey.Home, false)) {
        action = 5
    } elif (IsKeyPressed(ImGuiKey.End, false)) {
        action = 6
    }
    if (action == 0) return
    let io & = unsafe(GetIO())
    let selected = selected_source(state)
    let has_selection = selected.end_byte > selected.start_byte
    var next = state.focus_byte
    if (has_selection && !io.KeyShift && (action == 1 || action == 2)) {
        next = action == 1 ? selected.start_byte : selected.end_byte
        state.anchor_byte = next
    } elif (action == 1) {
        next = text_previous_grapheme_boundary(document.source, state.focus_byte)
    } elif (action == 2) {
        next = text_next_grapheme_boundary(document.source, state.focus_byte)
    } elif (action == 3 || action == 4) {
        next = vertical_byte(state, state.focus_byte, action == 3 ? -1 : 1)
    } else {
        next = line_boundary(state, state.focus_byte, action == 6)
    }
    if (!io.KeyShift) {
        state.anchor_byte = next
    }
    if (next != state.focus_byte || (has_selection && !io.KeyShift)) {
        state.focus_byte = next
        state.selection_changed = true
        state.selection_revision++
        state.navigation_revision++
    }
}

def private include_selection_rect(var state : TextSourceViewState; rect : float4) {
    if (!state.selection_screen_valid) {
        state.selection_screen_bounds = rect
        state.selection_screen_valid = true
    } else {
        state.selection_screen_bounds.x = min(state.selection_screen_bounds.x, rect.x)
        state.selection_screen_bounds.y = min(state.selection_screen_bounds.y, rect.y)
        state.selection_screen_bounds.z = max(state.selection_screen_bounds.z, rect.z)
        state.selection_screen_bounds.w = max(state.selection_screen_bounds.w, rect.w)
    }
}

def private selection_styles(document : TextSourceDocument const;
                             selection : TextSourceRange const) : TextSourceSelectionStyles {
    var result : TextSourceSelectionStyles
    var regular = false
    var markers = false
    for (span in document.spans) {
        if (span.source.end_byte <= selection.start_byte
                || span.source.start_byte >= selection.end_byte) continue
        markers = markers || span.styles.marker
        regular = regular || !span.styles.marker
    }
    result.all_regular = regular && !markers
    result.has_markers = markers
    result.has_mixed_styles = regular && markers
    return result
}

def private prepare_virtual_lines(document : TextSourceDocument const;
                                  var state : TextSourceViewState) {
    if (state.virtual_document_revision == document.revision) return
    delete state.virtual_line_starts
    state.virtual_line_starts |> push(0)
    state.virtual_max_line_bytes = 0
    var line_start = 0
    peek_data(document.source) $(bytes) {
        for (at in range(length(bytes))) {
            if (int(bytes[at]) != '\n') continue
            state.virtual_max_line_bytes = max(
                state.virtual_max_line_bytes, at - line_start)
            line_start = at + 1
            if (line_start < length(bytes)) {
                state.virtual_line_starts |> push(line_start)
            }
        }
    }
    state.virtual_max_line_bytes = max(state.virtual_max_line_bytes,
        length(document.source) - line_start)
    state.virtual_document_revision = document.revision
}

def private virtual_line_mark(document : TextSourceDocument const;
                              line_index : int;
                              var kind : TextSourceLineMarkKind&) : bool {
    for (mark in document.line_marks) {
        if (mark.line_index == line_index) {
            kind = mark.kind
            return true
        }
    }
    return false
}

def private virtual_first_span(document : TextSourceDocument const;
                               start_byte : int) : int {
    var lo = 0
    var hi = length(document.spans)
    while (lo < hi) {
        let mid = (lo + hi) / 2
        if (document.spans[mid].source.end_byte <= start_byte) {
            lo = mid + 1
        } else {
            hi = mid
        }
    }
    return lo
}

def private virtual_line_end(document : TextSourceDocument const;
                             state : TextSourceViewState const;
                             line_index : int) : int {
    var result = length(document.source)
    if (line_index + 1 < length(state.virtual_line_starts)) {
        result = max(state.virtual_line_starts[line_index],
            state.virtual_line_starts[line_index + 1] - 1)
    }
    if (result > state.virtual_line_starts[line_index]
            && slice(document.source, result - 1, result) == "\r") {
        result--
    }
    return result
}

def private virtual_range_width(source : string; start_byte, end_byte : int;
                                var font : ImFont?; font_size, cell_width : float;
                                tab_cells : int) : float {
    if (end_byte <= start_byte) return 0.0f
    var width = 0.0f
    peek_data(source) $(bytes) {
        var text_start = start_byte
        for (at in range(start_byte, end_byte)) {
            if (int(bytes[at]) != '\t') continue
            if (at > text_start) {
                width += CalcTextSizeForFontRange(font, font_size, source, length(source),
                    text_start, at).x
            }
            width += float(max(tab_cells, 1)) * cell_width
            text_start = at + 1
        }
        if (text_start < end_byte) {
            width += CalcTextSizeForFontRange(font, font_size, source, length(source),
                text_start, end_byte).x
        }
    }
    return width
}

def private draw_virtual_range(var dl : ImDrawList?; var font : ImFont?;
                               font_size : float; origin : float2; color : float4;
                               source : string; start_byte, end_byte : int;
                               cell_width : float; tab_cells : int) : float {
    if (end_byte <= start_byte) return 0.0f
    var x = origin.x
    peek_data(source) $(bytes) {
        var text_start = start_byte
        for (at in range(start_byte, end_byte)) {
            if (int(bytes[at]) != '\t') continue
            if (at > text_start) {
                *dl |> AddTextRange(font, font_size, float2(x, origin.y),
                    GetColorU32(color), source, length(source), text_start, at)
                x += CalcTextSizeForFontRange(font, font_size, source, length(source),
                    text_start, at).x
            }
            x += float(max(tab_cells, 1)) * cell_width
            text_start = at + 1
        }
        if (text_start < end_byte) {
            *dl |> AddTextRange(font, font_size, float2(x, origin.y),
                GetColorU32(color), source, length(source), text_start, end_byte)
            x += CalcTextSizeForFontRange(font, font_size, source, length(source),
                text_start, end_byte).x
        }
    }
    return x - origin.x
}

def private virtual_hit_byte(document : TextSourceDocument const;
                             state : TextSourceViewState const;
                             line_index : int; local_x : float;
                             var font : ImFont?; font_size, cell_width : float;
                             tab_cells : int) : int {
    let start_byte = state.virtual_line_starts[line_index]
    let end_byte = virtual_line_end(document, state, line_index)
    if (local_x <= 0.0f || end_byte <= start_byte) return start_byte
    let value = slice(document.source, start_byte, end_byte)
    var graphemes <- text_graphemes(value)
    var x = 0.0f
    var result = end_byte
    for (grapheme in graphemes) {
        let grapheme_start = start_byte + grapheme.source.start_byte
        let grapheme_end = start_byte + grapheme.source.end_byte
        let width = (grapheme.first_codepoint == 0x0009u
            ? float(max(tab_cells, 1)) * cell_width
            : CalcTextSizeForFontRange(font, font_size,
                document.source, length(document.source),
                grapheme_start, grapheme_end).x)
        if (local_x < x + width * 0.5f) {
            result = grapheme_start
            break
        }
        if (local_x < x + width) {
            result = grapheme_end
            break
        }
        x += width
    }
    delete graphemes
    return result
}

def private text_source_virtual_view(id : string;
                                     document : TextSourceDocument const;
                                     var state : TextSourceViewState;
                                     var style : TextSourceViewStyle) : TextSourceViewResult {
    var result = TextSourceViewResult()
    prepare_virtual_lines(document, state)
    state.virtual_mode = true
    state.virtual_visible_syntax_runs = 0
    state.render_revision++
    state.selection_changed = false
    state.selection_screen_valid = false
    state.caret_screen_valid = false
    var fallback = GetFont()
    var font = source_font(style, fallback)
    let zoom = source_zoom(style)
    let font_size = (style.fonts.size_px > 0.0f
        ? style.fonts.size_px : GetFontSize()) * zoom
    let line_height = max(font_size,
        GetTextLineHeight() * font_size / max(GetFontSize(), 1.0f))
    let cell_width = max(1.0f,
        CalcTextSizeForFontRange(font, font_size, "M", 1, 0, 1).x)
    let number_width = line_number_gutter_width(
        document, style, font, font_size)
    let gutter_width = (!empty(document.line_marks)
        ? max(1.0f, style.line_mark_width * zoom) : 0.0f)
    let mark_space = (gutter_width > 0.0f
        ? gutter_width + max(4.0f, GetStyle().ItemSpacing.x) : 0.0f)
    let gutter_space = number_width + mark_space
    let available = max(GetContentRegionAvail().x - gutter_space, 1.0f)
    let content_width = max(available,
        float(state.virtual_max_line_bytes) * cell_width)
    let initial_origin = GetCursorScreenPos()
    state.screen_origin = initial_origin + float2(gutter_space, 0.0f)
    state.screen_size = float2(content_width,
        float(length(state.virtual_line_starts)) * line_height)
    let local_mouse = GetMousePos() - state.screen_origin
    var hit_byte = -1
    if (!empty(state.virtual_line_starts)
            && (IsWindowHovered(ImGuiHoveredFlags.None)
                || state.selection_dragging)) {
        let hit_line = clamp(int(floor(local_mouse.y / line_height)),
            0, length(state.virtual_line_starts) - 1)
        hit_byte = virtual_hit_byte(document, state, hit_line,
            local_mouse.x, font, font_size, cell_width, style.tab_cells)
    }
    if (hit_byte >= 0 && IsWindowHovered(ImGuiHoveredFlags.None)
            && IsMouseClicked(ImGuiMouseButton.Left, false)) {
        state.anchor_byte = hit_byte
        state.focus_byte = hit_byte
        state.selection_dragging = true
        state.selection_changed = true
        state.selection_revision++
    } elif (hit_byte >= 0 && state.selection_dragging
            && IsMouseDown(ImGuiMouseButton.Left)) {
        if (state.focus_byte != hit_byte) {
            state.focus_byte = hit_byte
            state.selection_changed = true
            state.selection_revision++
        }
    }
    if (state.selection_dragging && IsMouseReleased(ImGuiMouseButton.Left)) {
        state.selection_dragging = false
        state.selection_commit_revision++
    }
    let selection = selected_source(state)
    let has_selection = (selection.start_byte >= 0
        && selection.end_byte > selection.start_byte)
    var dl = GetWindowDrawList()
    PushID(id)
    unsafe {
        var clipper = ImGuiListClipper()
        clipper |> Begin(length(state.virtual_line_starts), line_height)
        while (clipper |> Step()) {
            for (line_index in range(clipper.DisplayStart, clipper.DisplayEnd)) {
                let row_origin = GetCursorScreenPos()
                Dummy(float2(content_width + gutter_space, line_height))
                let start_byte = state.virtual_line_starts[line_index]
                let end_byte = virtual_line_end(document, state, line_index)
                var mark_kind : TextSourceLineMarkKind
                if (virtual_line_mark(document, line_index, mark_kind)) {
                    let color = line_mark_color(mark_kind, style)
                    if (style.line_mark_background_alpha > 0.0f) {
                        var background = color
                        background.w = style.line_mark_background_alpha
                        *dl |> AddRectFilled(row_origin,
                            row_origin + float2(content_width + gutter_space, line_height),
                            GetColorU32(background), 0.0f, ImDrawFlags.None)
                    }
                    *dl |> AddRectFilled(
                        float2(row_origin.x + number_width, row_origin.y),
                        float2(row_origin.x + number_width + gutter_width,
                            row_origin.y + line_height),
                        GetColorU32(color), 0.0f, ImDrawFlags.None)
                }
                if (line_index < length(document.line_numbers)) {
                    draw_line_number(dl, row_origin, 0.0f, number_width,
                        document.line_numbers[line_index], font, font_size,
                        style.line_number_color)
                }
                if (has_selection && selection.end_byte > start_byte
                        && selection.start_byte < end_byte) {
                    let selected_start = max(start_byte, selection.start_byte)
                    let selected_end = min(end_byte, selection.end_byte)
                    let selected_x0 = (row_origin.x + gutter_space
                        + virtual_range_width(document.source, start_byte, selected_start,
                            font, font_size, cell_width, style.tab_cells))
                    let selected_x1 = (row_origin.x + gutter_space
                        + virtual_range_width(document.source, start_byte, selected_end,
                            font, font_size, cell_width, style.tab_cells))
                    let selected_p0 = float2(selected_x0, row_origin.y)
                    let selected_p1 = float2(max(selected_x0 + 1.0f, selected_x1),
                        row_origin.y + line_height)
                    *dl |> AddRectFilled(selected_p0, selected_p1,
                        GetColorU32(style.selection_color),
                        1.0f * zoom, ImDrawFlags.None)
                    include_selection_rect(state,
                        float4(selected_p0.x, selected_p0.y,
                            selected_p1.x, selected_p1.y))
                }
                var x = row_origin.x + gutter_space
                var span_index = virtual_first_span(document, start_byte)
                while (span_index < length(document.spans)
                        && document.spans[span_index].source.start_byte < end_byte) {
                    let span = document.spans[span_index]
                    let run_start = max(start_byte, span.source.start_byte)
                    let run_end = min(end_byte, span.source.end_byte)
                    if (run_end > run_start) {
                        let run_style = style_id(span)
                        x += draw_virtual_range(dl, font, font_size,
                            float2(x, row_origin.y), source_color(run_style, style),
                            document.source, run_start, run_end,
                            cell_width, style.tab_cells)
                        if (run_style > SOURCE_STYLE_MARKER) {
                            state.virtual_visible_syntax_runs++
                        }
                    }
                    span_index++
                }
            }
        }
        clipper |> End()
    }
    let copy_requested = state.selection_copy_pending
    state.selection_copy_pending = false
    if (has_selection && copy_requested) {
        result.selection_copy_requested = true
        result.selection_copy_text := slice(document.source,
            selection.start_byte, selection.end_byte)
        state.selection_copy_text := result.selection_copy_text
        state.selection_copy_revision++
        if (style.write_system_clipboard) {
            result.selection_copied = (clipboard_set_text(result.selection_copy_text)
                == ClipboardStatus.ok)
        }
    }
    if (has_selection && hit_byte >= selection.start_byte
            && hit_byte <= selection.end_byte
            && IsMouseReleased(ImGuiMouseButton.Right)) {
        result.selection_context_requested = true
        result.selection_context_pos = GetMousePos()
        state.selection_context_pos = result.selection_context_pos
        state.selection_context_revision++
    }
    result.selection_changed = state.selection_changed
    result.selection = TextSourceSelection(
        active = has_selection,
        source = selection,
        styles = selection_styles(document, selection),
        screen_valid = state.selection_screen_valid,
        screen_bounds = state.selection_screen_bounds,
        screen_anchor = state.selection_screen_anchor)
    PopID()
    return <- result
}

def public text_source_view(id : string; document : TextSourceDocument const;
                            var state : TextSourceViewState;
                            var style : TextSourceViewStyle) : TextSourceViewResult {
    if (!style.wrap && length(document.source) >= 128 * 1024) {
        return <- text_source_virtual_view(id, document, state, style)
    }
    state.virtual_mode = false
    var result = TextSourceViewResult()
    var fallback = GetFont()
    var font = source_font(style, fallback)
    let zoom = source_zoom(style)
    let font_size = (style.fonts.size_px > 0.0f
        ? style.fonts.size_px : GetFontSize()) * zoom
    let number_width = line_number_gutter_width(
        document, style, font, font_size)
    let gutter_width = (!empty(document.line_marks)
        ? max(1.0f, style.line_mark_width * zoom) : 0.0f)
    let mark_space = (gutter_width > 0.0f
        ? gutter_width + max(4.0f, GetStyle().ItemSpacing.x) : 0.0f)
    let gutter_space = number_width + mark_space
    let available = max(GetContentRegionAvail().x - gutter_space, 1.0f)
    ensure_layout(document, state, style, available, font)
    state.render_revision++
    state.selection_changed = false
    state.selection_screen_valid = false
    state.caret_screen_valid = false

    let gutter_origin = GetCursorScreenPos()
    let origin = gutter_origin + float2(gutter_space, 0.0f)
    state.screen_origin = origin
    state.screen_size = float2(max(available, state.layout.size.x),
        max(state.key.line_height, state.layout.size.y))
    PushID(id)
    Dummy(float2(state.screen_size.x + gutter_space, state.screen_size.y))

    let window_pos = GetWindowPos()
    let window_size = GetWindowSize()
    let visible_min = window_pos
    let visible_max = window_pos + window_size
    let local_visible_top = visible_min.y - origin.y
    let local_visible_bottom = visible_max.y - origin.y
    var first_visible_line = line_for_y(state, max(0.0f, local_visible_top))
    var last_visible_line = line_for_y(state, max(0.0f, local_visible_bottom))
    if (!empty(state.layout.lines)) {
        if (first_visible_line < 0) {
            first_visible_line = (local_visible_top <= 0.0f
                ? 0 : length(state.layout.lines) - 1)
        }
        if (last_visible_line < 0) {
            last_visible_line = (local_visible_bottom <= 0.0f
                ? 0 : length(state.layout.lines) - 1)
        }
        first_visible_line = max(0, first_visible_line - 2)
        last_visible_line = min(length(state.layout.lines) - 1,
                                last_visible_line + 2)
    }
    rebuild_cells(document, state, font, first_visible_line, last_visible_line)

    let local_mouse = GetMousePos() - origin
    let hit = hit_test(state, local_mouse)
    if (hit.valid && IsWindowHovered(ImGuiHoveredFlags.None)
            && IsMouseClicked(ImGuiMouseButton.Left, false)) {
        state.anchor_byte = hit.byte_offset
        state.focus_byte = hit.byte_offset
        state.selection_dragging = true
        state.selection_changed = true
        state.selection_revision++
    } elif (hit.valid && state.selection_dragging && IsMouseDown(ImGuiMouseButton.Left)) {
        if (state.focus_byte != hit.byte_offset) {
            state.focus_byte = hit.byte_offset
            state.selection_changed = true
            state.selection_revision++
        }
    }
    if (state.selection_dragging && IsMouseReleased(ImGuiMouseButton.Left)) {
        state.selection_dragging = false
        state.selection_commit_revision++
    }

    navigate(document, state)
    let selection = selected_source(state)
    let has_selection = (selection.start_byte >= 0
        && selection.end_byte > selection.start_byte)
    var dl = GetWindowDrawList()
    for (mark in document.line_marks) {
        if (mark.line_index < 0 || mark.line_index >= length(state.layout.lines)) continue
        let line = state.layout.lines[mark.line_index]
        let p0 = gutter_origin + float2(0.0f, line.y)
        let p1 = origin + float2(state.screen_size.x, line.y + line.height)
        if (p1.y < visible_min.y || p0.y > visible_max.y) continue
        let color = line_mark_color(mark.kind, style)
        if (style.line_mark_background_alpha > 0.0f) {
            var background = color
            background.w = style.line_mark_background_alpha
            *dl |> AddRectFilled(p0, p1, GetColorU32(background), 0.0f, ImDrawFlags.None)
        }
        *dl |> AddRectFilled(float2(p0.x + number_width, p0.y),
            float2(p0.x + number_width + gutter_width, p1.y),
            GetColorU32(color), 0.0f, ImDrawFlags.None)
    }
    if (number_width > 0.0f) {
        for (line_index in range(min(length(document.line_numbers),
                length(state.layout.lines)))) {
            let line = state.layout.lines[line_index]
            let p0 = gutter_origin + float2(0.0f, line.y)
            if (p0.y + line.height < visible_min.y || p0.y > visible_max.y) continue
            draw_line_number(dl, gutter_origin, line.y, number_width,
                document.line_numbers[line_index], font, state.key.font_size,
                style.line_number_color)
        }
    }
    var first_visible_cell = 0
    var visible_hi = length(state.cells)
    while (first_visible_cell < visible_hi) {
        let mid = (first_visible_cell + visible_hi) / 2
        if (state.cells[mid].rect.w < local_visible_top) {
            first_visible_cell = mid + 1
        } else {
            visible_hi = mid
        }
    }
    for (cell_index in range(first_visible_cell, length(state.cells))) {
        let cell = state.cells[cell_index]
        if (cell.rect.y > local_visible_bottom) break
        let p0 = origin + float2(cell.rect.x, cell.rect.y)
        let p1 = origin + float2(cell.rect.z, cell.rect.w)
        if (p1.x < visible_min.x || p0.x > visible_max.x
                || p1.y < visible_min.y || p0.y > visible_max.y) continue
        let selected = (has_selection
            && cell.source.end_byte > selection.start_byte
            && cell.source.start_byte < selection.end_byte)
        if (selected) {
            let rect = float4(p0.x, p0.y, p1.x, p1.y)
            *dl |> AddRectFilled(p0, p1, GetColorU32(style.selection_color),
                                 1.0f * zoom, ImDrawFlags.None)
            include_selection_rect(state, rect)
            if (IsMouseReleased(ImGuiMouseButton.Right)
                    && IsMouseHoveringRect(p0, p1, true)) {
                result.selection_context_requested = true
                result.selection_context_pos = GetMousePos()
                state.selection_context_pos = result.selection_context_pos
                state.selection_context_revision++
            }
        }
        let draw_x = p0.x + max(0.0f,
            (p1.x - p0.x - cell.natural_width) * 0.5f)
        let color = source_color(cell.style_id, style)
        *dl |> PushClipRect(p0, p1, true)
        *dl |> AddTextRange(font, state.key.font_size,
            float2(draw_x, p0.y), GetColorU32(color),
            document.source, length(document.source),
            cell.source.start_byte, cell.source.end_byte)
        if (cell.style_id == SOURCE_STYLE_MARKER) {
            *dl |> AddTextRange(font, state.key.font_size,
                float2(draw_x + max(0.5f, 0.35f * zoom), p0.y), GetColorU32(color),
                document.source, length(document.source),
                cell.source.start_byte, cell.source.end_byte)
        }
        *dl |> PopClipRect()
    }

    if (state.focus_byte >= 0) {
        let caret = caret_for_byte(state, state.focus_byte)
        if (caret.valid) {
            let top = origin + caret.caret
            state.caret_screen_valid = true
            state.caret_screen_pos = top
            state.caret_screen_height = caret.height
            state.selection_screen_anchor = float2(top.x, top.y + caret.height)
            if (!has_selection && IsWindowFocused(ImGuiFocusedFlags.RootAndChildWindows)) {
                *dl |> AddLine(top, float2(top.x, top.y + caret.height),
                               GetColorU32(style.caret_color), max(1.0f, zoom))
            }
        }
    }

    let copy_requested = state.selection_copy_pending
    state.selection_copy_pending = false
    if (has_selection && copy_requested) {
        result.selection_copy_requested = true
        result.selection_copy_text := slice(document.source,
            selection.start_byte, selection.end_byte)
        state.selection_copy_text := result.selection_copy_text
        state.selection_copy_revision++
        if (style.write_system_clipboard) {
            result.selection_copied = (clipboard_set_text(result.selection_copy_text)
                == ClipboardStatus.ok)
        }
    }

    result.selection_changed = state.selection_changed
    result.selection = TextSourceSelection(
        active = has_selection,
        source = selection,
        styles = selection_styles(document, selection),
        screen_valid = state.selection_screen_valid,
        screen_bounds = state.selection_screen_bounds,
        screen_anchor = state.selection_screen_anchor)
    PopID()
    return <- result
}
