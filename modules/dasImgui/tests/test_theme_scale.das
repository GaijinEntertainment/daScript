options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui public
require imgui/imgui_theme_daslang

//! Unit smoke for HDPI theme scaling. Asserts the documented 1x logical
//! constants land where ``apply_daslang_theme`` says they do, then runs
//! ``ScaleAllSizes(2.0)`` (the same call ``live_imgui_init`` invokes when
//! ``glfwGetWindowContentScale`` reports 2x — macOS retina, Windows 200%)
//! and asserts paddings/spacings/rounding double. Border sizes intentionally
//! do NOT scale — stock ``ImGuiStyle::ScaleAllSizes`` skips them, and we
//! match that.

[test]
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

[test]
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
