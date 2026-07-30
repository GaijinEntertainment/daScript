options gen2
options _comment_hygiene = true
options indenting = 4

module popups

require imgui
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_scope_builtin
require imgui/imgui_style_builtin
require app_main_menu

// Mirrors imgui_demo.cpp:3695-3990 — static void ShowDemoWindowPopups().
//
// Four TreeNode'd sub-sections:
//   - Popups               (cpp:3719-3810)
//   - Context menus        (cpp:3812-3897)
//   - Modals               (cpp:3899-3973)
//   - Menus inside a window (cpp:3975-3989)
//
// Boost-surface conventions applied here:
//   - `popup` / `popup_modal` / `popup_context_item` containers replace the
//     raw Begin*/End* pairs. Triggers go through `open_popup(STATE)` for
//     stateful popups; `open_popup(str_id)` / `open_popup_on_item_click` for
//     stateless `popup_context_item` cases.
//   - `menu` / `menu_bar` containers wrap the dropdown bodies.
//   - `tree_node` containers wrap each sub-section (no test-engine markers).
//   - `with_style((ImGuiStyleVar.FramePadding, …))` scopes the (0,0) padding
//     push around the "Don't ask me next time" checkbox.
//   - Loop-driven selection rows use `selectable_label(STATE_TABLE[i], ...)`
//     with module-scope indexed tables — per-row ClickState gives telemetry
//     and path-based addressing.

// =============================================================================
// Module-scope state (plain non-widget vars + indexed widget tables).
// `[widget]`/`[container]`-managed state is auto-emitted from the IDENT arg.
// =============================================================================

// "Popups" sub-section
var private POPUPS_SELECTED_FISH = -1
let private FISH_NAMES <- ["Bream", "Haddock", "Mackerel", "Pollock", "Tilefish"]
// Loop indices for the Toggle/Sub-menu/Stacked popups — one toggle per fish.
var private POPUPS_TOGGLE_ITEM : table<int; ToggleState>
var private POPUPS_STACKED_TOGGLE_ITEM : table<int; ToggleState>
// Selectable rows inside the "Select.." popup body — one ClickState per fish.
var private POPUPS_FISH_ROW : table<int; ClickState>

// "Context menus" sub-section
var private POPUPS_CTX1_SELECTED = -1
var private POPUPS_CTX2_VALUE = 0.5f
// Example 1: 5 rows, each row has its own popup_context_item (right-click
// the row to open). Indexed state per row.
var private POPUPS_CTX1_POPUP : table<int; PopupContextItemState>
// Per-row selectable_label state for Example 1's index-selection grid.
var private POPUPS_CTX1_ROW : table<int; ClickState>

// "Modals" sub-section — Stacked combo & color editor
var private POPUPS_STACKED_COMBO_ITEMS <- ["aaaa", "bbbb", "cccc", "dddd", "eeee"]

// =============================================================================
// ShowDemoWindowPopups — dispatcher
// =============================================================================
def ShowDemoWindowPopups() {
    collapsing_header(POPUPS_SECTION,
        (text = "Popups & Modal windows", closable = false,
         flags = ImGuiTreeNodeFlags.None)) {

        PopupsSection()
        ContextMenusSection()
        ModalsSection()
        MenusInRegularWindowSection()
    }
}


// =============================================================================
// Popups — cpp:3719-3810
// =============================================================================
def private PopupsSection() {
    tree_node(POPUPS_POPUPS_SUB,
        (text = "Popups", flags = ImGuiTreeNodeFlags.None)) {

        text_wrapped((text = "When a popup is active, it inhibits interacting with windows " +
            "that are behind the popup. Clicking outside the popup closes it."))

        // ----- "Select.." popup -----
        if (button(POPUPS_SELECT_BTN, (text = "Select.."))) {
            open_popup(POPUPS_SELECT_POPUP)
        }
        same_line(POPUPS_SL_1)
        let fish_label = (POPUPS_SELECTED_FISH == -1) ? "<None>" : FISH_NAMES[POPUPS_SELECTED_FISH]
        text_unformatted((text = fish_label))
        popup(POPUPS_SELECT_POPUP,
              (text = "my_select_popup", flags = ImGuiWindowFlags.None)) {
            separator_text("Aquarium")
            for (i in range(length(FISH_NAMES))) {
                if (selectable_label(POPUPS_FISH_ROW[i], FISH_NAMES[i], false)) {
                    POPUPS_SELECTED_FISH = i
                }
            }
        }

        // ----- "Toggle.." popup -----
        if (button(POPUPS_TOGGLE_BTN, (text = "Toggle.."))) {
            open_popup(POPUPS_TOGGLE_POPUP)
        }
        popup(POPUPS_TOGGLE_POPUP,
              (text = "my_toggle_popup", flags = ImGuiWindowFlags.None)) {
            for (i in range(length(FISH_NAMES))) {
                menu_item(POPUPS_TOGGLE_ITEM[i], (text = FISH_NAMES[i]))
            }
            menu(POPUPS_TOGGLE_SUBMENU, (text = "Sub-menu", enabled = true)) {
                menu_label(POPUPS_TOGGLE_SUBMENU_CLICK,
                           (text = "Click me", shortcut = "",
                            selected = false, enabled = true))
            }

            separator(POPUPS_TOGGLE_SEP)
            text("Tooltip here")
            set_item_tooltip(POPUPS_TOGGLE_TIP, (text = "I am a tooltip over a popup"))

            // Stacked popup nested inside the toggle popup.
            if (button(POPUPS_STACKED_BTN, (text = "Stacked Popup"))) {
                open_popup(POPUPS_STACKED_POPUP)
            }
            popup(POPUPS_STACKED_POPUP,
                  (text = "another popup", flags = ImGuiWindowFlags.None)) {
                for (i in range(length(FISH_NAMES))) {
                    menu_item(POPUPS_STACKED_TOGGLE_ITEM[i],
                              (text = FISH_NAMES[i]))
                }
                menu(POPUPS_STACKED_SUBMENU, (text = "Sub-menu", enabled = true)) {
                    menu_label(POPUPS_STACKED_SUBMENU_CLICK,
                               (text = "Click me", shortcut = "",
                                selected = false, enabled = true))
                    // Deepest popup: ImGui tracks the stack by str_id (reusing "another popup" is fine); only the das state ident must be unique per scope.
                    if (button(POPUPS_DEEPEST_BTN, (text = "Stacked Popup"))) {
                        open_popup(POPUPS_DEEPEST_POPUP)
                    }
                    popup(POPUPS_DEEPEST_POPUP,
                          (text = "another popup",
                           flags = ImGuiWindowFlags.None)) {
                        text("I am the last one here.")
                    }
                }
            }
        }

        // ----- "With a menu.." popup (has a MenuBar inside) -----
        if (button(POPUPS_MENUBAR_BTN, (text = "With a menu.."))) {
            open_popup(POPUPS_MENUBAR_POPUP)
        }
        popup(POPUPS_MENUBAR_POPUP,
              (text = "my_file_popup", flags = ImGuiWindowFlags.MenuBar)) {
            menu_bar(POPUPS_MENUBAR_BAR) {
                menu(POPUPS_MENUBAR_FILE, (text = "File", enabled = true)) {
                    ShowExampleMenuFile()
                }
                menu(POPUPS_MENUBAR_EDIT, (text = "Edit", enabled = true)) {
                    menu_item(POPUPS_MENUBAR_DUMMY, (text = "Dummy"))
                }
            }
            text("Hello from popup!")
            button(POPUPS_MENUBAR_DUMMY_BTN, (text = "This is a dummy button.."))
        }
    }
}


// =============================================================================
// Context menus — cpp:3812-3897
// =============================================================================
def private ContextMenusSection() {
    tree_node(POPUPS_CTX_SUB,
        (text = "Context menus", flags = ImGuiTreeNodeFlags.None)) {

        // Inlined HelpMarker — TextDisabled "(?)" + hover tooltip.
        text_disabled("(?)")
        item_tooltip(POPUPS_CTX_HM_INTRO_TIP) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "\"Context\" functions are simple helpers to associate a Popup to " +
                    "a given Item or Window identifier."))
            }
        }

        // ----- Example 1 — popup attached to last-item ID (no str_id) -----
        let ex1_names = fixed_array("Label1", "Label2", "Label3", "Label4", "Label5")
        for (n in range(5)) {
            if (selectable_label(POPUPS_CTX1_ROW[n], ex1_names[n],
                                 POPUPS_CTX1_SELECTED == n)) {
                POPUPS_CTX1_SELECTED = n
            }
            // popup_context_item with str_id="" binds to the previous item's ID (matches C++ BeginPopupContextItem NULL semantics).
            popup_context_item(POPUPS_CTX1_POPUP[n],
                               (str_id = "", flags = ImGuiPopupFlags.MouseButtonRight)) {
                POPUPS_CTX1_SELECTED = n
                text("This a popup for \"{ex1_names[n]}\"!")
                if (button((text = "Close"))) {
                    close_current_popup()
                }
            }
            set_item_tooltip(POPUPS_CTX1_TIP, (text = "Right-click to open popup"))
        }

        // ----- Example 2 — popup on Text() with an explicit str_id -----
        text_disabled("(?)")
        item_tooltip(POPUPS_CTX2_HM_TIP) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted("Text() elements don't have stable identifiers so we need to provide one.")
            }
        }
        text("Value = {POPUPS_CTX2_VALUE} <-- (1) right-click this text")
        popup_context_item(POPUPS_CTX2_POPUP,
                           (str_id = "my popup",
                            flags = ImGuiPopupFlags.MouseButtonRight)) {
            if (selectable_label(POPUPS_CTX2_SEL_ZERO, "Set to zero", false)) {
                POPUPS_CTX2_VALUE = 0.0f
            }
            if (selectable_label(POPUPS_CTX2_SEL_PI, "Set to PI", false)) {
                POPUPS_CTX2_VALUE = 3.1415f
            }
            SetNextItemWidth(-3.40282347e38f)
            edit_drag_float(unsafe(addr(POPUPS_CTX2_VALUE)),
                (id = "POPUPS_CTX2_DRAG", text = "##Value", speed = 0.1f))
        }

        // Second text triggers the same popup via OpenPopupOnItemClick (string-id form, popup_context_item is stateless).
        text("(2) Or right-click this text")
        open_popup_on_item_click("my popup")

        // Third trigger: a plain Button that opens the same popup via the stateless string-id form.
        if (button(POPUPS_CTX2_BTN, (text = "(3) Or click this button"))) {
            open_popup("my popup")
        }

        // ----- Example 3 — last-item ID popup, dynamic label / stable ID -----
        text_disabled("(?)")
        item_tooltip(POPUPS_CTX3_HM_TIP) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "Showcase using a popup ID linked to item ID, with the item having " +
                    "a changing label + stable ID using the ### operator."))
            }
        }
        // ### operator: the label before ### changes per frame, the ID after ### stays stable, so the popup keeps binding to the same Button (dynamic label from input_text's .value).
        button(POPUPS_CTX3_BTN,
               (text = "Button: {POPUPS_CTX3_INPUT.value}###Button"))
        popup_context_item(POPUPS_CTX3_POPUP,
                           (str_id = "", flags = ImGuiPopupFlags.MouseButtonRight)) {
            text("Edit name:")
            input_text(POPUPS_CTX3_INPUT, (text = "##edit", init = "Label1"))
            if (button((text = "Close"))) {
                close_current_popup()
            }
        }
        same_line(POPUPS_CTX3_SL)
        text("(<-- right-click here)")
    }
}


// =============================================================================
// Modals — cpp:3899-3973
// =============================================================================
def private ModalsSection() {
    tree_node(POPUPS_MODALS_SUB,
        (text = "Modals", flags = ImGuiTreeNodeFlags.None)) {

        text_wrapped("Modal windows are like popups but the user cannot close them by clicking outside.")

        // ----- "Delete?" modal -----
        if (button(POPUPS_MODAL_DELETE_BTN, (text = "Delete.."))) {
            open_popup(POPUPS_MODAL_DELETE)
        }
        // Center on appear: compute viewport center from Pos + Size*0.5 (ImGuiViewport::GetCenter() isn't surfaced through the binding).
        let vp = GetMainViewport()
        let vp_pos = vp.Pos
        let vp_size = vp.Size
        let center = float2(vp_pos.x + vp_size.x * 0.5f,
                            vp_pos.y + vp_size.y * 0.5f)
        SetNextWindowPos(center, ImGuiCond.Appearing, float2(0.5f, 0.5f))
        popup_modal(POPUPS_MODAL_DELETE,
                    (text = "Delete?", closable = false,
                     flags = ImGuiWindowFlags.AlwaysAutoResize)) {
            text((text = "All those beautiful files will be deleted.\n" +
                         "This operation cannot be undone!"))
            separator(POPUPS_MODAL_DELETE_SEP)
            with_style((ImGuiStyleVar.FramePadding, float2(0.0f, 0.0f))) {
                checkbox(POPUPS_MODAL_DONT_ASK, (text = "Don't ask me next time"))
            }
            if (button(POPUPS_MODAL_DELETE_OK,
                       (text = "OK", size = float2(120.0f, 0.0f)))) {
                close_current_popup(POPUPS_MODAL_DELETE)
            }
            SetItemDefaultFocus()
            same_line(POPUPS_MODAL_DELETE_SL)
            if (button(POPUPS_MODAL_DELETE_CANCEL,
                       (text = "Cancel", size = float2(120.0f, 0.0f)))) {
                close_current_popup(POPUPS_MODAL_DELETE)
            }
        }

        // ----- "Stacked 1" / "Stacked 2" modals -----
        if (button(POPUPS_MODAL_STACK1_BTN, (text = "Stacked modals.."))) {
            open_popup(POPUPS_MODAL_STACK1)
        }
        popup_modal(POPUPS_MODAL_STACK1,
                    (text = "Stacked 1", closable = false,
                     flags = ImGuiWindowFlags.MenuBar)) {
            menu_bar(POPUPS_MODAL_STACK1_BAR) {
                menu(POPUPS_MODAL_STACK1_FILE, (text = "File", enabled = true)) {
                    menu_label(POPUPS_MODAL_STACK1_FILE_ITEM,
                               (text = "Some menu item", shortcut = "",
                                selected = false, enabled = true))
                }
            }
            text((text = "Hello from Stacked The First\n" +
                         "Using style.Colors[ImGuiCol_ModalWindowDimBg] behind it."))

            // Raw combo + ColorEdit4: ordinary widgets inside a modal can still spawn their own regular popups.
            combo(POPUPS_MODAL_STACK1_COMBO,
                  (text = "Combo", init = 1,
                   items <- POPUPS_STACKED_COMBO_ITEMS))
            color_edit4(POPUPS_MODAL_STACK1_COLOR,
                        (text = "color",
                         init = float4(0.4f, 0.7f, 0.0f, 0.5f)))

            if (button(POPUPS_MODAL_STACK2_BTN, (text = "Add another modal.."))) {
                open_popup(POPUPS_MODAL_STACK2)
            }
            // Stacked 2: closable=true demonstrates ImGui's X-button (v2 wires closable to state.open; C++ passes &unused_open through BeginPopupModal).
            popup_modal(POPUPS_MODAL_STACK2,
                        (text = "Stacked 2", closable = true,
                         flags = ImGuiWindowFlags.None)) {
                text("Hello from Stacked The Second!")
                if (button(POPUPS_MODAL_STACK2_CLOSE, (text = "Close"))) {
                    close_current_popup(POPUPS_MODAL_STACK2)
                }
            }

            if (button(POPUPS_MODAL_STACK1_CLOSE, (text = "Close"))) {
                close_current_popup(POPUPS_MODAL_STACK1)
            }
        }
    }
}


// =============================================================================
// Menus inside a regular window — cpp:3975-3989
// =============================================================================
def private MenusInRegularWindowSection() {
    tree_node(POPUPS_MENUS_IN_WIN_SUB,
        (text = "Menus inside a regular window", flags = ImGuiTreeNodeFlags.None)) {

        text_wrapped("Below we are testing adding menu items to a regular window. It's rather unusual but should work!")
        separator(POPUPS_MENUS_IN_WIN_SEP)

        menu_label(POPUPS_MENUS_IN_WIN_ITEM,
                   (text = "Menu item", shortcut = "CTRL+M",
                    selected = false, enabled = true))
        menu(POPUPS_MENUS_IN_WIN_MENU,
             (text = "Menu inside a regular window", enabled = true)) {
            ShowExampleMenuFile()
        }
    }
}
