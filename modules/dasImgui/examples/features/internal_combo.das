options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: internal_combo — the imgui_internal.h combo-dropdown primitives below
//          the public combo widget. Cherry-picked into the v2 binding
//          (bind_imgui.das internal_allow_func): BeginComboPopup (the low-level
//          dropdown the public combo is built on), BeginComboPreview /
//          EndComboPreview (custom-preview rendering for a CustomPreview combo). All
//          public types. Surface = three scope guards in imgui_scope_builtin.das:
//          with_combo_popup, with_custom_preview_combo (the enabling rail over the
//          public BeginCombo+CustomPreview), and with_combo_preview. This is the
//          "binding is usable" gate: it drives the real bound API at runtime.
// SHOWS:   a fully hand-rolled combo (a box button + OpenPopupEx + with_combo_popup
//          over the box's rect, items as selectables); and a custom-preview combo
//          whose CLOSED-state preview is custom-drawn (a colour swatch + name) via
//          with_custom_preview_combo + with_combo_preview.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_combo.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_combo.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_combo.das
// =============================================================================

var g_fruit = 0
var g_color = 0

def private fruit_name(i : int) : string {
    return i == 0 ? "Apple" : (i == 1 ? "Banana" : "Cherry")
}

def private color_name(i : int) : string {
    return i == 0 ? "Red" : (i == 1 ? "Green" : "Blue")
}

def private color_swatch(i : int) : uint {
    return i == 0 ? rgba(220u, 70u, 70u, 255u) : (i == 1 ? rgba(70u, 200u, 90u, 255u) : rgba(80u, 120u, 230u, 255u))
}

[export]
def init() {
    harness_init("internal_combo - BeginComboPopup / custom-preview combo", 760, 520)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 480.0), ImGuiCond.Always)
    window(CMB_WIN, (text = "internal_combo", closable = false,
                     flags = ImGuiWindowFlags.None)) {

        // ----- 1. low-level custom combo: box from ItemAdd + ButtonBehavior,
        //          dropdown from BeginComboPopup — exactly how the public combo is
        //          built. The box id is stable, so selection changes never shift it. -----
        text("BeginComboPopup(popup_id, bb, flags): the low-level dropdown, built by hand")
        text("  on ItemAdd + ButtonBehavior + the popup, exactly like the public combo.")
        let box_id = GetID("ll_combo")
        let popup_id = GetID("ll_combo_popup")
        let bp = GetCursorScreenPos()
        let box_bb = float4(bp.x, bp.y, bp.x + 240.0, bp.y + 32.0)
        ItemSize(box_bb)
        if (ItemAdd(box_bb, box_id)) {
            var hovered = false
            var held = false
            let pressed = ButtonBehavior(box_bb, box_id, hovered, held, ImGuiButtonFlags.None)
            // open only on a fresh press while closed (the public combo's own guard)
            if (pressed && !IsPopupOpen(popup_id, ImGuiPopupFlags.None)) {
                OpenPopupEx(popup_id, ImGuiPopupFlags.None)
            }
            let frame = held ? rgba(40u, 60u, 100u, 255u) : (hovered ? rgba(74u, 104u, 154u, 255u) : rgba(55u, 80u, 120u, 255u))
            with_window_drawlist() $(var dl) {
                dl |> add_rect_filled(ImVec2(box_bb.x, box_bb.y), ImVec2(box_bb.z, box_bb.w), frame, 3.0, ImDrawFlags.None)
                dl |> add_text(ImVec2(box_bb.x + 8.0, box_bb.y + 7.0), rgba(235u, 240u, 250u, 255u), "Fruit: {fruit_name(g_fruit)}   v")
            }
            with_combo_popup(popup_id, box_bb, ImGuiComboFlags.None) {
                if (selectable_label(LL_A, "Apple", g_fruit == 0)) {
                    g_fruit = 0
                }
                if (selectable_label(LL_B, "Banana", g_fruit == 1)) {
                    g_fruit = 1
                }
                if (selectable_label(LL_C, "Cherry", g_fruit == 2)) {
                    g_fruit = 2
                }
            }
        }
        separator()

        // ----- 2. custom-preview combo: the closed preview is custom-drawn -----
        text("Custom-preview combo: BeginCombo(CustomPreview) + BeginComboPreview —")
        text("  the closed-state preview is a hand-drawn colour swatch + name.")
        let cp_open = with_custom_preview_combo("##color_combo", ImGuiComboFlags.None) {
            if (selectable_label(CP_R, "Red", g_color == 0)) {
                g_color = 0
            }
            if (selectable_label(CP_G, "Green", g_color == 1)) {
                g_color = 1
            }
            if (selectable_label(CP_B, "Blue", g_color == 2)) {
                g_color = 2
            }
        }
        // the custom preview (drawn into the combo's preview box regardless of open state)
        with_combo_preview() {
            let p = GetCursorScreenPos()
            with_window_drawlist() $(var dl) {
                dl |> add_rect_filled(ImVec2(p.x, p.y), ImVec2(p.x + 16.0, p.y + 16.0),
                                      color_swatch(g_color), 2.0, ImDrawFlags.None)
            }
            dummy(SW, ImVec2(20.0, 16.0))
            same_line()
            text("{color_name(g_color)}")
        }
        text("selected colour = {color_name(g_color)}   (dropdown open = {cp_open})")
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
