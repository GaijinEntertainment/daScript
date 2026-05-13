options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 1.3 — io mirror.
//!
//! Drives `examples/features/io_mirror.das` to verify:
//!   1. snapshot.io is a top-level object alongside globals + frame.
//!   2. io.mouse_pos is a {x, y} object (JV(float2) shape).
//!   3. io.mouse_buttons is a 3-element bool array (left, middle, right).

let IO_FEATURE = "modules/dasImgui/examples/features/io_mirror.das"

[test]
def test_io_mirror_smoke(t : T?) {
    with_imgui_app(IO_FEATURE) $(d) {
        var snap = wait_for_render(d, "IO_DEMO_WIN", 15.0f)
        t |> success(snap != null, "IO_DEMO_WIN renders")
        if (snap == null) return

        // Gate 1: io key is present.
        var io = snap?["io"]
        t |> success(io != null, "snapshot.io is present")

        // Gate 2: mouse_pos has x and y fields.
        var mouse_pos = snap?["io"]?["mouse_pos"]
        t |> success(mouse_pos != null, "io.mouse_pos present")
        var mp_x = snap?["io"]?["mouse_pos"]?["x"]
        var mp_y = snap?["io"]?["mouse_pos"]?["y"]
        t |> success(mp_x != null && mp_y != null,
                     "io.mouse_pos has x and y fields")

        // Gate 3: mouse_buttons is a 3-element array of bools (left, middle, right).
        var buttons = snap?["io"]?["mouse_buttons"]
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
