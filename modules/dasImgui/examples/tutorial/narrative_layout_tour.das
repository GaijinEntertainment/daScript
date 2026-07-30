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
// TUTORIAL: narrative_layout_tour — one-window tour of read-only families.
//
// A single panel that shows narrative + layout + display + flat-tooltip
// widgets together, sized for an APNG recording. Paired driver:
//
//   modules/dasImgui/tests/integration/record_narrative_layout_tour.das
//
// All idents are intentionally short and stable so the driver can grab their
// bboxes by name. The driver narrates each stage with imgui_narrate; this
// tutorial only renders the widgets, no animation.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/narrative_layout_tour.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/narrative_layout_tour.das
// =============================================================================

var private g_progress : float = 0.0f

[export]
def init() {
    //! Window sized to keep the APNG recording under GitHub's 50 MB
    //! warning band. Bumping past 720x480 doubles the file size for the
    //! same story.
    live_create_window("dasImgui narrative+layout tour", 720, 480)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.15
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    g_progress += 0.005f
    if (g_progress > 1.0f) {
        g_progress--
    }

    SetNextWindowPos(ImVec2(20.0, 20.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(680.0, 440.0), ImGuiCond.Always)
    window(TOUR_WIN, (text = "Narrative + layout tour",
                      closable = false, flags = ImGuiWindowFlags.None)) {
        // ---- Stage 1: plain text ----
        text(INTRO, (text = "Plain text() - one line, telemetry-visible."))

        // ---- Stage 2: wrapped text ----
        text_wrapped(WRAP, (text = "text_wrapped() reflows to the window's right edge. Long strings flow naturally; the snapshot carries the full source string regardless of how the renderer wraps it."))

        // ---- Stage 3: colored / disabled ----
        text_colored(COLORED, (color = float4(0.7f, 0.9f, 0.4f, 1.0f),
                               text = "text_colored() with an inline float4."))
        text_disabled(DISABLED, (text = "text_disabled() - greyed for hints."))

        // ---- Stage 4: bullet ----
        bullet_text(BULLET_LINE, (text = "bullet_text() - marker + text in one call."))

        // ---- Stage 5: label/value pair ----
        label_text(LABEL_VERSION, (key = "Version", value = "v2.0-detour"))

        // ---- Stage 6: separator_text + spacing + dummy ----
        separator_text(SEP_TITLE, (text = "Display + tooltips"))
        dummy(SEP_DUMMY, (size = float2(0.0f, 8.0f)))

        // ---- Stage 7: progress_bar ----
        progress_bar(PROG_BAR, (fraction = g_progress,
                                size = float2(-1.0f, 0.0f),
                                overlay = ""))

        spacing(SP_BELOW)

        // ---- Stage 8: flat tooltip on a hover target ----
        if (button(HOVER_TIP, (text = "Hover me"))) {
            print("clicked\n")
        }
        set_item_tooltip(HOVER_TIP_TOOL, (text = "set_item_tooltip - fires on previous-item hover."))
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
