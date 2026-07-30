options gen2
options _comment_hygiene = true

require imgui/imgui_harness
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
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/widget_init_defaults.das -- --headless --headless-frames=60
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
    @optional pending_bump : bool                                       // imgui_force_set L3 -> bump count next frame
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
        widget_finalize(widget_ident, "show_defaults", entry,
                        addr(state),
                        reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<DefaultsState>)))
    }
    return state.count
}

[widget_dispatch]
def show_defaults(var state : DefaultsState; action : string; payload : string) {
    if (action == "set") {
        state.pending_bump = true
        state.has_pending = true
    }
}

[export]
def init() {
    harness_init("widget_init_defaults", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 320.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "widget_init_defaults", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Default-init smoke test. Snapshot to inspect DEFAULTS payload.")
        show_defaults(DEFAULTS, (text = "show_defaults"))
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
