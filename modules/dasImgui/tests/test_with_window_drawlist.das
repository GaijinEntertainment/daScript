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

//! Path-key nesting for the drawlist rail. Targets
//! ``examples/features/drawlist.das``. Telemetry nesting follows the
//! call-site's lexical container scope, NOT the drawlist that gets painted
//! (window / foreground / background drawlists are viewport-level; visual
//! clipping has no effect on registry path keys).
//!
//! Feature exercises both shapes:
//!   - inside ``window(DRAW_WIN, ...) { with_window_drawlist() $(dl) { ... } }``
//!     — every primitive's path key is nested under ``DRAW_WIN/``.
//!   - outside the window block, ``with_foreground_drawlist() $(dl) { ... }``
//!     and ``with_background_drawlist() $(dl) { ... }`` — primitives there
//!     have a top-level path key with no container prefix.

let DRAWLIST_FEATURE = "modules/dasImgui/examples/features/drawlist.das"

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

[test]
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
