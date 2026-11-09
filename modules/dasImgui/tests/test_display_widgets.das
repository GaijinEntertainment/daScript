options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require strings
require imgui/imgui_icons public

//! Display widgets: image, progress, plots, list boxes, icons.

let DISPLAY_IMAGE_FEATURE = "modules/dasImgui/examples/features/display_image.das"

[test(description = "Three image() call sites, plain, half-size and tinted, each register once the font atlas is built.", timeout = 1200)]
def test_display_image_smoke(t : T?) {
    with_imgui_app(DISPLAY_IMAGE_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/IMG_BASIC", 15.0f)
        t |> success(snap != null, "IMG_BASIC renders (font atlas built)")
        if (snap == null) return

        for (ident in ["IMG_BASIC", "IMG_HALF", "IMG_TINTED"]) {
            let path = "MAIN_WIN/{ident}"
            t |> success(widget_exists(snap, path), "{path} registered")
        }
    }
}

let DISPLAY_PROGRESS_FEATURE = "modules/dasImgui/examples/features/display_progress.das"

[test(description = "The static, driven and plain progress bars register, and the static bar reports its fraction near 0.33.", timeout = 1200)]
def test_display_progress_smoke(t : T?) {
    with_imgui_app(DISPLAY_PROGRESS_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/PB_STATIC", 15.0f)
        t |> success(snap != null, "PB_STATIC renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/PB_STATIC"), "PB_STATIC")
        t |> success(widget_exists(snap, "MAIN_WIN/PB_DRIVEN"), "PB_DRIVEN")
        t |> success(widget_exists(snap, "MAIN_WIN/PB_PLAIN"), "PB_PLAIN")

        let static_frac = double(widget_payload_field(snap, "MAIN_WIN/PB_STATIC", "fraction") ?? 0.0)
        t |> success(static_frac > 0.3lf && static_frac < 0.4lf,
                     "PB_STATIC fraction ~0.33 ({static_frac})")
    }
}

let PLOT_FEATURE = "modules/dasImgui/examples/features/plot_widgets.das"

[test(description = "plot_lines and plot_histogram over arrays register their kinds with title, bounds and sample counts matching the source arrays.", timeout = 1200)]
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
        let bounds = widget_payload_field(snap, "MAIN_WIN/SINE_WAVE", "bounds")
        t |> success(bounds != null, "SINE_WAVE.payload.bounds present")
    }
}

let PLOT_GETTER_FEATURE = "modules/dasImgui/examples/features/plot_getter.das"

[test(description = "plot_lines_getter and plot_histogram_getter render through their per-sample lambda shims and report their kinds and 64 samples each.", timeout = 1200)]
def test_plot_getters_render(t : T?) {
    with_imgui_app(PLOT_GETTER_FEATURE) $(d) {
        var snap0 = wait_for_render(d, "MAIN_WIN/SINE_PLOT", 15.0f)
        t |> success(snap0 != null, "SINE_PLOT registers (snapshot reachable)")
        if (snap0 == null) return
        t |> equal(snap0?["globals"]?["MAIN_WIN/SINE_PLOT"]?["kind"] ?? "", "plot_lines_getter",
                   "SINE_PLOT.kind = plot_lines_getter")
        t |> equal(snap0?["globals"]?["MAIN_WIN/SINE_PLOT"]?["payload"]?["samples"] ?? -1, 64,
                   "SINE_PLOT.payload.samples = 64")

        var snap1 = wait_for_render(d, "MAIN_WIN/BARS_PLOT", 15.0f)
        t |> success(snap1 != null, "BARS_PLOT reachable")
        if (snap1 == null) return
        t |> equal(snap1?["globals"]?["MAIN_WIN/BARS_PLOT"]?["kind"] ?? "", "plot_histogram_getter",
                   "BARS_PLOT.kind = plot_histogram_getter")
        t |> equal(snap1?["globals"]?["MAIN_WIN/BARS_PLOT"]?["payload"]?["samples"] ?? -1, 64,
                   "BARS_PLOT.payload.samples = 64")
    }
}

let LIST_BOX_FEATURE = "modules/dasImgui/examples/features/list_box.das"

[test(description = "The list_box container registers its kind and force_set steers its selected index, which persists in later snapshots.", timeout = 1200)]
def test_list_box_set_steers_selection(t : T?) {
    with_imgui_app(LIST_BOX_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/LANG", 15.0f)
        t |> success(snap0 != null, "LANG list_box registers")
        if (snap0 == null) return

        let lang_entry = find_widget(snap0, "MAIN_WIN/LANG")
        t |> equal(lang_entry?["kind"] ?? "", "list_box",
                   "LANG.kind == 'list_box'")

        // imgui_force_set steers selection to "Rust" (index 2).
        force_set_value(d, "MAIN_WIN/LANG", JV(2))
        t |> success(wait_for_int_value(d, "MAIN_WIN/LANG", "value", 2, 300),
                     "imgui_force_set steers LANG to index 2 (Rust)")

        // Another step — Python (4).
        force_set_value(d, "MAIN_WIN/LANG", JV(4))
        t |> success(wait_for_int_value(d, "MAIN_WIN/LANG", "value", 4, 300),
                     "imgui_force_set steers LANG to index 4 (Python)")

        // Final snapshot envelope.
        var snap_final = snapshot(d)
        let lang_final = find_widget(snap_final, "MAIN_WIN/LANG")
        t |> equal(lang_final?["payload"]?["value"] ?? -1, 4,
                   "LANG.payload.value persists final imgui_force_set")
    }
}

let LIST_CLIPPER_FEATURE = "modules/dasImgui/examples/features/list_clipper.das"

[test(description = "list_clipper over 1000 rows submits only the visible chunk: more than 5 and fewer than 200 rows register.", timeout = 1200)]
def test_list_clipper_culling(t : T?) {
    with_imgui_app(LIST_CLIPPER_FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/SCROLL_AREA", 15.0f)
        t |> success(snap != null, "SCROLL_AREA child registers")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/LEAD"), "LEAD registers")
        t |> success(widget_exists(snap, "MAIN_WIN/SCROLL_AREA/CLIPPED_ROW[0]"),
                     "CLIPPED_ROW[0] in initial visible chunk")

        // Count globals keys matching the clipped-row prefix. list_clipper is
        // imperative (not a [widget]), so we walk the globals map directly.
        let g = snap?["globals"]
        var clipped_count = 0
        if (g != null) {
            let obj & = unsafe(g as _object)
            for (k in keys(obj)) {
                if (k |> starts_with("MAIN_WIN/SCROLL_AREA/CLIPPED_ROW[")) {
                    clipped_count++
                }
            }
        }
        t |> success(clipped_count > 5,
                     "list_clipper rendered something ({clipped_count} rows of 1000)")
        t |> success(clipped_count < 200,
                     "list_clipper culling active ({clipped_count} rows << 1000)")
    }
}

[test(description = "Every glyph the markdown toolbar maps is registered in the icon catalog.")]
def test_icon_text_markup_set(t : T?) {
    // the markdown toolbar glyphs - a rename here breaks the
    // viewer's MD_TOOLBAR_ICONS mapping silently (draw_icon no-ops on misses)
    var names : table<string>
    for (i in range(icon_count())) {
        names |> insert(icon_name(i))
    }
    var missing = 0
    for (nm in ["heading", "bold", "italic", "code-inline", "image",
                "list-bullets", "list-ordered", "checkbox", "quote",
                "code-block", "table"]) {
        if (!key_exists(names, nm)) {
            missing++
        }
    }
    t |> equal(missing, 0, "every text-markup glyph is registered")
}

[test(description = "Every icon in the catalog has a unique, non-empty name and a category.")]
def test_icon_catalog(t : T?) {
    t |> success(icon_count() > 0, "icon set is non-empty")
    var seen : table<string>
    var bad = 0
    for (i in range(icon_count())) {
        let nm = icon_name(i)
        if (empty(nm) || empty(icon_category(i)) || key_exists(seen, nm)) {
            bad++
        }
        seen |> insert(nm)
    }
    t |> equal(bad, 0, "every icon has a unique, non-empty name and a category")
}
