options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 2.7 — plot getters.
//!
//! `plot_lines_getter` / `plot_histogram_getter` route through the new
//! `_builtin_PlotLines_cb` / `_builtin_PlotHistogram_cb` C++ shims, which
//! invoke a daslang lambda once per sample to produce values. Snapshot
//! payload mirrors the existing plot_lines/histogram shape (kind +
//! samples + bounds), so the gates verify the kind label and sample count
//! after a render cycle.
//!
//! Gates:
//!   1. SINE_PLOT renders, snapshot.kind == "plot_lines_getter".
//!   2. SINE_PLOT.payload.samples == 64 (matches `values_count` arg).
//!   3. BARS_PLOT renders, snapshot.kind == "plot_histogram_getter".
//!   4. BARS_PLOT.payload.samples == 64.

let PLOT_GETTER_FEATURE = "modules/dasImgui/examples/features/plot_getter.das"

[test]
def test_plot_getters_render(t : T?) {
    with_imgui_app(PLOT_GETTER_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SINE_PLOT", 15.0f)
        t |> success(snap0 != null, "SINE_PLOT registers (snapshot reachable)")
        if (snap0 == null) return
        t |> equal(snap0?["globals"]?["SINE_PLOT"]?["kind"] ?? "", "plot_lines_getter",
                   "SINE_PLOT.kind = plot_lines_getter")
        t |> equal(snap0?["globals"]?["SINE_PLOT"]?["payload"]?["samples"] ?? -1, 64,
                   "SINE_PLOT.payload.samples = 64")

        var snap1 = wait_for_widget(d, "BARS_PLOT", 15.0f)
        t |> success(snap1 != null, "BARS_PLOT reachable")
        if (snap1 == null) return
        t |> equal(snap1?["globals"]?["BARS_PLOT"]?["kind"] ?? "", "plot_histogram_getter",
                   "BARS_PLOT.kind = plot_histogram_getter")
        t |> equal(snap1?["globals"]?["BARS_PLOT"]?["payload"]?["samples"] ?? -1, 64,
                   "BARS_PLOT.payload.samples = 64")
    }
}
