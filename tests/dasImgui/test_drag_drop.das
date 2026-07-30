options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `drag_drop_source` + `drag_drop_target` containers.
//! Both register in the snapshot (stateless_finalize runs every frame). A real
//! synth drag (drag_to) is driven SOURCE -> TARGET and the drop is asserted via
//! the `accepted` telemetry rail — `drag_drop_target` counts the drops ImGui
//! actually delivers (GetDragDropPayload().Delivery), so the test proves the
//! drop semantics end-to-end, not just that the gesture was dispatched.

let DRAG_DROP_FEATURE = "modules/dasImgui/examples/features/drag_drop.das"

[test]
def test_drag_drop_smoke(t : T?) {
    with_imgui_app(DRAG_DROP_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/SOURCE_BTN", 15.0f)
        t |> success(snap0 != null, "SOURCE_BTN registers")
        if (snap0 == null) return

        let target_entry = find_widget(snap0, "MAIN_WIN/TARGET_BTN")
        t |> equal(target_entry?["kind"] ?? "", "button",
                   "TARGET_BTN.kind == 'button'")
        let src_dd = find_widget(snap0, "MAIN_WIN/SOURCE_DD")
        t |> equal(src_dd?["kind"] ?? "", "drag_drop_source",
                   "SOURCE_DD.kind == 'drag_drop_source'")
        let tgt_dd = find_widget(snap0, "MAIN_WIN/TARGET_DD")
        t |> equal(tgt_dd?["kind"] ?? "", "drag_drop_target",
                   "TARGET_DD.kind == 'drag_drop_target'")

        // Baseline: nothing delivered yet.
        t |> equal(find_widget(snap0, "MAIN_WIN/TARGET_DD")?["payload"]?["accepted"] ?? 0,
                   0, "TARGET_DD.accepted baseline == 0")

        // Drive an L1 drag from SOURCE_BTN's bbox center to TARGET_BTN's
        // bbox center. Server replies ok with the source target name.
        let resp = drag_to(d, "MAIN_WIN/SOURCE_BTN", "MAIN_WIN/TARGET_BTN", 8, 0)
        t |> success(resp != null, "drag_to returned a response")
        t |> equal(resp?["ok"] ?? false, true,
                   "drag_to response.ok == true")

        // Gate on the gesture fully landing (mouse released, drag coroutine
        // drained) before the value poll — don't lean on the poll window to
        // absorb the play on a starved render thread.
        t |> success(wait_for_mouse_idle(d, 10.0f), "drag gesture drains before accept gate")

        // The drop must actually LAND: drag_drop_target.accepted ticks 0 -> 1.
        // This is the real semantic the visual demo used to be the only proof of.
        let landed = wait_for_payload_value(d, "MAIN_WIN/TARGET_DD", "accepted", 1, 300)
        t |> success(landed, "drop delivered: TARGET_DD.accepted -> 1 after real synth drag")
    }
}
