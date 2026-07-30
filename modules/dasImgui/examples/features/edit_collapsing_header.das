options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require daslib/safe_addr

// =============================================================================
// FEATURE: edit_collapsing_header — closable CollapsingHeader on a caller-
//                                   owned bool? p_open.
// SHOWS:   `edit_collapsing_header(safe_addr(visible), (text = "label"))` —
//          while `*ptr == false` the header (and its body) is hidden
//          entirely. While `*ptr == true` the X close-button on the header
//          writes `*ptr = false` when clicked. Body of the conditional
//          runs only when the section is BOTH visible AND expanded.
//          Container-form sibling: `collapsing_header(closable=true)`.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/edit_collapsing_header.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/edit_collapsing_header.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/edit_collapsing_header.das
// =============================================================================

var private VISIBLE_A = true
var private VISIBLE_B = true

[export]
def init() {
    harness_init("edit_collapsing_header — closable", 480, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(440.0, 320.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "edit_collapsing_header", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Click the X on either header to hide it; use the buttons to restore."))

        if (edit_collapsing_header(safe_addr(VISIBLE_A), (id = "HDR_A", text = "Header A"))) {
            text(BODY_A_1, (text = "  body A — first line"))
            text(BODY_A_2, (text = "  body A — second line"))
        }

        if (edit_collapsing_header(safe_addr(VISIBLE_B), (id = "HDR_B", text = "Header B"))) {
            text(BODY_B_1, (text = "  body B — first line"))
            text(BODY_B_2, (text = "  body B — second line"))
        }

        separator(SEP)
        if (button(RESTORE_A, (text = "show A"))) {
            VISIBLE_A = true
        }
        same_line()
        if (button(RESTORE_B, (text = "show B"))) {
            VISIBLE_B = true
        }
        text(STATUS, (text = "A visible: {VISIBLE_A} | B visible: {VISIBLE_B}"))
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
