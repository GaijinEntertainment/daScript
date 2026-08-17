options gen2
options _comment_hygiene = true
options indenting = 4

// Windowed MSAA + dynamic-rendering tutorial: the offscreen scene blitted onto a GLFW swapchain
// image. Reuses build_msaa_resources from msaa_tut.das; the window-specific piece is the surface,
// swapchain, and per-frame blit from the (1x) resolve image into the swapchain target. The cube
// orbits with wall-clock time.
//   daslang -load_module <path-to-dasVulkan> tutorials/09_msaa/window/show_msaa.das

require glfw/glfw_boost
require vulkan
require vulkan/vulkan_boost
require vulkan/vulkan_window
require ../msaa_tut.das
require daslib/defer
require math

[export]
def main {   // nolint:STYLE038 - flat viewer lifecycle scaffold
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
    var window = glfwCreateWindow(MSAA_W, MSAA_H, "dasVulkan tutorial 09 - MSAA + dynamic rendering", null, null)
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
    // Vulkan 1.3 API: dynamic_rendering is core there
    var inscope instance <- create_instance("dasVulkan tutorial 09 (window)", make_api_version(1u, 3u, 0u), inst_exts)
    volkLoadInstance(boost_value_to_vk(instance))

    var inscope surface <- create_surface(instance, glfwGetNativeWindow(window), glfwGetNativeDisplay())

    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    if (!queue_family_supports_present(phys, gfx, surface)) {
        panic("graphics queue family does not support presentation")
    }
    // Swapchain extension + dynamicRendering Vulkan 1.3 feature (pNext chain)
    var features13 = VkPhysicalDeviceVulkan13Features()
    features13.dynamicRendering = 1u
    var inscope device <- create_device(phys, gfx, ["VK_KHR_swapchain"], features13)
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, gfx, 0u)

    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    var inscope res <- build_msaa_resources(device, phys, queue, pool)
    var inscope swap <- create_swapchain(device, phys, surface, MSAA_W, MSAA_H)
    var inscope sync <- create_frame_sync(device)

    // Three-state MSAA cycle: AUTO (flips every AUTO_PERIOD sec) -> FORCE_OFF -> FORCE_ON ->
    // AUTO. Default AUTO so the recording driver gets the comparison for free.
    var toggle = MsaaToggle.auto_toggle
    var m_was_down = false
    var title_label = ""

    while (glfwWindowShouldClose(window) == 0) {
        glfwPollEvents()

        // M-key edge-detect cycles the toggle. GLFW returns the current state every poll, so we
        // remember the prior state and only fire on the down-edge.
        let m_down = glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS
        if (m_down && !m_was_down) {
            toggle = next_msaa_toggle(toggle)
        }
        m_was_down = m_down

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
        // 24-second orbit (camera_t in [0, 1))
        let raw_cam = t * 0.042f
        let camera_t = raw_cam - floor(raw_cam)
        update_msaa_uniforms(res, device, t, camera_t)

        let use_msaa = msaa_active(toggle, t)
        // Update window title when the active mode label changes -- the on-image strip is the
        // primary indicator, but the title shows the toggle state ("AUTO/OFF/ON") explicitly.
        let new_label = "M = {msaa_toggle_label(toggle)} | drawing: {use_msaa ? "MSAA 4x" : "1x (no AA)"} - press M to cycle"
        if (new_label != title_label) {
            glfwSetWindowTitle(window, "dasVulkan tutorial 09 - {new_label}")
            title_label = new_label
        }

        let ok = present_frame(device, queue, swap, pool, sync) $(cmd; target; _idx) {
            record_msaa_render(res, cmd, use_msaa)

            let none : VkAccessFlags
            var transfer_write : VkAccessFlags
            transfer_write.transfer_write = true
            var top : VkPipelineStageFlags
            top.top_of_pipe = true
            var xfer : VkPipelineStageFlags
            xfer.transfer = true
            var bottom : VkPipelineStageFlags
            bottom.bottom_of_pipe = true
            let dst = vk_value_to_boost(target)
            transition_image(cmd, dst, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
                none, transfer_write, top, xfer)
            let src_img = final_image(res, use_msaa)
            var region : VkImageBlit
            region.srcSubresource.aspectMask.color = true
            region.srcSubresource.layerCount = 1u
            region.srcOffsets[1].x = MSAA_W
            region.srcOffsets[1].y = MSAA_H
            region.srcOffsets[1].z = 1
            region.dstSubresource.aspectMask.color = true
            region.dstSubresource.layerCount = 1u
            region.dstOffsets[1].x = swap.width
            region.dstOffsets[1].y = swap.height
            region.dstOffsets[1].z = 1
            unsafe {
                vkCmdBlitImage(boost_value_to_vk(cmd), boost_value_to_vk(src_img), VkImageLayout.TRANSFER_SRC_OPTIMAL,
                    target, VkImageLayout.TRANSFER_DST_OPTIMAL, 1u, addr(region), VkFilter.LINEAR)
            }
            transition_image(cmd, dst, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.PRESENT_SRC_KHR,
                transfer_write, none, xfer, bottom)
        }
        if (!ok) {
            vkDeviceWaitIdle(boost_value_to_vk(device))
            delete swap
            swap <- create_swapchain(device, phys, surface, fbw, fbh)
        }
    }
    vkDeviceWaitIdle(boost_value_to_vk(device))
}
