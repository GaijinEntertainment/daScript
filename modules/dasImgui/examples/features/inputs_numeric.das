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
// FEATURE: inputs_numeric — every Phase 0b.2 Input* numeric widget in one window.
// SHOWS:   input_float / input_float2/3/4
//          input_int   / input_int2/3/4
//          input_double
// DRIVE:   curl -X POST -d '{"name":"imgui_set","args":{"target":"MASS","value":12.5}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_set","args":{"target":"VEL_XYZ","value":[1.0,2.0,3.0]}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/inputs_numeric.das
// LIVE:       daslang-live modules/dasImgui/examples/features/inputs_numeric.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 0b.2 — input numeric", 800, 760)
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

    SetNextWindowSize(ImVec2(720.0, 720.0), ImGuiCond.Always)
    if (Begin("Numeric inputs — phase 0b.2", null, ImGuiWindowFlags.None)) {
        // ----- Float scalars (with step buttons) -----
        Text("Float scalars")
        Separator()
        // step / step_fast surface ImGui's +/- buttons. Zero defaults
        // hide them; explicit non-zero shows the buttons.
        input_float(MASS, (text = "Mass (kg)", step = 0.1f, step_fast = 1.0f))
        Text("mass = {MASS.value} kg")

        input_double(EPOCH_SEC, (text = "Epoch (sec)", step = 1.0lf, step_fast = 60.0lf, format = "%.3f"))
        Text("epoch = {EPOCH_SEC.value} s")

        // ----- Float vectors (no step buttons) -----
        Text("Float vectors")
        Separator()
        input_float2(POS_XY, (text = "Pos XY"))
        Text("pos = ({POS_XY.value.x}, {POS_XY.value.y})")

        input_float3(VEL_XYZ, (text = "Vel XYZ"))
        Text("vel = ({VEL_XYZ.value.x}, {VEL_XYZ.value.y}, {VEL_XYZ.value.z})")

        input_float4(QUAT_XYZW, (text = "Quat XYZW", format = "%.4f"))
        Text("quat = ({QUAT_XYZW.value.x}, {QUAT_XYZW.value.y}, {QUAT_XYZW.value.z}, {QUAT_XYZW.value.w})")

        Spacing()

        // ----- Int scalars + vectors -----
        Text("Int scalars")
        Separator()
        input_int(LEVEL, (text = "Level"))
        Text("level = {LEVEL.value}")

        Text("Int vectors")
        Separator()
        input_int2(GRID_XY, (text = "Grid XY"))
        Text("grid = ({GRID_XY.value.x}, {GRID_XY.value.y})")

        input_int3(BLOCK_XYZ, (text = "Block XYZ"))
        Text("block = ({BLOCK_XYZ.value.x}, {BLOCK_XYZ.value.y}, {BLOCK_XYZ.value.z})")

        input_int4(VIEWPORT_LTRB, (text = "Viewport LTRB"))
        Text("viewport = [{VIEWPORT_LTRB.value.x}, {VIEWPORT_LTRB.value.y}, {VIEWPORT_LTRB.value.z}, {VIEWPORT_LTRB.value.w}]")
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
