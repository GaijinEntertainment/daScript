options gen2
options _comment_hygiene = true

require imgui
require imgui_app
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live/opengl_live // nolint:STYLE030 - registers screenshot/record_* live commands for the recorder
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_visual_aids
require imgui/imgui_icons

// =============================================================================
// TUTORIAL: imgui_icons — an asset-free vector icon set.
//
// Every glyph is geometry drawn straight onto the ImGui draw list (no SVG, no
// texture atlas), tinted from the daslang amber accent. Two ways to use it:
//
//   icon_button(IDENT, (glyph = "play", tip = "Play"))   — a clickable, themed
//                                                           icon button. Uses
//                                                           ClickState like any
//                                                           button; returns true
//                                                           on click.
//   icon("waveform", 24.0f)                               — draw a glyph at the
//                                                           cursor (no interaction)
//                                                           and advance the cursor.
//
// Glyphs are addressed by name. Enumerate the whole set with icon_count() /
// icon_name(i) / icon_category(i) — e.g. to build an icon picker. The full
// catalog (with images) is in the docs: stdlib -> Builtin widgets -> Icon set.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/icons.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/icons.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"IC_WIN/IC_PLAY"}}' \
//        localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui icons tutorial", 680, 560)
    live_imgui_init(live_window)
    var style & = unsafe(GetStyle())
    style.FontScaleMain = 1.4f
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(640.0f, 520.0f), ImGuiCond.Always)
    window(IC_WIN, (text = "icons tutorial", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        text("Vector icons drawn on the draw list - no atlas, no asset files.")
        separator()

        // ---- Stage 1: icon_button — a clickable transport bar ----
        text(IC_HINT, (text = "icon_button: clickable, themed, addressed by glyph name."))
        icon_button(IC_PLAY,  (glyph = "play",   tip = "Play"));   same_line(IC_SL1)
        icon_button(IC_PAUSE, (glyph = "pause",  tip = "Pause"));  same_line(IC_SL2)
        icon_button(IC_STOP,  (glyph = "stop",   tip = "Stop"));   same_line(IC_SL3)
        icon_button(IC_REC,   (glyph = "record", tip = "Record"))
        text("clicks: play={IC_PLAY.click_count} pause={IC_PAUSE.click_count} stop={IC_STOP.click_count} rec={IC_REC.click_count}")
        spacing()

        // ---- Stage 2: tip + the rest of an edit toolbar ----
        text(IC_HINT2, (text = "A non-empty tip shows a hover tooltip - hover any glyph."))
        icon_button(IC_UNDO, (glyph = "undo", tip = "Undo")); same_line(IC_SL4)
        icon_button(IC_REDO, (glyph = "redo", tip = "Redo")); same_line(IC_SL5)
        icon_button(IC_CUT,  (glyph = "cut",  tip = "Cut"));  same_line(IC_SL6)
        icon_button(IC_SAVE, (glyph = "save", tip = "Save"))
        spacing()
        separator()

        // ---- Stage 3: icon() — draw-only, any size ----
        text(IC_DISP, (text = "icon(name, size): draw a glyph inline, no interaction. Sizes scale freely:"))
        icon("waveform", 20.0f); same_line()
        icon("waveform", 32.0f); same_line()
        icon("waveform", 48.0f)
        spacing()
        text("The set spans transport, edit, mix, synth, scene and chrome glyphs:")
        icon("filter", 28.0f);    same_line()
        icon("knob", 28.0f);      same_line()
        icon("wave-sine", 28.0f); same_line()
        icon("layers", 28.0f);    same_line()
        icon("camera", 28.0f);    same_line()
        icon("gear", 28.0f)
        spacing()
        separator()

        // ---- Stage 4: enumerate the catalog ----
        text("icon_count()/icon_name(i)/icon_category(i) enumerate the set:")
        text("{icon_count()} glyphs - e.g. [0] '{icon_name(0)}' in '{icon_category(0)}'.")
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
