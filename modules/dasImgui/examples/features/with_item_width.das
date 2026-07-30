options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: with_item_width — stateless block-arg wrapper around
//          PushItemWidth/PopItemWidth. Scoped widget-width override that
//          auto-restores on block exit.
// SHOWS:   nested scopes; positive (absolute), negative (right-edge minus N),
//          and default style width contrasted side-by-side.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/with_item_width.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/with_item_width.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/with_item_width.das
// =============================================================================

[export]
def init() {
    harness_init("with_item_width — scope wrapper", 720, 480)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(660.0, 420.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "with_item_width", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Default width (ImGui style):")
        slider_float(DEFAULT_W, (text = "default", bounds = (0.0f, 1.0f)))

        separator()
        text("Absolute 120 px:")
        with_item_width(120.0f) {
            slider_float(NARROW_W, (text = "120px", bounds = (0.0f, 1.0f)))
        }

        text("Absolute 320 px:")
        with_item_width(320.0f) {
            slider_float(WIDE_W, (text = "320px", bounds = (0.0f, 1.0f)))
        }

        separator()
        text("Right-edge minus 60 px (negative):")
        with_item_width(-60.0f) {
            slider_float(STRETCH_W, (text = "stretch", bounds = (0.0f, 1.0f)))
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
