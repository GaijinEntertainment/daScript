options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: with_font — Push/PopFont scope wrapper.
// SHOWS:   `with_font(font_ptr) { ... }` switches the active font for every
//          glyph drawn inside. Without a custom font atlas loaded, the demo
//          uses GetFontDefault() — visual change is null but the API surface
//          and AOT shape are exercised. Replace `font_ptr` with a real
//          PushFont target when integrating a multi-font app.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/font_stack.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/font_stack.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/font_stack.das
// =============================================================================

[export]
def init() {
    harness_init("with_font — boost v2 §3.6", 720, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 300.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "with_font", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "with_font(GetFont()) is a no-op visually but exercises Push/PopFont."))

        let default_font = GetFont()
        with_font(default_font) {
            text(INSIDE, (text = "inside with_font block — same glyphs since the font matches."))
            if (button(BTN_INSIDE, (text = "button under custom font"))) {}
        }

        separator(SEP)
        text(NOTE, (text = "When a real second font is loaded, swap default_font with the new ImFont? handle."))
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
