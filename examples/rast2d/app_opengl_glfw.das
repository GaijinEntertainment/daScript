module app_opengl_glfw private

require rast2d public

require glfw/glfw_boost
require opengl/opengl_boost
require glsl/glsl_opengl
require daslib/defer
require daslib/safe_addr
require math

var [[in, location=0]] v_position : float2
var [[in, location=2]] v_texcoord : float2
var [[inout]] f_texcoord : float2
var [[uniform]] f_tex : sampler2D
var [[out]] f_FragColor : float4

[vertex_program]
def vs_main
    f_texcoord = v_texcoord
    gl_Position = float4(v_position, 0.0, 1.0)

[fragment_program]
def fs_main
    f_FragColor = texture(f_tex,f_texcoord)

var program : uint
var vao : uint
var vbo : uint
var ebo : uint
var texture : uint

[vertex_buffer]
struct Vertex
    xy  : float2
    rgb : float3
    uv  : float2

let vertices = [[Vertex
    xy=float2(-1.0, 1.0), uv=float2(0.0,0.0);
    xy=float2( 1.0, 1.0), uv=float2(1.0,0.0);
    xy=float2( 1.0,-1.0), uv=float2(1.0,1.0);
    xy=float2(-1.0,-1.0), uv=float2(0.0,1.0)
]]

let indices = [[int 0; 1; 2; 2; 3; 0]]

var bb_width = 0
var bb_height = 0
var bb_data : array<uint>

def update_bb ( var w, h : int )
    w = (w+3) & ~3
    h = (h+3) & ~3
    if w==bb_width && h==bb_height
        return false
    bb_width = w
    bb_height = h
    bb_data |> clear()
    bb_data |> resize(w*h)
    return true

def create_gl_objects(w,h : int)
    program = create_shader_program(@@vs_main,@@fs_main)
    // vao
    glGenVertexArrays(1, safe_addr(vao))
    glBindVertexArray(vao);
    // vbo
    glGenBuffers(1, safe_addr(vbo))
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW)
    bind_vertex_buffer(null,type<Vertex>)
    // ebo
    glGenBuffers(1, safe_addr(ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW)
    // texture
    update_bb(w,h)

def public raster_app_main ( blk: block<(var back_buffer:Bitmap):void> )
    if glfwInit()==0
		panic("can't init glfw")
    defer <|
        glfwTerminate()
    glfwInitOpenGL(3,3)
    var window = glfwCreateWindow(1280, 720, "RASTER2D", null, null)
    if window==null
		panic("can't create window")
    defer <|
        glfwDestroyWindow(window)
    glfwMakeContextCurrent(window)
    var display_w, display_h : int
    glfwGetFramebufferSize(window, display_w, display_h)
    create_gl_objects(display_w, display_h)
    var tframe = ref_time_ticks()
    while glfwWindowShouldClose(window)==0
        var dt_frame = get_time_usec(tframe)
        tframe = ref_time_ticks()
        unsafe
            heap_collect(true,true)
        glfwPollEvents()
        glfwGetFramebufferSize(window, display_w, display_h)
        update_bb(display_w, display_h)
        // callbakc
        var bmp = [[Bitmap w=bb_width, h=bb_height, pixels=unsafe(addr(bb_data[0]))]]
        let t0 = ref_time_ticks()
        invoke(blk, bmp)
        let dt = get_time_usec(t0)
        glfwSetWindowTitle(window, "RASTER2D {bb_width}x{bb_height} frame:{dt_frame/1000}ms rast:{dt/1000}ms")
        // make a texture
        glGenTextures(1, safe_addr(texture))
        glBindTexture(GL_TEXTURE_2D, texture)
        glTexImage2D(GL_TEXTURE_2D, 0, int(GL_RGBA), bb_width, bb_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, unsafe(addr(bb_data[0])))
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        glBindTexture(GL_TEXTURE_2D, GL_INVALID_VALUE)
        // render
        glViewport(0, 0, display_w, display_h)
        glDisable(GL_DEPTH_TEST)
        glDepthMask(false)
        glUseProgram(program)
        f_tex := texture           // bind texture
        vs_main_bind_uniform(program)
        fs_main_bind_uniform(program)
        glBindVertexArray(vao)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null)
        glBindTexture(GL_TEXTURE_2D, GL_INVALID_VALUE)
        // kill texture
        glDeleteTextures(1, safe_addr(texture))
        // swap buffersa
        glfwMakeContextCurrent(window)
        glfwSwapBuffers(window)
