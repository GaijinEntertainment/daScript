options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: drawlist — window/foreground/background drawlist rail.
//          INSIDE window(DRAW_WIN, ...): `with_window_drawlist` exercises
//          the full shape set — 8 originals (line / rect / rect_filled /
//          circle / circle_filled / triangle / triangle_filled / text)
//          plus 8 v2 extras (rect_filled_multi_color / ellipse /
//          ellipse_filled / ngon / ngon_filled / polyline / bezier_cubic /
//          bezier_quadratic) nested under DRAW_WIN/.
//          OUTSIDE the window block: `with_foreground_drawlist` +
//          `with_background_drawlist` each call `add_rect_filled` at the
//          viewport level — those path keys have no container prefix.
// SHOWS:   3 scope wrappers + 16 shape primitives (originals + v2 extras);
//          nested-vs-unnested telemetry.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/drawlist.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/drawlist.das -- --headless --headless-frames=60
// =============================================================================

[export]
def init() {
    harness_init("drawlist — rail tour", 520, 540)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(480.0, 480.0), ImGuiCond.Always)
    window(DRAW_WIN, (text = "drawlist", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(DRAW_INTRO, (text = "Drawlist primitives below — bboxes are mouse-cards targetable by path."))
        separator()

        let origin = GetCursorScreenPos()
        let green  = rgba(60u, 220u, 100u, 255u)
        let blue   = rgba(70u, 130u, 240u, 255u)
        let yellow = rgba(240u, 220u,  60u, 255u)
        let gray   = rgba(200u, 200u, 210u, 255u)

        with_window_drawlist() $(var dl) {
            // Filled rect across the canvas band.
            dl |> add_rect_filled(float2(origin.x +   8.0f, origin.y +  8.0f),
                                  float2(origin.x + 460.0f, origin.y + 60.0f),
                                  rgba(35u, 35u, 50u, 200u), 6.0f)
            // Stroked rect inset.
            dl |> add_rect(float2(origin.x +  16.0f, origin.y + 16.0f),
                           float2(origin.x + 452.0f, origin.y + 52.0f),
                           gray, 4.0f, ImDrawFlags.None, 1.5f)
            // Diagonal line.
            dl |> add_line(float2(origin.x +  16.0f, origin.y + 16.0f),
                           float2(origin.x + 452.0f, origin.y + 52.0f),
                           yellow, 2.0f)
            // Circles.
            dl |> add_circle_filled(float2(origin.x +  60.0f, origin.y + 130.0f), 28.0f, blue, 0)
            dl |> add_circle(float2(origin.x + 130.0f, origin.y + 130.0f), 28.0f, blue, 0, 2.0f)
            // Triangles.
            dl |> add_triangle_filled(float2(origin.x + 220.0f, origin.y + 102.0f),
                                      float2(origin.x + 192.0f, origin.y + 158.0f),
                                      float2(origin.x + 248.0f, origin.y + 158.0f), green)
            dl |> add_triangle(float2(origin.x + 310.0f, origin.y + 102.0f),
                               float2(origin.x + 282.0f, origin.y + 158.0f),
                               float2(origin.x + 338.0f, origin.y + 158.0f), green, 2.0f)
            // Text label.
            dl |> add_text(float2(origin.x +  16.0f, origin.y + 180.0f), gray,
                           "add_text — window drawlist")
            // Multi-color gradient rect.
            dl |> add_rect_filled_multi_color(
                float2(origin.x +  16.0f, origin.y + 200.0f),
                float2(origin.x + 220.0f, origin.y + 218.0f),
                rgba(255u, 0u, 0u, 220u),    // upper-left
                rgba(0u, 255u, 0u, 220u),    // upper-right
                rgba(0u, 0u, 255u, 220u),    // bottom-right
                rgba(255u, 255u, 0u, 220u))  // bottom-left
            // Stroked + filled ellipse.
            dl |> add_ellipse(float2(origin.x + 270.0f, origin.y + 209.0f),
                              float2(28.0f, 14.0f), yellow, 0.0f, 0, 1.5f)
            dl |> add_ellipse_filled(float2(origin.x + 340.0f, origin.y + 209.0f),
                                     float2(28.0f, 14.0f), blue, 0.0f, 0)
            // Stroked + filled N-gon (hexagon).
            dl |> add_ngon(float2(origin.x +  40.0f, origin.y + 250.0f),
                           22.0f, green, 6, 2.0f)
            dl |> add_ngon_filled(float2(origin.x + 100.0f, origin.y + 250.0f),
                                  22.0f, green, 6)
            // Cubic + quadratic bezier curves.
            dl |> add_bezier_cubic(
                float2(origin.x + 150.0f, origin.y + 270.0f),
                float2(origin.x + 200.0f, origin.y + 220.0f),
                float2(origin.x + 260.0f, origin.y + 280.0f),
                float2(origin.x + 310.0f, origin.y + 230.0f),
                yellow, 2.0f, 0)
            dl |> add_bezier_quadratic(
                float2(origin.x + 320.0f, origin.y + 270.0f),
                float2(origin.x + 380.0f, origin.y + 230.0f),
                float2(origin.x + 440.0f, origin.y + 270.0f),
                blue, 2.0f, 0)
            // Polyline (open M-shape).
            var poly <- [
                float2(origin.x +  20.0f, origin.y + 300.0f),
                float2(origin.x +  40.0f, origin.y + 280.0f),
                float2(origin.x +  60.0f, origin.y + 295.0f),
                float2(origin.x +  80.0f, origin.y + 280.0f),
                float2(origin.x + 100.0f, origin.y + 300.0f)
            ]
            dl |> add_polyline(poly, gray, ImDrawFlags.None, 1.5f)
            delete poly
        }

        // Reserve layout space for the band painted above so subsequent
        // ImGui widgets land below.
        dummy(DRAW_BAND_SLOT, (size = float2(460.0f, 340.0f)))

        text(DRAW_FG_LABEL, (text = "foreground swatch (overlays window):"))
    }
    // Outside window() — primitives here have no DRAW_WIN/ prefix. Foreground /
    // background drawlists are viewport-level, reachable anywhere in the frame;
    // the test asserts both nested and unnested primitives reach the registry.
    let vp_pos = GetMainViewport().Pos
    let vp_size = GetMainViewport().Size
    let red    = rgba(220u,  60u,  60u, 255u)
    let bg_tint = rgba(20u, 30u, 50u, 120u)
    with_foreground_drawlist() $(var fdl) {
        fdl |> add_rect_filled(float2(vp_pos.x + 12.0f, vp_pos.y + 12.0f),
                               float2(vp_pos.x + 80.0f, vp_pos.y + 22.0f),
                               red)
    }
    with_background_drawlist() $(var bdl) {
        bdl |> add_rect_filled(float2(vp_pos.x, vp_pos.y),
                               float2(vp_pos.x + vp_size.x, vp_pos.y + vp_size.y),
                               bg_tint)
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
