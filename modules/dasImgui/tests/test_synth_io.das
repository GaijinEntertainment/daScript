options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui public
require imgui/imgui_live_core public
require imgui/imgui_playwright public
require daslib/enum_trait
require daslib/json public
require daslib/json_boost public

//! Synthetic input below the GLFW backend: the ImGui-bypass key and
//! mouse event queue, the playwright drag and type_text helpers built
//! on it, the io mirror, and chord naming.

[test(description = "imgui_key_is_nameable refuses ImGuiKey's three range markers, which ImGui's GetKeyName asserts on")]
def test_key_range_markers_are_not_nameable(t : T?) {
    // ImGuiKey carries three range markers alongside its keys, and ImGui's
    // GetKeyName asserts on all three. Iterating the enum walks straight into
    // them, so the chord lookup has to skip them by value, not by name.
    t |> success(!imgui_key_is_nameable(ImGuiKey.NamedKey_END),
                 "NamedKey_END is a range marker, not a key")
    t |> success(!imgui_key_is_nameable(ImGuiKey.NamedKey_COUNT),
                 "NamedKey_COUNT is a count, not a key")
    t |> success(!imgui_key_is_nameable(ImGuiKey.ImGuiMod_Mask_),
                 "ImGuiMod_Mask_ is a mask, not a key")
}

[test(description = "imgui_key_is_nameable admits None, real keys, the NamedKey_BEGIN alias of Tab and single modifiers")]
def test_real_keys_and_single_mods_are_nameable(t : T?) {
    t |> success(imgui_key_is_nameable(ImGuiKey.None), "None names itself")
    t |> success(imgui_key_is_nameable(ImGuiKey.Tab), "Tab is a key")
    t |> success(imgui_key_is_nameable(ImGuiKey.NamedKey_BEGIN),
                 "NamedKey_BEGIN aliases Tab and is a real key value")
    t |> success(imgui_key_is_nameable(ImGuiKey.F3), "F3 is a key")
    t |> success(imgui_key_is_nameable(ImGuiKey.Keypad4), "Keypad4 is a key")
    t |> success(imgui_key_is_nameable(ImGuiKey.ImGuiMod_Ctrl), "Ctrl is a single mod")
    t |> success(imgui_key_is_nameable(ImGuiKey.ImGuiMod_Super), "Super is a single mod")
}

[test(description = "walking the whole ImGuiKey enum the way imgui_key_chord does refuses exactly three values, all outside the named-key range")]
def test_enum_walk_admits_only_nameable_values(t : T?) {
    // The whole enum, the way imgui_key_chord walks it: exactly three members
    // must be refused, and every one refused must be outside the named range.
    var refused = 0
    for (key in type<ImGuiKey>) {
        continue if (imgui_key_is_nameable(key))
        refused++
        let value = int(key)
        t |> success(value < int(ImGuiKey.NamedKey_BEGIN)
                     || value >= int(ImGuiKey.NamedKey_END),
                     "a refused value sits outside the named-key range")
    }
    t |> equal(refused, 3, "three range markers are refused")
}

//! The feature app is glfw_synth_keys: it renders NAME_INPUT,
//! and its apply_synth_io_override() drives both the glfw_live
//! and the imgui_live tick.
[test(description = "after a synth click into an InputText the ImGui-bypass driver still routes Ctrl shortcuts: typed text lands, a held synth Ctrl shows in io, and Ctrl+A then Backspace empties the buffer", timeout = 1200)]
def test_imgui_synth_ctrl_shortcuts(t : T?) { // nolint:STYLE038
    with_imgui_app("modules/dasImgui/examples/features/glfw_synth_keys.das") $(d) {
        //! End-to-end: imgui_focus → imgui_key_type "Hi" → expect buffer "Hi".
        //! Same shape as test_key_type_lands_in_input but routed through the
        //! ImGui bypass.
        t |> run("key_type_lands_in_input") <| @(t : T?) {
            var snap0 = wait_for_render(d, "MAIN_WIN/NAME_INPUT", 15.0f)
            t |> success(snap0 != null, "NAME_INPUT renders")
            if (snap0 == null) return

            focus(d, "MAIN_WIN/NAME_INPUT")
            let focused = wait_until(d, 120) $(var snap) {
                return snap?["globals"]?["MAIN_WIN/NAME_INPUT"]?["focus"] ?? false
            }
            t |> success(focused != null, "NAME_INPUT receives keyboard focus")

            post_command(d, "imgui_key_type", JV((text = "Hi", per_char_ms = 30)))

            t |> success(wait_for_key_idle(d, 4.0f), "imgui_key_type timeline completes")

            let landed = wait_until(d, 180) $(var snap) {
                let v = snap?["globals"]?["MAIN_WIN/NAME_INPUT"]?["payload"]?["value"] ?? ""
                return v == "Hi"
            }
            t |> success(landed != null, "NAME_INPUT.value == \"Hi\" after imgui_key_type")
        }
        reset(d)

        //! Ctrl held via imgui_key_press should show up in the io modifier
        //! snapshot. Sanity check that imgui_live's bypass populates the
        //! modifier state (NOT a routing test — that's the next case).
        t |> run("key_chord_ctrl_visible") <| @(t : T?) {
            var snap0 = wait_for_render(d, "MAIN_WIN/NAME_INPUT", 15.0f)
            t |> success(snap0 != null, "NAME_INPUT renders")
            if (snap0 == null) return

            // Synth keys are logical: the synth layer pre-swaps Ctrl<->Super
            // under ConfigMacOSXBehaviors to cancel ImGui's AddKeyEvent swap,
            // so a synth LeftCtrl reads back as io.KeyCtrl on every platform.
            let ctrl_key = int(ImGuiKey.LeftCtrl)
            post_command(d, "imgui_key_press", JV((key = ctrl_key)))
            let saw_ctrl = wait_until(d, 240) $(var snap) {
                return snap?["io"]?["key_ctrl"] ?? false
            }
            t |> success(saw_ctrl != null, "synth Ctrl held shows up in the io modifier state")
            post_command(d, "imgui_key_release", JV((key = ctrl_key)))
            let cleared = wait_until(d, 240) $(var snap) {
                return !(snap?["io"]?["key_ctrl"] ?? true)
            }
            t |> success(cleared != null, "io modifier clears after release")
        }
        reset(d)

        // Canary for the click-then-Ctrl+A bug: synth-click NAME_INPUT, type text, select-all →
        // Backspace, expect an empty buffer (legacy glfw_live left Shortcut() routing broken, so
        // Ctrl+A no-opped). Chords are logical: synth Ctrl is the primary mod on every platform.
        t |> run("click_then_ctrl_a_clears_input") <| @(t : T?) {
            var snap0 = wait_for_render(d, "MAIN_WIN/NAME_INPUT", 15.0f)
            t |> success(snap0 != null, "NAME_INPUT renders")
            if (snap0 == null) return

            // Locate NAME_INPUT center for the synth click.
            let b = snap0?["globals"]?["MAIN_WIN/NAME_INPUT"]?["bbox"]
            let cx = ((b?["x"] ?? 0.0f) + (b?["z"] ?? 0.0f)) * 0.5f
            let cy = ((b?["y"] ?? 0.0f) + (b?["w"] ?? 0.0f)) * 0.5f
            var click_events <- [
                JV((t_ms = 0,   kind = "move",   x = cx, y = cy)),
                JV((t_ms = 50,  kind = "button", button = 0, action = "press")),
                JV((t_ms = 100, kind = "button", button = 0, action = "release"))
            ]
            post_command(d, "imgui_mouse_play", JV((events = click_events)))
            t |> success(wait_for_mouse_idle(d, 4.0f), "imgui_mouse_play synth click completes")

            let activated = wait_until(d, 240) $(var snap) {
                return snap?["globals"]?["MAIN_WIN/NAME_INPUT"]?["focus"] ?? false
            }
            t |> success(activated != null, "NAME_INPUT becomes focused after synth click")

            // Type "abc".
            post_command(d, "imgui_key_type", JV((text = "abc", per_char_ms = 30)))
            let typed = wait_until(d, 240) $(var snap) {
                let v = snap?["globals"]?["MAIN_WIN/NAME_INPUT"]?["payload"]?["value"] ?? ""
                return v == "abc"
            }
            t |> success(typed != null, "NAME_INPUT.value == \"abc\" before chord")

            // The value showing "abc" proves the chars landed, but the key_type
            // timeline's final per-char delay may still be ticking; let it reach
            // idle before the chord so the chord events don't interleave its tail.
            t |> success(wait_for_key_idle(d, 4.0f), "key_type timeline idle before chord")

            post_command(d, "imgui_key_chord", JV((mods = ["Ctrl"], key = "A")))
            t |> success(wait_for_key_idle(d, 4.0f), "Ctrl+A chord completes")

            // Backspace once → if selection is active, buffer empties.
            let bksp = int(ImGuiKey.Backspace)
            post_command(d, "imgui_key_press",   JV((key = bksp)))
            post_command(d, "imgui_key_release", JV((key = bksp)))

            let emptied = wait_until(d, 240) $(var snap) {
                let v = snap?["globals"]?["MAIN_WIN/NAME_INPUT"]?["payload"]?["value"] ?? "still-has-content"
                return v == ""
            }
            t |> success(emptied != null, "NAME_INPUT.value == \"\" after Ctrl+A then Backspace")
        }
    }
}

[test(description = "imgui_focus plus imgui_key_type, bypassing GLFW, type Hello into an initially empty focused input", timeout = 1200)]
def test_imgui_synth_keys_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/imgui_synth_keys.das") $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/NAME_INPUT", 15.0f)
        t |> success(snap != null, "NAME_INPUT registers")
        if (snap == null) return

        let input = find_widget(snap, "MAIN_WIN/NAME_INPUT")
        t |> equal(input?["kind"] ?? "", "input_text",
                   "NAME_INPUT.kind == 'input_text'")
        t |> equal(widget_payload_field(snap, "MAIN_WIN/NAME_INPUT", "value") ?? "?",
                   "",
                   "NAME_INPUT.value initially empty")

        // Focus then type via the ImGui-bypass path. Linux + macOS dispatch focus a frame later
        // than Windows, so settle on the payload's `focus` field before firing chars — otherwise
        // they arrive ahead of activation.
        focus(d, "MAIN_WIN/NAME_INPUT")
        let focused = wait_until(d, 120) $(var s) {
            return s?["globals"]?["MAIN_WIN/NAME_INPUT"]?["focus"] ?? false
        }
        t |> success(focused != null, "NAME_INPUT receives keyboard focus")
        post_command(d, "imgui_key_type", JV((text = "Hello", per_char_ms = 30)))
        t |> success(wait_for_key_idle(d, 4.0f), "key timeline drains")
        t |> success(wait_for_string_value(d, "MAIN_WIN/NAME_INPUT", "value", "Hello", 300),
                     "NAME_INPUT.value == 'Hello' after imgui_key_type")
    }
}

[test(description = "raw imgui_mouse_pos and imgui_mouse_click events at a button's bbox center raise its click_count from 0 to 1", timeout = 1200)]
def test_imgui_synth_mouse_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/imgui_synth_mouse.das") $(d) {
        var snap = wait_for_widget(d, "SYNTH_WIN/SYNTH_BTN", 15.0f)
        t |> success(snap != null, "SYNTH_BTN registers")
        if (snap == null) return

        let btn = find_widget(snap, "SYNTH_WIN/SYNTH_BTN")
        t |> equal(btn?["kind"] ?? "", "button",
                   "SYNTH_BTN.kind == 'button'")
        t |> equal(widget_payload_field(snap, "SYNTH_WIN/SYNTH_BTN", "click_count") ?? -1,
                   0,
                   "SYNTH_BTN.click_count == 0 initially")

        // Read bbox to compute mouse target — robust against geometry drift.
        let bbox = btn?["bbox"]
        let bx = ((bbox?["x"] ?? 0.0f) + (bbox?["z"] ?? 0.0f)) * 0.5f
        let by = ((bbox?["y"] ?? 0.0f) + (bbox?["w"] ?? 0.0f)) * 0.5f

        // Bypass mouse to button centre then press+release LMB.
        post_command(d, "imgui_mouse_pos", JV((x = bx, y = by)))
        post_command(d, "imgui_mouse_click", JV((button = 0, action = "press")))
        post_command(d, "imgui_mouse_click", JV((button = 0, action = "release")))
        t |> success(wait_for_mouse_idle(d, 4.0f), "mouse timeline drains")
        t |> success(wait_for_int_value(d, "SYNTH_WIN/SYNTH_BTN", "click_count", 1, 300),
                     "SYNTH_BTN.click_count == 1 after bypass click")
    }
}

[test(description = "the snapshot carries a top-level io object whose mouse_pos has x and y and whose three mouse_buttons read false while idle", timeout = 1200)]
def test_io_mirror_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/io_mirror.das") $(d) {
        var snap = wait_for_render(d, "IO_DEMO_WIN", 15.0f)
        t |> success(snap != null, "IO_DEMO_WIN renders")
        if (snap == null) return

        // Gate 1: io key is present.
        let io = snap?["io"]
        t |> success(io != null, "snapshot.io is present")

        // Gate 2: mouse_pos has x and y fields.
        let mouse_pos = snap?["io"]?["mouse_pos"]
        t |> success(mouse_pos != null, "io.mouse_pos present")
        let mp_x = snap?["io"]?["mouse_pos"]?["x"]
        let mp_y = snap?["io"]?["mouse_pos"]?["y"]
        t |> success(mp_x != null && mp_y != null,
                     "io.mouse_pos has x and y fields")

        // Gate 3: mouse_buttons is a 3-element array of bools (left, middle, right).
        let buttons = snap?["io"]?["mouse_buttons"]
        t |> success(buttons != null, "io.mouse_buttons present")
        // No widget interaction yet, so all three should be false.
        let b_left = snap?["io"]?["mouse_buttons"]?[0] ?? true
        let b_mid = snap?["io"]?["mouse_buttons"]?[1] ?? true
        let b_right = snap?["io"]?["mouse_buttons"]?[2] ?? true
        t |> equal(b_left, false, "mouse_buttons[0] (left) idle is false")
        t |> equal(b_mid, false, "mouse_buttons[1] (middle) idle is false")
        t |> equal(b_right, false, "mouse_buttons[2] (right) idle is false")
    }
}

//! No gate on io.active_widget mid-drag: it binds for ~10
//! frames, faster than the poll cadence; the value gate proves
//! the events landed.
[test(description = "drag rejects an unknown target, echoes a known one, and its synth press-move-release timeline moves the slider off 0", timeout = 1200)]
def test_imgui_drag_value_changes(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/io_synth_drag.das") $(d) {
        // Gate 1: DEMO_SLIDER reaches the snapshot.
        var snap0 = wait_for_widget(d, "MAIN_WIN/DEMO_SLIDER", 15.0f)
        t |> success(snap0 != null, "DEMO_SLIDER registers (snapshot reachable)")
        if (snap0 == null) return
        let v0 = snap0?["globals"]?["MAIN_WIN/DEMO_SLIDER"]?["payload"]?["value"] ?? -1.0f
        t |> equal(v0, 0.0f, "DEMO_SLIDER starts at 0.0")

        // Gate 2: error path — unknown target produces Result.err.
        let err_resp = drag(d, "DOES_NOT_EXIST", 100.0f, 0.0f)
        t |> equal(err_resp?["ok"] ?? true, false,
                   "unknown drag target -> response.ok = false")

        // Gate 3: drag with known target returns ok-Result. 8 steps over
        // ~120 px gives the slider plenty of pixel travel for a noticeable
        // value bump (bounds 0..1000, default slider width ~200 px).
        let drag_resp = drag(d, "MAIN_WIN/DEMO_SLIDER", 120.0f, 0.0f, 8)
        t |> success(drag_resp != null, "drag returned a response")
        t |> equal(drag_resp?["ok"] ?? false, true,
                   "drag response.ok = true on hit")
        t |> equal(drag_resp?["value"] ?? "", "MAIN_WIN/DEMO_SLIDER",
                   "drag response.value = target ident (Result wire)")

        // Gate the play to completion before polling value, instead of leaning
        // on the 5s poll window to absorb the drag timeline.
        t |> success(wait_for_mouse_idle(d, 10.0f), "drag timeline drains before value gate")

        // Gate 4: after the drag completes (mouse released), the slider value must have moved off
        // 0 — end-to-end proof that L1 mouse-down → moves → mouse-up reached the widget and drove
        // its interactive logic. Poll for value > 0.
        let moved = wait_until(d, 300) $(snap) => (snap?["globals"]?["MAIN_WIN/DEMO_SLIDER"]?["payload"]?["value"] ?? 0.0f) > 0.0f
        t |> success(moved != null, "DEMO_SLIDER value moved off 0 after L1 drag")
    }
}

let IO_SYNTH_TEXT_FEATURE = "modules/dasImgui/examples/features/io_synth_text.das"

[test(description = "type_text rejects empty text, echoes its target, fills the input with hello, and a newline in the stream inserts exactly one newline in a multiline buffer", timeout = 1200)]
def test_imgui_type_text_buffer_fills(t : T?) {
    with_imgui_app(IO_SYNTH_TEXT_FEATURE) $(d) {
        // Gate 1: DEMO_INPUT reaches the snapshot, buffer empty.
        var snap0 = wait_for_widget(d, "MAIN_WIN/DEMO_INPUT", 15.0f)
        t |> success(snap0 != null, "DEMO_INPUT registers (snapshot reachable)")
        if (snap0 == null) return
        t |> equal(snap0?["globals"]?["MAIN_WIN/DEMO_INPUT"]?["payload"]?["value"] ?? "?", "",
                   "DEMO_INPUT starts with empty buffer")

        // Gate 2: error path — empty text rejected client-side by type_text.
        let empty_resp = type_text(d, "MAIN_WIN/DEMO_INPUT", "")
        t |> equal(empty_resp?["ok"] ?? true, false,
                   "empty text -> response.ok = false")

        // Gate 3: payload accepted, ok-Result echoes target.
        let resp = type_text(d, "MAIN_WIN/DEMO_INPUT", "hello")
        t |> success(resp != null, "type_text returned a response")
        t |> equal(resp?["ok"] ?? false, true,
                   "type_text response.ok = true")
        t |> equal(resp?["value"] ?? "", "MAIN_WIN/DEMO_INPUT",
                   "type_text response.value = target ident")

        // Gate 4: poll until the buffer reflects the full payload — one char
        // per frame means 5 chars take ~5 frames; allow 5s for poll cadence.
        let done = wait_for_string_value(d, "MAIN_WIN/DEMO_INPUT", "value", "hello", 300)
        t |> success(done, "DEMO_INPUT.value reaches 'hello' after L1 keyboard stream")

        // Gate 5: a '\n' in the stream inserts exactly ONE newline in a multiline
        // buffer. imgui_key_type sends '\n' as the Enter key alone (no 0x0A char),
        // matching a real backend; emitting the char too would double-insert.
        let multi = type_text(d, "MAIN_WIN/DEMO_MULTI", "a\nb")
        t |> equal(multi?["ok"] ?? false, true, "type_text into multiline returned ok")
        let multi_done = wait_for_string_value(d, "MAIN_WIN/DEMO_MULTI", "value", "a\nb", 300)
        t |> success(multi_done, "DEMO_MULTI.value == 'a\\nb' (single newline, not doubled)")
    }
}

[test(description = "Shift+Enter and Shift+KeypadEnter each insert one newline in an active multiline editor", timeout = 1200)]
def test_multiline_shift_enter_inserts_newline(t : T?) {
    with_imgui_app(IO_SYNTH_TEXT_FEATURE) $(d) {
        let ready = wait_for_widget(d, "MAIN_WIN/DEMO_MULTI", 15.0f)
        t |> success(ready != null)
        if (ready == null) return
        let typed = type_text(d, "MAIN_WIN/DEMO_MULTI", "before")
        t |> success(typed?["ok"] ?? false)
        t |> success(wait_for_string_value(d, "MAIN_WIN/DEMO_MULTI", "value", "before", 300))
        key_chord_tap(d, ["Shift"], int(ImGuiKey.Enter))
        t |> success(wait_for_string_value(d, "MAIN_WIN/DEMO_MULTI", "value", "before\n", 120),
                     "Shift+Enter inserts a newline in the active multiline editor")
        key_chord_tap(d, ["Shift"], int(ImGuiKey.KeypadEnter))
        t |> success(wait_for_string_value(d, "MAIN_WIN/DEMO_MULTI", "value", "before\n\n", 120),
                     "Shift+KeypadEnter follows the same multiline policy")
    }
}
