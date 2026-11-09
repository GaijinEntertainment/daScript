options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Scroll and keyboard focus, captured and driven from code.

let SCROLL_FEATURE = "modules/dasImgui/examples/features/scroll_state.das"

[test(description = "An overflowing window and an overflowing child each report a non-zero scroll_max.y and a float2 scroll in their snapshot payloads.", timeout = 1200)]
def test_scroll_state_smoke(t : T?) {
    with_imgui_app(SCROLL_FEATURE) $(d) {
        var snap = wait_for_render(d, "SCROLLY_WIN", 15.0f)
        t |> success(snap != null, "SCROLLY_WIN renders")
        if (snap == null) return
        // Gate 1: window's overflow content gives scroll_max.y > 0.
        // JV(float2) serializes as an object {"x":..., "y":...} (not as
        // an array), so navigate by named field.
        var win_scroll_max = widget_payload_field(snap, "SCROLLY_WIN", "scroll_max")
        t |> success(win_scroll_max != null,
                     "SCROLLY_WIN.payload.scroll_max present")
        let win_max_y = win_scroll_max?["y"] ?? 0.0f
        t |> success(win_max_y > 0.0f,
                     "SCROLLY_WIN.scroll_max.y > 0 (got {win_max_y})")

        // Gate 2: same shape for child region.
        var child_snap = wait_for_render(d, "NESTED_WIN/LOG_REGION", 5.0f)
        t |> success(child_snap != null, "NESTED_WIN/LOG_REGION renders")
        if (child_snap == null) return
        var child_scroll_max = widget_payload_field(child_snap, "NESTED_WIN/LOG_REGION", "scroll_max")
        t |> success(child_scroll_max != null,
                     "NESTED_WIN/LOG_REGION.payload.scroll_max present")
        let child_max_y = child_scroll_max?["y"] ?? 0.0f
        t |> success(child_max_y > 0.0f,
                     "NESTED_WIN/LOG_REGION.scroll_max.y > 0 (got {child_max_y})")

        // Gate 3: scroll itself is a float2 object (initial value {x:0, y:0}).
        let win_scroll = widget_payload_field(child_snap, "SCROLLY_WIN", "scroll")
        t |> success(win_scroll != null, "SCROLLY_WIN.payload.scroll present")
    }
}

let SCROLL_HERE_FEATURE = "modules/dasImgui/examples/features/scroll_here_y.das"

[test(description = "A child opens scrolled to the top, set_scroll_here_y(0.5) after row 100 scrolls it past 1000 pixels, and the jump-to-top button brings it back to 0.", timeout = 1200)]
def test_scroll_here_y_smoke(t : T?) {
    with_imgui_app(SCROLL_HERE_FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/SCROLL_AREA", 15.0f)
        t |> success(snap != null, "SCROLL_AREA child registers")
        if (snap == null) return

        for (ident in ["LEAD", "JUMP_TOP", "JUMP_MIDDLE", "JUMP_BOTTOM"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "MAIN_WIN/{ident} registers")
        }
        t |> success(widget_exists(snap, "MAIN_WIN/SCROLL_AREA/ROW[0]"),
                     "ROW[0] in initial visible chunk")

        // Initial scroll: child opens at top.
        let initial_y = find_widget(snap, "MAIN_WIN/SCROLL_AREA")?["payload"]?["scroll"]?["y"] ?? -1.0f
        t |> success(initial_y == 0.0f,
                     "initial SCROLL_AREA.scroll.y = {initial_y}")

        // Click JUMP_MIDDLE -> set_scroll_here_y(0.5) after row 100 -> scroll snaps.
        click(d, "MAIN_WIN/JUMP_MIDDLE")
        let scrolled = wait_until(d, 300) $(var s) {
            let y = s?["globals"]?["MAIN_WIN/SCROLL_AREA"]?["payload"]?["scroll"]?["y"] ?? 0.0f
            return y >= 1000.0f
        }
        t |> success(scrolled != null,
                     "SCROLL_AREA.scroll.y >= 1000 after JUMP_MIDDLE click")

        // JUMP_TOP returns to top.
        click(d, "MAIN_WIN/JUMP_TOP")
        let back_to_top = wait_until(d, 300) $(var s) {
            let y = s?["globals"]?["MAIN_WIN/SCROLL_AREA"]?["payload"]?["scroll"]?["y"] ?? 1.0f
            return y == 0.0f
        }
        t |> success(back_to_top != null,
                     "SCROLL_AREA.scroll.y == 0 after JUMP_TOP click")
    }
}

let CHILD_REENTER_FEATURE = "modules/dasImgui/examples/features/child_scroll_reenter.das"

[test(description = "SetScrollY inside child_scroll_reenter moves the original child's observed scroll, so the wrapper re-enters the same child ID, not a phantom sibling.", timeout = 1200)]
def test_child_scroll_reenter_moves_scroll(t : T?) {
    with_imgui_app(CHILD_REENTER_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/SCROLLY", 15.0f)
        t |> success(snap0 != null, "SCROLLY child renders")
        if (snap0 == null) return
        // SCROLLY.scroll.y starts at 0.0 — the child opens scrolled to top.
        let y0 : double = find_widget(snap0, "MAIN_WIN/SCROLLY")?["payload"]?["scroll"]?["y"] ?? 0.0lf
        t |> equal(y0, 0.0lf, "initial SCROLLY.scroll.y == 0")
        // One click fires child_scroll_reenter once, moving scroll by `delta = 8.0f` (per the
        // feature file). Click several times for a clearly-non-zero observation robust to
        // single-frame jitter / button-repeat timing.
        for (_i in range(5)) {
            click(d, "MAIN_WIN/BTN_DOWN")
        }
        await_quiescent(d, 5.0f)
        var snap1 = snapshot(d)
        t |> success(snap1 != null, "post-click snapshot available")
        if (snap1 == null) return
        let y1 : double = find_widget(snap1, "MAIN_WIN/SCROLLY")?["payload"]?["scroll"]?["y"] ?? 0.0lf
        t |> success(y1 > 0.0lf,
                     "SCROLLY.scroll.y moved (y={y1}); wrapper's PushID(IDENT) reentered the same window")
    }
}

let FOCUS_CONTROL_FEATURE = "modules/dasImgui/examples/features/focus_control.das"

[test(description = "Clicking a refocus button routes keyboard focus through set_keyboard_focus_here to the input after it, and the other button routes it back.", timeout = 1200)]
def test_focus_control_smoke(t : T?) {
    with_imgui_app(FOCUS_CONTROL_FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/EMAIL_INPUT", 15.0f)
        t |> success(snap != null, "EMAIL_INPUT registers")
        if (snap == null) return

        for (ident in ["LEAD", "REFOCUS_NAME", "NAME_INPUT", "REFOCUS_EMAIL",
                       "EMAIL_INPUT", "STATUS"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "MAIN_WIN/{ident} registers")
        }

        // Click REFOCUS_EMAIL -> set_keyboard_focus_here(0) before EMAIL_INPUT
        // -> EMAIL_INPUT.focus becomes true next frame.
        click(d, "MAIN_WIN/REFOCUS_EMAIL")
        let email_focused = wait_until(d, 300) $(var s) {
            return s?["globals"]?["MAIN_WIN/EMAIL_INPUT"]?["focus"] ?? false
        }
        t |> success(email_focused != null, "EMAIL_INPUT.focus == true after REFOCUS_EMAIL click")

        // Now flip back: click REFOCUS_NAME -> NAME_INPUT focus.
        click(d, "MAIN_WIN/REFOCUS_NAME")
        let name_focused = wait_until(d, 300) $(var s) {
            return s?["globals"]?["MAIN_WIN/NAME_INPUT"]?["focus"] ?? false
        }
        t |> success(name_focused != null, "NAME_INPUT.focus == true after REFOCUS_NAME click")
    }
}

let FOCUS_WIDGET_FEATURE = "modules/dasImgui/examples/features/focus_widget.das"

[test(description = "imgui_focus on an input answers ok and gives it keyboard focus on a later frame, while a display-only widget refuses focus with an error.", timeout = 1200)]
def test_imgui_focus_keyboard(t : T?) {
    with_imgui_app(FOCUS_WIDGET_FEATURE) $(d) {
        // Gate 1: DEMO_INPUT reaches the snapshot, no focus initially.
        var snap0 = wait_for_widget(d, "MAIN_WIN/DEMO_INPUT", 15.0f)
        t |> success(snap0 != null, "DEMO_INPUT registers (snapshot reachable)")
        if (snap0 == null) return
        // @optional focus drops false values from JV; default false ensures
        // absent-key reads as not-focused.
        t |> equal(snap0?["globals"]?["MAIN_WIN/DEMO_INPUT"]?["focus"] ?? false, false,
                   "DEMO_INPUT.focus initially false")

        // Gate 2: focus dispatch returns ok-Result.
        let resp = focus(d, "MAIN_WIN/DEMO_INPUT")
        t |> success(resp != null, "focus returned a response")
        t |> equal(resp?["ok"] ?? false, true,
                   "focus response.ok = true on focusable target")
        t |> equal(resp?["value"] ?? "", "MAIN_WIN/DEMO_INPUT",
                   "focus response.value = target")

        // Gate 3: after focus dispatch, snapshot.focus flips true within a
        // few frames (widget_prelude consumes g_pending_focus on next render).
        let focused = wait_until(d, 300) $(snap) => snap?["globals"]?["MAIN_WIN/DEMO_INPUT"]?["focus"] ?? false
        t |> success(focused != null, "DEMO_INPUT.focus becomes true after imgui_focus dispatch")

        // Gate 4: display-only widget rejects focus dispatch.
        let bad_resp = focus(d, "MAIN_WIN/DEMO_TEXT")
        t |> equal(bad_resp?["ok"] ?? true, false,
                   "display-only target -> response.ok = false")
        t |> success(!empty(bad_resp?["error"] ?? ""),
                     "display-only target -> response.error populated")
    }
}
