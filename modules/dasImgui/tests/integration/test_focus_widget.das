options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 2.4 — imgui_focus dispatch.
//!
//! `imgui_focus` inserts the target's path into g_pending_focus; the next
//! widget_prelude run for that path calls SetKeyboardFocusHere(0) and clears
//! the entry. Display-only widgets (text_show, plot) never register in
//! g_focusable, so focus dispatch returns Result.err for them.
//!
//! Gates:
//!   1. DEMO_INPUT renders, focus is initially false.
//!   2. focus(DEMO_INPUT) returns Result.ok with target name.
//!   3. After the focus dispatch, snapshot.globals.DEMO_INPUT.focus == true.
//!   4. focus(DEMO_TEXT) — display-only widget — returns Result.err.

let FOCUS_WIDGET_FEATURE = "modules/dasImgui/examples/features/focus_widget.das"

[test]
def test_imgui_focus_keyboard(t : T?) {
    with_imgui_app(FOCUS_WIDGET_FEATURE) $(d) {
        // Gate 1: DEMO_INPUT reaches the snapshot, no focus initially.
        var snap0 = wait_for_widget(d, "DEMO_INPUT", 15.0f)
        t |> success(snap0 != null, "DEMO_INPUT registers (snapshot reachable)")
        if (snap0 == null) return
        // @optional focus drops false values from JV; default false ensures
        // absent-key reads as not-focused.
        t |> equal(snap0?["globals"]?["DEMO_INPUT"]?["focus"] ?? false, false,
                   "DEMO_INPUT.focus initially false")

        // Gate 2: focus dispatch returns ok-Result.
        let resp = focus(d, "DEMO_INPUT")
        t |> success(resp != null, "focus returned a response")
        t |> equal(resp?["ok"] ?? false, true,
                   "focus response.ok = true on focusable target")
        t |> equal(resp?["value"] ?? "", "DEMO_INPUT",
                   "focus response.value = target")

        // Gate 3: after focus dispatch, snapshot.focus flips true within a
        // few frames (widget_prelude consumes g_pending_focus on next render).
        let focused = wait_until(d, 300) $(snap) => snap?["globals"]?["DEMO_INPUT"]?["focus"] ?? false
        t |> success(focused != null, "DEMO_INPUT.focus becomes true after imgui_focus dispatch")

        // Gate 4: display-only widget rejects focus dispatch.
        let bad_resp = focus(d, "DEMO_TEXT")
        t |> equal(bad_resp?["ok"] ?? true, false,
                   "display-only target → response.ok = false")
        t |> success(!empty(bad_resp?["error"] ?? ""),
                     "display-only target → response.error populated")
    }
}
