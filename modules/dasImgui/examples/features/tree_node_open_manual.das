options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: tree_node_open + tree_pop — manual-pair tree node primitives.
// SHOWS:   - `tree_node_open(label, flags?)` — TreeNodeEx in OPEN form
//            WITHOUT auto-pair TreePop. Returns true when the node is open;
//            caller MUST invoke `tree_pop()` when true AND `NoTreePushOnOpen`
//            flag was NOT set.
//          - `tree_pop()` — manual TreePop. The `tree_node(...)` block-body
//            container auto-pairs TreePop; this wrapper is for cases where
//            the body needs to render OUTSIDE the conditional (e.g. inline
//            same_line() / extra widgets after the header trigger).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/tree_node_open_manual.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/tree_node_open_manual.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/tree_node_open_manual.das
// =============================================================================

[export]
def init() {
    harness_init("tree_node_open / tree_pop — manual-pair", 520, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(480.0, 320.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "tree_node_open + tree_pop", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Header has a sibling button on the SAME line — that pattern needs the manual pair."))

        // Render the header; if open, render an inline same_line widget
        // OUTSIDE the conditional body, then pop manually.
        let node_open = tree_node_open("Settings##manual")
        same_line()
        if (button(SETTINGS_RESET, (text = "reset"))) {}
        if (node_open) {
            text(BODY_HINT, (text = "(node body — rendered only while open)"))
            text(BODY_LINE_1, (text = "  - option A"))
            text(BODY_LINE_2, (text = "  - option B"))
            tree_pop()
        }

        separator(SEP1)
        text(NOTE, (text = "Compare: the tree_node(IDENT) container form auto-pairs TreePop for you."))
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
