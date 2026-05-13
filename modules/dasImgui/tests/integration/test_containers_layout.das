options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 0b.3 layout containers — tab_bar, tab_item,
//! tree_node, collapsing_header, menu_bar, menu. Verifies multi-level
//! nesting (window > tab_bar > tab_item > tree_node > leaf) and the
//! imgui_open / imgui_close path on tree nodes.

let LAYOUT_FEATURE = "modules/dasImgui/examples/features/containers_layout.das"

[test]
def test_containers_layout_smoke(t : T?) {
    with_imgui_app(LAYOUT_FEATURE) $(d) {
        // Wait for the deepest leaf to register — proves the entire nested
        // path was walked during the first frame.
        var snap0 = wait_for_widget(d, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE/GRAVITY", 15.0f)
        t |> success(snap0 != null,
                     "deeply-nested GRAVITY leaf registered under 5-level path")
        if (snap0 == null) return

        // ----- Each container registers under its own path key -----
        t |> equal(find_widget(snap0, "LAYOUT_WIN")?["kind"] ?? "",
                   "window", "LAYOUT_WIN.kind == window")
        t |> equal(find_widget(snap0, "LAYOUT_WIN/MAIN_TABS")?["kind"] ?? "",
                   "tab_bar", "tab_bar registers under window's path")
        t |> equal(find_widget(snap0, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB")?["kind"] ?? "",
                   "tab_item", "tab_item registers under tab_bar's path")
        t |> equal(find_widget(snap0, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE")?["kind"] ?? "",
                   "tree_node", "tree_node registers under tab_item's path")

        // ----- Sibling tab is also registered (tab_bar paints all tab_item
        //       headers regardless of which one is currently active) -----
        t |> success(widget_exists(snap0, "LAYOUT_WIN/MAIN_TABS/AUDIO_TAB"),
                     "sibling AUDIO_TAB also registered alongside SETTINGS_TAB")

        // ----- Menu bar + menus are part of the path too -----
        t |> equal(find_widget(snap0, "LAYOUT_WIN/MAIN_BAR")?["kind"] ?? "",
                   "menu_bar", "menu_bar registers")
        // FILE_MENU's body only renders when the user clicks open the menu,
        // so its content nesting (FILE_MENU/FILE_NEW etc.) is tested
        // implicitly only — the menu container itself registers every frame.
        t |> equal(find_widget(snap0, "LAYOUT_WIN/MAIN_BAR/FILE_MENU")?["kind"] ?? "",
                   "menu", "menu registers under menu_bar's path")

        // ----- imgui_set steers a leaf nested inside tree_node -----
        set_value(d, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE/GRAVITY", JV(12.5f))
        t |> success(wait_for_payload_value(d,
                        "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE/GRAVITY",
                        "value", 12.5f, 300),
                     "imgui_set on 5-deep nested leaf works")

        // ----- collapsing_header path key -----
        t |> equal(find_widget(snap0, "LAYOUT_WIN/LOG_HEADER")?["kind"] ?? "",
                   "collapsing_header", "collapsing_header registers")
        // It's flagged DefaultOpen so opened=true is expected on first frame.
        t |> success(wait_for_bool_value(d, "LAYOUT_WIN/LOG_HEADER", "opened", true, 300),
                     "DefaultOpen header reports opened=true")

        // ----- imgui_close on a tree_node closes it via SetNextItemOpen -----
        // We assert by leaf-visibility (closed tree_node hides its leaves)
        // rather than `opened==false`, because `@optional opened : bool`
        // omits the field from the JSON envelope when false. Leaf
        // disappearance is the more meaningful signal anyway — it's what
        // a UI-driving test actually cares about.
        close_widget(d, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE")
        var snap_closed = wait_until(d, 300) $(var snap) {
            return !widget_rendered(snap, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE/GRAVITY")
        }
        t |> success(snap_closed != null,
                     "imgui_close on tree_node hides its nested leaves next frame")

        // ----- imgui_open re-opens it -----
        open_widget(d, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE")
        t |> success(wait_for_render(d,
                        "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE/GRAVITY",
                        5.0f) != null,
                     "imgui_open on tree_node re-renders its leaves")
    }
}
