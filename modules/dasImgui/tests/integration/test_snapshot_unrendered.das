options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 1.1 — long-lived registry contract.
//!
//! Drives `examples/features/snapshot_unrendered.das` to verify:
//!   1. VISIBLE_VAL appears in `globals` AND has a bbox (rendered this frame).
//!   2. NEVER_RENDERED appears in `globals` even though its render path is
//!      never reached at runtime — it's a private function never called.
//!      The [widget] macro registered it at module init via
//!      register`imgui`widgets() / register_widget().
//!   3. NEVER_RENDERED has `rendered:false` and a payload (state struct
//!      readable through the captured serializer lambda).

let SNAPSHOT_UNRENDERED_FEATURE = "modules/dasImgui/examples/features/snapshot_unrendered.das"

[test]
def test_unrendered_widget_visible(t : T?) {
    with_imgui_app(SNAPSHOT_UNRENDERED_FEATURE) $(d) {
        var snap = wait_for_widget(d, "VISIBLE_VAL", 15.0f)
        t |> success(snap != null, "VISIBLE_VAL appears in registry")
        if (snap == null) return

        // Gate 1: VISIBLE_VAL is rendered (has bbox).
        t |> success(widget_rendered(snap, "VISIBLE_VAL"),
                     "VISIBLE_VAL has bbox — rendered this frame")
        var visible = find_widget(snap, "VISIBLE_VAL")
        t |> equal(visible?["kind"] ?? "", "slider_float",
                   "VISIBLE_VAL.kind")
        t |> success(visible?["payload"]?["value"] != null,
                     "VISIBLE_VAL.payload.value present")

        // Gate 2: NEVER_RENDERED appears at all — proves [init] registration
        // fires for widgets whose render path is unreachable at runtime.
        t |> success(widget_exists(snap, "NEVER_RENDERED"),
                     "NEVER_RENDERED appears in g_widgets via [init], not via render")

        // Gate 3: NEVER_RENDERED carries rendered:false and a state payload.
        var hidden = find_widget(snap, "NEVER_RENDERED")
        t |> success(!widget_rendered(snap, "NEVER_RENDERED"),
                     "NEVER_RENDERED has rendered:false — not rendered this frame")
        t |> equal(hidden?["rendered"] ?? true, false,
                   "NEVER_RENDERED.rendered == false")
        t |> equal(hidden?["kind"] ?? "", "slider_float",
                   "NEVER_RENDERED.kind preserved through serializer")
        t |> success(hidden?["payload"]?["value"] != null,
                     "NEVER_RENDERED.payload.value still readable via captured serializer")
    }
}
