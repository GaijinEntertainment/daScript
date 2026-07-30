options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 color family on external pointers.

let FEATURE = "modules/dasImgui/examples/features/edit_external_color.das"

[test]
def test_edit_external_color_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/CE3", 15.0f)
        t |> success(snap != null, "CE3 renders")
        if (snap == null) return

        for (ident in ["CE3", "CE4", "CP3", "CP4"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registered")
        }

        // imgui_force_set on edit_color_edit4 round-trips RGBA.
        force_set_value(d, "MAIN_WIN/CE4", JV((x = 0.9f, y = 0.1f, z = 0.2f, w = 0.5f)))
        let after = wait_until(d, 300) $(var s) {
            let v = widget_payload_field(s, "MAIN_WIN/CE4", "value")
            return v != null && v?["w"] != null
        }
        t |> success(after != null, "edit_color_edit4 RGBA round-trip")
    }
}
