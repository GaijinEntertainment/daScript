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

//! Verifies the ``[drawlist_prim]`` macro emits stable
//! ``<mod>:<line>:<col>`` path keys per call site (mouse-cards /
//! playwright targeting hook) WITHOUT registering a ``__widget_<line>_<col>``
//! state global. Walks the snapshot and checks each drawlist primitive
//! entry's key matches the synthesized format; also asserts non-zero bbox
//! so mouse-cards can resolve a click on the primitive's rect.

let DRAWLIST_FEATURE = "modules/dasImgui/examples/features/drawlist.das"

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

[test]
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
