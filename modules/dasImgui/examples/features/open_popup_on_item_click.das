options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: open_popup_on_item_click — right-click an item to open a popup.
// SHOWS:   The stateful overload — `open_popup_on_item_click(POPUP_STATE,
//          flags)` — checks IsMouseReleased + IsItemHovered on the previously
//          submitted widget and queues `state.pending_open` on the bound
//          `popup(IDENT, ...)` container. The container then opens itself
//          inside its own PushID(IDENT) scope on the next frame. Trigger by
//          right-clicking BTN_TARGET; the popup's BTN_OK click increments a
//          counter rendered into STATUS.
// DRIVE:   curl -X POST -d '{"name":"imgui_click","args":{"target":"MAIN_WIN/BTN_TARGET","button":1}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/open_popup_on_item_click.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/open_popup_on_item_click.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/open_popup_on_item_click.das
// =============================================================================

var private g_picks = 0

[export]
def init() {
    harness_init("open_popup_on_item_click", 520, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(480.0, 320.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "open_popup_on_item_click", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Right-click the button below to open a popup."))

        if (button(BTN_TARGET, (text = "Right-click me"))) {}
        // Stateful binding: pairs with the `popup(MENU, ...)` below.
        open_popup_on_item_click(MENU, ImGuiPopupFlags.MouseButtonRight)

        popup(MENU, (text = "MyPopup", flags = ImGuiWindowFlags.None)) {
            text(POPUP_LEAD, (text = "Pick one:"))
            if (button(BTN_OK, (text = "Pick OK"))) {
                g_picks++
            }
            same_line()
            if (button(BTN_CANCEL, (text = "Cancel"))) {}
        }

        text(STATUS, (text = "picks = {g_picks}"))
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
