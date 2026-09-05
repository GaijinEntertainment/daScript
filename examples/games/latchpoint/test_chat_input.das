options gen2
require dastest/testing_boost public
require main
require lp_dialogue
require lp_controls
require lp_lighting

def prepare_chat_input() {
    avatar = patch_position + float3(0.0, -2.0, 1.65)
    pending_job = ""
    menu_open = false
    editing = false
    input_text = ""
}

[test]
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
