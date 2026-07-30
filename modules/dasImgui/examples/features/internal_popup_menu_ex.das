options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require strings

// =============================================================================
// FEATURE: internal_popup_menu_ex — the imgui_internal.h popup / tooltip / menu
//          *Ex entry points. Cherry-picked into the v2 binding (bind_imgui.das
//          internal_allow_func): OpenPopupEx, ClosePopupToLevel,
//          ClosePopupsExceptModals, IsPopupOpen (raw-ImGuiID overload),
//          BeginPopupEx, BeginTooltipEx, BeginTooltipHidden, BeginMenuEx,
//          MenuItemEx. The four Begin*Ex pair with the public End* and are
//          surfaced as the unbalance-proof with_popup_ex / with_tooltip_ex /
//          with_tooltip_hidden / with_menu_ex guards (imgui_scope_builtin.das);
//          the five one-shots are raw on the ALLOWED_IMGUI allow-list. This is
//          the "binding is usable" gate: it calls the real bound API at runtime,
//          not just compiles it.
// SHOWS:   with_menu_ex (icon-column submenu) + MenuItemEx (icon + shortcut menu
//          item) in a menu bar; OpenPopupEx + IsPopupOpen + with_popup_ex driving
//          a popup by a precomputed ImGuiID (from GetID); ClosePopupToLevel /
//          ClosePopupsExceptModals popup-stack control; with_tooltip_ex
//          (OverridePrevious) on hover; with_tooltip_hidden measuring offscreen.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_popup_menu_ex.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_popup_menu_ex.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_popup_menu_ex.das
// =============================================================================

// ImGui IDs are conventionally shown as 8-digit hex.
def private hex8(v : uint) : string {
    let digits = "0123456789abcdef"
    return build_string() $(var writer) {
        writer |> write("0x")
        for (i in range(8)) {
            let nib = int((v >> uint((7 - i) * 4)) & 0xfu)
            writer |> write(slice(digits, nib, nib + 1))
        }
    }
}

// Auto-open the raw-id popup once so the popup path runtime-exercises headlessly
// (no click available); the "Open" button drives it interactively otherwise.
var g_first_frame = true
var g_last_menu_action = "(none yet)"

[export]
def init() {
    harness_init("internal_popup_menu_ex - imgui_internal.h popup / tooltip / menu *Ex", 780, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(740.0, 560.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "internal_popup_menu_ex", closable = false,
                      flags = ImGuiWindowFlags.MenuBar)) {

        // ----- menu bar: with_menu_ex (icon submenu) + MenuItemEx (icon items) -----
        // BeginMenuEx adds an icon column the public BeginMenu lacks; the body is
        // the dropdown. The inner MenuItemEx carries icon + shortcut columns.
        menu_bar(BAR) {
            with_menu_ex("File", "F", true) {
                if (MenuItemEx("New", "N", "Ctrl+N", false, true)) {
                    g_last_menu_action = "File / New"
                }
                if (MenuItemEx("Open", "O", "Ctrl+O", false, true)) {
                    g_last_menu_action = "File / Open"
                }
            }
            with_menu_ex("Edit", "E", true) {
                if (MenuItemEx("Copy", "C", "Ctrl+C", false, true)) {
                    g_last_menu_action = "Edit / Copy"
                }
            }
        }
        text("with_menu_ex / MenuItemEx — last menu action: {g_last_menu_action}")
        separator()

        // ----- raw-ImGuiID popup: OpenPopupEx / IsPopupOpen / with_popup_ex -----
        // The public popup container hashes a string id internally; the *Ex path
        // takes a precomputed ImGuiID, so it pairs with GetID / with_override_id.
        let pop_id = GetID("raw_id_popup")
        text("popup id (GetID) = {hex8(pop_id)}; IsPopupOpen = {IsPopupOpen(pop_id, ImGuiPopupFlags.None)}")
        if (button(OPEN_POPUP_BTN, (text = "OpenPopupEx(pop_id)"))) {
            OpenPopupEx(pop_id, ImGuiPopupFlags.None)
        }
        if (g_first_frame) {
            OpenPopupEx(pop_id, ImGuiPopupFlags.None)
            g_first_frame = false
        }
        with_popup_ex(pop_id, ImGuiWindowFlags.None) {
            text("raw-id popup body (with_popup_ex / BeginPopupEx)")
            // MenuItemEx works inside popups too (context-menu styling).
            if (MenuItemEx("Close to level 0", "X", "", false, true)) {
                // Safe: we are inside the popup, so the stack has >= 1 level.
                ClosePopupToLevel(0, true)
            }
            if (button(CLOSE_CUR_BTN, (text = "close_current_popup"))) {
                close_current_popup()
            }
        }
        if (button(CLOSE_ALL_BTN, (text = "ClosePopupsExceptModals()"))) {
            ClosePopupsExceptModals()
        }
        separator()

        // ----- tooltips: with_tooltip_ex (OverridePrevious) + with_tooltip_hidden -----
        text("Hover me for an OverridePrevious tooltip (with_tooltip_ex)")
        if (IsItemHovered()) {
            with_tooltip_ex(ImGuiTooltipFlags.OverridePrevious, ImGuiWindowFlags.None) {
                text("tooltip via with_tooltip_ex — replaces, not appends")
            }
        }
        // with_tooltip_hidden begins an off-screen tooltip purely to MEASURE its
        // content; nothing is shown. Run every frame to exercise the binding.
        with_tooltip_hidden() {
            text("measured offscreen by with_tooltip_hidden")
        }
        text("with_tooltip_hidden ran (offscreen measure — nothing shown)")
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
