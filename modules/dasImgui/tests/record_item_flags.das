options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record item_flags.apng against examples/tutorial/item_flags.das.
//! The imgui 1.92 item-flag stack via with_item_flag, each claim driven for real and verified:
//!   CB_PLAIN  -> click, value flips (a normal checkbox).
//!   CB_MIXED  -> click, value STILL flips underneath, but MixedValue draws it as a dash regardless.
//!   SL_RW     -> drag the handle, value MOVES (editable).
//!   SL_RO     -> drag the handle, value stays put (ReadOnly|NoNav freezes it) -> record_check_unchanged.
//!   OVL_OVER  -> click the small button sitting on top of the wide one; AllowOverlap lets it win the
//!                overlap, so its click_count ticks.
//! A no-op interaction (or the read-only slider unexpectedly moving) aborts the recording at teardown.

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

[export]
def main {
    with_recording_app("examples/tutorial/item_flags.das",
                       "item_flags.apng", 55) $(app) {
        let T_PLAIN = "IF_WIN/CB_PLAIN"
        let T_MIXED = "IF_WIN/CB_MIXED"
        let T_RW    = "IF_WIN/SL_RW"
        let T_RO    = "IF_WIN/SL_RO"
        let T_OVER  = "IF_WIN/OVL_OVER"

        var snap = wait_for_render(app, T_PLAIN, 10.0f)
        if (snap == null) {
            panic("{T_PLAIN} never rendered - wrong app running?")
        }

        // ---- Stage 1: plain checkbox -- the baseline ----
        move_to(app, widget_click_point(snap, T_PLAIN), 600)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "plain checkbox", T_PLAIN,
            [voice = "Here is an ordinary checkbox. Click it and the tick appears, just as you'd expect."]),
            [T_PLAIN])

        // ---- Stage 2: MixedValue -- same checkbox, drawn as a dash ----
        // The click still flips the value underneath; MixedValue only changes the DRAWING (a dash),
        // so visually it stays a dash either way - that is the tri-state look.
        move_to(app, widget_click_point(snap, T_MIXED), 700)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "MixedValue - tri-state dash", T_MIXED,
            [voice = "This checkbox carries the MixedValue item flag. It draws as an indeterminate dash no matter what its real value is - the classic tri-state look. The click still toggles the value beneath the dash."]),
            [T_MIXED])

        // ---- Stage 3: editable slider -- drag moves the value ----
        let bbRW = widget_bbox(snap, T_RW)
        let cyRW = (bbRW.y + bbRW.w) * 0.5f
        move_to(app, (bbRW.x + 14.0f, cyRW), 700)
        hold_content(app, SETTLE_MS)
        let dRW = say_begin(app, "editable slider - drag moves it", T_RW,
            [voice = "A normal slider. Grab the handle, scrub it, and the value follows your drag."])
        drag_through_voice(app, dRW, T_RW, (bbRW.x + 0.70f * (bbRW.z - bbRW.x), cyRW))

        // ---- Stage 4: read-only slider -- drag does NOTHING ----
        // ReadOnly|NoNav: the same drag gesture must leave the value untouched. Capture it before,
        // perform a real drag, settle, then assert it did NOT move (the negative self-test).
        let bbRO = widget_bbox(snap, T_RO)
        let cyRO = (bbRO.y + bbRO.w) * 0.5f
        let before_jv = widget_payload_field(snapshot(app), T_RO, "value")
        let before = before_jv != null ? write_json(before_jv) : ""
        move_to(app, (bbRO.x + 14.0f, cyRO), 700)
        hold_content(app, SETTLE_MS)
        let dRO = say_begin(app, "ReadOnly|NoNav - drag is ignored", T_RO,
            [voice = "This slider is wrapped in ReadOnly combined with NoNav. Watch - I drag the very same way, but the value will not budge, and keyboard navigation skips right past it."])
        let anchorRO = record_frame_count(app)
        hold_content(app, 700u)
        var ev : array<JsonValue?>
        ev |> drag_along(0, (bbRO.x + 14.0f, cyRO), (bbRO.x + 0.70f * (bbRO.z - bbRO.x), cyRO), 1400, 150)
        post_command(app, "imgui_mouse_play", JV((events = ev)))
        unsafe { delete ev; }
        wait_for_mouse_idle(app)
        record_check_unchanged(app, T_RO, "value", before)
        hold_remainder_content(app, dRO, anchorRO)

        // ---- Stage 5: AllowOverlap -- the small button on top wins the click ----
        move_to(app, widget_click_point(snap, T_OVER), 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "AllowOverlap - top button wins", T_OVER,
            [voice = "A small button sits right on top of a wide one. Because the wide button allows overlap, the little button on top wins the click - watch its counter tick, not the one underneath."]),
            [T_OVER])
    }
}
