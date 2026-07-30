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

var private SCROLL_ROW : table<int; NarrativeState>

// nolint:STYLE014 — file-header tutorial banner displaced below a var decl
// =============================================================================
// TUTORIAL: child — BeginChild/EndChild sub-windows.
//
//   child(IDENT, (text = "..", size = float2(w, h),
//                 child_flags = ..., window_flags = ...)) { body }
//
// child is the workhorse for nesting a scrollable, optionally-bordered region
// inside another window. Three knobs:
//
//   1. size — float2(w, h). (0,0) auto-fits to the available row/column
//      depending on flags. Positive = explicit pixels. Negative = "fill the
//      remainder, minus N pixels".
//
//   2. child_flags — ImGui's child-specific bitfield: Border, AlwaysAutoResize,
//      AutoResizeX/Y, FrameStyle, NavFlattened, ResizeX/Y.
//
//   3. window_flags — same flags every other window takes. Scrollbar control
//      lives here: HorizontalScrollbar, AlwaysVerticalScrollbar, etc.
//
// ChildState observed each frame: scroll (float2 current), scroll_max (float2
// max). The body's render runs only while BeginChild returned true; scroll is
// captured INSIDE that gate so a hidden child reports the last-known values.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/child.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/child.das
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui child tutorial", 800, 540)
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
    SetNextWindowSize(ImVec2(760.0f, 500.0f), ImGuiCond.Always)
    window(CHILD_WIN, (text = "child tutorial", closable = false,
                       flags = ImGuiWindowFlags.None)) {

        text("Three child variants - one each for borders, autosize, and h-scroll.")
        separator()

        // ---- A: bordered, fixed-size, vertical scroll ----
        text("A) Bordered fixed-size with vertical scroll")
        child(SCROLL_A, (text = "scroll_a",
                         size = float2(360.0f, 140.0f),
                         child_flags = ImGuiChildFlags.Borders,
                         window_flags = ImGuiWindowFlags.None)) {
            for (i in range(20)) {
                text(SCROLL_ROW[i], (text = "row {i} - content overflows the visible region"))
            }
        }
        text("scroll.y = {SCROLL_A.scroll.y} / {SCROLL_A.scroll_max.y}")
        spacing()

        // ---- B: auto-resize Y, FrameStyle (input-group look) ----
        text("B) AutoResizeY + FrameStyle - height tracks content")
        child(AUTO_B, (text = "auto_b",
                       size = float2(360.0f, 0.0f),
                       child_flags = ImGuiChildFlags.AutoResizeY | ImGuiChildFlags.FrameStyle,
                       window_flags = ImGuiWindowFlags.None)) {
            text("Three rows of input chrome")
            checkbox(B_VSYNC, (text = "VSync"))
            slider_int(B_LIMIT, (text = "frame limit"))
        }
        // AutoResizeY trims overflow to zero, so scroll = (0,0) and
        // scroll_max = (0,0). The child's actual size lives in size.y;
        // print scroll fields directly to show "no overflow".
        text("AUTO_B.scroll = ({AUTO_B.scroll.x}, {AUTO_B.scroll.y}); scroll_max = ({AUTO_B.scroll_max.x}, {AUTO_B.scroll_max.y})")
        spacing()

        // ---- C: bordered, horizontal scroll ----
        text("C) Border + horizontal scroll - wide content")
        child(SCROLL_C, (text = "scroll_c",
                        size = float2(720.0f, 90.0f),
                        child_flags = ImGuiChildFlags.Borders,
                        window_flags = ImGuiWindowFlags.HorizontalScrollbar)) {
            // One long line — forces horizontal scroll because no wrap.
            text(LONG_LINE, (text = "very wide content: lorem ipsum dolor sit amet consectetur adipiscing elit sed do eiusmod tempor incididunt ut labore et dolore magna aliqua ut enim ad minim veniam quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo"))
            text("(scroll horizontally to read; scroll.x mirrors below)")
        }
        text("scroll.x = {SCROLL_C.scroll.x} / {SCROLL_C.scroll_max.x}")
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
