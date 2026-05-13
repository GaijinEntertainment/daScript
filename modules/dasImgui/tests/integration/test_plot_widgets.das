options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 1.5 — plot_lines + plot_histogram (array form).
//!
//! Drives `examples/features/plot_widgets.das` to verify:
//!   1. Both plot widgets register and render under nested paths.
//!   2. Telemetry fields land: title (string), bounds (tuple), samples (int).
//!   3. Sample counts match the source-driven array sizes (64 for sine, 8
//!      for histogram).

let PLOT_FEATURE = "modules/dasImgui/examples/features/plot_widgets.das"

[test]
def test_plot_widgets_smoke(t : T?) {
    with_imgui_app(PLOT_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/SINE_WAVE", 15.0f)
        t |> success(snap != null, "MAIN_WIN/SINE_WAVE renders")
        if (snap == null) return

        // Gate 1: kinds and presence.
        var sine = find_widget(snap, "MAIN_WIN/SINE_WAVE")
        t |> equal(sine?["kind"] ?? "", "plot_lines",
                   "SINE_WAVE.kind = plot_lines")
        var hist = find_widget(snap, "MAIN_WIN/HISTOGRAM")
        t |> equal(hist?["kind"] ?? "", "plot_histogram",
                   "HISTOGRAM.kind = plot_histogram")

        // Gate 2: payload fields.
        let sine_title = widget_payload_field(snap, "MAIN_WIN/SINE_WAVE", "title") ?? ""
        t |> equal(sine_title, "Sine wave", "SINE_WAVE.payload.title")
        let sine_samples = widget_payload_field(snap, "MAIN_WIN/SINE_WAVE", "samples") ?? 0
        t |> equal(sine_samples, 64, "SINE_WAVE.payload.samples = 64")

        let hist_samples = widget_payload_field(snap, "MAIN_WIN/HISTOGRAM", "samples") ?? 0
        t |> equal(hist_samples, 8, "HISTOGRAM.payload.samples = 8")

        // Gate 3: bounds is a tuple — check its serialized shape.
        var bounds = widget_payload_field(snap, "MAIN_WIN/SINE_WAVE", "bounds")
        t |> success(bounds != null, "SINE_WAVE.payload.bounds present")
    }
}
