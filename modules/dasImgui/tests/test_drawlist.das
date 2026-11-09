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

//! The drawlist rail: primitives, path keys, paths, channels, clips.

let DRAWLIST_FEATURE = "modules/dasImgui/examples/features/drawlist.das"

let EXPECTED_KINDS : array<string> = [
    // Original 8 primitives.
    "add_line", "add_rect", "add_rect_filled",
    "add_circle", "add_circle_filled",
    "add_triangle", "add_triangle_filled",
    "add_text",
    // v2 shape extras (added in Phase 1A).
    "add_rect_filled_multi_color",
    "add_ellipse", "add_ellipse_filled",
    "add_ngon", "add_ngon_filled",
    "add_polyline",
    "add_bezier_cubic", "add_bezier_quadratic"
]

def has_kind(var snap : JsonValue?; kind : string) : bool {
    var globals = snap?["globals"]
    return false if (globals == null || !(globals.value is _object))
    for (v in values(globals.value as _object)) {
        if ((v?["kind"] ?? "") == kind) return true
    }
    return false
}

[test(description = "Every add_* drawlist primitive, the original eight and the v2 shape extras, registers a snapshot entry of its kind.", timeout = 1200)]
def test_drawlist_primitives_smoke(t : T?) {
    with_imgui_app(DRAWLIST_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "DRAW_WIN", 15.0f)
        t |> success(snap0 != null, "DRAW_WIN registers (feature renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "DRAW_WIN")?["kind"] ?? "", "window",
                   "DRAW_WIN.kind == 'window'")
        // wait for the band-slot dummy so all the with_window_drawlist
        // calls above it have run at least once.
        var snap = wait_for_widget(d, "DRAW_WIN/DRAW_BAND_SLOT", 15.0f)
        t |> success(snap != null, "DRAW_BAND_SLOT renders (window drawlist body executed)")
        if (snap == null) return
        for (kind in EXPECTED_KINDS) {
            t |> success(has_kind(snap, kind),
                         "snapshot contains a `{kind}` primitive entry")
        }
    }
}

def is_drawlist_kind(kind : string) : bool {
    return false if (length(kind) < 5)
    return slice(kind, 0, 4) == "add_"
}

def has_modline_shape(s : string) : bool {
    //! Format: ``<mod>:<line>:<col>`` — at least one ':' separator plus
    //! numeric tail. We just check ': digit' appears so that synthetic
    //! macros aren't using sentinel idents like "anon" or empty strings.
    let colon = find(s, ":")
    return false if (colon < 0)
    let rest = slice(s, colon + 1)
    let colon2 = find(rest, ":")
    return colon2 > 0
}

[test(description = "Each drawlist primitive registers under a <mod>:<line>:<col> path key with a bbox and never as a __widget_<line>_<col> state global.", timeout = 1200)]
def test_drawlist_path_key_shape(t : T?) {
    with_imgui_app(DRAWLIST_FEATURE) $(d) {
        var snap = wait_for_widget(d, "DRAW_WIN/DRAW_BAND_SLOT", 15.0f)
        t |> success(snap != null, "feature renders")
        if (snap == null) return
        var globals = snap?["globals"]
        t |> success(globals != null && globals.value is _object,
                     "snapshot.globals is a json object")
        if (globals == null || !(globals.value is _object)) return
        var seen_drawlist = 0
        var bad_shape = ""
        var bad_bbox = ""
        for (k, v in keys(globals.value as _object), values(globals.value as _object)) {
            let kind = v?["kind"] ?? ""
            if (!is_drawlist_kind(kind)) continue
            seen_drawlist++
            // Strip container prefix (anything before the last '/')
            let tail = k
            var leaf = tail
            let last_slash = rfind(tail, "/")
            if (last_slash >= 0) {
                leaf = slice(tail, last_slash + 1)
            }
            if (!has_modline_shape(leaf) && length(bad_shape) < 200) {
                bad_shape = "{bad_shape}{leaf};"
            }
            let bbox = v?["bbox"]
            if (bbox == null && length(bad_bbox) < 200) {
                bad_bbox = "{bad_bbox}{k}(no bbox);"
            }
        }
        t |> success(seen_drawlist >= 8,
                     "saw at least 8 drawlist primitive entries (got {seen_drawlist})")
        t |> equal(bad_shape, "",
                   "every drawlist primitive path-key leaf matches `<mod>:<line>:<col>`")
        t |> equal(bad_bbox, "",
                   "every drawlist primitive has a bbox field")
        // No state-global emission for DRAWLIST primitives: assert no drawlist-kind path leaf
        // matches `__widget_<line>_<col>`. Sibling [widget] calls legitimately synthesize that
        // form (e.g. separator's Form 4), so we filter on kind first to test only the drawlist rail.
        var state_global_leaks = ""
        for (k, v in keys(globals.value as _object), values(globals.value as _object)) {
            let kind = v?["kind"] ?? ""
            if (!is_drawlist_kind(kind)) continue
            var leaf = k
            let last_slash = rfind(k, "/")
            if (last_slash >= 0) {
                leaf = slice(k, last_slash + 1)
            }
            if (length(leaf) > 9 && slice(leaf, 0, 9) == "__widget_" && length(state_global_leaks) < 200) {
                state_global_leaks = "{state_global_leaks}{leaf};"
            }
        }
        t |> equal(state_global_leaks, "",
                   "no drawlist primitive leaks a `__widget_<line>_<col>` state-global path")
    }
}

def count_primitives_under(var snap : JsonValue?; prefix : string) : int {
    var n = 0
    var globals = snap?["globals"]
    return n if (globals == null || !(globals.value is _object))
    for (k, v in keys(globals.value as _object), values(globals.value as _object)) {
        let kind = v?["kind"] ?? ""
        if (length(kind) > 4 && slice(kind, 0, 4) == "add_") {
            if (empty(prefix)) {
                n++ if (find(k, "/") == -1)
            } else {
                n++ if (length(k) > length(prefix) && slice(k, 0, length(prefix)) == prefix)
            }
        }
    }
    return n
}

[test(description = "Primitives drawn through with_window_drawlist inside a window nest under its path, while foreground and background drawlist primitives outside it register unprefixed.", timeout = 1200)]
def test_with_window_drawlist_nests(t : T?) {
    with_imgui_app(DRAWLIST_FEATURE) $(d) {
        var snap = wait_for_widget(d, "DRAW_WIN/DRAW_BAND_SLOT", 15.0f)
        t |> success(snap != null, "feature renders")
        if (snap == null) return
        // with_window_drawlist runs INSIDE the window() block, so every primitive call site nests
        // under DRAW_WIN. The feature exercises 8 primitives; expect ≥5 to land (a stable floor
        // that survives small feature edits).
        let nested = count_primitives_under(snap, "DRAW_WIN/")
        t |> success(nested >= 5,
                     "with_window_drawlist primitives nest under DRAW_WIN/ (got {nested})")
        // with_foreground_drawlist / with_background_drawlist run OUTSIDE the window() block here,
        // so their primitives register at top level (no container prefix). Foreground + background
        // each fire one add_rect_filled — expect ≥2 unnested entries.
        let unnested = count_primitives_under(snap, "")
        t |> success(unnested >= 2,
                     "with_foreground_drawlist / with_background_drawlist primitives are unnested (got {unnested})")
    }
}

let CHANNELS_FEATURE = "modules/dasImgui/examples/features/drawlist_channels.das"


[test(description = "Primitives drawn inside a channels_split / set_current / merge sequence register, and the slot after the split renders, so the channel stack drains cleanly.", timeout = 1200)]
def test_drawlist_channels_renders(t : T?) {
    with_imgui_app(CHANNELS_FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/LEAD", 15.0f)
        t |> success(snap != null, "MAIN_WIN/LEAD renders (window body opened)")
        if (snap == null) return
        t |> success(widget_exists(snap, "MAIN_WIN/CARD_SLOT"),
                     "MAIN_WIN/CARD_SLOT registers (split body completed; merge fired)")
        t |> success(has_kind(snap, "add_text"),
                     "add_text on channel 1 reaches the registry")
        t |> success(has_kind(snap, "add_rect_filled"),
                     "add_rect_filled on channel 0 reaches the registry")
    }
}

let PATH_FEATURE = "modules/dasImgui/examples/features/drawlist_path.das"

[test(description = "A body running every path_* call, filled convex and concave and stroked, renders through to the slot after it without a path-stack assert.", timeout = 1200)]
def test_drawlist_path_renders(t : T?) {
    with_imgui_app(PATH_FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/LEAD", 15.0f)
        t |> success(snap != null,
                     "MAIN_WIN/LEAD renders (path body reached the trailing text)")
        if (snap == null) return
        t |> success(widget_exists(snap, "MAIN_WIN/PATH_SLOT"),
                     "MAIN_WIN/PATH_SLOT registers (dummy after the path body completed)")
    }
}

let CLIP_RECT_FEATURE = "modules/dasImgui/examples/features/clip_rect.das"

[test(description = "A dummy inside with_clip_rect and the text after the clip block both register.", timeout = 1200)]
def test_clip_rect_smoke(t : T?) {
    with_imgui_app(CLIP_RECT_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/LEAD", 15.0f)
        t |> success(snap != null, "LEAD renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/GUARD"),
                     "dummy GUARD inside clip block registers")
        t |> success(widget_exists(snap, "MAIN_WIN/NOTE"),
                     "NOTE after clip block registers")
    }
}
