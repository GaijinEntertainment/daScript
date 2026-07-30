options gen2
options indenting = 4
options persistent_heap = true
options gc

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_imgui_app
require vulkan/vulkan_gltf
require vulkan/vulkan_reflect
require vulkan/spirv_vulkan_shader
require daslib/shader_lingua_franca
require spirv/spirv_reflect
require imgui/imgui_file_dialog
require daslib/safe_addr
require daslib/fio
require live/live_gc
require math
require daslib/math_boost
require strings

// dasGLTF mesh viewer (Vulkan + Dear ImGui) — the vulkan_gltf sibling of mesh_viewer_opengl. Loads a
// .glb/.gltf, orbits it under the same metallic-roughness PBR (shared gltf_pbr_common math), and drives
// the model transform, animation playback and lighting from docked ImGui panels. The scene renders into
// an offscreen color+depth target (the vulkan_imgui_app swapchain pass has no depth attachment), which a
// fullscreen triangle then samples inside harness_end_frame's scene block, under the UI. Drag to orbit,
// scroll to zoom; "Open..." picks another model. Defaults to the shipped BoomBox (full PBR).

// ----- viewer state (all main-thread; init() loads, update() drives) -----
var g_scene      : GltfScene
var g_model      : GltfVkModel
var g_have_model = false

var g_center : float3
var g_radius = 1.0f

// orbit camera (mouse drag + scroll over the central viewport)
var g_cam_yaw   = 0.6f
var g_cam_pitch = 0.3f
var g_cam_dist  = 2.2f

// model transform (Model window): euler degrees + uniform scale, applied about the model centre
var g_rot_euler   : float3
var g_scale       = 1.0f
var g_auto_rotate = false

// animation (Model window)
var g_clip       = 0
var g_playing    = true
var g_anim_time  = 0.0f
var g_clip_names : array<string>

// lighting (Lighting window) — flat globals so safe_addr binds a stable top-level address
var g_light_dir       = float3(0.3, 0.7, 0.5)
var g_light_color     = float3(1.0, 1.0, 1.0)
var g_light_intensity = 3.0f
var g_light_ambient   = float3(0.15, 0.15, 0.18)
var g_env_amount      = 1.0f

var g_model_name = ""
var g_load_error = ""

// ----- offscreen scene target + composite (rebuilt on window resize) -----
var g_scene_pass   : RenderPass          // color+depth, color ends SHADER_READ_ONLY for the composite
var g_target_color : OffscreenTarget
var g_target_depth : OffscreenDepth
var g_target_fb    : Framebuffer
var g_target_w     = 0
var g_target_h     = 0

var g_quad_vert        : ShaderModule
var g_quad_frag        : ShaderModule
var g_quad_layouts     : array<DescriptorSetLayout>
var g_quad_pipe_layout : PipelineLayout
var g_quad_pipeline    : Pipeline
var g_quad_pool        : DescriptorPool
var g_quad_set         : VkDescriptorSet
var g_quad_sampler     : Sampler

let SCENE_COLOR_FMT = VkFormat.R8G8B8A8_UNORM   // the glTF shader sRGB-encodes; an _SRGB target would double-encode
let SCENE_DEPTH_FMT = VkFormat.D32_SFLOAT

// ----- composite shaders: fullscreen triangle sampling the offscreen scene color -----

var @out @location = 0 vq_uv : float2

[vulkan_vertex_shader(name="viewer_quad_vs_spv")]
def viewer_quad_vs {
    let xs = fixed_array(-1.0, 3.0, -1.0)
    let ys = fixed_array(-1.0, -1.0, 3.0)
    let idx = gl_VertexIndex
    let x = xs[idx]
    let y = ys[idx]
    gl_Position = float4(x, y, 0.0, 1.0)
    vq_uv = float2((x + 1.0) * 0.5, (y + 1.0) * 0.5)
}

var @set = 0 @binding = 0 vq_tex : sampler2D
var @in @location = 0 fq_uv : float2
var @out @location = 0 fq_color : float4

[vulkan_fragment_shader(name="viewer_quad_fs_spv")]
def viewer_quad_fs {
    fq_color = texture(vq_tex, fq_uv)
}

// ----- paths / loading -----

def media_dir : string {
    return "{get_das_root()}/examples/gltf/media"
}

def model_path : string {
    // get_command_line_arguments() includes the interpreter + script + harness flags, so pick the first
    // argument that actually names a glTF file; otherwise fall back to the shipped BoomBox.
    for (a in get_command_line_arguments()) {
        if (ends_with(a, ".glb") || ends_with(a, ".gltf")) {
            return a
        }
    }
    return "{media_dir()}/BoomBox.glb"
}

// Load a model, validate it, then swap it in (keeping the old one on failure) and reframe the camera.
// Shared by init() and the "Open..." dialog. The device is idle between frames (run_cmd_sync + the
// harness present both wait), so replacing GPU resources here is safe.
def load_model(path : string) {
    if (!stat(path).is_valid) {
        g_load_error = "not found: {base_name(path)}"
        return
    }
    var scene <- load_gltf(path)
    if (empty(scene.meshes)) {
        delete scene
        g_load_error = "no meshes in {base_name(path)}"
        return
    }
    var model <- gltf_vk_upload(harness_device(), harness_phys(), harness_queue(), harness_pool(),
        g_scene_pass, scene)
    delete g_model
    delete g_scene
    g_scene <- scene
    g_model <- model
    g_have_model = true
    // equirect HDR environment for image-based ambient (metals reflect it); CC0, shared with GL tutorial 12
    var env <- load_texture_hdr(harness_device(), harness_phys(), harness_queue(), harness_pool(),
        "{get_das_root()}/tutorials/_assets/hdri/cannon_2k.hdr")
    gltf_vk_set_environment(g_model, harness_device(), env)

    let bounds = gltf_scene_bounds(g_scene)
    g_center = (bounds._0 + bounds._1) * 0.5f
    g_radius = max(length(bounds._1 - bounds._0) * 0.5f, 0.001f)
    g_cam_dist = g_radius * 2.2f

    g_model_name = base_name(path)
    g_clip = 0
    g_anim_time = 0.0f
    g_playing = true
    g_rot_euler = float3(0.0, 0.0, 0.0)
    g_scale = 1.0f
    g_clip_names |> clear()
    g_clip_names |> reserve(length(g_scene.animations))
    for (a in g_scene.animations) {
        g_clip_names |> push(empty(a.name) ? "clip {length(g_clip_names)}" : a.name)
    }
    g_load_error = ""
}

def open_model_dialog {
    file_dialog_open(FileDialogConfig(title = "Open glTF model", start_dir = media_dir(),
        filters <- [FileDialogFilter(caption = "glTF (*.glb *.gltf)", ext = "glb;gltf")]))
}

// Restore the lighting controls to the GltfPbrLight defaults (single source of truth).
def reset_lighting {
    let d = GltfPbrLight()
    g_light_dir = d.direction
    g_light_color = d.color
    g_light_intensity = d.intensity
    g_light_ambient = d.ambient
    g_env_amount = d.environment
}

// T(centre) * R * S * T(-centre): rotate/scale about the model centre so the framed model stays framed.
def model_root_transform : float4x4 {
    let q = quat_from_euler(g_rot_euler * (PI / 180.0f))
    let rs = compose(float3(0.0, 0.0, 0.0), q, float3(g_scale))
    return translation(g_center) * rs * translation(-g_center)
}

// perspective_rh_0_to_1 gives Vulkan's z in [0,1]; Vulkan NDC additionally has +Y down, so flip Y
def perspective_vk_01(fovy, aspect, znear, zfar : float) : float4x4 {
    var m = perspective_rh_0_to_1(fovy, aspect, znear, zfar)
    m[1] = float4(m[1].x, -m[1].y, m[1].z, m[1].w)
    return m
}

// ----- offscreen target + composite plumbing -----

def rebuild_scene_target(w, h : int) {
    unsafe {
        delete g_target_fb
        delete g_target_color
        delete g_target_depth
    }
    var samples : VkSampleCountFlags
    samples._1 = true
    var extra : VkImageUsageFlags
    extra.sampled = true                        // the composite pass samples it
    g_target_color <- build_offscreen_target(harness_device(), harness_phys(), w, h, SCENE_COLOR_FMT, samples, extra)
    g_target_depth <- build_offscreen_depth(harness_device(), harness_phys(), w, h, SCENE_DEPTH_FMT)
    g_target_fb <- create_framebuffer(harness_device(), FramebufferCreateInfo(
        renderPass = weak_copy(g_scene_pass),
        pAttachments <- [weak_copy(g_target_color.view), weak_copy(g_target_depth.view)],
        width = uint(w),
        height = uint(h),
        layers = 1u))
    g_target_w = w
    g_target_h = h
    write_quad_set()
}

def write_quad_set {
    var writes : array<WriteDescriptorSet>
    var w = WriteDescriptorSet(
        dstSet = vk_value_to_boost(g_quad_set),
        dstBinding = 0u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER,
        descriptorCount = 1u)
    w.pImageInfo |> push(DescriptorImageInfo(
        sampler = weak_copy(g_quad_sampler),
        imageView = weak_copy(g_target_color.view),
        imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL))
    writes |> emplace(w)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(harness_device(), writes, no_copies)
    unsafe {
        delete writes
    }
}

def build_composite {
    g_quad_vert <- create_shader_module(harness_device(), viewer_quad_vs_spv)
    g_quad_frag <- create_shader_module(harness_device(), viewer_quad_fs_spv)
    var reflections <- [decode_reflection(viewer_quad_vs_spv_reflect), decode_reflection(viewer_quad_fs_spv_reflect)]
    g_quad_layouts <- build_descriptor_set_layouts(harness_device(), reflections)
    g_quad_pipe_layout <- build_pipeline_layout(harness_device(), g_quad_layouts, reflections)
    delete reflections
    let sz = harness_size()
    g_quad_pipeline <- create_graphics_pipeline_simple(harness_device(), harness_render_pass(),
        g_quad_pipe_layout, g_quad_vert, g_quad_frag, sz.x, sz.y, true)   // dynamic viewport: resize-proof
    let sci = SamplerCreateInfo(
        magFilter = VkFilter.LINEAR,
        minFilter = VkFilter.LINEAR,
        mipmapMode = VkSamplerMipmapMode.NEAREST,
        addressModeU = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeV = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeW = VkSamplerAddressMode.CLAMP_TO_EDGE,
        maxLod = 1.0f)
    g_quad_sampler <- create_sampler(harness_device(), sci)
    var dpci = DescriptorPoolCreateInfo(
        maxSets = 1u,
        pPoolSizes <- [DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)])
    g_quad_pool <- create_descriptor_pool(harness_device(), dpci)
    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(g_quad_pool)
    dsai.descriptorSetCount = 1u
    var raw_layout = boost_value_to_vk(g_quad_layouts[0])
    unsafe {
        dsai.pSetLayouts = addr(raw_layout)
        vk_check(vkAllocateDescriptorSets(boost_value_to_vk(harness_device()), dsai, addr(g_quad_set)), null)
    }
}

// ----- UI (identical layout to mesh_viewer_opengl) -----

// Dock the two panels down the left column (Model over Lighting); the central node stays passthru for the 3D.
def setup_default_layout(dock_id : uint) {
    DockBuilderRemoveNode(dock_id)
    DockBuilderAddDockSpaceNode(dock_id, ImGuiDockNodeFlags.None)
    let vp = GetMainViewport()
    DockBuilderSetNodeSize(dock_id, ImVec2(vp.Size.x, vp.Size.y))
    var left_id = 0u
    var center_id = 0u
    DockBuilderSplitNode(dock_id, ImGuiDir.Left, 0.24f, left_id, center_id)
    var left_top = 0u
    var left_bottom = 0u
    DockBuilderSplitNode(left_id, ImGuiDir.Up, 0.5f, left_top, left_bottom)
    DockBuilderDockWindow("Model", left_top)
    DockBuilderDockWindow("Lighting", left_bottom)
    DockBuilderFinish(dock_id)
}

def build_ui {
    dockspace(DOCK_ROOT, (flags = ImGuiDockNodeFlags.PassthruCentralNode)) {
        if (!DOCK_ROOT.has_initial_layout && DOCK_ROOT.dock_id != 0u) {
            setup_default_layout(DOCK_ROOT.dock_id)
            DOCK_ROOT.has_initial_layout = true
        }
        dock_window(MODEL_WIN, (text = "Model", closable = false, flags = ImGuiWindowFlags.None)) {
            text("{g_model_name}")
            same_line(SL_OPEN)
            if (button(OPEN_BTN, (text = "Open..."))) {
                open_model_dialog()
            }
            if (!empty(g_load_error)) {
                text("{g_load_error}")
            }
            text("meshes {length(g_scene.meshes)}   materials {length(g_scene.materials)}   nodes {length(g_scene.nodes)}")
            separator(SEP_ANIM)
            if (!empty(g_scene.animations)) {
                edit_combo(safe_addr(g_clip), (id = "CLIP", text = "Clip", items = g_clip_names))
                g_clip = clamp(g_clip, 0, length(g_scene.animations) - 1)
                edit_checkbox(safe_addr(g_playing), (id = "PLAY", text = "Play"))
                edit_slider_float(safe_addr(g_anim_time),
                    (id = "TIME", text = "Time", v_min = 0.0f, v_max = max(g_scene.animations[g_clip].duration, 0.001f)))
            } else {
                text("(no animations)")
            }
            separator(SEP_XFORM)
            edit_slider_float3(safe_addr(g_rot_euler), (id = "ROT", text = "Rotation", v_min = -180.0f, v_max = 180.0f))
            edit_slider_float(safe_addr(g_scale), (id = "SCALE", text = "Scale", v_min = 0.05f, v_max = 4.0f))
            edit_checkbox(safe_addr(g_auto_rotate), (id = "AUTOROT", text = "Auto-rotate"))
            separator(SEP_HINT)
            text("drag: orbit   scroll: zoom")
        }
        dock_window(LIGHT_WIN, (text = "Lighting", closable = false, flags = ImGuiWindowFlags.None)) {
            edit_slider_float3(safe_addr(g_light_dir), (id = "LDIR", text = "Direction", v_min = -1.0f, v_max = 1.0f))
            edit_slider_float(safe_addr(g_light_intensity), (id = "LINT", text = "Intensity", v_min = 0.0f, v_max = 10.0f))
            edit_color_edit3(safe_addr(g_light_color), (id = "LCOL", text = "Color"))
            edit_color_edit3(safe_addr(g_light_ambient), (id = "LAMB", text = "Ambient"))
            edit_slider_float(safe_addr(g_env_amount), (id = "ENV", text = "Environment", v_min = 0.0f, v_max = 2.0f))
            separator(SEP_LIGHT)
            if (button(RESET_LIGHT_BTN, (text = "Reset"))) {
                reset_lighting()
            }
        }
    }
}

// Mouse orbit + scroll zoom while the pointer isn't over an ImGui window, then advance/pose the animation.
def update_input_and_anim(dt : float) {
    let io & = unsafe(GetIO())
    if (!io.WantCaptureMouse) {
        if (IsMouseDown(ImGuiMouseButton.Left)) {
            g_cam_yaw -= io.MouseDelta.x * 0.01f
            g_cam_pitch = clamp(g_cam_pitch + io.MouseDelta.y * 0.01f, -1.5f, 1.5f)
        }
        if (io.MouseWheel != 0.0f) {
            g_cam_dist = clamp(g_cam_dist * (io.MouseWheel > 0.0f ? 0.9f : 1.1f), g_radius * 1.1f, g_radius * 40.0f)
        }
    }
    if (g_auto_rotate) {
        g_cam_yaw += dt * 0.5f
    }
    let anim_count = length(g_scene.animations)
    if (anim_count > 0) {
        let dur = g_scene.animations[g_clip].duration
        if (g_playing) {
            g_anim_time += dt
        }
        if (dur > 0.0f && g_anim_time >= dur) {
            g_anim_time -= dur * floor(g_anim_time / dur)
        }
        evaluate_animation(g_scene, g_clip, g_anim_time)
    }
}

// Render the glTF scene into the offscreen color+depth target (own submit; the harness pass then
// samples the result). Runs every frame — without a model it just clears the target.
def render_scene_offscreen {
    let sz = harness_size()
    if (sz.x != g_target_w || sz.y != g_target_h) {
        rebuild_scene_target(sz.x, sz.y)
    }
    let aspect = g_target_h != 0 ? float(g_target_w) / float(g_target_h) : 1.0f
    let cp = cos(g_cam_pitch)
    let eye = g_center + float3(cp * sin(g_cam_yaw), sin(g_cam_pitch), cp * cos(g_cam_yaw)) * g_cam_dist
    let view = look_at_rh(eye, g_center, float3(0.0, 1.0, 0.0))
    let proj = perspective_vk_01(45.0f * PI / 180.0f, aspect, g_radius * 0.05f, g_radius * 60.0f)
    let light = GltfPbrLight(direction = g_light_dir, color = g_light_color,
        intensity = g_light_intensity, ambient = g_light_ambient, environment = g_env_amount)

    if (g_have_model) {
        gltf_vk_update(harness_device(), g_model, g_scene, model_root_transform(), view, proj, eye, light)
    }
    run_cmd_sync(harness_device(), harness_pool(), harness_queue()) $(cmd) {
        var clears <- [clear_color(0.10f, 0.10f, 0.12f, 1.0f), clear_depth(1.0f)]
        record_render_pass(cmd, g_scene_pass, g_target_fb, full_area(g_target_w, g_target_h), clears) {
            if (g_have_model) {
                gltf_vk_render(cmd, g_model, g_scene, model_root_transform(), g_target_w, g_target_h)
            }
        }
        delete clears
    }
}

def draw_composite(cmd : CommandBuffer) {
    cmd_bind_pipeline(cmd, g_quad_pipeline)
    var sets <- [vk_value_to_boost(g_quad_set)]
    var no_dyn : array<uint>
    cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, g_quad_pipe_layout, 0u, sets, no_dyn)
    cmd_draw(cmd, 3u)
    delete no_dyn
    unsafe {
        delete sets
    }
}

[export]
def init {
    let path = model_path()
    harness_init("dasGLTF Mesh Viewer (Vulkan)", 1280, 720)
    var io & = unsafe(GetIO())
    io.ConfigFlags |= ImGuiConfigFlags.DockingEnable

    g_scene_pass <- create_render_pass_color_depth(harness_device(), SCENE_COLOR_FMT, SCENE_DEPTH_FMT,
        VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
    build_composite()
    let sz = harness_size()
    rebuild_scene_target(sz.x, sz.y)
    load_model(path)
}

[export]
def update {
    if (!harness_begin_frame()) {
        return
    }
    harness_new_frame()

    build_ui()
    if (file_dialog_draw() == FileDialogResult.confirmed) {
        load_model(file_dialog_selected_path())
    }
    let io & = unsafe(GetIO())
    update_input_and_anim(io.DeltaTime)
    render_scene_offscreen()

    harness_end_frame() $(cmd) {
        draw_composite(cmd)
    }
}

[export]
def shutdown {
    unsafe {
        delete g_model
        delete g_quad_pipeline
        delete g_quad_pipe_layout
        delete g_quad_pool
        delete g_quad_layouts
        delete g_quad_sampler
        delete g_quad_vert
        delete g_quad_frag
        delete g_target_fb
        delete g_target_color
        delete g_target_depth
        delete g_scene_pass
    }
    delete g_scene
    harness_shutdown()
}

[export]
def main {
    init()
    while (!exit_requested()) {
        update()
        maybe_collect_gc()
    }
    shutdown()
}
