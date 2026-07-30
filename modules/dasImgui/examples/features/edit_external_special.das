options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require daslib/safe_addr
require math

// =============================================================================
// FEATURE: edit_external_special — slider_angle on external float? pointer.
// SHOWS:   edit_slider_angle(safe_addr(rad_value), (id=…, text=…,
//                              v_degrees_min=-180, v_degrees_max=180,
//                              format="%.0f°")). The pointee is RADIANS;
//          the slider displays + edits DEGREES via ImGui's internal
//          conversion. Useful for rotation editors backed by quaternion /
//          matrix math storage on the daslang side.
// DRIVE:   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/ANGLE","value":1.5707}}' \
//              localhost:9090/command
//          # → sets g_angle_rad to ~π/2 (90° on the slider).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/edit_external_special.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/edit_external_special.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/edit_external_special.das
// =============================================================================

var private g_angle_rad : float = 0.7854f  // π/4 → ~45° initial

[export]
def init() {
    harness_init("edit_external_special — boost v2 §3.7", 720, 320)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 260.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "edit_slider_angle", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Slider reads/writes radians; UI displays degrees."))
        edit_slider_angle(safe_addr(g_angle_rad), (id = "ANGLE",
                                                   text = "rotation",
                                                   v_degrees_min = -180.0f,
                                                   v_degrees_max = 180.0f,
                                                   format = "%.0f deg"))
        text(STATUS, (text = "g_angle_rad = {g_angle_rad} (≈ {int(g_angle_rad * 180.0f / PI)}°)"))
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
