options gen2
options _comment_hygiene = true
options indenting = 4

module app_main_menu

require imgui
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_drawlist_builtin     // with_window_drawlist + add_rect_filled
require daslib/enum_trait

// Indexed widget tables for the per-frame loops:
//   FILE_CHILD_SCROLL — 10 text() lines inside the scrolling child window
//   FILE_COLORS_DUMMY / FILE_COLORS_SAMELINE / FILE_COLORS_ITEM — one
//   dummy+same_line+menu_item per ImGuiCol entry in the Colors submenu.
// `[widget]` macros don't auto-emit table globals — indexed forms require
// the table declared at module scope.
var private FILE_CHILD_SCROLL : table<int; NarrativeState>
var private FILE_COLORS_DUMMY : table<int; EmptyMarkerState>
var private FILE_COLORS_SAMELINE : table<int; EmptyMarkerState>
var private FILE_COLORS_ITEM : table<int; ToggleState>

// Mirrors imgui_demo.cpp:7009-7030 — static void ShowExampleAppMainMenuBar().
def ShowExampleAppMainMenuBar() {
    main_menu_bar(MAIN_BAR_DEMO) {
        menu(APP_MENU_FILE, (text = "File", enabled = true)) {
            ShowExampleMenuFile()
        }
        menu(APP_MENU_EDIT, (text = "Edit", enabled = true)) {
            if (menu_item(EDIT_UNDO, (text = "Undo", shortcut = "CTRL+Z"))) {}
            // C++ passes enabled=false to render a disabled "Redo" — static
            // (no toggle), so menu_label is the v2 form.
            if (menu_label(EDIT_REDO, (text = "Redo", shortcut = "CTRL+Y",
                                       selected = false, enabled = false))) {}
            separator(EDIT_SEP)
            if (menu_item(EDIT_CUT, (text = "Cut", shortcut = "CTRL+X"))) {}
            if (menu_item(EDIT_COPY, (text = "Copy", shortcut = "CTRL+C"))) {}
            if (menu_item(EDIT_PASTE, (text = "Paste", shortcut = "CTRL+V"))) {}
        }
    }
}

// Mirrors imgui_demo.cpp:7034-7113 — static void ShowExampleMenuFile().
def ShowExampleMenuFile() {
    // C++ uses MenuItem with enabled=false as a static section-header label —
    // menu_label is the v2 form (no toggle behavior, just click_count).
    menu_label(FILE_DEMO_HEADER, (text = "(demo menu)", shortcut = "",
                                  selected = false, enabled = false))
    if (menu_item(FILE_NEW, (text = "New"))) {}
    if (menu_item(FILE_OPEN, (text = "Open", shortcut = "Ctrl+O"))) {}
    menu(FILE_OPEN_RECENT, (text = "Open Recent", enabled = true)) {
        menu_item(FILE_FISH_C, (text = "fish_hat.c"))
        menu_item(FILE_FISH_INL, (text = "fish_hat.inl"))
        menu_item(FILE_FISH_H, (text = "fish_hat.h"))
        menu(FILE_MORE, (text = "More..", enabled = true)) {
            menu_item(FILE_HELLO, (text = "Hello"))
            menu_item(FILE_SAILOR, (text = "Sailor"))
            // Recurse re-enters ShowExampleMenuFile — every [widget] global aliases
            // the outer call (same module-scope identifier). ImGui's max-depth limit
            // gates the dropdown; no test reads per-level state, so aliasing is fine.
            menu(FILE_RECURSE, (text = "Recurse..", enabled = true)) {
                ShowExampleMenuFile()
            }
        }
    }
    if (menu_item(FILE_SAVE, (text = "Save", shortcut = "Ctrl+S"))) {}
    if (menu_item(FILE_SAVE_AS, (text = "Save As.."))) {}

    separator(FILE_SEP_A)

    menu(FILE_OPTIONS_A, (text = "Options", enabled = true)) {
        menu_item(FILE_OPTIONS_ENABLED, (text = "Enabled", init = true))
        child(FILE_CHILD, (text = "child", size = float2(0.0f, 60.0f),
                           child_flags = ImGuiChildFlags.Borders,
                           window_flags = ImGuiWindowFlags.None)) {
            for (i in range(10)) {
                text(FILE_CHILD_SCROLL[i], (text = "Scrolling Text {i}"))
            }
        }
        slider_float(FILE_OPTIONS_SLIDER, (text = "Value",
                                           init = 0.5f, bounds = (0.0f, 1.0f)))
        input_float(FILE_OPTIONS_INPUT, (text = "Input",
                                         init = 0.5f, step = 0.1f))
        combo(MENU_FILE_COMBO, (text = "Combo",
                                items <- ["Yes", "No", "Maybe"]))
    }

    menu(FILE_COLORS, (text = "Colors", enabled = true)) {
        let sz = GetTextLineHeight()
        for (col in type<ImGuiCol>) {
            if (col == ImGuiCol.COUNT) continue
            let name = GetStyleColorName(col)
            let p = GetCursorScreenPos()
            with_window_drawlist() $(var dl) {
                dl |> add_rect_filled(p, float2(p.x + sz, p.y + sz), GetColorU32(col))
            }
            dummy(FILE_COLORS_DUMMY[int(col)], (size = float2(sz, sz)))
            same_line(FILE_COLORS_SAMELINE[int(col)])
            menu_item(FILE_COLORS_ITEM[int(col)], (text = name))
        }
    }

    // Appending again to an existing menu (same label re-opens the merged
    // body in ImGui). DIFFERENT ident than FILE_OPTIONS_A so the v2 state
    // globals don't collide.
    menu(FILE_OPTIONS_B, (text = "Options", enabled = true)) {
        checkbox(FILE_OPTIONS_APPEND_CHECK, (text = "SomeOption", init = true))
    }

    menu(FILE_DISABLED, (text = "Disabled", enabled = false)) {
        // Body unreachable while enabled=false; left for parity with the
        // C++ `IM_ASSERT(0)` shape.
    }
    // C++ passes selected=true for a permanent check mark — menu_label since
    // it doesn't toggle.
    if (menu_label(FILE_CHECKED, (text = "Checked", shortcut = "",
                                  selected = true))) {}
    separator(FILE_SEP_B)
    if (menu_item(FILE_QUIT, (text = "Quit", shortcut = "Alt+F4"))) {}
}
