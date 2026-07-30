options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require app_documents

// =============================================================================
// HARNESS: imgui_demo Example: Documents — single-scene driver for the
//          C++ ShowExampleAppDocuments port (Target_Tab mode only). The
//          function opens its own window (MenuBar + tab bar + per-doc tab
//          + close-confirmation popup_modal).
// SHOWS:   DOCS_WIN host with File menu, per-doc tabs, close confirmation.
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_documents.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_documents.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_app_documents.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - Documents", 760, 540)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    app_documents::ShowExampleAppDocuments()
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
