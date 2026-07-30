options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require math public

//! Driver script: record ``drag_drop.apng`` against
//! ``examples/features/drag_drop.das``. Three voiced beats — introduce the
//! drag SOURCE, introduce the drop TARGET, then perform a REAL synth drag of
//! the source onto the target. The drop is SELF-VERIFIED: ``drag_drop_target``
//! now counts ImGui's deliveries into ``TARGET_DD.accepted``, so the beat
//! asserts ``accepted`` ticks 0 -> 1 — a missed drop aborts the recording at
//! teardown rather than recording a silent no-op.

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(
        b?["x"] ?? 0.0f,
        b?["y"] ?? 0.0f,
        b?["z"] ?? 0.0f,
        b?["w"] ?? 0.0f
    )
}

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = widget_bbox(snap, ident)
    return ((b.x + b.z) * 0.5f, (b.y + b.w) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/features/drag_drop.das",
                       "drag_drop.apng", 60) $(app) {
        let T_SOURCE    = "MAIN_WIN/SOURCE_BTN"
        let T_TARGET    = "MAIN_WIN/TARGET_BTN"
        let T_TARGET_DD = "MAIN_WIN/TARGET_DD"

        var snap = wait_for_widget(app, T_SOURCE, 10.0f)
        if (snap == null) {
            panic("{T_SOURCE} never registered — wrong app running?")
        }
        let p_source = widget_center(snap, T_SOURCE)
        let p_target = widget_center(snap, T_TARGET)

        //! Initial cursor placement (synth IO flips cursor_owned on first move).
        move_to(app, p_source, 600)
        hold_content(app, SETTLE_MS)

        // ---- Beat 1: the source ----
        say(app, "drag source - publishes a payload", T_SOURCE,
            [voice = "This button is a drag source. Pick it up, and it hands out a small payload. Here that payload is just an integer."])

        // ---- Beat 2: the target ----
        move_to(app, p_target, 800)
        hold_content(app, SETTLE_MS)
        say(app, "drop target - accepts a matching type", T_TARGET,
            [voice = "This one is a drop target. It lights up only for a payload whose type it recognizes, and quietly ignores anything else."])

        // ---- Beat 3: the real drag, self-verified ----
        let dwell = say_begin(app, "drag source onto target", [T_SOURCE, T_TARGET],
            [voice = "Now the real thing. Press on the source, drag it across, and release over the target. The value rides along with the cursor, and the moment you let go the target accepts it and the drop count ticks up."])
        let anchor = record_frame_count(app)
        let lead = clamp(dwell / 3u, 700u, 2500u)
        hold_content(app, lead)
        // Re-resolve both centers from a fresh snapshot, then scrub source -> target
        // with the button held (a real drop gesture). drag_along approaches the
        // source from the current cursor (the target), so the re-grab is visible.
        var snap_d = snapshot(app)
        let src_c = widget_center(snap_d, T_SOURCE)
        let tgt_c = widget_center(snap_d, T_TARGET)
        var ev : array<JsonValue?>
        ev |> drag_along(0, src_c, tgt_c, 1100, 500)
        post_command(app, "imgui_mouse_play", JV((events = ev)))
        unsafe { delete ev; }
        let play = uint(500 + 300 + 1100) + 400u   // approach + release-tail + drag + settle cushion
        hold_content(app, play)
        // The drop must LAND: the target's delivery counter goes 0 -> 1.
        record_check_value(app, T_TARGET_DD, "accepted", 1, 180)
        hold_remainder_content(app, dwell, anchor)
    }
}
