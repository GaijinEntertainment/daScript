options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for the 7 visual-aid [live_command] wrappers in
//! widgets/imgui_visual_aids.das. The dashboard demo is a passive stage
//! with named widget targets (TARGET_BTN, TARGET_CHECK, TARGET_INPUT).
//! We post each of the 7 verbs and assert ok=true on the response,
//! verifying dispatch wiring. Visual correctness is exercised by the
//! per-overlay tests (test_visual_aids_focus_rect.das,
//! test_visual_aids_narrate.das) and the record_*.das drivers.

let FEATURE = "modules/dasImgui/examples/features/visual_aids_dashboard.das"

[test]
def test_visual_aids_dashboard_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/TARGET_BTN", 15.0f)
        t |> success(snap != null, "TARGET_BTN registers")
        if (snap == null) return

        for (ident in ["LEAD", "TARGET_BTN", "TARGET_CHECK", "TARGET_INPUT", "STATUS"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "MAIN_WIN/{ident} registers")
        }

        // 1. imgui_highlight — flash colored rect on target.bbox.
        let hl = post_command(d, "imgui_highlight",
                              JV((target = "MAIN_WIN/TARGET_BTN", frames = 60)))
        t |> equal(hl?["ok"] ?? false, true, "imgui_highlight ok=true")

        // 2. imgui_mouse_trail — fading dotted line behind cursor.
        let mt = post_command(d, "imgui_mouse_trail", JV((enabled = true)))
        t |> equal(mt?["ok"] ?? false, true, "imgui_mouse_trail ok=true")

        // 3. imgui_narrate — callout box w/ optional connector to target.
        let nr = post_command(d, "imgui_narrate",
                              JV((text = "stage smoke", target = "MAIN_WIN/TARGET_BTN",
                                  frames = 30)))
        t |> equal(nr?["ok"] ?? false, true, "imgui_narrate ok=true")

        // 4. imgui_auto_highlight — flag toggle for auto-flash on every command.
        let ah = post_command(d, "imgui_auto_highlight", JV((enabled = true)))
        t |> equal(ah?["ok"] ?? false, true, "imgui_auto_highlight ok=true")

        // 5. imgui_cursor_sprite — synth-cursor pointer.
        let cs = post_command(d, "imgui_cursor_sprite", JV((enabled = true)))
        t |> equal(cs?["ok"] ?? false, true, "imgui_cursor_sprite ok=true")

        // 6. imgui_key_hud — bottom-center keycap + modifier strip.
        let kh = post_command(d, "imgui_key_hud",
                              JV((enabled = true, show_modifiers = true)))
        t |> equal(kh?["ok"] ?? false, true, "imgui_key_hud ok=true")

        // 7. imgui_focus_rect — rect around io.active_widget.
        let fr = post_command(d, "imgui_focus_rect", JV((enabled = true)))
        t |> equal(fr?["ok"] ?? false, true, "imgui_focus_rect ok=true")

        // Symmetric disable on the four toggle-style commands.
        let mt_off = post_command(d, "imgui_mouse_trail", JV((enabled = false)))
        t |> equal(mt_off?["enabled"] ?? true, false, "imgui_mouse_trail off")
        let cs_off = post_command(d, "imgui_cursor_sprite", JV((enabled = false)))
        t |> equal(cs_off?["enabled"] ?? true, false, "imgui_cursor_sprite off")
        let kh_off = post_command(d, "imgui_key_hud", JV((enabled = false)))
        t |> equal(kh_off?["enabled"] ?? true, false, "imgui_key_hud off")
        let fr_off = post_command(d, "imgui_focus_rect", JV((enabled = false)))
        t |> equal(fr_off?["enabled"] ?? true, false, "imgui_focus_rect off")
        let ah_off = post_command(d, "imgui_auto_highlight", JV((enabled = false)))
        t |> equal(ah_off?["enabled"] ?? true, false, "imgui_auto_highlight off")
    }
}
