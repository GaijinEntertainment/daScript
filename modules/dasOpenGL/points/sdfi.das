require glfw/glfw_boost
require opengl/opengl_boost
require glsl/glsl_opengl
require daslib/defer
require daslib/math_boost

require daslib/safe_addr

var [[in, location=0]] v_position : float2
var [[in, location=1]] v_color : float3
var [[in, location=2]] v_texcoord : float2
var [[inout]] f_color : float3
var [[inout]] f_texcoord : float2
var [[uniform]] f_tex : sampler2D
var [[out]] f_FragColor : float4

[vertex_program]
def vs_main
    f_color = v_color
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
    var tmin = 1.0
    var tmax = 20.0

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

let iTime = 0.
let iResolution = float2(1280.0,720.0)

[fragment_program]
def fs_main
    let an = 12.0 - sin(0.1*iTime)
    let ro = float3( 3.0*cos(0.1*an), 1.0, -3.0*sin(0.1*an) )
    let ta = float3( 0.0, -0.4, 0.0 )
    let ca = setCamera(ro, ta, 0.0)
    let tech = 1
    let p = f_texcoord * 2.0 - float2(1.0)
    let rd = ca * normalize(float3(p.x,-p.y,2.0))
    var col = render(ro,rd,tech)
    col = pow(col, float3(0.4545))
    f_FragColor.xyz = col
    f_FragColor.w = 1.

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
    xy=float2(-1.0, 1.0), rgb=float3(0.0,0.0,0.0), uv=float2(0.0,0.0);
    xy=float2( 1.0, 1.0), rgb=float3(1.0,1.0,0.0), uv=float2(1.0,0.0);
    xy=float2( 1.0,-1.0), rgb=float3(1.0,0.0,0.0), uv=float2(1.0,1.0);
    xy=float2(-1.0,-1.0), rgb=float3(1.0,1.0,0.0), uv=float2(0.0,1.0)
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
    // texture
    texture = load_image_from_file("{get_das_root()}/modules/dasOpenGL/examples/image.png")

[export]
def main

    print("{fract(float3(1.2,-2.3,3.4))}\n")

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
    while glfwWindowShouldClose(window)==0
        glfwPollEvents()
        var display_w, display_h : int
        glfwGetFramebufferSize(window, display_w, display_h)
        // render
        glViewport(0, 0, display_w, display_h)
        glClearColor(0.2, 0.2, 0.2, 1.0)
        glClearDepth(1.0lf)
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
        glEnable(GL_DEPTH_TEST)
        glDepthFunc(GL_LEQUAL)
        glUseProgram(program)
        f_tex := texture           // bind texture
        vs_main_bind_uniform(program)
        fs_main_bind_uniform(program)
        glBindVertexArray(vao)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null)
        // swap buffersa
        glfwMakeContextCurrent(window)
        glfwSwapBuffers(window)