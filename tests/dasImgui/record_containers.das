options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``containers.apng`` against
//! ``examples/tutorial/containers.das``. Real-input, self-verifying tour of the
//! four container kinds in the window chrome:
//!   1. tab_bar  - flip the General tab's WIRE checkbox
//!   2. tab_item - click the Audio tab header (a real header click, now that
//!      tab_item registers its header bbox) and PROVE exclusive bodies: the
//!      Audio body's MUTE renders while the General body's WIRE stops rendering
//!   3. popup    - the Open button raises pending_open; toggle the popup's
//!      VSync checkbox inside the popup body
//!   4. popup close - the popup's own Close button raises pending_close
//!   5. item_tooltip - hover the button; the tooltip body runs while hovered

[export]
def main {
    with_recording_app("examples/tutorial/containers.das",
                       "containers.apng", 80) $(app) {
        let T_AUDIO   = "CONT_WIN/MAIN_TABS/AUDIO_TAB"
        let T_WIRE    = "CONT_WIN/MAIN_TABS/GENERAL_TAB/WIRE"
        let T_MUTE    = "CONT_WIN/MAIN_TABS/AUDIO_TAB/MUTE"
        let T_OPEN    = "CONT_WIN/OPEN_POPUP_BTN"
        let T_VSYNC   = "CONT_WIN/OPTIONS_POPUP/OPT_VSYNC"
        let T_CLOSE   = "CONT_WIN/OPTIONS_POPUP/POPUP_CLOSE_BTN"
        let T_HOVER   = "CONT_WIN/HOVER_BTN"
        let T_TIP     = "CONT_WIN/HOVER_TIP"

        var snap = wait_for_render(app, T_OPEN, 10.0f)
        if (snap == null) {
            panic("{T_OPEN} never rendered - wrong app running?")
        }
        let p_wire  = widget_click_point(snap, T_WIRE)
        let p_audio = widget_click_point(snap, T_AUDIO)
        let p_open  = widget_click_point(snap, T_OPEN)
        let p_hover = widget_click_point(snap, T_HOVER)

        //! Initial cursor placement - flips synth_cursor_owned.
        move_to(app, (30.0f, 24.0f), 600)
        hold_content(app, SETTLE_MS)

        // ---- Stage 1: tab_bar General tab + WIRE checkbox ----
        move_to(app, p_wire, 900)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "menu_bar + tab_bar share the window chrome", T_WIRE,
            [voice = "A window can hold a menu bar and a tab bar right in its frame. The General tab is open, and its wireframe checkbox sits in that tab's body. I turn it on."]),
            [T_WIRE])

        // ---- Stage 2: switch tabs -> only the open tab's body runs ----
        move_to(app, p_audio, 900)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "tab_item - only the open tab's body runs", T_AUDIO,
            [voice = "Each tab is its own block, and only the open one runs. I click the Audio tab. Its controls appear, and the General tab stops running entirely."])
        let anchor2 = record_frame_count(app)
        let lead2 = d2 > 1200u ? 600u : d2 / 3u
        hold_content(app, lead2)
        wait_for_mouse_idle(app)                             // SYNC: the approach glide finished before the click coroutine starts
        click(app, T_AUDIO)                                  // real tab-header click switches the active tab
        record_check_rendered(app, T_MUTE, true, 120)        // Audio body now runs
        record_check_rendered(app, T_WIRE, false, 120)       // General body stopped - exclusive tabs
        hold_remainder_content(app, d2, anchor2)

        // ---- Stage 3: popup opened by a pending flag + a checkbox inside it ----
        move_to(app, p_open, 900)
        hold_content(app, SETTLE_MS)
        let d3 = say_begin(app, "popup - opened by a pending flag", T_OPEN,
            [voice = "This button raises a pending flag, and a popup appears. Its checkbox lives inside the popup body. I switch it on."])
        let anchor3 = record_frame_count(app)
        let lead3 = d3 > 1200u ? 600u : d3 / 3u
        hold_content(app, lead3)
        wait_for_mouse_idle(app)
        click(app, T_OPEN)                                   // button sets OPTIONS_POPUP.pending_open
        record_check_rendered(app, T_VSYNC, true, 120)       // popup body runs next frame
        var snap_pop = wait_for_render(app, T_VSYNC, 5.0f)
        if (snap_pop == null) {
            panic("{T_VSYNC} never rendered after opening the popup")
        }
        let p_vsync = widget_click_point(snap_pop, T_VSYNC)
        let p_close = widget_click_point(snap_pop, T_CLOSE)
        move_to(app, p_vsync, 600)
        wait_for_mouse_idle(app)                             // SYNC: the glide to the checkbox must finish before the click coroutine, or the two synth cursors fight and the toggle misses
        click(app, T_VSYNC)
        record_check_value(app, T_VSYNC, "value", true, 120) // checkbox inside the popup flipped on
        hold_remainder_content(app, d3, anchor3)

        // ---- Stage 4: the popup's Close button raises pending_close ----
        move_to(app, p_close, 700)
        hold_content(app, SETTLE_MS)
        let d4 = say_begin(app, "Close button raises pending_close", T_CLOSE,
            [voice = "The popup carries its own close button, which raises the matching close flag. I click it, and the popup goes away."])
        let anchor4 = record_frame_count(app)
        let lead4 = d4 > 1200u ? 600u : d4 / 3u
        hold_content(app, lead4)
        wait_for_mouse_idle(app)                             // SYNC: the approach glide finished before the click coroutine starts
        click(app, T_CLOSE)                                  // sets OPTIONS_POPUP.pending_close
        record_check_rendered(app, T_VSYNC, false, 120)      // popup gone - its body no longer runs
        hold_remainder_content(app, d4, anchor4)

        // ---- Stage 5: item_tooltip runs only while the button is hovered ----
        move_to(app, p_hover, 900)
        hold_content(app, SETTLE_MS)
        let d5 = say_begin(app, "item_tooltip - hover-gated", T_HOVER,
            [voice = "Last, a tooltip. It shows itself automatically whenever the cursor rests on its widget. Here it is, gated on hover."])
        record_check_rendered(app, T_TIP, true, 200)         // tooltip body runs while hovered
        hold_content(app, d5)
    }
}
