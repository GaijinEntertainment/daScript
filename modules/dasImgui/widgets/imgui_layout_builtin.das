options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_layout_builtin shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_boost_v2 public
require daslib/json public
require math

//! §4.6 layout helpers — stateful, draggable.
//!
//! Three widgets ride on the same `SliderStateFloat` shape:
//!   - `split_h(SPLIT, init, bounds, ${left}, ${right})` — value is the left
//!     pane's fraction of available width (0..1). Drag handle between panes.
//!   - `split_v(SPLIT, init, bounds, ${top}, ${bottom})` — value is the top
//!     pane's fraction of available height.
//!   - `dock_left(SIDEBAR, init, bounds) { sidebar contents }` — value is the
//!     sidebar's pixel width. No second block; main pane content flows after
//!     the call via SameLine.
//!
//! Reusing `SliderStateFloat` means the existing `pending_value_finalize`
//! dispatcher handles `imgui_set` remote control with no per-kind boilerplate.
//! The `kind` string distinguishes telemetry; the state shape is identical.

let private HANDLE_PX = 4.0f
let private HANDLE_HIT_PAD = 2.0f  // extra hit-area on each side for easier grab

// ===== Draggable handle primitives =====

def private draggable_handle_v(var state : SliderStateFloat; px_per_unit : float;
                               size : ImVec2; tag : string) : bool {
    //! Vertical splitter line; drag horizontally to resize. Returns true on
    //! mouse drag, with `state.value` already clamped to bounds.
    let cursor_min = GetCursorScreenPos()
    InvisibleButton(tag, size)
    let cursor_max = ImVec2(cursor_min.x + size.x, cursor_min.y + size.y)
    let hot = IsItemHovered() || IsItemActive()
    if (hot) {
        SetMouseCursor(ImGuiMouseCursor.ResizeEW)
    }
    var line_col = GetColorU32(ImGuiCol.Separator)
    if (hot) {
        line_col = GetColorU32(ImGuiCol.SeparatorActive)
    }
    let mid_x = (cursor_min.x + cursor_max.x) * 0.5f
    *GetWindowDrawList() |> AddLine(ImVec2(mid_x, cursor_min.y),
                                    ImVec2(mid_x, cursor_max.y), line_col, 1.0f)
    var changed = false
    if (IsItemActive()) {
        let dx = GetIO().MouseDelta.x
        if (dx != 0.0f && px_per_unit != 0.0f) {
            let (mn, mx) = state.bounds
            state.value = clamp(state.value + dx / px_per_unit, mn, mx)
            changed = true
        }
    }
    return changed
}

def private draggable_handle_h(var state : SliderStateFloat; px_per_unit : float;
                               size : ImVec2; tag : string) : bool {
    //! Horizontal splitter line; drag vertically to resize. Sibling of
    //! `draggable_handle_v` for split_v.
    let cursor_min = GetCursorScreenPos()
    InvisibleButton(tag, size)
    let cursor_max = ImVec2(cursor_min.x + size.x, cursor_min.y + size.y)
    let hot = IsItemHovered() || IsItemActive()
    if (hot) {
        SetMouseCursor(ImGuiMouseCursor.ResizeNS)
    }
    var line_col = GetColorU32(ImGuiCol.Separator)
    if (hot) {
        line_col = GetColorU32(ImGuiCol.SeparatorActive)
    }
    let mid_y = (cursor_min.y + cursor_max.y) * 0.5f
    *GetWindowDrawList() |> AddLine(ImVec2(cursor_min.x, mid_y),
                                    ImVec2(cursor_max.x, mid_y), line_col, 1.0f)
    var changed = false
    if (IsItemActive()) {
        let dy = GetIO().MouseDelta.y
        if (dy != 0.0f && px_per_unit != 0.0f) {
            let (mn, mx) = state.bounds
            state.value = clamp(state.value + dy / px_per_unit, mn, mx)
            changed = true
        }
    }
    return changed
}

// ===== split_h =====

[container]
def split_h(var state : SliderStateFloat; left_blk : block; right_blk : block) {
    //! Horizontal split — left fraction in `state.value` (0..1). The handle
    //! is a thin vertical line between two BeginChild panes. Auto-emits
    //! `SliderStateFloat` global with `init=` / `bounds=` named-arg sugar.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let avail = GetContentRegionAvail()
    let total_w = avail.x
    let total_h = avail.y
    let handle_w = HANDLE_PX + HANDLE_HIT_PAD * 2.0f
    let cur_pct = clamp(state.value, mn, mx)
    var left_w = total_w * cur_pct - handle_w * 0.5f
    var right_w = total_w - left_w - handle_w
    if (left_w < 0.0f) {
        left_w = 0.0f
    }
    if (right_w < 0.0f) {
        right_w = 0.0f
    }
    BeginChild("{widget_ident}__L", ImVec2(left_w, total_h),
               ImGuiChildFlags.None, ImGuiWindowFlags.None)
    invoke(left_blk)
    EndChild()
    SameLine(0.0f, 0.0f)
    if (draggable_handle_v(state, total_w, ImVec2(handle_w, total_h),
                           "{widget_ident}__H")) {
        state.changed = true
    }
    SameLine(0.0f, 0.0f)
    BeginChild("{widget_ident}__R", ImVec2(right_w, total_h),
               ImGuiChildFlags.None, ImGuiWindowFlags.None)
    invoke(right_blk)
    EndChild()
    pending_value_container_finalize(widget_ident, "split_h", state)
}

// ===== split_v =====

[container]
def split_v(var state : SliderStateFloat; top_blk : block; bottom_blk : block) {
    //! Vertical split — top fraction in `state.value` (0..1). Handle is a thin
    //! horizontal line between two BeginChild panes.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let avail = GetContentRegionAvail()
    let total_w = avail.x
    let total_h = avail.y
    let handle_h = HANDLE_PX + HANDLE_HIT_PAD * 2.0f
    let cur_pct = clamp(state.value, mn, mx)
    var top_h = total_h * cur_pct - handle_h * 0.5f
    var bottom_h = total_h - top_h - handle_h
    if (top_h < 0.0f) {
        top_h = 0.0f
    }
    if (bottom_h < 0.0f) {
        bottom_h = 0.0f
    }
    BeginChild("{widget_ident}__T", ImVec2(total_w, top_h),
               ImGuiChildFlags.None, ImGuiWindowFlags.None)
    invoke(top_blk)
    EndChild()
    if (draggable_handle_h(state, total_h, ImVec2(total_w, handle_h),
                           "{widget_ident}__H")) {
        state.changed = true
    }
    BeginChild("{widget_ident}__B", ImVec2(total_w, bottom_h),
               ImGuiChildFlags.None, ImGuiWindowFlags.None)
    invoke(bottom_blk)
    EndChild()
    pending_value_container_finalize(widget_ident, "split_v", state)
}

// ===== dock_left =====

[container]
def dock_left(var state : SliderStateFloat; blk : block) {
    //! Fixed-orientation sidebar on the left — `state.value` is the sidebar
    //! width in pixels (NOT a fraction). Real docking (DockBuilder) is
    //! `imgui_internal.h` and off-limits per the project rules, so this is
    //! a stable-position sidebar with a draggable right edge. Main pane
    //! content flows after the call via the trailing SameLine.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let avail = GetContentRegionAvail()
    let total_h = avail.y
    let handle_w = HANDLE_PX + HANDLE_HIT_PAD * 2.0f
    var sidebar_w = clamp(state.value, mn, mx)
    if (sidebar_w < 0.0f) {
        sidebar_w = 0.0f
    }
    BeginChild("{widget_ident}__SIDE", ImVec2(sidebar_w, total_h),
               ImGuiChildFlags.None, ImGuiWindowFlags.None)
    invoke(blk)
    EndChild()
    SameLine(0.0f, 0.0f)
    if (draggable_handle_v(state, 1.0f, ImVec2(handle_w, total_h),
                           "{widget_ident}__H")) {
        state.changed = true
    }
    SameLine(0.0f, 0.0f)
    pending_value_container_finalize(widget_ident, "dock_left", state)
}
