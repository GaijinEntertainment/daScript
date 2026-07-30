options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: popup_window — [container] for BeginPopup/EndPopup with manual open.
//          Unlike popup_context_item (right-click on preceding item) or
//          popup_context_window (right-click anywhere in window), popup_window
//          has NO auto-trigger — the caller drives open_popup(str_id) on any
//          custom predicate. Body runs only while ImGui reports the popup open.
// SHOWS:   two buttons share str_id "fruit_picker"; each opens the same popup
//          under its own with_id scope. The hover-region trigger demonstrates
//          the cpp section 21 sub 2 pattern (IsMouseReleased(Right) +
//          !IsAnyItemHovered, no preceding item involved).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/popup_window.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/popup_window.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/popup_window.das
// =============================================================================

var LAST_PICK : string = "(none yet)"

[export]
def init() {
    harness_init("popup_window — container", 640, 480)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(580.0, 420.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "popup_window", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Two triggers share str_id \"fruit_picker\":")
        bullet()
        text("Left-click [Open via button] — explicit trigger.")
        bullet()
        text("Right-click the hover region — manual cpp-style trigger.")

        // ----- Trigger 1: left-click button -----
        with_id("via_button") {
            if (button(OPEN_BTN, (text = "Open via button"))) {
                open_popup("fruit_picker")
            }
            popup_window(BTN_POPUP, (str_id = "fruit_picker",
                                     flags = ImGuiWindowFlags.None)) {
                draw_picker_body()
            }
        }

        separator()

        // ----- Trigger 2: hover region + right-click (no preceding item) -----
        with_id("via_region") {
            text(HOVER_HINT, (text = "(hover this region, then right-click)"))
            let region_min = GetItemRectMin()
            let region_max = GetItemRectMax()
            let in_region = (IsMouseHoveringRect(region_min, region_max, true) &&
                             !IsAnyItemHovered())
            if (in_region && IsMouseReleased(ImGuiMouseButton.Right)) {
                open_popup("fruit_picker")
            }
            popup_window(REG_POPUP, (str_id = "fruit_picker",
                                     flags = ImGuiWindowFlags.None)) {
                draw_picker_body()
            }
        }

        separator()
        text("Last pick: {LAST_PICK}")
    }

    harness_end_frame()
}

def private draw_picker_body() {
    text("Pick a fruit:")
    if (menu_label(PICK_APPLE, (text = "Apple"))) {
        LAST_PICK = "Apple"
        close_current_popup()
    }
    if (menu_label(PICK_BANANA, (text = "Banana"))) {
        LAST_PICK = "Banana"
        close_current_popup()
    }
    if (menu_label(PICK_CHERRY, (text = "Cherry"))) {
        LAST_PICK = "Cherry"
        close_current_popup()
    }
    separator()
    if (button(CLOSE_BTN, (text = "Close"))) {
        close_current_popup()
    }
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
