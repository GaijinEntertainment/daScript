options gen2
options _comment_hygiene = true

// The canonical windowed "hello triangle": a GLFW window + Vulkan swapchain,
// rendering the RGB triangle every frame (FIFO present). Run from the repo root:
//   daslang -load_module <path-to-dasVulkan> examples/window_triangle.das
// Reuses the offscreen triangle's shaders; close the window to exit.

require glfw/glfw_boost
require vulkan
require vulkan/vulkan_boost
require vulkan/vulkan_window
require triangle_example_shaders
require daslib/defer

let W = 800
let H = 600

[export]
def main {   // nolint:STYLE038 - flat example scaffold
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
    var window = glfwCreateWindow(W, H, "dasVulkan - window triangle (resizable)", null, null)
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
    var inscope instance <- create_instance("window triangle", make_api_version(1u, 3u, 0u), inst_exts)
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

    // tri_vert_spv / tri_frag_spv: SPIR-V words emitted by dasSpirv from triangle_example_shaders.das
    var inscope vert <- create_shader_module(device, tri_vert_spv)
    var inscope frag <- create_shader_module(device, tri_frag_spv)
    var inscope layout <- create_pipeline_layout(device)
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
        // draw_frame returns false when the swapchain went out of date mid-frame
        let ok = draw_frame(device, queue, swap, render_pass, pool, sync, clear, true) $(cmd) {
            cmd_bind_pipeline(cmd, pipeline)
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
