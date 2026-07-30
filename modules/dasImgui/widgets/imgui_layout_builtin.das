options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options _allow_imgui_legacy = true

module imgui_layout_builtin shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_boost_v2 public
require daslib/json public
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost
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
//! dispatcher handles `imgui_force_set` remote control with no per-kind boilerplate.
//! The `kind` string distinguishes telemetry; the state shape is identical.

let private HANDLE_PX = 4.0f
let private HANDLE_HIT_PAD = 2.0f  // extra hit-area on each side for easier grab

// ===== Draggable handle primitives =====

def private draggable_handle_v(var state : SliderStateFloat; px_per_unit : float;
                               size : ImVec2; tag : string) : bool {
    let cursor_min = GetCursorScreenPos()
    InvisibleButton(tag, size)
    register_handle_bbox(tag)   // expose handle geometry as <container>/HANDLE for real synth drags
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
    let cursor_min = GetCursorScreenPos()
    InvisibleButton(tag, size)
    register_handle_bbox(tag)   // expose handle geometry as <container>/HANDLE for real synth drags
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
    //! Horizontal split — left pane width as fraction of total in ``state.value`` (0..1); thin draggable vertical handle between two ``BeginChild`` panes.
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
    // Group the whole split as ONE layout item (see split_v) so a splitter composed
    // after a full-height SameLine'd sibling stacks/flows as a unit, not line-by-line.
    BeginGroup()
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
    EndGroup()
    pending_value_container_finalize(widget_ident, "split_h", state)
}

// ===== split_v =====

[container]
def split_v(var state : SliderStateFloat; top_blk : block; bottom_blk : block) {
    //! Vertical split — top pane height as fraction of total in ``state.value`` (0..1); thin draggable horizontal handle between two ``BeginChild`` panes.
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
    // BeginGroup makes the whole vertical split ONE layout item. Without it, when split_v is composed after
    // dock_left's trailing SameLine, the top child shares a line with the full-height sidebar; closing it then
    // advances the cursor by the sidebar's height (not top_h), dropping the handle + bottom pane below the fold.
    BeginGroup()
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
    EndGroup()
    pending_value_container_finalize(widget_ident, "split_v", state)
}

// ===== dock_left =====

[container]
def dock_left(var state : SliderStateFloat; blk : block) {
    //! Left sidebar with a draggable right edge — ``state.value`` is sidebar width in *pixels* (not a fraction). Main pane flows after the call via ``SameLine``.
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

// (split_h / split_v / dock_left all use SliderStateFloat — the dispatcher
//  for that state struct lives in imgui_widgets_builtin and covers them
//  automatically since dispatch is keyed by state ti.hash.)

// ===== columns =====

def public columns_open(n : int; id : string = ""; border : bool = true) {
    //! Open an N-column block. `id` (default empty) is the ImGui id-stack tag for column-width persistence;
    //! `border` (default on) draws the vertical dividers. Primitive emitted by the ``columns(N, …)``
    //! call_macro; callable directly when the column count or shape is dynamic.
    Columns(n, id, border)
}

def public next_col() {
    //! Advance to the next column. Underlying primitive emitted by the ``columns(N, …)`` call_macro.
    NextColumn()
}

def public columns_close() {
    //! Close a columns block (return to single-column layout). Underlying primitive emitted by the ``columns(N, …)`` call_macro.
    Columns(1, "", false)
}

// ===== tree_node primitives =====

def public tree_node_open(text : string;
                          flags : ImGuiTreeNodeFlags = ImGuiTreeNodeFlags.None) : bool {
    //! `TreeNodeEx(label, flags) : bool` — open form WITHOUT auto-pair TreePop (use when the body renders
    //! outside the conditional, e.g. ``same_line()`` after the header). Caller MUST invoke ``tree_pop()`` when the
    //! return is true AND ``NoTreePushOnOpen`` was NOT set (that flag skips TreePush, so a TreePop would underflow).
    return TreeNodeEx(text, flags)
}

def public tree_pop() {
    //! Pair with `tree_node_open` (or the leaf-bool TreeNodeEx overload) when the return is true AND
    //! `NoTreePushOnOpen` was NOT set. The `tree_node` container auto-pairs TreePop; this covers the manual case.
    TreePop()
}

// ===== list_clipper =====

def public list_clipper(items_count : int;
                        body : block<(start : int; end_ : int) : void>) {
    //! Cull a virtual list of `items_count` items to the visible region.
    //! The block fires once per visible chunk with `[start, end_)` indices;
    //! render exactly those rows inside the block.
    unsafe {
        var clipper = ImGuiListClipper()
        clipper |> Begin(items_count, -1.0f)
        while (clipper |> Step()) {
            invoke(body, clipper.DisplayStart, clipper.DisplayEnd)
        }
        clipper |> End()
    }
}

[call_macro(name = "columns")]
class private ColumnsCallMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        let nargs = length(call.arguments)
        macro_verify(nargs >= 2, prog, call.at,
                     "columns(N, ...) requires the column count and at least one block")
        return null if (nargs < 2)
        let nArg = call.arguments[0]
        macro_verify(nArg is ExprConstInt, prog, nArg.at,
                     "columns: first argument must be an integer literal")
        return null if (!(nArg is ExprConstInt))
        let nLit = (nArg as ExprConstInt).value
        macro_verify(nLit > 0, prog, nArg.at,
                     "columns: column count must be > 0")
        return null if (nLit <= 0)
        let blockCount = nargs - 1
        macro_verify(blockCount == nLit, prog, call.at,
                     "columns({nLit}, ...): expected {nLit} block args, got {blockCount}")
        return null if (blockCount != nLit)
        var bodyStmts : array<ExpressionPtr>
        bodyStmts |> reserve(blockCount * 2 + 2)
        bodyStmts |> push <| qmacro_expr(${
            columns_open($v(nLit))
        })
        for (i in range(blockCount)) {
            let blkArg = call.arguments[i + 1]
            macro_verify(blkArg is ExprMakeBlock, prog, blkArg.at,
                         "columns: column argument {i} must be a block literal")
            return null if (!(blkArg is ExprMakeBlock))
            bodyStmts |> push <| qmacro_expr(${
                invoke($e(blkArg))
            })
            if (i < blockCount - 1) {
                bodyStmts |> push <| qmacro_expr(${
                    next_col()
                })
            }
        }
        bodyStmts |> push <| qmacro_expr(${
            columns_close()
        })
        return qmacro_block() {
            $b(bodyStmts)
        }
    }
}
