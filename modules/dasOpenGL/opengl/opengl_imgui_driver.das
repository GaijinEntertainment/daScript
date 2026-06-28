options gen2
options indenting = 4

// A Dear ImGui renderer backend (the dasOpenGL<->dasImgui bridge), written entirely in daslang -- no
// C++. ImGui is API-agnostic: each frame it produces an `ImDrawData` (vertex/index buffers + draw
// commands + clip rects + texture refs), and a "backend" is just a driver that replays that into a
// graphics API. dasImgui exposes the whole ImDrawData surface to daslang and dasOpenGL exposes the GL
// primitives, so the driver is pure das: upload each command list's vertex/index buffers, walk the
// draw commands, set the per-command scissor, bind the command's texture, and issue an indexed draw.
// This is the OpenGL twin of dasVulkan's vulkan_imgui_driver and replaces stock imgui_impl_opengl3.
//
// ImGui 1.92's dynamic-texture model is honored: each frame we drain the platform texture queue,
// creating GL textures on demand and handing ImGui back an `ImTextureID` (the GL texture name) via
// SetTexID.
//
// The renderer's own shaders are authored in daslang and lowered to GLSL by the [vertex_program] /
// [fragment_program] macros -- the exact equivalent of the stock imgui_impl_opengl3 glsl.

module opengl_imgui_driver shared

require opengl/opengl_boost public
require glsl/glsl_opengl public
require daslib/safe_addr
require imgui

// ===== shaders (daslang -> GLSL) =====
// Vertex maps screen-space ImDrawVert positions into GL clip space via scale/translate (the ImGui
// ortho projection collapsed: scale = 2/DisplaySize with Y negated, translate places the top-left at
// (-1,+1)); fragment modulates the per-vertex color by the bound texture. GL clip space is +Y up while
// ImGui screen space is +Y down, so scale.y is negative.

var @in @location = 0 i_pos : float2
var @in @location = 1 i_uv  : float2
var @in @location = 2 i_col : float4
var @inout io_col : float4
var @inout io_uv  : float2
var @uniform u_scale     : float2
var @uniform u_translate : float2

[vertex_program]
def imgui_gl_vs {
    io_col = i_col
    io_uv = i_uv
    gl_Position = float4(i_pos * u_scale + u_translate, 0.0, 1.0)
}

var @uniform u_tex : sampler2D
var @out o_col : float4

[fragment_program]
def imgui_gl_fs {
    o_col = io_col * texture(u_tex, io_uv)
}

// ===== renderer =====

// sizeof(ImDrawVert) = ImVec2 pos (8) + ImVec2 uv (8) + ImU32 col (4); ImDrawIdx = unsigned short.
let private VERT_SIZE = 20
let private IDX_SIZE  = 2

struct public ImguiGlRenderer {
    program  : uint
    vao      : uint
    vbo      : uint
    ebo      : uint
    textures : array<uint>      // GL texture names we created (== the ImTextureIDs), freed at finalize
    is_web   : bool             // WebGL2 (emscripten) lacks glDrawElementsBaseVertex; pick the draw at create
}

//! Build the renderer: compile+link the imgui shaders, create the VAO and the streaming vertex/index
//! buffers, and record the ImDrawVert attribute layout (pos/uv as float, packed RGBA8 color as a
//! normalized unsigned-byte vec4) into the VAO.
def public imgui_gl_create : ImguiGlRenderer {
    // get_running_platform_name() (unlike get_platform_name(), which const-folds to the COMPILE host)
    // is a real runtime call, so it reports "emscripten" both for the wasm interpreter and for a
    // cross-compiled exe executing in the browser -> the right WebGL2 branch in both.
    var r = ImguiGlRenderer(is_web = get_running_platform_name() == "emscripten")
    r.program = create_shader_program(@@imgui_gl_vs, @@imgui_gl_fs)
    glGenVertexArrays(1, safe_addr(r.vao))
    glGenBuffers(1, safe_addr(r.vbo))
    glGenBuffers(1, safe_addr(r.ebo))
    glBindVertexArray(r.vao)
    glBindBuffer(GL_ARRAY_BUFFER, r.vbo)
    glEnableVertexAttribArray(0u)
    glVertexAttribPointer(0u, 2, GL_FLOAT, false, VERT_SIZE, 0)
    glEnableVertexAttribArray(1u)
    glVertexAttribPointer(1u, 2, GL_FLOAT, false, VERT_SIZE, 8)
    glEnableVertexAttribArray(2u)
    glVertexAttribPointer(2u, 4, GL_UNSIGNED_BYTE, true, VERT_SIZE, 16)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.ebo)
    glBindVertexArray(0u)
    return <- r
}

def public finalize(var r : ImguiGlRenderer) {
    for (t in r.textures) {
        var tt = t
        glDeleteTextures(1, safe_addr(tt))
    }
    delete r.textures
    // safe_addr needs a local; r is a by-ref param, so its fields aren't local values
    var ebo = r.ebo
    var vbo = r.vbo
    var vao = r.vao
    glDeleteBuffers(1, safe_addr(ebo))
    glDeleteBuffers(1, safe_addr(vbo))
    glDeleteVertexArrays(1, safe_addr(vao))
    glDeleteProgram(r.program)
}

//! Create a GL texture for one ImTextureData (RGBA8) and register it; returns the ImTextureID (the GL
//! texture name as a u64).
def private create_imgui_texture(var r : ImguiGlRenderer; tex : ImTextureData?) : uint64 {
    var gltex : uint
    glGenTextures(1, safe_addr(gltex))
    glBindTexture(GL_TEXTURE_2D, gltex)
    // RGBA8 rows are 4-aligned, but save/restore UNPACK_ALIGNMENT so we don't leak it to app GL code.
    var prev_align : int
    glGetIntegerv(GL_UNPACK_ALIGNMENT, safe_addr(prev_align))
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
    glTexImage2D(GL_TEXTURE_2D, 0, int(GL_RGBA), tex.Width, tex.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        unsafe(reinterpret<void?>(tex.Pixels)))
    glPixelStorei(GL_UNPACK_ALIGNMENT, prev_align)
    r.textures |> push(gltex)
    return uint64(gltex)
}

//! Re-upload an already-created texture's pixels (ImGui 1.92 bakes glyphs on demand and flags the atlas
//! WantUpdates -- without this, on-demand glyphs render blank). Full-rect re-upload of the current
//! pixels; atlas updates are infrequent so the cost is negligible.
def private update_imgui_texture(tex : ImTextureData?) {
    let th & = unsafe(*tex)        // GetTexID is a method -- needs a value, not the pointer
    glBindTexture(GL_TEXTURE_2D, uint(th.GetTexID()))
    var prev_align : int
    glGetIntegerv(GL_UNPACK_ALIGNMENT, safe_addr(prev_align))
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex.Width, tex.Height, GL_RGBA, GL_UNSIGNED_BYTE,
        unsafe(reinterpret<void?>(tex.Pixels)))
    glPixelStorei(GL_UNPACK_ALIGNMENT, prev_align)
}

//! Drain ImGui's per-frame texture queue: create GL textures ImGui asked for (font atlas, user images)
//! and hand back their ids. Call once per frame after ImGui::Render, before imgui_gl_render.
def public imgui_gl_update_textures(var r : ImguiGlRenderer) {
    // `ImDrawData.Textures` is a *pointer* to the shared texture list; the embedded list lives on
    // `GetPlatformIO().Textures` -- the canonical place a backend drains texture create/update requests.
    let pio & = unsafe(GetPlatformIO())
    for (i in range(length(pio.Textures))) {
        // const-strip: `pio` is const, so the element is a const-pointee pointer; SetTexID/SetStatus
        // below need a mutable ImTextureData.
        var tex = unsafe(reinterpret<ImTextureData? -const>(pio.Textures[i]))
        if (tex == null) {
            continue
        }
        if (tex.Status == ImTextureStatus.WantCreate) {
            let tid = create_imgui_texture(r, tex)
            var th & = unsafe(*tex)
            th |> SetTexID(tid)
            th |> SetStatus(ImTextureStatus.OK)
        } elif (tex.Status == ImTextureStatus.WantUpdates) {
            update_imgui_texture(tex)
            var th & = unsafe(*tex)
            th |> SetStatus(ImTextureStatus.OK)
        }
        // WantDestroy is left for renderer finalize (the font atlas isn't destroyed mid-run); a churny
        // user-texture workload would want explicit glDeleteTextures + ack here.
    }
}

//! Per-frame GL state for ImGui: alpha blend, no cull/depth/stencil, scissor enabled, the imgui
//! program with its scale/translate + sampler uniforms bound, and the VAO selected.
def private setup_render_state(r : ImguiGlRenderer; ds, dp : float2; fb_w, fb_h : int) {
    glEnable(GL_BLEND)
    glBlendEquation(GL_FUNC_ADD)
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, uint(GL_ONE), GL_ONE_MINUS_SRC_ALPHA)
    glDisable(GL_CULL_FACE)
    glDisable(GL_DEPTH_TEST)
    glDisable(GL_STENCIL_TEST)
    glEnable(GL_SCISSOR_TEST)
    glViewport(0, 0, fb_w, fb_h)
    glUseProgram(r.program)
    u_scale = float2(2.0f / ds.x, -2.0f / ds.y)
    u_translate = float2(-1.0f - 2.0f * dp.x / ds.x, 1.0f + 2.0f * dp.y / ds.y)
    imgui_gl_vs_bind_uniform(r.program)
    imgui_gl_fs_bind_uniform(r.program)     // sets the sampler uniform to texture unit 0
    glBindVertexArray(r.vao)
}

//! Replay the UI into the current framebuffer. Per command list: stream its vertex/index data into the
//! shared buffers, then replay each draw command with its own (y-flipped) scissor and texture.
def public imgui_gl_render(r : ImguiGlRenderer) {
    let dd = GetDrawData()
    if (dd == null || dd.TotalVtxCount == 0 || dd.DisplaySize.x <= 0.0f || dd.DisplaySize.y <= 0.0f) {
        return
    }
    let ds = dd.DisplaySize
    let dp = dd.DisplayPos
    let fb = dd.FramebufferScale
    let fb_w = int(ds.x * fb.x)
    let fb_h = int(ds.y * fb.y)
    if (fb_w <= 0 || fb_h <= 0) {
        return
    }
    setup_render_state(r, ds, dp, fb_w, fb_h)

    for (i in range(length(dd.CmdLists))) {
        let cl = dd.CmdLists[i]
        let vcnt = length(cl.VtxBuffer)
        let icnt = length(cl.IdxBuffer)
        if (vcnt == 0 || icnt == 0) {
            continue
        }
        glBindBuffer(GL_ARRAY_BUFFER, r.vbo)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.ebo)
        // block-form unsafe: expression-form would not authorize the nested addr() (CLAUDE.md gotcha)
        unsafe {
            glBufferData(GL_ARRAY_BUFFER, int64(vcnt * VERT_SIZE),
                reinterpret<void?>(addr(cl.VtxBuffer[0])), GL_STREAM_DRAW)
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, int64(icnt * IDX_SIZE),
                reinterpret<void?>(addr(cl.IdxBuffer[0])), GL_STREAM_DRAW)
        }

        for (j in range(length(cl.CmdBuffer))) {
            let c & = unsafe(cl.CmdBuffer[j])
            if (c.ElemCount == 0u) {
                continue
            }
            let clip = c.ClipRect
            var minx = (clip.x - dp.x) * fb.x
            var miny = (clip.y - dp.y) * fb.y
            var maxx = (clip.z - dp.x) * fb.x
            var maxy = (clip.w - dp.y) * fb.y
            // clamp to the framebuffer: a partially off-screen window can push the clip rect past the
            // target, and glScissor rejects a negative origin / out-of-range extent with GL_INVALID_VALUE.
            if (minx < 0.0f) {
                minx = 0.0f
            }
            if (miny < 0.0f) {
                miny = 0.0f
            }
            if (maxx > float(fb_w)) {
                maxx = float(fb_w)
            }
            if (maxy > float(fb_h)) {
                maxy = float(fb_h)
            }
            if (maxx <= minx || maxy <= miny) {
                continue
            }
            // GL scissor origin is bottom-left, so the clip rect's top (miny) maps to fb_h - maxy.
            glScissor(int(minx), fb_h - int(maxy), int(maxx - minx), int(maxy - miny))
            bind_sampler_2d(0, uint(c.GetTexID()))
            if (r.is_web) {
                // WebGL2 has no BaseVertex draw; with RendererHasVtxOffset cleared (see backend flags)
                // ImGui keeps VtxOffset == 0 by splitting any >64K-vertex list, so plain glDrawElements
                // over the freshly-bound per-list buffers is correct.
                glDrawElements(GL_TRIANGLES, int(c.ElemCount), GL_UNSIGNED_SHORT,
                    unsafe(reinterpret<void?>(int(c.IdxOffset) * IDX_SIZE)))
            } else {
                glDrawElementsBaseVertex(GL_TRIANGLES, int(c.ElemCount), GL_UNSIGNED_SHORT,
                    unsafe(reinterpret<void?>(int(c.IdxOffset) * IDX_SIZE)), int(c.VtxOffset))
            }
        }
    }

    // leave scissor disabled so the next frame's glClear isn't clipped to the last command's rect
    glDisable(GL_SCISSOR_TEST)
    glBindVertexArray(0u)
    glUseProgram(0u)
}

//! Set the ImGui backend flags this renderer satisfies (so ImGui drives the dynamic-texture queue and
//! emits per-command VtxOffset). The C++ ImGui_ImplOpenGL3_Init set these; the das driver's owner must.
def public imgui_gl_set_backend_flags() {
    var io & = unsafe(GetIO())
    io.BackendFlags |= ImGuiBackendFlags.RendererHasTextures
    // WebGL2 can't honor per-command VtxOffset (no BaseVertex draw); leaving this flag off makes ImGui
    // split >64K-vertex draw lists itself so the web path's plain glDrawElements stays correct.
    // get_running_platform_name() (real runtime call, not the host-folded get_platform_name()) so this
    // is correct in a cross-compiled in-browser exe too.
    if (get_running_platform_name() != "emscripten") {
        io.BackendFlags |= ImGuiBackendFlags.RendererHasVtxOffset
    }
}
