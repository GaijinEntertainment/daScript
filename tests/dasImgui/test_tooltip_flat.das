options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 set_tooltip / set_item_tooltip.
//! set_item_tooltip is gated by hover so it only registers when the
//! previous widget is hovered. The flat-form set_tooltip is gated by
//! IsItemHovered in user code; both are visible to playwright as
//! NarrativeState payloads when active.

let FEATURE = "modules/dasImgui/examples/features/tooltip_flat.das"

[test]
def test_tooltip_flat_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/LEAD", 15.0f)
        t |> success(snap != null, "LEAD renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/HOVER_BTN"),
                     "hover button HOVER_BTN registered")
        t |> success(widget_exists(snap, "MAIN_WIN/ALWAYS_BTN"),
                     "always button ALWAYS_BTN registered")
        // The set_item_tooltip carries its NarrativeState payload to the snapshot even
        // unhovered (registered at module load), so it's always enumerable.
        t |> success(widget_exists(snap, "TOOLTIP_ON_BUTTON"),
                     "hover-gated tooltip registered (payload visible)")
        // ...but it reports rendered=true only on frames it actually shows: with no cursor over
        // HOVER_BTN its finalize is skipped, so the rendered-this-frame key is absent. Guards the
        // finalize-only-when-shown fix (unconditional finalize would surface it as rendered here).
        t |> success(!widget_rendered(snap, "MAIN_WIN/TOOLTIP_ON_BUTTON"),
                     "hover-gated tooltip not rendered while unhovered")
    }
}
