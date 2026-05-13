options gen2

require imgui
require imgui_app
require glfw/glfw_boost
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live/opengl_live
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
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
// DRIVE:   curl -X POST -d '{"name":"imgui_set","args":{"target":"NAME","value":"hello"}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_set","args":{"target":"BIO","value":"line\nbreak"}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_set","args":{"target":"GROW","value":"............................................................"}}' \
//               localhost:9090/command   (forces a CallbackResize past 256B)
//          curl -X POST -d '{"name":"imgui_set","args":{"target":"FONT","value":2}}' \
//               localhost:9090/command   (combo_getter index)
// STANDALONE: daslang.exe modules/dasImgui/examples/features/inputs_text.das
// LIVE:       daslang-live modules/dasImgui/examples/features/inputs_text.das
// =============================================================================

let FONTS = ["Sans", "Serif", "Mono", "Cursive", "Display"]

[export]
def init() {
    live_create_window("Phase 0b.4 — text inputs", 760, 620)
    live_imgui_init(live_window)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 1.5
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplOpenGL3_NewFrame()
    ImGui_ImplGlfw_NewFrame()
    NewFrame()

    SetNextWindowSize(ImVec2(720.0, 580.0), ImGuiCond.Always)
    if (Begin("Text inputs — phase 0b.4", null, ImGuiWindowFlags.None)) {
        Text("input_text — fixed 256-byte buffer")
        Separator()
        input_text(NAME, (text = "Name"))
        Text("name = '{NAME.value}'")

        Spacing()
        Text("input_text_with_hint — placeholder when empty")
        Separator()
        input_text_with_hint(EMAIL, (text = "Email", hint = "you@example.com"))
        Text("email = '{EMAIL.value}'")

        Spacing()
        Text("input_text_multiline — CR/LF tolerant")
        Separator()
        input_text_multiline(BIO, (text = "Bio", size = float2(0.0f, 90.0f)))
        Text("bio length = {length(BIO.value)} bytes")

        Spacing()
        Text("input_text_growable — CallbackResize grows buffer past 256B")
        Separator()
        input_text_growable(GROW, (text = "Free-form"))
        Text("capacity = {GROW.capacity}, length = {length(GROW.value)} bytes")

        Spacing()
        Text("combo_getter — procedural list (no array<string>)")
        Separator()
        combo_getter(FONT, (text = "Font", items_count = length(FONTS),
            getter = @(idx : int; var result : string&) : bool {
                if (idx >= 0 && idx < length(FONTS)) {
                    result = FONTS[idx]
                    return true
                }
                return false
            }))
        Text("font index = {FONT.value} ('{FONTS[clamp(FONT.value, 0, length(FONTS) - 1)]}')")

        Spacing()
        Text("imgui_set with target=NAME/EMAIL/BIO/GROW + string value writes the buffer")
        Text("imgui_set with target=FONT + int value picks the dropdown row")
    }
    End()

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData())

    live_end_frame()
}

[export]
def shutdown() {
    live_imgui_shutdown()
    live_destroy_window()
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
