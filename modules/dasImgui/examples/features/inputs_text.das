options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require strings
require math

// =============================================================================
// FEATURE: inputs_text — Phase 0b.4 InputText family. Buffer-as-pointer path:
//          state owns `array<uint8> buffer` + a string mirror; the C++ side
//          does NO mirror-struct marshaling. `input_text_growable` plumbs
//          ImGui's CallbackResize through a daslang lambda + stack-thunk.
//          `combo_getter` exercises the same trampoline pattern for combos.
// SHOWS:   input_text / input_text_with_hint / input_text_multiline /
//          input_text_growable / combo_getter
// DRIVE:   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/NAME","value":"hello"}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/BIO","value":"line\nbreak"}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/GROW","value":"............................................................"}}' \
//               localhost:9090/command   (forces a CallbackResize past 256B)
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/FONT","value":2}}' \
//               localhost:9090/command   (combo_getter index)
// STANDALONE: daslang.exe modules/dasImgui/examples/features/inputs_text.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/inputs_text.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/inputs_text.das
// =============================================================================

let FONTS = ["Sans", "Serif", "Mono", "Cursive", "Display"]

[export]
def init() {
    harness_init("Phase 0b.4 — text inputs", 760, 620)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 580.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Text inputs — phase 0b.4", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("input_text — fixed 256-byte buffer")
        separator()
        input_text(NAME, (text = "Name"))
        text("name = '{NAME.value}'")

        spacing()
        text("input_text_with_hint — placeholder when empty")
        separator()
        input_text_with_hint(EMAIL, (text = "Email", hint = "you@example.com"))
        text("email = '{EMAIL.value}'")

        spacing()
        text("input_text_multiline — CR/LF tolerant")
        separator()
        input_text_multiline(BIO, (text = "Bio", size = float2(0.0f, 90.0f)))
        text("bio length = {length(BIO.value)} bytes")

        spacing()
        text("input_text_growable — CallbackResize grows buffer past 256B")
        separator()
        input_text_growable(GROW, (text = "Free-form"))
        text("capacity = {GROW.capacity}, length = {length(GROW.value)} bytes")

        spacing()
        text("combo_getter — procedural list (no array<string>)")
        separator()
        combo_getter(FONT, (text = "Font", items_count = length(FONTS),
            getter = @(idx : int; var result : string&) : bool {
                if (idx >= 0 && idx < length(FONTS)) {
                    result = FONTS[idx]
                    return true
                }
                return false
            }))
        text("font index = {FONT.value} ('{FONTS[clamp(FONT.value, 0, length(FONTS) - 1)]}')")

        spacing()
        text("imgui_force_set with target=MAIN_WIN/NAME (or EMAIL/BIO/GROW) + string value writes the buffer")
        text("imgui_force_set with target=MAIN_WIN/FONT + int value picks the dropdown row")
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    //! Standalone entrypoint. daslang-live drives init/update/shutdown
    //! directly and ignores main(); plain `daslang.exe <file>` runs this loop.
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
