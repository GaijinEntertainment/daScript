options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 13 - the runnable view. Opens a GLFW window + Vulkan swapchain and presents the live
// GPU-driven mesh-shader render: one cmd_draw_mesh_tasks_e_x_t dispatches a task workgroup that culls
// meshlets 3 and 4 and dispatches a mesh workgroup per survivor, each amplifying one colored quad.
// The pixel-oracle (test_mesh.das) renders offscreen for CI; this is the "run it and see it" companion.
// It needs a display + the GLFW module + a VK_EXT_mesh_shader-capable driver, so it lives in this
// `window/` subfolder which the tutorials .das_test skips in CI (lavapipe is built -DDAS_GLFW_DISABLED).
// Run from the daslang root with a mesh-capable daslang:
//   daslang -load_module <path-to-dasVulkan> <path-to>/tutorials/13_mesh/window/show_mesh.das
// Close the window to exit.

require glfw/glfw_boost
require vulkan
require vulkan/vulkan_boost
require vulkan/vulkan_window
require ../mesh_tut_shaders.das
require daslib/defer

let W = 800
let H = 600

[export]
def main {   // nolint:STYLE038 - flat viewer lifecycle scaffold
    if (volkInitialize() != 0) {
        panic("no vulkan loader")
    }
    glfwInitVulkanLoader(vk_get_instance_proc_addr())   // must precede glfwInit (macOS loader discovery)
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    defer() { glfwTerminate() }
    if (glfwVulkanSupported() == 0) {
        panic("glfw reports no vulkan support")
    }
    glfwWindowHint(int(GLFW_CLIENT_API), GLFW_NO_API)
    glfwWindowHint(int(GLFW_RESIZABLE), GLFW_TRUE)
    var window = glfwCreateWindow(W, H, "dasVulkan tutorial 13 - mesh shaders", null, null)
    if (window == null) {
        panic("can't create window")
    }
    defer() { glfwDestroyWindow(window) }

    var ext_count = 0u
    let glfw_exts = glfwGetRequiredInstanceExtensions(unsafe(addr(ext_count)))
    var inst_exts : array<string>
    inst_exts |> reserve(int(ext_count))
    for (i in range(int(ext_count))) {
        unsafe {
            inst_exts |> push(glfw_exts[i])
        }
    }
    var inscope instance <- create_instance("dasVulkan tutorial 13", make_api_version(1u, 3u, 0u), inst_exts)
    volkLoadInstance(boost_value_to_vk(instance))

    var inscope surface <- create_surface(instance, glfwGetNativeWindow(window), glfwGetNativeDisplay())

    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    if (!queue_family_supports_present(phys, gfx, surface)) {
        panic("graphics queue family does not support presentation")
    }
    if (!mesh_shader_supported(phys)) {
        panic("this device does not support VK_EXT_mesh_shader (meshShader+taskShader); cannot run tutorial 13")
    }
    // device with the swapchain extension AND the mesh-shader extension + features (general overload)
    var mf : VkPhysicalDeviceMeshShaderFeaturesEXT
    mf.meshShader = 1u
    mf.taskShader = 1u
    var inscope device <- create_device(phys, gfx, ["VK_KHR_swapchain", "VK_EXT_mesh_shader"], mf)
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, gfx, 0u)

    var inscope swap <- create_swapchain(device, phys, surface, W, H)
    var inscope render_pass <- create_render_pass_single_color(device, swap.format, VkImageLayout.PRESENT_SRC_KHR)
    build_swapchain_framebuffers(device, swap, render_pass)

    // task / mesh / fragment SPIR-V emitted by dasSpirv from mesh_tut_shaders.das
    var inscope task <- create_shader_module(device, mesh_tut_task_spv)
    var inscope mesh <- create_shader_module(device, mesh_tut_mesh_spv)
    var inscope frag <- create_shader_module(device, mesh_tut_frag_spv)
    var inscope layout <- create_pipeline_layout(device)
    // dynamic viewport/scissor so the one pipeline survives every resize
    var inscope pipeline <- create_mesh_pipeline(device, render_pass, layout, task, mesh, frag, swap.width, swap.height, true)

    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    var inscope sync <- create_frame_sync(device)

    let clear = clear_color(0.05f, 0.05f, 0.08f, 1.0f)
    var frames = 0
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
            recreate_swapchain(swap, device, phys, surface, render_pass, fbw, fbh)
        }
        let ok = draw_frame(device, queue, swap, render_pass, pool, sync, clear, true) $(cmd) {
            cmd_bind_pipeline(cmd, pipeline)
            cmd_draw_mesh_tasks_e_x_t(cmd, 1u, 1u, 1u)   // one task workgroup -> cull -> mesh workgroups
        }
        if (!ok) {
            recreate_swapchain(swap, device, phys, surface, render_pass, fbw, fbh)
        }
        frames ++
    }
    vkDeviceWaitIdle(boost_value_to_vk(device))
    print("window closed after {frames} frames\n")
}
