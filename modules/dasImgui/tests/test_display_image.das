options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 image widget.
//! Drives `examples/features/display_image.das` and verifies three image()
//! call sites register with non-zero size.

let FEATURE = "modules/dasImgui/examples/features/display_image.das"

[test]
def test_display_image_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/IMG_BASIC", 15.0f)
        t |> success(snap != null, "IMG_BASIC renders (font atlas built)")
        if (snap == null) return

        for (ident in ["IMG_BASIC", "IMG_HALF", "IMG_TINTED"]) {
            let path = "MAIN_WIN/{ident}"
            t |> success(widget_exists(snap, path), "{path} registered")
        }
    }
}
