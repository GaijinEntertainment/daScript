options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration coverage for the two recording-sync primitives added in dasImgui PR #186:
//! `wait_for_hover` and `record_check`. Uses the `color_button_hover` feature — the CBH_RED
//! swatch carries the per-widget IsItemHovered telemetry (the `hover` field) that wait_for_hover
//! polls. One app spawn covers both (record_check's `app` arg is unused).

let FEATURE = "modules/dasImgui/examples/features/color_button_hover.das"
let TARGET = "CBH_WIN/CBH_RED"

[test]
def test_recording_sync(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, TARGET, 15.0f)
        t |> success(snap != null, "{TARGET} renders")
        if (snap == null) return

        // ---- wait_for_hover ----
        // Move onto the swatch — it must report hovered (the move -> wait_for_hover -> click gate).
        move_to(d, widget_click_point(snap, TARGET))
        t |> success(wait_for_hover(d, TARGET) != null, "wait_for_hover registers hover after move_to")

        // Move off — hover must clear (expected=false on a PRESENT widget).
        move_to(d, (4.0f, 4.0f))
        t |> success(wait_for_hover(d, TARGET, false) != null, "wait_for_hover sees hover clear after moving away")

        // Regression for the #186-A false-pass: a missing/misspelled ident must NOT pass as "not
        // hovered". The old predicate coalesced null hover to false and returned on the first poll;
        // the widget_exists guard makes it time out instead (tiny timeout still separates the two).
        t |> success(wait_for_hover(d, "CBH_WIN/__no_such_widget__", false, 0.3f) == null,
                     "missing widget does not false-pass wait_for_hover(expected=false)")

        // ---- record_check ----
        // Returns its `ok` argument and accumulates (does NOT panic) on false; reaching the line
        // after the false call proves it didn't panic mid-body.
        t |> success(record_check(d, "ok branch returns true", true), "record_check returns true when ok")
        let failed = record_check(d, "intentional miss (accumulated, not panicked)", false)
        t |> success(!failed, "record_check returns false and accumulates instead of panicking")
    }
}
