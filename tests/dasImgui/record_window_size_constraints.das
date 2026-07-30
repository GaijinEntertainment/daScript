options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver script: record ``window_size_constraints.apng`` against
//! ``examples/tutorial/window_size_constraints.das``. Three voiced,
//! self-verifying beats — each drags a window's resize grip (the REAL
//! "drag border" gesture the tutorial teaches, NOT a programmatic
//! resize) to an off-constraint shape and asserts the callback snapped
//! the window to the exact constrained size:
//!   1. Square      — drag to 400x180; max(w,h) forces 400x400.
//!   2. Aspect 16:9 — drag to 480x400; height tracks width/ratio -> 480x270.
//!   3. Step 50     — drag to 333x217; both dims snap to 350x200.
//! The grip is the window's bottom-right corner (pos + size); ImGui zeroes
//! a window's reported bbox, so grip/target are computed from the payload's
//! pos + size. Disabling imgui.ini persistence (live_imgui_init) keeps the
//! FirstUseEver layout deterministic across runs.

def win_pos(var snap : JsonValue?; ident : string) : float2 {
    let p = snap?["globals"]?[ident]?["payload"]?["pos"]
    return float2(p?["x"] ?? 0.0f, p?["y"] ?? 0.0f)
}

def win_size(var snap : JsonValue?; ident : string) : float2 {
    let s = snap?["globals"]?[ident]?["payload"]?["size"]
    return float2(s?["x"] ?? 0.0f, s?["y"] ?? 0.0f)
}

def win_grip(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let p = win_pos(snap, ident)
    let s = win_size(snap, ident)
    return (p.x + s.x, p.y + s.y)
}

def win_corner(var snap : JsonValue?; ident : string; req : float2) : tuple<float; float> {
    let p = win_pos(snap, ident)
    return (p.x + req.x, p.y + req.y)
}

//! Drag ``win``'s grip to ``corner`` under an already-posted voice line, then assert the
//! constraint callback snapped ``win`` to ``expected``. Caller has already move_to'd the grip
//! and posted say_begin (dwell passed here) so prepare_recording scans the say lines in place.
def drag_under_voice(app : ImguiApp; dwell : uint; win : string;
                     grip : tuple<float; float>; corner : tuple<float; float>;
                     expected : float2) {
    let anchor = record_frame_count(app)
    let lead = dwell > 1200u ? 700u : dwell / 3u
    hold_content(app, lead)
    var ev : array<JsonValue?>
    ev |> drag_along(0, grip, corner, 900, 200)
    post_command(app, "imgui_mouse_play", JV((events = ev)))
    unsafe { delete ev; }
    let play = uint(200 + 300 + 900) + 400u   // drag_along timeline + settle
    hold_content(app, play)
    record_check_value(app, win, "size", expected, 180)
    hold_remainder_content(app, dwell, anchor)
}

[export]
def main {
    with_recording_app("examples/tutorial/window_size_constraints.das",
                       "window_size_constraints.apng", 55) $(app) {
        let T_SQUARE = "SQUARE_WIN/SQUARE_TEXT"
        let T_ASPECT = "ASPECT_WIN/ASPECT_TEXT"
        let T_STEP   = "STEP_WIN/STEP_TEXT"

        var snap = wait_for_render(app, T_SQUARE, 10.0f)
        if (snap == null) {
            panic("{T_SQUARE} never rendered - wrong app running?")
        }

        // Warm up the capture pipeline before the first voice: cold-start frames render at lower
        // fps, and counting them under the first beat skews convert's frame->time map (can crowd
        // the first two captions into voice overlap). A brief settled intro absorbs them.
        move_to(app, (200.0f, 120.0f), 800)
        hold_content(app, 1800u)

        // ---- Beat 1: Square — max(w, h) wins both dims ----
        var s1 = snapshot(app)
        let g1 = win_grip(s1, "SQUARE_WIN")
        let c1 = win_corner(s1, "SQUARE_WIN", float2(400.0, 180.0))
        move_to(app, g1, 800)
        hold_content(app, SETTLE_MS)
        drag_under_voice(app, say_begin(app, "always square (max of w, h)", T_SQUARE,
            [voice = "Drag this window to a wide, short shape, and the callback snaps it straight back to a perfect square."]),
            "SQUARE_WIN", g1, c1, float2(400.0, 400.0))

        // ---- Beat 2: Aspect 16:9 — height tracks width / ratio ----
        var s2 = snapshot(app)
        let g2 = win_grip(s2, "ASPECT_WIN")
        let c2 = win_corner(s2, "ASPECT_WIN", float2(480.0, 400.0))
        move_to(app, g2, 800)
        hold_content(app, SETTLE_MS)
        drag_under_voice(app, say_begin(app, "locked to 16:9", T_ASPECT,
            [voice = "The second window locks to sixteen by nine. Drag it tall, and the height snaps back to match the width."]),
            "ASPECT_WIN", g2, c2, float2(480.0, 270.0))

        // ---- Beat 3: Step 50 — both dims snap to the nearest 50 px ----
        var s3 = snapshot(app)
        let g3 = win_grip(s3, "STEP_WIN")
        let c3 = win_corner(s3, "STEP_WIN", float2(333.0, 217.0))
        move_to(app, g3, 800)
        hold_content(app, SETTLE_MS)
        drag_under_voice(app, say_begin(app, "snaps to a 50px grid", T_STEP,
            [voice = "The third snaps to a grid. Drag it anywhere off-grid, and both sides jump to the nearest fifty pixels."]),
            "STEP_WIN", g3, c3, float2(350.0, 200.0))
    }
}
