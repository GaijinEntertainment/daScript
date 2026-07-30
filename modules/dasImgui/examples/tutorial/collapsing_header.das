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
// TUTORIAL: collapsing_header — expandable section header with optional X-button.
//
//   collapsing_header(IDENT, (text = "..", closable = bool,
//                             flags = ImGuiTreeNodeFlags....)) { body }
//
// Same shape as tree_node BUT no TreePop pair — ImGui owns the close lifecycle.
// Two distinct gates control visibility, each on its own field:
//
//   1. EXPANDED (chevron) — controls whether the BODY runs. state.opened
//      mirrors CollapsingHeader's per-frame return. pending_open /
//      pending_close (driven by imgui_open / imgui_close) override the
//      chevron via SetNextItemOpen on the next frame.
//
//   2. CLOSABLE (X-button) — controls whether the entire HEADER STRIP
//      renders. Only active when closable=true. ImGui flips state.open=false
//      on X-click; the wrapper then skips BeginCollapsingHeader entirely so
//      the strip vanishes. imgui_open re-sets state.open=true; imgui_close
//      does NOT — it only collapses the chevron via the expanded channel.
//      To programmatically hide the strip, write state.open=false from app
//      code.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/collapsing_header.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/collapsing_header.das
//
// DRIVE (when running live):
//   # Collapse the chevron (body stops rendering; header strip stays):
//   curl -X POST -d '{"name":"imgui_close","args":{"target":"CH_WIN/CLOSABLE_CH"}}' \
//        localhost:9090/command
//   # Re-expand the chevron AND re-show the strip if X-button hid it:
//   curl -X POST -d '{"name":"imgui_open","args":{"target":"CH_WIN/CLOSABLE_CH"}}' \
//        localhost:9090/command
//   # To HIDE the strip, click the X-button or write CLOSABLE_CH.open=false
//   # from app code — there's no imgui_close path that does this.
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui collapsing_header tutorial", 760, 520)
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
    SetNextWindowSize(ImVec2(720.0f, 480.0f), ImGuiCond.Always)
    window(CH_WIN, (text = "collapsing_header tutorial", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        text("Three collapsing_header variants - basic, closable, default-open.")
        separator()

        // ---- A: basic (non-closable) ----
        collapsing_header(BASIC_CH, (text = "Basic header", closable = false,
                                     flags = ImGuiTreeNodeFlags.None)) {
            text("Click the chevron to fold this section.")
            checkbox(BASIC_LIVE, (text = "Receive updates"))
            text("opened = {BASIC_CH.opened}")
        }

        // ---- B: closable (renders an X-button) ----
        collapsing_header(CLOSABLE_CH, (text = "Closable header",
                                        closable = true,
                                        flags = ImGuiTreeNodeFlags.DefaultOpen)) {
            text("X-button on the right of the header strip closes this section.")
            text("Once closed, the whole row disappears - flip back via")
            text("imgui_open against CH_WIN/CLOSABLE_CH.")
            VOLUME.bounds = (0, 100)
            slider_int(VOLUME, (text = "volume"))
        }
        text("CLOSABLE_CH.open = {CLOSABLE_CH.open}, opened = {CLOSABLE_CH.opened}")
        separator()

        // ---- C: DefaultOpen flag (chevron already down on first frame) ----
        collapsing_header(STARTUP_CH, (text = "Starts open (DefaultOpen flag)",
                                       closable = false,
                                       flags = ImGuiTreeNodeFlags.DefaultOpen)) {
            text(STARTUP_HINT, (text = "ImGuiTreeNodeFlags.DefaultOpen makes the chevron expanded"))
            text("on first render; later frames respect user input.")
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
