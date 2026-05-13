options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 0b.2 Slider* widgets — exercises every
//! scalar/vector/vertical kind through `imgui_set` via the playwright-style
//! harness and asserts the snapshot payload converges.

let SLIDER_FEATURE = "modules/dasImgui/examples/features/inputs_slider.das"

[test]
def test_inputs_slider_smoke(t : T?) {
    with_imgui_app(SLIDER_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "GAIN", 15.0f)
        t |> success(snap0 != null, "GAIN registered after first frame")
        if (snap0 == null) return

        // ----- Snapshot shape -----
        var gain0 = find_widget(snap0, "GAIN")
        t |> equal(gain0?["kind"] ?? "", "slider_float", "GAIN.kind == 'slider_float'")
        t |> success(gain0?["payload"] != null, "GAIN.payload present")

        // ----- Float scalar -----
        set_value(d, "GAIN", JV(0.7f))
        t |> success(wait_for_payload_value(d, "GAIN", "value", 0.7f, 300),
                     "imgui_set steers GAIN.value to 0.7")

        // ----- Float vectors -----
        set_value(d, "UV_OFFSET", JV(float2(-0.5f, 0.25f)))
        t |> success(wait_for_payload_value(d, "UV_OFFSET", "value", float2(-0.5f, 0.25f), 300),
                     "imgui_set steers UV_OFFSET to (-0.5, 0.25)")

        set_value(d, "WAVE_RGB", JV(float3(0.1f, 0.5f, 0.9f)))
        t |> success(wait_for_payload_value(d, "WAVE_RGB", "value", float3(0.1f, 0.5f, 0.9f), 300),
                     "imgui_set steers WAVE_RGB to (0.1, 0.5, 0.9)")

        set_value(d, "WAVE_RGBA", JV(float4(0.2f, 0.4f, 0.6f, 0.8f)))
        t |> success(wait_for_payload_value(d, "WAVE_RGBA", "value", float4(0.2f, 0.4f, 0.6f, 0.8f), 300),
                     "imgui_set steers WAVE_RGBA to (0.2, 0.4, 0.6, 0.8)")

        // ----- Int scalar + vectors -----
        set_value(d, "LIVES", JV(7))
        t |> success(wait_for_int_value(d, "LIVES", "value", 7, 300),
                     "imgui_set steers LIVES.value to 7")

        set_value(d, "SHIFT_XY", JV(int2(-3, 5)))
        t |> success(wait_for_payload_value(d, "SHIFT_XY", "value", int2(-3, 5), 300),
                     "imgui_set steers SHIFT_XY to (-3, 5)")

        set_value(d, "TINT_BUDGET_RGB", JV(int3(64, 128, 200)))
        t |> success(wait_for_payload_value(d, "TINT_BUDGET_RGB", "value", int3(64, 128, 200), 300),
                     "imgui_set steers TINT_BUDGET_RGB to (64, 128, 200)")

        set_value(d, "FRAME_LTRB", JV(int4(8, 16, 504, 240)))
        t |> success(wait_for_payload_value(d, "FRAME_LTRB", "value", int4(8, 16, 504, 240), 300),
                     "imgui_set steers FRAME_LTRB to (8, 16, 504, 240)")

        // ----- Vertical sliders (reuse SliderStateFloat / SliderStateInt) -----
        set_value(d, "VOL_L", JV(0.85f))
        t |> success(wait_for_payload_value(d, "VOL_L", "value", 0.85f, 300),
                     "imgui_set steers vslider VOL_L.value to 0.85")

        set_value(d, "BUS_GAIN", JV(-3))
        t |> success(wait_for_int_value(d, "BUS_GAIN", "value", -3, 300),
                     "imgui_set steers vslider BUS_GAIN.value to -3")

        // ----- Snapshot envelope -----
        var snap_final = snapshot(d)
        var vsl = find_widget(snap_final, "VOL_L")
        t |> equal(vsl?["kind"] ?? "", "vslider_float", "VOL_L.kind == 'vslider_float'")
        var vsi = find_widget(snap_final, "BUS_GAIN")
        t |> equal(vsi?["kind"] ?? "", "vslider_int", "BUS_GAIN.kind == 'vslider_int'")
    }
}
