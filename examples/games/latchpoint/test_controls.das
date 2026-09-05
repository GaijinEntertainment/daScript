options gen2
require dastest/testing_boost public
require lp_controls

[test]
def test_pointer_and_retina(t : T?) {
    t |> success(camera_dragging(true, false))
    t |> success(camera_dragging(false, true))
    t |> success(!camera_dragging(false, false))
    t |> equal(ui_scissor_rect(10, 20, 100, 40, int2(1280, 800), int2(2560, 1600)), int4(20, 1480, 200, 80))
    t |> equal(ui_scissor_rect(10, 20, 100, 40, int2(1280, 800), int2(1280, 800)), int4(10, 740, 100, 40))
}

[test]
def test_navigation(suite : T?) {
    suite |> run("menu hit testing matches visible buttons") @(t : T?) {
        let screen = float2(1280.0, 800.0)
        t |> equal(menu_hit(float2(640.0, 375.0), screen), 0)
        t |> equal(menu_hit(float2(640.0, 450.0), screen), 1)
        t |> equal(menu_hit(float2(640.0, 420.0), screen), -1)
        t |> equal(menu_hit(float2(200.0, 375.0), screen), -1)
    }
    suite |> run("arrows and WASD share movement semantics") @(t : T?) {
        var state : bool[512]
        state[GLFW_KEY_UP] = true
        t |> equal(movement_axes(state), float2(0.0, 1.0))
        state[GLFW_KEY_UP] = false
        state[GLFW_KEY_W] = true
        t |> equal(movement_axes(state), float2(0.0, 1.0))
        state[GLFW_KEY_W] = false
        state[GLFW_KEY_LEFT] = true
        t |> equal(movement_axes(state), float2(-1.0, 0.0))
    }
    suite |> run("duplicate keys do not double speed and diagonals are normalized") @(t : T?) {
        var state : bool[512]
        state[GLFW_KEY_RIGHT] = true
        state[GLFW_KEY_D] = true
        t |> equal(movement_axes(state), float2(1.0, 0.0))
        state[GLFW_KEY_UP] = true
        t |> success(abs(length(movement_axes(state)) - 1.0) < 0.00001)
        state[GLFW_KEY_LEFT] = true
        t |> equal(movement_axes(state), float2(0.0, 1.0))
    }
}
