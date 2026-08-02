options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 0b.2 Color* widgets — every editor / picker
//! / swatch kind, driven through `imgui_force_set` (RGB / RGBA) and `imgui_click`
//! (color_button) via the playwright-style harness.

let COLOR_FEATURE = "modules/dasImgui/examples/features/inputs_color.das"

[test]
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
