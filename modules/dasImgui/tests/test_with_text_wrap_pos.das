options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `with_text_wrap_pos` — stateless block-arg wrapper
//! around PushTextWrapPos/PopTextWrapPos. Text wrap is purely visual; this
//! test just verifies the demo runs and MAIN_WIN reaches the snapshot.

let WITH_TEXT_WRAP_POS_FEATURE = "modules/dasImgui/examples/features/with_text_wrap_pos.das"

[test]
def test_with_text_wrap_pos_smoke(t : T?) {
    with_imgui_app(WITH_TEXT_WRAP_POS_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN", 15.0f)
        t |> success(snap0 != null, "MAIN_WIN registers (demo renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_WIN")?["kind"] ?? "", "window",
                   "MAIN_WIN.kind == 'window'")
    }
}
