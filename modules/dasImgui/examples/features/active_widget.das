options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: active_widget — Phase 2.1 foundation gate.
// SHOWS:   GetActiveID() binding (cherry-picked from imgui_internal.h) +
//          g_id_to_path / g_id_to_bbox reverse maps populated by widget_finalize +
//          io_jv() surfacing active_id/active_widget when something is active +
//          Result<T,string> wire format on imgui_click responses + with_await
//          per-command modifier merging (await/timeout_sec/fire_and_forget).
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.io'
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"MAIN_WIN/DEMO_BTN"}}' \
//              localhost:9090/command          # → {"ok": true, "value": "MAIN_WIN/DEMO_BTN", ...}
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"MAIN_WIN/DEMO_BTN","await":{"fire_and_forget":true}}}' \
//              localhost:9090/command          # → bare {"ok":true,"value":"..."} (no timeout_sec)
// STANDALONE: daslang.exe modules/dasImgui/examples/features/active_widget.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/active_widget.das -- --headless --headless-frames=600
// LIVE:       daslang-live modules/dasImgui/examples/features/active_widget.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 2.1 — active_widget foundation", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 280.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "active_widget — phase 2.1", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Foundation gate: GetActiveID + reverse maps + Result wire.")
        text("Drag the slider to make snapshot.io.active_widget resolve.")

        // Single button + single slider — minimal surface; the test reads
        // snapshot.io.active_widget, snapshot.io.mouse_buttons, and the
        // imgui_click response wire shape.
        if (button(DEMO_BTN, (text = "Click me"))) {
            print("DEMO_BTN clicked, count={DEMO_BTN.click_count}\n")
        }
        text("clicks: {DEMO_BTN.click_count}")

        slider_float(DEMO_SLIDER, (text = "Drag me"))
        text("slider value = {DEMO_SLIDER.value}")
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
