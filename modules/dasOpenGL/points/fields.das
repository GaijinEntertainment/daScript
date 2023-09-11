require glfw/glfw_boost
require opengl/opengl_boost
require opengl/opengl_gen
require opengl/opengl_ttf
require daslib/defer
require math
require daslib/math_boost
require daslib/safe_addr
require daslib/random

require sdf_soft
require debug_draw
require points_draw

var [[in, location=0]] v_position : float2
var [[in, location=1]] v_texcoord : float2
var [[inout]] f_texcoord : float2
var [[out]] f_FragColor : float4
var [[uniform]] f_w2l : float4x4
var [[uniform]] f_l2w : float4x4
var [[uniform]] f_w2sdf : float4x4
var [[uniform]] f_camera_position : float3
var [[uniform, stage=0]] t_sdf : sampler3D

[vertex_program]
def vs_effect
    f_texcoord = v_texcoord
    gl_Position = float4(v_position, 0.0, 1.0)

def cast_ray( ro,rd:float3; to,tmax:float )
	var t = to
    for i in 0..512
        let pos = ro + t*rd
        let tpos = (f_w2sdf * float4(pos,1.0)).xyz
	    let h = texture( t_sdf, tpos ).x
        if abs(h)<(0.005*t) || t>tmax
            return 1. // break
        t += h*0.99
    return 0. // t

[fragment_program]
def fs_effect
    let vuv = f_texcoord*2.0 - float2(1.0)
    let p = float3(vuv, 0.0)
    let wp = f_l2w * float4(p, 1.0)
    let dir = normalize(wp.xyz - f_camera_position)
    let t = cast_ray( f_camera_position, dir, 0.001, 5. )
    f_FragColor = float4(t, 0., 0., 1.0)

var font : Font

def preview ( scene:CsgScene; v_rot,v_view,v_projection:float4x4 )
    for node in scene.nodes
        if node.nodeType==CsgNodeType cube
            debug_draw_cube ( v_rot*node.local2world, v_view, v_projection, node.color.xyz )
        elif node.nodeType==CsgNodeType sphere
            debug_draw_sphere ( v_rot*node.local2world, v_view, v_projection, node.color.xyz )
        else
            assert(false,"invalid node type")

struct SdfNodePreview
    quads : array<FontVertex>
    model : float4x4
    bbox  : float4x4

struct SdfPrewiew
    nodes : array<SdfNodePreview>

def build_sdf_preview ( field:SdfField )
    var result : SdfPrewiew
    result.nodes |> reserve(field.dim.x*field.dim.y*field.dim.z)
    let xyzd = float3(field.dim)
    let xyzs = float3(1.0)/xyzd
    for z in 0..field.dim.z
        for y in 0..field.dim.y
            for x in 0..field.dim.x
                let d = field.data[x + y*field.dim.x + z*field.dim.x*field.dim.y]
                var node : SdfNodePreview

                let xyz = float3(x,y,z) / xyzd

                let bmin = (field.local2world * float4(xyz,1.)).xyz
                let bmax = (field.local2world * float4(xyz+xyzs,1.)).xyz

                node.bbox = gen_bbox_transform(bmin,bmax)

                let ws_xyz = (field.local2world * float4(xyz + xyzs*0.5,1.)).xyz
                node.quads <- font |> create_quads("{int(d*256.)}") // " {x} {y} {z}")
                let dim = quads_dim(node.quads)
                let qv = quads_view(node.quads, float2(3./(dim.vmax.x-dim.vmin.x)))
                let qm = compose ( float4(ws_xyz,1.), float4(1,0,0,0), float4(length(xyzs)*0.2))
                node.model = qm * qv

                result.nodes |> emplace(node)
    return <- result


def preview ( sdfp:SdfPrewiew; v_rot,v_view,v_projection:float4x4  )
    glDisable(GL_DEPTH_TEST)
    glDepthMask(false)
    glDisable(GL_CULL_FACE)
    for node in sdfp.nodes
        let mvp = v_projection * v_view * (v_rot * node.model)
        font |> draw_quads(node.quads, mvp)
    for node in sdfp.nodes
        debug_draw_bbox(
            v_rot * node.bbox,
            v_view, v_projection, float3(0.5,0.5,0.1)
        )
    glEnable(GL_DEPTH_TEST)
    glDepthMask(true)
    glEnable(GL_CULL_FACE)

[vertex_buffer]
struct EffectVertex
    xy : float2
    uv : float2

let effect_vertices = [[EffectVertex
    xy=float2(-1.0, 1.0), uv=float2(0.0,0.0);
    xy=float2( 1.0, 1.0), uv=float2(1.0,0.0);
    xy=float2( 1.0,-1.0), uv=float2(1.0,1.0);
    xy=float2(-1.0,-1.0), uv=float2(0.0,1.0)
]]

let effect_indices = [[int 0; 1; 2; 2; 3; 0]]

def map3d ( field : SdfField; txyz:float3 )
    let fxyz = txyz - floor(txyz)
    let xyz = int3(txyz)
    let t000 = field |> at(xyz+int3(0,0,0))
    let t001 = field |> at(xyz+int3(0,0,1))
    let t010 = field |> at(xyz+int3(0,1,0))
    let t011 = field |> at(xyz+int3(0,1,1))
    let t100 = field |> at(xyz+int3(1,0,0))
    let t101 = field |> at(xyz+int3(1,0,1))
    let t110 = field |> at(xyz+int3(1,1,0))
    let t111 = field |> at(xyz+int3(1,1,1))
    let t00 = lerp(t000,t001,fxyz.x)
    let t01 = lerp(t010,t011,fxyz.x)
    let t10 = lerp(t100,t101,fxyz.x)
    let t11 = lerp(t110,t111,fxyz.x)
    let t0 = lerp(t00,t01,fxyz.y)
    let t1 = lerp(t10,t11,fxyz.y)
    return lerp(t0, t1, fxyz.x)


def normal3d ( field:SdfField; p:float3 )
    let eps = 0.01
    let h = float2(eps,0.)
    return normalize( float3(map3d(field, p+h.xyy) - map3d(field, p-h.xyy),
                             map3d(field, p+h.yxy) - map3d(field, p-h.yxy),
                             map3d(field, p+h.yyx) - map3d(field, p-h.yyx) ) )

def gen_point_cloud ( field : SdfField; step:int; wst:float )
    var cloud : GeometryFragment
    cloud.prim = GeometryFragmentType points
    let xyzd = float3(field.dim)
    for z in range(field.dim.z*step)
        for y in range(field.dim.y*step)
            for x in range(field.dim.x*step)
                let xyz = float3(x,y,z)
                let txyz = xyz / float3(step)
                let d = map3d(field, txyz)
                if abs(d) < wst
                    let n = normal3d(field, txyz)
                    var vtx = [[GeometryPreviewVertex
                        xyz = (field.local2world * float4(txyz / xyzd,1.)).xyz,
                        normal = normalize ( (field.local2world * float4(n,0.)).xyz )
                    ]]
                    cloud.vertices |> push(vtx)
    gen_bbox(cloud)
    print("total {cloud.vertices |> length} points\n")
    return <- cloud

[export]
def main
    // make a CSG scene
    var scene : CsgScene
    scene |> addCube(
        float3(-0.5,-0.5,0.),
        float3(1,1,1),
        quat_from_unit_vec_ang(float3(0.,1.,0.), 0.)
    )
    scene |> addSphere(
        float3(0.,0.2,-1.),
        float3(1.2,1.2,1.2),
        quat_from_unit_vec_ang(float3(0.,1.,0.), 0.)
    )
    scene |> seal()

    var sdf <- SdfField ( scene.dmin, scene.dmax, 1.)
    sdf |> seal(scene)

    if glfwInit()==0
		panic("can't init glfw")
    defer <|
        glfwTerminate()
    glfwInitOpenGL(3,3)
    var window = glfwCreateWindow(1280, 720, "SDF preview", null, null)
    if window==null
		panic("can't create window")
    defer <|
        glfwDestroyWindow(window)
    glfwMakeContextCurrent(window)

    create_ttf_objects()
    font <- load_ttf("{get_das_root()}/modules/dasBGFX/bgfx/bgfx/examples/runtime/font/droidsansmono.ttf")

    var sdfp <- build_sdf_preview(sdf)

    let display_program = create_shader_program(@@vs_effect,@@fs_effect)

    var t3d : uint
    glGenTextures(1,safe_addr(t3d))
    glBindTexture(GL_TEXTURE_3D, t3d)
    glTexImage3D(GL_TEXTURE_3D, 0, int(GL_R32F), sdf.dim.x, sdf.dim.y, sdf.dim.z, 0, GL_RED, GL_FLOAT, unsafe(addr(sdf.data[0])))
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE )
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE )
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE )
    glBindTexture(GL_TEXTURE_3D, 0u)

    var vao, vbo, ebo : uint
    glGenVertexArrays(1, safe_addr(vao))
    glBindVertexArray(vao);
    glGenBuffers(1, safe_addr(vbo))
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBufferData(GL_ARRAY_BUFFER, effect_vertices, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<EffectVertex>)
    glGenBuffers(1, safe_addr(ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, effect_indices, GL_STATIC_DRAW)

    var cloud <- gen_point_cloud(sdf, 30, 0.25)
    var point_mesh <- cloud |> create_points_fragment

    init_points()

    init_debug_draw()
    var camera_distance = 4.
    var camera_rotation : float
    var camera_position_y : float
    var cursor_position_xy = glfwGetCursorPos(window)
    var cursor_enabled = false
    unsafe
        glfwSetScrollCallback(window) <| @ [[&camera_distance]] ( w, x, y )
            if y>0.0lf
                camera_distance *= 1.1
            else
                camera_distance /= 1.1
        glfwSetCursorPosCallback(window) <| @ [[&cursor_position_xy,&cursor_enabled,&camera_position_y,&camera_rotation,&camera_distance]] ( w, x, y )
            if cursor_enabled
                let cxy = glfwGetCursorPos(window)
                let dxy = cxy - cursor_position_xy
                var display_w, display_h : int
                glfwGetFramebufferSize(window, display_w, display_h)
                camera_position_y += dxy.y * 4. * camera_distance/float(display_h)
                camera_rotation += dxy.x * 720. / float(display_w)
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
        // render
        glViewport(0, 0, display_w, display_h)
        glClearColor(0.2, 0.2, 0.2, 1.0)
        glClearDepth(1.0lf)
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
        glEnable(GL_DEPTH_TEST)
        glEnable(GL_CULL_FACE)
        glCullFace(GL_BACK)
        glDepthFunc(GL_LEQUAL)

        f_camera_position = float3(0.,camera_position_y+camera_distance*0.5,camera_distance)
        let v_view = look_at_rh(f_camera_position,float3(0,0,0),float3(0,1,0))
        let rot = quat_from_unit_vec_ang(normalize(float3(0.,1.,0.)), camera_rotation*PI/180.)
        let v_rot = compose(float4(0,0,0,1),rot,float4(1,1,1,1))
        let v_projection = perspective_rh_opengl( 45.0f*PI/180., aspect, 0.1f, 50.0f)

        /*

        preview(scene, v_rot, v_view, v_projection)

        debug_draw_bbox(
            v_rot * gen_bbox_transform(scene.dmin, scene.dmax),
            v_view, v_projection, float3(1,1,1)
        )

        debug_draw_bbox(
            v_rot * gen_bbox_transform(sdf.dmin, sdf.dmax),
            v_view, v_projection, float3(1,1,1)
        )

        */


        // preview ( sdfp, v_rot, v_view, v_projection )


        draw_point_mesh(point_mesh, v_rot, v_view, v_projection)

        // display it
/*
        f_w2l = v_projection * v_view
        f_l2w = inverse(f_w2l)

        glDisable(GL_DEPTH_TEST)
        glDisable(GL_CULL_FACE)
        glDepthMask(false)
        glViewport(0, 0, display_w, display_h)
        glUseProgram(display_program)
        f_w2sdf := sdf.world2local
        t_sdf := t3d
        vs_effect_bind_uniform(display_program)
        fs_effect_bind_uniform(display_program)
        glBindVertexArray(vao)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null)
*/
        glfwMakeContextCurrent(window)
        glfwSwapBuffers(window)
