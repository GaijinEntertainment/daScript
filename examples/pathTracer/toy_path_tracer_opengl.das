require daslib/glfw_boost
require daslib/opengl_boost
require daslib/glsl
require daslib/defer
require math

require daslib/safe_addr
require daslib/random
require daslib/tga

require path_tracer

var [[in, location=0]] v_position : float2
var [[in, location=1]] v_color : float3
var [[in, location=2]] v_texcoord : float2
var [[inout]] f_color : float3
var [[inout]] f_texcoord : float2
var [[out]] f_outputColor : float4
var [[uniform]] f_tex : sampler2D
var [[uniform, format=rgba8, binding=0]] c_destTex : image2D

[vertex_program(name="VERTEX_SRC",version=330)]
def vs_effect
    f_color = v_color
    f_texcoord = v_texcoord
    gl_Position = float4(v_position.x, v_position.y, 0.0, 1.0)

[fragment_program(name="FRAGMENT_SRC",version=330)]
def ps_effect
    f_outputColor = texture(f_tex,f_texcoord) // * float4(f_color.x, f_color.y, f_color.z, 1.0)

[compute_program(name="COMPUTE_SRC",version=430,local_size_x=16,local_size_y=16)]
def compute_effect
    let storePos = int2(gl_GlobalInvocationID.xy)
    random_seed2D(RAND_SEED,storePos)
    let rgb = draw_ray(storePos.x,255-storePos.y,256,256)
    let srgb = linear_to_RGB(rgb)
    imageStore(c_destTex,storePos, float4(srgb.x, srgb.y, srgb.z, 1.0))

var compute_program, display_program : uint
var vao : uint
var vbo : uint
var ebo : uint
var texture : uint

[vertex_buffer]
struct EffectVertex
    [[type=GL_FLOAT, size=2, normalized=false]] xy  : float2
    [[type=GL_FLOAT, size=3, normalized=false]] rgb : float3
    [[type=GL_FLOAT, size=2, normalized=false]] uv  : float2

let effect_vertices = [[EffectVertex
        xy=float2(-1.0, 1.0), rgb=float3(0.0,0.0,0.0), uv=float2(0.0,0.0);
        xy=float2( 1.0, 1.0), rgb=float3(1.0,1.0,0.0), uv=float2(1.0,0.0);
        xy=float2( 1.0,-1.0), rgb=float3(1.0,0.0,0.0), uv=float2(1.0,1.0);
        xy=float2(-1.0,-1.0), rgb=float3(1.0,1.0,0.0), uv=float2(0.0,1.0)
]]

let effect_indices = [[int 0; 1; 2; 2; 3; 0]]

def create_gl_objects
    compute_program = create_compute_shader_program(COMPUTE_SRC)
    display_program = create_shader_program(VERTEX_SRC,FRAGMENT_SRC)
    glGenVertexArrays(1, safe_addr(vao))
    glBindVertexArray(vao);
    glGenBuffers(1, safe_addr(vbo))
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBufferData(GL_ARRAY_BUFFER, effect_vertices, GL_STATIC_DRAW)
    glGenBuffers(1, safe_addr(ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, effect_indices, GL_STATIC_DRAW)
    glGenTextures(1, safe_addr(texture))
    glBindTexture(GL_TEXTURE_2D, texture)
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 256, 256 );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)

[export]
def main
    if glfwInit()==0
		panic("can't init glfw")
    defer <|
        glfwTerminate()
    glfwInitOpenGL(4,3)
    var window = glfwCreateWindow(1280, 720, "Hello compute shader", null, null)
    if window==null
		panic("can't create window")
    defer <|
        glfwDestroyWindow(window)
    glfwMakeContextCurrent(window)
    create_gl_objects()
    f_tex.texture = 0
    while glfwWindowShouldClose(window)==0
        glfwPollEvents()
        var display_w, display_h : int
        glfwGetFramebufferSize(window, display_w, display_h)

        glUseProgram(compute_program)
        c_destTex.texture = 0
        compute_effect_bind_uniform(compute_program)
        glBindImageTexture(0u, texture, 0, false, 0, GL_WRITE_ONLY, GL_RGBA8)
        glDispatchCompute(16u,16u,1u)

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT)

        glViewport(0, 0, display_w, display_h)
        glUseProgram(display_program)
        glBindBuffer(GL_ARRAY_BUFFER, vbo)
        bind_vertex_buffer(type<EffectVertex>)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
        f_tex.texture = 0
        bind_texture_2d(0, texture)
        vs_effect_bind_uniform(display_program)
        ps_effect_bind_uniform(display_program)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null)

        glfwMakeContextCurrent(window)
        glfwSwapBuffers(window)

options debugger
require daslib/debug
