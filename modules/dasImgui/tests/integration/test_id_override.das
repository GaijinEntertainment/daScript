options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! §4.8 ID-override surface. Exercises `with_id(s) { ... }` plus per-call
//! `id=` / `path=` named-arg overrides on widget call macros.

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

[test]
def test_per_call_id_override(t : T?) {
    with_imgui_app(ID_OVERRIDE_FEATURE) $(d) {
        // id="hashed_v1" — telemetry path is the BARE identifier (no path
        // override), but the button's hex_id is mangled with "hashed_v1".
        var snap0 = wait_for_widget(d, "MAIN_WIN/HASHED_BTN", 15.0f)
        t |> success(snap0 != null,
                     "id= override: telemetry path stays the bare identifier")
        if (snap0 == null) return

        // Baseline hex_id: any widget rendered without id= override at the
        // same depth. SAVE_A_BTN sits inside with_id("section_a") so its
        // hex_id is also mangled — not a baseline. Use NESTED_BTN (deepest
        // with_id chain) just to confirm hex_ids are present and uint-valued.
        let hashed = find_widget(snap0, "MAIN_WIN/HASHED_BTN")?["hex_id"] ?? 0u
        t |> success(hashed != 0u,
                     "id= override: hex_id exposed and non-zero")
    }
}

[test]
def test_per_call_path_override(t : T?) {
    with_imgui_app(ID_OVERRIDE_FEATURE) $(d) {
        // path="rps_v1" — telemetry path adopts the override; bare identifier
        // RPS_PATH is NOT in the registry.
        var snap0 = wait_for_widget(d, "MAIN_WIN/rps_v1", 15.0f)
        t |> success(snap0 != null,
                     "path= override: telemetry path becomes the override literal")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_WIN/rps_v1")?["kind"] ?? "",
                   "slider_float", "path= override widget registers as slider_float kind")
        t |> success(!widget_exists(snap0, "MAIN_WIN/RPS_PATH"),
                     "path= override: bare identifier path is NOT registered")
    }
}

[test]
def test_per_call_id_and_path_combined(t : T?) {
    with_imgui_app(ID_OVERRIDE_FEATURE) $(d) {
        // id="rps_a", path="rps_b" — path leaf is "rps_b"; hex_id mangled
        // by both PushID("rps_a") (from id=) AND widget_prelude's PushID
        // ("rps_b" from widget_ident).
        var snap0 = wait_for_widget(d, "MAIN_WIN/rps_b", 15.0f)
        t |> success(snap0 != null,
                     "id=+path= combined: path takes the path= value")
        if (snap0 == null) return
        t |> success(!widget_exists(snap0, "MAIN_WIN/RPS_BOTH"),
                     "id=+path= combined: bare identifier path is NOT registered")
        t |> success(!widget_exists(snap0, "MAIN_WIN/rps_a"),
                     "id=+path= combined: id= literal does NOT become a path segment")

        // hex_id for rps_b must differ from rps_v1's (which had no id=
        // wrapping). Same widget_ident contribution but different outer
        // PushID, so hashes diverge.
        let combined = find_widget(snap0, "MAIN_WIN/rps_b")?["hex_id"] ?? 0u
        let path_only = find_widget(snap0, "MAIN_WIN/rps_v1")?["hex_id"] ?? 0u
        t |> success(combined != 0u && path_only != 0u,
                     "both hex_ids present")
        t |> success(combined != path_only,
                     "id= layered on path= yields a distinct hex_id from path= alone")
    }
}
