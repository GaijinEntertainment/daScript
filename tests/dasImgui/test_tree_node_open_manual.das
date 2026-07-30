options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `tree_node_open` + `tree_pop` — the manual-pair
//! tree node primitives. `tree_node_open` is imperative (no [widget] kind,
//! no IDENT-bound path), so the test verifies the surrounding [widget]
//! siblings register in the expected closed-state shape: SETTINGS_RESET
//! same_line button + SEP1 + NOTE present, body widgets absent.

let FEATURE = "modules/dasImgui/examples/features/tree_node_open_manual.das"

[test]
def test_tree_node_open_manual_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/LEAD", 15.0f)
        t |> success(snap != null, "LEAD registers")
        if (snap == null) return

        // Same-line sibling rendered AFTER the imperative tree_node_open
        // header — proves the imperative didn't unbalance the ID stack.
        let reset = find_widget(snap, "MAIN_WIN/SETTINGS_RESET")
        t |> equal(reset?["kind"] ?? "", "button",
                   "SETTINGS_RESET registers as 'button'")

        // Trailing widgets after the optional `tree_pop()`.
        t |> success(widget_exists(snap, "MAIN_WIN/SEP1"),  "SEP1 registers")
        t |> success(widget_exists(snap, "MAIN_WIN/NOTE"),  "NOTE registers")

        // Closed-state — body widgets gated on `if (node_open)` do NOT
        // register. (tree_node_open default is closed; no DefaultOpen flag.)
        t |> success(!widget_exists(snap, "MAIN_WIN/BODY_HINT"),
                     "BODY_HINT absent while node closed")
        t |> success(!widget_exists(snap, "MAIN_WIN/BODY_LINE_1"),
                     "BODY_LINE_1 absent while node closed")
        t |> success(!widget_exists(snap, "MAIN_WIN/BODY_LINE_2"),
                     "BODY_LINE_2 absent while node closed")
    }
}
