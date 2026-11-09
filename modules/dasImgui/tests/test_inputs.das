options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require imgui/imgui_boost_v2
require imgui/imgui_boost_runtime
require imgui/imgui_containers_builtin
require imgui public

//! Input widgets: kinds, force_set round-trips, indexed forms.

let NUMERIC_FEATURE = "modules/dasImgui/examples/features/inputs_numeric.das"

[test(description = "Every input_* numeric kind, float, double and int scalars and vectors, round-trips a force_set value exactly.", timeout = 1200)]
def test_inputs_numeric_smoke(t : T?) {
    with_imgui_app(NUMERIC_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/MASS", 15.0f)
        t |> success(snap0 != null, "MASS registered after first frame")
        if (snap0 == null) return

        // ----- Snapshot shape -----
        var mass0 = find_widget(snap0, "MAIN_WIN/MASS")
        t |> equal(mass0?["kind"] ?? "", "input_float", "MASS.kind == 'input_float'")
        t |> success(mass0?["payload"] != null, "MASS.payload present")

        // ----- Float scalar -----
        force_set_value(d, "MAIN_WIN/MASS", JV(12.5f))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/MASS", "value", 12.5f, 300),
                     "imgui_force_set steers MASS.value to 12.5")

        // ----- Double scalar -----
        force_set_value(d, "MAIN_WIN/EPOCH_SEC", JV(1700000000.5lf))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/EPOCH_SEC", "value", 1700000000.5lf, 300),
                     "imgui_force_set steers EPOCH_SEC.value to 1.7e9")

        // ----- Float vectors -----
        force_set_value(d, "MAIN_WIN/POS_XY", JV(float2(10.0f, -3.5f)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/POS_XY", "value", float2(10.0f, -3.5f), 300),
                     "imgui_force_set steers POS_XY to (10, -3.5)")

        force_set_value(d, "MAIN_WIN/VEL_XYZ", JV(float3(1.0f, 2.0f, 3.0f)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/VEL_XYZ", "value", float3(1.0f, 2.0f, 3.0f), 300),
                     "imgui_force_set steers VEL_XYZ to (1, 2, 3)")

        force_set_value(d, "MAIN_WIN/QUAT_XYZW", JV(float4(0.0f, 0.0f, 0.0f, 1.0f)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/QUAT_XYZW", "value", float4(0.0f, 0.0f, 0.0f, 1.0f), 300),
                     "imgui_force_set steers QUAT_XYZW to identity quaternion")

        // ----- Int scalar + vectors -----
        force_set_value(d, "MAIN_WIN/LEVEL", JV(42))
        t |> success(wait_for_int_value(d, "MAIN_WIN/LEVEL", "value", 42, 300),
                     "imgui_force_set steers LEVEL.value to 42")

        force_set_value(d, "MAIN_WIN/GRID_XY", JV(int2(8, 16)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/GRID_XY", "value", int2(8, 16), 300),
                     "imgui_force_set steers GRID_XY to (8, 16)")

        force_set_value(d, "MAIN_WIN/BLOCK_XYZ", JV(int3(2, 4, 8)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/BLOCK_XYZ", "value", int3(2, 4, 8), 300),
                     "imgui_force_set steers BLOCK_XYZ to (2, 4, 8)")

        force_set_value(d, "MAIN_WIN/VIEWPORT_LTRB", JV(int4(0, 0, 1920, 1080)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/VIEWPORT_LTRB", "value", int4(0, 0, 1920, 1080), 300),
                     "imgui_force_set steers VIEWPORT_LTRB to FullHD frame")

        // ----- Snapshot envelope -----
        var snap_final = snapshot(d)
        var dbl = find_widget(snap_final, "MAIN_WIN/EPOCH_SEC")
        t |> equal(dbl?["kind"] ?? "", "input_double", "EPOCH_SEC.kind == 'input_double'")
        var i4 = find_widget(snap_final, "MAIN_WIN/VIEWPORT_LTRB")
        t |> equal(i4?["kind"] ?? "", "input_int4", "VIEWPORT_LTRB.kind == 'input_int4'")
    }
}

let SLIDER_FEATURE = "modules/dasImgui/examples/features/inputs_slider.das"

[test(description = "Every slider_* scalar, vector and vertical kind round-trips a force_set value and reports its kind.", timeout = 1200)]
def test_inputs_slider_smoke(t : T?) {
    with_imgui_app(SLIDER_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/GAIN", 15.0f)
        t |> success(snap0 != null, "GAIN registered after first frame")
        if (snap0 == null) return

        // ----- Snapshot shape -----
        var gain0 = find_widget(snap0, "MAIN_WIN/GAIN")
        t |> equal(gain0?["kind"] ?? "", "slider_float", "GAIN.kind == 'slider_float'")
        t |> success(gain0?["payload"] != null, "GAIN.payload present")

        // ----- Float scalar -----
        force_set_value(d, "MAIN_WIN/GAIN", JV(0.7f))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/GAIN", "value", 0.7f, 300),
                     "imgui_force_set steers GAIN.value to 0.7")

        // ----- Float vectors -----
        force_set_value(d, "MAIN_WIN/UV_OFFSET", JV(float2(-0.5f, 0.25f)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/UV_OFFSET", "value", float2(-0.5f, 0.25f), 300),
                     "imgui_force_set steers UV_OFFSET to (-0.5, 0.25)")

        force_set_value(d, "MAIN_WIN/WAVE_RGB", JV(float3(0.1f, 0.5f, 0.9f)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/WAVE_RGB", "value", float3(0.1f, 0.5f, 0.9f), 300),
                     "imgui_force_set steers WAVE_RGB to (0.1, 0.5, 0.9)")

        force_set_value(d, "MAIN_WIN/WAVE_RGBA", JV(float4(0.2f, 0.4f, 0.6f, 0.8f)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/WAVE_RGBA", "value", float4(0.2f, 0.4f, 0.6f, 0.8f), 300),
                     "imgui_force_set steers WAVE_RGBA to (0.2, 0.4, 0.6, 0.8)")

        // ----- Int scalar + vectors -----
        force_set_value(d, "MAIN_WIN/LIVES", JV(7))
        t |> success(wait_for_int_value(d, "MAIN_WIN/LIVES", "value", 7, 300),
                     "imgui_force_set steers LIVES.value to 7")

        force_set_value(d, "MAIN_WIN/SHIFT_XY", JV(int2(-3, 5)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/SHIFT_XY", "value", int2(-3, 5), 300),
                     "imgui_force_set steers SHIFT_XY to (-3, 5)")

        force_set_value(d, "MAIN_WIN/TINT_BUDGET_RGB", JV(int3(64, 128, 200)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/TINT_BUDGET_RGB", "value", int3(64, 128, 200), 300),
                     "imgui_force_set steers TINT_BUDGET_RGB to (64, 128, 200)")

        force_set_value(d, "MAIN_WIN/FRAME_LTRB", JV(int4(8, 16, 504, 240)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/FRAME_LTRB", "value", int4(8, 16, 504, 240), 300),
                     "imgui_force_set steers FRAME_LTRB to (8, 16, 504, 240)")

        // ----- Vertical sliders (reuse SliderStateFloat / SliderStateInt) -----
        force_set_value(d, "MAIN_WIN/VOL_L", JV(0.85f))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/VOL_L", "value", 0.85f, 300),
                     "imgui_force_set steers vslider VOL_L.value to 0.85")

        force_set_value(d, "MAIN_WIN/BUS_GAIN", JV(-3))
        t |> success(wait_for_int_value(d, "MAIN_WIN/BUS_GAIN", "value", -3, 300),
                     "imgui_force_set steers vslider BUS_GAIN.value to -3")

        // ----- Snapshot envelope -----
        var snap_final = snapshot(d)
        var vsl = find_widget(snap_final, "MAIN_WIN/VOL_L")
        t |> equal(vsl?["kind"] ?? "", "vslider_float", "VOL_L.kind == 'vslider_float'")
        var vsi = find_widget(snap_final, "MAIN_WIN/BUS_GAIN")
        t |> equal(vsi?["kind"] ?? "", "vslider_int", "BUS_GAIN.kind == 'vslider_int'")
    }
}

let DRAG_FEATURE = "modules/dasImgui/examples/features/inputs_drag.das"

[test(description = "Every drag_* scalar, vector and range kind round-trips a force_set value, a range pair landing atomically.", timeout = 1200)]
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

let COLOR_FEATURE = "modules/dasImgui/examples/features/inputs_color.das"

//! A second click posted mid-release replaces the first: gate on idle
[test(description = "Color editors and pickers round-trip force_set RGB and RGBA values, and color_button swatches count each click, two in a row included.", timeout = 1200)]
def test_inputs_color_smoke(t : T?) {
    with_imgui_app(COLOR_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/TINT_RGB", 15.0f)
        t |> success(snap0 != null, "TINT_RGB registered after first frame")
        if (snap0 == null) return

        // ----- Snapshot shape -----
        var tint0 = find_widget(snap0, "MAIN_WIN/TINT_RGB")
        t |> equal(tint0?["kind"] ?? "", "color_edit3", "TINT_RGB.kind == 'color_edit3'")
        t |> success(tint0?["payload"] != null, "TINT_RGB.payload present")

        // ----- Editors — float3 / float4 -----
        force_set_value(d, "MAIN_WIN/TINT_RGB", JV(float3(0.2f, 0.5f, 0.9f)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/TINT_RGB", "value", float3(0.2f, 0.5f, 0.9f), 300),
                     "imgui_force_set steers TINT_RGB to (0.2, 0.5, 0.9)")

        force_set_value(d, "MAIN_WIN/BG_COLOR", JV(float4(0.1f, 0.2f, 0.3f, 0.4f)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/BG_COLOR", "value", float4(0.1f, 0.2f, 0.3f, 0.4f), 300),
                     "imgui_force_set steers BG_COLOR to (0.1, 0.2, 0.3, 0.4)")

        // ----- Pickers — same JSON shape as editors -----
        force_set_value(d, "MAIN_WIN/SUN_RGB", JV(float3(1.0f, 0.95f, 0.8f)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/SUN_RGB", "value", float3(1.0f, 0.95f, 0.8f), 300),
                     "imgui_force_set steers SUN_RGB to warm white")

        force_set_value(d, "MAIN_WIN/SHIRT_COLOR", JV(float4(0.7f, 0.1f, 0.1f, 1.0f)))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/SHIRT_COLOR", "value", float4(0.7f, 0.1f, 0.1f, 1.0f), 300),
                     "imgui_force_set steers SHIRT_COLOR to maroon (with ref_col)")

        // ----- ColorButton swatches — fire-on-click via imgui_click -----
        var red_before = find_widget(snap0, "MAIN_WIN/PALETTE_RED")
        t |> equal(red_before?["kind"] ?? "", "color_button", "PALETTE_RED.kind == 'color_button'")
        let red_count_before = red_before?["payload"]?["click_count"] ?? 0

        click(d, "MAIN_WIN/PALETTE_RED")
        t |> success(wait_for_int_value(d, "MAIN_WIN/PALETTE_RED", "click_count", red_count_before + 1, 300),
                     "imgui_click bumps PALETTE_RED.click_count")

        // Two clicks must NOT race: a second imgui_click coroutine posted while the
        // first's release is still in flight replaces it, under-counting to 1. Gate
        // on mouse-idle between (and after) so both press+release pairs fully land.
        click(d, "MAIN_WIN/PALETTE_GREEN")
        t |> success(wait_for_mouse_idle(d, 10.0f), "first PALETTE_GREEN click drains before second")
        click(d, "MAIN_WIN/PALETTE_GREEN")
        t |> success(wait_for_mouse_idle(d, 10.0f), "second PALETTE_GREEN click drains before count")
        t |> success(wait_for_int_value(d, "MAIN_WIN/PALETTE_GREEN", "click_count", 2, 300),
                     "two imgui_clicks accumulate on PALETTE_GREEN.click_count")

        // ----- Snapshot envelope -----
        var snap_final = snapshot(d)
        var pp4 = find_widget(snap_final, "MAIN_WIN/SHIRT_COLOR")
        t |> equal(pp4?["kind"] ?? "", "color_picker4", "SHIRT_COLOR.kind == 'color_picker4'")
        var pp3 = find_widget(snap_final, "MAIN_WIN/SUN_RGB")
        t |> equal(pp3?["kind"] ?? "", "color_picker3", "SUN_RGB.kind == 'color_picker3'")
    }
}

let CHOICE_FEATURE = "modules/dasImgui/examples/features/inputs_choice.das"

[test(description = "combo and list_box register their kinds and force_set steers each to an index that persists in later snapshots.", timeout = 1200)]
def test_inputs_choice_smoke(t : T?) {
    with_imgui_app(CHOICE_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/FRUIT", 15.0f)
        t |> success(snap0 != null, "FRUIT registered after first frame")
        if (snap0 == null) return

        // ----- Snapshot shape -----
        var fruit0 = find_widget(snap0, "MAIN_WIN/FRUIT")
        t |> equal(fruit0?["kind"] ?? "", "combo", "FRUIT.kind == 'combo'")
        t |> success(fruit0?["payload"] != null, "FRUIT.payload present")

        var size0 = find_widget(snap0, "MAIN_WIN/SHIRT_SIZE")
        t |> equal(size0?["kind"] ?? "", "list_box", "SHIRT_SIZE.kind == 'list_box'")

        // ----- Combo — imgui_force_set jumps to N-th item -----
        force_set_value(d, "MAIN_WIN/FRUIT", JV(2))
        t |> success(wait_for_int_value(d, "MAIN_WIN/FRUIT", "value", 2, 300),
                     "imgui_force_set steers FRUIT to index 2 (Cherry)")

        force_set_value(d, "MAIN_WIN/FRUIT", JV(4))
        t |> success(wait_for_int_value(d, "MAIN_WIN/FRUIT", "value", 4, 300),
                     "imgui_force_set steers FRUIT to index 4 (Elderberry)")

        force_set_value(d, "MAIN_WIN/WEEKDAY", JV(6))
        t |> success(wait_for_int_value(d, "MAIN_WIN/WEEKDAY", "value", 6, 300),
                     "imgui_force_set steers WEEKDAY to index 6 (Sun)")

        // ----- ListBox — same dispatch shape -----
        force_set_value(d, "MAIN_WIN/SHIRT_SIZE", JV(3))
        t |> success(wait_for_int_value(d, "MAIN_WIN/SHIRT_SIZE", "value", 3, 300),
                     "imgui_force_set steers SHIRT_SIZE to index 3 (L)")

        force_set_value(d, "MAIN_WIN/SHIRT_SIZE", JV(0))
        t |> success(wait_for_int_value(d, "MAIN_WIN/SHIRT_SIZE", "value", 0, 300),
                     "imgui_force_set steers SHIRT_SIZE to index 0 (XS)")

        // ----- Final snapshot envelope -----
        var snap_final = snapshot(d)
        var fruit_final = find_widget(snap_final, "MAIN_WIN/FRUIT")
        t |> equal(fruit_final?["payload"]?["value"] ?? -1, 4,
                   "FRUIT.payload.value persists final imgui_force_set")
        var size_final = find_widget(snap_final, "MAIN_WIN/SHIRT_SIZE")
        t |> equal(size_final?["payload"]?["value"] ?? -1, 0,
                   "SHIRT_SIZE.payload.value persists final imgui_force_set")
    }
}

let TEXT_FEATURE = "modules/dasImgui/examples/features/inputs_text.das"

[test(description = "The input_text family round-trips force_set strings, a multiline newline included, input_text_growable grows past its 256-byte capacity, and combo_getter round-trips an index.", timeout = 1200)]
def test_inputs_text_smoke(t : T?) {
    with_imgui_app(TEXT_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/NAME", 15.0f)
        t |> success(snap0 != null, "NAME registered after first frame")
        if (snap0 == null) return

        // ----- Snapshot kinds -----
        var name0 = find_widget(snap0, "MAIN_WIN/NAME")
        t |> equal(name0?["kind"] ?? "", "input_text",
                   "NAME.kind == 'input_text'")
        var email0 = find_widget(snap0, "MAIN_WIN/EMAIL")
        t |> equal(email0?["kind"] ?? "", "input_text_with_hint",
                   "EMAIL.kind == 'input_text_with_hint'")
        var bio0 = find_widget(snap0, "MAIN_WIN/BIO")
        t |> equal(bio0?["kind"] ?? "", "input_text_multiline",
                   "BIO.kind == 'input_text_multiline'")
        var grow0 = find_widget(snap0, "MAIN_WIN/GROW")
        t |> equal(grow0?["kind"] ?? "", "input_text_growable",
                   "GROW.kind == 'input_text_growable'")
        var font0 = find_widget(snap0, "MAIN_WIN/FONT")
        t |> equal(font0?["kind"] ?? "", "combo_getter",
                   "FONT.kind == 'combo_getter'")

        // ----- input_text — string round-trip -----
        force_set_value(d, "MAIN_WIN/NAME", JV("Alice"))
        t |> success(wait_for_string_value(d, "MAIN_WIN/NAME", "value", "Alice", 300),
                     "imgui_force_set steers NAME.value to 'Alice'")
        force_set_value(d, "MAIN_WIN/NAME", JV("Bob"))
        t |> success(wait_for_string_value(d, "MAIN_WIN/NAME", "value", "Bob", 300),
                     "imgui_force_set re-steers NAME.value to 'Bob'")

        // ----- input_text_with_hint — same round-trip -----
        force_set_value(d, "MAIN_WIN/EMAIL", JV("bob@example.com"))
        t |> success(wait_for_string_value(d, "MAIN_WIN/EMAIL", "value", "bob@example.com", 300),
                     "imgui_force_set steers EMAIL.value")

        // ----- input_text_multiline — embedded newline survives -----
        force_set_value(d, "MAIN_WIN/BIO", JV("line one\nline two"))
        t |> success(wait_for_string_value(d, "MAIN_WIN/BIO", "value", "line one\nline two", 300),
                     "imgui_force_set steers BIO.value with embedded newline")

        // ----- input_text_growable — long string forces CallbackResize -----
        // 400 bytes overflows the default 256-byte capacity; the trampoline lambda must resize
        // state.buffer + repoint data.Buf for ImGui to accept the assignment.
        var long_text = ""
        for (_ in range(400)) {
            long_text += "x"
        }
        force_set_value(d, "MAIN_WIN/GROW", JV(long_text))
        t |> success(wait_for_string_value(d, "MAIN_WIN/GROW", "value", long_text, 480),
                     "imgui_force_set steers GROW.value past 256B (CallbackResize fired)")
        var snap_grew = snapshot(d)
        let final_capacity = widget_payload_field(snap_grew, "MAIN_WIN/GROW", "capacity") ?? 0
        t |> success(final_capacity > 256,
                     "GROW.capacity grew past 256 (got {final_capacity})")

        // ----- combo_getter — int round-trip via lambda getter -----
        force_set_value(d, "MAIN_WIN/FONT", JV(2))
        t |> success(wait_for_int_value(d, "MAIN_WIN/FONT", "value", 2, 300),
                     "imgui_force_set steers FONT to row 2 (Mono)")
        force_set_value(d, "MAIN_WIN/FONT", JV(4))
        t |> success(wait_for_int_value(d, "MAIN_WIN/FONT", "value", 4, 300),
                     "imgui_force_set re-steers FONT to row 4 (Display)")

        // ----- Final snapshot persistence -----
        var snap_final = snapshot(d)
        t |> equal(widget_payload_field(snap_final, "MAIN_WIN/EMAIL", "value") ?? "",
                   "bob@example.com",
                   "EMAIL.value persists final imgui_force_set")
        t |> equal(widget_payload_field(snap_final, "MAIN_WIN/FONT", "value") ?? -1,
                   4,
                   "FONT.value persists final imgui_force_set")
    }
}

let TEXT_CALLBACK_FEATURE = "modules/dasImgui/examples/features/input_text_callback.das"

[test(description = "input_text_callback registers its kind with an empty value and round-trips force_set strings.", timeout = 1200)]
def test_input_text_callback_smoke(t : T?) {
    with_imgui_app(TEXT_CALLBACK_FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/CMD", 15.0f)
        t |> success(snap != null, "CMD registers")
        if (snap == null) return

        let cmd = find_widget(snap, "MAIN_WIN/CMD")
        t |> equal(cmd?["kind"] ?? "", "input_text_callback",
                   "CMD.kind == 'input_text_callback'")
        t |> equal(widget_payload_field(snap, "MAIN_WIN/CMD", "value") ?? "?",
                   "",
                   "CMD.value initially empty")

        // Surrounding siblings register.
        t |> success(widget_exists(snap, "MAIN_WIN/LEAD"), "LEAD registers")
        t |> success(widget_exists(snap, "MAIN_WIN/SEP"),  "SEP registers")
        t |> success(widget_exists(snap, "MAIN_WIN/STATIC"), "STATIC registers")

        // Round-trip via imgui_force_set dispatcher (inherited from input_text family).
        force_set_value(d, "MAIN_WIN/CMD", JV("he"))
        t |> success(wait_for_string_value(d, "MAIN_WIN/CMD", "value", "he", 300),
                     "imgui_force_set steers CMD.value to 'he'")
        force_set_value(d, "MAIN_WIN/CMD", JV("hello"))
        t |> success(wait_for_string_value(d, "MAIN_WIN/CMD", "value", "hello", 300),
                     "imgui_force_set re-steers CMD.value to 'hello'")
    }
}

let TRIGGERS_FEATURE = "modules/dasImgui/examples/features/triggers.das"

def alias_x(var snap : JsonValue?; v_button : int) : float =>
    find_widget(snap, "MAIN_WIN/DIFFICULTY#{v_button}")?["bbox"]?["x"] ?? -1.0f

[test(description = "Each site of a grouped radio_button_int registers a left-to-right #<v_button> alias whose click selects that option, while force_set on the shared path still steers the group.", timeout = 1200)]
def test_radio_int_per_site_targeting(t : T?) {
    with_imgui_app(TRIGGERS_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/DIFFICULTY", 15.0f)
        t |> success(snap0 != null, "DIFFICULTY group registered after first frame")
        if (snap0 == null) return

        // Gate 1: per-site aliases exist, each a radio_button_int.
        for (vb in range(3)) {
            let path = "MAIN_WIN/DIFFICULTY#{vb}"
            t |> success(widget_exists(snap0, path), "{path} alias registered")
            t |> equal(find_widget(snap0, path)?["kind"] ?? "", "radio_button_int",
                       "{path}.kind == radio_button_int")
        }

        // Gate 2: the three alias bboxes are distinct and ordered left-to-right (Easy/Normal/Hard
        // laid out with same_line). The bare path bbox is the LAST site (Hard), proving the aliases
        // capture per-site geometry the shared entry can't.
        let x0 = alias_x(snap0, 0)
        let x1 = alias_x(snap0, 1)
        let x2 = alias_x(snap0, 2)
        t |> success(x0 >= 0.0f && x1 >= 0.0f && x2 >= 0.0f, "all three aliases expose a bbox")
        t |> success(x0 < x1 && x1 < x2, "alias bboxes are distinct, left-to-right ({x0} < {x1} < {x2})")

        // Gate 3: clicking each alias selects that v_button (shared value follows).
        click(d, "MAIN_WIN/DIFFICULTY#2")
        t |> success(wait_for_int_value(d, "MAIN_WIN/DIFFICULTY", "value", 2, 300),
                     "click DIFFICULTY#2 -> value 2 (Hard)")
        click(d, "MAIN_WIN/DIFFICULTY#0")
        t |> success(wait_for_int_value(d, "MAIN_WIN/DIFFICULTY", "value", 0, 300),
                     "click DIFFICULTY#0 -> value 0 (Easy)")
        click(d, "MAIN_WIN/DIFFICULTY#1")
        t |> success(wait_for_int_value(d, "MAIN_WIN/DIFFICULTY", "value", 1, 300),
                     "click DIFFICULTY#1 -> value 1 (Normal)")

        // Gate 4: the shared bare path still drives the value via imgui_force_set (unchanged).
        force_set_value(d, "MAIN_WIN/DIFFICULTY", JV(2))
        t |> success(wait_for_int_value(d, "MAIN_WIN/DIFFICULTY", "value", 2, 300),
                     "force_set on shared DIFFICULTY still steers the selection")
    }
}

let INDEXED_FEATURE = "modules/dasImgui/examples/features/inputs_indexed.das"

[test(description = "Int- and string-keyed indexed widgets register a distinct path per subscript, and force_set on one key leaves the other keys at their defaults.", timeout = 1200)]
def test_inputs_indexed_smoke(t : T?) {
    with_imgui_app(INDEXED_FEATURE) $(d) {
        // First frame: the wrapper's first-touch path runs for each rendered
        // key. Wait for the int-keyed [0] entry — proves the macro emitted
        // the indexed wrapper and it registered the per-key path.
        var snap0 = wait_for_widget(d, "MIX_WIN/CHANNEL_VOL[0]", 15.0f)
        t |> success(snap0 != null, "MIX_WIN/CHANNEL_VOL[0] registered after first frame")
        return if (snap0 == null)

        // ----- Path key shape: int-keyed -----
        for (i in range(4)) {
            var entry = find_widget(snap0, "MIX_WIN/CHANNEL_VOL[{i}]")
            t |> success(entry != null, "MIX_WIN/CHANNEL_VOL[{i}] in snapshot")
            t |> equal(entry?["kind"] ?? "", "slider_float", "[{i}].kind == 'slider_float'")
            t |> success(entry?["bbox"] != null, "[{i}].bbox present")
            t |> success(entry?["hex_id"] != null, "[{i}].hex_id present (distinct per key)")
        }

        // ----- Path key shape: string-keyed -----
        for (name in ["bass", "lead", "drum", "vocals"]) {
            var entry = find_widget(snap0, "MIX_WIN/TRACK_VOL[{name}]")
            t |> success(entry != null, "MIX_WIN/TRACK_VOL[{name}] in snapshot")
            t |> equal(entry?["kind"] ?? "", "slider_float", "[{name}].kind == 'slider_float'")
        }

        // ----- imgui_force_set lands on the right key (cross-key isolation) -----
        force_set_value(d, "MIX_WIN/CHANNEL_VOL[2]", JV(0.123f))
        t |> success(wait_for_payload_value(d, "MIX_WIN/CHANNEL_VOL[2]", "value", 0.123f, 300),
                     "imgui_force_set MIX_WIN/CHANNEL_VOL[2]=0.123 lands")
        var snap_after_set = snapshot(d)
        // Other keys must be untouched.
        let v0 = widget_payload_field(snap_after_set, "MIX_WIN/CHANNEL_VOL[0]", "value")
        t |> equal(v0 ?? -1.0f, 0.5f, "MIX_WIN/CHANNEL_VOL[0].value still 0.5 (init default)")
        let v3 = widget_payload_field(snap_after_set, "MIX_WIN/CHANNEL_VOL[3]", "value")
        t |> equal(v3 ?? -1.0f, 0.5f, "MIX_WIN/CHANNEL_VOL[3].value still 0.5 (init default)")

        // ----- String-keyed isolation -----
        force_set_value(d, "MIX_WIN/TRACK_VOL[lead]", JV(0.95f))
        t |> success(wait_for_payload_value(d, "MIX_WIN/TRACK_VOL[lead]", "value", 0.95f, 300),
                     "imgui_force_set MIX_WIN/TRACK_VOL[lead]=0.95 lands")
        var snap_after_str = snapshot(d)
        let bass_v = widget_payload_field(snap_after_str, "MIX_WIN/TRACK_VOL[bass]", "value")
        t |> equal(bass_v ?? -1.0f, 0.7f, "MIX_WIN/TRACK_VOL[bass].value still 0.7 (init default)")
    }
}

let DYN_FEATURE = "modules/dasImgui/examples/features/indexed_dynamic.das"

[test(description = "An indexed widget over a growing and shrinking table registers an entry per added key, isolates force_set per key, drops an erased key, and re-adds it with fresh defaults.", timeout = 1200)]
def test_indexed_dynamic_add_remove(t : T?) {
    with_imgui_app(DYN_FEATURE) $(d) {
        // Wait for the window's controls to register.
        var snap0 = wait_for_widget(d, "DYN_WIN/ADD_CH", 15.0f)
        t |> success(snap0 != null, "DYN_WIN/ADD_CH registered after first frame")
        return if (snap0 == null)
        // Empty table: no CHANNEL_VOL[*] entries.
        t |> success(!widget_exists(snap0, "DYN_WIN/CHANNEL_VOL[0]"),
                     "CHANNEL_VOL empty initially (no [0] entry)")

        // ADD → snapshot grows by one.
        click(d, "DYN_WIN/ADD_CH")
        t |> success(wait_for_widget(d, "DYN_WIN/CHANNEL_VOL[0]", 5.0f) != null,
                     "ADD inserts CHANNEL_VOL[0]")

        // imgui_force_set lands on the new entry; other keys (none) unaffected.
        force_set_value(d, "DYN_WIN/CHANNEL_VOL[0]", JV(0.42f))
        t |> success(wait_for_payload_value(d, "DYN_WIN/CHANNEL_VOL[0]", "value", 0.42f, 300),
                     "imgui_force_set CHANNEL_VOL[0]=0.42 lands")

        // ADD again → [1] appears alongside [0].
        click(d, "DYN_WIN/ADD_CH")
        t |> success(wait_for_widget(d, "DYN_WIN/CHANNEL_VOL[1]", 5.0f) != null,
                     "second ADD inserts CHANNEL_VOL[1]")
        var snap_two = snapshot(d)
        t |> success(widget_exists(snap_two, "DYN_WIN/CHANNEL_VOL[0]"),
                     "[0] still present after second ADD")
        t |> equal(widget_payload_field(snap_two, "DYN_WIN/CHANNEL_VOL[0]", "value") ?? -1.0f,
                   0.42f, "[0].value still 0.42 (cross-key isolation)")

        // imgui_force_set on [1] doesn't perturb [0].
        force_set_value(d, "DYN_WIN/CHANNEL_VOL[1]", JV(0.91f))
        t |> success(wait_for_payload_value(d, "DYN_WIN/CHANNEL_VOL[1]", "value", 0.91f, 300),
                     "imgui_force_set CHANNEL_VOL[1]=0.91 lands")
        var snap_after_set1 = snapshot(d)
        t |> equal(widget_payload_field(snap_after_set1, "DYN_WIN/CHANNEL_VOL[0]", "value") ?? -1.0f,
                   0.42f, "[0].value unchanged after setting [1]")

        // REMOVE highest key → [1] vanishes, [0] stays.
        click(d, "DYN_WIN/REMOVE_CH")
        // §G expiration: indexed entry expires when last_seen_frame < g_frame-1.
        // Wait through a couple of frames for the snapshot to drop it.
        let dropped = wait_until(d, 300) $(var snap : JsonValue?) : bool {
            return !widget_exists(snap, "DYN_WIN/CHANNEL_VOL[1]")
        }
        t |> success(dropped != null, "REMOVE drops CHANNEL_VOL[1] from snapshot")
        var snap_after_remove = snapshot(d)
        t |> success(widget_exists(snap_after_remove, "DYN_WIN/CHANNEL_VOL[0]"),
                     "[0] still present after REMOVE of [1]")
        t |> equal(widget_payload_field(snap_after_remove, "DYN_WIN/CHANNEL_VOL[0]", "value") ?? -1.0f,
                   0.42f, "[0].value still 0.42 after REMOVE")

        // BUMP_LAST raises [0].value (only [0] remains).
        click(d, "DYN_WIN/BUMP_LAST")
        t |> success(wait_for_payload_value(d, "DYN_WIN/CHANNEL_VOL[0]", "value", 0.52f, 300),
                     "BUMP_LAST nudges [0].value by 0.1 (0.42 -> 0.52)")

        // ADD again → key 1 re-appears with fresh defaults (value=0.5).
        click(d, "DYN_WIN/ADD_CH")
        // Gate on the converged fresh-default value directly: existence-only
        // wait_for_widget can return before the first painted frame carries 0.5,
        // racing the snapshot read against the re-added entry's default.
        t |> success(wait_for_payload_value(d, "DYN_WIN/CHANNEL_VOL[1]", "value", 0.5f, 300),
                     "re-added [1].value is fresh default 0.5, not the prior 0.91")
    }
}

struct NamedDefaultsState {
    first : int
    caption : string
    extent : float2
}
var private NAMED_ROWS : table<int; NamedDefaultsState>
[widget]
def named_defaults_probe(var state : NamedDefaultsState; first : int = 7; caption : string = "default"; extent : float2 = float2(1, 2)) {
    state.first = first; state.caption = caption; state.extent = extent
    PopID()
}
[test(description = "An indexed or single [widget] call fills omitted parameters from their defaults and binds named-tuple arguments by name, mixed with positional ones in any order.")]
def indexed_named_arguments_skip_defaults_and_follow_names(t : T?) {
    let context = CreateContext(null)
    var io & = unsafe(GetIO())
    io.DisplaySize = float2(640, 480)
    io.BackendFlags |= ImGuiBackendFlags.RendererHasTextures
    NewFrame()
    window(TEST_WINDOW, (text = "test", closable = false, flags = ImGuiWindowFlags.None)) {
        named_defaults_probe(NAMED_ROWS[0], (extent = float2(30, 40)))
        named_defaults_probe(NAMED_ROWS[1], (caption = "second", first = 19))
        named_defaults_probe(NAMED_ROWS[2], 23, (extent = float2(50, 60)))
        named_defaults_probe(NAMED_ROWS[3], (caption = "named"), 31, float2(70, 80))
        named_defaults_probe(MIXED_SINGLE, 29, (extent = float2(90, 100)))
        t |> equal(NAMED_ROWS[2].first, 23)
        t |> equal(NAMED_ROWS[2].caption, "default")
        t |> equal(NAMED_ROWS[2].extent, float2(50, 60))
        t |> equal(NAMED_ROWS[3].first, 31)
        t |> equal(NAMED_ROWS[3].caption, "named")
        t |> equal(NAMED_ROWS[3].extent, float2(70, 80))
        t |> equal(MIXED_SINGLE.first, 29)
        t |> equal(MIXED_SINGLE.caption, "default")
        t |> equal(MIXED_SINGLE.extent, float2(90, 100))
        t |> equal(NAMED_ROWS[0].first, 7)
        t |> equal(NAMED_ROWS[0].caption, "default")
        t |> equal(NAMED_ROWS[0].extent, float2(30, 40))
        t |> equal(NAMED_ROWS[1].first, 19)
        t |> equal(NAMED_ROWS[1].caption, "second")
        t |> equal(NAMED_ROWS[1].extent, float2(1, 2))
    }
    EndFrame()
    clear(g_registry); clear(g_widgets); clear(NAMED_ROWS)
    DestroyContext(context)
}

let DEFAULTS_FEATURE = "modules/dasImgui/examples/features/widget_init_defaults.das"

[test(description = "A [widget] state global auto-emitted by the macro starts from its struct's field defaults, and force_set still steers it.", timeout = 1200)]
def test_widget_init_defaults(t : T?) {
    with_imgui_app(DEFAULTS_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/DEFAULTS", 15.0f)
        t |> success(snap0 != null, "DEFAULTS auto-emitted (registry has it)")
        if (snap0 == null) return

        // ===== Field defaults must fire =====
        // Every field below has a non-zero default in DefaultsState. Pre-fix the macro emitted
        // `var DEFAULTS : DefaultsState` (no init) and these came back zero; post-fix they round-trip.
        let count = widget_payload_field(snap0, "MAIN_WIN/DEFAULTS", "count") ?? 0
        t |> equal(count, 42, "count default fires (int = 42)")

        let ratio = widget_payload_field(snap0, "MAIN_WIN/DEFAULTS", "ratio") ?? 0.0f
        t |> equal(ratio, 1.5f, "ratio default fires (float = 1.5)")

        let caption = widget_payload_field(snap0, "MAIN_WIN/DEFAULTS", "caption") ?? ""
        t |> equal(caption, "default-label", "caption default fires (string = 'default-label')")

        let color = from_JV(widget_payload_field(snap0, "MAIN_WIN/DEFAULTS", "color"), type<float3>, float3(0.0f, 0.0f, 0.0f))
        t |> equal(color, float3(0.5f, 0.6f, 0.7f), "color default fires (float3 ctor)")

        let mode = from_JV(widget_payload_field(snap0, "MAIN_WIN/DEFAULTS", "mode"), type<ImGuiButtonFlags>, ImGuiButtonFlags.None)
        t |> equal(mode, ImGuiButtonFlags.MouseButtonRight, "mode default fires (enum value)")

        // ===== Live state writes still work — confirm the default-init global
        // didn't trip a const-shadow trap. imgui_force_set bumps count by 1 next frame.
        force_set_value(d, "MAIN_WIN/DEFAULTS", JV(0))
        t |> success(wait_for_int_value(d, "MAIN_WIN/DEFAULTS", "count", 43, 300),
                     "imgui_force_set still steers state after default-init")
    }
}
