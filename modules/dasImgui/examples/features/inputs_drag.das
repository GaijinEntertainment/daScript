options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: inputs_drag — every Phase 0b.2 Drag* widget in one window.
// SHOWS:   DragFloat / DragFloat2 / DragFloat3 / DragFloat4
//          DragInt / DragInt2 / DragInt3 / DragInt4
//          DragFloatRange2 / DragIntRange2
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/OPACITY","value":0.5}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/SCALE_XY","value":[2.0, 1.5]}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/PRICE","value":[5, 12]}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/inputs_drag.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/inputs_drag.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/inputs_drag.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 0b.2 — drag inputs", 800, 700)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 660.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Drag inputs — phase 0b.2", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        // ----- Float scalars -----
        // Bounds set once-per-frame from source; assignment is idempotent.
        // `speed` / `format` / `flags` are per-call args (defaults 1.0f/"%.3f"/None).
        text("Float scalars")
        separator()
        OPACITY.bounds = (0.0f, 1.0f)
        drag_float(OPACITY, (text = "Opacity", speed = 0.01f))
        text("opacity = {OPACITY.value}")

        // ----- Float vectors -----
        text("Float vectors")
        separator()
        SCALE_XY.bounds = (0.0f, 4.0f)
        drag_float2(SCALE_XY, (text = "Scale XY", speed = 0.05f))
        text("scale = ({SCALE_XY.value.x}, {SCALE_XY.value.y})")

        TINT_RGB.bounds = (0.0f, 1.0f)
        drag_float3(TINT_RGB, (text = "Tint RGB", speed = 0.01f))
        text("tint = ({TINT_RGB.value.x}, {TINT_RGB.value.y}, {TINT_RGB.value.z})")

        TINT_RGBA.bounds = (0.0f, 1.0f)
        drag_float4(TINT_RGBA, (text = "Tint RGBA", speed = 0.01f))
        text("tint_a = {TINT_RGBA.value.w}")

        spacing()

        // ----- Int scalars -----
        text("Int scalars")
        separator()
        SCORE.bounds = (0, 100)
        drag_int(SCORE, (text = "Score"))
        text("score = {SCORE.value}")

        // ----- Int vectors -----
        text("Int vectors")
        separator()
        TILE_XY.bounds = (-32, 32)
        drag_int2(TILE_XY, (text = "Tile XY"))
        text("tile = ({TILE_XY.value.x}, {TILE_XY.value.y})")

        VOXEL_XYZ.bounds = (-16, 16)
        drag_int3(VOXEL_XYZ, (text = "Voxel XYZ"))
        text("voxel = ({VOXEL_XYZ.value.x}, {VOXEL_XYZ.value.y}, {VOXEL_XYZ.value.z})")

        RECT_LTRB.bounds = (0, 256)
        drag_int4(RECT_LTRB, (text = "Rect LTRB"))
        text("rect_w = {RECT_LTRB.value.z - RECT_LTRB.value.x}")

        spacing()

        // ----- Range pairs -----
        text("Range pairs")
        separator()
        SPEED_BAND.bounds = (0.0f, 100.0f)
        drag_float_range2(SPEED_BAND, (text = "Speed band", speed = 0.5f))
        text("speed range = [{SPEED_BAND.value.x}, {SPEED_BAND.value.y}]")

        PRICE.bounds = (0, 50)
        drag_int_range2(PRICE, (text = "Price"))
        text("price = [{PRICE.value.x}, {PRICE.value.y}]")
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
