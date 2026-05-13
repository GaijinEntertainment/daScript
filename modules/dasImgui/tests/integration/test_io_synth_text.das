options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 2.2 — L1 keyboard input synthesis.
//!
//! `imgui_type_text` runs a coroutine that pushes one
//! AddInputCharactersUTF8 call per frame. The DEMO_INPUT widget
//! auto-focuses on first render; ImGui routes the chars into its buffer.
//!
//! Gates:
//!   1. DEMO_INPUT renders and starts empty.
//!   2. imgui_type_text with empty text returns Result.err.
//!   3. imgui_type_text with payload returns Result.ok.
//!   4. After the chars stream in, DEMO_INPUT.payload.value == payload.

let IO_SYNTH_TEXT_FEATURE = "modules/dasImgui/examples/features/io_synth_text.das"

[test]
def test_imgui_type_text_buffer_fills(t : T?) {
    with_imgui_app(IO_SYNTH_TEXT_FEATURE) $(d) {
        // Gate 1: DEMO_INPUT reaches the snapshot, buffer empty.
        var snap0 = wait_for_widget(d, "DEMO_INPUT", 15.0f)
        t |> success(snap0 != null, "DEMO_INPUT registers (snapshot reachable)")
        if (snap0 == null) return
        t |> equal(snap0?["globals"]?["DEMO_INPUT"]?["payload"]?["value"] ?? "?", "",
                   "DEMO_INPUT starts with empty buffer")

        // Gate 2: error path — empty text rejected.
        let empty_resp = post_command(d, "imgui_type_text",
                                      JV((target = "DEMO_INPUT", text = "")))
        t |> equal(empty_resp?["ok"] ?? true, false,
                   "empty text → response.ok = false")

        // Gate 3: payload accepted, ok-Result echoes target.
        let resp = type_text(d, "DEMO_INPUT", "hello")
        t |> success(resp != null, "imgui_type_text returned a response")
        t |> equal(resp?["ok"] ?? false, true,
                   "type_text response.ok = true")
        t |> equal(resp?["value"] ?? "", "DEMO_INPUT",
                   "type_text response.value = target ident")

        // Gate 4: poll until the buffer reflects the full payload — one char
        // per frame means 5 chars take ~5 frames; allow 5s for poll cadence.
        let done = wait_for_string_value(d, "DEMO_INPUT", "value", "hello", 300)
        t |> success(done, "DEMO_INPUT.value reaches 'hello' after L1 keyboard stream")
    }
}
