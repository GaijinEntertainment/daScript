options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: menu_label_static — static (non-toggleable) menu items (boost v2 §3.4).
// SHOWS:   menu_label(IDENT, (text=…, selected=…, enabled=…)) — the static
//          sibling of menu_item. `selected` is a plain bool (permanent check
//          mark when true), `enabled=false` greys out without flipping any
//          state on click. Backed by ClickState (click telemetry only — no
//          `value`). Use cases: section headers, permanent-state markers,
//          disabled placeholders.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals."MAIN_WIN/MENU/MI_SECTION_HEADER"'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/menu_label_static.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/menu_label_static.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/menu_label_static.das
// =============================================================================

[export]
def init() {
    harness_init("menu_label_static — boost v2 §3.4", 720, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 300.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "menu_label vs menu_item", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Right-click below to open a menu mixing static (menu_label) and toggleable (menu_item)."))

        if (button(ANCHOR, (text = "right-click me"))) {}
        popup_context_item(EDIT_POPUP, (str_id = "edit", flags = ImGuiPopupFlags.MouseButtonRight)) {
            // Section header — disabled + not selected; ClickState carries no value.
            menu_label(MI_SECTION_HEADER, (text = "Recent files", shortcut = "",
                                           selected = false, enabled = false))
            menu_label(MI_RECENT_A, (text = "file_a.das", shortcut = "",
                                     selected = false, enabled = true))
            menu_label(MI_RECENT_B, (text = "file_b.das", shortcut = "",
                                     selected = false, enabled = true))
            separator(POPUP_SEP_1)
            // Permanent-check label — selected always true, no flip on click.
            menu_label(MI_AUTOSAVE_ON, (text = "Autosave (always on)", shortcut = "",
                                        selected = true, enabled = false))
            separator(POPUP_SEP_2)
            // Toggleable real menu_item for contrast — flips state.value on click.
            menu_item(MI_AUTOSAVE_TOGGLE, (text = "Autosave (toggleable)",
                                           shortcut = "", enabled = true))
        }

        text(STATUS, (text = "MI_AUTOSAVE_TOGGLE state.value = {MI_AUTOSAVE_TOGGLE.value}"))
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
