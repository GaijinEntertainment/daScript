options gen2
options _comment_hygiene = true
options indenting = 4

// Windowed skybox: the tutorial 06 graphics pipeline (skybox_vert_spv + skybox_frag_spv) rendered
// into the same offscreen colour attachment the [test] uses, then blitted onto the swapchain image.
// show_skybox owns its own instance (with surface extensions) + device (with VK_KHR_swapchain),
// then calls build_skybox_resources(device, phys, queue, pool) to share the whole offscreen path
// with the headless oracle. Run from the daslang root:
//   daslang -load_module <path-to-dasVulkan> <path-to>/tutorials/06_skybox/window/show_skybox.das
// Close the window to exit.

require glfw/glfw_boost
require vulkan
require vulkan/vulkan_boost
require vulkan/vulkan_window
require ../skybox_tut.das
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
    var window = glfwCreateWindow(SKY_W, SKY_H, "dasVulkan tutorial 06 - skybox (blit)", null, null)
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
    var inscope instance <- create_instance("dasVulkan tutorial 06 (window)", make_api_version(1u, 3u, 0u), inst_exts)
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

    var inscope res <- build_skybox_resources(device, phys, queue, pool)
    var inscope swap <- create_swapchain(device, phys, surface, SKY_W, SKY_H)
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
        // 30-second camera orbit -- one full circle per 30s of wall clock.
        let raw_cam = t * 0.0333f
        let camera_t = raw_cam - floor(raw_cam)
        update_skybox_uniforms(res, device, t, camera_t)

        let ok = present_frame(device, queue, swap, pool, sync) $(cmd; target; _idx) {
            record_skybox_render_pass(res, cmd)

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
            var region : VkImageBlit
            region.srcSubresource.aspectMask.color = true
            region.srcSubresource.layerCount = 1u
            region.srcOffsets[1].x = SKY_W
            region.srcOffsets[1].y = SKY_H
            region.srcOffsets[1].z = 1
            region.dstSubresource.aspectMask.color = true
            region.dstSubresource.layerCount = 1u
            region.dstOffsets[1].x = swap.width
            region.dstOffsets[1].y = swap.height
            region.dstOffsets[1].z = 1
            unsafe {
                vkCmdBlitImage(boost_value_to_vk(cmd), boost_value_to_vk(res.color.image), VkImageLayout.TRANSFER_SRC_OPTIMAL,
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
