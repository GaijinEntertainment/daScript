options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 2.1 foundations.
//!
//! Phase 2.1 lands the load-bearing scaffolding for the rest of Phase 2:
//! GetActiveID() binding (cherry-picked from imgui_internal.h), the
//! `g_id_to_path` / `g_id_to_bbox` reverse maps, the coroutine runner
//! (`g_coroutines` + `advance_coroutines`), the Result<T,string> wire
//! format, and the per-command `with_await` modifier wrapper.
//!
//! The active_widget VALUE check (drag a slider → io.active_widget resolves
//! to its path) needs L1 input synthesis from sub-phase 2.2; this test
//! verifies the SHAPE additions that are observable without input synthesis.
//!
//! Gates:
//!   1. Foundation regression — snapshot still produces globals + io.
//!   2. io.mouse_buttons is a 3-element bool array (multi-arg JV form).
//!   3. io.active_id absent when nothing is active (idle state).
//!   4. imgui_click response uses Result wire shape: {ok, value, ...}.
//!   5. with_await default merges timeout_sec into responses.
//!   6. fire_and_forget modifier suppresses the merge.

let ACTIVE_WIDGET_FEATURE = "modules/dasImgui/examples/features/active_widget.das"

[test]
def test_active_widget_foundation(t : T?) {
    with_imgui_app(ACTIVE_WIDGET_FEATURE) $(d) {
        // Gate 1: foundation regression — snapshot returns globals + io.
        var snap0 = wait_for_widget(d, "DEMO_BTN", 15.0f)
        t |> success(snap0 != null, "DEMO_BTN registers (snapshot reachable)")
        if (snap0 == null) return
        t |> success(snap0?["io"] != null, "snapshot.io block present")

        // Gate 2: mouse_buttons is the 3-element bool array via multi-arg JV.
        let mb = snap0?["io"]?["mouse_buttons"]
        t |> success(mb != null && (mb.value is _array), "io.mouse_buttons is an array")
        if (mb != null && (mb.value is _array)) {
            t |> equal(length(mb.value as _array), 3,
                       "io.mouse_buttons has 3 entries (L/M/R)")
        }

        // Gate 3: with no input driving anything, GetActiveID() returns 0
        // and io_jv() omits both keys. daslib/json's `?[]` returns a
        // JV(null) placeholder for missing object keys (not literal null),
        // so we use the from_JV-with-default idiom that imgui_playwright's
        // widget_rendered helper uses too.
        t |> equal(snap0?["io"]?["active_id"] ?? 0u, 0u,
                   "io.active_id reads 0 when nothing is active")
        t |> success(empty(snap0?["io"]?["active_widget"] ?? ""),
                     "io.active_widget reads empty when nothing is active")

        // Gate 4: imgui_click response wire format is Result<string, string>.
        // {"ok": true, "value": "DEMO_BTN", ...} — vs Phase 1's
        // {"ok": true, "queued": "DEMO_BTN"}.
        let click_resp = click(d, "DEMO_BTN")
        t |> success(click_resp != null, "imgui_click returned a response")
        t |> equal(click_resp?["ok"] ?? false, true,
                   "click response.ok = true on hit")
        t |> equal(click_resp?["value"] ?? "", "DEMO_BTN",
                   "click response.value = target ident (Result wire)")

        // Gate 5: with_await default merges timeout_sec (5.0) into the response.
        t |> equal(click_resp?["timeout_sec"] ?? 0.0f, 5.0f,
                   "with_await merges timeout_sec=5.0 by default")

        // Gate 6: fire_and_forget=true skips the await merge — bare Result.
        let ff_resp = post_command(d, "imgui_click", JV((
            target = "DEMO_BTN",
            await = JV((fire_and_forget = true))
        )))
        t |> success(ff_resp != null, "fire_and_forget click returned a response")
        t |> equal(ff_resp?["timeout_sec"] ?? 0.0f, 0.0f,
                   "fire_and_forget suppresses timeout_sec merge (reads as default 0.0)")
        t |> equal(ff_resp?["value"] ?? "", "DEMO_BTN",
                   "fire_and_forget still carries the Result.value")

        // Gate 7: error path — unknown target produces Result.err shape.
        let err_resp = click(d, "DOES_NOT_EXIST")
        t |> equal(err_resp?["ok"] ?? true, false,
                   "unknown target → response.ok = false")
        t |> success(!empty(err_resp?["error"] ?? ""),
                     "unknown target → response.error populated")
    }
}
