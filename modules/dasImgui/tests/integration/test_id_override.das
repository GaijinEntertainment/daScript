options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! §4.8 ID-override surface. This file currently exercises `with_id(s) { ... }`;
//! per-call `id=` / `path=` named-arg overrides land in a later step and add
//! their own `[test]` functions to this file.

let ID_OVERRIDE_FEATURE = "modules/dasImgui/examples/features/id_override.das"

[test]
def test_with_id_distinct_scopes(t : T?) {
    with_imgui_app(ID_OVERRIDE_FEATURE) $(d) {
        // Wait for the deepest leaf to land — proves the entire with_id chain
        // walked during the first frame.
        var snap0 = wait_for_widget(d, "MAIN_WIN/outer/inner/NESTED_BTN", 15.0f)
        t |> success(snap0 != null, "nested with_id leaf registered under 4-level path")
        if (snap0 == null) return

        // Each with_id scope shows up as its own path segment.
        t |> success(widget_exists(snap0, "MAIN_WIN/section_a/SAVE_A_BTN"),
                     "SAVE_A_BTN registers under section_a scope")
        t |> success(widget_exists(snap0, "MAIN_WIN/section_b/SAVE_B_BTN"),
                     "SAVE_B_BTN registers under section_b scope")

        // Same single-global widget rendered twice under distinct with_id
        // scopes — check_unique_render keys off the full path, so no panic.
        t |> success(widget_exists(snap0, "MAIN_WIN/xa/SHARED_BTN"),
                     "SHARED_BTN registers under xa scope")
        t |> success(widget_exists(snap0, "MAIN_WIN/xb/SHARED_BTN"),
                     "SHARED_BTN also registers under xb scope (same state global, distinct path)")

        // Distinct ImGui hashes — PushID(s) is part of with_id, so the two
        // SHARED_BTN render sites must have different hex_id values. hex_id
        // is a uint in WidgetEntry; coerce default to 0u accordingly.
        let hex_a = find_widget(snap0, "MAIN_WIN/xa/SHARED_BTN")?["hex_id"] ?? 0u
        let hex_b = find_widget(snap0, "MAIN_WIN/xb/SHARED_BTN")?["hex_id"] ?? 0u
        t |> success(hex_a != 0u && hex_b != 0u,
                     "both SHARED_BTN renders expose hex_id")
        t |> success(hex_a != hex_b,
                     "with_id makes ImGui hashes diverge between scopes")
    }
}

[test]
def test_with_id_nested(t : T?) {
    with_imgui_app(ID_OVERRIDE_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/outer/inner/NESTED_BTN", 15.0f)
        t |> success(snap0 != null, "nested with_id chain produces composed path")
        if (snap0 == null) return

        // The nested path leaf exists; sibling lookup at intermediate levels
        // would not (with_id doesn't register a container entry of its own,
        // it just contributes a path segment).
        t |> equal(find_widget(snap0, "MAIN_WIN/outer/inner/NESTED_BTN")?["kind"] ?? "",
                   "button", "nested leaf registers as button kind")
    }
}
