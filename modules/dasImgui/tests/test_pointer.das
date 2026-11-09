options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require imgui/imgui_touch
require math

//! Pointer input: clicks, hover targets, drag-drop, touch scroll.

let ACTIVE_WIDGET_FEATURE = "modules/dasImgui/examples/features/active_widget.das"

[test(description = "The snapshot carries an io block with a three-button mouse array and no active widget at idle, and imgui_click answers in the Result wire shape with the await timeout merged unless fire_and_forget is set.", timeout = 1200)]
def test_active_widget_foundation(t : T?) {
    with_imgui_app(ACTIVE_WIDGET_FEATURE) $(d) {
        // Gate 1: foundation regression — snapshot returns globals + io.
        var snap0 = wait_for_widget(d, "MAIN_WIN/DEMO_BTN", 15.0f)
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

        // Gate 3: with nothing active, GetActiveID() returns 0 and io_jv() omits both keys.
        // daslib/json's `?[]` returns a JV(null) placeholder for missing keys (not literal null),
        // so we use the from_JV-with-default idiom (as imgui_playwright's widget_rendered does).
        t |> equal(snap0?["io"]?["active_id"] ?? 0u, 0u,
                   "io.active_id reads 0 when nothing is active")
        t |> success(empty(snap0?["io"]?["active_widget"] ?? ""),
                     "io.active_widget reads empty when nothing is active")

        // Gate 4: imgui_click response wire format is Result<string, string>.
        // {"ok": true, "value": "MAIN_WIN/DEMO_BTN", ...} — vs Phase 1's
        // {"ok": true, "queued": "MAIN_WIN/DEMO_BTN"}.
        let click_resp = click(d, "MAIN_WIN/DEMO_BTN")
        t |> success(click_resp != null, "imgui_click returned a response")
        t |> equal(click_resp?["ok"] ?? false, true,
                   "click response.ok = true on hit")
        t |> equal(click_resp?["value"] ?? "", "MAIN_WIN/DEMO_BTN",
                   "click response.value = target ident (Result wire)")

        // Gate 5: with_await default merges timeout_sec (5.0) into the response.
        t |> equal(click_resp?["timeout_sec"] ?? 0.0f, 5.0f,
                   "with_await merges timeout_sec=5.0 by default")

        // Gate 6: fire_and_forget=true skips the await merge — bare Result.
        let ff_resp = post_command(d, "imgui_click", JV((
            target = "MAIN_WIN/DEMO_BTN",
            await = JV((fire_and_forget = true))
        )))
        t |> success(ff_resp != null, "fire_and_forget click returned a response")
        t |> equal(ff_resp?["timeout_sec"] ?? 0.0f, 0.0f,
                   "fire_and_forget suppresses timeout_sec merge (reads as default 0.0)")
        t |> equal(ff_resp?["value"] ?? "", "MAIN_WIN/DEMO_BTN",
                   "fire_and_forget still carries the Result.value")

        // Gate 7: error path — unknown target produces Result.err shape.
        let err_resp = click(d, "DOES_NOT_EXIST")
        t |> equal(err_resp?["ok"] ?? true, false,
                   "unknown target -> response.ok = false")
        t |> success(!empty(err_resp?["error"] ?? ""),
                     "unknown target -> response.error populated")
    }
}

let HEX_ID_CLICK_FEATURE = "modules/dasImgui/examples/features/hex_id_click.das"

[test(description = "imgui_click on a widget's 0x hex_id resolves to its path and fires the click, while an unknown hex_id returns an error.", timeout = 1200)]
def test_hex_id_click_dispatches_via_path(t : T?) {
    with_imgui_app(HEX_ID_CLICK_FEATURE) $(d) {
        // Gate 1: DEMO_BTN reaches the snapshot with a non-zero hex_id.
        var snap0 = wait_for_widget(d, "MAIN_WIN/DEMO_BTN", 15.0f)
        t |> success(snap0 != null, "DEMO_BTN registers (snapshot reachable)")
        if (snap0 == null) return
        let hex_id = snap0?["globals"]?["MAIN_WIN/DEMO_BTN"]?["hex_id"] ?? 0u
        t |> success(hex_id != 0u, "DEMO_BTN.hex_id is non-zero")
        let hex_str = "0x{hex_id:x}"

        // Gate 2: hex_id click returns ok-Result with the RESOLVED path.
        let resp = click(d, hex_str)
        t |> success(resp != null, "hex_id click returned a response")
        t |> equal(resp?["ok"] ?? false, true,
                   "hex_id click response.ok = true")
        t |> equal(resp?["value"] ?? "", "MAIN_WIN/DEMO_BTN",
                   "hex_id click response.value = resolved path (DEMO_BTN)")

        // Gate 3: click_count actually incremented — proves the L2
        // dispatcher fired, not just the resolution.
        let counted = wait_for_int_value(d, "MAIN_WIN/DEMO_BTN", "click_count", 1, 300)
        t |> success(counted, "DEMO_BTN.click_count reached 1 after hex_id click")

        // Gate 4: garbage hex (well-formed prefix, but no widget at that id)
        // → unresolvable, both g_id_to_path and g_id_to_bbox miss.
        let bad_resp = click(d, "0xdeadbeef")
        t |> equal(bad_resp?["ok"] ?? true, false,
                   "garbage hex_id -> response.ok = false")
        t |> success(!empty(bad_resp?["error"] ?? ""),
                     "garbage hex_id -> response.error populated")
    }
}

let SELECTABLE_HOVER_FEATURE = "modules/dasImgui/examples/features/selectable_hover.das"

[test(description = "The selectable_hover demo renders its indexed rows with the selectable_hover kind.", timeout = 1200)]
def test_selectable_hover_smoke(t : T?) {
    with_imgui_app(SELECTABLE_HOVER_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SH_WIN", 15.0f)
        t |> success(snap0 != null, "SH_WIN registers (demo renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "SH_WIN")?["kind"] ?? "", "window",
                   "SH_WIN.kind == 'window'")
        var snap_row = wait_for_widget(d, "SH_WIN/SH_ROW[0]", 15.0f)
        t |> success(snap_row != null, "SH_ROW[0] hover-row renders")
        if (snap_row != null) {
            t |> equal(find_widget(snap_row, "SH_WIN/SH_ROW[0]")?["kind"] ?? "",
                       "selectable_hover",
                       "SH_ROW[0].kind == 'selectable_hover'")
        }
    }
}

let COLOR_BUTTON_HOVER_FEATURE = "modules/dasImgui/examples/features/color_button_hover.das"

[test(description = "The color_button_hover demo renders its swatches with the color_button_hover kind and its status text.", timeout = 1200)]
def test_color_button_hover_smoke(t : T?) {
    with_imgui_app(COLOR_BUTTON_HOVER_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "CBH_WIN", 15.0f)
        t |> success(snap0 != null, "CBH_WIN registers (demo renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "CBH_WIN")?["kind"] ?? "", "window",
                   "CBH_WIN.kind == 'window'")
        var snap_red = wait_for_widget(d, "CBH_WIN/CBH_RED", 15.0f)
        t |> success(snap_red != null, "CBH_RED hover-swatch renders")
        if (snap_red != null) {
            t |> equal(find_widget(snap_red, "CBH_WIN/CBH_RED")?["kind"] ?? "",
                       "color_button_hover",
                       "CBH_RED.kind == 'color_button_hover'")
        }
        let snap_status = wait_for_widget(d, "CBH_WIN/CBH_STATUS", 15.0f)
        t |> success(snap_status != null, "CBH_STATUS narrative renders")
    }
}

let DRAG_DROP_FEATURE = "modules/dasImgui/examples/features/drag_drop.das"

[test(description = "drag_drop_source and drag_drop_target register their kinds, and a synth drag from source to target delivers one drop the target counts.", timeout = 1200)]
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

let TOUCH_SCROLL_FEATURE = "modules/dasImgui/examples/features/touch_scroll.das"
let STEP = 70.0

def private fingers(a, b : float2; ended : bool = false) : array<TouchPoint> =>
    <- [TouchPoint(id = 1, pos = a, ended = ended), TouchPoint(id = 2, pos = b, ended = ended)]

[test(description = "The touch gesture machine scrolls by the two-finger centroid delta, re-seeds when a finger lands or lifts, decays a flick to rest, and never scrolls for one finger.")]
def test_touch_scroll_machine(t : T?) {
    t |> run("the second finger releases the mouse button and scrolls nothing yet") @(t : T?) {
        var st : TouchScrollState
        let g = touch_scroll_step(st, fingers(float2(100.0, 100.0), float2(120.0, 100.0)), STEP)
        t |> success(g.release_button)
        t |> equal(g.wheel, float2(0.0, 0.0))
        t |> success(g.has_hover)
        t |> equal(g.hover, float2(110.0, 100.0))
        t |> success(st.two_down)
    }
    t |> run("two fingers moving scroll by their centroid delta in wheel units") @(t : T?) {
        var st : TouchScrollState
        touch_scroll_step(st, fingers(float2(100.0, 100.0), float2(120.0, 100.0)), STEP)
        let g = touch_scroll_step(st, fingers(float2(100.0, 65.0), float2(120.0, 65.0)), STEP)
        t |> success(!g.release_button)
        t |> success(!g.has_hover, "the hover is sent once, on the frame the second finger lands")
        t |> equal(g.wheel, float2(0.0, -35.0 / STEP))
        let h = touch_scroll_step(st, fingers(float2(80.0, 65.0), float2(100.0, 65.0)), STEP)
        t |> equal(h.wheel, float2(-20.0 / (STEP * TOUCH_WHEEL_X_PER_Y), 0.0), "a horizontal move uses ImGui's shorter horizontal step")
    }
    t |> run("a third finger landing or lifting moves the centroid but not the content") @(t : T?) {
        var st : TouchScrollState
        touch_scroll_step(st, fingers(float2(100.0, 100.0), float2(120.0, 100.0)), STEP)
        let three <- [TouchPoint(id = 1, pos = float2(100.0, 100.0)), TouchPoint(id = 2, pos = float2(120.0, 100.0)), TouchPoint(id = 3, pos = float2(110.0, 300.0))]
        let g = touch_scroll_step(st, three, STEP)
        t |> equal(g.wheel, float2(0.0, 0.0))
        t |> success(!g.release_button)
        let h = touch_scroll_step(st, fingers(float2(100.0, 90.0), float2(120.0, 90.0)), STEP)
        t |> equal(h.wheel, float2(0.0, 0.0), "the lift re-seeds too")
        let i = touch_scroll_step(st, fingers(float2(100.0, 80.0), float2(120.0, 80.0)), STEP)
        t |> equal(i.wheel, float2(0.0, -10.0 / STEP), "then the two fingers scroll again")
    }
    t |> run("lifting the fingers keeps scrolling with a decaying flick, then stops") @(t : T?) {
        var st : TouchScrollState
        touch_scroll_step(st, fingers(float2(100.0, 100.0), float2(120.0, 100.0)), STEP)
        touch_scroll_step(st, fingers(float2(100.0, 80.0), float2(120.0, 80.0)), STEP)
        let none : array<TouchPoint>
        let first = touch_scroll_step(st, none, STEP)
        t |> success(!st.two_down)
        t |> equal(first.wheel, float2(0.0, -20.0 / STEP))
        let second = touch_scroll_step(st, none, STEP)
        t |> success(abs(second.wheel.y) < abs(first.wheel.y) && second.wheel.y < 0.0)
        var frames = 0
        var last = second.wheel.y
        while (last != 0.0 && frames < 200) {
            last = touch_scroll_step(st, none, STEP).wheel.y
            frames++
        }
        t |> success(frames < 200, "the flick stops")
        t |> equal(st.velocity, float2(0.0, 0.0))
    }
    t |> run("one finger is not a scroll") @(t : T?) {
        var st : TouchScrollState
        var one <- [TouchPoint(id = 1, pos = float2(100.0, 100.0))]
        touch_scroll_step(st, one, STEP)
        one[0].pos = float2(100.0, 40.0)
        let g = touch_scroll_step(st, one, STEP)
        t |> success(!g.release_button)
        t |> success(!g.has_hover)
        t |> equal(g.wheel, float2(0.0, 0.0))
    }
    t |> run("an ended finger no longer counts") @(t : T?) {
        var st : TouchScrollState
        touch_scroll_step(st, fingers(float2(100.0, 100.0), float2(120.0, 100.0)), STEP)
        let g = touch_scroll_step(st, fingers(float2(100.0, 60.0), float2(120.0, 60.0), true), STEP)
        t |> equal(g.wheel, float2(0.0, 0.0))
        t |> success(!st.two_down)
    }
}

def private inject(t : T?; d : ImguiApp; y : float; began : bool = false; ended : bool = false) {
    let reply = post_command(d, "imgui_touch_inject", JV((touches = [
        JV((id = 1, x = 150.0, y = y, began = began, ended = ended)),
        JV((id = 2, x = 170.0, y = y, began = began, ended = ended))])))
    t |> equal(reply?["count"] ?? -1, 2, "two fingers injected: {sprint_json(reply, false)}")
}

[test(description = "Two injected fingers hover their centroid without pressing a button, scroll the window under them by their travel without moving it, and let the flick come to rest after they lift.", timeout = 1200)]
def test_touch_scroll_end_to_end(t : T?) {
    with_imgui_app(TOUCH_SCROLL_FEATURE) $(d) {
        var state : JsonValue?
        let ready = wait_cond(d, 15.0) $() : bool {
            state = post_command(d, "touch_scroll_state", null)
            return (state?["frames"] ?? 0) > 2 && (state?["scroll_max_y"] ?? 0.0) > 0.0
        }
        t |> success(ready, "the window renders with content to scroll")
        if (!ready) return
        let win_x = state?["win_x"] ?? -1.0
        let win_y = state?["win_y"] ?? -1.0
        t |> equal(state?["scroll_y"] ?? -1.0, 0.0, "starts unscrolled")

        inject(t, d, 150.0, began = true)
        let armed = wait_cond(d, 5.0) $() : bool {
            state = post_command(d, "touch_scroll_state", null)
            return abs((state?["mouse_x"] ?? 0.0) - 160.0) < 1.0 && abs((state?["mouse_y"] ?? 0.0) - 150.0) < 1.0
        }
        t |> success(armed, "the two fingers hover their centroid, so the wheel lands on the window under them: {sprint_json(state, false)}")
        t |> success(!(state?["mouse_down"] ?? true), "no button is down while two fingers scroll")
        inject(t, d, 90.0)
        let scrolled = wait_cond(d, 5.0) $() : bool {
            state = post_command(d, "touch_scroll_state", null)
            return (state?["scroll_y"] ?? 0.0) > 30.0
        }
        t |> success(scrolled, "fingers moving up scroll the content down by about their travel")
        t |> equal(state?["win_x"] ?? -1.0, win_x, "the window did not move")
        t |> equal(state?["win_y"] ?? -1.0, win_y, "the window did not move")
        inject(t, d, 90.0, ended = true)
        var last_y = -1.0
        let settled = wait_cond(d, 5.0) $() : bool {
            state = post_command(d, "touch_scroll_state", null)
            let y = state?["scroll_y"] ?? -1.0
            let still = y == last_y
            last_y = y
            return still
        }
        t |> success(settled, "the flick decays and the scroll comes to rest after the fingers lift")
    }
}
