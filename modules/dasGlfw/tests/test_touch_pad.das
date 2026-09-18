options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require live/glfw_touch_pad
require glfw/glfw_boost

//! The on-screen pad without a window: fingers are injected in window points and the keys a
//! zone holds are read back through ``pad_key_held``.

def private finger(id : int; x, y : float; began : bool = false; ended : bool = false) : PadTouch {
    return PadTouch(id = id, pos = float2(x, y), began = began, ended = ended)
}

def private frame(touches : array<PadTouch>) {
    pad_inject(touches)
    pad_begin_frame()
}

[test]
def test_touch_pad(t : T?) {
    pad_set_enabled(true)
    pad_set_window_size(1000.0, 500.0)

    t |> run("a finger inside a button holds its keys, outside holds nothing") @(t : T?) {
        frame([finger(1, 900.0, 400.0, began = true)])
        t |> success(pad_button(0.9, 0.8, 0.1, GLFW_KEY_SPACE, GLFW_KEY_Z))
        t |> success(pad_key_held(GLFW_KEY_SPACE))
        t |> success(pad_key_held(GLFW_KEY_Z))
        pad_end_frame()
        frame([finger(1, 500.0, 100.0)])
        t |> success(!pad_button(0.9, 0.8, 0.1, GLFW_KEY_SPACE))
        t |> success(!pad_key_held(GLFW_KEY_SPACE), "released on the next frame")
        pad_end_frame()
    }

    t |> run("a finger released inside a button holds nothing on its release frame") @(t : T?) {
        frame([finger(7, 900.0, 400.0, began = true)])
        t |> success(pad_button(0.9, 0.8, 0.1, GLFW_KEY_SPACE))
        pad_end_frame()
        frame([finger(7, 900.0, 400.0, ended = true)])
        t |> success(!pad_button(0.9, 0.8, 0.1, GLFW_KEY_SPACE), "the release frame is not a press")
        t |> success(!pad_key_held(GLFW_KEY_SPACE))
        t |> equal(pad_dpad(0.9, 0.8, 0.1, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN), int2(0, 0))
        t |> success(!pad_zone(0.8, 0.6, 0.2, 0.4, GLFW_KEY_Z))
        pad_end_frame()
    }

    t |> run("a held key reads back through glfwGetKey, and the table clears") @(t : T?) {
        frame([finger(8, 900.0, 400.0, began = true)])
        t |> success(pad_button(0.9, 0.8, 0.1, GLFW_KEY_SPACE))
        t |> equal(glfwGetKey(null, GLFW_KEY_SPACE), GLFW_PRESS, "the game's polling code sees the pad's key")
        t |> success(!pad_key_held(-1), "the absent alt key holds nothing")
        pad_end_frame()
        glfw_set_virtual_key(GLFW_KEY_A, true)
        t |> equal(glfwGetKey(null, GLFW_KEY_A), GLFW_PRESS)
        glfw_clear_virtual_keys()
        frame([finger(8, 900.0, 400.0, began = true)])
        t |> success(pad_button(0.9, 0.8, 0.1, GLFW_KEY_SPACE))
        pad_end_frame()
    }

    t |> run("a zone claims its finger, so a later tap does not see it") @(t : T?) {
        frame([finger(9, 100.0, 100.0, began = true)])
        t |> success(pad_zone(0.0, 0.0, 0.5, 1.0, GLFW_KEY_Z))
        t |> success(!pad_tap(GLFW_KEY_SPACE))
        t |> success(pad_axis_x() < 0.0)
        pad_end_frame()
    }

    t |> run("a same-frame tap on a button still counts") @(t : T?) {
        frame([finger(2, 900.0, 400.0, began = true, ended = true)])
        t |> success(pad_button(0.9, 0.8, 0.1, GLFW_KEY_SPACE))
        pad_end_frame()
    }

    t |> run("zones claim fingers by half of the screen") @(t : T?) {
        frame([finger(1, 100.0, 250.0), finger(2, 800.0, 250.0)])
        t |> success(pad_zone(0.0, 0.0, 0.5, 1.0, GLFW_KEY_LEFT))
        t |> success(pad_zone(0.5, 0.0, 0.5, 1.0, GLFW_KEY_RIGHT))
        t |> success(pad_key_held(GLFW_KEY_LEFT) && pad_key_held(GLFW_KEY_RIGHT))
        pad_end_frame()
    }

    t |> run("the d-pad picks the dominant axis and ignores the dead center") @(t : T?) {
        frame([finger(1, 150.0, 400.0)])
        t |> equal(pad_dpad(0.2, 0.7, 0.2, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN), int2(-1, 0))
        t |> success(pad_key_held(GLFW_KEY_LEFT) && !pad_key_held(GLFW_KEY_UP))
        pad_end_frame()
        frame([finger(1, 205.0, 280.0)])
        t |> equal(pad_dpad(0.2, 0.7, 0.2, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN), int2(0, -1))
        pad_end_frame()
        frame([finger(1, 205.0, 355.0)])
        t |> equal(pad_dpad(0.2, 0.7, 0.2, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN), int2(0, 0), "dead center")
        t |> success(!pad_key_held(GLFW_KEY_LEFT))
        pad_end_frame()
    }

}

[test]
def test_touch_pad_gestures(t : T?) {
    pad_set_enabled(true)
    pad_set_window_size(1000.0, 500.0)

    t |> run("a diagonal d-pad holds both axes only when asked") @(t : T?) {
        frame([finger(1, 255.0, 405.0)])
        t |> equal(pad_dpad(0.2, 0.7, 0.2, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN, diagonal = true), int2(1, 1))
        t |> success(pad_key_held(GLFW_KEY_RIGHT) && pad_key_held(GLFW_KEY_DOWN))
        pad_end_frame()
        frame([finger(1, 255.0, 405.0)])
        t |> equal(pad_dpad(0.2, 0.7, 0.2, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN), int2(1, 0), "four-way keeps the dominant axis")
        pad_end_frame()
    }

    t |> run("a swipe pulses once past the threshold, then measures again from there") @(t : T?) {
        frame([finger(1, 500.0, 250.0, began = true)])
        t |> equal(pad_swipe(GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN), int2(0, 0))
        pad_end_frame()
        frame([finger(1, 510.0, 250.0)])
        t |> equal(pad_swipe(GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN), int2(0, 0), "under the threshold")
        pad_end_frame()
        frame([finger(1, 540.0, 250.0)])
        t |> equal(pad_swipe(GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN), int2(1, 0))
        t |> success(pad_key_held(GLFW_KEY_RIGHT))
        pad_end_frame()
        frame([finger(1, 545.0, 255.0)])
        t |> equal(pad_swipe(GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN), int2(0, 0), "measured from the last swipe")
        t |> success(!pad_key_held(GLFW_KEY_RIGHT))
        pad_end_frame()
        frame([finger(1, 545.0, 200.0)])
        t |> equal(pad_swipe(GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN), int2(0, -1))
        pad_end_frame()
    }

    t |> run("a tap presses for one frame and a claimed finger does not tap") @(t : T?) {
        frame([finger(1, 900.0, 400.0, began = true), finger(2, 300.0, 100.0, began = true)])
        pad_button(0.9, 0.8, 0.1, GLFW_KEY_Z)
        t |> success(pad_tap(GLFW_KEY_SPACE))
        pad_end_frame()
        frame([finger(1, 900.0, 400.0), finger(2, 300.0, 100.0)])
        pad_button(0.9, 0.8, 0.1, GLFW_KEY_Z)
        t |> success(!pad_tap(GLFW_KEY_SPACE), "held fingers do not tap again")
        t |> success(!pad_key_held(GLFW_KEY_SPACE))
        pad_end_frame()
    }

    t |> run("the drag axis is the free finger's x fraction, -1 with none") @(t : T?) {
        frame([finger(1, 900.0, 400.0), finger(2, 250.0, 100.0)])
        pad_button(0.9, 0.8, 0.1, GLFW_KEY_Z)
        t |> equal(pad_axis_x(), 0.25)
        pad_end_frame()
        var none : array<PadTouch>
        frame(none)
        t |> equal(pad_axis_x(), -1.0)
        pad_end_frame()
    }
}

[test]
def test_touch_pad_modes(t : T?) {
    pad_set_enabled(true)
    pad_set_window_size(1000.0, 500.0)

    t |> run("a landscape game in a portrait window asks to rotate and holds no keys") @(t : T?) {
        pad_prefer_landscape()
        pad_set_window_size(400.0, 800.0)
        frame([finger(1, 360.0, 700.0)])
        t |> success(pad_wants_rotate())
        t |> success(!pad_button(0.9, 0.9, 0.1, GLFW_KEY_SPACE))
        t |> success(!pad_key_held(GLFW_KEY_SPACE))
        t |> success(!pad_zone(0.0, 0.0, 1.0, 1.0, GLFW_KEY_Z))
        t |> equal(pad_dpad(0.9, 0.9, 0.1, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN), int2(0, 0))
        t |> equal(pad_swipe(GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN), int2(0, 0))
        t |> success(!pad_tap(GLFW_KEY_SPACE))
        t |> success(pad_axis_x() < 0.0)
        pad_end_frame()
        pad_set_window_size(1000.0, 500.0)
        t |> success(!pad_wants_rotate())
        pad_prefer_landscape(false)
    }

    t |> run("a disabled pad holds nothing") @(t : T?) {
        pad_set_enabled(false)
        frame([finger(1, 900.0, 400.0, began = true)])
        t |> success(!pad_button(0.9, 0.8, 0.1, GLFW_KEY_SPACE))
        t |> success(!pad_tap(GLFW_KEY_SPACE))
        pad_end_frame()
        pad_set_enabled(true)
    }
}
