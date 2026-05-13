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
// FEATURE: inputs_choice — Phase 0b.2 Combo + ListBox (deprecated single-call
//          forms). Both widgets take `items : array<string>` directly — daslang
//          marshals it C-side without a shim.
// SHOWS:   combo / list_box
// DRIVE:   curl -X POST -d '{"name":"imgui_set","args":{"target":"FRUIT","value":2}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_set","args":{"target":"SHIRT_SIZE","value":1}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/inputs_choice.das
// LIVE:       daslang-live modules/dasImgui/examples/features/inputs_choice.das
//
// NOT YET COVERED HERE:
//   - BeginCombo/EndCombo (block-arg manual loop) → 0b.3 containers
//   - BeginListBox/EndListBox (block-arg)         → 0b.3 containers
//   - _builtin_Combo (callback-getter)            → 0b.4 ABI shim
// =============================================================================

[export]
def init() {
    live_create_window("Phase 0b.2 — choice inputs", 720, 500)
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

    SetNextWindowSize(ImVec2(660.0, 460.0), ImGuiCond.Always)
    if (Begin("Choice inputs — phase 0b.2", null, ImGuiWindowFlags.None)) {
        Text("Combo (dropdown selector)")
        Separator()

        combo(FRUIT, (text = "Fruit",
                      items <- ["Apple", "Banana", "Cherry", "Durian", "Elderberry"]))
        Text("fruit index = {FRUIT.value}")

        // popup_max_height_in_items caps the dropdown height — useful for long lists.
        combo(WEEKDAY, (text = "Weekday",
                        items <- ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"],
                        popup_max_height_in_items = 4))
        Text("weekday index = {WEEKDAY.value}")

        Spacing()
        Text("ListBox (always-open scrollable list)")
        Separator()

        list_box(SHIRT_SIZE, (text = "Shirt size",
                              items <- ["XS", "S", "M", "L", "XL", "XXL"],
                              height_in_items = 4))
        Text("shirt size index = {SHIRT_SIZE.value}")

        Spacing()
        Text("imgui_set with target=FRUIT, value=N jumps to N-th item next frame")
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
