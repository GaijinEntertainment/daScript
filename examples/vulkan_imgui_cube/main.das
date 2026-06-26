options gen2
options indenting = 4

// Spinning cube rendered with dasVulkan, with a Dear ImGui control panel whose slider drives the
// rotation speed -- and the ImGui UI is rasterized by a renderer backend written entirely in
// daslang (vulkan/vulkan_imgui_driver). Cube and UI share one swapchain render pass: the cube draws first
// (back-face culled, so no depth buffer is needed for a convex mesh), then the UI blends on top.
//
// Run (from the repo root):
//   daslang -project_root examples/vulkan_imgui_cube examples/vulkan_imgui_cube/main.das
// Close the window to exit.

require glfw/glfw_boost
require vulkan
require vulkan/vulkan_boost
require vulkan/vulkan_window
require vulkan/vulkan_live
require vulkan/vulkan_reflect
require cube_shaders
require vulkan/vulkan_imgui_driver
require imgui
require imgui/imgui_theme_daslang
require daslib/defer
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

[export]
def main {
    if (volkInitialize() != 0) {
        panic("no vulkan loader")
    }
    glfwInitVulkanLoader(vk_get_instance_proc_addr())
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    defer() { glfwTerminate() }
    if (glfwVulkanSupported() == 0) {
        panic("glfw reports no vulkan support")
    }
    glfwWindowHint(int(GLFW_CLIENT_API), GLFW_NO_API)
    glfwWindowHint(int(GLFW_RESIZABLE), GLFW_TRUE)
    var window = glfwCreateWindow(WIN_W, WIN_H, "dasVulkan cube + daslang ImGui backend", null, null)
    if (window == null) {
        panic("can't create window")
    }
    defer() { glfwDestroyWindow(window) }

    // ----- instance / surface / device / queue / pool -----
    var ext_count = 0u
    let glfw_exts = glfwGetRequiredInstanceExtensions(unsafe(addr(ext_count)))
    var inst_exts : array<string>
    for (i in range(int(ext_count))) {
        unsafe {
            inst_exts |> push(glfw_exts[i])
        }
    }
    var inscope instance <- create_instance("vulkan_imgui_cube", make_api_version(1u, 3u, 0u), inst_exts)
    volkLoadInstance(boost_value_to_vk(instance))
    var inscope surface <- create_surface(instance, glfwGetNativeWindow(window), glfwGetNativeDisplay())
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    if (!queue_family_supports_present(phys, gfx, surface)) {
        panic("graphics queue family does not support presentation")
    }
    var inscope device <- create_device(phys, gfx, ["VK_KHR_swapchain"])
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    // ----- swapchain + color-only render pass (present) + framebuffers -----
    // UNORM (pass-through) swapchain: the cube + ImGui both output already-sRGB colors, so we must NOT
    // let the GPU sRGB-encode them again. Matches how the GL tools render (GL_FRAMEBUFFER_SRGB off).
    var inscope swap <- create_swapchain(device, phys, surface, WIN_W, WIN_H, VkFormat.B8G8R8A8_UNORM)
    var inscope render_pass <- create_render_pass_single_color(device, swap.format, VkImageLayout.PRESENT_SRC_KHR)
    build_swapchain_framebuffers(device, swap, render_pass)
    var inscope sync <- create_frame_sync(device)

    // hand vulkan_live the objects its screenshot / recording commands read (it owns none of them)
    vk_live_set_target(device, phys, queue, pool, swap)

    // ----- cube resources -----
    var vb_usage : VkBufferUsageFlags
    vb_usage.vertex_buffer = true
    var verts_copy := cube_vertices
    var inscope cube_vb <- create_host_buffer_from_bytes(device, phys, vb_usage, verts_copy)
    delete verts_copy
    var ib_usage : VkBufferUsageFlags
    ib_usage.index_buffer = true
    var indices_copy := cube_indices
    var inscope cube_ib <- create_host_buffer_from_bytes(device, phys, ib_usage, indices_copy)
    delete indices_copy

    var cube_refl <- [decode_reflection(cube_vert_spv_reflect), decode_reflection(cube_frag_spv_reflect)]
    var inscope cube_set_layouts <- build_descriptor_set_layouts(device, cube_refl)
    var inscope cube_pipe_layout <- build_pipeline_layout(device, cube_set_layouts, cube_refl)
    delete cube_refl
    var inscope cube_vert <- create_shader_module(device, cube_vert_spv)
    var inscope cube_frag <- create_shader_module(device, cube_frag_spv)
    var inscope cube_pipeline <- build_cube_pipeline(device, render_pass, cube_pipe_layout, cube_vert, cube_frag)

    // ----- ImGui context + the 100%-daslang Vulkan renderer backend -----
    let ctx = CreateContext(null)
    load_daslang_font()       // daslang.io look (JetBrains Mono + amber theme), like the other tools
    apply_daslang_theme()
    var io & = unsafe(GetIO())
    io.BackendFlags |= ImGuiBackendFlags.RendererHasTextures      // we drain the texture queue ourselves
    io.BackendFlags |= ImGuiBackendFlags.RendererHasVtxOffset
    var inscope igvk <- imgui_vk_create(device, render_pass)

    var speed = 1.0f
    var angle = 0.0f
    var last_t = float(glfwGetTime())
    var shot_key_was_down = false   // edge-detect the screenshot key (P)

    while (glfwWindowShouldClose(window) == 0) {
        glfwPollEvents()
        var fbw = 0
        var fbh = 0
        glfwGetFramebufferSize(window, fbw, fbh)
        if (fbw == 0 || fbh == 0) {
            glfwWaitEvents()
            continue
        }
        if (fbw != swap.width || fbh != swap.height) {
            vkDeviceWaitIdle(boost_value_to_vk(device))
            recreate_swapchain(swap, device, phys, surface, render_pass, fbw, fbh)
        }

        let t = float(glfwGetTime())
        let dt = max(t - last_t, 0.0001f)
        last_t = t
        angle += speed * dt

        // ----- feed ImGui input (mouse + display) and build the UI -----
        io.DisplaySize = float2(float(fbw), float(fbh))
        io.DeltaTime = dt
        // cursor pos is in window points but DisplaySize is framebuffer pixels -- scale to match on HiDPI
        var winw = 0
        var winh = 0
        glfwGetWindowSize(window, unsafe(addr(winw)), unsafe(addr(winh)))
        let csx = winw > 0 ? float(fbw) / float(winw) : 1.0f
        let csy = winh > 0 ? float(fbh) / float(winh) : 1.0f
        let mpos = glfwGetCursorPos(window)
        io |> AddMousePosEvent(mpos.x * csx, mpos.y * csy)
        io |> AddMouseButtonEvent(0, glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)   // ImGui button 0 = left

        var win_flags : ImGuiWindowFlags
        var slider_flags : ImGuiSliderFlags
        NewFrame()
        if (Begin("Controls", null, win_flags)) {
            Text("Cube: dasVulkan. UI: a 100% daslang ImGui->Vulkan backend.")
            Text("Drag the slider to change the spin speed.")
            SliderFloat("Rotation speed", unsafe(addr(speed)), 0.0f, 6.0f, "%.2f", slider_flags)
        }
        End()
        Render()

        // upload any textures ImGui requested (the font atlas on the first frame)
        imgui_vk_update_textures(igvk, device, phys, queue, pool)

        // ----- per-frame matrices -----
        let aspect = float(swap.width) / float(swap.height)
        let proj = perspective_vk(45.0f * PI / 180.0f, aspect, 0.1f, 100.0f)
        let view = look_at_rh(float3(0.0f, 1.1f, 3.2f), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f))
        let model = rotate_y(angle)
        let mvp = proj * view * model

        let ok = vk_live_draw_frame(render_pass, sync,
                            clear_color(0.10f, 0.11f, 0.14f, 1.0f), true) $(cmd) {
            // cube
            cmd_bind_pipeline(cmd, cube_pipeline)
            cpc.mvp = mvp
            cpc.model = model
            cube_vs_push_constants(cmd, cube_pipe_layout)
            let vbufs <- [weak_copy(cube_vb.buffer)]
            var voffs : array<uint64>
            voffs |> push(0ul)
            cmd_bind_vertex_buffers(cmd, 0u, vbufs, voffs)
            cmd_bind_index_buffer(cmd, weak_copy(cube_ib.buffer), 0ul, VkIndexType.UINT16)
            cmd_draw_indexed(cmd, 36u, 1u, 0u, 0, 0u)
            // UI on top, via the daslang backend
            imgui_vk_render(igvk, device, phys, cmd)
        }
        if (!ok) {
            vkDeviceWaitIdle(boost_value_to_vk(device))
            recreate_swapchain(swap, device, phys, surface, render_pass, fbw, fbh)
        }
        // press P to capture the current frame via vulkan_live (swapchain readback -> PNG)
        let shot_key_down = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS
        if (shot_key_down && !shot_key_was_down) {
            if (vk_live_save_screenshot("vulkan_imgui_cube.png")) {
                print("saved vulkan_imgui_cube.png\n")
            }
        }
        shot_key_was_down = shot_key_down
    }
    vkDeviceWaitIdle(boost_value_to_vk(device))
    DestroyContext(ctx)
}
