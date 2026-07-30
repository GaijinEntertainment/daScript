options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: hex_id_click — Phase 2.3 hex_id-targeted dispatch.
// SHOWS:   imgui_click resolves "0x..." hex strings via g_id_to_path. The
//          path lookup hits → dispatch goes through the L2 path the widget
//          was registered under, so click_count increments just like a
//          name-targeted click. Hex_ids without a path fall to L1 click
//          synthesis (out-of-band; not exercised here — see classify_target
//          comments for the L1 fallback contract).
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals."MAIN_WIN/DEMO_BTN".hex_id'    # → 2749588296
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"0xa3d68f08"}}' \
//              localhost:9090/command                    # → {"ok":true,"value":"MAIN_WIN/DEMO_BTN", ...}
// STANDALONE: daslang.exe modules/dasImgui/examples/features/hex_id_click.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/hex_id_click.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/hex_id_click.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 2.3 — hex_id_click", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 220.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "hex_id_click — phase 2.3", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Click via name or hex_id: imgui_click resolves both forms.")
        text("Hex strings ('0x...') reverse-map through g_id_to_path to L2.")
        if (button(DEMO_BTN, (text = "Click me"))) {
            print("DEMO_BTN clicked, count={DEMO_BTN.click_count}\n")
        }
        text("clicks: {DEMO_BTN.click_count}")
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
