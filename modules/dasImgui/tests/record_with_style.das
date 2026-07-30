options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record with_style.apng against examples/tutorial/with_style.das. with_style brackets a
//! PushStyleColor/Var ... PopStyleColor/Var pair around a block, so the teaching is visual (scoped
//! color + metric overrides, nesting, and automatic pop on block exit). We still drive it like a
//! human: each styled button is a REAL click, so the press animation is visible and the click is
//! verified (click_count++) - proof these are live restyled buttons, not a static picture. A click
//! that misses aborts the recording at teardown.

[export]
def main {
    with_recording_app("examples/tutorial/with_style.das",
                       "with_style.apng", 40) $(app) {
        let T_STYLED   = "STYLE_WIN/STYLED_BTN"
        let T_NESTED   = "STYLE_WIN/NESTED_STYLED_BTN"
        let T_UNSTYLED = "STYLE_WIN/UNSTYLED_BTN"

        var snap = wait_for_widget(app, T_STYLED, 10.0f)
        if (snap == null) {
            panic("{T_STYLED} never registered - wrong app running?")
        }

        // ---- Stage 1: one block, color + metric override ----
        move_to(app, widget_click_point(snap, T_STYLED), 600)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "with_style - scoped color + metric", T_STYLED,
            [voice = "A with_style block changes colors and metrics just for the widgets inside it. One block makes this button red and gives it rounded corners. Click it - it is a real button, only restyled."]),
            [T_STYLED])

        // ---- Stage 2: nested blocks stack ----
        move_to(app, widget_click_point(snap, T_NESTED), 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "nested blocks stack", T_NESTED,
            [voice = "Style blocks nest. The outer block paints the text yellow, and the inner one adds a blue rounded button - so this button picks up both at once."]),
            [T_NESTED])

        // ---- Stage 3: styles pop automatically when the block ends ----
        move_to(app, widget_click_point(snap, T_UNSTYLED), 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "styles pop when the block ends", T_UNSTYLED,
            [voice = "When a block ends its styles pop automatically. This last button sits outside every block, so it is back to the plain default look - no manual pop, no leak."]),
            [T_UNSTYLED])
    }
}
