options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: main_menu_bar — top-level menu container (boost v2 §3.3).
// SHOWS:   main_menu_bar { menu(...) { menu_item(...) } } — screen-top bar,
//          distinct from window-attached menu_bar. The container emits a
//          MainMenuBarState global, and the children's idents nest under
//          MAIN_MENU/<menu>/<item> path keys. Click an item via imgui_click
//          to toggle its check state.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals|keys|map(select(startswith("MAIN_MENU_BAR/")))'
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"MAIN_MENU_BAR/FILE_MENU/MI_SAVE"}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/menu_main.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/menu_main.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/menu_main.das
// =============================================================================

[export]
def init() {
    harness_init("main_menu_bar — boost v2 §3.3", 720, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    main_menu_bar(MAIN_MENU_BAR) {
        menu(FILE_MENU, (text = "File", enabled = true)) {
            menu_item(MI_SAVE, (text = "Save", shortcut = "Ctrl+S"))
            menu_item(MI_RELOAD, (text = "Reload", shortcut = "Ctrl+R"))
            separator(FILE_SEP)
            menu_item(MI_QUIT, (text = "Quit", shortcut = "Ctrl+Q"))
        }
        menu(VIEW_MENU, (text = "View", enabled = true)) {
            menu_item(MI_GRID, (text = "Grid"))
            menu_item(MI_RULER, (text = "Ruler"))
        }
    }

    SetNextWindowPos(ImVec2(60.0, 60.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(620.0, 220.0), ImGuiCond.Always)
    window(BODY, (text = "main_menu_bar body", closable = false,
                  flags = ImGuiWindowFlags.None)) {
        text(BODY_LEAD, (text = "Top-bar is the main_menu_bar above; this window is unrelated."))
        text(BODY_SAVE, (text = "Save toggled: {MI_SAVE.value}"))
        text(BODY_RELOAD, (text = "Reload toggled: {MI_RELOAD.value}"))
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
