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
// FEATURE: triggers — every Phase 0b.1 trigger widget in one window.
// SHOWS:   Button / SmallButton / ArrowButton / InvisibleButton / ImageButton
//          / Checkbox / RadioButton (bool + grouped int) / Selectable / MenuItem
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"SAVE_BTN"}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_set","args":{"target":"OPT_A","value":true}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_set","args":{"target":"DIFFICULTY","value":2}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/triggers.das
// LIVE:       daslang-live modules/dasImgui/examples/features/triggers.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 0b.1 — triggers", 800, 600)
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

    SetNextWindowSize(ImVec2(720.0, 560.0), ImGuiCond.Always)
    if (Begin("Triggers — phase 0b.1", null, ImGuiWindowFlags.None)) {
        // ----- Click family -----
        Text("Click family")
        Separator()
        if (button(SAVE_BTN, (text = "Save"))) {
            print("save clicked, count={SAVE_BTN.click_count}\n")
        }
        Text("save clicks: {SAVE_BTN.click_count}")
        small_button(QUIT_BTN, (text = "Quit"))
        Text("quit clicks: {QUIT_BTN.click_count}")
        SameLine(0.0f, -1.0f)
        arrow_button(BACK_BTN, (text = "back", dir = ImGuiDir.Left))
        SameLine(0.0f, -1.0f)
        arrow_button(FWD_BTN, (text = "fwd", dir = ImGuiDir.Right))
        Text("nav clicks: back={BACK_BTN.click_count}, fwd={FWD_BTN.click_count}")
        invisible_button(HOTSPOT, (text = "hotspot", size = float2(120.0f, 24.0f)))
        Text("invisible-hotspot clicks: {HOTSPOT.click_count}")

        let io & = unsafe(GetIO())
        let font_tex = io.Fonts.TexID
        if (font_tex != null) {
            image_button(FONT_BTN, (text = "font_atlas", user_texture_id = font_tex,
                                    size = float2(48.0f, 48.0f)))
            Text("image-button clicks: {FONT_BTN.click_count}")
        }

        Spacing()

        // ----- Toggle family -----
        Text("Toggle family")
        Separator()
        checkbox(WIREFRAME, (text = "Wireframe"))
        Text("wireframe = {WIREFRAME.value}")
        radio_button(MUTE, (text = "Mute"))
        Text("mute = {MUTE.value}")
        selectable(SEL_ITEM, (text = "Selectable item"))
        Text("selectable = {SEL_ITEM.value}")
        menu_item(QUICK_SAVE, (text = "Quick save", shortcut = "Ctrl+S"))
        Text("quick-save = {QUICK_SAVE.value}")

        Spacing()

        // ----- Grouped radios (shared state) -----
        Text("Difficulty (radio_button_int — three calls share DIFFICULTY)")
        Separator()
        radio_button_int(DIFFICULTY, (text = "Easy", v_button = 0))
        SameLine(0.0f, -1.0f)
        radio_button_int(DIFFICULTY, (text = "Normal", v_button = 1))
        SameLine(0.0f, -1.0f)
        radio_button_int(DIFFICULTY, (text = "Hard", v_button = 2))
        Text("difficulty = {DIFFICULTY.value}")
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
    //! Standalone entrypoint. daslang-live drives init/update/shutdown
    //! directly and ignores main(); plain `daslang.exe <file>` runs this loop.
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
