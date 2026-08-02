options gen2
options _comment_hygiene = true
options gc
options persistent_heap

require imgui/imgui_harness

// =============================================================================
// TUTORIAL: application_lifecycle - one source file for standalone and live.
//
// The lifecycle exports are owned by daslang-live when hosted. main() owns the
// same lifecycle in standalone mode. After update() returns, main calls the
// harness GC helper: frame locals are dead, and the helper is a no-op when the
// same source is hosted by daslang-live.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/application_lifecycle.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/application_lifecycle.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/tutorial/application_lifecycle.das -- --headless --headless-frames=600
// =============================================================================

var private g_frames = 0

[export]
def init() {
    harness_init("dasImgui application lifecycle", 760, 440)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowPos(ImVec2(40.0f, 40.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(680.0f, 340.0f), ImGuiCond.Always)
    window(APPLICATION_WIN, (text = "Application lifecycle", closable = false,
                             flags = ImGuiWindowFlags.None)) {
        text("init -> update* -> shutdown")
        separator()
        text("frame {g_frames}")
        text("The same file runs under daslang and daslang-live.")
        text("Standalone GC runs only between completed frames.")
        spacing(APP_SPACING)
        if (button(QUIT_BUTTON, (text = "Quit"))) {
            request_exit()
        }
    }

    harness_end_frame()
    g_frames++
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
        // Safe boundary: update returned, so no frame-local collectable value
        // remains live on the JIT stack. The live host collects for us.
        harness_maybe_collect_gc()
    }
    shutdown()
}
