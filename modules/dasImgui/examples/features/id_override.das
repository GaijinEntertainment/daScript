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
require imgui/imgui_id_builtin

// =============================================================================
// FEATURE: id_override — §4.8 explicit ID-stack scoping via with_id.
// SHOWS:   - `with_id("section_a") { button(...) }` segments the widget path so
//            two render sites of the same widget kind (distinct identifiers)
//            register under "<window>/section_a/<IDENT>" vs ".../section_b/...".
//          - Nested `with_id` chains compose ("outer/inner/NESTED_BTN").
//          - The same single-global widget rendered under two distinct with_id
//            scopes does NOT trip the once-per-frame panic — check_unique_render
//            keys off the full path, not the bare identifier.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          # → widgets register under MAIN_WIN/section_a/SAVE_A_BTN etc.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/id_override.das
// LIVE:       daslang-live modules/dasImgui/examples/features/id_override.das
// =============================================================================

[export]
def init() {
    live_create_window("§4.8 — id_override (with_id)", 720, 540)
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

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(640.0f, 480.0f), ImGuiCond.FirstUseEver)
    window(MAIN_WIN, (text = "with_id demo", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        Text("Distinct widgets, distinct scopes:")
        with_id("section_a") {
            button(SAVE_A_BTN, (text = "Save A"))
        }
        with_id("section_b") {
            button(SAVE_B_BTN, (text = "Save B"))
        }

        Separator()
        Text("Same single-global widget under two scopes:")
        // SHARED_BTN's state global is shared; only the paths and ImGui
        // hashes differ. click_count aggregates across both render sites.
        with_id("xa") {
            button(SHARED_BTN, (text = "Shared (A)"))
        }
        with_id("xb") {
            button(SHARED_BTN, (text = "Shared (B)"))
        }
        Text("shared clicks (sum): {SHARED_BTN.click_count}")

        Separator()
        Text("Nested with_id chains:")
        with_id("outer") {
            with_id("inner") {
                button(NESTED_BTN, (text = "Nested"))
            }
        }
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
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
