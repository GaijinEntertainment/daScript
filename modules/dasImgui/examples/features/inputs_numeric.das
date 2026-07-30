options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: inputs_numeric — every Phase 0b.2 Input* numeric widget in one window.
// SHOWS:   input_float / input_float2/3/4
//          input_int   / input_int2/3/4
//          input_double
// DRIVE:   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/MASS","value":12.5}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/VEL_XYZ","value":[1.0,2.0,3.0]}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/inputs_numeric.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/inputs_numeric.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/inputs_numeric.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 0b.2 — input numeric", 800, 760)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 720.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Numeric inputs — phase 0b.2", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        // ----- Float scalars (with step buttons) -----
        text("Float scalars")
        separator()
        // step / step_fast surface ImGui's +/- buttons. Zero defaults
        // hide them; explicit non-zero shows the buttons.
        input_float(MASS, (text = "Mass (kg)", step = 0.1f, step_fast = 1.0f))
        text("mass = {MASS.value} kg")

        input_double(EPOCH_SEC, (text = "Epoch (sec)", step = 1.0lf, step_fast = 60.0lf, format = "%.3f"))
        text("epoch = {EPOCH_SEC.value} s")

        // ----- Float vectors (no step buttons) -----
        text("Float vectors")
        separator()
        input_float2(POS_XY, (text = "Pos XY"))
        text("pos = ({POS_XY.value.x}, {POS_XY.value.y})")

        input_float3(VEL_XYZ, (text = "Vel XYZ"))
        text("vel = ({VEL_XYZ.value.x}, {VEL_XYZ.value.y}, {VEL_XYZ.value.z})")

        input_float4(QUAT_XYZW, (text = "Quat XYZW", format = "%.4f"))
        text("quat = ({QUAT_XYZW.value.x}, {QUAT_XYZW.value.y}, {QUAT_XYZW.value.z}, {QUAT_XYZW.value.w})")

        spacing()

        // ----- Int scalars + vectors -----
        text("Int scalars")
        separator()
        input_int(LEVEL, (text = "Level"))
        text("level = {LEVEL.value}")

        text("Int vectors")
        separator()
        input_int2(GRID_XY, (text = "Grid XY"))
        text("grid = ({GRID_XY.value.x}, {GRID_XY.value.y})")

        input_int3(BLOCK_XYZ, (text = "Block XYZ"))
        text("block = ({BLOCK_XYZ.value.x}, {BLOCK_XYZ.value.y}, {BLOCK_XYZ.value.z})")

        input_int4(VIEWPORT_LTRB, (text = "Viewport LTRB"))
        text("viewport = [{VIEWPORT_LTRB.value.x}, {VIEWPORT_LTRB.value.y}, {VIEWPORT_LTRB.value.z}, {VIEWPORT_LTRB.value.w}]")
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
