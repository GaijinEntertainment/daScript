options gen2
options _comment_hygiene = true
options indenting = 4

module app_dockspace

require imgui public
require imgui/imgui_widgets_builtin public
require imgui/imgui_containers_builtin public
require imgui/imgui_docking_builtin public
require imgui/imgui_style_builtin public
require daslib/safe_addr

// Mirrors imgui_demo.cpp:8424-8551 — static void ShowExampleAppDockSpace(bool* p_open).
//
// The C++ original supports a "fullscreen" mode that claims the full viewport
// via SetNextWindowPos/Size/Viewport + WindowRounding/BorderSize style-vars +
// NoTitleBar/NoCollapse/NoResize/NoMove window flags. We port the simpler
// floating-host form (which the C++ comments call out as the typical use).
// The floating form still shows the load-bearing pieces: a host window with
// its own MenuBar + NoDocking (so the host itself isn't dockable into),
// dockspace_in_window for the explicit DockSpace call, dockspace flag
// toggles, and the "Close" menu item routing through take_close_request().

// ===== Module-scope state =====

var private DS_PADDING = false
var private DS_FLAG_NO_DOCKING_OVER_CENTRAL = false
var private DS_FLAG_NO_DOCKING_SPLIT = false
var private DS_FLAG_NO_UNDOCKING = false
var private DS_FLAG_NO_RESIZE = false
var private DS_FLAG_AUTO_HIDE_TAB_BAR = false
var private DS_FLAG_PASSTHRU_CENTRAL = false

// Set by the "Close" menu entry. ShowExampleAppDockSpace returns false when this
// flips OR when the X-button closes the window, so the aggregator's
// `if (SHOW_X && !ShowExampleAppDockSpace()) SHOW_X = false` works for both paths.
var private DS_REQUEST_CLOSE = false

def private current_dockspace_flags() : ImGuiDockNodeFlags {
    var flags = ImGuiDockNodeFlags.None
    if (DS_FLAG_NO_DOCKING_OVER_CENTRAL) {
        flags |= ImGuiDockNodeFlags.NoDockingOverCentralNode
    }
    if (DS_FLAG_NO_DOCKING_SPLIT) {
        flags |= ImGuiDockNodeFlags.NoDockingSplit
    }
    if (DS_FLAG_NO_UNDOCKING) {
        flags |= ImGuiDockNodeFlags.NoUndocking
    }
    if (DS_FLAG_NO_RESIZE) {
        flags |= ImGuiDockNodeFlags.NoResize
    }
    if (DS_FLAG_AUTO_HIDE_TAB_BAR) {
        flags |= ImGuiDockNodeFlags.AutoHideTabBar
    }
    if (DS_FLAG_PASSTHRU_CENTRAL) {
        flags |= ImGuiDockNodeFlags.PassthruCentralNode
    }
    return flags
}

def private current_host_flags() : ImGuiWindowFlags {
    // PassthruCentralNode needs a background-less host — without NoBackground the host fill covers the central node.
    var flags = ImGuiWindowFlags.MenuBar | ImGuiWindowFlags.NoDocking
    if (DS_FLAG_PASSTHRU_CENTRAL) {
        flags |= ImGuiWindowFlags.NoBackground
    }
    return flags
}

def private render_ds_window() {
    window(DS_WIN, (text = "DockSpace Demo", closable = true,
                    flags = current_host_flags())) {
        menu_bar(DS_MENU) {
            menu(DS_OPTIONS_MENU, (text = "Options", enabled = true)) {
                edit_checkbox(safe_addr(DS_PADDING),
                    (id = "DS_PADDING_CB", text = "Padding"))
                separator(DS_SEP_PAD)

                edit_checkbox(safe_addr(DS_FLAG_NO_DOCKING_OVER_CENTRAL),
                    (id = "DS_F_NDOC_CN", text = "Flag: NoDockingOverCentralNode"))
                edit_checkbox(safe_addr(DS_FLAG_NO_DOCKING_SPLIT),
                    (id = "DS_F_NDOC_SPLIT", text = "Flag: NoDockingSplit"))
                edit_checkbox(safe_addr(DS_FLAG_NO_UNDOCKING),
                    (id = "DS_F_NO_UNDOCK", text = "Flag: NoUndocking"))
                edit_checkbox(safe_addr(DS_FLAG_NO_RESIZE),
                    (id = "DS_F_NO_RESIZE", text = "Flag: NoResize"))
                edit_checkbox(safe_addr(DS_FLAG_AUTO_HIDE_TAB_BAR),
                    (id = "DS_F_AUTO_HIDE", text = "Flag: AutoHideTabBar"))
                edit_checkbox(safe_addr(DS_FLAG_PASSTHRU_CENTRAL),
                    (id = "DS_F_PASSTHRU", text = "Flag: PassthruCentralNode"))
                separator(DS_SEP_CLOSE)

                if (menu_item(DS_CLOSE_ITEM,
                              (text = "Close", shortcut = "", enabled = true))) {
                    DS_REQUEST_CLOSE = true
                }
            }
        }

        dockspace_in_window(DS, (size = float2(0.0f, 0.0f),
                                 flags = current_dockspace_flags())) {
            // Sample dockable panels so the dockspace has content.
            dock_window(DS_PANEL_A, (text = "Panel A", closable = false,
                                     flags = ImGuiWindowFlags.None)) {
                text(DS_PANEL_A_LINE, (text = "Drag this tab to dock / undock."))
            }
            dock_window(DS_PANEL_B, (text = "Panel B", closable = false,
                                     flags = ImGuiWindowFlags.None)) {
                text(DS_PANEL_B_LINE, (text = "Right-click the title for the window menu."))
            }
        }
    }
}

def public ShowExampleAppDockSpace() : bool {
    // ConfigFlags.DockingEnable is set by the standalone harness; the
    // imgui_demo aggregator path doesn't, so flip it on at entry. Idempotent.
    var io & = unsafe(GetIO())
    io.ConfigFlags |= ImGuiConfigFlags.DockingEnable
    // X-button closes by flipping DS_WIN.open=false; the aggregator then clears its
    // SHOW flag and stops calling us. On re-check we're re-entered with stale
    // open=false — reset so the window renders again.
    DS_WIN.open = true
    SetNextWindowSize(ImVec2(720.0f, 480.0f), ImGuiCond.FirstUseEver)
    // Match the C++ original: when "Padding" is off, push zero WindowPadding
    // around the host window's Begin so the dockspace fills edge-to-edge.
    if (DS_PADDING) {
        render_ds_window()
    } else {
        with_style((ImGuiStyleVar.WindowPadding, float2(0.0f, 0.0f))) {
            render_ds_window()
        }
    }
    let menu_closed = DS_REQUEST_CLOSE
    DS_REQUEST_CLOSE = false
    return DS_WIN.open && !menu_closed
}
