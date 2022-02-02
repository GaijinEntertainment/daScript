require glfw/glfw_boost
require opengl/opengl_boost
require glsl/glsl_opengl
require daslib/defer
require daslib/math_boost

require daslib/safe_addr

var [[in, location=0]] v_position : float2
var [[in, location=1]] v_texcoord : float2
var [[inout]] f_texcoord : float2
var [[out]] f_FragColor : float4

var [[uniform]] f_Camera2World : float4x4
var [[uniform]] f_CameraNearFar : float2

[vertex_program]
def vs_main
    f_texcoord = v_texcoord
    gl_Position = float4(v_position, 0.0, 1.0)

def sdPlane ( p:float3 )
    return p.y

def sdBox ( p,b:float3 )
    let d = abs(p) - b
    return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,float3(0.0)))

def map ( pos:float3 )
    let qos = float3 ( fract(pos.x+0.5)-0.5, pos.yz )
    return min( sdPlane(     pos.xyz-float3( 0.0,0.00, 0.0)),
                sdBox(       qos.xyz-float3( 0.0,0.25, 0.0), float3(0.2,0.5,0.2) ) )

def calcSoftshadow ( ro,rd:float3; mint,tmax:float; tech:int )
    var res = 1.0
    var t = mint
    var ph = 1e10
    for i in 0..32
        let h = map ( ro + rd*t )
        if tech==0
            res = min(res, 10.0*h/t)
        else
            let y = h*h/(2.0*ph)
            let d = sqrt(h*h-y*y)
            res = min(res, 10.0*d/max(0.,t-y))
            ph = h
        t += h
        if res<0.0001 || t>tmax
            break
    res = saturate(res)
    return res*res*(3.0-2.0*res)

def calcNormal ( pos:float3 )
    let e = float2(1.0,-1.0)*0.5773*0.0005
    return normalize( e.xyy*map( pos + e.xyy ) +
					  e.yyx*map( pos + e.yyx ) +
					  e.yxy*map( pos + e.yxy ) +
					  e.xxx*map( pos + e.xxx ) )

def castRay ( ro,rd:float3 )
    var tmin = 0.05
    var tmax = 25.0

    // bounding volume (optional)
    let tp1 = (0.0-ro.y)/rd.y
    if tp1>0.0
        tmax = min(tmax, tp1)
    let tp2 = (1.0-ro.y)/rd.y
    if tp2>0.0
        if ro.y>1.0
            tmin = max(tmin, tp2)
        else
            tmax = min(tmax, tp2)

    var t = tmin
    for i in 0..64
        let precis = 0.0005*t
        let res = map(ro+rd*t)
        if res<precis || t>tmax
            break
        t += res
    if t > tmax
        t = -1.0
    return t

def calcAO ( pos,nor:float3 )
    var occ = 0.0
    var sca = 1.0
    for i in 0..5
        let h = 0.001 + 0.15*float(i)/4.0
        let d = map( pos + h*nor )
        occ += (h-d)*sca
        sca *= 0.95
    return saturate(1.0-1.5*occ)

def render ( ro,rd:float3; tech:int )
    var col = float3(0.0)
    let t = castRay(ro,rd)
    if t > -0.5
        let pos = ro + rd*t
        let nor = calcNormal(pos)
        let mate = float3(0.3)
        let lig = normalize(float3(-0.1,0.3,0.6))
        let hal = normalize(lig-rd)
        let dif = saturate(dot(nor,lig)) * calcSoftshadow(pos,lig,0.01,3.0,tech)
        let spe = pow(saturate(dot(nor,hal)),16.0) * dif * (0.04 + 0.96*pow(saturate(1.0+dot(hal,rd)),5.0))
        col = mate * 4.0*dif*float3(1.0,0.7,0.5)
        col += 12.0*spe*float3(1.,0.7,0.5)
        let occ = calcAO(pos,nor)
        let amb = saturate(0.5+0.5*nor.y)
        col += mate*amb*occ*float3(0.0,0.08,0.1)
        col *= exp(-0.0005*t*t*t)
    return col

def setCamera ( ro,ta:float3; cr:float )
    let cw = normalize(ta-ro)
    let cp = float3(sin(cr),cos(cr),0.0)
    let cu = normalize(cross(cw,cp))
    let cv = normalize(cross(cu,cw))
    var res : float3x3
    res[0] = cu
    res[1] = cv
    res[2] = cw
    return res

[fragment_program]
def fs_main
    let tech = 1
    let near = f_CameraNearFar.x
    let far = f_CameraNearFar.y
    let p = f_texcoord * 2.0 - float2(1.0)
    let pos = float2(p.x,-p.y)
    let ro = (f_Camera2World * float4(pos, -1.0, 1.0) * near).xyz
    let pd = (f_Camera2World * float4(pos * (far - near), far + near, far - near)).xyz
    let rd = normalize(pd - ro)
    var col = render(ro,rd,tech)
    col = pow(col, float3(0.4545))
    f_FragColor.xyz = col
    f_FragColor.w = 1.

var program : uint
var vao : uint
var vbo : uint
var ebo : uint

[vertex_buffer]
struct Vertex
    xy  : float2
    uv  : float2

let vertices = [[Vertex
    xy=float2(-1.0, 1.0), uv=float2(0.0,0.0);
    xy=float2( 1.0, 1.0), uv=float2(1.0,0.0);
    xy=float2( 1.0,-1.0), uv=float2(1.0,1.0);
    xy=float2(-1.0,-1.0), uv=float2(0.0,1.0)
]]

let indices = [[int 0; 1; 2; 2; 3; 0]]

def create_gl_objects
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

def camera_rotation ( camera_rotation_xy:float2 )
    let rot_x = un_quat_from_unit_vec_ang(normalize(float3(1.,0.,0.)), camera_rotation_xy.x*PI/180.)
    let rot_y = un_quat_from_unit_vec_ang(normalize(float3(0.,1.,0.)), camera_rotation_xy.y*PI/180.)
    let rot = quat_mul(rot_x,rot_y)
    return <- compose(float4(0,0,0,1), rot, float4(1.))

[export]
def main
    if glfwInit()==0
		panic("can't init glfw")
    defer <|
        glfwTerminate()
    glfwInitOpenGL(3,3)
    var window = glfwCreateWindow(1280, 720, "Hello distance fields with soft shadows", null, null)
    if window==null
		panic("can't create window")
    defer <|
        glfwDestroyWindow(window)
    glfwMakeContextCurrent(window)
    create_gl_objects()
    var camera_position = float3(0,3,15)
    var camera_rotation_xy = float2(0,0)
    var cursor_position_xy = glfwGetCursorPos(window)
    var cursor_enabled = false
    unsafe
        glfwSetKeyCallback(window) <| @ [[&camera_position,&camera_rotation_xy]] ( w, kk, sc, act, mod )
            var y = 0.
            if sc==17       //w
                y = 1.
            elif sc==31     //s
                y = -1.
            if y != 0.
                let v_cam = inverse(camera_rotation(camera_rotation_xy))
                camera_position -= v_cam[2].xyz * float(y)
        glfwSetScrollCallback(window) <| @ [[&camera_position,&camera_rotation_xy]] ( w, x, y )
            let v_cam = inverse(camera_rotation(camera_rotation_xy))
            camera_position -= v_cam[2].xyz * float(y)
        glfwSetCursorPosCallback(window) <| @ [[&cursor_position_xy,&cursor_enabled,&camera_rotation_xy]] ( w, x, y )
            if cursor_enabled
                let cxy = glfwGetCursorPos(window)
                let dxy = cxy - cursor_position_xy
                var display_w, display_h : int
                glfwGetFramebufferSize(window, display_w, display_h)
                camera_rotation_xy += float2(dxy.y, dxy.x)
                cursor_position_xy = float2(display_w/2, display_h/2)
                glfwSetCursorPos(window, double(cursor_position_xy.x), double(cursor_position_xy.y))
        glfwSetMouseButtonCallback(window) <| @ [[&cursor_enabled,&cursor_position_xy]] ( w, b, a, m )
            if b==int(GLFW_MOUSE_BUTTON_1)
                cursor_enabled = a==int(GLFW_PRESS)
                if cursor_enabled
                    glfwSetInputMode(window, int(GLFW_CURSOR), int(GLFW_CURSOR_HIDDEN))
                    glfwSetInputMode(window, int(GLFW_RAW_MOUSE_MOTION), int(GLFW_TRUE))
                    var display_w, display_h : int
                    glfwGetFramebufferSize(window, display_w, display_h)
                    cursor_position_xy = float2(display_w/2, display_h/2)
                    glfwSetCursorPos(window, double(cursor_position_xy.x), double(cursor_position_xy.y))
                else
                    glfwSetInputMode(window, int(GLFW_CURSOR), int(GLFW_CURSOR_NORMAL))
                    glfwSetInputMode(window, int(GLFW_RAW_MOUSE_MOTION), int(GLFW_FALSE))

    while glfwWindowShouldClose(window)==0
        glfwPollEvents()
        var display_w, display_h : int
        glfwGetFramebufferSize(window, display_w, display_h)
        let aspect = display_h!=0 ? float(display_w)/float(display_h) : 1.
        glViewport(0, 0, display_w, display_h)
        glClearColor(0.2, 0.2, 0.2, 1.0)
        glClearDepth(1.0lf)
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
        glEnable(GL_DEPTH_TEST)
        glDepthFunc(GL_LEQUAL)
        glUseProgram(program)
        f_CameraNearFar = float2(0.05f,50.0f)
        let v_view = camera_rotation(camera_rotation_xy) * compose(float4(-camera_position,1.), float4(0), float4(1.))
        let v_projection = perspective_rh_opengl( 45.0f*PI/180., aspect, f_CameraNearFar.x, f_CameraNearFar.y )
        f_Camera2World = inverse(v_projection * v_view)
        vs_main_bind_uniform(program)
        fs_main_bind_uniform(program)
        glBindVertexArray(vao)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null)
        glfwMakeContextCurrent(window)
        glfwSwapBuffers(window)