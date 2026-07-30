options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: with_clip_rect — Push/PopClipRect scope wrapper.
// SHOWS:   `with_clip_rect(min, max, intersect_with_current) { ... }`
//          restricts every widget and drawlist primitive submitted inside
//          the block to the given screen-space rect. Typical use: rendering
//          DrawList shapes that should not bleed outside a widget box.
//          Compare the left circle (no clip — spills past the band) vs the
//          right circle (inside with_clip_rect — cleanly cut at the band
//          edge).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/clip_rect.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/clip_rect.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/clip_rect.das
// =============================================================================

[export]
def init() {
    harness_init("with_clip_rect — clip scope", 520, 320)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(480.0, 280.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "with_clip_rect", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Left circle: no clip. Right circle: clipped to its band."))

        let origin = GetCursorScreenPos()
        let blue = rgba(70u, 130u, 240u, 240u)
        let band = rgba(60u, 60u, 90u, 200u)

        with_window_drawlist() $(var dl) {
            // Left half — band painted, circle NOT clipped, spills out.
            dl |> add_rect_filled(
                float2(origin.x + 16.0f, origin.y + 24.0f),
                float2(origin.x + 200.0f, origin.y + 64.0f),
                band)
            dl |> add_circle_filled(
                float2(origin.x + 200.0f, origin.y + 44.0f),
                30.0f, blue, 0)
        }

        // Right half — same circle, clipped to its band by with_clip_rect.
        let r_x0 = origin.x + 240.0f
        let r_y0 = origin.y + 24.0f
        let r_x1 = origin.x + 424.0f
        let r_y1 = origin.y + 64.0f
        with_clip_rect(float2(r_x0, r_y0), float2(r_x1, r_y1), true) {
            with_window_drawlist() $(var dl2) {
                dl2 |> add_rect_filled(float2(r_x0, r_y0), float2(r_x1, r_y1), band)
                dl2 |> add_circle_filled(float2(r_x1, r_y0 + 20.0f),
                                         30.0f, blue, 0)
            }
            // GUARD dummy inside the clip block — proves widget registration
            // works normally inside with_clip_rect (no scope-leak).
            dummy(GUARD, (size = float2(r_x1 - r_x0, r_y1 - r_y0)))
        }

        dummy(BAND_SLOT, (size = float2(460.0f, 100.0f)))
        text(NOTE, (text = "(The right circle's overflow is clipped at the band edge — the left one spills past.)"))
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
