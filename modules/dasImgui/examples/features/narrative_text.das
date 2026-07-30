options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: narrative_text — telemetry-bearing text widgets (boost v2 §3.2).
// SHOWS:   text / text_unformatted / text_wrapped / text_colored / text_disabled
//          all auto-emit NarrativeState globals and serialize through to
//          imgui_snapshot. Each call site shows up under MAIN_WIN/<IDENT> with
//          payload {value: "<echoed text>"} — the precondition for any
//          playwright test that needs to assert on the rendered copy. The raw
//          `Text(s)` form has NO telemetry and is invisible to imgui_snapshot;
//          this demo's whole point is the boost wrappers above.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals."MAIN_WIN/NARRATIVE_LEAD"'
//          # → {"payload":{"value":"Telemetry-bearing narrative…"}, "kind":"text", ...}
// STANDALONE: daslang.exe modules/dasImgui/examples/features/narrative_text.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/narrative_text.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/narrative_text.das
// =============================================================================

[export]
def init() {
    harness_init("narrative_text — boost v2 §3.2", 760, 460)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 400.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "narrative widgets", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(NARRATIVE_LEAD, (text = "Telemetry-bearing narrative — each line emits its rendered string back as payload value under its IDENT."))
        text_unformatted(NARRATIVE_UNFORMATTED, (text = "text_unformatted: %s passes through literally — no printf parse."))
        text_wrapped(NARRATIVE_WRAPPED, (text = "text_wrapped soft-wraps at the window edge instead of clipping. Useful for any paragraph copy that needs to flow with the user's column width — release notes, tooltips, help panels."))
        text_colored(NARRATIVE_COLORED, (color = float4(0.95f, 0.65f, 0.20f, 1.0f), text = "text_colored: per-call color, payload mirrors the rendered string."))
        text_disabled(NARRATIVE_DISABLED, (text = "text_disabled: dimmed style for caption / hint copy."))
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
