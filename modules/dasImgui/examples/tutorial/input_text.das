options gen2
options _comment_hygiene = true

require imgui
require imgui_app
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_visual_aids
require strings

// =============================================================================
// TUTORIAL: input_text family — text editing widgets.
//
//   input_text(IDENT, (text = "..", flags = ImGuiInputTextFlags....))
//   input_text_with_hint(IDENT, (text, hint, flags))
//   input_text_multiline(IDENT, (text, size = float2(w, h), flags))
//   input_text_growable(IDENT, (text, flags))         — auto-grows buffer
//   input_text_callback(IDENT, text, flags, cb)       — user callback
//   text_filter(IDENT, (text, width)) + passes_filter(IDENT, line)
//
// Six widgets, six modalities sharing one InputTextState struct (text_filter
// uses TextFilterState — different shape, integrates with the family).
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/input_text.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/input_text.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"IT_WIN/IT_NAME","value":"hello"}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"IT_WIN/IT_BIO","value":"line1\nline2"}}' \
//        localhost:9090/command
// =============================================================================

let private COMMANDS <- ["help", "history", "clear", "classify", "echo", "exit"]

let private LOG_LINES <- [
    "[info] startup complete",
    "[warn] cache miss for key=42",
    "[info] connected to api.example.com",
    "[error] failed to write tmp/foo: disk full",
    "[info] saved settings",
    "[debug] frame budget = 16.6ms"
]

//! Indexed-form text widget requires explicit table declaration at module scope.
var private IT_LINE : table<int; NarrativeState>


def private completion_cb(var data : ImGuiInputTextCallbackData) : int {
    if (data.EventFlag == ImGuiInputTextFlags.CallbackCompletion) {
        let prefix = clone_string(data.Buf)
        var matches <- [for (c in COMMANDS); c; where c |> starts_with(prefix)]
        if (length(matches) == 1) {
            data |> DeleteChars(0, data.BufTextLen)
            data |> InsertChars(0, matches[0])
        }
        delete matches
    }
    return 0
}

[export]
def init() {
    live_create_window("dasImgui input_text tutorial", 760, 760)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.4
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(720.0f, 720.0f), ImGuiCond.Always)
    window(IT_WIN, (text = "input_text tutorial", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        // ---- Stage 1: basic input_text ----
        input_text(IT_NAME, (text = "name"))
        text("IT_NAME.value = '{IT_NAME.value}'  (capacity={IT_NAME.capacity})")
        spacing()

        // ---- Stage 2: with_hint — placeholder when empty ----
        input_text_with_hint(IT_EMAIL, (text = "email",
                                        hint = "you@example.com"))
        text("IT_EMAIL.value = '{IT_EMAIL.value}'")
        spacing()

        // ---- Stage 3: multiline — CR/LF tolerant, sized box ----
        input_text_multiline(IT_BIO, (text = "bio",
                                      size = float2(0.0f, 90.0f)))
        text("IT_BIO length = {length(IT_BIO.value)} bytes")
        spacing()

        // ---- Stage 4: growable — buffer expands past `capacity` automatically ----
        input_text_growable(IT_FREE, (text = "free-form"))
        text("IT_FREE capacity = {IT_FREE.capacity}, length = {length(IT_FREE.value)}")
        spacing()

        // ---- Stage 5: callback — TAB completes against COMMANDS dictionary ----
        text("Type a prefix (h/cla/ex) and press TAB:")
        if (input_text_callback(IT_CMD, "command",
                                ImGuiInputTextFlags.CallbackCompletion,
                                @(var data : ImGuiInputTextCallbackData) =>
                                    completion_cb(data))) {}
        spacing()
        separator()

        // ---- Stage 6: text_filter — incl,-excl token filter ----
        text("text_filter - type 'info' or '-debug' to filter the log:")
        text_filter(IT_FILTER)
        for (i in range(length(LOG_LINES))) {
            if (passes_filter(IT_FILTER, LOG_LINES[i])) {
                text(IT_LINE[i], (text = LOG_LINES[i]))
            }
        }
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    live_imgui_render()

    live_end_frame()
}

[export]
def shutdown() {
    live_imgui_shutdown()
    live_destroy_window()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
