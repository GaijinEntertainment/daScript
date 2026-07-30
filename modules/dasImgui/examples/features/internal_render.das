options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: internal_render — exercises the imgui_internal.h Render* draw-helper
//          family (#1, bound via bind_imgui.das internal_allow_func). Every
//          helper writes straight into an ImDrawList with no ImGui state, so the
//          example lays them out on the window drawlist and lets you eyeball
//          that each one actually renders. This is the "binding is usable" gate
//          for family #1 — it calls the real bound API, not just compiles it.
// SHOWS:   RenderArrow (all four ImGuiDir), RenderBullet, RenderCheckMark,
//          RenderArrowPointingAt, RenderColorRectWithAlphaCheckerboard,
//          RenderFrame + RenderFrameBorder, RenderText.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_render.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_render.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_render.das
// =============================================================================

[export]
def init() {
    harness_init("internal_render - imgui_internal.h Render* family", 760, 470)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    let white = rgba(235u, 235u, 245u, 255u)
    let cyan  = rgba(80u, 200u, 230u, 255u)
    let amber = rgba(240u, 190u, 70u, 255u)
    let green = rgba(90u, 220u, 120u, 255u)

    SetNextWindowSize(ImVec2(720.0, 430.0), ImGuiCond.Always)
    window(RENDER_WIN, (text = "internal_render", closable = false,
                        flags = ImGuiWindowFlags.None)) {
        text("imgui_internal.h Render* helpers - each writes straight to a drawlist.")
        separator()

        let o = GetCursorScreenPos()
        with_window_drawlist() $(var dl) {
            // RenderArrow - one per ImGuiDir, left to right.
            RenderArrow(dl, ImVec2(o.x + 24.0, o.y + 28.0), white, ImGuiDir.Left, 1.0)
            RenderArrow(dl, ImVec2(o.x + 64.0, o.y + 28.0), white, ImGuiDir.Right, 1.0)
            RenderArrow(dl, ImVec2(o.x + 104.0, o.y + 28.0), white, ImGuiDir.Up, 1.0)
            RenderArrow(dl, ImVec2(o.x + 144.0, o.y + 28.0), white, ImGuiDir.Down, 1.0)
            // RenderBullet + RenderCheckMark.
            RenderBullet(dl, ImVec2(o.x + 204.0, o.y + 38.0), cyan)
            RenderCheckMark(dl, ImVec2(o.x + 232.0, o.y + 28.0), green, 13.0)
            // RenderArrowPointingAt - arrowhead fitted into a half-size box.
            RenderArrowPointingAt(dl, ImVec2(o.x + 304.0, o.y + 38.0),
                                  ImVec2(8.0, 10.0), ImGuiDir.Right, amber)
            // RenderColorRectWithAlphaCheckerboard - alpha-preview swatch.
            RenderColorRectWithAlphaCheckerboard(dl,
                ImVec2(o.x + 24.0, o.y + 70.0), ImVec2(o.x + 168.0, o.y + 150.0),
                rgba(120u, 80u, 200u, 120u), 8.0, ImVec2(0.0, 0.0), 4.0, ImDrawFlags.None)
        }

        // RenderFrame / RenderFrameBorder / RenderText draw into the current
        // window's drawlist implicitly (no ImDrawList argument).
        RenderFrame(ImVec2(o.x + 200.0, o.y + 70.0), ImVec2(o.x + 380.0, o.y + 150.0),
                    rgba(45u, 55u, 75u, 255u), true, 4.0)
        RenderFrameBorder(ImVec2(o.x + 200.0, o.y + 70.0), ImVec2(o.x + 380.0, o.y + 150.0), 4.0)
        RenderText(ImVec2(o.x + 214.0, o.y + 102.0), "RenderText + RenderFrame")

        dummy(SPACER, ImVec2(700.0, 168.0))
        separator()
        text("Every draw above is imgui_internal.h Render* family #1.")
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
