options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 0b.2 Drag* widgets.
//!
//! Spawns daslang-live against `examples/features/inputs_drag.das`, waits for
//! the GLFW window to render its first frame, then drives every Drag widget
//! through `imgui_force_set` and asserts the snapshot payload converges. Vector
//! values use `wait_for_payload_value` (generic on the JV-roundtrippable
//! type) instead of per-component checks — the post is atomic and so is
//! the assertion.

let DRAG_FEATURE = "modules/dasImgui/examples/features/inputs_drag.das"

[test]
def test_inputs_drag_smoke(t : T?) {
    with_imgui_app(DRAG_FEATURE) $(d) {
        // Gate on first-frame render via a known scalar widget.
        var snap0 = wait_for_widget(d, "MAIN_WIN/OPACITY", 15.0f)
        t |> success(snap0 != null, "OPACITY registered after first frame")
        if (snap0 == null) return

        // ----- Snapshot shape -----
        var op0 = find_widget(snap0, "MAIN_WIN/OPACITY")
        t |> equal(op0?["kind"] ?? "", "drag_float", "OPACITY.kind == 'drag_float'")
        t |> success(op0?["payload"] != null, "OPACITY.payload present")

        // ----- Float scalar — imgui_force_set on a single float -----
        force_set_value(d, "MAIN_WIN/OPACITY", JV(0.42f))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/OPACITY", "value", 0.42f, 300),
                     "imgui_force_set steers OPACITY.value to 0.42")

        // ----- Float vectors -----
        force_set_value(d, "MAIN_WIN/SCALE_XY", JV(float2(2.0f, 1.5f)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/SCALE_XY", "value", float2(2.0f, 1.5f), 300),
                     "imgui_force_set steers SCALE_XY.value to (2.0, 1.5)")

        force_set_value(d, "MAIN_WIN/TINT_RGB", JV(float3(0.25f, 0.5f, 0.75f)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/TINT_RGB", "value", float3(0.25f, 0.5f, 0.75f), 300),
                     "imgui_force_set steers TINT_RGB.value to (0.25, 0.5, 0.75)")

        force_set_value(d, "MAIN_WIN/TINT_RGBA", JV(float4(0.1f, 0.2f, 0.3f, 0.4f)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/TINT_RGBA", "value", float4(0.1f, 0.2f, 0.3f, 0.4f), 300),
                     "imgui_force_set steers TINT_RGBA.value to (0.1, 0.2, 0.3, 0.4)")

        // ----- Int scalar -----
        force_set_value(d, "MAIN_WIN/SCORE", JV(73))
        t |> success(wait_for_int_value(d, "MAIN_WIN/SCORE", "value", 73, 300),
                     "imgui_force_set steers SCORE.value to 73")

        // ----- Int vectors -----
        force_set_value(d, "MAIN_WIN/TILE_XY", JV(int2(-7, 11)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/TILE_XY", "value", int2(-7, 11), 300),
                     "imgui_force_set steers TILE_XY.value to (-7, 11)")

        force_set_value(d, "MAIN_WIN/VOXEL_XYZ", JV(int3(1, -2, 3)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/VOXEL_XYZ", "value", int3(1, -2, 3), 300),
                     "imgui_force_set steers VOXEL_XYZ.value to (1, -2, 3)")

        force_set_value(d, "MAIN_WIN/RECT_LTRB", JV(int4(10, 20, 200, 100)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/RECT_LTRB", "value", int4(10, 20, 200, 100), 300),
                     "imgui_force_set steers RECT_LTRB.value to (10, 20, 200, 100)")

        // ----- Range pairs (float2/int2 atomically) -----
        force_set_value(d, "MAIN_WIN/SPEED_BAND", JV(float2(15.0f, 60.0f)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/SPEED_BAND", "value", float2(15.0f, 60.0f), 300),
                     "imgui_force_set writes SPEED_BAND range atomically")

        force_set_value(d, "MAIN_WIN/PRICE", JV(int2(5, 30)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/PRICE", "value", int2(5, 30), 300),
                     "imgui_force_set writes PRICE range atomically")

        // ----- Snapshot envelope -----
        var snap_final = snapshot(d)
        var price = find_widget(snap_final, "MAIN_WIN/PRICE")
        t |> equal(price?["kind"] ?? "", "drag_int_range2", "PRICE.kind == 'drag_int_range2'")
        t |> success(price?["hex_id"] != null, "PRICE.hex_id present")
        t |> success(price?["bbox"] != null, "PRICE.bbox present")
    }
}
