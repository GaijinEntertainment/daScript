options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require imgui/imgui_transport public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 2.6 — Transport lambda module.
//!
//! `live_api_transport(url)` returns a `Transport` lambda (typedef from
//! widgets/imgui_transport.das) bound to the daslang-live REST endpoint.
//! `receive_imgui_snapshot` fetches a snapshot through it; `send_imgui_command`
//! issues arbitrary commands; `await_imgui` polls quiescence with timeout.
//!
//! Gates:
//!   1. live_api_transport returns a usable Transport.
//!   2. receive_imgui_snapshot returns a non-null snapshot with globals.
//!   3. send_imgui_command can dispatch imgui_click and the response is
//!      Result-shaped.
//!   4. await_imgui returns true when nothing's pending.

let TRANSPORT_DEMO_FEATURE = "modules/dasImgui/examples/features/transport_demo.das"

[test]
def test_transport_lambda_round_trip(t : T?) {
    with_imgui_app(TRANSPORT_DEMO_FEATURE) $(d) {
        // Wait for the widget so we know the host is fully ready before
        // routing the smoke commands through the transport lambda.
        let snap0 = wait_for_widget(d, "DEMO_BTN", 15.0f)
        t |> success(snap0 != null, "DEMO_BTN reachable via imgui_playwright")
        if (snap0 == null) return

        // Gate 1+2: build transport, fetch a snapshot through the lambda.
        let transport <- live_api_transport(d.base_url)
        var snap = receive_imgui_snapshot(transport)
        t |> success(snap != null, "transport receive_imgui_snapshot non-null")
        t |> success(snap?["globals"] != null, "snapshot has globals")

        // Gate 3: send a click via the transport; verify Result wire shape.
        var click_resp = send_imgui_command(transport, "imgui_click",
                                            JV((target = "DEMO_BTN")))
        t |> success(click_resp != null, "click via transport non-null")
        t |> equal(click_resp?["ok"] ?? false, true,
                   "transport click response.ok = true")
        t |> equal(click_resp?["value"] ?? "", "DEMO_BTN",
                   "transport click response.value = DEMO_BTN")

        // Gate 4: await_imgui converges on quiescence within the default
        // 5s timeout (no L1 coroutines in flight, no active item).
        t |> success(await_imgui(transport, "quiescent", 5.0f),
                     "await_imgui returns true when idle")
    }
}
