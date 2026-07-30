options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: containers_window — Phase 0b.3 window/child/group containers.
// SHOWS:   `[container]` macro hierarchical path keys; window/child/group
//          composition; closable window with imgui_open / imgui_close.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          # leaves nested under MAIN_WIN appear as MAIN_WIN/<ident>
//          curl -X POST -d '{"name":"imgui_force_set",
//                            "args":{"target":"MAIN_WIN/SPEED","value":7.5}}' \
//                       localhost:9090/command
//          curl -X POST -d '{"name":"imgui_open",
//                            "args":{"target":"DETAIL_WIN"}}' \
//                       localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/containers_window.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/containers_window.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/containers_window.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 0b.3 — containers (window family)", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    // Leaf widgets nested inside containers get hierarchical registry keys:
    // "MAIN_WIN/SAVE" / "MAIN_WIN/SPEED" / "MAIN_WIN/LEFT_PANEL/WIRE" / …
    SPEED.bounds = (0.0f, 10.0f)
    POWER.bounds = (1, 100)

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(420.0f, 480.0f), ImGuiCond.FirstUseEver)
    window(MAIN_WIN, (text = "Main", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("window/child/group composition")
        separator()

        button(SAVE, (text = "Save"))
        text("save clicks: {SAVE.click_count}")
        slider_float(SPEED, (text = "speed"))
        text("speed = {SPEED.value}")

        spacing()
        text("layout: child + group")
        separator()

        child(LEFT_PANEL, (text = "left", size = float2(180.0f, 120.0f),
                           child_flags = ImGuiChildFlags.Borders,
                           window_flags = ImGuiWindowFlags.None)) {
            text("child window")
            checkbox(WIRE, (text = "Wireframe"))
            text("wireframe = {WIRE.value}")
        }
        same_line((spacing = 8.0f))
        group(RIGHT_GROUP) {
            text("group (no chrome)")
            slider_int(POWER, (text = "power"))
            small_button(RESET_BTN, (text = "Reset"))
            text("reset clicks: {RESET_BTN.click_count}")
        }
    }

    // A second, closable window — driveable via imgui_open / imgui_close.
    // Initial state.open is false (auto-emit zero); enqueue open via
    //   curl -X POST -d '{"name":"imgui_open","args":{"target":"DETAIL_WIN"}}' localhost:9090/command
    SetNextWindowPos(ImVec2(460.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(300.0f, 240.0f), ImGuiCond.FirstUseEver)
    window(DETAIL_WIN, (text = "Detail (closable)", closable = true,
                        flags = ImGuiWindowFlags.None)) {
        text("close me with the X, or via imgui_close")
        separator()
        text("size: {DETAIL_WIN.size.x}, {DETAIL_WIN.size.y}")
        button(PING, (text = "Ping"))
        text("ping clicks: {PING.click_count}")
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    //! Standalone entrypoint. daslang-live drives init/update/shutdown
    //! directly and ignores main(); plain `daslang.exe <file>` runs this loop.
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
