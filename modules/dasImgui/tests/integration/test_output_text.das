options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 1.4 — text_show display widget.
//!
//! Drives `examples/features/output_text.das` to verify:
//!   1. text_show(IDENT, value=...) form keeps state.value pinned to the
//!      source-provided literal every frame (BANNER == "ready").
//!   2. text_show(IDENT) form starts empty; pressing the bump button
//!      writes "bumped at frame N" via direct daslang mutation; snapshot
//!      reflects the write next frame.
//!   3. imgui_set on a text_show target round-trips: write "remote" via
//!      imgui_set, snapshot returns "remote".

let OUTPUT_TEXT_FEATURE = "modules/dasImgui/examples/features/output_text.das"

[test]
def test_output_text_smoke(t : T?) {
    with_imgui_app(OUTPUT_TEXT_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/BANNER", 15.0f)
        t |> success(snap != null, "MAIN_WIN/BANNER renders")
        if (snap == null) return

        // Gate 1: source-init form keeps banner pinned to "ready".
        let banner = widget_payload_field(snap, "MAIN_WIN/BANNER", "value") ?? ""
        t |> equal(banner, "ready", "BANNER value pinned to source-init literal")

        // Gate 2: display-only form starts empty, then bump button writes it.
        let status0 = widget_payload_field(snap, "MAIN_WIN/STATUS_LINE", "value") ?? "<missing>"
        t |> equal(status0, "", "STATUS_LINE starts empty")

        click(d, "MAIN_WIN/BUMP_BTN")
        var snap_after_bump = wait_until(d, 300) $(var snap1) {
            let v = widget_payload_field(snap1, "MAIN_WIN/STATUS_LINE", "value") ?? ""
            return !empty(v)
        }
        t |> success(snap_after_bump != null,
                     "STATUS_LINE picks up daslang-side write after bump")

        // Gate 3: imgui_set on a text_show round-trips.
        set_value(d, "MAIN_WIN/STATUS_LINE", JV("remote"))
        t |> success(wait_for_string_value(d, "MAIN_WIN/STATUS_LINE", "value", "remote", 300),
                     "imgui_set writes 'remote' to STATUS_LINE.value")
    }
}
