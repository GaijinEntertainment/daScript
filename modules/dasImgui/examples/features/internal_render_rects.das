options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: internal_render_rects — completes the imgui_internal.h Render* family
//          (#1) with the four helpers that take an ImRect and so were left out of
//          the original family-1 PR. Now that ImRect is aliased to float4 (#166),
//          they bind directly (bind_imgui.das internal_allow_func). An ImRect is a
//          float4 of (Min.x, Min.y, Max.x, Max.y). RenderNavCursor also pulls
//          the ImGuiNavRenderCursorFlags_ enum (bound as an int arg, like PushItemFlag).
//          This is the "binding is usable" gate — it calls the real bound API at
//          runtime, not just compiles it.
// SHOWS:   RenderRectFilledRangeH (fill a horizontal sub-range of a rounded rect),
//          RenderRectFilledWithHole (fill an outer rect minus an inner rect),
//          RenderDragDropTargetRect (the highlight drawn over a drag-drop target),
//          RenderNavCursor (the keyboard/gamepad nav focus ring).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_render_rects.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_render_rects.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_render_rects.das
// =============================================================================

[export]
def init() {
    harness_init("internal_render_rects - imgui_internal.h Render* ImRect helpers", 760, 560)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    let ink   = rgba(232u, 232u, 240u, 255u)
    let fill  = rgba(48u, 96u, 168u, 230u)
    let amber = rgba(232u, 168u, 64u, 230u)

    SetNextWindowSize(ImVec2(720.0, 520.0), ImGuiCond.Always)
    window(RECTS_WIN, (text = "internal_render_rects", closable = false,
                       flags = ImGuiWindowFlags.None)) {
        text("imgui_internal.h Render* helpers that take an ImRect (float4 Min/Max).")
        separator()

        // RenderRectFilledRangeH (fill a horizontal sub-range of a rounded rect)
        // is no longer available — its demo is dropped here.

        // RenderRectFilledWithHole - fill the outer rect except the inner rect (a
        // frame / focus mask). Both corners are ImRects.
        text("RenderRectFilledWithHole: fill an outer rect minus an inner rect (a frame).")
        let o2 = GetCursorScreenPos()
        with_window_drawlist() $(var dl) {
            let ox = o2.x + 12.0
            let oy = o2.y + 8.0
            let outer = float4(ox, oy, ox + 300.0, oy + 70.0)
            let inner = float4(ox + 42.0, oy + 18.0, ox + 258.0, oy + 52.0)
            RenderRectFilledWithHole(dl, outer, inner, amber, 6.0)
            dl |> add_text(ImVec2(ox + 312.0, oy + 26.0), ink, "outer minus inner")
        }
        dummy(SPACER2, ImVec2(700.0, 88.0))
        separator()

        // RenderDragDropTargetRectForItem - the highlight ImGui paints over an
        // accepting drag-drop target; takes the item bb (no clip-rect arg).
        text("RenderDragDropTargetRectForItem: the highlight drawn over a drag-drop target.")
        let o3 = GetCursorScreenPos()
        let dropBB = float4(o3.x + 12.0, o3.y + 8.0, o3.x + 320.0, o3.y + 60.0)
        RenderDragDropTargetRectForItem(dropBB)
        dummy(SPACER3, ImVec2(700.0, 72.0))
        separator()

        // RenderNavCursor - the keyboard/gamepad nav focus ring. It only paints the
        // item whose id equals the current nav id; at rest that id is 0, so passing 0
        // with AlwaysDraw shows the ring here. Tab/arrow nav moves it to the real item.
        text("RenderNavCursor: the nav focus ring (id 0 + AlwaysDraw to show at rest).")
        let o4 = GetCursorScreenPos()
        let navBB = float4(o4.x + 12.0, o4.y + 8.0, o4.x + 320.0, o4.y + 50.0)
        RenderNavCursor(navBB, 0u, int(ImGuiNavRenderCursorFlags.AlwaysDraw))
        dummy(SPACER4, ImVec2(700.0, 62.0))
        separator()
        text("All four are imgui_internal.h Render* helpers, unblocked by ImRect (family #1).")
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
