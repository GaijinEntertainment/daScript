options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: unindent — manual cursor-unindent paired with indent.
// SHOWS:   unindent(IDENT, (indent_w?)) — pull the cursor left by indent_w
//          (or ImGui's IndentSpacing default when omitted). Pair with the
//          `indent` rail to bracket a section manually. For block-scoped
//          indent prefer `with_indent` (which handles the pop on exit).
//          unindent is useful when:
//            - The indent crosses block boundaries (early returns from a
//              section that already pushed indent).
//            - You want symmetric source for indent/unindent calls (e.g.
//              the layout reads as a tree structure).
//            - You need to unindent more than you indented (overrun negative
//              indent for back-into-margin effects).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/unindent.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/unindent.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/unindent.das
// =============================================================================

[export]
def init() {
    harness_init("unindent — boost v2 §3.5", 720, 420)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 360.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "indent / unindent — manual bracketing",
                      closable = false, flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "indent + unindent bracket a section manually. Default uses ImGui's IndentSpacing."))

        text(BEFORE, (text = "Before indent (column 0)"))
        indent(IND_1)
        text(INSIDE_1, (text = "After indent — one level deeper"))
        indent(IND_2, (indent_w = 32.0f))
        text(INSIDE_2, (text = "After a second 32px indent"))
        unindent(UND_2, (indent_w = 32.0f))
        text(BACK_1, (text = "After unindent(32) — back to one level"))
        unindent(UND_1)
        text(BACK_0, (text = "After unindent — back to column 0"))

        separator(SEP)
        text(LEAD_2, (text = "Asymmetric: 3 indents, 1 large unindent."))

        indent(IND_A)
        indent(IND_B)
        indent(IND_C)
        text(THREE_DEEP, (text = "3 indents deep"))
        unindent(UND_BIG, (indent_w = 64.0f))
        text(POP_TO_NEAR_MARGIN, (text = "After unindent(64) — closer to margin"))
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
