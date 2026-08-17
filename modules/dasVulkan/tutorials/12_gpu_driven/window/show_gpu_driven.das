options gen2
options _comment_hygiene = true
options indenting = 4

// Windowed tutorial 12: the GPU-driven frame blitted onto a GLFW swapchain image. Reuses
// build_gpu_driven_resources + record_frame from gpu_driven_tut.das, so the only window-specific
// parts are the surface + swapchain + per-frame blit. The occluder wall sweeps across the field with
// wall-clock time, so the culled set (the red ghosts in the right/god panel) sweeps with it while the
// left/camera panel stays correct -- occlusion culling is a perf win that is visually transparent.
//   daslang -load_module <path-to-dasVulkan> tutorials/12_gpu_driven/window/show_gpu_driven.das

require glfw/glfw_boost
require vulkan
require vulkan/vulkan_boost
require vulkan/vulkan_window
require ../gpu_driven_tut.das
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
    var window = glfwCreateWindow(OUT_W, OUT_H, "dasVulkan tutorial 12 - GPU-driven Hi-Z occlusion culling", null, null)
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
    var inscope instance <- create_instance("dasVulkan tutorial 12 (window)", make_api_version(1u, 2u, 0u), inst_exts)
    volkLoadInstance(boost_value_to_vk(instance))

    var inscope surface <- create_surface(instance, glfwGetNativeWindow(window), glfwGetNativeDisplay())

    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    if (!queue_family_supports_present(phys, gfx, surface)) {
        panic("graphics queue family does not support presentation")
    }
    var inscope device <- create_device_gpu_driven(phys, gfx, ["VK_KHR_swapchain"])
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, gfx, 0u)

    var inscope pool <- create_command_pool(device, CommandPoolCreateInfo(queueFamilyIndex = gfx))
    var inscope res <- build_gpu_driven_resources(device, phys, queue, pool)
    update_uniforms(res, device)                          // the scene UBO is static; only the wall (push) moves

    var inscope swap <- create_swapchain(device, phys, surface, OUT_W, OUT_H)
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
        let wall_x = 5.0f * sin(t * 1.05f)                // sweep the wall across the field (~6 s round trip)

        let ok = present_frame(device, queue, swap, pool, sync) $(cmd; target; _idx) {
            record_frame(res, cmd, wall_x)

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
            region.srcOffsets[1].x = OUT_W
            region.srcOffsets[1].y = OUT_H
            region.srcOffsets[1].z = 1
            region.dstSubresource.aspectMask.color = true
            region.dstSubresource.layerCount = 1u
            region.dstOffsets[1].x = swap.width
            region.dstOffsets[1].y = swap.height
            region.dstOffsets[1].z = 1
            unsafe {
                vkCmdBlitImage(boost_value_to_vk(cmd), boost_value_to_vk(res.out_color.image), VkImageLayout.TRANSFER_SRC_OPTIMAL,
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
