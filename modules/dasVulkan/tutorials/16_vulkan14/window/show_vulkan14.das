options gen2
options _comment_hygiene = true
options indenting = 4

// Windowed tutorial 16: the three cubes lit through the local-read G-buffer, blitted onto a GLFW
// swapchain image every frame. Reuses build_vulkan14_resources + record_vulkan14_frame from
// vulkan14_tut.das, so the window-specific parts are the 1.4 instance with the surface extensions, the
// 1.4 device with VK_KHR_swapchain, and the per-frame blit. Needs a display, the glfw module and a
// Vulkan 1.4 driver; it lives in this window/ subfolder the tutorials .das_test skips in CI. Run from
// the daslang root:
//   daslang -load_module <path-to-dasVulkan> <path-to>/tutorials/16_vulkan14/window/show_vulkan14.das
// Close the window to exit.

require glfw/glfw_boost
require vulkan
require vulkan/vulkan_boost
require vulkan/vulkan_window
require ../vulkan14_tut.das
require daslib/defer

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
    var window = glfwCreateWindow(V14_W, V14_H, "dasVulkan tutorial 16 - Vulkan 1.4: push descriptors, host image copy, local read", null, null)
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
    var inscope instance <- create_instance("dasVulkan tutorial 16 (window)", make_api_version(1u, 4u, 0u), inst_exts)
    volkLoadInstance(boost_value_to_vk(instance))

    var inscope surface <- create_surface(instance, glfwGetNativeWindow(window), glfwGetNativeDisplay())

    let phys = select_physical_device(instance)
    if (!vulkan14_supported(phys)) {
        panic("this device or driver is below Vulkan 1.4, or lacks pushDescriptor / hostImageCopy / dynamicRenderingLocalRead")
    }
    let gfx = select_graphics_queue_family(phys)
    if (!queue_family_supports_present(phys, gfx, surface)) {
        panic("graphics queue family does not support presentation")
    }
    var inscope device <- create_device_vulkan14(phys, gfx, ["VK_KHR_swapchain"])
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, gfx, 0u)

    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    var inscope res <- build_vulkan14_resources(device, phys)
    var inscope swap <- create_swapchain(device, phys, surface, V14_W, V14_H)
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
            recreate_swapchain(swap, device, phys, surface, fbw, fbh)
        }
        let t = float(glfwGetTime())
        update_vulkan14_uniforms(res, device, t)
        let ok = present_frame(device, queue, swap, pool, sync) $(cmd; target; _idx) {
            record_vulkan14_frame(res, cmd, t)
            blit_to_swapchain(cmd, res.color.image, VkImageLayout.TRANSFER_SRC_OPTIMAL, V14_W, V14_H, swap, target)
        }
        if (!ok) {
            recreate_swapchain(swap, device, phys, surface, fbw, fbh)
        }
    }
    vkDeviceWaitIdle(boost_value_to_vk(device))
}
