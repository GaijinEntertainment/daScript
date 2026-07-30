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
require imgui/imgui_docking_builtin
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: docking — full-viewport dockspace + draggable Begin/End windows.
//
// Three things compose to make dockable UI:
//   1. `io.ConfigFlags |= ImGuiConfigFlags.DockingEnable` in `init()` — turns
//      on ImGui's docking machinery for the whole session.
//   2. `dockspace(DOCK_ROOT, (flags=...))` in the frame loop — wraps
//      DockSpaceOverViewport so the whole window becomes a dock target, and
//      captures the returned `state.dock_id` for the layout helper to use.
//   3. `dock_window(NAME, (text=..., closable=..., flags=...))` per panel —
//      Begin/End-wrapped windows that ImGui dynamically docks/undocks based
//      on the user dragging their tab.
//
// The DockBuilder helper (`setup_default_layout`) seeds a 3-pane layout on
// the first frame so the user sees an arranged UI without having to drag
// tabs manually. After the first run, dock state persists via ImGui's
// `imgui.ini` — drag a tab anywhere and the layout sticks.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/docking.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/docking.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_undock","args":{"target":"DOCK_ROOT/OUTPUT"}}'    localhost:9090/command
//   curl -X POST -d '{"name":"imgui_set_window_pos","args":{"target":"DOCK_ROOT/OUTPUT","value":{"x":580,"y":220,"w":360,"h":220}}}' localhost:9090/command
//   curl -X POST -d '{"name":"imgui_dock_reset","args":{"target":"DOCK_ROOT"}}'        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_close","args":{"target":"DOCK_ROOT/OUTPUT"}}'      localhost:9090/command
//
// Note: dockspace is a [container] that pushes its identifier onto the registry
// path, so dock_window targets are path-qualified (``DOCK_ROOT/<name>``).
// =============================================================================

def setup_default_layout(dock_id : uint) {
    //! Seed a 3-pane layout on the first frame: Explorer (left, full height) |
    //! Source over Output (right column, split). Each pane's window-title string
    //! MUST match its dock_window's `text=` — that's how ImGui ties windows to nodes.
    DockBuilderRemoveNode(dock_id)
    DockBuilderAddDockSpaceNode(dock_id, ImGuiDockNodeFlags.None)
    let vp = GetMainViewport()
    DockBuilderSetNodeSize(dock_id, ImVec2(vp.Size.x, vp.Size.y))
    var left_id : uint = 0u
    var right_id : uint = 0u
    DockBuilderSplitNode(dock_id, ImGuiDir.Left, 0.25f, left_id, right_id)
    var top_id : uint = 0u
    var bottom_id : uint = 0u
    DockBuilderSplitNode(right_id, ImGuiDir.Up, 0.6f, top_id, bottom_id)
    DockBuilderDockWindow("Explorer", left_id)
    DockBuilderDockWindow("Source",   top_id)
    DockBuilderDockWindow("Output",   bottom_id)
    DockBuilderFinish(dock_id)
}

[export]
def init() {
    live_create_window("dasImgui docking tutorial", 1024, 720)
    live_imgui_init(live_window)
    // Docking layout persists to imgui.ini; disable that so the recording always
    // starts from setup_default_layout's seeded 3-pane arrangement (a leftover
    // ini from a prior run would otherwise override the seed non-deterministically).
    DisableIniPersistence()
    var io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.5
    // The single flag that turns docking on for the whole session. Without
    // this, DockSpace() / DockSpaceOverViewport() render nothing.
    io.ConfigFlags |= ImGuiConfigFlags.DockingEnable
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    // Full-viewport dockspace. PassthruCentralNode lets the OS-window
    // background show through the unsplit center (irrelevant here — we
    // split everything — but it's the conventional default).
    dockspace(DOCK_ROOT, (flags = ImGuiDockNodeFlags.PassthruCentralNode)) {
        // Seed the layout once per session. has_initial_layout flips false on
        // imgui_dock_reset; the renderer then re-runs setup. dock_id is
        // captured by the dockspace wrapper before this block runs.
        if (!DOCK_ROOT.has_initial_layout && DOCK_ROOT.dock_id != 0u) {
            setup_default_layout(DOCK_ROOT.dock_id)
            DOCK_ROOT.has_initial_layout = true
        }
        dock_window(EXPLORER, (text = "Explorer", closable = false,
                               flags = ImGuiWindowFlags.None)) {
            text("Files")
            button(REFRESH_BTN, (text = "Refresh"))
        }
        dock_window(SOURCE, (text = "Source", closable = false,
                             flags = ImGuiWindowFlags.None)) {
            text("// drag tabs to rearrange")
            text("// - layout sticks via imgui.ini")
            button(SAVE_BTN, (text = "Save"))
        }
        dock_window(OUTPUT, (text = "Output", closable = true,
                             flags = ImGuiWindowFlags.None)) {
            text("> ready.")
            button(CLEAR_BTN, (text = "Clear"))
        }
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
