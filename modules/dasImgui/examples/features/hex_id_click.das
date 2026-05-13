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

// =============================================================================
// FEATURE: hex_id_click — Phase 2.3 hex_id-targeted dispatch.
// SHOWS:   imgui_click resolves "0x..." hex strings via g_id_to_path. The
//          path lookup hits → dispatch goes through the L2 path the widget
//          was registered under, so click_count increments just like a
//          name-targeted click. Hex_ids without a path fall to L1 click
//          synthesis (out-of-band; not exercised here — see classify_target
//          comments for the L1 fallback contract).
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals.DEMO_BTN.hex_id'           # → 2749588296
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"0xa3d68f08"}}' \
//              localhost:9090/command                    # → {"ok":true,"value":"DEMO_BTN", ...}
// STANDALONE: daslang.exe modules/dasImgui/examples/features/hex_id_click.das
// LIVE:       daslang-live modules/dasImgui/examples/features/hex_id_click.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 2.3 — hex_id_click", 800, 600)
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

    SetNextWindowSize(ImVec2(720.0, 220.0), ImGuiCond.Always)
    if (Begin("hex_id_click — phase 2.3", null, ImGuiWindowFlags.None)) {
        Text("Click via name or hex_id: imgui_click resolves both forms.")
        Text("Hex strings ('0x...') reverse-map through g_id_to_path to L2.")
        if (button(DEMO_BTN, (text = "Click me"))) {
            print("DEMO_BTN clicked, count={DEMO_BTN.click_count}\n")
        }
        Text("clicks: {DEMO_BTN.click_count}")
    }
    End()

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
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
