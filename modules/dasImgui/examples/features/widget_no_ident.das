options gen2
options _comment_hygiene = true
// This demo deliberately exercises the Form 2 named-tuple-without-ident shape
// that STYLE001 forbids on single-arg narrative widgets like `text`. Opt out
// per-file so the educational `text((text=...))` line below renders instead
// of becoming a hard build error.
options _allow_imgui_legacy = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: widget_no_ident — smoke of the three [widget] call forms.
// SHOWS:   1. EXPLICIT  text(IDENT, (text=...))     — leading ident
//          2. AUTO-GEN  text((text=...))            — first arg is named-tuple
//          3. POSITIONAL text("string")             — single-string sugar
// STANDALONE: daslang.exe modules/dasImgui/examples/features/widget_no_ident.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/widget_no_ident.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/widget_no_ident.das
// =============================================================================

[export]
def init() {
    harness_init("widget_no_ident smoke", 720, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(640.0, 300.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "no-ident forms",
                      closable = false, flags = ImGuiWindowFlags.None)) {
        // Form 1: explicit ident.
        text(EXPLICIT_HELLO, (text = "explicit ident form"))

        // Form 2: no-ident named-tuple. Synthesized state global is internal;
        // user can't read .value (not that text has a useful one anyway).
        text((text = "no-ident named-tuple form"))

        // Form 3: positional-string sugar (single-string text family only).
        text("positional-string form")
        text_unformatted("text_unformatted positional form")
        text_wrapped("text_wrapped positional form — long enough to wrap and exercise the auto-emit across a reflow")
        text_disabled("text_disabled positional form")
        bullet_text("bullet_text positional form")
        separator_text("separator_text positional form")
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
