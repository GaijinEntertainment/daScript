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
require imgui/imgui_id_builtin
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: popup_window — stateless BeginPopup/EndPopup with manual trigger.
//
//   popup_window(IDENT, (str_id = "..", flags = ..)) { body }
//
// Distinct from the auto-triggered relatives:
//
//   popup_context_item    — right-click on the preceding item opens the popup.
//   popup_context_window  — right-click anywhere in the window opens it.
//   popup_window          — NO auto-trigger; caller drives open_popup(str_id).
//
// The manual form maps the cpp pattern from `imgui_demo.cpp`'s table-context
// section: one shared str_id, multiple PushID-scoped instances, custom
// hover/key/release predicates feeding the same OpenPopup call.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/popup_window.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/popup_window.das
// =============================================================================

var private LAST_PICK : string = "(none yet)"

[export]
def init() {
    live_create_window("dasImgui popup_window tutorial", 760, 480)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.4
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(40.0f, 40.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(560.0f, 340.0f), ImGuiCond.Always)
    window(PW_WIN, (text = "popup_window", closable = false,
                    flags = ImGuiWindowFlags.None)) {
        text("Two scopes share str_id \"fruit_picker\":")

        // Scope A: explicit click trigger.
        with_id("via_button") {
            if (button(PW_BTN, (text = "Open via button"))) {
                open_popup("fruit_picker")
            }
            popup_window(PW_BTN_POPUP, (str_id = "fruit_picker",
                                        flags = ImGuiWindowFlags.None)) {
                draw_picker_body()
            }
        }

        separator()

        // Scope B: hover-region + right-click — no preceding item involved.
        with_id("via_region") {
            text(PW_HOVER, (text = "(hover this region, right-click)"))
            let region_min = GetItemRectMin()
            let region_max = GetItemRectMax()
            let in_region = (IsMouseHoveringRect(region_min, region_max, true) &&
                             !IsAnyItemHovered())
            if (in_region && IsMouseReleased(ImGuiMouseButton.Right)) {
                open_popup("fruit_picker")
            }
            popup_window(PW_REG_POPUP, (str_id = "fruit_picker",
                                        flags = ImGuiWindowFlags.None)) {
                draw_picker_body()
            }
        }

        separator()
        text(PW_STATUS, (text = "Last pick: {LAST_PICK}"))
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

def private draw_picker_body() {
    text("Pick a fruit:")
    if (menu_label(PW_APPLE, (text = "Apple"))) {
        LAST_PICK = "Apple"
        close_current_popup()
    }
    if (menu_label(PW_BANANA, (text = "Banana"))) {
        LAST_PICK = "Banana"
        close_current_popup()
    }
    if (menu_label(PW_CHERRY, (text = "Cherry"))) {
        LAST_PICK = "Cherry"
        close_current_popup()
    }
    separator()
    if (button(PW_CLOSE, (text = "Close"))) {
        close_current_popup()
    }
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
