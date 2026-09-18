options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require imgui/imgui_touch
require daslib/json public
require daslib/json_boost public
require math

//! Two fingers scroll the hovered ImGui window; one finger never does. The gesture machine is
//! checked as a pure function, then end to end through injected fingers on the touch_scroll feature.

let FEATURE = "modules/dasImgui/examples/features/touch_scroll.das"
let STEP = 70.0

def private fingers(a, b : float2; ended : bool = false) : array<TouchPoint> {
    return <- [TouchPoint(id = 1, pos = a, ended = ended), TouchPoint(id = 2, pos = b, ended = ended)]
}

[test]
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

[test]
def test_touch_scroll_end_to_end(t : T?) {
    with_imgui_app(FEATURE) $(d) {
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
