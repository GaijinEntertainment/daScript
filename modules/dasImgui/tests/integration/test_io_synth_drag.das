options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 2.2 — L1 mouse-drag synthesis.
//!
//! Phase 2.2 wires `imgui_drag` to a coroutine that pushes one IO event per
//! frame: AddMousePosEvent(start) + AddMouseButtonEvent(down), then `steps`
//! interpolated AddMousePosEvent updates, then AddMouseButtonEvent(up).
//! End-to-end gates: target resolution + L1 reaches the widget + slider value
//! actually moves.
//!
//! Gates:
//!   1. DEMO_SLIDER renders and starts at 0.
//!   2. imgui_drag with unknown target returns Result.err.
//!   3. imgui_drag with known target returns Result.ok with the target name.
//!   4. After the drag settles, slider value moved away from 0 — end-to-end
//!      proof that L1 mouse events drove the widget's interactive logic.
//!
//! NOT verified here: snapshot.io.active_widget transient binding during the
//! drag. The drag completes in ~10 frames; daslang-live runs at ~700fps in
//! headed mode, so the active_widget window closes in ~13ms — too fast for
//! the 50ms poll cadence to reliably catch. The value-change gate already
//! proves the IO events landed; binding visibility is a 2.5 imgui_await
//! concern, not a 2.2 sanity check.

let IO_SYNTH_DRAG_FEATURE = "modules/dasImgui/examples/features/io_synth_drag.das"

[test]
def test_imgui_drag_value_changes(t : T?) {
    with_imgui_app(IO_SYNTH_DRAG_FEATURE) $(d) {
        // Gate 1: DEMO_SLIDER reaches the snapshot.
        var snap0 = wait_for_widget(d, "DEMO_SLIDER", 15.0f)
        t |> success(snap0 != null, "DEMO_SLIDER registers (snapshot reachable)")
        if (snap0 == null) return
        let v0 = snap0?["globals"]?["DEMO_SLIDER"]?["payload"]?["value"] ?? -1.0f
        t |> equal(v0, 0.0f, "DEMO_SLIDER starts at 0.0")

        // Gate 2: error path — unknown target produces Result.err.
        let err_resp = drag(d, "DOES_NOT_EXIST", 100.0f, 0.0f)
        t |> equal(err_resp?["ok"] ?? true, false,
                   "unknown drag target → response.ok = false")

        // Gate 3: imgui_drag with known target returns ok-Result. 8 steps over
        // ~120 px gives the slider plenty of pixel travel for a noticeable
        // value bump (bounds 0..1000, default slider width ~200 px).
        let drag_resp = drag(d, "DEMO_SLIDER", 120.0f, 0.0f, 8)
        t |> success(drag_resp != null, "imgui_drag returned a response")
        t |> equal(drag_resp?["ok"] ?? false, true,
                   "drag response.ok = true on hit")
        t |> equal(drag_resp?["value"] ?? "", "DEMO_SLIDER",
                   "drag response.value = target ident (Result wire)")

        // Gate 4: after the drag completes (mouse released), the slider
        // value should have moved away from 0. End-to-end proof that L1
        // mouse-down → moves → mouse-up reached the widget and drove its
        // interactive logic. Poll up to 5s for value > 0.
        let moved = wait_until(d, 300) $(snap) => (snap?["globals"]?["DEMO_SLIDER"]?["payload"]?["value"] ?? 0.0f) > 0.0f
        t |> success(moved != null, "DEMO_SLIDER value moved off 0 after L1 drag")
    }
}
