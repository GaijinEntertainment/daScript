options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smokes for the imgui_demo scenes: each boots one
//! single-scene harness driver under examples/imgui_demo.

[test(description = "the about scene boots and ABOUT_WINDOW registers as a window", timeout = 1200)]
def test_imgui_demo_about_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/imgui_demo/harness_about.das") $(d) {
        var snap0 = wait_for_widget(d, "ABOUT_WINDOW", 15.0f)
        t |> success(snap0 != null, "ABOUT_WINDOW registers (demo renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "ABOUT_WINDOW")?["kind"] ?? "", "window",
                   "ABOUT_WINDOW.kind == 'window'")
    }
}

[test(description = "the app_console scene boots CONSOLE_WIN as a window holding its CONSOLE_INPUT command field", timeout = 1200)]
def test_imgui_demo_app_console_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/imgui_demo/harness_app_console.das") $(d) {
        var snap0 = wait_for_widget(d, "CONSOLE_WIN", 15.0f)
        t |> success(snap0 != null, "CONSOLE_WIN registers (console renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "CONSOLE_WIN")?["kind"] ?? "", "window",
                   "CONSOLE_WIN.kind == 'window'")
        let snap1 = wait_for_widget(d, "CONSOLE_WIN/CONSOLE_INPUT", 15.0f)
        t |> success(snap1 != null, "CONSOLE_INPUT registers inside CONSOLE_WIN")
    }
}

[test(description = "the app_log scene boots LOG_WIN as a window holding its LOG_BTN_ADD5 debug button", timeout = 1200)]
def test_imgui_demo_app_log_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/imgui_demo/harness_app_log.das") $(d) {
        var snap0 = wait_for_widget(d, "LOG_WIN", 15.0f)
        t |> success(snap0 != null, "LOG_WIN registers (log renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "LOG_WIN")?["kind"] ?? "", "window",
                   "LOG_WIN.kind == 'window'")
        let snap1 = wait_for_widget(d, "LOG_WIN/LOG_BTN_ADD5", 15.0f)
        t |> success(snap1 != null, "LOG_BTN_ADD5 registers inside LOG_WIN")
    }
}

[test(description = "the app_main_menu scene boots and MAIN_BAR_DEMO registers as a main_menu_bar", timeout = 1200)]
def test_imgui_demo_app_main_menu_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/imgui_demo/harness_app_main_menu.das") $(d) {
        var snap0 = wait_for_widget(d, "MAIN_BAR_DEMO", 15.0f)
        t |> success(snap0 != null, "MAIN_BAR_DEMO registers (demo renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_BAR_DEMO")?["kind"] ?? "", "main_menu_bar",
                   "MAIN_BAR_DEMO.kind == 'main_menu_bar'")
    }
}

[test(description = "the columns scene, hosted in MAIN_WIN, renders its COLUMNS_OUTER tree node inside the host window", timeout = 1200)]
def test_imgui_demo_columns_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/imgui_demo/harness_columns.das") $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN", 15.0f)
        t |> success(snap0 != null, "MAIN_WIN registers (host window renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_WIN")?["kind"] ?? "", "window",
                   "MAIN_WIN.kind == 'window'")
        let snap1 = wait_for_widget(d, "MAIN_WIN/COLUMNS_OUTER", 15.0f)
        t |> success(snap1 != null, "COLUMNS_OUTER registers inside MAIN_WIN")
    }
}

[test(description = "the inputs scene, hosted in MAIN_WIN, renders its INPUTS_SECTION collapsing header inside the host window", timeout = 1200)]
def test_imgui_demo_inputs_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/imgui_demo/harness_inputs.das") $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN", 15.0f)
        t |> success(snap0 != null, "MAIN_WIN registers (host window renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_WIN")?["kind"] ?? "", "window",
                   "MAIN_WIN.kind == 'window'")
        let snap1 = wait_for_widget(d, "MAIN_WIN/INPUTS_SECTION", 15.0f)
        t |> success(snap1 != null, "INPUTS_SECTION registers inside MAIN_WIN")
    }
}

[test(description = "the popups scene, hosted in MAIN_WIN, renders its POPUPS_SECTION collapsing header inside the host window", timeout = 1200)]
def test_imgui_demo_popups_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/imgui_demo/harness_popups.das") $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN", 15.0f)
        t |> success(snap0 != null, "MAIN_WIN registers (host window renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_WIN")?["kind"] ?? "", "window",
                   "MAIN_WIN.kind == 'window'")
        let snap1 = wait_for_widget(d, "MAIN_WIN/POPUPS_SECTION", 15.0f)
        t |> success(snap1 != null, "POPUPS_SECTION registers inside MAIN_WIN")
    }
}

[test(description = "the style_editor scene, hosted in MAIN_WIN, renders its STYLE_TABS tab bar inside the host window", timeout = 1200)]
def test_imgui_demo_style_editor_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/imgui_demo/harness_style_editor.das") $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN", 15.0f)
        t |> success(snap0 != null, "MAIN_WIN registers (host window renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_WIN")?["kind"] ?? "", "window",
                   "MAIN_WIN.kind == 'window'")
        let snap1 = wait_for_widget(d, "MAIN_WIN/STYLE_TABS", 15.0f)
        t |> success(snap1 != null, "STYLE_TABS registers inside MAIN_WIN")
    }
}

[test(description = "the tables scene, hosted in MAIN_WIN, renders its TABLES_SECTION collapsing header inside the host window", timeout = 1200)]
def test_imgui_demo_tables_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/imgui_demo/harness_tables.das") $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN", 15.0f)
        t |> success(snap0 != null, "MAIN_WIN registers (host window renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_WIN")?["kind"] ?? "", "window",
                   "MAIN_WIN.kind == 'window'")
        let snap1 = wait_for_widget(d, "MAIN_WIN/TABLES_SECTION", 15.0f)
        t |> success(snap1 != null, "TABLES_SECTION registers inside MAIN_WIN")
    }
}

[test(description = "the user_guide scene boots and its MAIN_WIN host registers as a window", timeout = 1200)]
def test_imgui_demo_user_guide_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/imgui_demo/harness_user_guide.das") $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN", 15.0f)
        t |> success(snap0 != null, "MAIN_WIN registers (host window renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_WIN")?["kind"] ?? "", "window",
                   "MAIN_WIN.kind == 'window'")
    }
}

[test(description = "the widgets scene, hosted in MAIN_WIN, renders its WIDGETS_SECTION collapsing header inside the host window", timeout = 1200)]
def test_imgui_demo_widgets_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/imgui_demo/harness_widgets.das") $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN", 15.0f)
        t |> success(snap0 != null, "MAIN_WIN registers (host window renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_WIN")?["kind"] ?? "", "window",
                   "MAIN_WIN.kind == 'window'")
        let snap1 = wait_for_widget(d, "MAIN_WIN/WIDGETS_SECTION", 15.0f)
        t |> success(snap1 != null, "WIDGETS_SECTION registers inside MAIN_WIN")
    }
}
