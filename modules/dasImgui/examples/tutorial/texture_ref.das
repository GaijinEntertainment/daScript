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
require stbimage/stbimage_boost
require daslib/safe_addr

// =============================================================================
// TUTORIAL: texture_ref — display YOUR OWN texture via the imgui 1.92 ImTextureRef.
//
// 1.92 replaced the raw `ImTextureID` handle with `ImTextureRef`. To show a real
// image (not the font atlas), the steps are:
//
//   1. decode a PNG to RGBA pixels      — dasStbImage `Image::load(path, 4)`
//   2. upload to a GL texture           — glGenTextures + glTexImage2D
//   3. wrap the GL handle               — var ref : ImTextureRef; ref._TexID = handle
//   4. draw it                          — image(IDENT, (user_texture_id = ref, ...))
//
// `_TexID` is the user slot of ImTextureRef (an ImTextureID == the GL texture name
// here). With `_TexData` left null, the backend treats it as a user-managed texture
// and binds the GL id directly — that is how you pass your own texture in 1.92.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/texture_ref.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/texture_ref.das
// =============================================================================

// One of the repo's icon PNGs stands in for an application texture.
let PICTURE_PATH = "{get_das_root()}/modules/dasImgui/doc/source/_static/icons/cube.png"

var private g_tex : uint = 0u            // GL texture name
var private g_tex_w = 0
var private g_tex_h = 0
var private g_load_err = ""

def private upload_picture() {
    var img : Image
    let (ok, err) = img->load(PICTURE_PATH, 4)   // request 4 channels => RGBA
    if (!ok) {
        g_load_err = "load failed: {err}"
        return
    }
    g_tex_w = img.width
    g_tex_h = img.height
    glGenTextures(1, safe_addr(g_tex))
    glBindTexture(GL_TEXTURE_2D, g_tex)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    glTexImage2D(GL_TEXTURE_2D, 0, int(GL_RGBA), img.width, img.height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, unsafe(addr(img.bytes[0])))
}

// Build the 1.92 image() handle from the GL texture name. _TexID is the user
// slot (ImTextureID == ImU64 == uint64 here); _TexData stays null, so the
// backend binds the GL id directly as a user-managed texture.
def private texture_ref() : ImTextureRef {
    unsafe {
        var ref : ImTextureRef
        ref._TexID = uint64(g_tex)
        return ref
    }
}

[export]
def init() {
    live_create_window("dasImgui texture_ref tutorial", 820, 520)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.3
    // GL context is live now — decode + upload the picture once.
    upload_picture()
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(40.0f, 40.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(740.0f, 440.0f), ImGuiCond.Always)
    window(TR_WIN, (text = "texture_ref", closable = false,
                    flags = ImGuiWindowFlags.None)) {
        if (g_tex == 0u) {
            text(TR_ERR, (text = "no texture: {g_load_err}"))
        } else {
            text("A real PNG, decoded + uploaded to a GL texture, shown through")
            text("an ImTextureRef whose _TexID is the GL texture name.")
            separator()

            // Full image, then a tinted + bordered copy — same texture reused.
            image(TR_PIC, (user_texture_id = texture_ref(),
                           size = float2(float(g_tex_w), float(g_tex_h)),
                           uv0 = float2(0.0f, 0.0f),
                           uv1 = float2(1.0f, 1.0f),
                           tint_col = float4(1.0f, 1.0f, 1.0f, 1.0f),
                           border_col = float4(0.6f, 0.6f, 0.6f, 1.0f)))
            same_line(TR_GAP)
            image(TR_TINT, (user_texture_id = texture_ref(),
                            size = float2(float(g_tex_w), float(g_tex_h)),
                            uv0 = float2(0.0f, 0.0f),
                            uv1 = float2(1.0f, 1.0f),
                            tint_col = float4(0.4f, 0.8f, 1.0f, 1.0f),
                            border_col = float4(1.0f, 1.0f, 1.0f, 1.0f)))
            separator()
            text("{g_tex_w}x{g_tex_h}. Swap PICTURE_PATH / _TexID for your own texture.")
        }
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
    if (g_tex != 0u) {
        glDeleteTextures(1, safe_addr(g_tex))
    }
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
