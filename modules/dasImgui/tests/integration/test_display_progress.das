options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 progress_bar.
//! Drives `examples/features/display_progress.das` and verifies the driven
//! bar's fraction is non-zero (animation hooked up).

let FEATURE = "modules/dasImgui/examples/features/display_progress.das"

[test]
def test_display_progress_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/PB_STATIC", 15.0f)
        t |> success(snap != null, "PB_STATIC renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/PB_STATIC"), "PB_STATIC")
        t |> success(widget_exists(snap, "MAIN_WIN/PB_DRIVEN"), "PB_DRIVEN")
        t |> success(widget_exists(snap, "MAIN_WIN/PB_PLAIN"), "PB_PLAIN")

        let static_frac = double(widget_payload_field(snap, "MAIN_WIN/PB_STATIC", "fraction") ?? 0.0)
        t |> success(static_frac > 0.3lf && static_frac < 0.4lf,
                     "PB_STATIC fraction ≈ 0.33 ({static_frac})")
    }
}
