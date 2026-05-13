options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver script: re-record `visual_aids_recording.apng` against an
//! **already-running** daslang-live.
//!
//! Workflow (run both commands from the repo root, in two separate shells):
//!     # window 1 — keep this running:
//!     bin/Release/daslang-live.exe modules/dasImgui/examples/tutorial/visual_aids_tour.das
//!     # window 2 — fires the recording, exits when done:
//!     bin/Release/daslang.exe modules/dasImgui/tests/integration/record_visual_aids.das
//!
//! The driver enables mouse trail + cursor sprite, sweeps through every
//! widget's snapshot bbox with a synth click at each, then calls record_stop
//! to flush the writer cleanly. The APNG is written to daslang-live's working
//! directory (the repo root, when launched manually as above).

let BASE_URL = "http://127.0.0.1:9090"
let OUTPUT   = "visual_aids_recording.apng"

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    //! Read globals[ident].bbox (float4 serialized as {x,y,z,w}). float4 below
    //! is (x0, y0, x1, y1) — the same layout WidgetEntry.bbox uses upstream.
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

def stop_at(var events : array<JsonValue?>; t_ms : int; pos : tuple<float; float>) {
    //! Append a move + press + release at (pos) starting at `t_ms`. 600ms total
    //! (200 move-in + 100 press hold + 200 release-hold + 100 post-release hold).
    //! The post-release hold is a second move waypoint at the same position —
    //! `mouse_tick` interpolates linearly between move events, so without an
    //! anchor at the release frame the cursor would already be drifting toward
    //! the NEXT stop_at's target by then, taking the release out of the
    //! widget's bbox and the click wouldn't register.
    events |> push(JV((t_ms = t_ms,       kind = "move",   x = pos._0, y = pos._1)))
    events |> push(JV((t_ms = t_ms + 200, kind = "button", button = 0, action = "press")))
    events |> push(JV((t_ms = t_ms + 300, kind = "button", button = 0, action = "release")))
    events |> push(JV((t_ms = t_ms + 400, kind = "move",   x = pos._0, y = pos._1)))
}

def drag_along(var events : array<JsonValue?>;
               t_ms : int;
               from_pos : tuple<float; float>;
               to_pos : tuple<float; float>;
               drag_ms : int) {
    //! Append a press-and-drag sequence: park at from, press, move-while-pressed
    //! to `to_pos` over `drag_ms`, release, hold at end. mouse_tick's lerp
    //! between the press-time waypoint and the release-time waypoint produces
    //! smooth motion under the held button.
    events |> push(JV((t_ms = t_ms,                  kind = "move",   x = from_pos._0, y = from_pos._1)))
    events |> push(JV((t_ms = t_ms + 100,            kind = "button", button = 0, action = "press")))
    events |> push(JV((t_ms = t_ms + 100 + drag_ms,  kind = "move",   x = to_pos._0,   y = to_pos._1)))
    events |> push(JV((t_ms = t_ms + 200 + drag_ms,  kind = "button", button = 0, action = "release")))
    events |> push(JV((t_ms = t_ms + 300 + drag_ms,  kind = "move",   x = to_pos._0,   y = to_pos._1)))
}

[export]
def main {
    let app = ImguiApp(
        base_url = BASE_URL,
        feature_path = "",
        transport <- live_api_transport(BASE_URL)
    )
    print("[record] connecting to {BASE_URL}\n")
    if (!wait_until_ready(app, 5.0f)) {
        panic("daslang-live not responding on {BASE_URL} — is it running with visual_aids_tour.das?")
    }

    // Resolve widget centers from the running app's snapshot — tolerant to
    // layout drift. SAVE_BTN is the gate: if we can see it we're on the tour.
    var snap = wait_for_render(app, "SAVE_BTN", 10.0f)
    if (snap == null) {
        panic("SAVE_BTN never rendered — wrong app running on {BASE_URL}?")
    }
    let p_hi_status   = widget_center(snap, "BTN_HIGHLIGHT_STATUS")
    let p_hi_volume   = widget_center(snap, "BTN_HIGHLIGHT_VOLUME")
    let p_hi_save     = widget_center(snap, "BTN_HIGHLIGHT_SAVE")
    let p_narrate_vol  = widget_center(snap, "BTN_NARRATE_VOLUME")
    let p_narrate_save = widget_center(snap, "BTN_NARRATE_SAVE")
    let p_name_input   = widget_center(snap, "NAME_INPUT")
    // Slider drag: from ~20% along VOLUME's bbox to ~85%, vertically centered.
    let vol_bb = widget_bbox(snap, "VOLUME")
    let vol_y   = (vol_bb.y + vol_bb.w) * 0.5f
    let vol_x0  = vol_bb.x + (vol_bb.z - vol_bb.x) * 0.20f
    let vol_x1  = vol_bb.x + (vol_bb.z - vol_bb.x) * 0.85f

    // Visual aids on. mouse_trail draws the synth path; cursor_sprite is the
    // in-frame cursor pointer; click flash fires on every synth press (button
    // events live in glfw_live, painter in imgui_visual_aids). key_hud +
    // focus_rect are new for the keyboard half of the tour.
    post_command(app, "imgui_mouse_trail",   JV((enabled = true)))
    post_command(app, "imgui_cursor_sprite", JV((enabled = true)))
    post_command(app, "imgui_key_hud",       JV((enabled = true, show_modifiers = true)))
    post_command(app, "imgui_focus_rect",    JV((enabled = true)))
    post_command(app, "record_start", JV((file = OUTPUT, fps = 30, max_seconds = 60)))

    // --- Mouse tour ---
    //   - Click each highlight button (yellow rect appears around target).
    //   - Drag the VOLUME slider (press → move-while-pressed → release).
    //   - Click each narrate button (callout box + connector line appears).
    // Highlights last ~2s by default so a prior highlight is still visible
    // while we tour the next widget.
    var events : array<JsonValue?>
    events |> push(JV((t_ms = 0, kind = "move", x = 40.0f, y = 380.0f)))
    events |> stop_at(700,  p_hi_status)
    events |> stop_at(1500, p_hi_volume)
    events |> stop_at(2300, p_hi_save)
    events |> drag_along(3100, (vol_x0, vol_y), (vol_x1, vol_y), 1000)
    events |> stop_at(5500, p_narrate_vol)
    events |> stop_at(6400, p_narrate_save)
    events |> push(JV((t_ms = 7400, kind = "move", x = 500.0f, y = 300.0f)))
    post_command(app, "mouse_play", JV((events = events)))

    // Sleep through the mouse tour. Polling mouse_status in a tight loop
    // while the recorder is grabbing every frame thrashes the host's HTTP
    // handler — sleep through it instead. Mouse tour ends at ~7.4s + extra
    // buffer so the last narrate callout / click flash play out fully and
    // the viewer has a beat to take it in before keyboard tour starts.
    sleep(9000u)

    // --- Keyboard tour ---
    // Move cursor visually to NAME_INPUT, then activate via imgui_focus
    // (not a synth click). The cursor motion gives a visible cue; the
    // SetKeyboardFocusHere path lights up the focus rect AND keeps Ctrl
    // shortcuts working inside the InputText afterward. Synth mouse-clicks
    // into an InputText leave ImGui in an internal state where subsequent
    // Ctrl+A / Ctrl+Backspace don't reach the widget's shortcut handler
    // (the nav-route condition only auto-selects when activation goes
    // through the nav path, not the mouse-click path). Discovered while
    // re-recording the keyboard tour — Boris caught the visible regression
    // and we traced it through imgui_widgets.cpp:4282-4286.
    var move_events <- [
        JV((t_ms = 0,   kind = "move", x = p_name_input._0, y = p_name_input._1)),
        JV((t_ms = 400, kind = "move", x = p_name_input._0, y = p_name_input._1))
    ]
    post_command(app, "mouse_play", JV((events = move_events)))
    sleep(800u)
    post_command(app, "imgui_focus", JV((target = "NAME_INPUT")))
    sleep(400u)

    // Narrate the keyboard half so the viewer knows what's coming.
    post_command(app, "imgui_narrate", JV((
        text = "Watch the keycap HUD as we type.",
        target = "NAME_INPUT",
        frames = 180
    )))
    sleep(2200u)

    // Type "Hello, World!" over 4 seconds. Mixed caps + comma + space + bang
    // exercise auto-Shift around capitals and shifted punctuation, so the
    // modifier strip should flash amber on H, W, and ! while the keycaps
    // pop at bottom-center.
    post_command(app, "key_type", JV((text = "Hello, World!", total_duration_s = 4.0f)))
    sleep(4800u)

    // Demonstrate a chord: Ctrl+A. Both Ctrl pill and the "A" keycap pop.
    post_command(app, "imgui_narrate", JV((
        text = "Ctrl+A selects all.",
        target = "NAME_INPUT",
        frames = 150
    )))
    sleep(1600u)
    post_command(app, "key_chord", JV((mods = ["Ctrl"], key = "A")))
    sleep(1400u)

    // Clear the field — Backspace once (selection is active, so one press
    // empties the buffer). "Bksp" appears in the keycap row.
    post_command(app, "key_press",   JV((keycode = 259)))   // GLFW_KEY_BACKSPACE
    post_command(app, "key_release", JV((keycode = 259)))
    sleep(1800u)

    let stopped = post_command(app, "record_stop", null)
    let stop_dump = stopped != null ? write_json(stopped) : "<null>"
    print("[record] record_stop -> {stop_dump}\n")
    print("[record] APNG saved to {OUTPUT} (next to daslang-live's CWD)\n")

    // Disable overlays so a follow-up session doesn't inherit them.
    post_command(app, "imgui_key_hud",       JV((enabled = false)))
    post_command(app, "imgui_focus_rect",    JV((enabled = false)))
    post_command(app, "imgui_cursor_sprite", JV((enabled = false)))
    post_command(app, "imgui_mouse_trail",   JV((enabled = false)))
}
