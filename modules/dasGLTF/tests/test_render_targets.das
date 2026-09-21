options gen2
options persistent_heap
require dastest/testing_boost public
require daslib/defer
require daslib/safe_addr
require glfw/glfw_boost
require gltf/gltf_pbr
require gltf/gltf_types
require math

def private render_target(internal : int; format, attachment : uint) : uint {
    var texture : uint
    glGenTextures(1, safe_addr(texture))
    glBindTexture(GL_TEXTURE_2D, texture)
    glTexImage2D(GL_TEXTURE_2D, 0, internal, 4, 4, 0, format, GL_HALF_FLOAT, null)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0)
    return texture
}

def private render_fixture_scene : GltfScene {
    var scene : GltfScene
    var mesh : GltfMesh
    var primitive = GltfPrimitive()
    primitive.vertices <- [
        GltfVertex(position = float3(-1.0, -1.0, 0.0), normal = float3(0.0, 0.0, 1.0), tangent = float4(1.0, 0.0, 0.0, 1.0), uv0 = float2(0.5)),
        GltfVertex(position = float3(3.0, -1.0, 0.0), normal = float3(0.0, 0.0, 1.0), tangent = float4(1.0, 0.0, 0.0, 1.0), uv0 = float2(0.5)),
        GltfVertex(position = float3(-1.0, 3.0, 0.0), normal = float3(0.0, 0.0, 1.0), tangent = float4(1.0, 0.0, 0.0, 1.0), uv0 = float2(0.5))
    ]
    primitive.indices <- [0u, 1u, 2u]
    mesh.primitives |> emplace(primitive)
    scene.meshes |> emplace(mesh)
    scene.nodes |> emplace(GltfNode(mesh = 0, world = identity4x4()))
    return <- scene
}

def private read_rgba(attachment : uint) : float4 {
    glReadBuffer(attachment)
    var pixel : float4
    glReadPixels(1, 1, 1, 1, GL_RGBA, GL_FLOAT, unsafe(addr(pixel)))
    return pixel
}

def private read_motion : float2 {
    glReadBuffer(GL_COLOR_ATTACHMENT2)
    var pixel : float2
    glReadPixels(1, 1, 1, 1, GL_RG, GL_FLOAT, unsafe(addr(pixel)))
    return pixel
}

def private check_surface_motion(t : T?; var renderer : GltfPbrRenderer; model : GltfGlModel; var scene : GltfScene){
    glClearColor(0.0, 0.0, 0.0, 0.0); glClear(GL_COLOR_BUFFER_BIT)
    gltf_pbr_render_normals(renderer, model, scene, identity4x4(), identity4x4(), false, false, true)
    t |> equal(renderer.normalProgram, 0u, "surface mode does not create an unused normal program")
    t |> equal(renderer.motionProgram, 0u, "surface mode does not create the optional motion program")
    let normal_roughness = read_rgba(GL_COLOR_ATTACHMENT0)
    let albedo_metallic = read_rgba(GL_COLOR_ATTACHMENT1)
    t |> success(abs(normal_roughness.x - 0.5) < 0.01 && abs(normal_roughness.y - 0.5) < 0.01 && normal_roughness.z > 0.99 && normal_roughness.w > 0.99,
        "surface attachment carries encoded normal and roughness")
    t |> success(albedo_metallic.x > 0.99 && albedo_metallic.y > 0.99 && albedo_metallic.z > 0.99 && albedo_metallic.w > 0.99,
        "material attachment carries base color and metallic")

    var inscope previous = GltfMotionPose()
    gltf_motion_capture(previous, scene, identity4x4())
    gltf_pbr_render_normals(renderer, model, scene, identity4x4(), identity4x4(), false, false, false, true,
        unsafe(addr(previous)), identity4x4(), float2(0.0), 1)
    t |> equal(float2(0.0), read_motion(), "valid unchanged history emits zero motion")
    scene.nodes[0].world[3].x = 0.5
    gltf_pbr_render_normals(renderer, model, scene, identity4x4(), identity4x4(), false, false, false, true,
        unsafe(addr(previous)), identity4x4(), float2(0.0), 1)
    let translated_motion = read_motion()
    t |> success(abs(translated_motion.x + 0.25) < 0.01 && abs(translated_motion.y) < 0.01,
        "translated geometry emits previous-minus-current UV motion")
    scene.nodes[0].world = identity4x4()
    var inscope missing = GltfMotionPose()
    gltf_pbr_render_normals(renderer, model, scene, identity4x4(), identity4x4(), false, false, false, true,
        unsafe(addr(missing)), identity4x4(), float2(0.0), 1)
    t |> equal(float2(2.0), read_motion(), "missing node history emits rejection sentinel")
    gltf_pbr_render_normals(renderer, model, scene, identity4x4(), identity4x4(), false, false, false, true,
        null, identity4x4(), float2(0.0), 1)
    t |> equal(float2(2.0), read_motion(), "null history emits rejection sentinel")
    gltf_pbr_render_normals(renderer, model, scene, identity4x4(), identity4x4(), false, false, false, true,
        null, identity4x4(), float2(0.0), 0)
    t |> equal(float2(4.0), read_motion(), "static/camera history emits reconstruction sentinel")
    gltf_pbr_render_normals(renderer, model, scene, identity4x4(), identity4x4(), false, false, false, true,
        null, identity4x4(), float2(0.0), 2)
    t |> equal(float2(2.0), read_motion(), "invalid history emits rejection sentinel")

}

def private check_shadow_filters(t : T?; var renderer : GltfPbrRenderer; model : GltfGlModel; scene : GltfScene){
    var color_only = GL_COLOR_ATTACHMENT0
    glDrawBuffers(1, safe_addr(color_only))
    glClearColor(0.0, 0.0, 0.0, 0.0); glClear(GL_COLOR_BUFFER_BIT)
    renderer.shadowFilter = 1
    let light = GltfPbrLight(direction = float3(0.0, 0.0, 1.0), color = float3(0.0), intensity = 0.0,
        ambient = float3(1.0), environment = 0.0)
    gltf_pbr_render(renderer, model, scene, identity4x4(), identity4x4(), float3(0.0, 0.0, 1.0), light)
    let fallback_pixel = read_rgba(GL_COLOR_ATTACHMENT0)
    t |> success(fallback_pixel.x > 0.99 && fallback_pixel.y > 0.99 && fallback_pixel.z > 0.99,
        "hardware shadow mode renders through the lit fallback when no shadow map is attached")

    var shadow_texture : uint
    glGenTextures(1, safe_addr(shadow_texture))
    defer(){glDeleteTextures(1, safe_addr(shadow_texture))}
    glBindTexture(GL_TEXTURE_2D, shadow_texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE)
    var shadow_depth = 1.0
    glTexImage2D(GL_TEXTURE_2D, 0, int(GL_DEPTH_COMPONENT32F), 1, 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, unsafe(addr(shadow_depth)))
    gltf_pbr_set_shadow(renderer, shadow_texture, identity4x4(), 1, 0.0, 1.0)
    let direct_light = GltfPbrLight(direction = float3(0.0, 0.0, 1.0), color = float3(1.0), intensity = 4.0,
        ambient = float3(0.0), environment = 0.0)
    glClear(GL_COLOR_BUFFER_BIT)
    gltf_pbr_render(renderer, model, scene, identity4x4(), identity4x4(), float3(0.0, 0.0, 1.0), direct_light)
    let lit_pixel = read_rgba(GL_COLOR_ATTACHMENT0)
    shadow_depth = 0.0
    glBindTexture(GL_TEXTURE_2D, shadow_texture)
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, unsafe(addr(shadow_depth)))
    glClear(GL_COLOR_BUFFER_BIT)
    gltf_pbr_render(renderer, model, scene, identity4x4(), identity4x4(), float3(0.0, 0.0, 1.0), direct_light)
    let shadowed_pixel = read_rgba(GL_COLOR_ATTACHMENT0)
    t |> success(lit_pixel.x > 0.1 && shadowed_pixel.x < 0.01 && lit_pixel.x > shadowed_pixel.x + 0.1,
        "hardware comparison shadow map toggles direct light")
    t |> equal(uint(GL_NO_ERROR), glGetError())
}

[test]
def gltf_surface_motion_and_shadow_fallback_render_on_gl(t : T?){
    if (glfwInit() == 0){t |> skip("GLFW display unavailable"); return }
    glfwInitOpenGL(3, 3)
    glfwWindowHint(int(GLFW_VISIBLE), GLFW_FALSE)
    let window = glfwCreateWindow(32, 32, "glTF render targets", null, null)
    if (window == null){glfwTerminate(); t |> skip("OpenGL context unavailable"); return }
    glfwMakeContextCurrent(window)
    defer(){gl_binding_clear(); glfwDestroyWindow(window); glfwTerminate()}

    var scene <- render_fixture_scene()
    defer(){delete scene}
    var model <- upload_gltf(scene)
    defer(){delete model}
    var scratch_texture : uint
    glGenTextures(1, safe_addr(scratch_texture))
    glActiveTexture(GL_TEXTURE0 + 3u)
    glBindTexture(GL_TEXTURE_2D, scratch_texture)
    var active_before, binding_before : int
    glGetIntegerv(GL_ACTIVE_TEXTURE, safe_addr(active_before))
    glGetIntegerv(GL_TEXTURE_BINDING_2D, safe_addr(binding_before))
    var renderer <- gltf_pbr_create()
    defer(){
        delete renderer
        glDeleteTextures(1, safe_addr(scratch_texture))
    }
    var active_after, binding_after : int
    glGetIntegerv(GL_ACTIVE_TEXTURE, safe_addr(active_after))
    glGetIntegerv(GL_TEXTURE_BINDING_2D, safe_addr(binding_after))
    t |> equal(active_before, active_after, "renderer creation preserves the active texture unit")
    t |> equal(binding_before, binding_after, "renderer creation preserves the active unit's 2D binding")

    var fbo : uint
    glGenFramebuffers(1, safe_addr(fbo))
    glBindFramebuffer(GL_FRAMEBUFFER, fbo)
    let surface = render_target(int(GL_RGBA16F), GL_RGBA, GL_COLOR_ATTACHMENT0)
    let material = render_target(int(GL_RGBA16F), GL_RGBA, GL_COLOR_ATTACHMENT1)
    let motion = render_target(int(GL_RG16F), GL_RG, GL_COLOR_ATTACHMENT2)
    defer(){
        var a = surface; var b = material; var c = motion
        glDeleteTextures(1, safe_addr(a)); glDeleteTextures(1, safe_addr(b)); glDeleteTextures(1, safe_addr(c))
        glDeleteFramebuffers(1, safe_addr(fbo))
    }
    var mrt = fixed_array(GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2)
    glDrawBuffers(3, safe_addr(mrt[0]))
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){t |> skip("float MRT unavailable"); return }
    glViewport(0, 0, 4, 4)
    glDisable(GL_DEPTH_TEST)

    check_surface_motion(t, renderer, model, scene)

    check_shadow_filters(t, renderer, model, scene)
}
