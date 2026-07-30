options gen2
options _comment_hygiene = true
options rtti = true

// =============================================================================
// make_icon_doc — render every glyph in the imgui_icons set to a per-icon PNG
// under doc/source/_static/icons/<name>.png. The single regeneration entry
// point for the icon catalog: the doc generator (utils/imgui2rst) and the
// `:icon:` Sphinx role reference the PNGs this writes; re-run after adding an
// icon. Drives a real ImGui frame and reads back the framebuffer (faithful
// pixels), so it must run WINDOWED — not headless (no GL).
//
//   daslang.exe -project_root <dasimgui> utils/make_icon_doc.das
// =============================================================================

require imgui
require imgui_app
require opengl/opengl_boost
require opengl/opengl
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
require imgui/imgui_icons
require stbimage
require daslib/fio
require daslib/rtti
require daslib/ast

let WIN_PX = 128   // above the Windows min window width so the framebuffer stays square
let ICON_PX = 72.0f
let BG = float4(0.0f, 0.0f, 0.0f, 0.0f)   // transparent attempt; falls back if FB has no alpha

def icons_out_dir() : string {
    let m = find_module("imgui_icons")
    assert(m != null, "imgui_icons not loaded - wrong project_root?")
    return dir_name(dir_name(string(m.fileName))) + "/doc/source/_static/icons"
}

def capture_png(fname : string) {
    var w, h : int
    live_get_framebuffer_size(w, h)
    if (w <= 0 || h <= 0) {
        return
    }
    var pixels : array<uint8>
    pixels |> resize(w * h * 4)
    unsafe {
        glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, addr(pixels[0]))
    }
    // OpenGL origin is bottom-left; flip rows so the PNG is top-down.
    let row = w * 4
    var tmp : array<uint8>
    tmp |> resize(row)
    for (y in range(h / 2)) {
        let t = y * row
        let b = (h - 1 - y) * row
        unsafe {
            memcpy(addr(tmp[0]), addr(pixels[t]), row)
            memcpy(addr(pixels[t]), addr(pixels[b]), row)
            memcpy(addr(pixels[b]), addr(tmp[0]), row)
        }
    }
    unsafe {
        stbi_write_png(fname, w, h, 4, addr(pixels[0]), row)
    }
    delete pixels
    delete tmp
}

def render_icon_frame(name : string; capture_to : string) {
    if (!live_begin_frame()) {
        return
    }
    begin_frame()
    ImGui_ImplGlfw_NewFrame()
    NewFrame()

    SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(float(WIN_PX), float(WIN_PX)), ImGuiCond.Always)
    let flags = (ImGuiWindowFlags.NoTitleBar | ImGuiWindowFlags.NoResize |
        ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoScrollbar |
        ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.NoBackground |
        ImGuiWindowFlags.NoSavedSettings | ImGuiWindowFlags.NoBringToFrontOnFocus)
    window(ICON_WIN, (text = "##icon", closable = false, flags = flags)) {
        let avail = GetContentRegionAvail()
        let cur = GetCursorPos()
        SetCursorPos(ImVec2(cur.x + (avail.x - ICON_PX) * 0.5f,
                            cur.y + (avail.y - ICON_PX) * 0.5f))
        icon(name, ICON_PX)
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(BG.x, BG.y, BG.z, BG.w)
    glClear(GL_COLOR_BUFFER_BIT)
    live_imgui_render()
    // Read back BEFORE the swap: the just-rendered icon is in the back buffer.
    if (!empty(capture_to)) {
        capture_png(capture_to)
    }
    live_end_frame()
}

[export]
def main() {
    live_create_window("dasImgui icon doc renderer", WIN_PX, WIN_PX)
    live_imgui_init(live_window)

    let out = icons_out_dir()
    mkdir(out)
    to_log(LOG_INFO, "rendering {icon_count()} icons -> {out}\n")

    // Warm up so ImGui settles the window + font atlas before the first capture.
    for (_w in range(3)) {
        render_icon_frame("play", "")
    }

    for (i in range(icon_count())) {
        let nm = icon_name(i)
        render_icon_frame(nm, "")                  // settle this glyph
        render_icon_frame(nm, out + "/" + nm + ".png")  // capture
        to_log(LOG_INFO, "  [{i + 1}/{icon_count()}] {nm}\n")
    }

    live_imgui_shutdown()
    live_destroy_window()
    to_log(LOG_INFO, "done\n")
}
