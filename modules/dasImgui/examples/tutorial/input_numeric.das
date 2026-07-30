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

// =============================================================================
// TUTORIAL: input_numeric widgets — type the number, optionally with
// +/- step buttons.
//
//   input_float(IDENT, (text = "..", step = 0.0f, step_fast = 0.0f,
//                       format = "%.3f", flags = ImGuiInputTextFlags....))
//   input_int(IDENT, (text = "..", step = 1, step_fast = 100,
//                     flags = ImGuiInputTextFlags....))
//   input_double / input_float2/3/4 / input_int2/3/4 — same shape;
//   scalar / vector / double-precision variants.
//
// Scalar forms get step / step_fast args — non-zero shows ImGui's +/-
// buttons, click = step, ctrl-click = step_fast. Vector forms (2/3/4)
// have no step args — the buttons would crowd the row.
//
// Default int step = 1 / step_fast = 100; float defaults are 0.0 (no
// buttons). input_double defaults to 0.0lf (no buttons) with format
// "%.6f" for the extra precision.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/input_numeric.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/input_numeric.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"IN_WIN/I_FLOAT","value":12.5}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"IN_WIN/I_VEC3","value":[1.0,2.0,3.0]}}' \
//        localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui input_numeric tutorial", 760, 560)
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
    SetNextWindowSize(ImVec2(720.0f, 520.0f), ImGuiCond.Always)
    window(IN_WIN, (text = "input_numeric tutorial", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        text("Click to focus, type the number, Enter to commit.")
        text(I_HINT, (text = "Scalar forms have step / step_fast - non-zero shows +/- buttons."))
        separator()

        // ---- Stage 1: scalar float with step buttons ----
        input_float(I_FLOAT, (text = "mass (kg)",
                              step = 0.1f, step_fast = 1.0f,
                              format = "%.3f"))
        text("I_FLOAT.value = {I_FLOAT.value}")
        spacing()

        // ---- Stage 2: scalar int with step buttons ----
        input_int(I_INT, (text = "level",
                          step = 1, step_fast = 100))
        text("I_INT.value = {I_INT.value}")
        spacing()

        // ---- Stage 3: vector float3 — no step buttons (vector forms omit them) ----
        input_float3(I_VEC3, (text = "position", format = "%.2f"))
        text("I_VEC3.value = ({I_VEC3.value.x}, {I_VEC3.value.y}, {I_VEC3.value.z})")
        spacing()

        // ---- Stage 4: double precision ----
        input_double(I_DOUBLE, (text = "epoch (s)",
                                step = 1.0lf, step_fast = 60.0lf,
                                format = "%.6f"))
        text("I_DOUBLE.value = {I_DOUBLE.value}")
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
