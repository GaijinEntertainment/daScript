options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: internal_splitter_scrollbar — the imgui_internal.h axis-aligned
//          interactive layout primitives. Cherry-picked into the v2 binding:
//          Scrollbar(axis) raw (bind_imgui.das internal_allow_func); ScrollbarEx
//          (ImS64& scroll out-param) and SplitterBehavior (float& size1/size2
//          out-params) as manual C++ wrappers (dasIMGUI.main.cpp), so their in/out
//          numeric state comes back through references. New internal enum ImGuiAxis
//          (None / X / Y). This is the "binding is usable" gate: it drives the real
//          bound API at runtime, not just compiles it.
// SHOWS:   a draggable vertical SPLITTER resizing two panes (drag the bar — the two
//          widths update live via float& out-params); a custom vertical SCROLLBAR
//          (ScrollbarEx) driving a scrolling list of rows through an ImS64& position;
//          and Scrollbar(axis) drawing a child window's own scrollbar.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_splitter_scrollbar.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_splitter_scrollbar.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_splitter_scrollbar.das
// =============================================================================

var g_left = 240.0
var g_right = 300.0
var g_scroll = int64(0)

[export]
def init() {
    harness_init("internal_splitter_scrollbar - splitter + scrollbar behaviors", 820, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    let ink = rgba(235u, 240u, 250u, 255u)
    let pane_l = rgba(55u, 85u, 130u, 255u)
    let pane_r = rgba(70u, 110u, 70u, 255u)
    let bar_col = rgba(150u, 160u, 180u, 255u)

    SetNextWindowSize(ImVec2(780.0, 560.0), ImGuiCond.Always)
    window(SS_WIN, (text = "internal_splitter_scrollbar", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        // ----- SplitterBehavior: drag the bar to resize two panes -----
        text("SplitterBehavior(bb, id, axis, size1, size2, min1, min2): drag the bar.")
        let pane_h = 150.0
        let bar_w = 8.0
        let o = GetCursorScreenPos()
        let bar_x = o.x + g_left
        let split_bb = float4(bar_x, o.y, bar_x + bar_w, o.y + pane_h)
        // float& size1/size2 -> g_left / g_right update in place on drag.
        let dragging = SplitterBehavior(split_bb, GetID("vsplitter"), ImGuiAxis.X, g_left, g_right, 80.0, 80.0)
        with_window_drawlist() $(var dl) {
            dl |> add_rect_filled(ImVec2(o.x, o.y), ImVec2(o.x + g_left, o.y + pane_h), pane_l, 4.0, ImDrawFlags.None)
            dl |> add_rect_filled(ImVec2(bar_x, o.y), ImVec2(bar_x + bar_w, o.y + pane_h), bar_col, 0.0, ImDrawFlags.None)
            dl |> add_rect_filled(ImVec2(bar_x + bar_w, o.y), ImVec2(bar_x + bar_w + g_right, o.y + pane_h), pane_r, 4.0, ImDrawFlags.None)
            dl |> add_text(ImVec2(o.x + 12.0, o.y + 10.0), ink, "left pane")
            dl |> add_text(ImVec2(bar_x + bar_w + 12.0, o.y + 10.0), ink, "right pane")
        }
        dummy(SP1, ImVec2(g_left + bar_w + g_right, pane_h + 6.0))
        text("  left = {int(g_left)}px   right = {int(g_right)}px   (dragging = {dragging})")
        separator()

        // ----- ScrollbarEx: a custom scrollbar driving a scrolling row list -----
        text("ScrollbarEx(bb, id, axis, scroll, avail, contents, flags): drag to scroll.")
        let view_h = 200.0
        let row_h = 30.0
        let n_rows = 40
        let content_h = float(n_rows) * row_h
        let v = GetCursorScreenPos()
        let list_w = 560.0
        let sb_w = 16.0
        let view_top = v.y
        let view_bot = v.y + view_h
        let sb_bb = float4(v.x + list_w + 6.0, view_top, v.x + list_w + 6.0 + sb_w, view_bot)
        // ImS64& scroll -> g_scroll updates as the grab is dragged.
        let scrolled = ScrollbarEx(sb_bb, GetID("custom_scrollbar"), ImGuiAxis.Y, g_scroll,
                                   int64(view_h), int64(content_h), ImDrawFlags.None)
        with_window_drawlist() $(var dl) {
            dl |> add_rect_filled(ImVec2(v.x, view_top), ImVec2(v.x + list_w, view_bot),
                                  rgba(35u, 40u, 55u, 255u), 0.0, ImDrawFlags.None)
            for (i in range(n_rows)) {
                let ry = view_top + float(i) * row_h - float(g_scroll)
                continue if (ry + row_h < view_top || ry > view_bot)
                let band = (i % 2 == 0) ? rgba(50u, 58u, 78u, 255u) : rgba(42u, 49u, 66u, 255u)
                dl |> add_rect_filled(ImVec2(v.x + 2.0, ry + 2.0), ImVec2(v.x + list_w - 2.0, ry + row_h - 2.0),
                                      band, 0.0, ImDrawFlags.None)
                dl |> add_text(ImVec2(v.x + 12.0, ry + 7.0), ink, "row {i}")
            }
        }
        dummy(SP2, ImVec2(list_w + sb_w + 12.0, view_h + 6.0))
        text("  scroll = {int(g_scroll)} / {int(content_h - view_h)}   (held = {scrolled})")
        separator()

        // ----- Scrollbar(axis): a child window's own scrollbar (raw binding) -----
        // NoScrollbar suppresses the child's auto scrollbar so the manual
        // Scrollbar(ImGuiAxis.Y) below is the only one drawn for the (overflowing) child.
        text("Scrollbar(axis): the raw current-window scrollbar, drawn in this child:")
        child(SS_CHILD, (text = "scroll_child", size = float2(560.0f, 96.0f),
                         child_flags = ImGuiChildFlags.Borders,
                         window_flags = ImGuiWindowFlags.NoScrollbar)) {
            text("a child whose content is taller than its frame:")
            dummy(CHILD_FILL, ImVec2(1.0, 240.0))
            text("...bottom of the child content.")
            Scrollbar(ImGuiAxis.Y)
        }
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
