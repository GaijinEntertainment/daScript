options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 0b.4 text-input widgets — buffer-as-pointer
//! `input_text` / `input_text_with_hint` / `input_text_multiline`,
//! `input_text_growable` (CallbackResize lambda thunk), and
//! `combo_getter` (ImGuiComboGetter lambda thunk). All five exercise the
//! new no-mirror C++ shim path; the growable/combo_getter pair specifically
//! covers the stack-allocated thunk + synchronous-callback contract.

let TEXT_FEATURE = "modules/dasImgui/examples/features/inputs_text.das"

[test]
def test_inputs_text_smoke(t : T?) {
    with_imgui_app(TEXT_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "NAME", 15.0f)
        t |> success(snap0 != null, "NAME registered after first frame")
        if (snap0 == null) return

        // ----- Snapshot kinds -----
        var name0 = find_widget(snap0, "NAME")
        t |> equal(name0?["kind"] ?? "", "input_text",
                   "NAME.kind == 'input_text'")
        var email0 = find_widget(snap0, "EMAIL")
        t |> equal(email0?["kind"] ?? "", "input_text_with_hint",
                   "EMAIL.kind == 'input_text_with_hint'")
        var bio0 = find_widget(snap0, "BIO")
        t |> equal(bio0?["kind"] ?? "", "input_text_multiline",
                   "BIO.kind == 'input_text_multiline'")
        var grow0 = find_widget(snap0, "GROW")
        t |> equal(grow0?["kind"] ?? "", "input_text_growable",
                   "GROW.kind == 'input_text_growable'")
        var font0 = find_widget(snap0, "FONT")
        t |> equal(font0?["kind"] ?? "", "combo_getter",
                   "FONT.kind == 'combo_getter'")

        // ----- input_text — string round-trip -----
        set_value(d, "NAME", JV("Alice"))
        t |> success(wait_for_string_value(d, "NAME", "value", "Alice", 300),
                     "imgui_set steers NAME.value to 'Alice'")
        set_value(d, "NAME", JV("Bob"))
        t |> success(wait_for_string_value(d, "NAME", "value", "Bob", 300),
                     "imgui_set re-steers NAME.value to 'Bob'")

        // ----- input_text_with_hint — same round-trip -----
        set_value(d, "EMAIL", JV("bob@example.com"))
        t |> success(wait_for_string_value(d, "EMAIL", "value", "bob@example.com", 300),
                     "imgui_set steers EMAIL.value")

        // ----- input_text_multiline — embedded newline survives -----
        set_value(d, "BIO", JV("line one\nline two"))
        t |> success(wait_for_string_value(d, "BIO", "value", "line one\nline two", 300),
                     "imgui_set steers BIO.value with embedded newline")

        // ----- input_text_growable — long string forces CallbackResize -----
        // 400 bytes overflows the default 256-byte capacity; the trampoline
        // lambda must resize state.buffer + repoint data.Buf for ImGui to
        // accept the assignment.
        var long_text = ""
        for (_ in range(400)) {
            long_text += "x"
        }
        set_value(d, "GROW", JV(long_text))
        t |> success(wait_for_string_value(d, "GROW", "value", long_text, 480),
                     "imgui_set steers GROW.value past 256B (CallbackResize fired)")
        var snap_grew = snapshot(d)
        let final_capacity = widget_payload_field(snap_grew, "GROW", "capacity") ?? 0
        t |> success(final_capacity > 256,
                     "GROW.capacity grew past 256 (got {final_capacity})")

        // ----- combo_getter — int round-trip via lambda getter -----
        set_value(d, "FONT", JV(2))
        t |> success(wait_for_int_value(d, "FONT", "value", 2, 300),
                     "imgui_set steers FONT to row 2 (Mono)")
        set_value(d, "FONT", JV(4))
        t |> success(wait_for_int_value(d, "FONT", "value", 4, 300),
                     "imgui_set re-steers FONT to row 4 (Display)")

        // ----- Final snapshot persistence -----
        var snap_final = snapshot(d)
        t |> equal(widget_payload_field(snap_final, "EMAIL", "value") ?? "",
                   "bob@example.com",
                   "EMAIL.value persists final imgui_set")
        t |> equal(widget_payload_field(snap_final, "FONT", "value") ?? -1,
                   4,
                   "FONT.value persists final imgui_set")
    }
}
