options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module opengl_gen public

require opengl/opengl_boost public
require geometry/geom_gen public
require daslib/safe_addr
require math

[vertex_buffer]
struct private PreviewVertex
    xyz : float3
    [[normalized=true]] normal : float3
    uv : float2

struct OpenGLGeometryFragment
    vao, vbo, ebo : uint
    primitive : uint
    total_indices : int
    total_vertices : int

def public finalize ( var frag:OpenGLGeometryFragment )
    with frag
        if vao != 0u
            glDeleteVertexArrays(1,unsafe(addr(vao)))
        if vbo != 0u
            glDeleteBuffers(1,unsafe(addr(vbo)))
        if ebo != 0u
            glDeleteBuffers(1,unsafe(addr(ebo)))

def public create_geometry_fragment ( var geometry:GeometryFragment; kill_geometry:bool=true )
    var fragment : OpenGLGeometryFragment
    with fragment
        glGenVertexArrays(1, safe_addr(vao))
        glBindVertexArray(vao)
        glGenBuffers(1, safe_addr(vbo))
        glBindBuffer(GL_ARRAY_BUFFER, vbo)
        glBufferData(GL_ARRAY_BUFFER, geometry.vertices, GL_STATIC_DRAW)
        bind_vertex_buffer(null,type<PreviewVertex>)
        glGenBuffers(1, safe_addr(ebo))
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.indices, GL_STATIC_DRAW)
        total_indices = length(geometry.indices)
        total_vertices = length(geometry.vertices)
        assert(geometry.prim==GeometryFragmentType triangles)
        primitive = GL_TRIANGLES
        if kill_geometry
            delete geometry
    return <- fragment

def public create_points_fragment ( var geometry:GeometryFragment; kill_geometry:bool=true )
    var fragment : OpenGLGeometryFragment
    with fragment
        glGenVertexArrays(1, safe_addr(vao))
        glBindVertexArray(vao)
        glGenBuffers(1, safe_addr(vbo))
        glBindBuffer(GL_ARRAY_BUFFER, vbo)
        glBufferData(GL_ARRAY_BUFFER, geometry.vertices, GL_STATIC_DRAW)
        bind_vertex_buffer(null,type<PreviewVertex>)
        total_indices = length(geometry.indices)
        total_vertices = length(geometry.vertices)
        primitive = GL_POINTS
        if kill_geometry
            delete geometry
    return <- fragment

def public draw_geometry_fragment ( frag:OpenGLGeometryFragment )
    with frag
        glBindVertexArray(vao)
        if frag.primitive == GL_TRIANGLES
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
            glDrawElements(primitive, total_indices, GL_UNSIGNED_INT, null)
        else
            glDrawArrays(primitive, 0, total_vertices)

def public create_texture ( var surf:ImageSurface; kill_pixels:bool=true )
    if length(surf.pixels) == 0
        return 0u
    var tex = 0u
    unsafe
        tex = load_image_from_bytes(surf.width, surf.height, addr(surf.pixels[0]))
    if kill_pixels
        delete surf.pixels
    return tex
