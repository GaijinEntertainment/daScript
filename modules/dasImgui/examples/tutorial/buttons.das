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
// TUTORIAL: buttons — six shapes of click trigger.
//
//   button(IDENT, (text = ".."))                       — the workhorse.
//   small_button(IDENT, (text = ".."))                 — no frame padding,
//                                                        for inline tools.
//   arrow_button(IDENT, (text = "..", dir = ...))      — triangular glyph,
//                                                        ImGuiDir.{Up,Down,
//                                                        Left,Right}.
//   invisible_button(IDENT, (text = "..", size = ...)) — hit area without
//                                                        rendering — overlay
//                                                        a clickable region
//                                                        on a drawlist scene.
//   image_button(IDENT, (text = "..",                  — textured button. The
//                        user_texture_id = tex_id,        font atlas works as
//                        size = ...))                     a no-setup texture.
//   tab_item_button(IDENT, (text = "..",               — button styled as a
//                           flags = ...))                 tab — MUST be called
//                                                        INSIDE a tab_bar.
//
// All six use `ClickState`: `state.clicked` is the per-frame click bool,
// `state.click_count` accumulates total clicks.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/buttons.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/buttons.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"BT_WIN/BT_GO"}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"BT_WIN/BAR/BT_ADD"}}' \
//        localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui buttons tutorial", 800, 720)
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
    SetNextWindowSize(ImVec2(760.0f, 680.0f), ImGuiCond.Always)
    window(BT_WIN, (text = "buttons tutorial", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        text("Six shapes — ClickState everywhere; state.click_count accumulates.")
        text(BT_HINT, (text = "Plain / small / arrow / invisible / image / tab_item — pick by visual fit."))
        separator()

        // ---- Stage 1: button — the workhorse ----
        if (button(BT_GO, (text = "Go"))) {
            // Hook your click handler here.
        }
        text("BT_GO clicks = {BT_GO.click_count}")
        spacing()

        // ---- Stage 2: small_button — no padding, inline tool ----
        small_button(BT_SMALL, (text = "quit"))
        text("BT_SMALL clicks = {BT_SMALL.click_count}  // SmallButton has zero frame padding")
        spacing()

        // ---- Stage 3: arrow_button — directional pair ----
        arrow_button(BT_BACK, (text = "##back", dir = ImGuiDir.Left)); same_line(BT_SL1)
        arrow_button(BT_FWD,  (text = "##fwd",  dir = ImGuiDir.Right))
        text("nav clicks: back={BT_BACK.click_count}, fwd={BT_FWD.click_count}")
        spacing()

        // ---- Stage 4: invisible_button — hit area without a glyph ----
        text("Invisible hotspot below (120×24, no glyph):")
        invisible_button(BT_HOT, (text = "##hotspot", size = float2(120.0f, 24.0f)))
        text("BT_HOT clicks = {BT_HOT.click_count}  // overlay on a drawlist / image / etc.")
        spacing()

        // ---- Stage 5: image_button — textured button (font atlas) ----
        let io & = unsafe(GetIO())
        if (io.Fonts.TexData != null) {
            image_button(BT_IMG, (text = "##font",
                                  user_texture_id = io.Fonts.TexRef,
                                  size = float2(48.0f, 48.0f)))
            text("BT_IMG clicks = {BT_IMG.click_count}  // textured 48×48 of the font atlas")
        }
        spacing()

        // ---- Stage 6: tab_item_button — must live inside a tab_bar ----
        text("tab_item_button — leading '?' tallies, trailing '+' grows the bar:")
        tab_bar(BT_BAR, (text = "MyBar",
                          flags = ImGuiTabBarFlags.FittingPolicyShrink)) {
            tab_item_button(BT_HELP, (text = "?",
                                       flags = ImGuiTabItemFlags.Leading |
                                               ImGuiTabItemFlags.NoTooltip))
            tab_item_button(BT_ADD, (text = "+",
                                      flags = ImGuiTabItemFlags.Trailing |
                                              ImGuiTabItemFlags.NoTooltip))
            tab_item(BT_TAB_A, (text = "alpha", closable = false,
                                 flags = ImGuiTabItemFlags.None)) {
                text(BT_BODY_A, (text = "tab alpha body"))
            }
            tab_item(BT_TAB_B, (text = "beta", closable = false,
                                 flags = ImGuiTabItemFlags.None)) {
                text(BT_BODY_B, (text = "tab beta body"))
            }
        }
        text("BT_HELP clicks = {BT_HELP.click_count}, BT_ADD clicks = {BT_ADD.click_count}")
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
