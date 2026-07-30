options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_demo

require imgui public
require imgui/imgui_widgets_builtin public
require imgui/imgui_containers_builtin public
require imgui/imgui_scope_builtin public
require imgui/imgui_table_builtin public
require imgui/imgui_boost_v2 public
require daslib/safe_addr
require daslib/enum_trait
require math

require widgets public
require layout public
require popups public
require tables public
require columns public
require inputs public
require about public
require style_editor public
require user_guide public
require app_main_menu public
require app_console public
require app_log public
require app_custom_rendering public
require app_dockspace public
require app_documents public
require app_small public

// Mirrors imgui_demo.cpp:275-649 — void ImGui::ShowDemoWindow(bool* p_open).
// Renamed to avoid collision with ImGui's built-in ShowDemoWindow binding.
//
// The window carries a MenuBar with three sub-menus:
//   Menu     — calls ShowExampleMenuFile() (shared with the modals demos
//              and ShowExampleAppMainMenuBar).
//   Examples — toggles for the 13 mini-app / app windows (Main menu bar +
//              8 small apps + 4 full apps).
//   Tools    — toggles for Metrics / Debug Log / ID Stack / Style Editor /
//              About; each pops its own ImGui-native window via the
//              ShowMetricsWindow / ShowDebugLogWindow / etc. bindings.
//
// Beneath the menubar live three meta-sections (Help / Configuration /
// Window options), then the six demo-window section dispatchers (Widgets,
// Layout, Popups, Tables, Columns, Inputs).

// Examples-menu toggles — 13 entries (1 main menu bar + 8 small + 4 full).
var SHOW_APP_MAIN_MENU_BAR = false
var SHOW_APP_CONSOLE = false
var SHOW_APP_CUSTOM_RENDERING = false
var SHOW_APP_DOCKSPACE = false
var SHOW_APP_DOCUMENTS = false
var SHOW_APP_LOG = false
var SHOW_APP_PROPERTY_EDITOR = false
var SHOW_APP_LAYOUT = false
var SHOW_APP_SIMPLE_OVERLAY = false
var SHOW_APP_AUTO_RESIZE = false
var SHOW_APP_CONSTRAINED_RESIZE = false
var SHOW_APP_FULLSCREEN = false
var SHOW_APP_LONG_TEXT = false
var SHOW_APP_WINDOW_TITLES = false

// Tools-menu toggles — 5 entries pointing at the ImGui-native windows.
var SHOW_TOOL_METRICS = false
var SHOW_TOOL_DEBUG_LOG = false
var SHOW_TOOL_ID_STACK = false
var SHOW_TOOL_STYLE_EDITOR = false
var SHOW_TOOL_ABOUT = false

// Window-options toggles (cpp:335-346) — 12 booleans wired into DEMO_WIN's flags;
// each `no_*` ORs the matching WindowFlag bit (no_menu inverts, since the default
// renders the menu bar). `no_close` is informational only — this port isn't closable.
var WO_NO_TITLEBAR = false
var WO_NO_SCROLLBAR = false
var WO_NO_MENU = false
var WO_NO_MOVE = false
var WO_NO_RESIZE = false
var WO_NO_COLLAPSE = false
var WO_NO_CLOSE = false
var WO_NO_NAV = false
var WO_NO_BACKGROUND = false
var WO_NO_BRING_TO_FRONT = false
var WO_NO_DOCKING = false
var WO_UNSAVED_DOCUMENT = false

def private compute_demo_window_flags() : ImGuiWindowFlags {
    var flags = ImGuiWindowFlags.None
    if (WO_NO_TITLEBAR) {
        flags |= ImGuiWindowFlags.NoTitleBar
    }
    if (WO_NO_SCROLLBAR) {
        flags |= ImGuiWindowFlags.NoScrollbar
    }
    if (!WO_NO_MENU) {
        flags |= ImGuiWindowFlags.MenuBar
    }
    if (WO_NO_MOVE) {
        flags |= ImGuiWindowFlags.NoMove
    }
    if (WO_NO_RESIZE) {
        flags |= ImGuiWindowFlags.NoResize
    }
    if (WO_NO_COLLAPSE) {
        flags |= ImGuiWindowFlags.NoCollapse
    }
    if (WO_NO_NAV) {
        flags |= ImGuiWindowFlags.NoNav
    }
    if (WO_NO_BACKGROUND) {
        flags |= ImGuiWindowFlags.NoBackground
    }
    if (WO_NO_BRING_TO_FRONT) {
        flags |= ImGuiWindowFlags.NoBringToFrontOnFocus
    }
    if (WO_NO_DOCKING) {
        flags |= ImGuiWindowFlags.NoDocking
    }
    if (WO_UNSAVED_DOCUMENT) {
        flags |= ImGuiWindowFlags.UnsavedDocument
    }
    return flags
}

def RunImGuiDemo() {
    window(DEMO_WIN, (text = "Dear ImGui Demo (daslang port)",
                      closable = false,
                      flags = compute_demo_window_flags())) {
        // Menu bar — gated by the WO_NO_MENU toggle below (ImGui silently
        // ignores menu_bar() when the window lacks the MenuBar flag).
        menu_bar(DEMO_MENU_BAR) {
            menu(DEMO_MENU_FILE, (text = "Menu", enabled = true)) {
                ShowExampleMenuFile()
            }
            menu(DEMO_MENU_EXAMPLES, (text = "Examples", enabled = true)) {
                edit_menu_item(safe_addr(SHOW_APP_MAIN_MENU_BAR),
                    (id = "EX_MAIN_MENU_BAR", text = "Main menu bar"))
                separator_text(DEMO_EX_SEP_MINI, "Mini apps")
                edit_menu_item(safe_addr(SHOW_APP_CONSOLE),
                    (id = "EX_CONSOLE", text = "Console"))
                edit_menu_item(safe_addr(SHOW_APP_CUSTOM_RENDERING),
                    (id = "EX_CUSTOM_RENDERING", text = "Custom rendering"))
                edit_menu_item(safe_addr(SHOW_APP_DOCKSPACE),
                    (id = "EX_DOCKSPACE", text = "Dockspace"))
                edit_menu_item(safe_addr(SHOW_APP_DOCUMENTS),
                    (id = "EX_DOCUMENTS", text = "Documents"))
                edit_menu_item(safe_addr(SHOW_APP_LOG),
                    (id = "EX_LOG", text = "Log"))
                edit_menu_item(safe_addr(SHOW_APP_PROPERTY_EDITOR),
                    (id = "EX_PROP_ED", text = "Property editor"))
                edit_menu_item(safe_addr(SHOW_APP_LAYOUT),
                    (id = "EX_LAYOUT", text = "Simple layout"))
                edit_menu_item(safe_addr(SHOW_APP_SIMPLE_OVERLAY),
                    (id = "EX_OVERLAY", text = "Simple overlay"))
                separator_text(DEMO_EX_SEP_CONCEPTS, "Concepts")
                edit_menu_item(safe_addr(SHOW_APP_AUTO_RESIZE),
                    (id = "EX_AUTO_RESIZE", text = "Auto-resizing window"))
                edit_menu_item(safe_addr(SHOW_APP_CONSTRAINED_RESIZE),
                    (id = "EX_CONSTR_RESIZE", text = "Constrained-resizing window"))
                edit_menu_item(safe_addr(SHOW_APP_FULLSCREEN),
                    (id = "EX_FULLSCREEN", text = "Fullscreen window"))
                edit_menu_item(safe_addr(SHOW_APP_LONG_TEXT),
                    (id = "EX_LONG_TEXT", text = "Long text display"))
                edit_menu_item(safe_addr(SHOW_APP_WINDOW_TITLES),
                    (id = "EX_WIN_TITLES", text = "Manipulating window titles"))
            }
            menu(DEMO_MENU_TOOLS, (text = "Tools", enabled = true)) {
                edit_menu_item(safe_addr(SHOW_TOOL_METRICS),
                    (id = "TOOL_METRICS", text = "Metrics/Debugger"))
                edit_menu_item(safe_addr(SHOW_TOOL_DEBUG_LOG),
                    (id = "TOOL_DEBUG_LOG", text = "Debug Log"))
                edit_menu_item(safe_addr(SHOW_TOOL_ID_STACK),
                    (id = "TOOL_ID_STACK", text = "ID Stack Tool"))
                edit_menu_item(safe_addr(SHOW_TOOL_STYLE_EDITOR),
                    (id = "TOOL_STYLE_ED", text = "Style Editor"))
                separator(DEMO_TOOLS_SEP)
                edit_menu_item(safe_addr(SHOW_TOOL_ABOUT),
                    (id = "TOOL_ABOUT", text = "About Dear ImGui"))
            }
        }

        text("dear imgui says hello! ({GetVersion()}) ({IMGUI_VERSION_NUM})")
        spacing(DEMO_INTRO_SPACING)

        // Big-widget default width — 12 characters of right-pad for labels;
        // mirrors cpp:380 PushItemWidth(GetFontSize() * -12).
        with_item_width(GetFontSize() * -12.0f) {
            ShowDemoWindowHelp()
            ShowDemoWindowConfiguration()
            ShowDemoWindowWindowOptions()

            ShowDemoWindowWidgets()
            ShowDemoWindowLayout()
            ShowDemoWindowPopups()
            ShowDemoWindowTables()
            ShowDemoWindowColumns()
            ShowDemoWindowInputs()
        }
    }

    // Example-app windows live OUTSIDE the demo window — each opens its own.
    if (SHOW_APP_MAIN_MENU_BAR) {
        ShowExampleAppMainMenuBar()
    }
    if (SHOW_APP_CONSOLE) {
        ShowExampleAppConsole()
        // Console's "Close Console" title-bar context menu sets an internal
        // flag; consume it here to flip our visibility toggle off so the
        // window goes away on the next frame.
        if (take_close_request()) {
            SHOW_APP_CONSOLE = false
        }
    }
    if (SHOW_APP_LOG) {
        ShowExampleAppLog()
    }
    // app_small entries — each ShowExampleApp* returns true while open and
    // false when the user clicked X. Mirror the return value back to the
    // toggle so the menu check-mark stays in sync.
    if (SHOW_APP_LAYOUT && !ShowExampleAppLayout()) {
        SHOW_APP_LAYOUT = false
    }
    if (SHOW_APP_PROPERTY_EDITOR && !ShowExampleAppPropertyEditor()) {
        SHOW_APP_PROPERTY_EDITOR = false
    }
    if (SHOW_APP_LONG_TEXT && !ShowExampleAppLongText()) {
        SHOW_APP_LONG_TEXT = false
    }
    if (SHOW_APP_AUTO_RESIZE && !ShowExampleAppAutoResize()) {
        SHOW_APP_AUTO_RESIZE = false
    }
    if (SHOW_APP_CONSTRAINED_RESIZE && !ShowExampleAppConstrainedResize()) {
        SHOW_APP_CONSTRAINED_RESIZE = false
    }
    if (SHOW_APP_SIMPLE_OVERLAY && !ShowExampleAppSimpleOverlay()) {
        SHOW_APP_SIMPLE_OVERLAY = false
    }
    if (SHOW_APP_FULLSCREEN && !ShowExampleAppFullscreen()) {
        SHOW_APP_FULLSCREEN = false
    }
    // WindowTitles is closable=false in cpp (all 3 windows have nullptr p_open).
    if (SHOW_APP_WINDOW_TITLES) {
        ShowExampleAppWindowTitles()
    }
    if (SHOW_APP_CUSTOM_RENDERING && !ShowExampleAppCustomRendering()) {
        SHOW_APP_CUSTOM_RENDERING = false
    }
    if (SHOW_APP_DOCKSPACE && !ShowExampleAppDockSpace()) {
        SHOW_APP_DOCKSPACE = false
    }
    if (SHOW_APP_DOCUMENTS && !ShowExampleAppDocuments()) {
        SHOW_APP_DOCUMENTS = false
    }

    // Tools menu — each Show*Window mutates its bool? when the user clicks X
    // on the tool window's titlebar, so the menu check-mark auto-syncs.
    if (SHOW_TOOL_METRICS) {
        ShowMetricsWindow(safe_addr(SHOW_TOOL_METRICS))
    }
    if (SHOW_TOOL_DEBUG_LOG) {
        ShowDebugLogWindow(safe_addr(SHOW_TOOL_DEBUG_LOG))
    }
    if (SHOW_TOOL_ID_STACK) {
        ShowIDStackToolWindow(safe_addr(SHOW_TOOL_ID_STACK))
    }
    if (SHOW_TOOL_STYLE_EDITOR) {
        window(DEMO_STYLE_ED_WIN, (text = "Dear ImGui Style Editor",
                                    closable = true,
                                    flags = ImGuiWindowFlags.None)) {
            style_editor::ShowStyleEditor()
        }
        // Style Editor uses [container] WindowState — close via window's X
        // flips DEMO_STYLE_ED_WIN.open false; mirror back to the toggle.
        if (!DEMO_STYLE_ED_WIN.open) {
            SHOW_TOOL_STYLE_EDITOR = false
            DEMO_STYLE_ED_WIN.open = true
        }
    }
    if (SHOW_TOOL_ABOUT) {
        ShowAboutWindow(safe_addr(SHOW_TOOL_ABOUT))
    }
}

// =============================================================================
// Help — cpp:444-462
// =============================================================================
def private ShowDemoWindowHelp() {
    collapsing_header(DEMO_HELP_HEADER, (text = "Help",
                                          closable = false,
                                          flags = ImGuiTreeNodeFlags.None)) {
        separator_text(DEMO_HELP_SEP_ABOUT, "ABOUT THIS DEMO:")
        bullet_text("Sections below are demonstrating many aspects of the library.")
        bullet_text("The \"Examples\" menu above leads to more demo contents.")
        bullet_text((text = "The \"Examples\" menu above gives access to: About Box, " +
            "Style Editor,\nand Metrics/Debugger (general purpose Dear ImGui " +
            "debugging tool)."))

        separator_text(DEMO_HELP_SEP_PROG, "PROGRAMMER GUIDE:")
        bullet_text("See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!")
        bullet_text("See comments in imgui.cpp.")
        bullet_text("See example applications in the examples/ folder.")
        bullet_text("Read the FAQ at https://www.dearimgui.com/faq/")
        bullet_text("Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls.")
        bullet_text("Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls.")

        separator_text(DEMO_HELP_SEP_USER, "USER GUIDE:")
        user_guide::ShowUserGuide()
    }
}

// =============================================================================
// Configuration — cpp:465-616
// =============================================================================

// Help markers (cpp:473..) are dropped — "(?)"+item_tooltip chains add a lot of
// state for a debug-helper section; easy to add later if needed.
def private ShowDemoWindowConfiguration() {
    collapsing_header(DEMO_CONFIG_HEADER, (text = "Configuration",
                                             closable = false,
                                             flags = ImGuiTreeNodeFlags.None)) {
        tree_node(DEMO_CONFIG_TREE, (text = "Configuration",
                                       flags = ImGuiTreeNodeFlags.None)) {
            var io & = unsafe(GetIO())
            var cf_ptr = unsafe(addr(io.ConfigFlags))

            separator_text(DEMO_CFG_SEP_GENERAL, "General")
            edit_checkbox_flags(cf_ptr,
                (id = "CFG_NAV_KBD", text = "io.ConfigFlags: NavEnableKeyboard",
                 flags_value = ImGuiConfigFlags.NavEnableKeyboard))
            edit_checkbox_flags(cf_ptr,
                (id = "CFG_NAV_PAD", text = "io.ConfigFlags: NavEnableGamepad",
                 flags_value = ImGuiConfigFlags.NavEnableGamepad))
            edit_checkbox(unsafe(addr(io.ConfigNavMoveSetMousePos)),
                (id = "CFG_NAV_MOUSE_POS",
                 text = "io.ConfigNavMoveSetMousePos"))
            edit_checkbox_flags(cf_ptr,
                (id = "CFG_NO_MOUSE", text = "io.ConfigFlags: NoMouse",
                 flags_value = ImGuiConfigFlags.NoMouse))
            // cpp:481-489 — when NoMouse is on, flash a "press SPACE" reminder and consume Space to unset it, so a tester can't lock themselves out.
            if (bool(io.ConfigFlags & ImGuiConfigFlags.NoMouse)) {
                if (fmod(float(GetTime()), 0.40f) < 0.20f) {
                    same_line(DEMO_CFG_NOMOUSE_SL)
                    text(DEMO_CFG_NOMOUSE_HINT, (text = "<<PRESS SPACE TO DISABLE>>"))
                }
                if (IsKeyPressed(ImGuiKey.Space, true)) {
                    io.ConfigFlags &= ~ImGuiConfigFlags.NoMouse
                }
            }
            edit_checkbox_flags(cf_ptr,
                (id = "CFG_NO_MOUSE_CURSOR",
                 text = "io.ConfigFlags: NoMouseCursorChange",
                 flags_value = ImGuiConfigFlags.NoMouseCursorChange))
            edit_checkbox(unsafe(addr(io.ConfigInputTrickleEventQueue)),
                (id = "CFG_TRICKLE_QUEUE",
                 text = "io.ConfigInputTrickleEventQueue"))
            edit_checkbox(unsafe(addr(io.MouseDrawCursor)),
                (id = "CFG_MOUSE_DRAW_CURSOR", text = "io.MouseDrawCursor"))

            separator_text(DEMO_CFG_SEP_DOCKING, "Docking")
            edit_checkbox_flags(cf_ptr,
                (id = "CFG_DOCKING_ENABLE",
                 text = "io.ConfigFlags: DockingEnable",
                 flags_value = ImGuiConfigFlags.DockingEnable))
            if (bool(io.ConfigFlags & ImGuiConfigFlags.DockingEnable)) {
                with_indent(0.0f) {
                    edit_checkbox(unsafe(addr(io.ConfigDockingNoSplit)),
                        (id = "CFG_DOCK_NO_SPLIT",
                         text = "io.ConfigDockingNoSplit"))
                    edit_checkbox(unsafe(addr(io.ConfigDockingWithShift)),
                        (id = "CFG_DOCK_WITH_SHIFT",
                         text = "io.ConfigDockingWithShift"))
                    edit_checkbox(unsafe(addr(io.ConfigDockingAlwaysTabBar)),
                        (id = "CFG_DOCK_ALWAYS_TAB_BAR",
                         text = "io.ConfigDockingAlwaysTabBar"))
                    edit_checkbox(unsafe(addr(io.ConfigDockingTransparentPayload)),
                        (id = "CFG_DOCK_TRANSP_PAYLOAD",
                         text = "io.ConfigDockingTransparentPayload"))
                }
            }

            separator_text(DEMO_CFG_SEP_VIEWPORTS, "Multi-viewports")
            edit_checkbox_flags(cf_ptr,
                (id = "CFG_VIEWPORTS_ENABLE",
                 text = "io.ConfigFlags: ViewportsEnable",
                 flags_value = ImGuiConfigFlags.ViewportsEnable))
            if (bool(io.ConfigFlags & ImGuiConfigFlags.ViewportsEnable)) {
                with_indent(0.0f) {
                    edit_checkbox(unsafe(addr(io.ConfigViewportsNoAutoMerge)),
                        (id = "CFG_VP_NO_AUTO_MERGE",
                         text = "io.ConfigViewportsNoAutoMerge"))
                    edit_checkbox(unsafe(addr(io.ConfigViewportsNoTaskBarIcon)),
                        (id = "CFG_VP_NO_TASKBAR",
                         text = "io.ConfigViewportsNoTaskBarIcon"))
                    edit_checkbox(unsafe(addr(io.ConfigViewportsNoDecoration)),
                        (id = "CFG_VP_NO_DECORATION",
                         text = "io.ConfigViewportsNoDecoration"))
                    edit_checkbox(unsafe(addr(io.ConfigViewportsNoDefaultParent)),
                        (id = "CFG_VP_NO_DEFAULT_PARENT",
                         text = "io.ConfigViewportsNoDefaultParent"))
                }
            }

            separator_text(DEMO_CFG_SEP_WIDGETS, "Widgets")
            edit_checkbox(unsafe(addr(io.ConfigInputTextCursorBlink)),
                (id = "CFG_TEXT_BLINK", text = "io.ConfigInputTextCursorBlink"))
            edit_checkbox(unsafe(addr(io.ConfigInputTextEnterKeepActive)),
                (id = "CFG_TEXT_ENTER_KEEP_ACTIVE",
                 text = "io.ConfigInputTextEnterKeepActive"))
            edit_checkbox(unsafe(addr(io.ConfigDragClickToInputText)),
                (id = "CFG_DRAG_CLICK_TO_INPUT",
                 text = "io.ConfigDragClickToInputText"))
            edit_checkbox(unsafe(addr(io.ConfigWindowsResizeFromEdges)),
                (id = "CFG_WIN_RESIZE_EDGES",
                 text = "io.ConfigWindowsResizeFromEdges"))
            edit_checkbox(unsafe(addr(io.ConfigWindowsMoveFromTitleBarOnly)),
                (id = "CFG_WIN_MOVE_FROM_TITLE",
                 text = "io.ConfigWindowsMoveFromTitleBarOnly"))
            edit_checkbox(unsafe(addr(io.ConfigMacOSXBehaviors)),
                (id = "CFG_MACOS_BEHAVIORS", text = "io.ConfigMacOSXBehaviors"))
            text("Also see Style->Rendering for rendering options.")

            separator_text(DEMO_CFG_SEP_DEBUG, "Debug")
            edit_checkbox(unsafe(addr(io.ConfigDebugIsDebuggerPresent)),
                (id = "CFG_DBG_DEBUGGER_PRESENT",
                 text = "io.ConfigDebugIsDebuggerPresent"))
            // cpp:552 — grey out (BeginDisabled) the next checkbox: the option only means anything if set at boot, so it's advisory.
            with_disabled(true) {
                edit_checkbox(unsafe(addr(io.ConfigDebugBeginReturnValueOnce)),
                    (id = "CFG_DBG_BEGIN_RETVAL_ONCE",
                     text = "io.ConfigDebugBeginReturnValueOnce"))
            }
            edit_checkbox(unsafe(addr(io.ConfigDebugBeginReturnValueLoop)),
                (id = "CFG_DBG_BEGIN_RETVAL_LOOP",
                 text = "io.ConfigDebugBeginReturnValueLoop"))
            edit_checkbox(unsafe(addr(io.ConfigDebugIgnoreFocusLoss)),
                (id = "CFG_DBG_IGNORE_FOCUS_LOSS",
                 text = "io.ConfigDebugIgnoreFocusLoss"))
            edit_checkbox(unsafe(addr(io.ConfigDebugIniSettings)),
                (id = "CFG_DBG_INI_SETTINGS", text = "io.ConfigDebugIniSettings"))
        }
    }
}

// =============================================================================
// Window options — cpp:619-637
// =============================================================================
def private ShowDemoWindowWindowOptions() {
    collapsing_header(DEMO_WIN_OPTS_HEADER,
                      (text = "Window options", closable = false,
                       flags = ImGuiTreeNodeFlags.None)) {
        data_table(DEMO_WIN_OPTS_TABLE,
                   (text = "split", columns = 3,
                    flags = ImGuiTableFlags.None,
                    outer_size = float2(0.0f, 0.0f),
                    inner_width = 0.0f)) {
            table_next_column()
            edit_checkbox(safe_addr(WO_NO_TITLEBAR),
                (id = "WO_NO_TITLEBAR", text = "No titlebar"))
            table_next_column()
            edit_checkbox(safe_addr(WO_NO_SCROLLBAR),
                (id = "WO_NO_SCROLLBAR", text = "No scrollbar"))
            table_next_column()
            edit_checkbox(safe_addr(WO_NO_MENU),
                (id = "WO_NO_MENU", text = "No menu"))
            table_next_column()
            edit_checkbox(safe_addr(WO_NO_MOVE),
                (id = "WO_NO_MOVE", text = "No move"))
            table_next_column()
            edit_checkbox(safe_addr(WO_NO_RESIZE),
                (id = "WO_NO_RESIZE", text = "No resize"))
            table_next_column()
            edit_checkbox(safe_addr(WO_NO_COLLAPSE),
                (id = "WO_NO_COLLAPSE", text = "No collapse"))
            table_next_column()
            edit_checkbox(safe_addr(WO_NO_CLOSE),
                (id = "WO_NO_CLOSE", text = "No close"))
            table_next_column()
            edit_checkbox(safe_addr(WO_NO_NAV),
                (id = "WO_NO_NAV", text = "No nav"))
            table_next_column()
            edit_checkbox(safe_addr(WO_NO_BACKGROUND),
                (id = "WO_NO_BG", text = "No background"))
            table_next_column()
            edit_checkbox(safe_addr(WO_NO_BRING_TO_FRONT),
                (id = "WO_NO_BRING_TO_FRONT", text = "No bring to front"))
            table_next_column()
            edit_checkbox(safe_addr(WO_NO_DOCKING),
                (id = "WO_NO_DOCKING", text = "No docking"))
            table_next_column()
            edit_checkbox(safe_addr(WO_UNSAVED_DOCUMENT),
                (id = "WO_UNSAVED_DOCUMENT", text = "Unsaved document"))
        }
    }
}
