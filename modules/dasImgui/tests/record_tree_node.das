options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record tree_node.apng against examples/tutorial/tree_node.das.
//! Three patterns, each driven the human way (real header / arrow clicks, never imgui_open/close)
//! and self-verified — the recording doubles as a test. The toggle is asserted by the CHILD's
//! rendered state ("the body runs only while the node is open"), which is exactly what the widget
//! teaches and is readable even when the container's own opened/flags are absent from the payload:
//!   PHYSICS (DefaultOpen) -> click the header to collapse, click again to expand
//!   RENDER  (nested)      -> click the header; the Camera sub-tree opens with it (slider 3 deep)
//!   ASSETS  (OpenOnArrow) -> the arrow toggles; clicking the LABEL is a no-op
//! tree_node now exposes its header bbox (current_item_bbox -> open_close_finalize), so the chevron
//! and label are real click targets; toggle_tree_voice picks arrow vs label and asserts the result.

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    var b = snap?["globals"]?[ident]?["bbox"]
    let x = b?["x"] ?? 0.0f
    let y = b?["y"] ?? 0.0f
    let z = b?["z"] ?? 0.0f
    let w = b?["w"] ?? 0.0f
    return ((x + z) * 0.5f, (y + w) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/tree_node.das",
                       "tree_node.apng", 60) $(app) {
        let H_PHYS  = "TN_WIN/PHYSICS_TREE"
        let H_REND  = "TN_WIN/RENDER_TREE"
        let H_ASSET = "TN_WIN/ASSETS_TREE"
        let C_PHYS  = "TN_WIN/PHYSICS_TREE/PHYSICS_GRAVITY"
        let C_FOV   = "TN_WIN/RENDER_TREE/CAMERA_SUBTREE/RENDER_FOV"
        let C_HINT  = "TN_WIN/ASSETS_TREE/ASSETS_HINT"

        var snap = wait_for_render(app, H_PHYS, 10.0f)
        if (snap == null) {
            panic("{H_PHYS} never rendered - wrong app running?")
        }

        // ---- Stage 1: PHYSICS (DefaultOpen) — click the header to collapse ----
        move_to(app, widget_center(snap, H_PHYS), 600)
        hold_content(app, SETTLE_MS)
        let d1 = say_begin(app, "tree node - click to collapse", H_PHYS,
            [voice = "A tree node folds a group of controls under one header. Physics starts open. Click the header and it collapses, and the sliders inside simply stop drawing."])
        toggle_tree_voice(app, d1, H_PHYS, false, C_PHYS, false)

        // ---- Stage 2: PHYSICS — click again to expand ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, H_PHYS), 400)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "click again to expand", H_PHYS,
            [voice = "Click the header again and the controls come right back. The body only runs while the node is open."])
        toggle_tree_voice(app, d2, H_PHYS, false, C_PHYS, true)

        // ---- Stage 3: RENDER — nested tree_nodes ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, H_REND), 600)
        hold_content(app, SETTLE_MS)
        let d3 = say_begin(app, "nodes nest", H_REND,
            [voice = "Tree nodes nest freely. Open Render and its Camera sub-tree opens with it, exposing a slider three levels deep."])
        toggle_tree_voice(app, d3, H_REND, false, C_FOV, true)

        // ---- Stage 4: ASSETS (OpenOnArrow) — the arrow toggles ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, H_ASSET), 700)
        hold_content(app, SETTLE_MS)
        let d4 = say_begin(app, "OpenOnArrow: the arrow toggles", H_ASSET,
            [voice = "This node uses the open on arrow flag. Only the little arrow toggles it. Click the arrow and it folds away."])
        toggle_tree_voice(app, d4, H_ASSET, true, C_HINT, false)

        // ---- Stage 5: ASSETS — clicking the label is a no-op ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, H_ASSET), 400)
        hold_content(app, SETTLE_MS)
        let d5 = say_begin(app, "the label does nothing", H_ASSET,
            [voice = "Now click the label itself, and nothing happens. With open on arrow the label is free for selection instead of toggling."])
        toggle_tree_voice(app, d5, H_ASSET, false, C_HINT, false)

        // ---- Stage 6: ASSETS — the arrow opens it again ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, H_ASSET), 400)
        hold_content(app, SETTLE_MS)
        let d6 = say_begin(app, "the arrow opens it again", H_ASSET,
            [voice = "Click the arrow once more and it opens. That is the whole point: the row stays clickable while the arrow controls the fold."])
        toggle_tree_voice(app, d6, H_ASSET, true, C_HINT, true)
    }
}
