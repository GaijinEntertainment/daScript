options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``tree_image_misc.apng`` against ``examples/tutorial/tree_image_misc.das``.
//! Three voiced beats over the two leaf widgets the container forms don't cover:
//!   1. intro - name both widgets.
//!   2. tree_node_ex - click the header to expand; the three children appear and
//!      TI_TREE.opened flips to true (the leaf form's open bool, verified directly -
//!      the @optional `opened` field is absent while closed, present+true once open).
//!   3. image - the display-only sibling of image_button; nothing to click, so the
//!      beat verifies it is on screen (record_check_rendered) while the voice explains.

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = snap?["globals"]?[ident]?["bbox"]
    return (((b?["x"] ?? 0.0f) + (b?["z"] ?? 0.0f)) * 0.5f,
            ((b?["y"] ?? 0.0f) + (b?["w"] ?? 0.0f)) * 0.5f)
}

//! Click the tree header UNDER an already-posted voice line, then assert it opened
//! (TI_TREE.opened == true) - the children render as a side effect.
def expand_under_voice(app : ImguiApp; dwell : uint; tree : string) {
    let anchor = record_frame_count(app)
    let lead = dwell > 1400u ? 650u : dwell / 3u
    hold_content(app, lead)
    post_command(app, "imgui_click", JV((target = tree)))
    record_check_value(app, tree, "opened", true, 180)
    hold_remainder_content(app, dwell, anchor)
}

[export]
def main {
    with_recording_app("examples/tutorial/tree_image_misc.das",
                       "tree_image_misc.apng", 35) $(app) {
        let T_TREE = "TI_WIN/TI_TREE"
        let T_IMG  = "TI_WIN/TI_IMG"

        var snap = wait_for_widget(app, T_TREE, 10.0f)
        if (snap == null) {
            panic("{T_TREE} never registered - wrong app running?")
        }
        let p_tree = widget_center(snap, T_TREE)

        // Park the cursor on the tree header and warm up the capture pipeline.
        move_to(app, p_tree, 800)
        hold_content(app, 1800u)

        // ---- Beat 1: intro ----
        say(app, "tree_node_ex + image", T_TREE,
            [voice = "Two leaf widgets the container forms don't cover - tree_node_ex, and image."])

        // ---- Beat 2: tree_node_ex - click to expand ----
        expand_under_voice(app, say_begin(app, "tree_node_ex: leaf form, returns the open bool", T_TREE,
            [voice = "tree_node_ex is the leaf form. It returns the open bool, and the caller pairs tree_pop. Click the header and the three children appear."]),
            T_TREE)

        // ---- Beat 3: image - display only ----
        var s = snapshot(app)
        let p_img = widget_center(s, T_IMG)
        move_to(app, p_img, 700)
        hold_content(app, SETTLE_MS)
        record_check_rendered(app, T_IMG, true, 180)
        say(app, "image: display-only, paints a texture", T_IMG,
            [voice = "image is the display-only sibling of image_button. It paints a texture, with no click and no padding."])
    }
}
