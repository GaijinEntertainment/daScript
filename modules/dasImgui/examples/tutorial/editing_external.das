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
require daslib/safe_addr

// =============================================================================
// TUTORIAL: editing_external — the [edit_widget] rail for caller-owned data.
//
// State-managed boost widgets (slider_float, checkbox, combo, …) auto-emit a
// per-call global state struct that owns `.value`. That's fine for new code
// but doesn't help when the data already lives in someone else's struct, a
// daslang global, or a C-side configuration block.
//
// The `edit_*` family takes a caller-owned `T?` pointer instead:
//
//   edit_slider_float(safe_addr(g_vol), (id="VOL", text="Volume",
//                                        v_min=0.0f, v_max=1.0f))
//
// - `safe_addr(local_or_global)` is the safe form for plain locals/globals.
// - `unsafe(addr(struct.field))` is needed when the pointee is a struct
//   field accessed via reference (safe_addr rejects field-via-reference).
// - The `id="…"` literal is REQUIRED — it keys the telemetry path. Missing
//   id is a compile-time error from EditExternalCallMacro.
// - No auto-emit global — the boost wrapper deep-copies *ptr into a hidden
//   mirror for snapshot stability.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/editing_external.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/editing_external.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"EDITOR_WIN/VOL_SLIDER","value":0.8}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//        | jq '.globals."EDITOR_WIN/VOL_SLIDER".payload.value'
// =============================================================================

// Owner-side state — all `var` so the pointee can be written through.
var private g_volume : float = 0.5f
var private g_count : int = 10
var private g_enabled : bool = true
var private g_pos : float3 = float3(0.0f, 0.0f, 0.0f)
var private g_accent : float3 = float3(0.4f, 0.8f, 0.6f)
var private g_quality_idx : int = 1
var private g_angle : float = 0.0f

let private g_qualities : array<string> <- ["Low", "Medium", "High", "Ultra"]

[export]
def init() {
    //! Sized so all seven editors AND their `var private` readouts fit without
    //! scrolling -- the readout lines are the proof each gesture wrote through
    //! the pointer, so they must stay on-screen as the recording drives each one.
    live_create_window("dasImgui editing_external", 760, 560)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.2
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(20.0, 20.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(720.0, 520.0), ImGuiCond.Always)
    window(EDITOR_WIN, (text = "External-data editor", closable = false,
                        flags = ImGuiWindowFlags.None)) {
        text("Every widget below edits a daslang `var private` directly via safe_addr.")

        separator_text("Scalars")

        edit_slider_float(safe_addr(g_volume), (id = "VOL_SLIDER",
                                                text = "Volume",
                                                v_min = 0.0f, v_max = 1.0f))
        edit_input_int(safe_addr(g_count), (id = "COUNT_INPUT",
                                            text = "Count",
                                            step = 1))
        edit_checkbox(safe_addr(g_enabled), (id = "ENABLED_CHECK",
                                             text = "Enabled"))

        separator_text("Vectors")

        edit_drag_float3(safe_addr(g_pos), (id = "POS_DRAG",
                                            text = "Position",
                                            speed = 0.05f))
        edit_color_edit3(safe_addr(g_accent), (id = "ACCENT_COLOR",
                                               text = "Accent"))

        separator_text("Combo and angle")

        edit_combo(safe_addr(g_quality_idx), (id = "QUALITY_COMBO",
                                              text = "Quality",
                                              items <- g_qualities))
        edit_slider_angle(safe_addr(g_angle), (id = "ANGLE_SLIDER",
                                               text = "Angle (rad)",
                                               v_degrees_min = -180.0f,
                                               v_degrees_max = 180.0f))

        separator(SEP_READOUT)
        text("g_volume = {g_volume}")
        text("g_count = {g_count}")
        text("g_enabled = {g_enabled}")
        text("g_pos = ({g_pos.x}, {g_pos.y}, {g_pos.z})")
        text("g_accent = ({g_accent.x}, {g_accent.y}, {g_accent.z})")
        text("g_quality_idx = {g_quality_idx} ({g_qualities[g_quality_idx]})")
        text("g_angle = {g_angle} rad")
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
