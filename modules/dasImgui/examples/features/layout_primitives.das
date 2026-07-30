options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: layout_primitives — same_line / new_line / spacing / dummy.
// SHOWS:   Four EmptyMarkerState siblings of separator. Each emits a `{}`
//          payload under its IDENT so playwright can assert presence and
//          ordering without needing payload content. `dummy(size=...)` is
//          the only one that's argument-bearing — useful for grid spacers.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals|keys|map(select(startswith("MAIN_WIN/LAY_")))'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/layout_primitives.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/layout_primitives.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/layout_primitives.das
// =============================================================================

[export]
def init() {
    harness_init("layout_primitives — boost v2 §3.2", 720, 420)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 360.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "same_line / new_line / spacing / dummy",
                      closable = false, flags = ImGuiWindowFlags.None)) {
        text(LAY_LEAD, (text = "Four EmptyMarkerState siblings. Each is telemetry-visible."))

        // same_line — explicit horizontal continuation
        if (button(BTN_A, (text = "A"))) {}
        same_line(LAY_SAME_LINE)
        if (button(BTN_B, (text = "B"))) {}
        same_line(LAY_SAME_LINE_2, (offset_x = 0.0f, spacing = 20.0f))
        if (button(BTN_C, (text = "C"))) {}

        spacing(LAY_SPACING_ROW)

        // dummy — zero-content rectangle that consumes layout space
        text(LAY_DUMMY_PREV, (text = "before dummy"))
        dummy(LAY_DUMMY, (size = float2(60.0f, 30.0f)))
        text(LAY_DUMMY_NEXT, (text = "after dummy (offset by 60×30)"))

        new_line(LAY_NEW_LINE)
        text(LAY_AFTER_NEW, (text = "after new_line — guaranteed fresh row."))
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
