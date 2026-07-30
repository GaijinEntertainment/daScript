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
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: recording — the meta tutorial.
//
// Every previous tutorial's APNG was produced by the same workflow:
//
//   daslang  runs a DRIVER script that:
//     - uses with_recording_app to SPAWN this SUBJECT app as daslang-live
//     - the helper auto-toggles imgui_mouse_trail + imgui_cursor_sprite
//     - the helper auto-bracketts record_start / record_stop
//     - the body posts imgui_narrate + cursor moves + clicks / drags
//
// The DRIVER is the artifact that teaches recording — the subject is
// just whatever app you want to record. This file is intentionally tiny
// (one slider, one button) so the recording recipe is obvious: every
// frame of the APNG corresponds to one specific driver-body call.
//
// The driver script for this tutorial is:
//   tests/integration/record_recording.das
//
// The helper passes --imgui-content-scale=1.0 + --no-hdpi-framebuffer
// to the spawned daslang-live so APNG capture stays at logical 1x.
// When you run this tutorial DIRECTLY (no driver), no flags are passed,
// so retina users get native HDPI scaling.
//
// Read the driver alongside the walkthrough in the RST companion — the
// .das + driver + APNG triple is the tutorial.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/recording.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/recording.das
//
// DRIVE: see tests/integration/record_recording.das
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui recording tutorial", 940, 560)
    live_imgui_init(live_window)
    DisableIniPersistence()
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.5
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(30.0f, 30.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(520.0f, 280.0f), ImGuiCond.FirstUseEver)
    window(REC_WIN, (text = "subject", closable = false,
                     flags = ImGuiWindowFlags.None)) {
        text("Tiny subject - driver narrates what's happening.")
        slider_float(VOLUME, (text = "Volume"))
        if (button(SAVE_BTN, (text = "Save"))) {}
        text("SAVE_BTN.click_count = {SAVE_BTN.click_count}")
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
