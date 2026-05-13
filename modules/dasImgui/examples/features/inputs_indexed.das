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
require imgui/imgui_containers_builtin

// =============================================================================
// FEATURE: inputs_indexed — table-keyed slider widgets per API_REWORK §4.4.
// SHOWS:   `slider_float(CHANNEL_VOL[i], ...)` (int-keyed),
//          `slider_float(TRACK_VOL[name], ...)` (string-keyed).
// DRIVE:   curl -s -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          # snapshot.globals has "MIX_WIN/CHANNEL_VOL[3]" and "MIX_WIN/TRACK_VOL[lead]"
//          curl -s -X POST -d '{"name":"imgui_set","args":{
//               "target":"MIX_WIN/CHANNEL_VOL[3]","value":0.7}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/inputs_indexed.das
// LIVE:       daslang-live modules/dasImgui/examples/features/inputs_indexed.das
// =============================================================================

// Tables are user-declared; the [widget] macro detects subscript form and
// emits a per-(kind, IDENT) wrapper that registers each rendered key into
// g_widgets on first touch. Bounds/initial value are populated imperatively
// in init() — per-call init=/bounds= sugar is not available on indexed form
// (V1 limitation; see plan §H).
var private CHANNEL_VOL : table<int; SliderStateFloat>
var private TRACK_VOL : table<string; SliderStateFloat>

let TRACK_NAMES : array<string> <- ["bass", "lead", "drum", "vocals"]

[export]
def init() {
    live_create_window("inputs_indexed", 800, 600)
    live_imgui_init(live_window)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 1.5
    // Pre-populate so first-frame snapshot already has the right bounds + value.
    for (i in range(4)) {
        CHANNEL_VOL[i].bounds = (0.0f, 1.0f)
        CHANNEL_VOL[i].value = 0.5f
    }
    for (name in TRACK_NAMES) {
        TRACK_VOL[name].bounds = (0.0f, 1.0f)
        TRACK_VOL[name].value = 0.7f
    }
}

[export]
def update() {
    if (!live_begin_frame()) {
        return
    }
    begin_frame()

    ImGui_ImplOpenGL3_NewFrame()
    ImGui_ImplGlfw_NewFrame()
    NewFrame()

    SetNextWindowSize(ImVec2(720.0, 520.0), ImGuiCond.Always)
    // Use the boost `window` container so indexed leaves get the path
    // prefix MIX_WIN/CHANNEL_VOL[i] in g_widgets — proves container_path
    // composition works for indexed leaves.
    window(MIX_WIN, (text = "MIX_WIN", closable = false, flags = ImGuiWindowFlags.None)) {
        Text("Int-keyed channels (CHANNEL_VOL[0..3]):")
        Separator()
        for (i in range(4)) {
            // Each [i] resolves to a distinct path component CHANNEL_VOL[<i>]
            // — separate g_widgets entry, separate dispatcher, etc.
            slider_float(CHANNEL_VOL[i], (text = "ch{i}"))
        }

        Spacing()
        Text("String-keyed tracks (TRACK_VOL[name]):")
        Separator()
        for (name in TRACK_NAMES) {
            let _slid = slider_float(TRACK_VOL[name], (text = name))
        }
    }

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
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
