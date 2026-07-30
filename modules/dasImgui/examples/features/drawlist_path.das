options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require math

// =============================================================================
// FEATURE: drawlist path family — path_clear / path_line_to / path_arc_to /
//          path_bezier_quadratic_curve_to / path_fill_convex /
//          path_fill_concave / path_stroke.
// SHOWS:   Build a complex shape on the drawlist's internal path stack, then
//          render it in one go via the path_stroke / path_fill_convex /
//          path_fill_concave terminator. Cheaper than a polyline for
//          dynamic shapes because intermediate vertex counts can vary and
//          ImGui only allocates one draw command.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/drawlist_path.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/drawlist_path.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/drawlist_path.das
// =============================================================================

[export]
def init() {
    harness_init("drawlist path family", 520, 420)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(480.0, 380.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "drawlist_path", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Three path-built shapes: stroked rounded rect, filled convex pentagon, filled concave arrow."))

        let origin = GetCursorScreenPos()
        let blue   = rgba(70u, 130u, 240u, 255u)
        let green  = rgba(60u, 220u, 100u, 230u)
        let orange = rgba(240u, 150u,  60u, 230u)

        with_window_drawlist() $(var dl) {
            // 1) Stroked rounded-rect built from 4 arcs.
            let r = 16.0f
            let x0 = origin.x +  16.0f
            let y0 = origin.y +  16.0f
            let x1 = origin.x + 180.0f
            let y1 = origin.y +  72.0f
            dl |> path_clear()
            dl |> path_arc_to(float2(x0 + r, y0 + r), r, PI, 1.5f * PI, 16)
            dl |> path_arc_to(float2(x1 - r, y0 + r), r, 1.5f * PI, 2.0f * PI, 16)
            dl |> path_arc_to(float2(x1 - r, y1 - r), r, 0.0f, 0.5f * PI, 16)
            dl |> path_arc_to(float2(x0 + r, y1 - r), r, 0.5f * PI, PI, 16)
            dl |> path_stroke(blue, ImDrawFlags.Closed, 2.0f)

            // 2) Filled convex pentagon built from 5 line-tos.
            let cx = origin.x + 260.0f
            let cy = origin.y +  48.0f
            dl |> path_clear()
            for (i in range(5)) {
                let theta = float(i) * (2.0f * PI / 5.0f) - 0.5f * PI
                dl |> path_line_to(float2(cx + cos(theta) * 32.0f,
                                          cy + sin(theta) * 32.0f))
            }
            dl |> path_fill_convex(green)

            // 3) Filled concave arrow built from line-tos + a quadratic curve.
            let ax = origin.x +  16.0f
            let ay = origin.y + 120.0f
            dl |> path_clear()
            dl |> path_line_to(float2(ax +   0.0f, ay +  16.0f))
            dl |> path_line_to(float2(ax +  60.0f, ay +  16.0f))
            dl |> path_line_to(float2(ax +  60.0f, ay +   0.0f))
            dl |> path_bezier_quadratic_curve_to(
                float2(ax + 120.0f, ay +  16.0f),
                float2(ax +  60.0f, ay +  32.0f), 8)
            dl |> path_line_to(float2(ax +  60.0f, ay +  16.0f))
            dl |> path_fill_concave(orange)
        }

        // Reserve layout space.
        dummy(PATH_SLOT, (size = float2(460.0f, 240.0f)))
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
