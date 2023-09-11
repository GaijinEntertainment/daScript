require glfw/glfw_boost
require opengl/opengl_boost
require glsl/glsl_opengl
require daslib/defer
require daslib/math_boost

require daslib/safe_addr

require sdf

let AA = 1
let MAX_CAST_STEPS = 256
let GEN_IN_SOFTWARE = false
let BORDER = 0.1
let SDF_STEP = float2(0.025,0.006)
let FAST_SHADOWS = false
let USE_ANALYTIC_SDF = false

var [[in, location=0]] v_position : float2
var [[in, location=1]] v_texcoord : float2
var [[inout]] f_texcoord : float2
var [[out]] f_FragColor : float4
var [[uniform]] f_Camera2World : float4x4
var [[uniform]] f_CameraNearFar : float2
var [[uniform]] f_PixelSize : float2
var [[uniform, stage=0]] t_sdf : sampler3D
var [[uniform, stage=1]] t_sdf_color : sampler3D

var [[uniform]] c_dim : int3
var [[uniform]] c_bmin : float3
var [[uniform]] c_step : float
var [[uniform, format=r32f, stage=0]] c_sdf : image3D
var [[uniform, format=rgba8, stage=1]] c_sdf_color : image3D

[compute_program(local_size_x=8,local_size_y=8,local_size_z=8)]
def map_effect
    let xyz = int3(gl_GlobalInvocationID.xyz)
    if xyz.x<c_dim.x && xyz.y<c_dim.y && xyz.z<c_dim.z
        let p = c_bmin + float3(xyz) * c_step
        let c = map_(p)
        imageStore(c_sdf, xyz, c)               // only x get written
        imageStore(c_sdf_color, xyz, c.yzww)    // yzww get written

[vertex_program]
def vs_main
    f_texcoord = v_texcoord
    gl_Position = float4(v_position, 0.0, 1.0)

def xCol ( x:float; m:float )
    let col = float3(0.2) + sin( float3(m)*2.0 + float3(0.0,1.0,2.0) ) * 0.2
    return float4 ( x, col )

def xCol ( x:float; m:float3 )
    return float4 ( x, m )

def opU ( d1,d2:float4 )
	return d1.x<d2.x ? d1 : d2

def checker ( var p:float3 )
    p *= 4.0
    return ((int(p.x)^int(p.z)) & 1) != 0 ? float3(0.3,0.3,0.3) : float3(0.6,0.6,0.6)

var t3d : uint
var t3d_color : uint
var c_disp_size : uint3

def gen_map_texture ( var bmin,bmax:float3; _step:float2=SDF_STEP )
    let step = GEN_IN_SOFTWARE ? _step.x : _step.y
    bmin -= float3(BORDER)
    bmax += float3(BORDER)
    let idim = int3 ( (bmax-bmin)/step )
    print("idim = {idim}\n")

    var ph = null
    let pgb = null

    if GEN_IN_SOFTWARE
        var h : array<float>
        var gb : array<uint>
        h |> resize(idim.x*idim.y*idim.z)
        gb |> resize(idim.x*idim.y*idim.z)
        for z in 0..idim.z
            let oz = z*idim.y*idim.x
            for y in 0..idim.y
                let oyz = oz + y*idim.x
                for x in 0..idim.x
                    let p = bmin + float3(x,y,z) * step // + float3(step*0.5)
                    let c = map_(p)
                    h[oyz + x] = c.x
                    gb[oyz + x] = pack_float_to_byte(c.yzww * float4(255.0))
        ph = unsafe(addr(h[0]))
        pgb = unsafe(addr(gb[0]))

    glGenTextures(1, safe_addr(t3d))
    glBindTexture(GL_TEXTURE_3D, t3d)
    glTexImage3D(GL_TEXTURE_3D, 0, int(GL_R32F), idim.x, idim.y, idim.z, 0, GL_RED, GL_FLOAT, ph)
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE )
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE )
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE )
    glBindTexture(GL_TEXTURE_3D, 0u)

    glGenTextures(1, safe_addr(t3d_color))
    glBindTexture(GL_TEXTURE_3D, t3d_color)
    glTexImage3D(GL_TEXTURE_3D, 0, int(GL_RGBA), idim.x, idim.y, idim.z, 0, GL_RGBA, GL_UNSIGNED_BYTE, pgb)
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE )
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE )
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE )
    glBindTexture(GL_TEXTURE_3D, 0u)

    c_disp_size = uint3(idim+int3(7)/8)
    c_bmin = bmin
    c_step = step
    c_dim = idim

    if !GEN_IN_SOFTWARE
        glUseProgram(map_program)
        c_sdf := t3d
        c_sdf_color := t3d_color
        map_effect_bind_uniform(map_program)
        glDispatchCompute(c_disp_size.x, c_disp_size.y, c_disp_size.z)
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT)
        glUseProgram(0u)


def map_ ( pos:float3 )
    var res = xCol ( 1e10, 0.0 )
    res = xCol(sdPlane(pos.xyz-float3( 0.0,0.00, 0.0)), checker(pos) )
    // [[-2.25, 0, -0.25]] [[-1.75, 0.5, 0.25]]
    res = opU ( res, xCol(sdSphere(pos-float3(-2.0,0.25, 0.0),0.25),26.9) )
    // [[-0.35, 0, -3.5]]   [[0.35, 0.6, 1.5]]
    if sdBox( pos-float3(0.0,0.3,-1.0),float3(0.35,0.3,2.5) ) < res.x
        res = opU( res, xCol(sdBoundingBox( pos-float3( 0.0,0.25, 0.0), float3(0.3,0.25,0.2), 0.025 ), 16.9) )
        res = opU( res, xCol(sdTorus(      (pos-float3( 0.0,0.30, 1.0)).xzy, float2(0.25,0.05) ), 25.0) )
        res = opU( res, xCol(sdCone(        pos-float3( 0.0,0.45,-1.0), float2(0.6,0.8),0.45 ), 55.0) )
        res = opU( res, xCol(sdCappedCone(  pos-float3( 0.0,0.25,-2.0), 0.25, 0.25, 0.1 ), 13.67) )
        res = opU( res, xCol(sdSolidAngle(  pos-float3( 0.0,0.00,-3.0), float2(3,4)/5.0, 0.4 ), 49.13) )
    // [[-0.65, 0, -3.5]]   [[1.35, 0.6, 1.5]][]
    if sdBox( pos-float3(1.0,0.3,-1.0),float3(0.35,0.3,2.5) ) < res.x
        res = opU( res, xCol(sdCappedTorus((pos-float3( 1.0,0.30, 1.0))*float3(1,-1,1), float2(0.866025,-0.5), 0.25, 0.05), 8.5 ) )
        res = opU( res, xCol(sdBox(         pos-float3( 1.0,0.25, 0.0), float3(0.3,0.25,0.1) ), 3.0 ) )
        res = opU( res, xCol(sdCapsule(     pos-float3( 1.0,0.00,-1.0),float3(-0.1,0.1,-0.1), float3(0.2,0.4,0.2), 0.1  ), 31.0 ) )
        res = opU( res, xCol(sdCylinder(    pos-float3( 1.0,0.25,-2.0), float2(0.15,0.25) ), 8.0 ) )
        res = opU( res, xCol(sdHexPrism(    pos-float3( 1.0,0.2,-3.0), float2(0.2,0.05) ), 18.4 ) )
    // [[-1.35, 0, -3.5]]   [[0.65, 0.7, 1.5]]
    if sdBox( pos-float3(-1.0,0.35,-1.0),float3(0.35,0.35,2.5)) < res.x
        res = opU( res, xCol(sdPyramid(    pos-float3(-1.0,-0.6,-3.0), 1.0 ), 13.56 ) )
        res = opU( res, xCol(sdOctahedron( pos-float3(-1.0,0.15,-2.0), 0.35 ), 23.56 ) )
        res = opU( res, xCol(sdTriPrism(   pos-float3(-1.0,0.15,-1.0), float2(0.3,0.05) ), 43.5 ) )
// disabling elipsoid for now
//      res = opU( res, xCol(sdEllipsoid(  pos-float3(-1.0,0.25, 0.0), float3(0.2, 0.25, 0.05) ), 43.17 ) )
        res = opU( res, xCol(sdRhombus(   (pos-float3(-1.0,0.34, 1.0)).xzy, 0.15, 0.25, 0.04, 0.08 ), 17.0 ) )
    // float3(2.0,0.3,-1.0),float3(0.35,0.3,2.5)
    // [[1.65, 0, -3.5]] [[2.35, 0.6, 1.5]]
    if sdBox( pos-float3(2.0,0.3,-1.0),float3(0.35,0.3,2.5) ) < res.x
        res = opU( res, xCol(sdOctogonPrism(pos-float3( 2.0,0.2,-3.0), 0.2, 0.05), 51.8 ) )
        res = opU( res, xCol(sdCylinder(    pos-float3( 2.0,0.15,-2.0), float3(0.1,-0.1,0.0), float3(-0.2,0.35,0.1), 0.08), 31.2 ) )
        res = opU( res, xCol(sdCappedCone(  pos-float3( 2.0,0.10,-1.0), float3(0.1,0.0,0.0), float3(-0.2,0.40,0.1), 0.15, 0.05), 45.1 ) )
        res = opU( res, xCol(sdRoundCone(   pos-float3( 2.0,0.15, 0.0), float3(0.1,0.0,0.0), float3(-0.1,0.35,0.1), 0.15, 0.05), 51.7 ) )
        res = opU( res, xCol(sdRoundCone(   pos-float3( 2.0,0.20, 1.0), 0.2, 0.1, 0.3 ), 37.0 ) )
    return res

def map_h ( pos:float3 )
    var bmin = float3(-2.25, 0.0, -3.5) - float3(BORDER)
    var bmax = float3(2.35, 0.7, 1.5) + float3(BORDER)
    /*
    let c = (bmin+bmax) * 0.5
    let d = (bmax-bmin) * 0.5
    if sdBox(pos-c,d) > 0.
        return sdPlane(pos.xyz-float3( 0.0,0.00, 0.0))
    */
    let tx = (pos - bmin) * ( 1.0 / (bmax - bmin) )
    return texture(t_sdf, tx).x

def map_c ( pos:float3 )
    var bmin = float3(-2.25, 0.0, -3.5) - float3(BORDER)
    var bmax = float3(2.35, 0.7, 1.5) + float3(BORDER)
    let tx = (pos - bmin) * ( 1.0 / (bmax - bmin) )
    return texture(t_sdf_color, tx)

def map_h_ ( pos:float3 )
    if USE_ANALYTIC_SDF
        return map_(pos).x
    else
        return map_h(pos)

def map_c_ ( pos:float3 )
    if USE_ANALYTIC_SDF
        return map_(pos).yzww
    else
        return map_c(pos)

def calcSoftshadow ( ro,rd:float3; mint,tmax:float )
    var res = 1.0
    var t = mint
    var ph = 1e10
    for i in 0..32
        let h = map_h_ ( ro + rd*t )
        if FAST_SHADOWS
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
    return normalize( e.xyy*map_h_( pos + e.xyy ) +
					  e.yyx*map_h_( pos + e.yyx ) +
					  e.yxy*map_h_( pos + e.yxy ) +
					  e.xxx*map_h_( pos + e.xxx ) )

def castRay ( ro,rd:float3 )
    var tmin = 0.05
    var tmax = 25.0
    var res = -1.
    var t = tmin
    for i in 0..MAX_CAST_STEPS
        let h = map_h_(ro+rd*t)
        if abs(h)<(0.0001*t)
            res = t
        t += h
    let c = map_c_(ro+rd*t)
    return float4(t,c.x,c.y,c.z)

def calcAO ( pos,nor:float3 )
    var occ = 0.0
    var sca = 1.0
    for i in 0..5
        let h = 0.001 + 0.15*float(i)/4.0
        let d = map_h_( pos + h*nor )
        occ += (h-d)*sca
        sca *= 0.95
    return saturate(1.0-1.5*occ)

def render ( ro,rd:float3 )

    // bbox
    var bmin = float3(-2.25, 0.0, -3.5) - float3(BORDER)
    var bmax = float3(2.35, 0.7, 1.5) + float3(BORDER)
    if !is_intersecting([[Ray origin=ro, dir=rd]], [[AABB min=bmin, max=bmax]] )
        return float3(0.0,0.0,0.0)

    var col = float3(0.0)
    let res = castRay(ro,rd)
    let t = res.x
    let mat = res.yzw
    if t > -0.5
        let pos = ro + rd*t
        let nor = calcNormal(pos)
        let lig = normalize(float3(-0.1,0.3,0.6))
        let hal = normalize(lig-rd)
        let dif = saturate(dot(nor,lig)) * calcSoftshadow(pos,lig,0.01,3.0)
        let spe = pow(saturate(dot(nor,hal)),16.0) * dif * (0.04 + 0.96*pow(saturate(1.0+dot(hal,rd)),5.0))
        let ligCol = float3(1.0,0.7,0.5)
        col = mat * dif * ligCol + 12.0 * spe * ligCol
        let occ = calcAO(pos,nor)
        let amb = saturate(0.5+0.5*nor.y)
        col += mat*amb*occ*float3(0.0,0.08,0.1)
        col *= exp(-0.0005*t*t*t)
    return col

[fragment_program]
def fs_main
    let near = f_CameraNearFar.x
    let far = f_CameraNearFar.y
    var col = float3(0.0)
    if AA>1
        for m in range(AA)
            for n in range(AA)
                let p = f_texcoord * 2.0 - float2(1.0)
                let pos = float2(p.x,-p.y) + float2(m-AA/2,n-AA/2) * ( f_PixelSize / float(AA) )
                let ro = (f_Camera2World * float4(pos, -1.0, 1.0) * near).xyz
                let pd = (f_Camera2World * float4(pos * (far - near), far + near, far - near)).xyz
                let rd = normalize(pd - ro)
                let ccol = render(ro,rd)
                col += pow(ccol, float3(0.4545))
        col /= float(AA*AA)
    else
        let p = f_texcoord * 2.0 - float2(1.0)
        let pos = float2(p.x,-p.y)
        let ro = (f_Camera2World * float4(pos, -1.0, 1.0) * near).xyz
        let pd = (f_Camera2World * float4(pos * (far - near), far + near, far - near)).xyz
        let rd = normalize(pd - ro)
        col = render(ro,rd)
        col = pow(col, float3(0.4545))
    f_FragColor.xyz = col
    f_FragColor.w = 1.

var program, map_program : uint
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
    map_program = create_compute_shader_program(@@map_effect)
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
    let rot_x = quat_from_unit_vec_ang(normalize(float3(1.,0.,0.)), camera_rotation_xy.x*PI/180.)
    let rot_y = quat_from_unit_vec_ang(normalize(float3(0.,1.,0.)), camera_rotation_xy.y*PI/180.)
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

    gen_map_texture ( float3(-2.25, 0.0, -3.5), float3(2.35, 0.7, 1.5) )

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
        f_PixelSize = float2(1.) / float2(display_w, display_h)
        let v_view = camera_rotation(camera_rotation_xy) * compose(float4(-camera_position,1.), float4(0), float4(1.))
        let v_projection = perspective_rh_opengl( 45.0f*PI/180., aspect, f_CameraNearFar.x, f_CameraNearFar.y )
        f_Camera2World = inverse(v_projection * v_view)
        t_sdf.texture3D := t3d
        t_sdf_color.texture3D := t3d_color
        vs_main_bind_uniform(program)
        fs_main_bind_uniform(program)
        glBindVertexArray(vao)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null)
        glfwMakeContextCurrent(window)
        glfwSwapBuffers(window)