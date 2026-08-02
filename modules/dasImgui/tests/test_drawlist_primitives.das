options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for the drawlist rail's shape primitives — 8
//! originals + 8 v2 extras (rect_filled_multi_color / ellipse / ngon /
//! polyline / bezier). Targets the ``examples/features/drawlist.das``
//! feature which exercises one call of each ``add_*`` primitive under
//! ``DRAW_WIN``. Walks the per-frame snapshot and asserts every
//! primitive kind appears under at least one path key (the
//! auto-synthesized ``<mod>:<line>:<col>``). Path/channel rails get
//! their own dedicated smoke tests.

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

[test]
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
