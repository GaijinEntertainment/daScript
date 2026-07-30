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
require imgui/imgui_window_constraints_builtin
require imgui/imgui_visual_aids
require math

// =============================================================================
// TUTORIAL: window_size_constraints — lambda-callback overload for
//           ImGui::SetNextWindowSizeConstraints.
//
// The 2-arg form `SetNextWindowSizeConstraints(min, max)` is plain-allowed
// at user-call sites — clamp the window between two corner sizes. The 3-arg
// form takes a `ImGuiSizeCallback`, a C function pointer ImGui invokes on
// every resize so the callback can re-shape the requested size (aspect-ratio
// lock, fixed step quantization, "always square", ...).
//
// The boost module `imgui/imgui_window_constraints_builtin` provides a
// daslang-side `ImGuiSizeConstraints` struct + ctor that wraps a daslang
// lambda; the matching 3-arg overload threads the wrapper through the
// existing C++ trampoline at `src/dasIMGUI.main.cpp:361-384`. End-user code
// can then write:
//
//   let aspect = @ capture(= ratio) (var data : ImGuiSizeCallbackData) : void {
//       data.DesiredSize.y = data.DesiredSize.x / ratio
//   }
//   SetNextWindowSizeConstraints(float2(0,0), float2(FLT_MAX, FLT_MAX),
//                                ImGuiSizeConstraints(aspect))
//
// This tutorial mounts three resizable windows side-by-side — square,
// aspect-16:9, fixed-step-50 — each with a body line printing its current
// (width, height) so resizing visibly snaps to the constraint.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/window_size_constraints.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/window_size_constraints.das
// =============================================================================

// Module-scope holders for the three ImGuiSizeConstraints. ImGui stores the
// callback pointer + user-data for use by the NEXT Begin(); the wrapper
// struct's address must outlive the SetNextWindowSizeConstraints call.
// Stack-locals don't survive long enough — auto-fit invokes the callback
// from inside Begin AFTER the wrapper function has returned. Module
// scope keeps the struct alive for the lifetime of the program.
var private SQUARE_CN : ImGuiSizeConstraints
var private ASPECT_CN : ImGuiSizeConstraints
var private STEP_CN : ImGuiSizeConstraints

[export]
def init() {
    live_create_window("dasImgui window_size_constraints tutorial", 980, 560)
    live_imgui_init(live_window)
    // Relies on FirstUseEver window sizes; disable imgui.ini so the documented
    // layout (and recording) is deterministic every run instead of restoring a
    // prior session. Tutorial-scoped on purpose — not a live_imgui_init change.
    DisableIniPersistence()
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.2

    // Seed the three constraints once at init — daslang lambdas hold their
    // own capture state, so a single per-shape struct is enough for all
    // future Begin() invocations of the matching window.
    SQUARE_CN <- ImGuiSizeConstraints(@(var data : ImGuiSizeCallbackData) : void {
        let s = max(data.DesiredSize.x, data.DesiredSize.y)
        data.DesiredSize = float2(s, s)
    })
    let ratio = 16.0f / 9.0f
    ASPECT_CN <- ImGuiSizeConstraints(@ capture(= ratio) (var data : ImGuiSizeCallbackData) : void {
        data.DesiredSize = float2(data.DesiredSize.x,
                                  float(int(data.DesiredSize.x / ratio)))
    })
    let step = 50.0f
    STEP_CN <- ImGuiSizeConstraints(@ capture(= step) (var data : ImGuiSizeCallbackData) : void {
        let rx = float(int(data.DesiredSize.x / step + 0.5f)) * step
        let ry = float(int(data.DesiredSize.y / step + 0.5f)) * step
        data.DesiredSize = float2(rx, ry)
    })
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    // -------- 1. Square — max(w, h) wins both dims. --------
    SetNextWindowPos(ImVec2(20.0f, 60.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(240.0f, 240.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSizeConstraints(float2(120.0f, 120.0f), float2(FLT_MAX, FLT_MAX),
                                 SQUARE_CN)
    window(SQUARE_WIN, (text = "Square", closable = false,
                        flags = ImGuiWindowFlags.None)) {
        let s = SQUARE_WIN.size
        text(SQUARE_TEXT, (text = "Drag border - always square.\n"
                                + "Size: {int(s.x)} x {int(s.y)}"))
    }

    // -------- 2. Aspect ratio 16:9 — height = width / aspect. --------
    SetNextWindowPos(ImVec2(290.0f, 60.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(320.0f, 180.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSizeConstraints(float2(160.0f, 0.0f), float2(FLT_MAX, FLT_MAX),
                                 ASPECT_CN)
    window(ASPECT_WIN, (text = "Aspect 16:9", closable = false,
                        flags = ImGuiWindowFlags.None)) {
        let s = ASPECT_WIN.size
        text(ASPECT_TEXT, (text = "Drag border - height tracks width / 16:9.\n"
                                + "Size: {int(s.x)} x {int(s.y)}"))
    }

    // -------- 3. Fixed step 50 — both dims snap to nearest 50 px. --------
    SetNextWindowPos(ImVec2(640.0f, 60.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(300.0f, 250.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSizeConstraints(float2(100.0f, 100.0f), float2(FLT_MAX, FLT_MAX),
                                 STEP_CN)
    window(STEP_WIN, (text = "Step 50", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        let s = STEP_WIN.size
        text(STEP_TEXT, (text = "Drag border - both dims snap to 50 px.\n"
                              + "Size: {int(s.x)} x {int(s.y)}"))
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
