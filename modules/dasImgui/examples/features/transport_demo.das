options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: transport_demo — Phase 2.6 lambda transport.
// SHOWS:   widgets/imgui_transport.das ships a `Transport` lambda type +
//          `live_api_transport(url)` constructor + `await_imgui` /
//          `send_with_await` polling helpers. Tests build one transport per
//          fixture and route every command through it; users can substitute
//          mocks/in-process buses without touching call sites.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals."MAIN_WIN/DEMO_BTN"'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/transport_demo.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/transport_demo.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/transport_demo.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 2.6 — transport_demo", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 200.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "transport_demo — phase 2.6", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Transport lambda routes commands; await_imgui polls quiescence.")
        if (button(DEMO_BTN, (text = "Click me"))) {
            print("DEMO_BTN clicked, count={DEMO_BTN.click_count}\n")
        }
        text("clicks: {DEMO_BTN.click_count}")
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
