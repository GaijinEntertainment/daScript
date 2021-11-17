options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module opengl_ttf shared private

require opengl/opengl_boost
require ttf/stb_ttf_common public
require daslib/safe_addr
require daslib/math_boost

var [[in, location=0]] v_position : float2
var [[in, location=1]] v_texcoord : float2
var [[uniform]] v_mvp : float4x4
var [[inout]] f_texcoord : float2
var [[uniform]] f_tex : sampler2D
var [[uniform]] f_tint : float3
var [[out]] f_FragColor : float4

[vertex_program(name="TTF_VERTEX_SRC")]
def vs_main
    f_texcoord = v_texcoord
    gl_Position = v_mvp * float4(v_position, 0.0, 1.0)

[fragment_program(name="TTF_FRAGMENT_SRC")]
def fs_main
    let tex = texture(f_tex,f_texcoord)
    f_FragColor = float4(f_tint,tex.w)

[vertex_buffer]
struct TTF_FontVertex
    xy : float2
    uv : float2

class OpenGLTTFApiAdapter : TTFApiAdapter
    def override create_font_texture ( dim:int2; data:array<uint8> ) : uint64
        var tex = 0u
        glGenTextures(1, safe_addr(tex))
        glBindTexture(GL_TEXTURE_2D,tex)
        unsafe
            glTexImage2D(GL_TEXTURE_2D, 0, int(GL_ALPHA), dim.x, dim.y, 0, GL_ALPHA, GL_UNSIGNED_BYTE, addr(data[0]))
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        return uint64(tex)
    def override destroy_font_texture ( tex:uint64 ) : void
        var _tex = uint(tex)
        glDeleteTextures(1, safe_addr(_tex))

var private program : uint

def public create_ttf_adapter
    if !has_api_adapter()
        set_api_adapter <| new OpenGLTTFApiAdapter()

def public create_ttf_objects
    create_ttf_adapter()
    program = create_shader_program(@@vs_main,@@fs_main)
    return program

def public get_ttf_program : tuple<vtx:string;frag:string>
    return [[auto TTF_VERTEX_SRC, TTF_FRAGMENT_SRC]]

def public set_ttf_objects(prog:uint)
    create_ttf_adapter()
    program = prog

def public draw_quads_2d ( font:Font; quads : array<FontVertex>; w,h,x,y:int; tint:float3 = float3(1.) )
    let view = translation(float3(x, y, 0))
    let projection = ortho_rh(0., float(w), float(h), 0., -1., 1.)
    let mvp = projection * view
    font |> draw_quads(quads, mvp, tint)

def public draw_quads ( font:Font; quads : array<FontVertex>; mvp:float4x4; tint:float3 = float3(1.) )
    assert(program!=0u,"missing create_ttf_objects or cache_ttf_objects somewhere in the initialization")
    if empty(quads)
        return
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glUseProgram(program)
    f_tex.texture2D = uint(font.tex)
    f_tint = tint
    v_mvp = mvp
    vs_main_bind_uniform(program)
    fs_main_bind_uniform(program)
    glBindVertexArray(0u)
    glBindBuffer(GL_ARRAY_BUFFER, 0u)
    unsafe
        bind_vertex_buffer(addr(quads[0]), type<TTF_FontVertex>)
    glDrawArrays(GL_QUADS, 0, length(quads))
    glUseProgram(0u)
    glDisable(GL_BLEND)
