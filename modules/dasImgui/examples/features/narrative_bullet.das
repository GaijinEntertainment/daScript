options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: narrative_bullet — bullet + bullet_text widgets (boost v2 §3.2).
// SHOWS:   bullet() emits an EmptyMarkerState global (zero-payload, kind="bullet")
//          + bullet_text() emits a NarrativeState with the rendered copy.
//          Together they cover both the standalone marker form (after which
//          `Text` continues on the same line) and the labelled form. Useful
//          for any feature-list / change-log copy that needs telemetry.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals."MAIN_WIN/BULLET_LIST_FAST"'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/narrative_bullet.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/narrative_bullet.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/narrative_bullet.das
// =============================================================================

[export]
def init() {
    harness_init("narrative_bullet — boost v2 §3.2", 720, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 300.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "bullet + bullet_text", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        bullet_text(BULLET_LIST_FAST, (text = "bullet_text(IDENT, (text=...)) — bullet glyph + text together, single telemetry slot."))
        bullet_text(BULLET_LIST_TYPED, (text = "Payload echoes the rendered string under the value field; same shape as text() / text_wrapped()."))
        bullet_text(BULLET_LIST_RELOAD, (text = "Live-reload safe: NarrativeState carries no @live fields beyond the echoed copy."))

        // Standalone bullet marker — emit bullet, then continue with Text on
        // the SAME line. State is empty; payload is `{}`. Use when you need
        // bullet-glyph-then-formatted-line (e.g. inline counters).
        bullet(MARKER_ONE)
        text("plain text after bullet marker — telemetry attaches via auto-emit")
        bullet(MARKER_TWO)
        text("second marker, second tail")
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
