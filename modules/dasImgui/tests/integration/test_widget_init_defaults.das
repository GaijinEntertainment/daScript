options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require imgui public
require daslib/json public
require daslib/json_boost public

//! Integration test for [widget] auto-emit's call to the state struct's
//! default constructor. Drives `examples/features/widget_init_defaults.das`
//! to verify that `field : T = default` declarations on the state struct
//! actually fire on the auto-emitted module global — i.e. the macro emits
//! `var IDENT : T = T()` (ExprMakeStruct(useInitializer)), not the bare
//! `var IDENT : T` it emitted prior to this fix.

let DEFAULTS_FEATURE = "modules/dasImgui/examples/features/widget_init_defaults.das"

[test]
def test_widget_init_defaults(t : T?) {
    with_imgui_app(DEFAULTS_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "DEFAULTS", 15.0f)
        t |> success(snap0 != null, "DEFAULTS auto-emitted (registry has it)")
        if (snap0 == null) return

        // ===== Field defaults must fire — every field below has a non-zero
        // default in DefaultsState. Pre-fix, the macro emitted `var DEFAULTS :
        // DefaultsState` (no init); these all came back as zero/empty. Post-fix
        // they round-trip through JV(state) -> snapshot.
        let count = widget_payload_field(snap0, "DEFAULTS", "count") ?? 0
        t |> equal(count, 42, "count default fires (int = 42)")

        let ratio = widget_payload_field(snap0, "DEFAULTS", "ratio") ?? 0.0f
        t |> equal(ratio, 1.5f, "ratio default fires (float = 1.5)")

        let caption = widget_payload_field(snap0, "DEFAULTS", "caption") ?? ""
        t |> equal(caption, "default-label", "caption default fires (string = 'default-label')")

        let color = from_JV(widget_payload_field(snap0, "DEFAULTS", "color"), type<float3>, float3(0.0f, 0.0f, 0.0f))
        t |> equal(color, float3(0.5f, 0.6f, 0.7f), "color default fires (float3 ctor)")

        let mode = from_JV(widget_payload_field(snap0, "DEFAULTS", "mode"), type<ImGuiButtonFlags>, ImGuiButtonFlags.None)
        t |> equal(mode, ImGuiButtonFlags.MouseButtonRight, "mode default fires (enum value)")

        // ===== Live state writes still work — confirm the default-init global
        // didn't trip a const-shadow trap. imgui_set bumps count by 1 next frame.
        set_value(d, "DEFAULTS", JV(0))
        t |> success(wait_for_int_value(d, "DEFAULTS", "count", 43, 300),
                     "imgui_set still steers state after default-init")
    }
}
