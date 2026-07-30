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
// TUTORIAL: drag widgets — click-and-scrub numeric editing.
//
//   drag_float(IDENT, (text = "..", speed = 0.01f, format = "%.3f",
//                      flags = ImGuiSliderFlags....))
//   drag_int / drag_float2/3/4 / drag_int2/3/4 / drag_float_range2 /
//   drag_int_range2 — same shape; scalar / vector / range variants.
//
// Bounds live on the state struct (`IDENT.bounds = (lo, hi)`), set once per
// frame from app code. Zero-init = unclamped (drag scrubs to any value).
// `speed` is units-per-pixel of horizontal drag; `format` is the printf-style
// label format.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/drag.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/drag.das
//
// DRIVE (when running live):
//   # Set scalar value directly:
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRAG_WIN/D_FLOAT","value":0.75}}' \
//        localhost:9090/command
//   # Set vector value (one number per component):
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRAG_WIN/D_VEC3","value":[1.0,2.0,3.0]}}' \
//        localhost:9090/command
//   # Set range (lo, hi):
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRAG_WIN/D_RANGE","value":[10.0,40.0]}}' \
//        localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui drag tutorial", 760, 520)
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
    SetNextWindowSize(ImVec2(720.0f, 480.0f), ImGuiCond.Always)
    window(DRAG_WIN, (text = "drag tutorial", closable = false,
                      flags = ImGuiWindowFlags.None)) {

        text("Click and drag horizontally to scrub the value.")
        text(D_HINT, (text = "Bounds set via IDENT.bounds = (lo, hi). Zero-init = unclamped."))
        separator()

        // ---- Stage 1: scalar float, clamped 0..1 ----
        D_FLOAT.bounds = (0.0f, 1.0f)
        drag_float(D_FLOAT, (text = "opacity", speed = 0.005f, format = "%.3f"))
        text("D_FLOAT.value = {D_FLOAT.value}")
        spacing()

        // ---- Stage 2: scalar int ----
        D_INT.bounds = (0, 100)
        drag_int(D_INT, (text = "score", speed = 1.0f, format = "%d"))
        text("D_INT.value = {D_INT.value}")
        spacing()

        // ---- Stage 3: vector — three components on one row ----
        D_VEC3.bounds = (-10.0f, 10.0f)
        drag_float3(D_VEC3, (text = "position", speed = 0.05f, format = "%.2f"))
        text("D_VEC3.value = ({D_VEC3.value.x}, {D_VEC3.value.y}, {D_VEC3.value.z})")
        spacing()

        // ---- Stage 4: range — paired min/max scrubbing, lo <= hi enforced ----
        D_RANGE.bounds = (0.0f, 100.0f)
        drag_float_range2(D_RANGE, (text = "band", speed = 0.5f, format = "%.1f"))
        text("D_RANGE.value = [{D_RANGE.value.x}, {D_RANGE.value.y}]")
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
