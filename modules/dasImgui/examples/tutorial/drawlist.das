options gen2
options _comment_hygiene = true

require imgui
require imgui_app
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_scope_builtin
require imgui/imgui_drawlist_builtin
require imgui/imgui_colors
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: drawlist — three scope wrappers (with_window_drawlist /
//           with_foreground_drawlist / with_background_drawlist) and the
//           full primitive set: 8 originals (add_line, add_rect,
//           add_rect_filled, add_circle, add_circle_filled, add_triangle,
//           add_triangle_filled, add_text) plus the v2 additions
//           (add_rect_filled_multi_color, add_ellipse, add_ellipse_filled,
//           add_ngon, add_ngon_filled, add_polyline, add_bezier_cubic,
//           add_bezier_quadratic). For the path-building, channel-splitting,
//           and clip-rect rails see the dedicated feature files
//           (examples/features/drawlist_path.das,
//           examples/features/drawlist_channels.das,
//           examples/features/clip_rect.das).
//
// The rail bridges to ImGui's three viewport-level draw lists with the same
// telemetry surface as `[widget]`: path-keyed entries land in `g_registry`
// so mouse-cards / playwright can target a specific call site by
// `<window>/<mod>:<line>:<col>`. No per-call state global is registered —
// primitives are decoration-only, last write wins on overlapping draws.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/drawlist.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/drawlist.das
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui drawlist tutorial", 1000, 800)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.2
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(30.0f, 30.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(700.0f, 700.0f), ImGuiCond.Always)
    window(DRAW_WIN, (text = "drawlist - rail tour", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(DRAW_INTRO, (text = "Three scope wrappers + shape primitives (8 originals + v2 extras)."))
        separator()

        let origin = GetCursorScreenPos()
        let red    = rgba(220u,  60u,  60u, 255u)
        let green  = rgba(60u, 220u, 100u, 255u)
        let blue   = rgba(70u, 130u, 240u, 255u)
        let yellow = rgba(240u, 220u,  60u, 255u)
        let gray   = rgba(200u, 200u, 210u, 255u)
        let bg_tint = rgba(20u, 30u, 50u, 120u)

        with_window_drawlist() $(var dl) {
            dl |> add_rect_filled(float2(origin.x +   8.0f, origin.y +  8.0f),
                                  float2(origin.x + 660.0f, origin.y + 80.0f),
                                  rgba(35u, 35u, 50u, 200u), 6.0f)
            dl |> add_rect(float2(origin.x +  16.0f, origin.y + 16.0f),
                           float2(origin.x + 652.0f, origin.y + 72.0f),
                           gray, 4.0f, ImDrawFlags.None, 1.5f)
            dl |> add_line(float2(origin.x +  16.0f, origin.y + 16.0f),
                           float2(origin.x + 652.0f, origin.y + 72.0f),
                           yellow, 2.0f)
            dl |> add_circle_filled(float2(origin.x +  60.0f, origin.y + 160.0f), 32.0f, blue, 0)
            dl |> add_circle(float2(origin.x + 150.0f, origin.y + 160.0f), 32.0f, blue, 0, 2.0f)
            dl |> add_triangle_filled(float2(origin.x + 260.0f, origin.y + 124.0f),
                                      float2(origin.x + 226.0f, origin.y + 196.0f),
                                      float2(origin.x + 294.0f, origin.y + 196.0f), green)
            dl |> add_triangle(float2(origin.x + 380.0f, origin.y + 124.0f),
                               float2(origin.x + 346.0f, origin.y + 196.0f),
                               float2(origin.x + 414.0f, origin.y + 196.0f), green, 2.0f)
            dl |> add_text(float2(origin.x +  16.0f, origin.y + 220.0f), gray,
                           "add_text - rendered onto the window drawlist")

            // ----- v2 additions: shape extras -----
            // Multi-color gradient rect.
            dl |> add_rect_filled_multi_color(
                float2(origin.x +  16.0f, origin.y + 260.0f),
                float2(origin.x + 240.0f, origin.y + 290.0f),
                rgba(255u, 0u, 0u, 220u), rgba(0u, 255u, 0u, 220u),
                rgba(0u, 0u, 255u, 220u), rgba(255u, 255u, 0u, 220u))
            // Stroked + filled ellipse.
            dl |> add_ellipse(float2(origin.x + 300.0f, origin.y + 275.0f),
                              float2(36.0f, 16.0f), yellow, 0.0f, 0, 1.5f)
            dl |> add_ellipse_filled(float2(origin.x + 400.0f, origin.y + 275.0f),
                                     float2(36.0f, 16.0f), blue, 0.0f, 0)
            // Stroked + filled N-gon (hexagon).
            dl |> add_ngon(float2(origin.x + 500.0f, origin.y + 275.0f),
                           22.0f, green, 6, 2.0f)
            dl |> add_ngon_filled(float2(origin.x + 580.0f, origin.y + 275.0f),
                                  22.0f, green, 6)
            // Cubic + quadratic bezier curves.
            dl |> add_bezier_cubic(
                float2(origin.x +  16.0f, origin.y + 330.0f),
                float2(origin.x + 110.0f, origin.y + 290.0f),
                float2(origin.x + 200.0f, origin.y + 360.0f),
                float2(origin.x + 300.0f, origin.y + 310.0f),
                yellow, 2.0f, 0)
            dl |> add_bezier_quadratic(
                float2(origin.x + 320.0f, origin.y + 330.0f),
                float2(origin.x + 430.0f, origin.y + 290.0f),
                float2(origin.x + 540.0f, origin.y + 330.0f),
                blue, 2.0f, 0)
            // Polyline (open M-shape) below.
            var poly <- [
                float2(origin.x +  16.0f, origin.y + 380.0f),
                float2(origin.x +  60.0f, origin.y + 350.0f),
                float2(origin.x + 100.0f, origin.y + 370.0f),
                float2(origin.x + 140.0f, origin.y + 350.0f),
                float2(origin.x + 180.0f, origin.y + 380.0f)
            ]
            dl |> add_polyline(poly, gray, ImDrawFlags.None, 1.5f)
            delete poly
        }

        dummy(DRAW_BAND_SLOT, (size = float2(660.0f, 400.0f)))

        text(DRAW_FG_LABEL, (text = "Foreground swatch (overlays everything):"))

        let vp_pos = GetMainViewport().Pos
        let vp_size = GetMainViewport().Size
        with_foreground_drawlist() $(var fdl) {
            fdl |> add_rect_filled(float2(vp_pos.x + 12.0f, vp_pos.y + 12.0f),
                                   float2(vp_pos.x + 100.0f, vp_pos.y + 26.0f), red)
            fdl |> add_text(float2(vp_pos.x + 20.0f, vp_pos.y + 12.0f),
                            rgba(255u, 255u, 255u, 255u), "FG")
        }

        with_background_drawlist() $(var bdl) {
            bdl |> add_rect_filled(float2(vp_pos.x, vp_pos.y),
                                   float2(vp_pos.x + vp_size.x, vp_pos.y + vp_size.y),
                                   bg_tint)
        }
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    live_imgui_render()

    live_end_frame()
}

[export]
def shutdown() {
    live_imgui_shutdown()
    live_destroy_window()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
