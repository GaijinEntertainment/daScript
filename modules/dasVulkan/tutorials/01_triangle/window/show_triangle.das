options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 01 - the runnable view. Opens a GLFW window + Vulkan swapchain and presents the rotating
// RGB triangle live (FIFO present, resizable), driving the same SpinPush angle from wall-clock time so
// you watch the push-constant spin the headless test only samples. The pixel-oracle (test_triangle.das)
// renders offscreen for CI; this is the "run it and see it" companion. It needs a display and the GLFW
// module, so it lives in this `window/` subfolder which the tutorials .das_test skips in CI (the
// lavapipe CI daslang build is built -DDAS_GLFW_DISABLED=ON). Run from the daslang root:
//   daslang -load_module <path-to-dasVulkan> <path-to>/tutorials/01_triangle/window/show_triangle.das
// Close the window to exit.

require glfw/glfw_boost
require vulkan
require vulkan/vulkan_boost
require vulkan/vulkan_window
require ../triangle_tut_shaders.das
require daslib/defer

let W = 800
let H = 600

[export]
def main {   // nolint:STYLE038 - flat viewer lifecycle scaffold
    if (volkInitialize() != 0) {
        panic("no vulkan loader")
    }
    // Hand GLFW the loader volk just found, so glfwVulkanSupported /
    // glfwGetRequiredInstanceExtensions work even where GLFW's own loader discovery
    // would miss it (e.g. the Homebrew prefix on macOS). Must precede glfwInit.
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
    var window = glfwCreateWindow(W, H, "dasVulkan tutorial 01 - rotating triangle", null, null)
    if (window == null) {
        panic("can't create window")
    }
    defer() { glfwDestroyWindow(window) }

    // instance with the surface extensions GLFW requires for this platform
    var ext_count = 0u
    let glfw_exts = glfwGetRequiredInstanceExtensions(unsafe(addr(ext_count)))
    var inst_exts : array<string>
    inst_exts |> reserve(int(ext_count))
    for (i in range(int(ext_count))) {
        unsafe {
            inst_exts |> push(glfw_exts[i])
        }
    }
    var inscope instance <- create_instance("dasVulkan tutorial 01", make_api_version(1u, 3u, 0u), inst_exts)
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

    var inscope swap <- create_swapchain(device, phys, surface, W, H)
    var inscope render_pass <- create_render_pass_single_color(device, swap.format, VkImageLayout.PRESENT_SRC_KHR)
    build_swapchain_framebuffers(device, swap, render_pass)

    // tri_spin_vert_spv / tri_spin_frag_spv: SPIR-V words emitted by dasSpirv from triangle_tut_shaders.das
    var inscope vert <- create_shader_module(device, tri_spin_vert_spv)
    var inscope frag <- create_shader_module(device, tri_spin_frag_spv)

    // pipeline layout with one vertex push-constant range (the angle: 4 bytes at offset 0)
    var plci : PipelineLayoutCreateInfo
    var pcr : PushConstantRange
    pcr.stageFlags.vertex = true
    pcr.offset = 0u
    pcr.size = 4u
    plci.pPushConstantRanges |> emplace(pcr)
    var inscope layout <- create_pipeline_layout(device, plci)
    delete plci.pPushConstantRanges   // owned input array; the layout copied the ranges, so free it now

    // dynamic viewport/scissor so the one pipeline survives every resize
    var inscope pipeline <- create_graphics_pipeline_simple(device, render_pass, layout, vert, frag, swap.width, swap.height, true)

    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    var inscope sync <- create_frame_sync(device)

    let clear = clear_color(0.1f, 0.1f, 0.15f, 1.0f)
    var frames = 0
    while (glfwWindowShouldClose(window) == 0) {
        glfwPollEvents()
        // skip rendering while minimized (zero-area framebuffer can't make a swapchain)
        var fbw = 0
        var fbh = 0
        glfwGetFramebufferSize(window, fbw, fbh)
        if (fbw == 0 || fbh == 0) {
            glfwWaitEvents()
            continue
        }
        // proactively match the window: recreate when the framebuffer size drifts
        if (fbw != swap.width || fbh != swap.height) {
            recreate_swapchain(swap, device, phys, surface, render_pass, fbw, fbh)
        }
        // one full turn every 2*pi seconds; pushed to the vertex shader each frame
        pc.angle = float(glfwGetTime())
        let ok = draw_frame(device, queue, swap, render_pass, pool, sync, clear, true) $(cmd) {
            cmd_bind_pipeline(cmd, pipeline)
            tri_spin_vert_push_constants(cmd, layout)
            cmd_draw(cmd, 3u)
        }
        if (!ok) {
            recreate_swapchain(swap, device, phys, surface, render_pass, fbw, fbh)
        }
        frames ++
    }
    vkDeviceWaitIdle(boost_value_to_vk(device))
    print("window closed after {frames} frames\n")
}
