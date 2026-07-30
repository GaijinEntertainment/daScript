options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: internal_item_flag — exercises the imgui_internal.h item-flag stack
//          (PushItemFlag/PopItemFlag, family #3) through the with_item_flag scope
//          guard. ImGuiItemFlags is the FIRST internal enum pulled into the v2
//          binding (bind_imgui.das internal_allow_enum). with_item_flag covers the
//          flags WITHOUT a dedicated public guard — Disabled / ButtonRepeat /
//          NoTabStop already have with_disabled / with_button_repeat / with_tab_stop.
//          This is the "binding is usable" gate for family #3 — it calls the real
//          bound API at runtime, not just compiles it.
// SHOWS:   MixedValue (checkbox drawn as an indeterminate dash), ReadOnly | NoNav
//          (slider ignores drags + is skipped by nav — two flags combined with the
//          | operator from imgui_enums, since ImGuiItemFlags is an internal enum the
//          binder gave no flag-ops), AllowOverlap (a wide button yields hover to a
//          small button placed on top of it).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_item_flag.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_item_flag.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_item_flag.das
// =============================================================================

[export]
def init() {
    harness_init("internal_item_flag - imgui_internal.h PushItemFlag/PopItemFlag family", 720, 460)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 420.0), ImGuiCond.Always)
    window(ITEM_FLAG_WIN, (text = "internal_item_flag", closable = false,
                           flags = ImGuiWindowFlags.None)) {
        text("with_item_flag wraps PushItemFlag/PopItemFlag for internal item flags")
        text("that have no dedicated public guard.")
        separator()

        // MixedValue — the checkbox draws as an indeterminate dash regardless of
        // its real checked state (the classic tri-state look).
        text("MixedValue:")
        checkbox(CB_NORMAL, (text = "plain checkbox"))
        with_item_flag(ImGuiItemFlags.None | ImGuiItemFlagsPrivate.MixedValue, true) {
            checkbox(CB_MIXED, (text = "same checkbox, drawn as a dash"))
        }
        separator()

        // ReadOnly | NoNav — combined into one push via the cross-enum | operator
        // (imgui_enums): ReadOnly (private ImGuiItemFlagsPrivate) freezes the value
        // on drag, NoNav (public) skips the slider in keyboard/gamepad nav.
        text("ReadOnly | NoNav (flags combined with the | operator):")
        slider_float(SL_RW, (text = "editable"))
        with_item_flag(ImGuiItemFlagsPrivate.ReadOnly | ImGuiItemFlags.NoNav, true) {
            slider_float(SL_RO, (text = "read-only (drag does nothing), skipped by nav"))
        }
        separator()

        // AllowOverlap — the wide button yields hover to a later widget drawn over
        // it; without the flag the wide button would swallow the overlap.
        text("AllowOverlap: the small button sits on top of the wide one.")
        let p = GetCursorScreenPos()
        with_item_flag(ImGuiItemFlags.None | ImGuiItemFlagsPrivate.AllowOverlap, true) {
            button(OVL_UNDER, (text = "wide button underneath", size = float2(300.0, 36.0)))
        }
        SetCursorScreenPos(p)
        small_button(OVL_OVER, (text = "on top"))
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
