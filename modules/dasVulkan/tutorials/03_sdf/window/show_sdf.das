options gen2
options _comment_hygiene = true
options indenting = 4

// Windowed SDF: the tutorial 03 compute shader (sdf_spv) recomputed every frame from a single `time`
// push constant, blitted straight onto the swapchain image (no render pass, no fragment shader). The
// per-frame compute uses resident_compute's build_compute_image / record_compute -- a generic resident
// single-float-pushconstant compute-to-storage-image builder; the SDF shader has the interface it expects
// (Rgba8 storage image at set=0/binding=0, one float push constant, 8x8 local size). Run from the daslang root:
//   daslang -load_module <path-to-dasVulkan> <path-to>/tutorials/03_sdf/window/show_sdf.das
// Close the window to exit.

require glfw/glfw_boost
require vulkan
require vulkan/vulkan_boost
require vulkan/vulkan_window
require ../sdf_tut_shaders.das
require resident_compute
require daslib/defer

[export]
def main {   // nolint:STYLE038 - flat viewer lifecycle scaffold
    if (volkInitialize() != 0) {
        panic("no vulkan loader")
    }
    // Hand GLFW the loader volk just found (Homebrew prefix on macOS). Must precede glfwInit.
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
    var window = glfwCreateWindow(SDF_DIM, SDF_DIM, "dasVulkan tutorial 03 - SDF raymarch (blit)", null, null)
    if (window == null) {
        panic("can't create window")
    }
    defer() { glfwDestroyWindow(window) }

    var ext_count = 0u
    let glfw_exts = glfwGetRequiredInstanceExtensions(unsafe(addr(ext_count)))
    var inst_exts : array<string>
    inst_exts |> reserve(int(ext_count))
    for (i in range(int(ext_count))) {
        inst_exts |> push(unsafe(glfw_exts[i]))
    }
    var inscope instance <- create_instance("dasVulkan tutorial 03 (window)", make_api_version(1u, 3u, 0u), inst_exts)
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

    // resident compute resources -- recomputed every frame with a new time push constant
    var inscope sc <- build_compute_image(device, phys, sdf_spv, SDF_DIM, SDF_DIM)

    var inscope swap <- create_swapchain(device, phys, surface, SDF_DIM, SDF_DIM)

    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    var inscope sync <- create_frame_sync(device)

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
            delete swap
            swap <- create_swapchain(device, phys, surface, fbw, fbh)
        }
        let t = float(glfwGetTime())
        let ok = present_frame(device, queue, swap, pool, sync) $(cmd; target; _idx) {
            transition_image2(cmd, sc.image, VkImageLayout.UNDEFINED, VkImageLayout.GENERAL,
                PIPELINE_STAGE_2_NONE, ACCESS_2_NONE,
                VkPipelineStageFlags2.compute_shader, VkAccessFlags2.shader_storage_write)
            record_compute(cmd, sc, t)
            transition_image2(cmd, sc.image, VkImageLayout.GENERAL, VkImageLayout.GENERAL,
                VkPipelineStageFlags2.compute_shader, VkAccessFlags2.shader_storage_write,
                VkPipelineStageFlags2.blit, VkAccessFlags2.transfer_read)
            blit_to_swapchain(cmd, sc.image, VkImageLayout.GENERAL, sc.width, sc.height, swap, target)
        }
        if (!ok) {
            vkDeviceWaitIdle(boost_value_to_vk(device))
            delete swap
            swap <- create_swapchain(device, phys, surface, fbw, fbh)
        }
    }
    vkDeviceWaitIdle(boost_value_to_vk(device))
}
