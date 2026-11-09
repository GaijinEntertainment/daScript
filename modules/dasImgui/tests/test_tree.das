options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Tree nodes and collapsing headers: forms, open state, stacks.

let TREE_NODE_EX_FEATURE = "modules/dasImgui/examples/features/tree_node_ex.das"

[test(description = "Branch, leaf, arrow-only and bullet tree_node_ex forms all register, and their tree_pop pairing renders without underflowing the stack.", timeout = 1200)]
def test_tree_node_ex_smoke(t : T?) {
    with_imgui_app(TREE_NODE_EX_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/BRANCH", 15.0f)
        t |> success(snap != null, "BRANCH renders")
        if (snap == null) return

        for (ident in ["BRANCH", "LEAF_ROW", "ARROW_ONLY", "BULLET_ROW"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registered")
        }
    }
}

let TREE_OPEN_MANUAL_FEATURE = "modules/dasImgui/examples/features/tree_node_open_manual.das"

[test(description = "A closed manual tree_node_open / tree_pop pair keeps the ID stack balanced for its siblings and registers none of its body widgets.", timeout = 1200)]
def test_tree_node_open_manual_smoke(t : T?) {
    with_imgui_app(TREE_OPEN_MANUAL_FEATURE) $(d) {
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

let CLOSABLE_HEADER_FEATURE = "modules/dasImgui/examples/features/collapsing_header_closable.das"

[test(description = "A closable collapsing_header starts open, imgui_close hides its body, and imgui_open paints the body again.", timeout = 1200)]
def test_collapsing_header_closable_smoke(t : T?) {
    with_imgui_app(CLOSABLE_HEADER_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/HEADER_CLOSE", 15.0f)
        t |> success(snap != null, "HEADER_CLOSE renders open")
        if (snap == null) return

        let open0 = widget_payload_field(snap, "MAIN_WIN/HEADER_CLOSE", "open") ?? false
        t |> equal(open0, true, "header starts open")

        close_widget(d, "MAIN_WIN/HEADER_CLOSE")
        let closed = wait_until(d, 300) $(var s) {
            return !widget_exists(s, "MAIN_WIN/HEADER_CLOSE/REVEAL_BTN")
        }
        t |> success(closed != null, "closing the header hides REVEAL_BTN inside")

        // Re-open must gate on actual paint, not stale registry existence:
        // widget_exists returns true on the expunged-but-cached entry. wait_for_visible
        // polls widget_rendered (registered AND painted this frame).
        open_widget(d, "MAIN_WIN/HEADER_CLOSE")
        t |> success(wait_for_visible(d, "MAIN_WIN/HEADER_CLOSE/REVEAL_BTN", 5.0f) != null,
                     "imgui_open re-renders body")
    }
}
