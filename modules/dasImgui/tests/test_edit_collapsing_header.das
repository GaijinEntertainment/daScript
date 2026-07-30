options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `edit_collapsing_header` — closable CollapsingHeader
//! bidir-bound to a caller-owned `bool? p_open`. Verifies kind, initial bool
//! value, force_set_value dispatch through the [edit_widget] scalar-pointee path
//! (sscan_json_at writes through *ptr), and recovery via the RESTORE_A button.

let FEATURE = "modules/dasImgui/examples/features/edit_collapsing_header.das"

[test]
def test_edit_collapsing_header_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/HDR_A", 15.0f)
        t |> success(snap != null, "HDR_A registers")
        if (snap == null) return

        let hdr_a = find_widget(snap, "MAIN_WIN/HDR_A")
        t |> equal(hdr_a?["kind"] ?? "", "edit_collapsing_header",
                   "HDR_A.kind == 'edit_collapsing_header'")
        t |> success(widget_exists(snap, "MAIN_WIN/HDR_B"), "HDR_B registers")

        // Initial state — both VISIBLE_A and VISIBLE_B start true; the bool
        // payload echoes *ptr on every frame.
        t |> equal(widget_payload_field(snap, "MAIN_WIN/HDR_A", "value") ?? false,
                   true,
                   "HDR_A.value == true initially (VISIBLE_A = true)")
        t |> equal(widget_payload_field(snap, "MAIN_WIN/HDR_B", "value") ?? false,
                   true,
                   "HDR_B.value == true initially (VISIBLE_B = true)")

        // Surrounding siblings + sentinel STATUS line render.
        for (ident in ["LEAD", "SEP", "RESTORE_A", "RESTORE_B", "STATUS"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registers")
        }

        // Bidir-bind A→false via imgui_force_set; scalar-pointee dispatcher writes
        // through *ptr (VISIBLE_A := false), header hides next frame.
        force_set_value(d, "MAIN_WIN/HDR_A", JV(false))
        t |> success(wait_for_bool_value(d, "MAIN_WIN/HDR_A", "value", false, 300),
                     "HDR_A.value flips to false after force_set_value")

        // RESTORE_A button writes VISIBLE_A := true; reverse-direction proof.
        click(d, "MAIN_WIN/RESTORE_A")
        t |> success(wait_for_bool_value(d, "MAIN_WIN/HDR_A", "value", true, 300),
                     "HDR_A.value back to true after RESTORE_A click")
    }
}
