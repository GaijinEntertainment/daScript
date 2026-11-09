options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! dasImgui tests with no category file of their own yet.

[test(description = "clicking a button that calls set_window_size resizes the window at runtime, and the size readout follows each resize", timeout = 1200)]
def test_set_window_size_runtime_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/set_window_size_runtime.das") $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/BTN_LARGE", 15.0f)
        t |> success(snap != null, "BTN_LARGE registers")
        if (snap == null) return

        for (ident in ["BTN_SMALL", "BTN_MEDIUM", "BTN_LARGE", "SIZE_READOUT", "LEAD"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "MAIN_WIN/{ident} registers")
        }

        // Click LARGE -> set_window_size(640, 440) -> SIZE_READOUT updates.
        click(d, "MAIN_WIN/BTN_LARGE")
        t |> success(wait_for_int_value(d, "MAIN_WIN/BTN_LARGE", "click_count", 1, 300),
                     "BTN_LARGE.click_count == 1 after click")
        t |> success(wait_for_string_value(d, "MAIN_WIN/SIZE_READOUT", "value",
                                           "current = (640, 440)", 300),
                     "SIZE_READOUT reflects 640x440 after BTN_LARGE click")

        // Click MEDIUM -> 480x320 (round-trip back from LARGE).
        click(d, "MAIN_WIN/BTN_MEDIUM")
        t |> success(wait_for_string_value(d, "MAIN_WIN/SIZE_READOUT", "value",
                                           "current = (480, 320)", 300),
                     "SIZE_READOUT reflects 480x320 after BTN_MEDIUM click")
    }
}

[test(description = "text_filter registers as its own kind and, with an empty filter, every log row and its sibling widgets render", timeout = 1200)]
def test_text_filter_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/text_filter.das") $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/FILT", 15.0f)
        t |> success(snap != null, "FILT registers")
        if (snap == null) return

        let filt = find_widget(snap, "MAIN_WIN/FILT")
        t |> equal(filt?["kind"] ?? "", "text_filter",
                   "FILT.kind == 'text_filter'")

        for (i in range(10)) {
            t |> success(widget_exists(snap, "MAIN_WIN/LOG_ROW[{i}]"),
                         "LOG_ROW[{i}] renders with empty filter")
        }

        t |> success(widget_exists(snap, "MAIN_WIN/LEAD"), "LEAD registers")
        t |> success(widget_exists(snap, "MAIN_WIN/SEP"),  "SEP registers")
        //! filter mutation is out of reach: text_filter has no
        //! imgui_force_set dispatcher
    }
}
