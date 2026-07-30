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
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: main_menu_bar — top-level menu bar + menu_item vs menu_label split.
//
// Two related v2 additions live in this tutorial:
//
// 1. `main_menu_bar(IDENT) { ... }` is the screen-top bar (`BeginMainMenuBar`)
//    — distinct from `menu_bar(IDENT) { ... }` which attaches to the current
//    window (`BeginMenuBar`, requires `ImGuiWindowFlags.MenuBar`).
//
// 2. `menu_item` vs `menu_label`:
//    - menu_item(IDENT, (text=, shortcut=, enabled=)) → toggleable.
//      ToggleState.value mutates on click.
//    - menu_label(IDENT, (text=, shortcut=, selected=, enabled=)) → static.
//      ClickState.click_count accumulates; `selected` is a fixed check-mark.
//
// Use menu_label for section headers (`selected=false, enabled=false`),
// permanent-check labels (`selected=true`), and any "do something, don't
// toggle" menu entry.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/main_menu_bar.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/main_menu_bar.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"MAIN_BAR/FILE_MENU/MI_DARKMODE"}}' \
//        localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui main_menu_bar tutorial", 800, 480)
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

    // The bar sits at screen top, NOT inside any window().
    main_menu_bar(MAIN_BAR) {
        menu(FILE_MENU, (text = "File", enabled = true)) {
            // Toggleable — clicking flips MI_DARKMODE.value.
            menu_item(MI_DARKMODE, (text = "Dark mode", shortcut = "Ctrl+D"))
            menu_item(MI_AUTOSAVE, (text = "Auto-save", shortcut = ""))
            separator(FILE_SEP)
            // Section header — static, disabled, no check.
            menu_label(FILE_HEADER, (text = "(recent files)",
                                     shortcut = "", selected = false,
                                     enabled = false))
            menu_label(FILE_RECENT1, (text = "demo.das"))
            menu_label(FILE_RECENT2, (text = "tour.das"))
            separator(FILE_SEP2)
            // Permanent check — selected=true makes it always-checked.
            menu_label(FILE_STAR, (text = "Star this project",
                                   shortcut = "", selected = true))
            menu_item(MI_QUIT, (text = "Quit", shortcut = "Ctrl+Q"))
        }
        menu(EDIT_MENU, (text = "Edit", enabled = true)) {
            // menu_label for an action — emits click_count, no toggle.
            menu_label(EDIT_UNDO, (text = "Undo", shortcut = "Ctrl+Z"))
            menu_label(EDIT_REDO, (text = "Redo", shortcut = "Ctrl+Y",
                                   selected = false, enabled = false))
        }
    }

    SetNextWindowPos(ImVec2(60.0, 80.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(680.0, 320.0), ImGuiCond.Always)
    window(BODY, (text = "State readout", closable = false,
                  flags = ImGuiWindowFlags.None)) {
        text("Click items in the bar above; the snapshot updates below.")
        separator(BODY_SEP)
        text("MI_DARKMODE.value = {MI_DARKMODE.value}")
        text("MI_AUTOSAVE.value = {MI_AUTOSAVE.value}")
        text("EDIT_UNDO.click_count = {EDIT_UNDO.click_count}")
        text("MI_QUIT.value = {MI_QUIT.value}")
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
