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
require daslib/json public
require daslib/json_boost public

// =============================================================================
// FEATURE: widget_init_defaults — exercises [widget] auto-emit's invocation
//          of the state struct's default constructor, so source-declared
//          field defaults show up on the auto-emitted global.
// SHOWS:   numeric / string / vector / enum field defaults firing through
//          ExprMakeStruct(useInitializer) on the macro-emitted global.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/widget_init_defaults.das
// LIVE:       daslang-live modules/dasImgui/examples/features/widget_init_defaults.das
// =============================================================================

// State struct with one default per common field shape. Distinct from zero-init
// values so the assertion can prove the constructor actually ran (count != 0,
// ratio != 0, label != "", color != float3(), mode != 0).
struct DefaultsState {
    @live count : int = 42
    @live ratio : float = 1.5f
    @live caption : string = "default-label"
    @live color : float3 = float3(0.5f, 0.6f, 0.7f)
    @live mode : ImGuiButtonFlags = ImGuiButtonFlags.MouseButtonRight
    @optional pending_bump : bool                                       // imgui_set L3 -> bump count next frame
    @optional has_pending : bool
}

[widget]
def show_defaults(var state : DefaultsState; text : string) : int {
    if (state.has_pending) {
        state.count += 1
        state.has_pending = false
    }
    Text(text)
    Text("count={state.count} ratio={state.ratio} caption={state.caption}")
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue?) {
            if (action == "set") {
                state.pending_bump = true
                state.has_pending = true
            }
        }
        widget_finalize(widget_ident, "show_defaults", entry, ser, disp)
    }
    return state.count
}

[export]
def init() {
    live_create_window("widget_init_defaults", 800, 600)
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

    SetNextWindowSize(ImVec2(720.0, 320.0), ImGuiCond.Always)
    if (Begin("widget_init_defaults", null, ImGuiWindowFlags.None)) {
        Text("Default-init smoke test. Snapshot to inspect DEFAULTS payload.")
        show_defaults(DEFAULTS, (text = "show_defaults"))
    }
    End()

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
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
