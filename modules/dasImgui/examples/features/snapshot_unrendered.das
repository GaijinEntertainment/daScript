options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: snapshot_unrendered — Phase 1.1 long-lived registry contract.
// SHOWS:   widgets registered at module init via setup_call_list appear in
//          imgui_snapshot even when their render path never executes. The
//          [widget] macro auto-emits state globals + register_widget calls
//          into an [init] private register`imgui`widgets() function, so
//          unreachable widget call sites still populate g_widgets.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          → globals contains both MAIN_WIN/VISIBLE_VAL (rendered, has bbox)
//          and NEVER_RENDERED (rendered=false, no bbox, payload still present).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/snapshot_unrendered.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/snapshot_unrendered.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/snapshot_unrendered.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 1.1 — snapshot unrendered", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

// Never called at runtime, but the [widget] macro fires at COMPILE time on the
// slider below — auto-emitting NEVER_RENDERED as a global and appending a
// register_widget(...) line to [init], so it's in g_widgets though never painted.
def private register_only_never_rendered() {
    slider_float(NEVER_RENDERED, (text = "Hidden — never painted"))
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 240.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Phase 1.1 — registry covers unrendered widgets",
                      closable = false, flags = ImGuiWindowFlags.None)) {
        text("VISIBLE_VAL renders every frame. NEVER_RENDERED never does.")
        text("imgui_snapshot must list both, with rendered:false on the second.")

        slider_float(VISIBLE_VAL, (text = "Visible (rendered)"))
        text("VISIBLE_VAL.value = {VISIBLE_VAL.value}")
        text("NEVER_RENDERED.value = {NEVER_RENDERED.value}  (state readable; widget never painted)")
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
