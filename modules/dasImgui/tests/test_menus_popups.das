options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Menus, popups and tooltips: registration, open, pick.

let MENU_MAIN_FEATURE = "modules/dasImgui/examples/features/menu_main.das"

[test(description = "The main_menu_bar container registers while its menus are closed.", timeout = 1200)]
def test_menu_main_smoke(t : T?) {
    with_imgui_app(MENU_MAIN_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_MENU_BAR", 15.0f)
        t |> success(snap != null, "MAIN_MENU_BAR renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_MENU_BAR"),
                     "main_menu_bar container registered")
        // Menus + items only emit entries while the menu is open (BeginMenu returns true), so
        // container existence alone is the headless-drive smoke gate; opening + clicking is
        // exercised by record_menu_main.das.
    }
}

let MENU_LABEL_FEATURE = "modules/dasImgui/examples/features/menu_label_static.das"

[test(description = "The menu_label demo's popup anchor and the status text below it render.", timeout = 1200)]
def test_menu_label_static_smoke(t : T?) {
    with_imgui_app(MENU_LABEL_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/ANCHOR", 15.0f)
        t |> success(snap != null, "ANCHOR button renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/STATUS"),
                     "STATUS text renders below popup anchor")
    }
}

let POPUP_CTX_FEATURE = "modules/dasImgui/examples/features/popup_context_item.das"

[test(description = "A closed popup_context_item registers its container under the window path beside its target button.", timeout = 1200)]
def test_popup_context_item_smoke(t : T?) {
    with_imgui_app(POPUP_CTX_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/TARGET_BTN", 15.0f)
        t |> success(snap0 != null, "TARGET_BTN registers")
        if (snap0 == null) return

        // popup_context_item itself registers at MAIN_WIN/TARGET_CTX even
        // when closed — the [container] wrapper calls stateless_finalize
        // every frame regardless of Begin's return.
        let ctx_entry = find_widget(snap0, "MAIN_WIN/TARGET_CTX")
        t |> equal(ctx_entry?["kind"] ?? "", "popup_context_item",
                   "TARGET_CTX.kind == 'popup_context_item'")
    }
}

let POPUP_CTX_WINDOW_FEATURE = "modules/dasImgui/examples/features/popup_context_window.das"

//! Re-opened items persist in the registry: gate on wait_for_visible
[test(description = "A right-click opens popup_context_window, a click on its menu item records the pick, and re-opening it lets a second item be picked.", timeout = 1200)]
def test_popup_context_window_smoke(t : T?) {
    with_imgui_app(POPUP_CTX_WINDOW_FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/CTX", 15.0f)
        t |> success(snap != null, "CTX container registers")
        if (snap == null) return

        let ctx = find_widget(snap, "MAIN_WIN/CTX")
        t |> equal(ctx?["kind"] ?? "", "popup_context_window",
                   "CTX.kind == 'popup_context_window'")
        t |> equal(widget_payload_field(snap, "MAIN_WIN/STATUS", "value") ?? "",
                   "last pick: (none yet)",
                   "STATUS shows '(none yet)' initially")
        t |> success(!widget_exists(snap, "MAIN_WIN/CTX/ITEM_NEW"),
                     "ITEM_NEW absent while popup closed")

        // Right-click LEAD to open the context popup. Gate on wait_for_visible (rendered THIS frame),
        // not wait_for_widget (exists-only) — see the re-open note below.
        right_click(d, "MAIN_WIN/LEAD")
        let snap_open = wait_for_visible(d, "MAIN_WIN/CTX/ITEM_NEW", 5.0f)
        t |> success(snap_open != null, "ITEM_NEW visible after right-click on LEAD")
        if (snap_open == null) return

        // Pick "New" — a real synthetic click fires the demo's `if (menu_item(...))` branch.
        click(d, "MAIN_WIN/CTX/ITEM_NEW")
        t |> success(wait_for_string_value(d, "MAIN_WIN/STATUS", "value",
                                           "last pick: New", 300),
                     "STATUS reflects 'New' after menu pick")

        // Re-open + pick "Quit" — the case that used to race. ITEM_QUIT's registry entry persists
        // from the first open, so wait_for_widget (exists-only) returns stale while the popup is
        // closed and the click drops (no bbox). wait_for_visible gates on rendered-this-frame instead.
        right_click(d, "MAIN_WIN/LEAD")
        let snap_open2 = wait_for_visible(d, "MAIN_WIN/CTX/ITEM_QUIT", 5.0f)
        t |> success(snap_open2 != null, "ITEM_QUIT visible on re-open")
        if (snap_open2 == null) return
        click(d, "MAIN_WIN/CTX/ITEM_QUIT")
        let got_quit = wait_for_string_value(d, "MAIN_WIN/STATUS", "value",
                                             "last pick: Quit", 300)
        if (!got_quit) {
            // DIAGNOSTIC: on failure surface what the click did — STATUS shows the last pick that
            // registered (still 'New' = the click never landed), and whether the items still exist.
            var diag = snapshot(d)
            let stat   = widget_payload_field(diag, "MAIN_WIN/STATUS", "value") ?? "<none>"
            let iq_ex  = widget_exists(diag, "MAIN_WIN/CTX/ITEM_QUIT")
            let in_ex  = widget_exists(diag, "MAIN_WIN/CTX/ITEM_NEW")
            to_log(LOG_WARNING, "[popup-diag] second pick FAILED: STATUS='{stat}' ITEM_QUIT_exists={iq_ex} ITEM_NEW_exists={in_ex}\n")
        }
        t |> success(got_quit, "STATUS reflects 'Quit' after second menu pick")
    }
}

let POPUP_WINDOW_FEATURE = "modules/dasImgui/examples/features/popup_window.das"

[test(description = "Both popup_window instances sharing a str_id register every frame, and clicking the trigger button paints the popup's body at its scoped path.", timeout = 1200)]
def test_popup_window(t : T?) {
    with_imgui_app(POPUP_WINDOW_FEATURE) $(d) {
        t |> run("smoke") <| @(t : T?) {
            var snap0 = wait_for_widget(d, "MAIN_WIN/via_button/OPEN_BTN", 15.0f)
            t |> success(snap0 != null, "OPEN_BTN registers under with_id(via_button) scope")
            if (snap0 == null) return

            // Both popup_window instances register every frame via stateless_finalize,
            // regardless of open/closed.
            let btn_entry = find_widget(snap0, "MAIN_WIN/via_button/BTN_POPUP")
            t |> equal(btn_entry?["kind"] ?? "", "popup_window",
                       "BTN_POPUP.kind == 'popup_window'")
            let reg_entry = find_widget(snap0, "MAIN_WIN/via_region/REG_POPUP")
            t |> equal(reg_entry?["kind"] ?? "", "popup_window",
                       "REG_POPUP.kind == 'popup_window'")
        }
        reset(d)

        t |> run("button_opens") <| @(t : T?) {
            var snap0 = wait_for_widget(d, "MAIN_WIN/via_button/OPEN_BTN", 15.0f)
            t |> success(snap0 != null, "feature ready")
            if (snap0 == null) return

            // Click OPEN_BTN. The button handler calls open_popup("fruit_picker");
            // popup_window brackets the body on the next frame, so PICK_APPLE
            // appears in the snapshot at the scoped path.
            click(d, "MAIN_WIN/via_button/OPEN_BTN")
            // Popups are re-showable, so gate on actual paint — wait_for_visible is
            // exists AND rendered-this-frame. (widget_rendered now requires existence,
            // so it no longer false-passes for an absent path.)
            var snap_open = wait_for_visible(d, "MAIN_WIN/via_button/BTN_POPUP/PICK_APPLE", 5.0f)
            t |> success(snap_open != null,
                         "click OPEN_BTN opens BTN_POPUP body (PICK_APPLE visible)")
        }
    }
}

let OPEN_POPUP_FEATURE = "modules/dasImgui/examples/features/open_popup_on_item_click.das"

[test(description = "The popup bound by open_popup_on_item_click registers, opening it registers its body, and a click on its OK button updates the status.", timeout = 1200)]
def test_open_popup_on_item_click_smoke(t : T?) {
    with_imgui_app(OPEN_POPUP_FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/BTN_TARGET", 15.0f)
        t |> success(snap != null, "BTN_TARGET registers")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/MENU"),
                     "MENU popup container registers")
        t |> equal(find_widget(snap, "MAIN_WIN/MENU")?["kind"] ?? "", "popup",
                   "MENU.kind == popup")
        t |> equal(widget_payload_field(snap, "MAIN_WIN/STATUS", "value") ?? "",
                   "picks = 0", "initial STATUS = 'picks = 0'")

        // L2 open the popup directly (mirrors the trigger that the
        // open_popup_on_item_click sets when fired). Popup body registers.
        open_widget(d, "MAIN_WIN/MENU")
        var open_snap = wait_for_widget(d, "MAIN_WIN/MENU/BTN_OK", 5.0f)
        t |> success(open_snap != null, "BTN_OK registers once popup is open")
        if (open_snap == null) return

        t |> success(widget_exists(open_snap, "MAIN_WIN/MENU/POPUP_LEAD"),
                     "POPUP_LEAD registers inside popup")

        // Click OK; g_picks++ -> STATUS updates.
        click(d, "MAIN_WIN/MENU/BTN_OK")
        t |> success(wait_for_string_value(d, "MAIN_WIN/STATUS", "value",
                                           "picks = 1", 300),
                     "STATUS reflects 'picks = 1' after BTN_OK click")
    }
}

let TOOLTIP_FEATURE = "modules/dasImgui/examples/features/tooltip_flat.das"

[test(description = "A hover-gated item tooltip is registered from module load but reports not rendered while nothing hovers its button.", timeout = 1200)]
def test_tooltip_flat_smoke(t : T?) {
    with_imgui_app(TOOLTIP_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/LEAD", 15.0f)
        t |> success(snap != null, "LEAD renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/HOVER_BTN"),
                     "hover button HOVER_BTN registered")
        t |> success(widget_exists(snap, "MAIN_WIN/ALWAYS_BTN"),
                     "always button ALWAYS_BTN registered")
        // The set_item_tooltip carries its NarrativeState payload to the snapshot even
        // unhovered (registered at module load), so it's always enumerable.
        t |> success(widget_exists(snap, "TOOLTIP_ON_BUTTON"),
                     "hover-gated tooltip registered (payload visible)")
        // ...but it reports rendered=true only on frames it actually shows: with no cursor over
        // HOVER_BTN its finalize is skipped, so the rendered-this-frame key is absent. Guards the
        // finalize-only-when-shown fix (unconditional finalize would surface it as rendered here).
        t |> success(!widget_rendered(snap, "MAIN_WIN/TOOLTIP_ON_BUTTON"),
                     "hover-gated tooltip not rendered while unhovered")
    }
}
