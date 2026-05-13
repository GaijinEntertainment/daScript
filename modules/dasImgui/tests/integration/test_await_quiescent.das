options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 2.5 — imgui_await quiescence probe.
//!
//! `imgui_await` is a single-shot probe: server reports
//! `Result<AwaitResult, string>`. The Result JV nests payload under
//! `response.value`, so all reads go through `?["value"]?["..."]`.
//!
//! Server-side blocking is incompatible with the GLFW-main-thread
//! invariant — clients (Phase 2.6 transport) drive the wait loop.
//!
//! Gates:
//!   1. With nothing happening, imgui_await reports `quiescent: true` and
//!      `pending_coroutines: 0`.
//!   2. After spawning a long drag coroutine via imgui_drag, imgui_await
//!      reports `quiescent: false` AND `pending_coroutines >= 1`.
//!   3. After polling until quiescent, the value re-flips to `true` and
//!      pending_coroutines drops to 0.
//!   4. Per-command await modifier — issuing a command with `await: {
//!      await_frames: N }` surfaces `await_frames: N` in the response wrap
//!      (the modifier-merge path of with_await).

let AWAIT_QUIESCENT_FEATURE = "modules/dasImgui/examples/features/await_quiescent.das"

[test]
def test_imgui_await(t : T?) {
    with_imgui_app(AWAIT_QUIESCENT_FEATURE) $(d) {
        // Gate 1: idle quiescence.
        var snap0 = wait_for_widget(d, "DEMO_SLIDER", 15.0f)
        t |> success(snap0 != null, "DEMO_SLIDER reachable")
        if (snap0 == null) return
        let idle = await_probe(d)
        t |> equal(idle?["ok"] ?? false, true,
                   "await response.ok = true")
        t |> equal(idle?["value"]?["quiescent"] ?? false, true,
                   "idle: quiescent = true")
        t |> equal(idle?["value"]?["pending_coroutines"] ?? -1, 0,
                   "idle: pending_coroutines = 0")

        // Gate 2: long drag → coroutine in flight; await says not quiescent.
        // 30 steps × 1 frame each = ~30 frames in-flight at 60fps. At 700fps
        // headed mode the drag is ~40ms. Probe immediately after dispatch.
        let dr = drag(d, "DEMO_SLIDER", 100.0f, 0.0f, 30)
        t |> equal(dr?["ok"] ?? false, true, "drag dispatched")
        let busy = await_probe(d)
        t |> success((busy?["value"]?["pending_coroutines"] ?? 0) >= 1 ||
                     !(busy?["value"]?["quiescent"] ?? true),
                     "during drag: not quiescent OR pending_coroutines>=1")

        // Gate 3: poll until quiescent.
        let settled = wait_until(d, 300) $(snap) {
            let probe = await_probe(d)
            return probe?["value"]?["quiescent"] ?? false
        }
        t |> success(settled != null, "post-drag: quiescent recovers")

        // Gate 4: per-command await modifier surfaces in response wrap.
        let resp = post_command(d, "imgui_drag", JV((
            target = "DEMO_SLIDER",
            dx = 10.0f,
            dy = 0.0f,
            steps = 2,
            await = JV((await_frames = 7))
        )))
        t |> equal(resp?["await_frames"] ?? -1, 7,
                   "with_await merges await_frames=7 from per-command modifier")
    }
}
