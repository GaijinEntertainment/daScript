options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: tooltip_flat — flat-form tooltip widgets (boost v2 §3.2).
// SHOWS:   set_tooltip(text=…) — unconditional tooltip on the hover-stack.
//          set_item_tooltip(text=…) — fires only when the previous widget
//          is hovered (inlines IsItemHovered(ForTooltip) + SetTooltip since
//          ImGui's SetItemTooltip isn't bound to daslang).
//          Each carries a NarrativeState payload with the tooltip copy, so
//          playwright can assert content without driving the cursor over.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals."MAIN_WIN/TOOLTIP_ON_BUTTON"'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/tooltip_flat.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/tooltip_flat.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/tooltip_flat.das
// =============================================================================

[export]
def init() {
    harness_init("tooltip_flat — boost v2 §3.2", 720, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 300.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "set_tooltip / set_item_tooltip",
                      closable = false, flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Hover the button to see set_item_tooltip fire."))
        if (button(HOVER_BTN, (text = "hover me"))) {}
        set_item_tooltip(TOOLTIP_ON_BUTTON, (text = "Fires only while HOVER_BTN is hovered."))

        text(MID, (text = "Unconditional tooltip below — drives a popup any time hover-stack is active."))
        if (button(ALWAYS_BTN, (text = "always-tooltip target"))) {}
        if (IsItemHovered(ImGuiHoveredFlags.None)) {
            set_tooltip(TOOLTIP_ALWAYS, (text = "Fires whenever the surrounding hover gate matches."))
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
