options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module opengl_ttf shared private

require opengl/opengl_boost
require stbimage/stbimage_ttf public
require stbtruetype
require daslib/safe_addr
require daslib/math_boost
require strings

var @in @location v_position : float2
var @in @location v_texcoord : float2
var @uniform v_mvp : float4x4
var @inout f_texcoord : float2
var @uniform f_tex : sampler2D
var @uniform f_tint : float3
var @out f_FragColor : float4

[vertex_program(name="TTF_VERTEX_SRC")]
def vs_main {
    f_texcoord = v_texcoord
    gl_Position = v_mvp * float4(v_position, 0.0, 1.0)
}

[fragment_program(name="TTF_FRAGMENT_SRC")]
def fs_main {
    let tex = texture(f_tex, f_texcoord)
    f_FragColor = float4(f_tint, tex.x)
}

// ── Vertex type ──────────────────────────────────────────────────────────

[vertex_buffer]
struct public FontVertex {
    xy : float2
    uv : float2
}

// ── GL texture management ────────────────────────────────────────────────

def public upload_font_texture(var font : Font) {
    //! Upload the font atlas bitmap to an OpenGL texture.
    //! Sets ``font.tex`` to the GL texture handle.
    //! Does nothing if the texture is already uploaded.
    if (font.tex != 0ul) {
        return
    }
    assert(int(font.bitmap.width) > 0, "font bitmap is not valid")
    var tex = 0u
    glGenTextures(1, safe_addr(tex))
    glBindTexture(GL_TEXTURE_2D, tex)
    unsafe {
        glTexImage2D(GL_TEXTURE_2D, 0, int(GL_R8), int(font.bitmap.width), int(font.bitmap.height),
            0, GL_RED, GL_UNSIGNED_BYTE, addr(font.bitmap.bytes[0]))
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    font.tex = uint64(tex)
}

def public destroy_font_texture(var font : Font) {
    //! Delete the GL texture associated with the font.
    if (font.tex != 0ul) {
        var tex = uint(font.tex)
        glDeleteTextures(1, safe_addr(tex))
        font.tex = 0ul
    }
}

// ── Quad generation ──────────────────────────────────────────────────────

def public create_quads(font : Font; text : string; at : float2 = float2(0.)) {
    //! Generate an array of FontVertex quads for GPU text rendering.
    //! Each character produces 4 vertices (a quad). Use with ``draw_quads``.
    //! For best results, load the font with oversampling=2.
    var quads : array<FontVertex>
    quads |> reserve(length(text) * 4)
    var q : stbtt_aligned_quad
    var x = at.x
    var y = at.y
    let first = font.char_range.x
    let num_chars = font.char_range.y
    let bw = int(font.bitmap.width)
    let bh = int(font.bitmap.height)
    for (ch in text) {
        let idx = ch - first
        if (idx >= 0 && idx < num_chars) {
            unsafe {
                stbtt_GetPackedQuad(addr(font.chardata[0]), bw, bh, idx,
                    safe_addr(x), safe_addr(y), safe_addr(q), 0)
            }
            quads |> push(FontVertex(xy = float2(q.x0, q.y0), uv = float2(q.s0, q.t0)))
            quads |> push(FontVertex(xy = float2(q.x1, q.y0), uv = float2(q.s1, q.t0)))
            quads |> push(FontVertex(xy = float2(q.x1, q.y1), uv = float2(q.s1, q.t1)))
            quads |> push(FontVertex(xy = float2(q.x0, q.y1), uv = float2(q.s0, q.t1)))
        }
    }
    return <- quads
}

def public quads_dim(quads : array<FontVertex>) : tuple<vmin : float2; vmax : float2> {
    //! Compute the axis-aligned bounding box of a quad array.
    if (empty(quads)) {
        return (float2(0.), float2(0.))
    }
    var vmin = float2(FLT_MAX, FLT_MAX)
    var vmax = float2(-FLT_MAX, -FLT_MAX)
    for (q in quads) {
        vmin = min(vmin, q.xy)
        vmax = max(vmax, q.xy)
    }
    return (vmin, vmax)
}

enum public HJust {
    left
    center
    right
}

enum public VJust {
    top
    center
    bottom
}

def public quads_view(quads : array<FontVertex>; scale : float2; h : HJust = HJust.center; v : VJust = VJust.center) {
    //! Compute a model matrix that scales and justifies the quad text.
    if (empty(quads)) {
        return float4x4()
    }
    let d = quads_dim(quads)
    var o : float2
    if (h == HJust.left) {
        o.x = -(d.vmax.x - d.vmin.x) * scale.x
    } elif (h == HJust.center) {
        o.x = -(d.vmax.x + d.vmin.x) * 0.5 * scale.x
    } else {
        o.x = 0.
    }
    if (v == VJust.bottom) {
        o.y = -(d.vmax.y - d.vmin.y) * scale.y
    } elif (v == VJust.center) {
        o.y = -(d.vmax.y + d.vmin.y) * 0.5 * scale.y
    } else {
        o.y = 0.
    }
    return compose(float3(o, 0.), float4(1, 0, 0, 0), float3(scale, 0.))
}

// ── GL rendering ─────────────────────────────────────────────────────────

var private program : uint
var private vao, vbo : uint
var private ebo : uint

let private MAX_TTF_QUADS = 128

def public create_ttf_objects {
    //! Create the TTF shader program and GPU buffers (VAO, VBO, EBO).
    //! Call once during initialization.
    program = create_shader_program(@@vs_main, @@fs_main)
    // build static index buffer
    var indices : array<int>
    indices |> reserve(MAX_TTF_QUADS * 6)
    for (idx in range(MAX_TTF_QUADS)) {
        indices |> push(idx * 4 + 0)
        indices |> push(idx * 4 + 1)
        indices |> push(idx * 4 + 2)
        indices |> push(idx * 4 + 2)
        indices |> push(idx * 4 + 3)
        indices |> push(idx * 4 + 0)
    }
    // vertex arrays
    glGenVertexArrays(1, safe_addr(vao))
    glBindVertexArray(vao)
    // vbo
    glGenBuffers(1, safe_addr(vbo))
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBindBuffer(GL_ARRAY_BUFFER, 0u)
    glBindVertexArray(0u)
    // ebo
    glGenBuffers(1, safe_addr(ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u)
    return program
}

def public get_ttf_program : tuple<vtx : string; frag : string> {
    return (TTF_VERTEX_SRC, TTF_FRAGMENT_SRC)
}

def public set_ttf_objects(prog : uint) {
    //! Set an externally-created shader program for TTF rendering.
    //! Also creates GPU buffers if not already done.
    if (vao == 0u) {
        // build static index buffer
        var indices : array<int>
        indices |> reserve(MAX_TTF_QUADS * 6)
        for (idx in range(MAX_TTF_QUADS)) {
            indices |> push(idx * 4 + 0)
            indices |> push(idx * 4 + 1)
            indices |> push(idx * 4 + 2)
            indices |> push(idx * 4 + 2)
            indices |> push(idx * 4 + 3)
            indices |> push(idx * 4 + 0)
        }
        glGenVertexArrays(1, safe_addr(vao))
        glBindVertexArray(vao)
        glGenBuffers(1, safe_addr(vbo))
        glBindBuffer(GL_ARRAY_BUFFER, vbo)
        glBindBuffer(GL_ARRAY_BUFFER, 0u)
        glBindVertexArray(0u)
        glGenBuffers(1, safe_addr(ebo))
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u)
    }
    program = prog
}

def public draw_quads_2d(font : Font; quads : array<FontVertex>; w, h, x, y : int; tint : float3 = float3(1.)) {
    //! Draw text quads using 2D orthographic projection.
    let view = translation(float3(x, y, 0))
    let projection = ortho_rh(0., float(w), float(h), 0., -1., 1.)
    let mvp = projection * view
    font |> draw_quads(quads, mvp, tint)
}

def public draw_quads(font : Font; quads : array<FontVertex>; mvp : float4x4; tint : float3 = float3(1.)) {
    //! Draw text quads with a model-view-projection matrix.
    assert(program != 0u, "missing create_ttf_objects or cache_ttf_objects somewhere in the initialization")
    if (empty(quads)) {
        return
    }
    // buffer data
    glBindVertexArray(vao)
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBufferData(GL_ARRAY_BUFFER, quads, GL_DYNAMIC_DRAW)
    bind_vertex_buffer(null, type<FontVertex>)
    glBindBuffer(GL_ARRAY_BUFFER, 0u)
    // and draw
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glUseProgram(program)
    f_tex.texture2D = uint(font.tex)
    f_tint = tint
    v_mvp = mvp
    vs_main_bind_uniform(program)
    fs_main_bind_uniform(program)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
    let total = min(length(quads) / 4, MAX_TTF_QUADS)
    glDrawElements(GL_TRIANGLES, total * 6, GL_UNSIGNED_INT, null)
    glUseProgram(0u)
    glDisable(GL_BLEND)
    glBindVertexArray(0u)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u)
    bind_ffp()
}
