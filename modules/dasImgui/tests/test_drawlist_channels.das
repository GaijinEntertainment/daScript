options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for channels_split / channels_set_current /
//! channels_merge. The channel helpers themselves are control-flow
//! (no ``[drawlist_prim]`` rows), but the ``[drawlist_prim]`` calls
//! INSIDE the split (add_text, add_rect_filled) still register —
//! their presence in the snapshot proves the split→set_current→merge
//! sequence drained without ImGui asserting on channel-stack misuse.

let CHANNELS_FEATURE = "modules/dasImgui/examples/features/drawlist_channels.das"

def has_kind(var snap : JsonValue?; kind : string) : bool {
    var globals = snap?["globals"]
    return false if (globals == null || !(globals.value is _object))
    for (v in values(globals.value as _object)) {
        if ((v?["kind"] ?? "") == kind) return true
    }
    return false
}

[test]
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
