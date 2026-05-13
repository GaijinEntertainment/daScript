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

// =============================================================================
// FEATURE: snapshot_unrendered — Phase 1.1 long-lived registry contract.
// SHOWS:   widgets registered at module init via setup_call_list appear in
//          imgui_snapshot even when their render path never executes. The
//          [widget] macro auto-emits state globals + register_widget calls
//          into an [init] private register`imgui`widgets() function, so
//          unreachable widget call sites still populate g_widgets.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          → globals contains both VISIBLE_VAL (rendered, has bbox) and
//          NEVER_RENDERED (rendered=false, no bbox, payload still present).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/snapshot_unrendered.das
// LIVE:       daslang-live modules/dasImgui/examples/features/snapshot_unrendered.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 1.1 — snapshot unrendered", 800, 600)
    live_imgui_init(live_window)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 1.5
}

def private register_only_never_rendered() {
    //! Never called at runtime, but the [widget] macro fires at compile
    //! time on this slider call. That auto-emits NEVER_RENDERED as a
    //! module-scope global AND appends a register_widget(...) line into
    //! the per-module register`imgui`widgets() [init] function. So at
    //! module load NEVER_RENDERED is registered in g_widgets, even
    //! though imgui never paints it.
    slider_float(NEVER_RENDERED, (text = "Hidden — never painted"))
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplOpenGL3_NewFrame()
    ImGui_ImplGlfw_NewFrame()
    NewFrame()

    SetNextWindowSize(ImVec2(720.0, 240.0), ImGuiCond.Always)
    if (Begin("Phase 1.1 — registry covers unrendered widgets", null, ImGuiWindowFlags.None)) {
        Text("VISIBLE_VAL renders every frame. NEVER_RENDERED never does.")
        Text("imgui_snapshot must list both, with rendered:false on the second.")

        slider_float(VISIBLE_VAL, (text = "Visible (rendered)"))
        Text("VISIBLE_VAL.value = {VISIBLE_VAL.value}")
        Text("NEVER_RENDERED.value = {NEVER_RENDERED.value}  (state readable; widget never painted)")
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
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
