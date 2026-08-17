options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 13 render path: the GPU-driven mesh-shader pipeline (VK_EXT_mesh_shader). No vertex
// buffer, no index buffer, no vertex shader -- a single cmd_draw_mesh_tasks_e_x_t dispatches ONE task
// workgroup, which culls the meshlet grid and dispatches a mesh workgroup per survivor; each mesh
// workgroup amplifies one colored quad. The CI gate (test_mesh.das) asserts a survivor cell is lit
// and a culled cell is background, proving the task stage actually removed geometry on the GPU.

module mesh_tut

require vulkan public
require vulkan/vulkan_boost public
require mesh_tut_shaders

let public MESH_W = 256
let public MESH_H = 256

//! Probe for VK_EXT_mesh_shader -- both the extension string and the meshShader/taskShader feature bits
//! (mesh_shader_supported) -- so the pixel-oracle gate skips cleanly on a driver that lacks it (e.g. Mesa
//! lavapipe < 24.1) instead of crashing vkCreateDevice. Creates and tears down a throwaway instance.
def public mesh_shader_available() : bool {
    if (volkInitialize() != 0) {
        return false
    }
    var inscope instance <- create_instance("dasVulkan tutorial 13 probe", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    var count = 0u
    vkEnumeratePhysicalDevices(boost_value_to_vk(instance), count, null)
    if (count == 0u) {
        return false   // loader present but no device: soft-skip instead of panicking in select_physical_device
    }
    let phys = select_physical_device(instance)
    return mesh_shader_supported(phys)
}

//! Render the meshlet grid offscreen; returns MESH_W*MESH_H RGBA8 pixels. The device is created with
//! VK_EXT_mesh_shader enabled (create_device_mesh_shader); the pipeline is a task->mesh->fragment chain.
def public render_mesh() : array<uint8> {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan tutorial 13", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device_mesh_shader(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, gfx, 0u)
    let fmt = VkFormat.R8G8B8A8_SRGB   // sRGB: gamma-encode the linear output on write (recording readback + window blit match)

    var inscope target <- build_offscreen_target(device, phys, MESH_W, MESH_H, fmt)
    var inscope render_pass <- create_render_pass_single_color(device, fmt)
    var inscope framebuffer <- create_framebuffer(device, FramebufferCreateInfo(
        renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(target.view)],
        width = uint(MESH_W),
        height = uint(MESH_H),
        layers = 1u))

    var inscope task <- create_shader_module(device, mesh_tut_task_spv)
    var inscope mesh <- create_shader_module(device, mesh_tut_mesh_spv)
    var inscope frag <- create_shader_module(device, mesh_tut_frag_spv)
    var inscope layout <- create_pipeline_layout(device)
    var inscope pipeline <- create_mesh_pipeline(device, render_pass, layout, task, mesh, frag, MESH_W, MESH_H)

    let buf_size = uint64(MESH_W * MESH_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    run_cmd_sync(device, pool, queue) $(cmd) {
        record_render_pass(cmd, render_pass, framebuffer, full_area(MESH_W, MESH_H), clear_color(0.05f, 0.05f, 0.08f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipeline)
            cmd_draw_mesh_tasks_e_x_t(cmd, 1u, 1u, 1u)   // one task workgroup -> cull -> dispatch mesh workgroups
        }
        copy_image_to_buffer(cmd, target.image, readback, MESH_W, MESH_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! Sample the RGB of pixel (x,y) from a MESH_W*MESH_H RGBA8 buffer.
def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * MESH_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}
