options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! imgui_demo example apps open and register their widgets.

let CR_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_custom_rendering.das"

[test(description = "The custom-rendering app opens with its tab bar, renders the default Primitives tab, and installs its canvas context menu closed.", timeout = 1200)]
def test_app_custom_rendering_smoke(t : T?) {
    with_imgui_app(CR_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "CR_WIN", 15.0f)
        t |> success(snap0 != null, "CR_WIN host window renders")
        if (snap0 == null) return

        t |> success(widget_exists(snap0, "CR_WIN/CR_TABS"),
                     "CR_WIN/CR_TABS tab_bar registers")
        let tab_prim = find_widget(snap0, "CR_WIN/CR_TABS/CR_TAB_PRIM")
        t |> equal(tab_prim?["kind"] ?? "", "tab_item",
                   "CR_TAB_PRIM (default-active) renders as tab_item")

        // Context-menu popup is always installed (per the cpp port) but
        // open=false by default — confirms the popup container is wired.
        let ctx = find_widget(snap0, "CR_CV_CTX_MENU")
        t |> equal(ctx?["kind"] ?? "", "popup",
                   "CR_CV_CTX_MENU registered as kind=popup")
        t |> equal(ctx?["payload"]?["open"] ?? true, false,
                   "context menu starts closed")
    }
}

let DOCS_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_documents.das"

[test(description = "The documents app registers its File menu and tab bar, each open-checkbox mirrors its seeded doc.open, and the close-confirmation modal starts closed.", timeout = 1200)]
def test_app_documents_smoke(t : T?) {
    with_imgui_app(DOCS_FEATURE) $(d) {
        var snap = wait_for_widget(d, "DOCS_WIN", 15.0f)
        t |> success(snap != null, "DOCS_WIN host window renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "DOCS_WIN/DOCS_MENUBAR/DOCS_FILE_MENU"),
                     "File menu in the menu bar")
        t |> success(widget_exists(snap, "DOCS_WIN/DOCS_TABS"),
                     "DOCS_TABS tab_bar registers")

        // seed_docs() opens docs 0, 1, 2 and leaves 3, 4, 5 closed.
        let cb0_val = find_widget(snap, "DOCS_WIN/DOCS_OPEN_CHECKBOX[0]")?["payload"]?["value"] ?? false
        t |> equal(cb0_val, true, "DOCS_OPEN_CHECKBOX[0] mirrors seeded doc.open=true (Lettuce)")
        let cb3_val = find_widget(snap, "DOCS_WIN/DOCS_OPEN_CHECKBOX[3]")?["payload"]?["value"] ?? true
        t |> equal(cb3_val, false, "DOCS_OPEN_CHECKBOX[3] mirrors seeded doc.open=false (Tomato)")

        // Close-confirmation modal is installed; opens only when a dirty
        // doc is being closed. Fresh fixture: starts closed.
        let modal = find_widget(snap, "DOCS_CLOSE_MODAL")
        t |> equal(modal?["kind"] ?? "", "popup_modal",
                   "DOCS_CLOSE_MODAL registered as popup_modal")
        t |> equal(modal?["payload"]?["open"] ?? true, false,
                   "close-confirmation modal starts closed (no dirty docs)")
    }
}

let AUTO_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_auto_resize.das"

[test(description = "The auto-resize app's line-count slider registers with its seeded value 10 and force_set moves it to 5.", timeout = 1200)]
def test_app_small_auto_resize_smoke(t : T?) {
    with_imgui_app(AUTO_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SMALL_AUTO_WIN/SMALL_AUTO_SLIDER", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_AUTO_WIN/SMALL_AUTO_SLIDER registers after first frame")
        if (snap0 == null) return

        var slider = find_widget(snap0, "SMALL_AUTO_WIN/SMALL_AUTO_SLIDER")
        t |> equal(slider?["kind"] ?? "", "slider_int",
                   "slider registers as kind=slider_int")
        t |> equal(slider?["payload"]?["value"] ?? 0, 10,
                   "slider initial value matches the init=10 seed")

        // Drive value to 5 and verify the new line count via the per-line
        // text state count.
        force_set_value(d, "SMALL_AUTO_WIN/SMALL_AUTO_SLIDER", JV(5))
        t |> success(wait_for_int_value(d, "SMALL_AUTO_WIN/SMALL_AUTO_SLIDER",
                                        "value", 5, 300),
                     "imgui_force_set drops the slider to 5")
    }
}

let CONSTR_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_constrained_resize.das"

[test(description = "The constrained-resize app registers its constraint combo defaulting to the callback-driven 16:9 aspect type and its preset-size buttons.", timeout = 1200)]
def test_app_small_constrained_resize_smoke(t : T?) {
    with_imgui_app(CONSTR_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SMALL_CONSTR_WIN/SMALL_CONSTR_COMBO", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_CONSTR_WIN/SMALL_CONSTR_COMBO registers after first frame")
        if (snap0 == null) return

        var combo_entry = find_widget(snap0, "SMALL_CONSTR_WIN/SMALL_CONSTR_COMBO")
        t |> equal(combo_entry?["kind"] ?? "", "combo",
                   "constraint combo registers as kind=combo")
        t |> equal(combo_entry?["payload"]?["value"] ?? 0, 6,
                   "default constraint type is 6 (Aspect Ratio 16:9 - callback path)")

        var btn200 = find_widget(snap0, "SMALL_CONSTR_WIN/SMALL_CONSTR_BTN_200")
        t |> equal(btn200?["kind"] ?? "", "button",
                   "Set 200x200 button registers under window path")
    }
}

let FULL_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_fullscreen.das"

[test(description = "The fullscreen app registers its window and the Close button under the window path.", timeout = 1200)]
def test_app_small_fullscreen_smoke(t : T?) {
    with_imgui_app(FULL_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SMALL_FULL_WIN/SMALL_FULL_BTN_CLOSE", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_FULL_WIN/SMALL_FULL_BTN_CLOSE registers after first frame")
        if (snap0 == null) return

        var win_entry = find_widget(snap0, "SMALL_FULL_WIN")
        t |> equal(win_entry?["kind"] ?? "", "window",
                   "SMALL_FULL_WIN registers as kind=window")

        var btn = find_widget(snap0, "SMALL_FULL_WIN/SMALL_FULL_BTN_CLOSE")
        t |> equal(btn?["kind"] ?? "", "button",
                   "Close button registers under window path")
    }
}

let LAYOUT_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_layout.das"

[test(description = "The layout app registers its menu bar, the left pane's selectables under the child path and the Revert button under the right-pane group.", timeout = 1200)]
def test_app_small_layout_smoke(t : T?) {
    with_imgui_app(LAYOUT_FEATURE) $(d) {
        // First selectable in the left pane — nested under the LEFT_CHILD container.
        var snap0 = wait_for_widget(d, "SMALL_LAYOUT_WIN/SMALL_LAYOUT_LEFT_CHILD/SMALL_LAYOUT_LEFT_SEL[0]", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_LAYOUT_WIN/SMALL_LAYOUT_LEFT_CHILD/SMALL_LAYOUT_LEFT_SEL[0] registers after first frame")
        if (snap0 == null) return

        // Window container itself.
        var win_entry = find_widget(snap0, "SMALL_LAYOUT_WIN")
        t |> equal(win_entry?["kind"] ?? "", "window",
                   "SMALL_LAYOUT_WIN registers as kind=window")

        // Menu-bar + File menu container chain.
        var mb = find_widget(snap0, "SMALL_LAYOUT_WIN/SMALL_LAYOUT_MENU_BAR")
        t |> equal(mb?["kind"] ?? "", "menu_bar",
                   "menu_bar nested under window registers under hierarchical path")

        // First selectable is the kind=selectable confirmation.
        var sel0 = find_widget(snap0, "SMALL_LAYOUT_WIN/SMALL_LAYOUT_LEFT_CHILD/SMALL_LAYOUT_LEFT_SEL[0]")
        t |> equal(sel0?["kind"] ?? "", "selectable",
                   "first selectable in the 100-row loop registers under child path")

        // Right-pane chain: button is nested under the group container.
        var btn = find_widget(snap0, "SMALL_LAYOUT_WIN/SMALL_LAYOUT_RIGHT_GROUP/SMALL_LAYOUT_BTN_REVERT")
        t |> equal(btn?["kind"] ?? "", "button",
                   "Revert button registers under window/group hierarchical path")
    }
}

let LONG_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_long_text.das"

[test(description = "The long-text app registers its test-type combo and the scrolling log child under the window.", timeout = 1200)]
def test_app_small_long_text_smoke(t : T?) {
    with_imgui_app(LONG_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SMALL_LONG_WIN/SMALL_LONG_COMBO", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_LONG_WIN/SMALL_LONG_COMBO registers after first frame")
        if (snap0 == null) return

        var combo_entry = find_widget(snap0, "SMALL_LONG_WIN/SMALL_LONG_COMBO")
        t |> equal(combo_entry?["kind"] ?? "", "combo",
                   "test-type combo registers as kind=combo")

        var child = find_widget(snap0, "SMALL_LONG_WIN/SMALL_LONG_CHILD")
        t |> equal(child?["kind"] ?? "", "child",
                   "scrolling child container under window registers")
    }
}

let PROPED_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_property_editor.das"

[test(description = "The property-editor app registers its object table as a data_table under the window.", timeout = 1200)]
def test_app_small_property_editor_smoke(t : T?) {
    with_imgui_app(PROPED_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SMALL_PROPED_WIN/SMALL_PROPED_TABLE", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_PROPED_WIN/SMALL_PROPED_TABLE registers after first frame")
        if (snap0 == null) return

        var tab = find_widget(snap0, "SMALL_PROPED_WIN/SMALL_PROPED_TABLE")
        t |> equal(tab?["kind"] ?? "", "data_table",
                   "SMALL_PROPED_TABLE registers as kind=data_table")
    }
}

let OVERLAY_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_simple_overlay.das"

[test(description = "The simple-overlay app registers its overlay window and wires its right-click context popup container.", timeout = 1200)]
def test_app_small_simple_overlay_smoke(t : T?) {
    with_imgui_app(OVERLAY_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SMALL_OVERLAY_WIN/SMALL_OVERLAY_SEP", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_OVERLAY_WIN/SMALL_OVERLAY_SEP registers after first frame")
        if (snap0 == null) return

        var win_entry = find_widget(snap0, "SMALL_OVERLAY_WIN")
        t |> equal(win_entry?["kind"] ?? "", "window",
                   "SMALL_OVERLAY_WIN registers as kind=window")

        // popup_context_window registers but its body only fires when right-clicked.
        var ctx = find_widget(snap0, "SMALL_OVERLAY_WIN/SMALL_OVERLAY_CTX")
        t |> equal(ctx?["kind"] ?? "", "popup_context_window",
                   "context popup container registers under window path")
    }
}

let TITLES_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_window_titles.das"

[test(description = "Three windows sharing a visible title through ## and ### ID separators register as three distinct windows.", timeout = 1200)]
def test_app_small_window_titles_smoke(t : T?) {
    with_imgui_app(TITLES_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SMALL_TITLES_W1/SMALL_TITLES_W1_TEXT", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_TITLES_W1/SMALL_TITLES_W1_TEXT registers after first frame")
        if (snap0 == null) return

        var w1 = find_widget(snap0, "SMALL_TITLES_W1")
        var w2 = find_widget(snap0, "SMALL_TITLES_W2")
        var w3 = find_widget(snap0, "SMALL_TITLES_W3")
        t |> equal(w1?["kind"] ?? "", "window", "window 1 (##1) registers")
        t |> equal(w2?["kind"] ?? "", "window", "window 2 (##2) registers")
        t |> equal(w3?["kind"] ?? "", "window", "window 3 (###AnimatedTitle) registers")
    }
}
