options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for keyboard input synthesis.
//!
//! The playwright `type_text` helper focuses the target, then streams the text
//! as synthetic key+char events via `imgui_key_type` (the live timeline, on the
//! owned synth primitives). ImGui routes the chars into the focused buffer.
//!
//! Gates:
//!   1. DEMO_INPUT renders and starts empty.
//!   2. type_text with empty text returns ok=false.
//!   3. type_text with payload returns ok=true echoing the target.
//!   4. After the chars stream in, DEMO_INPUT.payload.value == payload.

let IO_SYNTH_TEXT_FEATURE = "modules/dasImgui/examples/features/io_synth_text.das"

[test]
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
                   "empty text → response.ok = false")

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
