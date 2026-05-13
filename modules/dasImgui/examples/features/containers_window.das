options gen2

require imgui
require imgui_app
require glfw/glfw_boost
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live/opengl_live
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin

// =============================================================================
// FEATURE: containers_window — Phase 0b.3 window/child/group containers.
// SHOWS:   `[container]` macro hierarchical path keys; window/child/group
//          composition; closable window with imgui_open / imgui_close.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          # leaves nested under MAIN_WIN appear as MAIN_WIN/<ident>
//          curl -X POST -d '{"name":"imgui_set",
//                            "args":{"target":"MAIN_WIN/SPEED","value":7.5}}' \
//                       localhost:9090/command
//          curl -X POST -d '{"name":"imgui_open",
//                            "args":{"target":"DETAIL_WIN"}}' \
//                       localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/containers_window.das
// LIVE:       daslang-live modules/dasImgui/examples/features/containers_window.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 0b.3 — containers (window family)", 800, 600)
    live_imgui_init(live_window)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 1.5
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplOpenGL3_NewFrame()
    ImGui_ImplGlfw_NewFrame()
    NewFrame()

    // Leaf widgets nested inside containers get hierarchical registry keys:
    // "MAIN_WIN/SAVE" / "MAIN_WIN/SPEED" / "MAIN_WIN/LEFT_PANEL/WIRE" / …
    SPEED.bounds = (0.0f, 10.0f)
    POWER.bounds = (1, 100)

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(420.0f, 480.0f), ImGuiCond.FirstUseEver)
    window(MAIN_WIN, (text = "Main", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        Text("window/child/group composition")
        Separator()

        button(SAVE, (text = "Save"))
        Text("save clicks: {SAVE.click_count}")
        slider_float(SPEED, (text = "speed"))
        Text("speed = {SPEED.value}")

        Spacing()
        Text("layout: child + group")
        Separator()

        child(LEFT_PANEL, (text = "left", size = float2(180.0f, 120.0f),
                           child_flags = ImGuiChildFlags.Border,
                           window_flags = ImGuiWindowFlags.None)) {
            Text("child window")
            checkbox(WIRE, (text = "Wireframe"))
            Text("wireframe = {WIRE.value}")
        }
        SameLine(0.0f, 8.0f)
        group(RIGHT_GROUP) {
            Text("group (no chrome)")
            slider_int(POWER, (text = "power"))
            small_button(RESET_BTN, (text = "Reset"))
            Text("reset clicks: {RESET_BTN.click_count}")
        }
    }

    // A second, closable window — driveable via imgui_open / imgui_close.
    // Initial state.open is false (auto-emit zero); enqueue open via
    //   curl -X POST -d '{"name":"imgui_open","args":{"target":"DETAIL_WIN"}}' \
    //                localhost:9090/command
    SetNextWindowPos(ImVec2(460.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(300.0f, 240.0f), ImGuiCond.FirstUseEver)
    window(DETAIL_WIN, (text = "Detail (closable)", closable = true,
                        flags = ImGuiWindowFlags.None)) {
        Text("close me with the X, or via imgui_close")
        Separator()
        Text("size: {DETAIL_WIN.size.x}, {DETAIL_WIN.size.y}")
        button(PING, (text = "Ping"))
        Text("ping clicks: {PING.click_count}")
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData())

    live_end_frame()
}

[export]
def shutdown() {
    live_imgui_shutdown()
    live_destroy_window()
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
