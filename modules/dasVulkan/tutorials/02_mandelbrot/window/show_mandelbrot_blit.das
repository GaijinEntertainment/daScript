options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 02, present method ONE of two: BLIT. An animated Mandelbrot zoom -- the compute shader
// recomputes the fractal every frame from a single `time` push constant (revealing real new detail as it
// zooms, not just scaling a texture), and each frame vkCmdBlitImage copies the result straight onto the
// acquired swapchain image (scaled to the window, linear filter). No graphics pipeline, no render pass,
// no fragment shader -- just compute + a transfer. Both the per-frame compute dispatch and the blit are
// recorded into vulkan_window's present_frame block (acquire -> record -> submit -> present, the
// non-render-pass sibling of draw_frame). Compare show_mandelbrot_sampled.das, which samples the result
// as a texture instead.
//
// Needs a display + the glfw module, so it lives in this window/ subfolder the tutorials .das_test skips
// in CI (the lavapipe CI daslang build is -DDAS_GLFW_DISABLED=ON). Run from the daslang root:
//   daslang -load_module <path-to-dasVulkan> <path-to>/tutorials/02_mandelbrot/window/show_mandelbrot_blit.das
// Close the window to exit.

require glfw/glfw_boost
require vulkan
require vulkan/vulkan_boost
require vulkan/vulkan_window
require mandelbrot_zoom_shaders
require mandelbrot_compute
require daslib/defer

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
    var window = glfwCreateWindow(ZOOM_DIM, ZOOM_DIM, "dasVulkan tutorial 02 - Mandelbrot zoom (blit)", null, null)
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
    var inscope instance <- create_instance("dasVulkan tutorial 02 (blit)", make_api_version(1u, 3u, 0u), inst_exts)
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
    var inscope mc <- build_mandel_compute(device, phys, mandelbrot_zoom_spv, ZOOM_DIM, ZOOM_DIM)

    // the blit path needs no render pass or framebuffers, only the swapchain images themselves
    var inscope swap <- create_swapchain(device, phys, surface, ZOOM_DIM, ZOOM_DIM)

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
        // the blit path has no framebuffers, so just remake the swapchain in place on resize
        if (fbw != swap.width || fbh != swap.height) {
            vkDeviceWaitIdle(boost_value_to_vk(device))
            delete swap
            swap <- create_swapchain(device, phys, surface, fbw, fbh)
        }
        let t = float(glfwGetTime())
        let ok = present_frame(device, queue, swap, pool, sync) $(cmd; target; _idx) {
            let none : VkAccessFlags
            var shader_write : VkAccessFlags
            shader_write.shader_write = true
            var transfer_read : VkAccessFlags
            transfer_read.transfer_read = true
            var transfer_write : VkAccessFlags
            transfer_write.transfer_write = true
            var top : VkPipelineStageFlags
            top.top_of_pipe = true
            var comp : VkPipelineStageFlags
            comp.compute_shader = true
            var xfer : VkPipelineStageFlags
            xfer.transfer = true
            var bottom : VkPipelineStageFlags
            bottom.bottom_of_pipe = true
            // recompute the fractal into the storage image (GENERAL) for this frame's time
            transition_image(cmd, mc.image, VkImageLayout.UNDEFINED, VkImageLayout.GENERAL,
                none, shader_write, top, comp)
            record_compute(cmd, mc, t)
            // make the compute writes available to the blit (image stays GENERAL, a legal blit source)
            transition_image(cmd, mc.image, VkImageLayout.GENERAL, VkImageLayout.GENERAL,
                shader_write, transfer_read, comp, xfer)
            // swapchain image: UNDEFINED -> TRANSFER_DST (we overwrite the whole image)
            let dst = vk_value_to_boost(target)
            transition_image(cmd, dst, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
                none, transfer_write, top, xfer)
            // blit the fractal onto the swapchain image, scaling ZOOM_DIM -> window, linear filter
            var region : VkImageBlit
            region.srcSubresource.aspectMask.color = true
            region.srcSubresource.layerCount = 1u
            region.srcOffsets[1].x = mc.width
            region.srcOffsets[1].y = mc.height
            region.srcOffsets[1].z = 1
            region.dstSubresource.aspectMask.color = true
            region.dstSubresource.layerCount = 1u
            region.dstOffsets[1].x = swap.width
            region.dstOffsets[1].y = swap.height
            region.dstOffsets[1].z = 1
            unsafe {
                vkCmdBlitImage(boost_value_to_vk(cmd), boost_value_to_vk(mc.image), VkImageLayout.GENERAL,
                    target, VkImageLayout.TRANSFER_DST_OPTIMAL, 1u, addr(region), VkFilter.LINEAR)
            }
            // TRANSFER_DST -> PRESENT_SRC for the present
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
