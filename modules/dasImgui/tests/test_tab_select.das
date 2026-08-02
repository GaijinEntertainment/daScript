options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `imgui_select` + tab-select introspection (`TabItemState.selected`).
//! The containers_layout fixture has a tab_bar (MAIN_TABS) with SETTINGS / AUDIO / ABOUT tabs;
//! SETTINGS is active by default (its PHYSICS_TREE child renders). `imgui_select(AUDIO_TAB)`
//! activates Audio — AUDIO_TAB.selected flips true, SETTINGS_TAB.selected false, and the
//! settings-tab child stops rendering. `selected` is snapshot-visible (the TabItemState struct
//! is reflected into the widget payload). Headless-safe (drives the dialog over the live API).

let FEATURE = "modules/dasImgui/examples/features/containers_layout.das"

def private is_selected(var snap : JsonValue?; tab : string) : bool {
    return widget_payload_field(snap, tab, "selected") ?? false
}

[test]
def test_tab_select_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE/GRAVITY", 15.0f)
        t |> success(snap != null, "settings-tab child renders initially (SETTINGS active)")
        if (snap == null) {
            return
        }

        // `selected` is exposed in the snapshot payload (TabItemState reflected).
        t |> success(is_selected(snap, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB"),
                     "SETTINGS_TAB.selected == true initially")
        t |> success(!is_selected(snap, "LAYOUT_WIN/MAIN_TABS/AUDIO_TAB"),
                     "AUDIO_TAB.selected == false initially")

        // Activate the Audio tab from automation.
        select_widget(d, "LAYOUT_WIN/MAIN_TABS/AUDIO_TAB")
        let became = wait_until_sec(d, 5.0f) $(var s) {
            return is_selected(s, "LAYOUT_WIN/MAIN_TABS/AUDIO_TAB")
        }
        t |> success(became != null, "AUDIO_TAB.selected == true after imgui_select")

        // The previously-active tab deselects and its child stops rendering.
        var s2 = snapshot(d)
        t |> success(!is_selected(s2, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB"),
                     "SETTINGS_TAB.selected == false after select")
        t |> success(!widget_rendered(s2, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE/GRAVITY"),
                     "settings-tab child stops rendering once Audio is active")
    }
}
