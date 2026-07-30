options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: popup_context_item — [container] for BeginPopupContextItem/EndPopup.
//          Right-click context menu attached to the previously submitted item.
//          ImGui drives open/close internally — no pending_open/close.
// SHOWS:   right-click any TARGET_BTN row to open its action menu; selected
//          action increments PICKED counter and updates LAST_ACTION text.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/popup_context_item.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/popup_context_item.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/popup_context_item.das
// =============================================================================

var LAST_ACTION : string = "(none yet)"

[export]
def init() {
    harness_init("popup_context_item — container", 640, 480)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(580.0, 420.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "popup_context_item", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Right-click the button below to open its context menu.")

        if (button(TARGET_BTN, (text = "Right-click me"))) {
            LAST_ACTION = "(left-click ignored)"
        }
        popup_context_item(TARGET_CTX, (str_id = "target_ctx",
                                        flags = ImGuiPopupFlags.MouseButtonRight)) {
            if (menu_item(ACTION_RENAME, (text = "Rename", shortcut = "F2"))) {
                LAST_ACTION = "Rename"
            }
            if (menu_item(ACTION_DELETE, (text = "Delete", shortcut = "Del"))) {
                LAST_ACTION = "Delete"
            }
            separator()
            if (menu_item(ACTION_INFO, (text = "Info", shortcut = ""))) {
                LAST_ACTION = "Info"
            }
        }

        separator()
        text("Last action: {LAST_ACTION}")
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
