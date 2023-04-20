module app private

require rast2d public

require glfw/glfw_boost
require opengl/opengl_boost
require glsl/glsl_opengl
require daslib/defer
require daslib/safe_addr
require math
require strings
require sound

var [[in, location=0]] v_position : float2
var [[in, location=2]] v_texcoord : float2
var [[inout]] f_texcoord : float2
var [[uniform]] f_tex : sampler2D
var [[uniform]] f_palette : float4[256]
var [[out]] f_FragColor : float4

[vertex_program]
def vs_main
    f_texcoord = v_texcoord
    gl_Position = float4(-v_position.yx, 0.0, 1.0)

[fragment_program]
def fs_main
    let index = int(texture(f_tex,f_texcoord).x*255.0)
    let color = f_palette[index]
    f_FragColor = color

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
var bb_data : array<uint8>

def public get_screen_size
    return int2(bb_width,bb_height)

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

var window : GLFWwindow?

let public VK_LSHIFT = GLFW_KEY_LEFT_SHIFT
let public VK_RSHIFT = GLFW_KEY_RIGHT_SHIFT
let public VK_LALT = GLFW_KEY_LEFT_ALT
let public VK_RALT = GLFW_KEY_RIGHT_ALT
let public VK_LCONTROL = GLFW_KEY_LEFT_CONTROL
let public VK_RCONTROL = GLFW_KEY_RIGHT_CONTROL
let public VK_LEFT = GLFW_KEY_LEFT
let public VK_RIGHT = GLFW_KEY_RIGHT
let public VK_UP = GLFW_KEY_UP
let public VK_DOWN = GLFW_KEY_DOWN
let public VK_1 = GLFW_KEY_1
let public VK_2 = GLFW_KEY_2
let public VK_3 = GLFW_KEY_3
let public VK_4 = GLFW_KEY_4
let public VK_5 = GLFW_KEY_5
let public VK_6 = GLFW_KEY_6
let public VK_7 = GLFW_KEY_7
let public VK_8 = GLFW_KEY_8
let public VK_9 = GLFW_KEY_9
let public VK_0 = GLFW_KEY_0
let public VK_SPACE = GLFW_KEY_SPACE
let public VK_A = GLFW_KEY_A
let public VK_D = GLFW_KEY_D
let public VK_F2 = GLFW_KEY_F2
let public VK_F3 = GLFW_KEY_F3
let public VK_F4 = GLFW_KEY_F4
let public VK_F5 = GLFW_KEY_F5
let public VK_F6 = GLFW_KEY_F6
let public VK_F7 = GLFW_KEY_F7
let public VK_F8 = GLFW_KEY_F8
let public VK_F9 = GLFW_KEY_F9
let public VK_F10 = GLFW_KEY_F10
let public VK_F11 = GLFW_KEY_F11
let public VK_F12 = GLFW_KEY_F12

def public is_key_pressed ( key:int )
    return glfwGetKey(window, key)!=0

var dt_sec = 0.0lf

def public get_delta_time_sec
    return float(dt_sec)

def public set_palette ( pal : uint[256 ])
    for d,s in f_palette, pal
        d = unpack_byte_to_float(s) / float4(255.)

def fmt_msec ( msecs : float )
    let flt = build_string <| $ ( writer )
        writer |> format("%.1f",msecs)
    return "{flt}ms"

struct PlayingSound
    snd : PcmSound
    handle : PlayingSoundHandle
    busy : bool
    is3d : bool
    xy   : float2
    radius : float

let MAX_SOUNDS = 32

var playing_sounds : PlayingSound[MAX_SOUNDS]
var sound_head_position : float2
var sound_head_direction : float2

def allocate_app_sound
    for snd,i in playing_sounds,count()
        if !snd.busy
            snd.busy = true
            return i
    return -1

def public set_app_sound_position ( xy:float2; dxy:float2 )
    sound_head_position = xy
    sound_head_direction = dxy

def public play_app_sound ( wave:array<float>; freq:int )
    let i = allocate_app_sound()
    if i<0
        return
    playing_sounds[i].snd <- create_sound(freq, wave)
    playing_sounds[i].handle = play_sound(playing_sounds[i].snd)
    playing_sounds[i].busy = true
    playing_sounds[i].is3d = false

def public play_app_sound_2d ( pos:float2; radius:float; wave:array<float>; freq:int )
    let i = allocate_app_sound()
    if i<0
        return
    playing_sounds[i].snd <- create_sound(freq, wave)
    playing_sounds[i].handle = play_sound(playing_sounds[i].snd)
    playing_sounds[i].busy = true
    playing_sounds[i].is3d = true
    playing_sounds[i].xy = pos
    playing_sounds[i].radius = radius


def update_sounds
    for snd in playing_sounds
        if snd.busy
            if !is_playing(snd.handle)
                delete snd.snd
                snd.busy = false
                continue
        if snd.is3d
            var rxy = snd.xy - sound_head_position
            rxy = float2(rxy.x * sound_head_direction.x + rxy.y * sound_head_direction.y,
                        -rxy.x * sound_head_direction.y + rxy.y * sound_head_direction.x)
            // linear volume attenuation???
            let attn = max(0.0f, 1.0f - length(rxy) / snd.radius)
            set_sound_volume(snd.handle, attn)
            // panning
            let nrxy = normalize(rxy)
            set_sound_pan(snd.handle, -nrxy.y )

def public raster_app_main ( blk: block<(dt:float; var back_buffer:Bitmap8):void> )
    if glfwInit()==0
		panic("can't init glfw")
    defer <|
        glfwTerminate()
    sound_initialize()
    defer <|
        sound_finalize()
    glfwInitOpenGL(3,3)
    window = glfwCreateWindow(1920, 1080, "RASTER2D", null, null)
    if window==null
		panic("can't create window")
    defer <|
        glfwDestroyWindow(window)
    glfwMakeContextCurrent(window)
    var display_w, display_h : int
    glfwGetFramebufferSize(window, display_w, display_h)
    create_gl_objects(display_w, display_h)
    var tframe = ref_time_ticks()
    var prev_sec = glfwGetTime()
    var msecs = 12.
    var fmsec = 16.
    while glfwWindowShouldClose(window)==0
        var tsec = glfwGetTime()
        dt_sec = tsec - prev_sec
        prev_sec = tsec
        var dt_frame = get_time_usec(tframe)
        tframe = ref_time_ticks()
        fmsec = fmsec*0.95 + float(dt_frame/1000)*0.05
        update_sounds()
        unsafe
            heap_collect(true,true)
        glfwPollEvents()
        glfwGetFramebufferSize(window, display_w, display_h)
        update_bb(display_w, display_h)
        // callbakc
        var bmp = [[Bitmap8 size=int2(bb_width,bb_height), data=unsafe(addr(bb_data[0]))]]
        let t0 = ref_time_ticks()
        invoke(blk, fmsec, bmp)
        let dt = get_time_usec(t0)
        msecs = msecs*0.95 + float(dt/1000)*0.05
        glfwSetWindowTitle(window, "{bb_width}x{bb_height} frame:{fmt_msec(fmsec)} rast:{fmt_msec(msecs)}")
        // make a texture
        glGenTextures(1, safe_addr(texture))
        glBindTexture(GL_TEXTURE_2D, texture)
        glTexImage2D(GL_TEXTURE_2D, 0, int(GL_R8), bb_height, bb_width, 0, GL_RED, GL_UNSIGNED_BYTE, unsafe(addr(bb_data[0])))
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
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
