options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require app_small

// =============================================================================
// HARNESS: imgui_demo app_small.ShowExampleAppPropertyEditor — single-scene driver.
// SHOWS:   data_table (2-col split) + recursive tree_node_ex rows + per-row
//          input_float / drag_float widgets via the placeholder member array.
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_property_editor.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_property_editor.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_app_small_property_editor.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - app_small PropertyEditor", 540, 540)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    ShowExampleAppPropertyEditor()
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
