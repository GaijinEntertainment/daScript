options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: popup_context_window — right-click context menu over the host
//                                 window.
// SHOWS:   `popup_context_window(POPUP, (str_id = "##ctx", flags = MouseButtonRight))`
//          opens whenever the user right-clicks anywhere inside the enclosing
//          window. ImGui drives open/close internally; the body runs only
//          while open. `str_id` is the popup's stack id — use a unique
//          non-empty string per consumer.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/popup_context_window.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/popup_context_window.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/popup_context_window.das
// =============================================================================

var private LAST_PICK = "(none yet)"

[export]
def init() {
    harness_init("popup_context_window — right-click anywhere", 520, 280)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(480.0, 240.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "popup_context_window", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Right-click anywhere in this window to open the context menu."))
        separator(SEP)
        text(STATUS, (text = "last pick: {LAST_PICK}"))

        popup_context_window(CTX, (str_id = "##ctxwin",
                                   flags = ImGuiPopupFlags.MouseButtonRight)) {
            if (menu_item(ITEM_NEW, (text = "New"))) {
                LAST_PICK = "New"
            }
            if (menu_item(ITEM_OPEN, (text = "Open"))) {
                LAST_PICK = "Open"
            }
            if (menu_item(ITEM_SAVE, (text = "Save"))) {
                LAST_PICK = "Save"
            }
            separator(SEP_MENU)
            if (menu_item(ITEM_QUIT, (text = "Quit"))) {
                LAST_PICK = "Quit"
            }
        }
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
