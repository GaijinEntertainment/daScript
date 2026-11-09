options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! edit_* containers and sliders bound to caller-owned values.

let EDIT_HEADER_FEATURE = "modules/dasImgui/examples/features/edit_collapsing_header.das"

[test(description = "edit_collapsing_header echoes its bound bool, a force_set false writes through the pointer, and a button setting the bool back re-opens the header.", timeout = 1200)]
def test_edit_collapsing_header_smoke(t : T?) {
    with_imgui_app(EDIT_HEADER_FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/HDR_A", 15.0f)
        t |> success(snap != null, "HDR_A registers")
        if (snap == null) return

        let hdr_a = find_widget(snap, "MAIN_WIN/HDR_A")
        t |> equal(hdr_a?["kind"] ?? "", "edit_collapsing_header",
                   "HDR_A.kind == 'edit_collapsing_header'")
        t |> success(widget_exists(snap, "MAIN_WIN/HDR_B"), "HDR_B registers")

        // Initial state — both VISIBLE_A and VISIBLE_B start true; the bool
        // payload echoes *ptr on every frame.
        t |> equal(widget_payload_field(snap, "MAIN_WIN/HDR_A", "value") ?? false,
                   true,
                   "HDR_A.value == true initially (VISIBLE_A = true)")
        t |> equal(widget_payload_field(snap, "MAIN_WIN/HDR_B", "value") ?? false,
                   true,
                   "HDR_B.value == true initially (VISIBLE_B = true)")

        // Surrounding siblings + sentinel STATUS line render.
        for (ident in ["LEAD", "SEP", "RESTORE_A", "RESTORE_B", "STATUS"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registers")
        }

        // Bidir-bind A→false via imgui_force_set; scalar-pointee dispatcher writes
        // through *ptr (VISIBLE_A := false), header hides next frame.
        force_set_value(d, "MAIN_WIN/HDR_A", JV(false))
        t |> success(wait_for_bool_value(d, "MAIN_WIN/HDR_A", "value", false, 300),
                     "HDR_A.value flips to false after force_set_value")

        // RESTORE_A button writes VISIBLE_A := true; reverse-direction proof.
        click(d, "MAIN_WIN/RESTORE_A")
        t |> success(wait_for_bool_value(d, "MAIN_WIN/HDR_A", "value", true, 300),
                     "HDR_A.value back to true after RESTORE_A click")
    }
}

let EDIT_TAB_FEATURE = "modules/dasImgui/examples/features/edit_tab_item.das"

[test(description = "edit_tab_item binds a closable tab to a module bool both ways: force_set on the tab closes it, and force_set on the linked checkbox restores it, each visible in the status line.", timeout = 1200)]
def test_edit_tab_item_smoke(t : T?) {
    with_imgui_app(EDIT_TAB_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/BAR/TAB_A", 15.0f)
        t |> success(snap != null, "TAB_A registers")
        if (snap == null) return

        t |> equal(find_widget(snap, "MAIN_WIN/BAR/TAB_A")?["kind"] ?? "", "edit_tab_item",
                   "TAB_A.kind == edit_tab_item")
        for (ident in ["LEAD", "CHECK_A", "CHECK_B", "STATUS"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "MAIN_WIN/{ident} registers")
        }
        t |> success(widget_exists(snap, "MAIN_WIN/BAR/TAB_B"), "TAB_B registers")

        // Initially both open: A is selected by default; BODY_A visible.
        t |> success(widget_exists(snap, "MAIN_WIN/BAR/TAB_A/BODY_A"),
                     "BODY_A visible when alpha tab is active")
        t |> equal(widget_payload_field(snap, "MAIN_WIN/STATUS", "value") ?? "",
                   "a=true b=true",
                   "STATUS == 'a=true b=true' initially")

        // Flip A closed via the external bool dispatcher; body+chrome vanish.
        force_set_value(d, "MAIN_WIN/BAR/TAB_A", JV(false))
        t |> success(wait_for_string_value(d, "MAIN_WIN/STATUS", "value",
                                           "a=false b=true", 300),
                     "STATUS reflects A closed after force_set_value(TAB_A, false)")

        // Restore A via the linked checkbox; bidir bind makes the tab reappear.
        force_set_value(d, "MAIN_WIN/CHECK_A", JV(true))
        t |> success(wait_for_string_value(d, "MAIN_WIN/STATUS", "value",
                                           "a=true b=true", 300),
                     "STATUS reflects A restored after force_set_value(CHECK_A, true)")
    }
}

let EDIT_MENU_FEATURE = "modules/dasImgui/examples/features/edit_menu_item.das"

//! menu_items register only while their menu is open
//! existence is therefore the headless gate
[test(description = "The edit_menu_item demo registers its main menu bar, both menu headers and the readout window.", timeout = 1200)]
def test_edit_menu_item_smoke(t : T?) {
    with_imgui_app(EDIT_MENU_FEATURE) $(d) {
        var snap = wait_for_render(d, "MENU_BAR", 15.0f)
        t |> success(snap != null, "MENU_BAR renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MENU_BAR"),
                     "main_menu_bar container registered")
        t |> success(widget_exists(snap, "MENU_BAR/FILE_MENU"),
                     "File menu header registered")
        t |> success(widget_exists(snap, "MENU_BAR/VIEW_MENU"),
                     "View menu header registered")
        t |> success(widget_exists(snap, "BODY"),
                     "BODY readout window registered")
    }
}

let EDIT_VSLIDERS_FEATURE = "modules/dasImgui/examples/features/edit_vsliders.das"

[test(description = "All five edit_vslider float and int widgets register, and force_set round-trips through a float and an int pointee.", timeout = 1200)]
def test_edit_vsliders_smoke(t : T?) {
    with_imgui_app(EDIT_VSLIDERS_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/VSLIDE_F1", 15.0f)
        t |> success(snap != null, "VSLIDE_F1 renders")
        if (snap == null) return

        for (ident in ["VSLIDE_F1", "VSLIDE_F2", "VSLIDE_F3",
                       "VSLIDE_I1", "VSLIDE_I2"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registered")
        }

        // imgui_force_set on edit_vslider_float — float pointee round-trips.
        force_set_value(d, "MAIN_WIN/VSLIDE_F1", JV(0.875f))
        let after_f = wait_until(d, 300) $(var s) {
            let v = double(widget_payload_field(s, "MAIN_WIN/VSLIDE_F1", "value") ?? 0.0)
            return v > 0.87lf && v < 0.88lf
        }
        t |> success(after_f != null, "edit_vslider_float write 0.875 round-trips")

        // imgui_force_set on edit_vslider_int — int pointee round-trips.
        force_set_value(d, "MAIN_WIN/VSLIDE_I2", JV(88))
        t |> success(wait_for_int_value(d, "MAIN_WIN/VSLIDE_I2", "value", 88, 300),
                     "edit_vslider_int write 88 round-trips")
    }
}
