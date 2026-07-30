options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `log_to_clipboard` / `log_text` / `log_finish` —
//! ImGui log capture rail. All three are imperatives (no snapshot kind);
//! the test value is "no panic on the call chain when the COPY_BTN fires
//! all three imperatives back-to-back, surrounding widgets still register."

let FEATURE = "modules/dasImgui/examples/features/log_to_text.das"

[test]
def test_log_to_text_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/COPY_BTN", 15.0f)
        t |> success(snap != null, "COPY_BTN registers")
        if (snap == null) return

        let btn = find_widget(snap, "MAIN_WIN/COPY_BTN")
        t |> equal(btn?["kind"] ?? "", "button",
                   "COPY_BTN.kind == 'button'")

        for (ident in ["LEAD", "SEP", "STATIC_1", "STATIC_2"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registers")
        }

        // Click COPY_BTN — fires the log_to_clipboard / 3× log_text /
        // log_finish sequence. Clipboard contents not snapshot-visible.
        // Value here = no panic on the imperative chain.
        click(d, "MAIN_WIN/COPY_BTN")
        t |> success(wait_for_int_value(d, "MAIN_WIN/COPY_BTN", "click_count", 1, 300),
                     "COPY_BTN.click_count == 1 after click (log chain non-fatal)")

        // STATIC_1/2 still register the frame after the log session closes.
        var snap_after = snapshot(d)
        t |> success(widget_exists(snap_after, "MAIN_WIN/STATIC_1"),
                     "STATIC_1 still registers after log session")
        t |> success(widget_exists(snap_after, "MAIN_WIN/STATIC_2"),
                     "STATIC_2 still registers after log session")
    }
}
