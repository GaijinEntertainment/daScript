options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: foundation — Phase 0b.0 [widget] foundation contract.
// SHOWS:   auto-emit (no pre-declared SAVE_BTN/SPEED/VOLUME globals) +
//          dotted-suffix flags (.PUBLIC, .NOTLIVE) + lazy serializer (per-kind
//          payload via g_serializers, materialized on imgui_snapshot only) +
//          unified L2/L3 dispatch (one g_dispatchers table for every kind).
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"MAIN_WIN/SAVE_BTN"}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/SPEED","value":7}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/VOLUME","value":0.5}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/foundation.das
// LIVE:       daslang-live modules/dasImgui/examples/features/foundation.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 0b.0 — foundation", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 320.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Foundation — phase 0b.0", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Three [widget] decls — every global below is auto-emitted.")

        // Button — return form. Identifier `SAVE_BTN` auto-emits the module-scope
        // `var SAVE_BTN : ClickState` global on first compile; `(text = "Save")` is
        // the named-tuple form the [widget] call_macro destructures to positionals.
        if (button(SAVE_BTN, (text = "Save"))) {
            print("save clicked, count={SAVE_BTN.click_count}\n")
        }
        text("save clicks: {SAVE_BTN.click_count}")

        // Slider int — emits SliderStateInt SPEED, marked PUBLIC so a sibling
        // test module can read `SPEED.value` across the module boundary.
        slider_int(SPEED.PUBLIC, (text = "Speed (int)"))
        text("speed = {SPEED.value}")

        // Slider float — emits SliderStateFloat VOLUME without @live; the
        // source-side initial value wins on reload (no preservation).
        slider_float(VOLUME.NOTLIVE, (text = "Volume (float)"))
        text("volume = {VOLUME.value}")
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
