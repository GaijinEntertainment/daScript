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
// TUTORIAL: containers — tab_bar, menu_bar, popup, item_tooltip.
//
// Containers wrap an ImGui begin/end pair around a daslang block. They
// share three properties:
//
//   1. Block-arg shape: the body runs once per frame inside the begin/end.
//   2. Path push: leaf widgets register under "<container>/<leaf>" — every
//      container contributes a segment to the registry path, just like
//      `window` and `with_id` did in earlier tutorials.
//   3. Open-state via pending flags: `(state).pending_open = true` queues
//      the container to open next frame; `pending_close = true` reverses
//      it. The same flag is what `imgui_open` / `imgui_close` mutate from
//      outside, so all three control surfaces (app code, live commands,
//      and the close-button on the chrome) share one channel.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/containers.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/containers.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_snapshot"}'                                                            localhost:9090/command
//   curl -X POST -d '{"name":"imgui_open","args":{"target":"CONT_WIN/OPTIONS_POPUP"}}'                     localhost:9090/command
//   curl -X POST -d '{"name":"imgui_close","args":{"target":"CONT_WIN/OPTIONS_POPUP"}}'                    localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui containers tutorial", 760, 560)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.5
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(30.0f, 30.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(680.0f, 480.0f), ImGuiCond.FirstUseEver)
    window(CONT_WIN, (text = "containers", closable = false,
                      flags = ImGuiWindowFlags.MenuBar)) {

        // ---- menu_bar (window must have ImGuiWindowFlags.MenuBar) ----
        // Each menu_item registers under "CONT_WIN/MAIN_BAR/FILE_MENU/NEW_ITEM".
        menu_bar(MAIN_BAR) {
            menu(FILE_MENU, (text = "File", enabled = true)) {
                menu_item(NEW_ITEM,  (text = "New",  shortcut = "Ctrl+N"))
                menu_item(OPEN_ITEM, (text = "Open", shortcut = "Ctrl+O"))
            }
        }

        // ---- tab_bar — three tabs, each its own block ----

        // Only the active tab's block runs each frame; inactive tabs never enter
        // their body, so their widgets aren't in the registry that frame.
        tab_bar(MAIN_TABS, (text = "MainTabs", flags = ImGuiTabBarFlags.None)) {
            tab_item(GENERAL_TAB, (text = "General", closable = false,
                                   flags = ImGuiTabItemFlags.None)) {
                text("Tabs share a window; only the active tab renders.")
                checkbox(WIRE, (text = "Wireframe"))
            }
            tab_item(AUDIO_TAB, (text = "Audio", closable = false,
                                 flags = ImGuiTabItemFlags.None)) {
                checkbox(MUTE, (text = "Mute"))
                slider_float(VOL, (text = "Volume"))
            }
            tab_item(INFO_TAB, (text = "Info", closable = false,
                                flags = ImGuiTabItemFlags.None)) {
                text("containers tutorial")
                text("each tab is a block - exclusive render")
            }
        }

        separator(CT_SEP_1)

        // ---- popup — opened by a pending flag ----

        // The button only flips the flag; the popup renders next frame when ImGui
        // sees `pending_open = true` and runs OpenPopup(). The same channel handles
        // imgui_open from outside (curl example in the DRIVE block).
        text("Popup - open via pending_open or imgui_open:")
        if (button(OPEN_POPUP_BTN, (text = "Open options"))) {
            OPTIONS_POPUP.pending_open = true
        }
        popup(OPTIONS_POPUP, (text = "OptionsPopup",
                              flags = ImGuiWindowFlags.None)) {
            text("Options")
            separator(CT_SEP_2)
            checkbox(OPT_VSYNC, (text = "VSync"))
            checkbox(OPT_HIDPI, (text = "HiDPI"))
            if (button(POPUP_CLOSE_BTN, (text = "Close"))) {
                OPTIONS_POPUP.pending_close = true
            }
        }

        separator(CT_SEP_3)

        // ---- item_tooltip — hover-gated overlay ----
        // BeginItemTooltip auto-checks IsItemHovered() under the hood, so
        // the block only runs while the previous widget is hovered.
        text("Tooltip - hover the button below:")
        button(HOVER_BTN, (text = "Hover me"))
        item_tooltip(HOVER_TIP) {
            text("This text appears on hover.")
            text("Driven by BeginItemTooltip (auto-gated).")
        }
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
