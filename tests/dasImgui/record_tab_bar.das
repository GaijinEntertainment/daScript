options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record tab_bar.apng against examples/tutorial/tab_bar.das. Tours tab switching the human
//! way - REAL synth clicks on the tab HEADERS, never a force command - and self-verifies the lesson
//! the widget teaches: only the active tab's body runs (a switched-away child flips to rendered:false,
//! the switched-to child renders), [container] state survives a round-trip (the Wireframe box ticked
//! in General is still on after touring away and back), and a closable tab's x really closes it
//! (a real click on the header's close glyph flips DRAFT_TAB.open false). A click that fails to
//! switch / toggle / close aborts the recording at teardown.

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(
        b?["x"] ?? 0.0f,
        b?["y"] ?? 0.0f,
        b?["z"] ?? 0.0f,
        b?["w"] ?? 0.0f
    )
}

[export]
def main {
    with_recording_app("examples/tutorial/tab_bar.das",
                       "tab_bar.apng", 60) $(app) {
        let T_GENERAL = "TB_WIN/MAIN_TABS/GENERAL_TAB"
        let T_AUDIO   = "TB_WIN/MAIN_TABS/AUDIO_TAB"
        let T_INFO    = "TB_WIN/MAIN_TABS/INFO_TAB"
        let T_DRAFT   = "TB_WIN/MAIN_TABS/DRAFT_TAB"
        // Active-tab children migrate to the container-path key on first render; an inactive tab's
        // child is a bare ident with rendered:false. These paths resolve once their tab has been open.
        let T_WIRE    = "TB_WIN/MAIN_TABS/GENERAL_TAB/G_WIRE"
        let T_MUTE    = "TB_WIN/MAIN_TABS/AUDIO_TAB/A_MUTE"
        let T_INFO_LEAD = "TB_WIN/MAIN_TABS/INFO_TAB/INFO_LEAD"
        let T_DRAFT_LINE = "TB_WIN/MAIN_TABS/DRAFT_TAB/D_LINE_LEN"

        var snap = wait_for_widget(app, T_GENERAL, 10.0f)
        if (snap == null) {
            panic("{T_GENERAL} never registered - wrong app running?")
        }

        // ---- Stage 1: General active - toggle Wireframe (leaves survivable state) ----
        move_to(app, widget_click_point(snap, T_WIRE), 700)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "only the active tab's body runs", T_WIRE,
            [voice = "A tab bar shows one panel at a time. The General tab is open, so only its controls are alive. I'll switch Wireframe on here - remember it, we will come back to it."]),
            [T_WIRE])

        // ---- Stage 2: switch to Audio - General body stops, Audio body starts ----
        move_to(app, widget_click_point(snap, T_AUDIO), 800)
        hold_content(app, SETTLE_MS)
        // tab_item is not a click-verified kind, so hold_through_voice clicks without auto-checking;
        // verify the switch ourselves: General's checkbox stops rendering, Audio's starts.
        hold_through_voice(app, say_begin(app, "click a header to switch", T_AUDIO,
            [voice = "Click a different tab header and the whole panel changes. The General controls stop running completely, and the Audio panel takes over."]),
            [T_AUDIO])
        record_check_rendered(app, T_WIRE, false, 180)
        record_check_rendered(app, T_MUTE, true, 180)

        // ---- Stage 3: in Audio - toggle Mute (the switched-to panel is live) ----
        move_to(app, widget_click_point(snap, T_AUDIO), 200)   // keep cursor near the strip
        hold_content(app, SETTLE_MS)
        var snap_audio = snapshot(app)
        move_to(app, widget_click_point(snap_audio, T_MUTE), 700)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "the new panel is interactive", T_MUTE,
            [voice = "Now that Audio is open its own controls respond. I'll toggle Mute - proof the panel you switched to is fully live, not a picture."]),
            [T_MUTE])

        // ---- Stage 4: switch to Info - Audio sleeps, Info wakes ----
        move_to(app, widget_click_point(snap, T_INFO), 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "one panel runs at a time", T_INFO,
            [voice = "Info is just another tab. Switching to it puts Audio to sleep and wakes Info. Only one panel ever runs, so the inactive tabs cost you nothing."]),
            [T_INFO])
        record_check_rendered(app, T_MUTE, false, 180)
        record_check_rendered(app, T_INFO_LEAD, true, 180)

        // ---- Stage 5: back to General - the Wireframe toggle survived ----
        move_to(app, widget_click_point(snap, T_GENERAL), 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "tab state survives the switch", T_GENERAL,
            [voice = "Here is the payoff. Back in General, the Wireframe box I ticked earlier is still on. Each tab keeps its state outside the panel, so it survives every switch."]),
            [T_GENERAL])
        record_check_rendered(app, T_WIRE, true, 180)
        record_check_value(app, T_WIRE, "value", true, 180)

        // ---- Stage 6a: open Draft, then close it with a real click on its x ----
        move_to(app, widget_click_point(snap, T_DRAFT), 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "a closable tab has an x", T_DRAFT,
            [voice = "The Draft tab is closable - see the little x on its header. Let me open it first."]),
            [T_DRAFT])
        record_check_rendered(app, T_DRAFT_LINE, true, 180)

        // ---- Stage 6b: click the close glyph (right edge of the active Draft header) ----
        var snap_draft = snapshot(app)
        let db = widget_bbox(snap_draft, T_DRAFT)
        // The close glyph sits ~11px in from the header's right edge at this font scale (verified
        // live). It is not a registered widget, so click the raw point via imgui_mouse_play.
        let x_pt = (db.z - 11.0f, (db.y + db.w) * 0.5f)
        let d6 = say_begin(app, "click the x to close it", T_DRAFT,
            [voice = "Now click that x, and the tab is gone from the strip. Its open flag flips to false and the panel stops drawing entirely."])
        let anchor6 = record_frame_count(app)
        let lead = d6 > 1200u ? 600u : d6 / 3u
        hold_content(app, lead)
        var ev : array<JsonValue?>
        click_at(ev, 0, x_pt, 500)
        post_command(app, "imgui_mouse_play", JV((events = ev)))
        unsafe { delete ev; }
        hold_content(app, 1300u)                         // click_at timeline (~1000ms) + close settle
        record_check_value(app, T_DRAFT, "open", false, 180)
        hold_remainder_content(app, d6, anchor6)
    }
}
