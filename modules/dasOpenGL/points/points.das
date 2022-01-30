require glfw/glfw_boost
require opengl/opengl_boost
require opengl/opengl_gen
require daslib/defer
require math
require daslib/math_boost
require daslib/safe_addr

var [[in, location=0]] v_position : float3
var [[in, location=1]] v_normal   : float3
var [[in, location=2]] v_texcoord : float2
var [[uniform]] v_model : float4x4
var [[uniform]] v_view : float4x4
var [[uniform]] v_projection : float4x4
var [[inout]] f_color : float3
var [[uniform]] f_camera_position : float3
var [[uniform, stage=0]] f_tex : sampler2D
var [[uniform, stage=1]] f_ptex : sampler2D
var [[out]] f_FragColor : float4

[vertex_program]
def vs_preview
    let normal = normalize ( float3x3(v_model) * v_normal )
    let tpos = v_model * float4(v_position, 1.0)
    gl_Position = v_projection * v_view * tpos
    gl_PointSize = 16.0 / gl_Position.w
    let pS = 24.0
    let light_dir = normalize(float3(-1,-2,-3))
    let camera_dir = normalize(tpos.xyz)
    let k = -dot(light_dir,normal)
    let diffuse = float3(saturate(k))
    let refl = normalize ( reflect(camera_dir,normal) )
    let kS = saturate(dot(-light_dir,refl))
    let specular = float3(pow(kS,pS)*kS)
    f_color = texture(f_tex,v_texcoord).xyz * diffuse + specular

[fragment_program(version=400)]
def fs_preview
    let ptex = texture(f_ptex, gl_PointCoord)
    if ptex.w < 0.01
        discard()
    f_FragColor.xyz = f_color
    f_FragColor.w = 1.0

require daslib/random
require daslib/algorithm

def gen_denisty ( frag : GeometryFragment )
    var surf : array<double>
    var total_surface = 0.0lf
    let total_triangles = length(frag.indices)/3
    surf |> reserve(total_triangles)
    for i in 0..total_triangles
        let i0 = frag.indices[i*3+0]
        let i1 = frag.indices[i*3+1]
        let i2 = frag.indices[i*3+2]
        let v0 = frag.vertices[i0].xyz
        let v1 = frag.vertices[i1].xyz
        let v2 = frag.vertices[i2].xyz
        let surface = double(dot(v1 - v0,v2 - v0) * 0.5)
        total_surface += surface
        surf |> push(surface)

    var density : array<int>
    density |> reserve(total_triangles)
    let ks = double(LCG_RAND_MAX_BIG) / total_surface
    var curs = 0
    for s in surf
        curs += int(s * ks)
        density |> push(curs)
    delete surf
    return <- density

def gen_point_cloud ( frag : GeometryFragment; numpoints : int )
    var dens <- gen_denisty(frag)
    var cloud : GeometryFragment
    cloud.prim = GeometryFragmentType points
    cloud.vertices |> reserve(numpoints)
    var seed = random_seed(13)
    random_int4(seed)
    for t in 0..numpoints
        let rnd = random_big_int(seed)
        let pidx = min(lower_bound(dens,rnd),length(dens)-1)
        let i0 = frag.indices[pidx*3+0]
        let i1 = frag.indices[pidx*3+1]
        let i2 = frag.indices[pidx*3+2]
        let v0 = frag.vertices[i0]
        let v1 = frag.vertices[i1]
        let v2 = frag.vertices[i2]
        // random point on triangle
        var u = random_float4(seed).xy
        if u.x+u.y > 1.0
            u.xy = float2(1.) - u.xy
        var vtx = [[GeometryPreviewVertex
            xyz = v0.xyz + (v1.xyz - v0.xyz)*u.x + (v2.xyz - v0.xyz)*u.y,
            normal = normalize ( v0.normal + (v1.normal - v0.normal)*u.x + (v2.normal - v0.normal)*u.y ),
            uv = v0.uv + (v1.uv - v0.uv)*u.x + (v2.uv - v0.uv)*u.y
        ]]
        cloud.vertices |> push(vtx)
    delete dens
    return <- cloud

[export]
def main
    if glfwInit()==0
		panic("can't init glfw")
    defer <|
        glfwTerminate()
    glfwInitOpenGL(4,3)
    var window = glfwCreateWindow(1280, 720, "Points", null, null)
    if window==null
		panic("can't create window")
    defer <|
        glfwDestroyWindow(window)
    glfwMakeContextCurrent(window)
    var program = create_shader_program(@@vs_preview,@@fs_preview)
    // var mesh <- gen_sphere(32,16,false) |> create_geometry_fragment
    let mesh_file_name = "{get_das_root()}/modules/dasBGFX/bgfx/bgfx/examples/assets/meshes/bunny.obj" // orb
    // var mesh <- load_obj_mesh(mesh_file_name) |> create_geometry_fragment
    var gmesh <- load_obj_mesh(mesh_file_name)
    var pcmesh <- gen_point_cloud(gmesh,1000000)
    var mesh <- pcmesh |> create_points_fragment
    let checkerboard_texture = gen_image_checkerboard(16,16, 0xff404040 , 0xff808080) |> create_texture
    let point_sprite_texture = gen_image_point(32,0xffffffff) |> create_texture
    glBindTexture(GL_TEXTURE_2D, checkerboard_texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
    glBindTexture(GL_TEXTURE_2D, 0u)
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
        glEnable(GL_PROGRAM_POINT_SIZE)
        // glEnable(GL_BLEND)
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        // glDepthMask(false)
        f_camera_position = float3(0.,camera_position_y+camera_distance*0.5,camera_distance)
        v_view = look_at_rh(f_camera_position,float3(0,0,0),float3(0,1,0))
        let rot = un_quat_from_unit_vec_ang(normalize(float3(0.,1.,0.)), camera_rotation*PI/180.)
        v_projection = perspective_rh_opengl( 45.0f*PI/180., aspect, 0.1f, 50.0f)
        f_tex := checkerboard_texture
        f_ptex := point_sprite_texture
        glUseProgram(program)
        v_model = compose(float4(0,0,0,0), rot, float4(1.0))
        vs_preview_bind_uniform(program)
        fs_preview_bind_uniform(program)
        draw_geometry_fragment(mesh)
        glUseProgram(0u)
        glfwMakeContextCurrent(window)
        glfwSwapBuffers(window)
