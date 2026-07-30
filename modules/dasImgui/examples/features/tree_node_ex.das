options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: tree_node_ex — leaf-form TreeNodeEx with explicit flag control.
// SHOWS:   tree_node_ex(IDENT, (text, flags)) — returns the open bool.
//          Unlike the `tree_node(IDENT, (...))` container which auto-pairs
//          TreePush/TreePop around its block body, the leaf form leaves
//          the caller in charge of TreePop. Use it when:
//            - You want ImGuiTreeNodeFlags.Leaf (no expand/collapse) for
//              clickable rows in a tree-shaped list.
//            - You want ImGuiTreeNodeFlags.OpenOnArrow + custom click
//              handling on the row's icon area.
//            - The body is conditional (e.g. lazy-render expensive sub-trees
//              only when open returns true).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/tree_node_ex.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/tree_node_ex.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/tree_node_ex.das
// =============================================================================

[export]
def init() {
    harness_init("tree_node_ex — boost v2 §3.4", 720, 480)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 420.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "tree_node_ex leaf form", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Leaf-form: caller pairs TreePop manually."))

        // Default node — no flags, behaves like an expandable header.
        let open_branch = tree_node_ex(BRANCH, (text = "Branch (expandable)",
                                                flags = ImGuiTreeNodeFlags.None))
        if (open_branch) {
            text(BRANCH_CHILD, (text = "Child line inside the expanded branch."))
            tree_pop()
        }

        // Leaf flag — no expand/collapse arrow, click handles like a row.
        let leaf_open = tree_node_ex(LEAF_ROW, (text = "Leaf (no arrow)",
                                                flags = ImGuiTreeNodeFlags.Leaf |
                                                        ImGuiTreeNodeFlags.NoTreePushOnOpen))
        // With NoTreePushOnOpen, the caller MUST NOT call TreePop().
        if (leaf_open) {
            // pure visual; nothing to render
        }

        // OpenOnArrow — click on the text doesn't toggle, only the arrow does.
        let arrow_open = tree_node_ex(ARROW_ONLY, (text = "OpenOnArrow",
                                                   flags = ImGuiTreeNodeFlags.OpenOnArrow))
        if (arrow_open) {
            text(ARROW_CHILD, (text = "Visible only when the arrow toggled it open."))
            tree_pop()
        }

        // Bullet glyph — leaf with a bullet marker.
        let bullet_open = tree_node_ex(BULLET_ROW, (text = "Bullet leaf",
                                                    flags = ImGuiTreeNodeFlags.Leaf |
                                                            ImGuiTreeNodeFlags.Bullet |
                                                            ImGuiTreeNodeFlags.NoTreePushOnOpen))
        if (bullet_open) {
            // pure visual; nothing to render
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
