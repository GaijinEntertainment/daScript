options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: containers_overlay — Phase 0b.3 popup / popup_modal / tooltip /
//          item_tooltip / combo_select / list_box containers.
// SHOWS:   imgui_open / imgui_close on popups, hover-driven tooltips, block-
//          arg combo + list_box variants for procedural-item lists.
// DRIVE:   curl -X POST -d '{"name":"imgui_open","args":{"target":"OVERLAY_WIN/OPTIONS_POPUP"}}' \
//                       localhost:9090/command
//          curl -X POST -d '{"name":"imgui_close","args":{"target":"OVERLAY_WIN/OPTIONS_POPUP"}}' \
//                       localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set",
//                            "args":{"target":"OVERLAY_WIN/PALETTE_COMBO","value":2}}' \
//                       localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/containers_overlay.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/containers_overlay.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/containers_overlay.das
// =============================================================================

let PALETTE_NAMES : array<string> <- ["Sunset", "Ocean", "Forest", "Mono"]
let TASK_NAMES : array<string> <- ["Compile", "Lint", "Test", "Package", "Deploy"]
var PALETTE_ROW : table<int; ClickState>
var TASK_ROW : table<int; ClickState>

[export]
def init() {
    harness_init("Phase 0b.3 — containers (overlay family)", 800, 640)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(720.0f, 580.0f), ImGuiCond.FirstUseEver)
    window(OVERLAY_WIN, (text = "Overlay demo", closable = false,
                         flags = ImGuiWindowFlags.None)) {
        text("popups, tooltips, and block-arg combos")
        separator()

        // ----- Popup driven by a button -----
        text("popup family")
        if (button(OPEN_POPUP_BTN, (text = "Open options popup"))) {
            OPTIONS_POPUP.pending_open = true
        }
        same_line((spacing = 8.0f))
        if (button(OPEN_MODAL_BTN, (text = "Open modal"))) {
            CONFIRM_MODAL.pending_open = true
        }

        popup(OPTIONS_POPUP, (text = "options_popup",
                              flags = ImGuiWindowFlags.None)) {
            text("Options")
            separator()
            checkbox(OPT_VSYNC, (text = "VSync"))
            checkbox(OPT_FULLSCREEN, (text = "Fullscreen"))
            if (button(POPUP_CLOSE_BTN, (text = "Close"))) {
                OPTIONS_POPUP.pending_close = true
            }
        }

        popup_modal(CONFIRM_MODAL, (text = "Confirm", closable = false,
                                    flags = ImGuiWindowFlags.AlwaysAutoResize)) {
            text("Are you sure?")
            separator()
            if (button(CONFIRM_YES, (text = "Yes"))) {
                CONFIRM_MODAL.pending_close = true
            }
            same_line((spacing = 8.0f))
            if (button(CONFIRM_NO, (text = "No"))) {
                CONFIRM_MODAL.pending_close = true
            }
        }

        spacing()

        // ----- Tooltips -----
        text("tooltip family")
        separator()
        button(HOVER_BTN, (text = "Hover me"))
        item_tooltip(HOVER_TIP) {
            text("ImGui's BeginItemTooltip auto-gates on IsItemHovered()")
            text("(no manual hover check needed)")
        }
        same_line((spacing = 8.0f))
        button(MANUAL_TIP_BTN, (text = "Hover for manual tooltip"))
        if (IsItemHovered(ImGuiHoveredFlags.None)) {
            tooltip(MANUAL_TIP) {
                text("BeginTooltip — caller-gated")
            }
        }

        spacing()

        // ----- Block-arg combo + list_box -----
        text("combo_select / list_box — body emits Selectables")
        separator()
        var palette_label = "(none)"
        if (PALETTE_COMBO.value >= 0 && PALETTE_COMBO.value < length(PALETTE_NAMES)) {
            palette_label = PALETTE_NAMES[PALETTE_COMBO.value]
        }
        combo_select(PALETTE_COMBO, (text = "palette",
                                     preview_value = palette_label,
                                     flags = ImGuiComboFlags.None)) {
            for (i in range(length(PALETTE_NAMES))) {
                let is_sel = (i == PALETTE_COMBO.value)
                if (selectable_label(PALETTE_ROW[i], PALETTE_NAMES[i], is_sel)) {
                    PALETTE_COMBO.value = i
                }
            }
        }
        text("palette = {palette_label} (idx {PALETTE_COMBO.value})")

        list_box(TASK_LIST, (text = "tasks",
                             size = float2(220.0f, 110.0f))) {
            for (i in range(length(TASK_NAMES))) {
                let is_sel = (i == TASK_LIST.value)
                if (selectable_label(TASK_ROW[i], TASK_NAMES[i], is_sel)) {
                    TASK_LIST.value = i
                }
            }
        }
        var task_label = "(none)"
        if (TASK_LIST.value >= 0 && TASK_LIST.value < length(TASK_NAMES)) {
            task_label = TASK_NAMES[TASK_LIST.value]
        }
        text("task = {task_label} (idx {TASK_LIST.value})")
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    //! Standalone entrypoint. daslang-live drives init/update/shutdown
    //! directly and ignores main(); plain `daslang.exe <file>` runs this loop.
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
