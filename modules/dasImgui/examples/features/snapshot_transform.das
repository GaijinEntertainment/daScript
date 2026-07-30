options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require imgui/imgui_boost_runtime

// =============================================================================
// FEATURE: snapshot_transform — exercises the snapshot item-transform stack in
//          isolation (no node-editor needed). A custom canvas would normally push
//          this from its coordinate mapping; here we push a known synthetic affine
//          around ONE widget so a test can assert the contract directly:
//            - widget under the transform: bbox = local*scale + offset, plus a
//              canvas_bbox carrying the original (screen-native) rect;
//            - widgets before the push / after the pop: screen-native bbox, no canvas_bbox.
// DRIVE:   tests/integration/test_snapshot_transform.das
// STANDALONE: daslang.exe modules/dasImgui/examples/features/snapshot_transform.das
// =============================================================================

let XFORM_SCALE  = float2(2.0, 3.0)
let XFORM_OFFSET = float2(100.0, 50.0)

[export]
def init() {
    harness_init("Snapshot item-transform", 640, 480)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    window(MAIN_WIN, (text = "Snapshot item-transform", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        // No active transform — screen-native bbox, no canvas_bbox.
        button(PLAIN_BTN, (text = "plain"))
        // Active transform — interior widget reports screen-space bbox + canvas_bbox.
        push_snapshot_item_transform(XFORM_SCALE, XFORM_OFFSET)
        button(XFORM_BTN, (text = "xform"))
        // A CONTAINER header (tab_item) under the transform: its bbox is captured via
        // current_item_bbox(), so it must also report screen-space, not canvas-local.
        tab_bar(XFORM_TABS, (text = "xform-tabs", flags = ImGuiTabBarFlags.None)) {
            tab_item(XFORM_TAB, (text = "T", closable = false,
                                 flags = ImGuiTabItemFlags.None)) {
                text("body")
            }
        }
        pop_snapshot_item_transform()
        // Popped — back to screen-native, no canvas_bbox.
        button(AFTER_BTN, (text = "after"))
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
