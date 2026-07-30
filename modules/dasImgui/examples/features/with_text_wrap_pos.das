options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: with_text_wrap_pos — stateless block-arg wrapper around
//          PushTextWrapPos/PopTextWrapPos. Scoped wrap-x override for long
//          text passed to TextWrapped or TextUnformatted.
// SHOWS:   default wrap (window right edge), narrow wrap (200 px), wider
//          wrap (400 px).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/with_text_wrap_pos.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/with_text_wrap_pos.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/with_text_wrap_pos.das
// =============================================================================

let LIPSUM = ("Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
              + "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.")

[export]
def init() {
    harness_init("with_text_wrap_pos — scope wrapper", 720, 540)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(660.0, 480.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "with_text_wrap_pos", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Default wrap (0.0f → window right edge):")
        with_text_wrap_pos(0.0f) {
            text_unformatted((text = LIPSUM))
        }

        separator()
        text("Wrap at 200 px:")
        with_text_wrap_pos(200.0f) {
            text_unformatted((text = LIPSUM))
        }

        separator()
        text("Wrap at 400 px:")
        with_text_wrap_pos(400.0f) {
            text_unformatted((text = LIPSUM))
        }
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
