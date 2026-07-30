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
// TUTORIAL: slider widgets — bounded numeric editing along a fixed track.
//
//   slider_float(IDENT, (text = "..", format = "%.3f",
//                        flags = ImGuiSliderFlags....))
//   slider_int / slider_float2/3/4 / slider_int2/3/4 / vslider_float /
//   vslider_int — same shape; scalar / vector / vertical variants.
//
// Bounds REQUIRED — slider has a fixed-width track between
// (state.bounds.min, state.bounds.max). Unlike drag, zero-init bounds
// = (0, 0) collapses the track to a single value.
//
// vslider takes a `size : float2` arg (width, height) and renders the
// track top-to-bottom; otherwise identical to slider_float.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/slider.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/slider.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"SLIDER_WIN/S_FLOAT","value":0.8}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"SLIDER_WIN/S_VEC3","value":[1.0,2.0,3.0]}}' \
//        localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui slider tutorial", 760, 520)
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
    window(SLIDER_WIN, (text = "slider tutorial", closable = false,
                        flags = ImGuiWindowFlags.None)) {

        text("Click anywhere on the track to jump; drag the handle to scrub.")
        text(S_HINT, (text = "Bounds REQUIRED - track spans (lo, hi) literally."))
        separator()

        // ---- Stage 1: scalar float, 0..1 ----
        S_FLOAT.bounds = (0.0f, 1.0f)
        slider_float(S_FLOAT, (text = "volume", format = "%.2f"))
        text("S_FLOAT.value = {S_FLOAT.value}")
        spacing()

        // ---- Stage 2: scalar int, 0..100 ----
        S_INT.bounds = (0, 100)
        slider_int(S_INT, (text = "percent", format = "%d%%"))
        text("S_INT.value = {S_INT.value}")
        spacing()

        // ---- Stage 3: vector — three handles on one row ----
        S_VEC3.bounds = (-1.0f, 1.0f)
        slider_float3(S_VEC3, (text = "rotation", format = "%.2f"))
        text("S_VEC3.value = ({S_VEC3.value.x}, {S_VEC3.value.y}, {S_VEC3.value.z})")
        spacing()

        // ---- Stage 4: vertical orientation — same state, different layout ----
        text("Vertical sliders (vslider_float) share SliderStateFloat:")
        V_LO.bounds = (0.0f, 1.0f)
        V_MID.bounds = (0.0f, 1.0f)
        V_HI.bounds = (0.0f, 1.0f)
        vslider_float(V_LO, (text = "##lo",
                             size = float2(36.0f, 140.0f), format = "%.2f"))
        same_line(SLINE_1)
        vslider_float(V_MID, (text = "##mid",
                              size = float2(36.0f, 140.0f), format = "%.2f"))
        same_line(SLINE_2)
        vslider_float(V_HI, (text = "##hi",
                             size = float2(36.0f, 140.0f), format = "%.2f"))
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
