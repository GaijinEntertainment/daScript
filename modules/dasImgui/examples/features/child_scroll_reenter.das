options gen2
options _comment_hygiene = true

require imgui/imgui_harness

var private SCROLL_ROW : table<int; NarrativeState>

// nolint:STYLE014 — file-header feature banner displaced below a var decl
// =============================================================================
// FEATURE: child_scroll_reenter — re-enter an existing child window from
//          outside its body to nudge its scroll position via SetScrollX/Y.
// SHOWS:   `child(SCROLLY, (text = "lines", ...)) { ... 50 rows ... }` paints
//          a scrollable child. Two repeat-fire buttons below it call
//          `child_scroll_reenter("SCROLLY", "lines") { SetScrollY(...) }` to
//          scroll up/down. The wrapper replicates the original `child(IDENT)`
//          container's `PushID(IDENT)` frame so `BeginChild("lines", ...)`
//          hashes to the SAME window. Drop the PushID and ImGui hashes to a
//          phantom sibling — scroll observably does nothing.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/child_scroll_reenter.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/child_scroll_reenter.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/child_scroll_reenter.das
// =============================================================================

[export]
def init() {
    harness_init("child_scroll_reenter — boost v2", 520, 520)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(480.0, 480.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "child_scroll_reenter", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Hold the buttons to nudge SCROLLY's scroll position."))

        child(SCROLLY, (text = "lines", size = float2(440.0f, 320.0f),
                        child_flags = ImGuiChildFlags.Borders,
                        window_flags = ImGuiWindowFlags.None)) {
            for (i in range(50)) {
                text(SCROLL_ROW[i], (text = "row {i}"))
            }
        }

        // Repeat-fire buttons so a single hold scrolls continuously.
        with_button_repeat(true) {
            if (button(BTN_UP, (text = "scroll up"))) {}
            same_line()
            if (button(BTN_DOWN, (text = "scroll down"))) {}
        }
        same_line()
        text(STATUS, (text = "scroll.y: {int(SCROLLY.scroll.y)} / {int(SCROLLY.scroll_max.y)}"))

        let delta = 8.0f
        if (BTN_UP.click_count > 0 && BTN_UP.clicked) {
            child_scroll_reenter("SCROLLY", "lines") {
                SetScrollY(GetScrollY() - delta)
            }
        }
        if (BTN_DOWN.click_count > 0 && BTN_DOWN.clicked) {
            child_scroll_reenter("SCROLLY", "lines") {
                SetScrollY(GetScrollY() + delta)
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
