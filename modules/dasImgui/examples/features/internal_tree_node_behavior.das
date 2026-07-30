options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: internal_tree_node_behavior — the imgui_internal.h tree-node state
//          machine the public tree_node widget is built on. Cherry-picked into the
//          v2 binding: TreeNodeBehavior (nullable label_end -> manual C++ wrapper in
//          dasIMGUI.main.cpp that pins it to nullptr) is the open/closed core;
//          TreeNodeSetOpen / TreeNodeUpdateNextOpen (raw, internal_allow_func) set /
//          resolve a node's open state by ID. Pairs with the already-bound
//          TreePushOverrideID + tree_pop(). This is the "binding is usable" gate: it
//          drives the real bound API at runtime, not just compiles it.
// SHOWS:   a hand-rolled tree (TreeNodeBehavior draws the arrow + label and toggles
//          on click — nested, with tree_pop() on open); imperative open/close of a
//          node by ID via TreeNodeSetOpen (no click needed); and TreeNodeUpdateNextOpen
//          reading back the resolved open state for an ID.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_tree_node_behavior.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_tree_node_behavior.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_tree_node_behavior.das
// =============================================================================

// A custom tree node from the raw primitive: TreeNodeBehavior draws the arrow +
// label, toggles open/closed on click, and (when open, without NoTreePushOnOpen)
// pushes a tree layer — so the caller renders children + tree_pop() on a true return.
def private custom_tree(lbl : string; flags : ImGuiTreeNodeFlags) : bool {
    return TreeNodeBehavior(GetID(lbl), flags, lbl)
}

[export]
def init() {
    harness_init("internal_tree_node_behavior - hand-rolled tree from primitives", 760, 560)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 500.0), ImGuiCond.Always)
    window(TN_WIN, (text = "internal_tree_node_behavior", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        // ----- 1. hand-rolled, nested tree nodes from TreeNodeBehavior -----
        text("TreeNodeBehavior(id, flags, label): the raw open/closed state machine.")
        text("  click an arrow to toggle — the public tree_node, assembled in user code.")
        if (custom_tree("Fruits", ImGuiTreeNodeFlags.None)) {
            text("- apple")
            text("- banana")
            if (custom_tree("Citrus", ImGuiTreeNodeFlags.None)) {
                text("- orange")
                text("- lemon")
                tree_pop()
            }
            tree_pop()
        }
        if (custom_tree("Vegetables", ImGuiTreeNodeFlags.None)) {
            text("- carrot")
            tree_pop()
        }
        separator()

        // ----- 2. imperative open/close by ID via TreeNodeSetOpen -----
        text("TreeNodeSetOpen(id, open): force a node's state by ID (no click needed).")
        if (button(BTN_OPEN, (text = "Open 'Fruits'"))) {
            TreeNodeSetOpen(GetID("Fruits"), true)
        }
        same_line()
        if (button(BTN_CLOSE, (text = "Close 'Fruits'"))) {
            TreeNodeSetOpen(GetID("Fruits"), false)
        }
        separator()

        // ----- 3. resolved open state by ID via TreeNodeUpdateNextOpen -----
        text("TreeNodeUpdateNextOpen(id, flags): the resolved open state for an ID.")
        let fruits_open = TreeNodeUpdateNextOpen(GetID("Fruits"), ImGuiTreeNodeFlags.None)
        text("  'Fruits' resolved-open = {fruits_open}")
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
