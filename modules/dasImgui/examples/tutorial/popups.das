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
// TUTORIAL: popups — two right-click context popup [container]s.
//
//   popup_context_item(IDENT, (str_id = "..",          — attaches to the
//                              flags = ...)) {           PREVIOUSLY submitted
//       <menu_item or any body>                          item; right-click
//   }                                                    THAT item to open.
//
//   popup_context_window(IDENT, (str_id = "..",        — attaches to the
//                                flags = ...)) {         enclosing window;
//       <menu_item or any body>                          right-click ANYWHERE
//   }                                                    inside opens.
//
// ImGui drives open/close internally — no pending_open / pending_close
// fields on the state; the body runs only on the frames the popup is
// visible. `str_id` is the popup's stack id; pass a unique non-empty
// string per consumer (the C-default `"##ContextWindow"` fires only on
// a NULL const char* which daslang's string binding doesn't expose).
//
// flags = ImGuiPopupFlags.MouseButtonRight is the conventional opener;
// MouseButtonLeft / MouseButtonMiddle work too (rarely useful).
//
// The window popup ORs in NoOpenOverItems so a right-click ON the button
// opens the ITEM menu, not the window menu. Without it the window popup
// swallows every right-click inside the window, items included, and the
// item menu can never open.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/popups.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/popups.das
// =============================================================================

var private g_item_action : string = "(right-click the button)"
var private g_win_action  : string = "(right-click empty area)"

[export]
def init() {
    live_create_window("dasImgui popups tutorial", 760, 560)
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

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(720.0f, 520.0f), ImGuiCond.Always)
    window(PO_WIN, (text = "popups tutorial", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        text("Right-click context popups - two flavors.")
        text(PO_HINT, (text = "popup_context_item attaches to the previous item; popup_context_window attaches to the enclosing window."))
        separator()

        // ---- Stage 1: popup_context_item — attached to a specific item ----
        text("Stage 1 - right-click the target button below:")
        button(PO_TARGET, (text = "Right-click me"))
        popup_context_item(PO_CTX_ITEM, (str_id = "ctx_item",
                                          flags = ImGuiPopupFlags.MouseButtonRight)) {
            if (menu_item(PO_RENAME, (text = "Rename", shortcut = "F2"))) {
                g_item_action = "Rename"
            }
            if (menu_item(PO_DELETE, (text = "Delete", shortcut = "Del"))) {
                g_item_action = "Delete"
            }
            separator()
            if (menu_item(PO_INFO, (text = "Info", shortcut = ""))) {
                g_item_action = "Info"
            }
        }
        text("last item action: {g_item_action}")
        spacing()
        separator()

        // ---- Stage 2: popup_context_window — anywhere in the window ----
        text("Stage 2 - right-click empty space (not over an item):")
        text("NoOpenOverItems cedes item right-clicks to the item menu above.")
        popup_context_window(PO_CTX_WIN, (str_id = "ctx_win",
                                           flags = ImGuiPopupFlags.MouseButtonRight |
                                                   ImGuiPopupFlags.NoOpenOverItems)) {
            if (menu_item(PO_REFRESH, (text = "Refresh", shortcut = "F5"))) {
                g_win_action = "Refresh"
            }
            if (menu_item(PO_RESET, (text = "Reset", shortcut = ""))) {
                g_win_action = "Reset"
            }
            separator()
            if (menu_item(PO_ABOUT, (text = "About", shortcut = ""))) {
                g_win_action = "About"
            }
        }
        text("last window action: {g_win_action}")
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
