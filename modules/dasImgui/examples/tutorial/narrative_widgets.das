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
// TUTORIAL: narrative_widgets — boost v2 read-only display widgets.
//
// All eight share a NarrativeState (or LabelTextState) payload: the call-site
// string echoes back into the snapshot so playwright tests can assert "the
// label says what I expect". No focus, no click, no telemetry beyond `value`.
//
// Covers: text / text_unformatted / text_wrapped / text_colored / text_disabled
//         / bullet / bullet_text / label_text / separator_text
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/narrative_widgets.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/narrative_widgets.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//        | jq '.globals."NARRATIVE_WIN/BULLET_MARK".payload.value'
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui narrative widgets", 760, 600)
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

    SetNextWindowPos(ImVec2(60.0, 60.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(640.0, 480.0), ImGuiCond.Always)
    window(NARRATIVE_WIN, (text = "Narrative widgets", closable = false,
                           flags = ImGuiWindowFlags.None)) {
        // Each call passes an explicit ident so the widget registers at
        // NARRATIVE_WIN/<ident> and a snapshot can assert "the label says what
        // I expect". NarrativeState.value mirrors the call-site string.
        text(PLAIN_LINE, (text = "text() - one line, telemetry-visible."))

        // text_wrapped reflows to the window's content edge.
        text_wrapped(WRAPPED_LINE, (text = "text_wrapped() reflows long strings to the window's right edge. Resize the window and the layout follows; the snapshot still carries the full source string regardless of wrapping."))

        separator_text(SEP_COLOR, (text = "Color and emphasis"))

        // Colored / disabled - visual variants, same payload shape.
        text_colored(COLORED_LINE, (color = float4(0.7f, 0.9f, 0.4f, 1.0f),
                                    text = "text_colored() with an inline float4 color."))
        text_disabled(DISABLED_LINE, (text = "text_disabled() - greyed for non-actionable hints."))

        separator_text(SEP_BULLET, (text = "Bulleted lines"))

        // bullet() emits the marker glyph; bullet_text() bundles marker + text.
        bullet(BULLET_MARK)
        bullet_text(BULLET_FIRST, (text = "First point of interest."))
        bullet_text(BULLET_SECOND, (text = "Second point - bullet glyph plus text in one call."))

        separator_text(SEP_LABEL, (text = "Label/value pair"))

        // label_text - two-string display, both strings echo into the payload.
        label_text(VERSION_LABEL, (key = "Version", value = "v2.0-detour"))
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
