options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module debug_draw private

require glfw/glfw_boost
require opengl/opengl_boost
require opengl/opengl_gen
require daslib/defer
require math
require daslib/math_boost

var private [[in, location=0]] v_position : float3
var private [[in, location=1]] v_normal   : float3
var private [[in, location=2]] v_texcoord : float2
var private [[uniform]] v_model : float4x4
var private [[uniform]] v_view : float4x4
var private [[uniform]] v_projection : float4x4
var private [[inout]] f_texcoord : float2
var private [[inout]] f_normal : float3
var private [[inout]] f_position : float3
var private [[uniform, stage=0]] f_tex : sampler2D
var private [[out]] f_FragColor : float4
var private [[uniform]] f_color : float3

[vertex_program]
def vs_preview_csg
    f_texcoord = v_texcoord
    f_normal = normalize ( float3x3(v_model) * v_normal )
    let tpos = v_model * float4(v_position, 1.0)
    f_position = tpos.xyz
    gl_Position = v_projection * v_view * tpos

[fragment_program]
def fs_preview_csg
    let pS = 24.0
    let light_dir = normalize(float3(-1,-2,-3))
    let camera_dir = normalize(f_position)
    let normal = normalize(f_normal)
    let k = -dot(light_dir,normal)
    let diffuse = float3(saturate(k))
    let refl = normalize ( reflect(camera_dir,normal) )
    let kS = saturate(dot(-light_dir,refl))
    let specular = float3(pow(kS,pS)*kS)
    f_FragColor.xyz = f_color * texture(f_tex,f_texcoord).xyz * diffuse + specular
    f_FragColor.w = 1.

[vertex_program]
def vs_preview
    f_texcoord = v_texcoord
    f_normal = normalize ( float3x3(v_model) * v_normal )
    let tpos = v_model * float4(v_position, 1.0)
    f_position = tpos.xyz
    gl_Position = v_projection * v_view * tpos

[fragment_program]
def fs_preview
    f_FragColor.xyz = f_color
    f_FragColor.w = 1.

var
    checkerboard_texture : uint
    preview_program : uint
    lines_program : uint
    sphere : OpenGLGeometryFragment
    cube : OpenGLGeometryFragment

def public gen_bbox_transform ( bmin, bmax : float3 )
    var tm : float4x4
    let db = (bmax - bmin) * 0.5
    tm[0] = float4(db.x, 0., 0., 0.)
    tm[1] = float4(0., db.y, 0., 0.)
    tm[2] = float4(0., 0., db.z, 0.)
    tm[3] = float4( (bmin+bmax)*0.5, 1. )
    return <- tm

def public debug_draw_bbox ( model, view, proj:float4x4; color:float3 )
    glDisable(GL_CULL_FACE)
    glDisable(GL_DEPTH_TEST)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
    glDepthMask(false)
    glUseProgram(lines_program)
    f_color = color
    f_tex := checkerboard_texture
    v_projection = proj
    v_view = view
    v_model = model
    vs_preview_bind_uniform(lines_program)
    fs_preview_bind_uniform(lines_program)
    draw_geometry_fragment(cube)
    glUseProgram(0u)
    glDepthMask(true)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
    glEnable(GL_DEPTH_TEST)
    glEnable(GL_CULL_FACE)

def public debug_draw_cube ( model, view, proj:float4x4; color:float3 )
    glUseProgram(preview_program)
    f_color = color
    f_tex := checkerboard_texture
    v_projection = proj
    v_view = view
    v_model = model
    vs_preview_csg_bind_uniform(preview_program)
    fs_preview_csg_bind_uniform(preview_program)
    draw_geometry_fragment(cube)
    glUseProgram(0u)

def public debug_draw_sphere ( model, view, proj:float4x4; color:float3 )
    glUseProgram(preview_program)
    f_color = color
    f_tex := checkerboard_texture
    v_projection = proj
    v_view = view
    v_model = model
    vs_preview_csg_bind_uniform(preview_program)
    fs_preview_csg_bind_uniform(preview_program)
    draw_geometry_fragment(sphere)
    glUseProgram(0u)


def public init_debug_draw
    preview_program = create_shader_program(@@vs_preview_csg,@@fs_preview_csg)
    lines_program = create_shader_program(@@vs_preview,@@fs_preview)
    sphere <- gen_sphere(32,16,false) |> create_geometry_fragment
    cube <- gen_cube() |> create_geometry_fragment
    checkerboard_texture = gen_image_checkerboard(16,16, 0xff404040 , 0xff808080) |> create_texture
    glBindTexture(GL_TEXTURE_2D, checkerboard_texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
    glBindTexture(GL_TEXTURE_2D, 0u)

