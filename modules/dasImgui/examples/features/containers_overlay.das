options gen2

require imgui
require imgui_app
require glfw/glfw_boost
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live/opengl_live
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin

// =============================================================================
// FEATURE: containers_overlay — Phase 0b.3 popup / popup_modal / tooltip /
//          item_tooltip / combo_select / list_box_select containers.
// SHOWS:   imgui_open / imgui_close on popups, hover-driven tooltips, block-
//          arg combo + list_box variants for procedural-item lists.
// DRIVE:   curl -X POST -d '{"name":"imgui_open","args":{"target":"OVERLAY_WIN/OPTIONS_POPUP"}}' \
//                       localhost:9090/command
//          curl -X POST -d '{"name":"imgui_close","args":{"target":"OVERLAY_WIN/OPTIONS_POPUP"}}' \
//                       localhost:9090/command
//          curl -X POST -d '{"name":"imgui_set",
//                            "args":{"target":"OVERLAY_WIN/PALETTE_COMBO","value":2}}' \
//                       localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/containers_overlay.das
// LIVE:       daslang-live modules/dasImgui/examples/features/containers_overlay.das
// =============================================================================

let PALETTE_NAMES : array<string> <- ["Sunset", "Ocean", "Forest", "Mono"]
let TASK_NAMES : array<string> <- ["Compile", "Lint", "Test", "Package", "Deploy"]

[export]
def init() {
    live_create_window("Phase 0b.3 — containers (overlay family)", 800, 640)
    live_imgui_init(live_window)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 1.5
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplOpenGL3_NewFrame()
    ImGui_ImplGlfw_NewFrame()
    NewFrame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(720.0f, 580.0f), ImGuiCond.FirstUseEver)
    window(OVERLAY_WIN, (text = "Overlay demo", closable = false,
                         flags = ImGuiWindowFlags.None)) {
        Text("popups, tooltips, and block-arg combos")
        Separator()

        // ----- Popup driven by a button -----
        Text("popup family")
        if (button(OPEN_POPUP_BTN, (text = "Open options popup"))) {
            OPTIONS_POPUP.pending_open = true
        }
        SameLine(0.0f, 8.0f)
        if (button(OPEN_MODAL_BTN, (text = "Open modal"))) {
            CONFIRM_MODAL.pending_open = true
        }

        popup(OPTIONS_POPUP, (text = "options_popup",
                              flags = ImGuiWindowFlags.None)) {
            Text("Options")
            Separator()
            checkbox(OPT_VSYNC, (text = "VSync"))
            checkbox(OPT_FULLSCREEN, (text = "Fullscreen"))
            if (button(POPUP_CLOSE_BTN, (text = "Close"))) {
                OPTIONS_POPUP.pending_close = true
            }
        }

        popup_modal(CONFIRM_MODAL, (text = "Confirm", closable = false,
                                    flags = ImGuiWindowFlags.AlwaysAutoResize)) {
            Text("Are you sure?")
            Separator()
            if (button(CONFIRM_YES, (text = "Yes"))) {
                CONFIRM_MODAL.pending_close = true
            }
            SameLine(0.0f, 8.0f)
            if (button(CONFIRM_NO, (text = "No"))) {
                CONFIRM_MODAL.pending_close = true
            }
        }

        Spacing()

        // ----- Tooltips -----
        Text("tooltip family")
        Separator()
        button(HOVER_BTN, (text = "Hover me"))
        item_tooltip(HOVER_TIP) {
            Text("ImGui's BeginItemTooltip auto-gates on IsItemHovered()")
            Text("(no manual hover check needed)")
        }
        SameLine(0.0f, 8.0f)
        button(MANUAL_TIP_BTN, (text = "Hover for manual tooltip"))
        if (IsItemHovered(ImGuiHoveredFlags.None)) {
            tooltip(MANUAL_TIP) {
                Text("BeginTooltip — caller-gated")
            }
        }

        Spacing()

        // ----- Block-arg combo + list_box -----
        Text("combo_select / list_box_select — body emits Selectables")
        Separator()
        var palette_label = "(none)"
        if (PALETTE_COMBO.value >= 0 && PALETTE_COMBO.value < length(PALETTE_NAMES)) {
            palette_label = PALETTE_NAMES[PALETTE_COMBO.value]
        }
        combo_select(PALETTE_COMBO, (text = "palette",
                                     preview_value = palette_label,
                                     flags = ImGuiComboFlags.None)) {
            for (i in range(length(PALETTE_NAMES))) {
                let is_sel = (i == PALETTE_COMBO.value)
                if (Selectable(PALETTE_NAMES[i], is_sel,
                               ImGuiSelectableFlags.None,
                               float2(0.0f, 0.0f))) {
                    PALETTE_COMBO.value = i
                }
            }
        }
        Text("palette = {palette_label} (idx {PALETTE_COMBO.value})")

        list_box_select(TASK_LIST, (text = "tasks",
                                    size = float2(220.0f, 110.0f))) {
            for (i in range(length(TASK_NAMES))) {
                let is_sel = (i == TASK_LIST.value)
                if (Selectable(TASK_NAMES[i], is_sel,
                               ImGuiSelectableFlags.None,
                               float2(0.0f, 0.0f))) {
                    TASK_LIST.value = i
                }
            }
        }
        var task_label = "(none)"
        if (TASK_LIST.value >= 0 && TASK_LIST.value < length(TASK_NAMES)) {
            task_label = TASK_NAMES[TASK_LIST.value]
        }
        Text("task = {task_label} (idx {TASK_LIST.value})")
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData())

    live_end_frame()
}

[export]
def shutdown() {
    live_imgui_shutdown()
    live_destroy_window()
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
