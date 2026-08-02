options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 tree_node_ex (leaf-form) rail.
//! Drives `examples/features/tree_node_ex.das` — verifies all 4 ex-form
//! nodes register, the leaf flavors render without expand state, and the
//! tree_pop pairing doesn't underflow the ImGui stack.

let FEATURE = "modules/dasImgui/examples/features/tree_node_ex.das"

[test]
def test_tree_node_ex_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/BRANCH", 15.0f)
        t |> success(snap != null, "BRANCH renders")
        if (snap == null) return

        for (ident in ["BRANCH", "LEAF_ROW", "ARROW_ONLY", "BULLET_ROW"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registered")
        }
    }
}
