options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module points_draw private

require glfw/glfw_boost
require opengl/opengl_boost
require opengl/opengl_gen
require daslib/defer
require math
require daslib/math_boost

var [[in, location=0]] v_position : float3
var [[in, location=1]] v_normal   : float3
var [[in, location=2]] v_texcoord : float2
var [[uniform]] v_model : float4x4
var [[uniform]] v_view : float4x4
var [[uniform]] v_projection : float4x4
var [[inout]] f_color : float3
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

var points_program : uint
var point_sprite_texture, checkerboard_texture : uint

def public init_points
    points_program = create_shader_program(@@vs_preview,@@fs_preview)
    checkerboard_texture = gen_image_checkerboard(16,16, 0xff404040 , 0xff808080) |> create_texture
    point_sprite_texture = gen_image_point(32,0xffffffff) |> create_texture

def public draw_point_mesh ( mesh:OpenGLGeometryFragment; r,v,p:float4x4 )
    glEnable(GL_DEPTH_TEST)
    glEnable(GL_CULL_FACE)
    glCullFace(GL_BACK)
    glDepthFunc(GL_LEQUAL)
    glEnable(GL_PROGRAM_POINT_SIZE)
    v_view = v
    v_projection = p
    v_model = r
    f_tex := checkerboard_texture
    f_ptex := point_sprite_texture
    glUseProgram(points_program)
    vs_preview_bind_uniform(points_program)
    fs_preview_bind_uniform(points_program)
    draw_geometry_fragment(mesh)
    glUseProgram(0u)
    glDisable(GL_PROGRAM_POINT_SIZE)