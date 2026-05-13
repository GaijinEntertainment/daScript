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
// FEATURE: save_demo — the spec §7.1 example. Three widgets exercised by
//          test_save_demo (the playwright-API end-to-end happy path):
//            NAME_INPUT   (input_text)  — user types here
//            SAVE_BTN     (button)      — click sets STATUS_TEXT
//            STATUS_TEXT  (text_show)   — reflects "saved" after click
// SHOWS:   First externally-facing test of the public imgui_playwright
//          surface — click + type_text (auto-focus) + expect_value flow.
// DRIVE:   curl -X POST -d '{"name":"imgui_click","args":{"target":"SAVE_BTN"}}' \
//               localhost:9090/command
//          → globals.STATUS_TEXT.payload.value flips to "saved"
//          curl -X POST -d '{"name":"imgui_set","args":{"target":"NAME_INPUT","value":"hello"}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/save_demo/main.das
// LIVE:       daslang-live modules/dasImgui/examples/save_demo/main.das
// =============================================================================

[export]
def init() {
    live_create_window("save_demo — playwright spec §7.1", 600, 240)
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
    advance_coroutines()
    NewFrame()

    SetNextWindowSize(ImVec2(560.0, 200.0), ImGuiCond.Always)
    if (Begin("Save demo", null, ImGuiWindowFlags.None)) {
        input_text(NAME_INPUT, (text = "Name"))
        if (button(SAVE_BTN, (text = "Save"))) {
            STATUS_TEXT.value := "saved"
        }
        text_show(STATUS_TEXT)
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
