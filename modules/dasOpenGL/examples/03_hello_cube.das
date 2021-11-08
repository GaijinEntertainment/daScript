require glfw/glfw_boost
require opengl/opengl_boost
require daslib/defer
require daslib/math_boost

require daslib/safe_addr
require daslib/math_boost

var [[in, location=0]] v_position : float3
var [[in, location=1]] v_normal : float3
var [[in, location=2]] v_texcoord : float2
var [[uniform]] v_model : float4x4
var [[uniform]] v_view : float4x4
var [[uniform]] v_projection : float4x4
var [[inout]] f_color : float3
var [[inout]] f_texcoord : float2
var [[uniform]] f_tex : sampler2D

[vertex_program]
def vs_main
    f_texcoord = v_texcoord
    let light = normalize(float3(1,-1,-1))
    let norm = normalize ( float3x3(v_model) * v_normal )
    f_color = float3(saturate(-dot(norm,light)))
    gl_Position = v_projection * v_view * v_model * float4(v_position, 1.0)

[fragment_program]
def fs_main
    gl_FragColor.xyz = texture2D(f_tex,f_texcoord).xyz * f_color
    gl_FragColor.w = 1.

var program : uint
var vao : uint
var vbo : uint
var ebo : uint
var texture : uint

[vertex_buffer]
struct Vertex
    xyz    : float3
    normal : float3
    uv     : float2

let vertices = [[Vertex
    xyz=float3( 1, 1, 1), normal=float3(  0, 0, 1),  uv=float2(0,0);              // v0 (front)
    xyz=float3(-1, 1, 1), normal=float3(  0, 0, 1),  uv=float2(1,0);              // v1
    xyz=float3(-1,-1, 1), normal=float3(  0, 0, 1),  uv=float2(1,1);              // v2
    xyz=float3( 1,-1, 1), normal=float3(  0, 0, 1),  uv=float2(0,1);              // v3
    xyz=float3( 1, 1, 1), normal=float3(  1, 0, 0),  uv=float2(0,0);              // v0 (right)
    xyz=float3( 1,-1, 1), normal=float3(  1, 0, 0),  uv=float2(1,0);              // v3
    xyz=float3( 1,-1,-1), normal=float3(  1, 0, 0),  uv=float2(1,1);              // v4
    xyz=float3( 1, 1,-1), normal=float3(  1, 0, 0),  uv=float2(0,1);              // v5
    xyz=float3( 1, 1, 1), normal=float3(  0, 1, 0),  uv=float2(0,0);              // v0 (top)
    xyz=float3( 1, 1,-1), normal=float3(  0, 1, 0),  uv=float2(1,0);              // v5
    xyz=float3(-1, 1,-1), normal=float3(  0, 1, 0),  uv=float2(1,1);              // v6
    xyz=float3(-1, 1, 1), normal=float3(  0, 1, 0),  uv=float2(0,1);              // v1
    xyz=float3(-1, 1, 1), normal=float3( -1, 0, 0),  uv=float2(0,0);              // v1 (left)
    xyz=float3(-1, 1,-1), normal=float3( -1, 0, 0),  uv=float2(1,0);              // v6
    xyz=float3(-1,-1,-1), normal=float3( -1, 0, 0),  uv=float2(1,1);              // v7
    xyz=float3(-1,-1, 1), normal=float3( -1, 0, 0),  uv=float2(0,1);              // v2
    xyz=float3(-1,-1,-1), normal=float3(  0,-1, 0),  uv=float2(0,0);              // v7 (bottom)
    xyz=float3( 1,-1,-1), normal=float3(  0,-1, 0),  uv=float2(1,0);              // v4
    xyz=float3( 1,-1, 1), normal=float3(  0,-1, 0),  uv=float2(1,1);              // v3
    xyz=float3(-1,-1, 1), normal=float3(  0,-1, 0),  uv=float2(0,1);              // v2
    xyz=float3( 1,-1,-1), normal=float3(  0, 0,-1),  uv=float2(0,0);              // v4 (back)
    xyz=float3(-1,-1,-1), normal=float3(  0, 0,-1),  uv=float2(1,0);              // v7
    xyz=float3(-1, 1,-1), normal=float3(  0, 0,-1),  uv=float2(1,1);              // v6
    xyz=float3( 1, 1,-1), normal=float3(  0, 0,-1),  uv=float2(0,1) ]]            // v5

let indices = [[int
     0; 1; 2;   2; 3; 0;      // front
     4; 5; 6;   6; 7; 4;      // right
     8; 9;10;  10;11; 8;      // top
    12;13;14;  14;15;12;      // left
    16;17;18;  18;19;16;      // bottom
    20;21;22;  22;23;20 ]]    // back

def create_gl_objects
    program = create_shader_program(@@vs_main,@@fs_main)
    // vao
    glGenVertexArrays(1, safe_addr(vao))
    glBindVertexArray(vao)
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
    texture = load_image_from_file("{get_das_root()}/modules/dasOpenGL/examples/image.png")

[export]
def main
    if glfwInit()==0
		panic("can't init glfw")
    defer <|
        glfwTerminate()
    glfwInitOpenGL(3,3,true)
    var window = glfwCreateWindow(1280, 720, "OpenGL - Hello cube", null, null)
    if window==null
		panic("can't create window")
    defer <|
        glfwDestroyWindow(window)
    glfwMakeContextCurrent(window)
    create_gl_objects()
    v_view = look_at_rh(float3(0,0,10),float3(0,0,0),float3(0,1,0))
    while glfwWindowShouldClose(window)==0
        glfwPollEvents()
        var display_w, display_h : int
        glfwGetFramebufferSize(window, display_w, display_h)
        let aspect = display_h!=0 ? float(display_w)/float(display_h) : 1.
        // render
        glViewport(0, 0, display_w, display_h)
        glClearColor(0.2, 0.2, 0.2, 1.0)
        glClearDepth(1.0lf)
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
        glEnable(GL_DEPTH_TEST)
        glEnable(GL_CULL_FACE)
        glCullFace(GL_BACK)
        glDepthFunc(GL_LEQUAL)
        let t = glfwGetTime()
        if t > 10lf
            glfwSetTime(t - 10lf)
        let rot = un_quat_from_unit_vec_ang(normalize(float3(1.,1.,0.)), float(t)*PI*2.*0.1)
        v_model = compose(float4(0,0,0,0), rot, float4(1.0))        // bind model
        v_projection = perspective_rh_opengl( 45.0f*PI/180., aspect, 0.1f, 50.0f)   // bind projection
        glUseProgram(program)
        f_tex := texture
        vs_main_bind_uniform(program)
        fs_main_bind_uniform(program)
        glBindVertexArray(vao)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, null)
        glUseProgram(0u)
        bind_ffp()
        // swap buffers
        glfwMakeContextCurrent(window)
        glfwSwapBuffers(window)
