options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Per-site targeting for grouped `radio_button_int`. The three DIFFICULTY sites in
//! triggers.das share one ident `MAIN_WIN/DIFFICULTY` (shared RadioIntState), so the bare
//! path only ever resolves the LAST site. The finalize now also registers a per-site geometry
//! alias `MAIN_WIN/DIFFICULTY#<v_button>` carrying that site's bbox, so each option is
//! independently click-targetable. Gates:
//!   1. All three aliases register, kind = radio_button_int.
//!   2. The three alias bboxes are distinct and left-to-right (Easy < Normal < Hard).
//!   3. `imgui_click` on each alias selects that v_button (shared DIFFICULTY.value follows).
//!   4. `imgui_force_set` on the shared bare path still steers the selection (unchanged).

let TRIGGERS_FEATURE = "modules/dasImgui/examples/features/triggers.das"

def alias_x(var snap : JsonValue?; v_button : int) : float {
    return find_widget(snap, "MAIN_WIN/DIFFICULTY#{v_button}")?["bbox"]?["x"] ?? -1.0f
}

[test]
def test_radio_int_per_site_targeting(t : T?) {
    with_imgui_app(TRIGGERS_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/DIFFICULTY", 15.0f)
        t |> success(snap0 != null, "DIFFICULTY group registered after first frame")
        if (snap0 == null) return

        // Gate 1: per-site aliases exist, each a radio_button_int.
        for (vb in range(3)) {
            let path = "MAIN_WIN/DIFFICULTY#{vb}"
            t |> success(widget_exists(snap0, path), "{path} alias registered")
            t |> equal(find_widget(snap0, path)?["kind"] ?? "", "radio_button_int",
                       "{path}.kind == radio_button_int")
        }

        // Gate 2: the three alias bboxes are distinct and ordered left-to-right (Easy/Normal/Hard
        // laid out with same_line). The bare path bbox is the LAST site (Hard), proving the aliases
        // capture per-site geometry the shared entry can't.
        let x0 = alias_x(snap0, 0)
        let x1 = alias_x(snap0, 1)
        let x2 = alias_x(snap0, 2)
        t |> success(x0 >= 0.0f && x1 >= 0.0f && x2 >= 0.0f, "all three aliases expose a bbox")
        t |> success(x0 < x1 && x1 < x2, "alias bboxes are distinct, left-to-right ({x0} < {x1} < {x2})")

        // Gate 3: clicking each alias selects that v_button (shared value follows).
        click(d, "MAIN_WIN/DIFFICULTY#2")
        t |> success(wait_for_int_value(d, "MAIN_WIN/DIFFICULTY", "value", 2, 300),
                     "click DIFFICULTY#2 -> value 2 (Hard)")
        click(d, "MAIN_WIN/DIFFICULTY#0")
        t |> success(wait_for_int_value(d, "MAIN_WIN/DIFFICULTY", "value", 0, 300),
                     "click DIFFICULTY#0 -> value 0 (Easy)")
        click(d, "MAIN_WIN/DIFFICULTY#1")
        t |> success(wait_for_int_value(d, "MAIN_WIN/DIFFICULTY", "value", 1, 300),
                     "click DIFFICULTY#1 -> value 1 (Normal)")

        // Gate 4: the shared bare path still drives the value via imgui_force_set (unchanged).
        force_set_value(d, "MAIN_WIN/DIFFICULTY", JV(2))
        t |> success(wait_for_int_value(d, "MAIN_WIN/DIFFICULTY", "value", 2, 300),
                     "force_set on shared DIFFICULTY still steers the selection")
    }
}
