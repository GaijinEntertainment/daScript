options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require imgui public
require imgui/imgui_theme_daslang

//! Style, font and item-flag scopes, and theme scaling.

let STYLE_FEATURE = "modules/dasImgui/examples/features/style_override.das"

//! An unbalanced pop asserts at EndFrame and kills the host
[test(description = "Buttons inside single and nested with_style blocks and after them all register, so the style pushes balance, and the styled buttons still count clicks.", timeout = 1200)]
def test_with_style_smoke(t : T?) {
    with_imgui_app(STYLE_FEATURE) $(d) {
        // All three buttons must reach the snapshot: STYLED_BTN under a 2-style with_style,
        // NESTED_STYLED_BTN under nested blocks, UNSTYLED_BTN AFTER all blocks (proves pops
        // balanced — otherwise EndFrame asserts and the subprocess dies before this snapshot).
        var snap0 = wait_for_widget(d, "STYLE_WIN/UNSTYLED_BTN", 15.0f)
        t |> success(snap0 != null, "post-with_style button registers (pop count balanced)")
        if (snap0 == null) return

        t |> success(widget_exists(snap0, "STYLE_WIN/STYLED_BTN"),
                     "STYLED_BTN inside 2-style with_style registers")
        t |> success(widget_exists(snap0, "STYLE_WIN/NESTED_STYLED_BTN"),
                     "NESTED_STYLED_BTN inside nested with_style registers")

        // Styled buttons stay interactive — a real synthetic click works through
        // the synthesized push/invoke/pop body.
        click(d, "STYLE_WIN/STYLED_BTN")
        t |> success(wait_for_int_value(d, "STYLE_WIN/STYLED_BTN", "click_count", 1, 300),
                     "STYLED_BTN.click_count increments after click")

        click(d, "STYLE_WIN/NESTED_STYLED_BTN")
        t |> success(wait_for_int_value(d, "STYLE_WIN/NESTED_STYLED_BTN", "click_count", 1, 300),
                     "NESTED_STYLED_BTN.click_count increments after click")
    }
}

let DISABLED_FEATURE = "modules/dasImgui/examples/features/disabled_block.das"

[test(description = "Widgets inside and after a with_disabled block all register whatever the disabled gate says.", timeout = 1200)]
def test_disabled_block_smoke(t : T?) {
    with_imgui_app(DISABLED_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/BTN_INSIDE", 15.0f)
        t |> success(snap != null, "BTN_INSIDE renders")
        if (snap == null) return

        for (ident in ["GATE_DISABLED", "BTN_INSIDE", "CB_INSIDE",
                       "SLIDER_INSIDE", "BTN_OUTSIDE"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registers regardless of disabled gate")
        }
    }
}

let FONT_STACK_FEATURE = "modules/dasImgui/examples/features/font_stack.das"

[test(description = "The text and button inside a with_font block render.", timeout = 1200)]
def test_font_stack_smoke(t : T?) {
    with_imgui_app(FONT_STACK_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/INSIDE", 15.0f)
        t |> success(snap != null, "with_font block body renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/INSIDE"), "inside text")
        t |> success(widget_exists(snap, "MAIN_WIN/BTN_INSIDE"), "inside button")
    }
}

let DYNAMIC_FONT_FEATURE = "modules/dasImgui/examples/features/dynamic_font_scale.das"

[test(description = "Text rows drawn under with_font at 16, 24 and 32 pixels report strictly taller bboxes, so the per-push size reaches PushFont.", timeout = 1200)]
def test_dynamic_font_scale_smoke(t : T?) {
    with_imgui_app(DYNAMIC_FONT_FEATURE) $(d) {
        var snap = wait_for_render(d, "FONT_WIN/SIZE_32", 15.0f)
        t |> success(snap != null, "sized text rows render")
        if (snap == null) return

        t |> success(widget_exists(snap, "FONT_WIN/GLOBAL_SCALE"), "global-scale slider registers")

        let bb16 = find_widget(snap, "FONT_WIN/SIZE_16")?["bbox"]
        let bb24 = find_widget(snap, "FONT_WIN/SIZE_24")?["bbox"]
        let bb32 = find_widget(snap, "FONT_WIN/SIZE_32")?["bbox"]
        let h16 = (bb16?["w"] ?? 0.0f) - (bb16?["y"] ?? 0.0f)
        let h24 = (bb24?["w"] ?? 0.0f) - (bb24?["y"] ?? 0.0f)
        let h32 = (bb32?["w"] ?? 0.0f) - (bb32?["y"] ?? 0.0f)
        t |> success(h16 > 0.0f, "16px row has a bbox ({h16})")
        t |> success(h24 > h16, "24px row taller than 16px ({h24} > {h16})")
        t |> success(h32 > h24, "32px row taller than 24px ({h32} > {h24})")
    }
}

let BUTTON_REPEAT_FEATURE = "modules/dasImgui/examples/features/button_repeat.das"

[test(description = "Buttons inside and after a with_button_repeat block register, the one after it with no clicks counted.", timeout = 1200)]
def test_button_repeat_smoke(t : T?) {
    with_imgui_app(BUTTON_REPEAT_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/BTN_INSIDE", 15.0f)
        t |> success(snap != null, "BTN_INSIDE registers")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/BTN_OUTSIDE"),
                     "fire-once button also registers")
        // The repeat-vs-fire-once behavioral difference requires hold-time
        // input; record_button_repeat.das exercises that. Smoke gate is
        // just existence + initial click_count == 0.
        let outside_clicks = widget_payload_field(snap, "MAIN_WIN/BTN_OUTSIDE", "click_count") ?? 0
        t |> equal(outside_clicks, 0, "outside button has not been clicked yet")
    }
}

let WITH_TAB_STOP_FEATURE = "modules/dasImgui/examples/features/with_tab_stop.das"

[test(description = "The with_tab_stop demo renders its window and the field wrapped in the no-tab-stop block.", timeout = 1200)]
def test_with_tab_stop_smoke(t : T?) {
    with_imgui_app(WITH_TAB_STOP_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "TS_WIN", 15.0f)
        t |> success(snap0 != null, "TS_WIN registers (demo renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "TS_WIN")?["kind"] ?? "", "window",
                   "TS_WIN.kind == 'window'")
        let snap_skip = wait_for_widget(d, "TS_WIN/TS_FIELD_SKIP", 15.0f)
        t |> success(snap_skip != null, "TS_FIELD_SKIP renders inside with_tab_stop block")
    }
}

[test(description = "apply_daslang_theme sets the documented 1x paddings, spacings, roundings and sizes.")]
def test_apply_daslang_theme_base_constants(t : T?) {
    let ctx = CreateContext(null)
    SetCurrentContext(ctx)
    apply_daslang_theme()
    let style & = unsafe(GetStyle())
    t |> equal(style.WindowPadding.x, 8.0f, "WindowPadding.x at scale=1")
    t |> equal(style.WindowPadding.y, 8.0f, "WindowPadding.y at scale=1")
    t |> equal(style.FramePadding.x, 6.0f, "FramePadding.x at scale=1")
    t |> equal(style.FramePadding.y, 3.0f, "FramePadding.y at scale=1")
    t |> equal(style.ItemSpacing.x, 8.0f, "ItemSpacing.x at scale=1")
    t |> equal(style.ItemSpacing.y, 5.0f, "ItemSpacing.y at scale=1")
    t |> equal(style.WindowRounding, 2.0f, "WindowRounding at scale=1")
    t |> equal(style.FrameRounding, 2.0f, "FrameRounding at scale=1")
    t |> equal(style.ScrollbarSize, 10.0f, "ScrollbarSize at scale=1")
    t |> equal(style.GrabMinSize, 10.0f, "GrabMinSize at scale=1")
    DestroyContext(ctx)
}

[test(description = "ScaleAllSizes(2) on the daslang theme doubles paddings, spacings, roundings and sizes and leaves border widths at 1.")]
def test_scale_all_sizes_doubles_at_2x(t : T?) {
    let ctx = CreateContext(null)
    SetCurrentContext(ctx)
    apply_daslang_theme()
    var style & = unsafe(GetStyle())
    style |> ScaleAllSizes(2.0f)
    t |> equal(style.WindowPadding.x, 16.0f, "WindowPadding.x scaled 2x")
    t |> equal(style.WindowPadding.y, 16.0f, "WindowPadding.y scaled 2x")
    t |> equal(style.FramePadding.x, 12.0f, "FramePadding.x scaled 2x")
    t |> equal(style.FramePadding.y, 6.0f, "FramePadding.y scaled 2x")
    t |> equal(style.ItemSpacing.x, 16.0f, "ItemSpacing.x scaled 2x")
    t |> equal(style.ItemSpacing.y, 10.0f, "ItemSpacing.y scaled 2x")
    t |> equal(style.WindowRounding, 4.0f, "WindowRounding scaled 2x")
    t |> equal(style.FrameRounding, 4.0f, "FrameRounding scaled 2x")
    t |> equal(style.ScrollbarSize, 20.0f, "ScrollbarSize scaled 2x")
    t |> equal(style.GrabMinSize, 20.0f, "GrabMinSize scaled 2x")
    // Border widths intentionally untouched by ScaleAllSizes — stock ImGui
    // behavior. Hairline 1px borders on retina are a deliberate choice; we
    // do not override it.
    t |> equal(style.WindowBorderSize, 1.0f, "WindowBorderSize unscaled (stock ImGui)")
    DestroyContext(ctx)
}
