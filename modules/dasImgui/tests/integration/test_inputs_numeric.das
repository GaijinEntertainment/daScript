options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 0b.2 Input* numeric widgets — every scalar
//! and vector kind including double, driven through `imgui_set` via the
//! playwright-style harness. Float comparisons use exact equality because
//! the harness pipes JSON-encoded values through with full float precision
//! (no rounding) and ImGui's Input* doesn't quantize to display format.

let NUMERIC_FEATURE = "modules/dasImgui/examples/features/inputs_numeric.das"

[test]
def test_inputs_numeric_smoke(t : T?) {
    with_imgui_app(NUMERIC_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MASS", 15.0f)
        t |> success(snap0 != null, "MASS registered after first frame")
        if (snap0 == null) return

        // ----- Snapshot shape -----
        var mass0 = find_widget(snap0, "MASS")
        t |> equal(mass0?["kind"] ?? "", "input_float", "MASS.kind == 'input_float'")
        t |> success(mass0?["payload"] != null, "MASS.payload present")

        // ----- Float scalar -----
        set_value(d, "MASS", JV(12.5f))
        t |> success(wait_for_payload_value(d, "MASS", "value", 12.5f, 300),
                     "imgui_set steers MASS.value to 12.5")

        // ----- Double scalar -----
        set_value(d, "EPOCH_SEC", JV(1700000000.5lf))
        t |> success(wait_for_payload_value(d, "EPOCH_SEC", "value", 1700000000.5lf, 300),
                     "imgui_set steers EPOCH_SEC.value to 1.7e9")

        // ----- Float vectors -----
        set_value(d, "POS_XY", JV(float2(10.0f, -3.5f)))
        t |> success(wait_for_payload_value(d, "POS_XY", "value", float2(10.0f, -3.5f), 300),
                     "imgui_set steers POS_XY to (10, -3.5)")

        set_value(d, "VEL_XYZ", JV(float3(1.0f, 2.0f, 3.0f)))
        t |> success(wait_for_payload_value(d, "VEL_XYZ", "value", float3(1.0f, 2.0f, 3.0f), 300),
                     "imgui_set steers VEL_XYZ to (1, 2, 3)")

        set_value(d, "QUAT_XYZW", JV(float4(0.0f, 0.0f, 0.0f, 1.0f)))
        t |> success(wait_for_payload_value(d, "QUAT_XYZW", "value", float4(0.0f, 0.0f, 0.0f, 1.0f), 300),
                     "imgui_set steers QUAT_XYZW to identity quaternion")

        // ----- Int scalar + vectors -----
        set_value(d, "LEVEL", JV(42))
        t |> success(wait_for_int_value(d, "LEVEL", "value", 42, 300),
                     "imgui_set steers LEVEL.value to 42")

        set_value(d, "GRID_XY", JV(int2(8, 16)))
        t |> success(wait_for_payload_value(d, "GRID_XY", "value", int2(8, 16), 300),
                     "imgui_set steers GRID_XY to (8, 16)")

        set_value(d, "BLOCK_XYZ", JV(int3(2, 4, 8)))
        t |> success(wait_for_payload_value(d, "BLOCK_XYZ", "value", int3(2, 4, 8), 300),
                     "imgui_set steers BLOCK_XYZ to (2, 4, 8)")

        set_value(d, "VIEWPORT_LTRB", JV(int4(0, 0, 1920, 1080)))
        t |> success(wait_for_payload_value(d, "VIEWPORT_LTRB", "value", int4(0, 0, 1920, 1080), 300),
                     "imgui_set steers VIEWPORT_LTRB to FullHD frame")

        // ----- Snapshot envelope -----
        var snap_final = snapshot(d)
        var dbl = find_widget(snap_final, "EPOCH_SEC")
        t |> equal(dbl?["kind"] ?? "", "input_double", "EPOCH_SEC.kind == 'input_double'")
        var i4 = find_widget(snap_final, "VIEWPORT_LTRB")
        t |> equal(i4?["kind"] ?? "", "input_int4", "VIEWPORT_LTRB.kind == 'input_int4'")
    }
}
