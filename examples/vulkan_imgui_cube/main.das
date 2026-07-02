options gen2
options indenting = 4

// Spinning cube rendered with dasVulkan, with a Dear ImGui control panel whose slider drives the
// rotation speed -- UI built with the imgui_boost_v2 widget DSL and rasterized by a renderer backend
// written entirely in daslang (vulkan/vulkan_imgui_driver). The vulkan_imgui_app harness owns the
// window/device/swapchain and wires the live stack, so the app is fully drivable under daslang-live:
// `imgui_snapshot` dumps the widget state, `imgui_force_set {"target":"CUBE_WIN/SPEED","value":4.0}`
// spins the cube faster, `screenshot {"file":"..."}` captures the frame. Cube and UI share one
// swapchain render pass (the cube draws in the harness_end_frame scene block).
//
// Run (from the repo root):
//   daslang -project_root examples/vulkan_imgui_cube examples/vulkan_imgui_cube/main.das
// Live:
//   daslang-live -project_root examples/vulkan_imgui_cube examples/vulkan_imgui_cube/main.das
// Close the window to exit; P saves screenshot.png.

require vulkan/vulkan_imgui_app
require vulkan/vulkan_reflect
require cube_shaders
require daslib/safe_addr
require math

let WIN_W = 1100
let WIN_H = 760

// ===== cube geometry: pos.xyz (+0) + normal.xyz (+12), 24 verts (4 per face), 36 indices =====

let private cube_vertices : array<float> <- [
    // +Z
    -0.5, -0.5, 0.5,  0.0, 0.0, 1.0,    0.5, -0.5, 0.5,  0.0, 0.0, 1.0,
     0.5, 0.5, 0.5,  0.0, 0.0, 1.0,    -0.5, 0.5, 0.5,  0.0, 0.0, 1.0,
    // -Z
     0.5, -0.5, -0.5,  0.0, 0.0, -1.0,  -0.5, -0.5, -0.5,  0.0, 0.0, -1.0,
    -0.5, 0.5, -0.5,  0.0, 0.0, -1.0,    0.5, 0.5, -0.5,  0.0, 0.0, -1.0,
    // +X
     0.5, -0.5, 0.5,  1.0, 0.0, 0.0,     0.5, -0.5, -0.5,  1.0, 0.0, 0.0,
     0.5, 0.5, -0.5,  1.0, 0.0, 0.0,     0.5, 0.5, 0.5,  1.0, 0.0, 0.0,
    // -X
    -0.5, -0.5, -0.5,  -1.0, 0.0, 0.0,  -0.5, -0.5, 0.5,  -1.0, 0.0, 0.0,
    -0.5, 0.5, 0.5,  -1.0, 0.0, 0.0,    -0.5, 0.5, -0.5,  -1.0, 0.0, 0.0,
    // +Y
    -0.5, 0.5, 0.5,  0.0, 1.0, 0.0,      0.5, 0.5, 0.5,  0.0, 1.0, 0.0,
     0.5, 0.5, -0.5,  0.0, 1.0, 0.0,    -0.5, 0.5, -0.5,  0.0, 1.0, 0.0,
    // -Y
    -0.5, -0.5, -0.5,  0.0, -1.0, 0.0,   0.5, -0.5, -0.5,  0.0, -1.0, 0.0,
     0.5, -0.5, 0.5,  0.0, -1.0, 0.0,   -0.5, -0.5, 0.5,  0.0, -1.0, 0.0]

let private cube_indices : array<uint16> <- [
    uint16(0), uint16(1), uint16(2),  uint16(0), uint16(2), uint16(3),
    uint16(4), uint16(5), uint16(6),  uint16(4), uint16(6), uint16(7),
    uint16(8), uint16(9), uint16(10), uint16(8), uint16(10), uint16(11),
    uint16(12), uint16(13), uint16(14), uint16(12), uint16(14), uint16(15),
    uint16(16), uint16(17), uint16(18), uint16(16), uint16(18), uint16(19),
    uint16(20), uint16(21), uint16(22), uint16(20), uint16(22), uint16(23)]

// ===== column-major float4x4 math =====

def perspective_vk(fov_y, aspect, znear, zfar : float) : float4x4 {
    let f = 1.0f / tan(fov_y * 0.5f)
    let nf = 1.0f / (znear - zfar)
    var m : float4x4
    m[0] = float4(f / aspect, 0.0f, 0.0f, 0.0f)
    m[1] = float4(0.0f, -f, 0.0f, 0.0f)
    m[2] = float4(0.0f, 0.0f, zfar * nf, -1.0f)
    m[3] = float4(0.0f, 0.0f, znear * zfar * nf, 0.0f)
    return m
}

def look_at_rh(eye, center, up : float3) : float4x4 {
    let f = normalize(center - eye)
    let s = normalize(cross(f, up))
    let u = cross(s, f)
    var m : float4x4
    m[0] = float4(s.x, u.x, -f.x, 0.0f)
    m[1] = float4(s.y, u.y, -f.y, 0.0f)
    m[2] = float4(s.z, u.z, -f.z, 0.0f)
    m[3] = float4(-dot(s, eye), -dot(u, eye), dot(f, eye), 1.0f)
    return m
}

def rotate_y(angle : float) : float4x4 {
    let c = cos(angle)
    let s = sin(angle)
    var m : float4x4
    m[0] = float4(c, 0.0f, -s, 0.0f)
    m[1] = float4(0.0f, 1.0f, 0.0f, 0.0f)
    m[2] = float4(s, 0.0f, c, 0.0f)
    m[3] = float4(0.0f, 0.0f, 0.0f, 1.0f)
    return m
}

// ===== cube pipeline: pos+normal vertex input, back-face cull, no depth, no blend, dynamic vp =====

def build_cube_pipeline(device : Device; render_pass : RenderPass; layout : PipelineLayout;
                        vert, frag : ShaderModule) : Pipeline {
    var stages : array<VkPipelineShaderStageCreateInfo>
    stages |> resize(2)
    stages[0] = VkPipelineShaderStageCreateInfo()
    stages[0].stage.vertex = true
    stages[0].module_ = boost_value_to_vk(vert)
    stages[0].pName = "main"
    stages[1] = VkPipelineShaderStageCreateInfo()
    stages[1].stage.fragment = true
    stages[1].module_ = boost_value_to_vk(frag)
    stages[1].pName = "main"

    var vbind : VkVertexInputBindingDescription
    vbind.binding = 0u
    vbind.stride = 24u
    vbind.inputRate = VkVertexInputRate.VERTEX
    var vbindings : array<VkVertexInputBindingDescription>
    vbindings |> push(vbind)
    var a_pos : VkVertexInputAttributeDescription
    a_pos.location = 0u
    a_pos.format = VkFormat.R32G32B32_SFLOAT
    a_pos.offset = 0u
    var a_nrm : VkVertexInputAttributeDescription
    a_nrm.location = 1u
    a_nrm.format = VkFormat.R32G32B32_SFLOAT
    a_nrm.offset = 12u
    var vattrs : array<VkVertexInputAttributeDescription>
    vattrs |> push(a_pos)
    vattrs |> push(a_nrm)

    var vinput = VkPipelineVertexInputStateCreateInfo()
    vinput.vertexBindingDescriptionCount = 1u
    vinput.vertexAttributeDescriptionCount = 2u
    var iasm = VkPipelineInputAssemblyStateCreateInfo()
    iasm.topology = VkPrimitiveTopology.TRIANGLE_LIST

    var vpstate = VkPipelineViewportStateCreateInfo()
    vpstate.viewportCount = 1u
    vpstate.scissorCount = 1u

    var raster = VkPipelineRasterizationStateCreateInfo()
    raster.polygonMode = VkPolygonMode.FILL
    raster.cullMode.back = true
    raster.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    raster.lineWidth = 1.0f
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true

    var blend_att : VkPipelineColorBlendAttachmentState
    blend_att.colorWriteMask.r = true
    blend_att.colorWriteMask.g = true
    blend_att.colorWriteMask.b = true
    blend_att.colorWriteMask.a = true
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 1u

    var dyn_states : array<VkDynamicState>
    dyn_states |> push(VkDynamicState.VIEWPORT)
    dyn_states |> push(VkDynamicState.SCISSOR)
    var dynstate = VkPipelineDynamicStateCreateInfo()
    dynstate.dynamicStateCount = 2u

    var gp = VkGraphicsPipelineCreateInfo()
    gp.stageCount = 2u
    gp.layout = boost_value_to_vk(layout)
    gp.renderPass = boost_value_to_vk(render_pass)
    var h : VkPipeline
    unsafe {
        dynstate.pDynamicStates = addr(dyn_states[0])
        gp.pDynamicState = addr(dynstate)
        blend.pAttachments = addr(blend_att)
        vinput.pVertexBindingDescriptions = addr(vbindings[0])
        vinput.pVertexAttributeDescriptions = addr(vattrs[0])
        gp.pStages = addr(stages[0])
        gp.pVertexInputState = addr(vinput)
        gp.pInputAssemblyState = addr(iasm)
        gp.pViewportState = addr(vpstate)
        gp.pRasterizationState = addr(raster)
        gp.pMultisampleState = addr(msaa)
        gp.pColorBlendState = addr(blend)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(gp), null, addr(h)), null)
    }
    delete dyn_states
    delete vbindings
    delete vattrs
    delete stages
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

// ===== app state (globals so daslang-live reloads keep them) =====

var g_speed = 1.0f
var g_angle = 0.0f
var g_cube_vb : HostBuffer
var g_cube_ib : HostBuffer
var g_cube_set_layouts : array<DescriptorSetLayout>
var g_cube_pipe_layout : PipelineLayout
var g_cube_pipeline : Pipeline

[export]
def init() {
    harness_init("dasVulkan cube + daslang ImGui backend", WIN_W, WIN_H)
    let device = harness_device()
    let phys = harness_phys()

    var vb_usage : VkBufferUsageFlags
    vb_usage.vertex_buffer = true
    var verts_copy := cube_vertices
    g_cube_vb <- create_host_buffer_from_bytes(device, phys, vb_usage, verts_copy)
    delete verts_copy
    var ib_usage : VkBufferUsageFlags
    ib_usage.index_buffer = true
    var indices_copy := cube_indices
    g_cube_ib <- create_host_buffer_from_bytes(device, phys, ib_usage, indices_copy)
    delete indices_copy

    var cube_refl <- [decode_reflection(cube_vert_spv_reflect), decode_reflection(cube_frag_spv_reflect)]
    g_cube_set_layouts <- build_descriptor_set_layouts(device, cube_refl)
    g_cube_pipe_layout <- build_pipeline_layout(device, g_cube_set_layouts, cube_refl)
    delete cube_refl
    var inscope cube_vert <- create_shader_module(device, cube_vert_spv)
    var inscope cube_frag <- create_shader_module(device, cube_frag_spv)
    g_cube_pipeline <- build_cube_pipeline(device, harness_render_pass(), g_cube_pipe_layout, cube_vert, cube_frag)
}

[export]
def update() {
    if (!harness_begin_frame()) {
        return
    }
    harness_new_frame()

    var io & = unsafe(GetIO())
    g_angle += g_speed * io.DeltaTime

    window(CUBE_WIN, (text = "Controls", closable = false,
                      flags = ImGuiWindowFlags.AlwaysAutoResize)) {
        text("Cube: dasVulkan. UI: a 100% daslang ImGui->Vulkan backend.")
        text("Drag the slider to change the spin speed.")
        edit_slider_float(safe_addr(g_speed), (id = "SPEED", text = "Rotation speed",
                                               v_min = 0.0f, v_max = 6.0f))
    }

    let sz = harness_size()
    let aspect = float(sz.x) / float(max(sz.y, 1))
    let proj = perspective_vk(45.0f * PI / 180.0f, aspect, 0.1f, 100.0f)
    let view = look_at_rh(float3(0.0f, 1.1f, 3.2f), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f))
    let model = rotate_y(g_angle)
    let mvp = proj * view * model

    harness_end_frame() $(cmd : CommandBuffer) {
        cmd_bind_pipeline(cmd, g_cube_pipeline)
        cpc.mvp = mvp
        cpc.model = model
        cube_vs_push_constants(cmd, g_cube_pipe_layout)
        let vbufs <- [weak_copy(g_cube_vb.buffer)]
        var voffs : array<uint64>
        voffs |> push(0ul)
        cmd_bind_vertex_buffers(cmd, 0u, vbufs, voffs)
        cmd_bind_index_buffer(cmd, weak_copy(g_cube_ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_draw_indexed(cmd, 36u, 1u, 0u, 0, 0u)
    }
}

[export]
def shutdown() {
    delete g_cube_pipeline
    delete g_cube_pipe_layout
    delete g_cube_set_layouts
    delete g_cube_ib
    delete g_cube_vb
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
