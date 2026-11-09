options gen2
require dastest/testing_boost public
require main
require lp_dialogue
require lp_controls
require lp_lighting
require lp_world
require daslib/fio

//! Player controls: pointer, keys, menu, chat, shutdown save.

def prepare_chat_input() {
    avatar = patch_position + float3(0.0, -2.0, 1.65)
    pending_job = ""
    menu_open = false
    editing = false
    input_text = ""
}

[test(description = "Either drag button drags the camera, and the UI scissor rectangle scales to the framebuffer and flips to the bottom-left origin on a retina display as on a 1:1 one.")]
def test_pointer_and_retina(t : T?) {
    t |> success(camera_dragging(true, false))
    t |> success(camera_dragging(false, true))
    t |> success(!camera_dragging(false, false))
    t |> equal(ui_scissor_rect(10, 20, 100, 40, int2(1280, 800), int2(2560, 1600)), int4(20, 1480, 200, 80))
    t |> equal(ui_scissor_rect(10, 20, 100, 40, int2(1280, 800), int2(1280, 800)), int4(10, 740, 100, 40))
}

[test(description = "Menu clicks hit only the visible buttons, arrows and WASD move the same way, and a doubled key never speeds up the normalized movement.")]
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

[test(description = "Closing the game from the untouched opening menu leaves the saved pose byte for byte, while closing during play saves the current pose.")]
def test_lifecycle_guards(t : T?) {
    let temp = create_temp_file_result("latchpoint_pose", ".json")
    t |> success(temp is value, "temporary pose file created")
    return if (!(temp is value))
    let path = temp as value
    t |> success(fwrite(path, "saved pose"))
    t |> success(!save_avatar_on_shutdown(true, path), "closing the untouched initial menu does not save")
    t |> equal(fread(path), "saved pose", "the existing Continue pose remains byte-identical")
    t |> success(save_avatar_on_shutdown(false, path), "an active game persists its current pose")
    t |> success(fread(path) != "saved pose", "active gameplay replaces the pose")
    remove(path)
}

[test(description = "Both Enter keys confirm a chat edit, a held key does not toggle a setting again, and the key that opens chat swallows its own character but not the next one typed.")]
def test_chat_activation(suite : T?) {
    suite |> run("numpad Enter confirms the same empty edit as main Enter") @(t : T?) {
        prepare_chat_input()
        editing = true
        input_text = ""
        t |> success(is_confirm_key(GLFW_KEY_KP_ENTER))
        t |> success(is_confirm_key(GLFW_KEY_ENTER))
        t |> success(!is_confirm_key(GLFW_KEY_SPACE))
    }
    suite |> run("held keys do not repeatedly toggle settings") @(t : T?) {
        prepare_chat_input()
        reduced_flashes = false
        invoke(on_key, null, GLFW_KEY_L, 0, GLFW_REPEAT, 0)
        t |> success(!reduced_flashes)
    }
    suite |> run("activation character is consumed but the next typed e is retained") @(t : T?) {
        prepare_chat_input()
        press_key(GLFW_KEY_E)
        invoke(on_char, null, uint('e'))
        t |> equal(input_text, "")
        invoke(on_key, null, GLFW_KEY_E, 0, GLFW_RELEASE, 0)
        invoke(on_char, null, uint('e'))
        t |> equal(input_text, "e")
    }
    suite |> run("a key-only activation does not swallow the first typed character") @(t : T?) {
        prepare_chat_input()
        press_key(GLFW_KEY_E)
        invoke(on_key, null, GLFW_KEY_E, 0, GLFW_RELEASE, 0)
        invoke(on_char, null, uint('h'))
        t |> equal(input_text, "h")
    }
}
