options gen2
options _comment_hygiene = true

require imgui
require imgui_app
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: tab_bar — BeginTabBar / EndTabBar with nested tab_item blocks.
//
//   tab_bar(IDENT, (text = "...", flags = ImGuiTabBarFlags....)) {
//       tab_item(TAB_A, (text = "A", closable = false, flags = ...)) { ... }
//       tab_item(TAB_B, (text = "B", closable = true,  flags = ...)) { ... }
//   }
//
// Key behaviors:
//
//   1. Only the ACTIVE tab's body block runs each frame — widgets inside
//      inactive tabs are NOT in the registry that frame. A driver doing
//      imgui_force_set against an inactive tab's child path gets a 404.
//
//   2. Closable tabs (closable=true) render an X-button. ImGui flips
//      state.open=false; the wrapper then skips BeginTabItem entirely so
//      the tab disappears from the strip until pending_open is set true
//      again.
//
//   3. ImGui owns active-tab selection — there is NO state.pending_select.
//      Drive it from outside by imgui_click against the tab_item path
//      (the click hits the tab strip header).
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/tab_bar.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/tab_bar.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"TB_WIN/MAIN_TABS/AUDIO_TAB"}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_close","args":{"target":"TB_WIN/MAIN_TABS/DRAFT_TAB"}}' \
//        localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui tab_bar tutorial", 760, 540)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.4
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(720.0f, 500.0f), ImGuiCond.Always)
    window(TB_WIN, (text = "tab_bar tutorial", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        text("Three tabs plus one closable Draft tab. Click a header to switch.")
        separator()

        // Reorderable + TabListPopupButton (the ▼ menu on the right).
        tab_bar(MAIN_TABS, (text = "MainTabs",
                            flags = ImGuiTabBarFlags.Reorderable |
                                    ImGuiTabBarFlags.TabListPopupButton)) {

            // ---- General tab (non-closable, default selection) ----
            tab_item(GENERAL_TAB, (text = "General", closable = false,
                                   flags = ImGuiTabItemFlags.None)) {
                text("Only the active tab's body block runs each frame.")
                text("Switch tabs - checkbox state below survives because the")
                text("[container] state lives outside the body.")
                checkbox(G_WIRE, (text = "Wireframe"))
                checkbox(G_LIVE, (text = "Live preview"))
            }

            // ---- Audio tab ----
            tab_item(AUDIO_TAB, (text = "Audio", closable = false,
                                 flags = ImGuiTabItemFlags.None)) {
                slider_int(A_VOLUME, (text = "Volume"))
                checkbox(A_MUTE, (text = "Mute"))
                text("VOLUME = {A_VOLUME.value}  MUTE = {A_MUTE.value}")
            }

            // ---- Info tab ----
            tab_item(INFO_TAB, (text = "Info", closable = false,
                                flags = ImGuiTabItemFlags.None)) {
                text(INFO_LEAD, (text = "Tab metadata - read-only."))
                text("MAIN_TABS.flags = Reorderable | TabListPopupButton")
                text("Inactive tab bodies do NOT register child widgets.")
            }

            // ---- Draft tab (closable, has X-button) ----
            tab_item(DRAFT_TAB, (text = "Draft", closable = true,
                                 flags = ImGuiTabItemFlags.None)) {
                text("Draft. Click the x on the tab header to close it.")
                text("DRAFT_TAB.open = {DRAFT_TAB.open}")
                slider_int(D_LINE_LEN, (text = "Line length"))
            }
        }

        separator()
        text("Outside the tab_bar - runs every frame regardless of active tab.")
        text("DRAFT_TAB.open is observable from out here: {DRAFT_TAB.open}")
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    live_imgui_render()

    live_end_frame()
}

[export]
def shutdown() {
    live_imgui_shutdown()
    live_destroy_window()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
