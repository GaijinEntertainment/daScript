options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 15, live: the camera orbits the box while every frame is hardware ray-traced --
// vkCmdTraceRaysKHR fires one ray per pixel into the TLAS, the closest-hit casts its shadow ray,
// and the raygen writes the storage image that vkCmdBlitImage then copies onto the acquired
// swapchain image (the same compute-free present path as tutorial 02's blit variant). The whole
// RT stack (BLAS/TLAS, pipeline, SBT) is built once by build_rt_view; the per-frame cost is one
// push constant (the orbit angle) and the trace itself.
//
// Needs a display + the glfw module + an RT-capable GPU, so it lives in this window/ subfolder
// the tutorials .das_test skips in CI. Run from the daslang root:
//   daslang -load_module <path-to-dasVulkan> <path-to>/tutorials/15_raytracing/window/show_raytracing.das
// Close the window to exit.

require glfw/glfw_boost
require vulkan
require vulkan/vulkan_boost
require vulkan/vulkan_window
require ../raytracing_tut.das
require daslib/defer

let RT_DIM = 768

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
    var window = glfwCreateWindow(RT_DIM, RT_DIM, "dasVulkan tutorial 15 - hardware ray tracing", null, null)
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
    var inscope instance <- create_instance("dasVulkan tutorial 15", make_api_version(1u, 3u, 0u), inst_exts)
    volkLoadInstance(boost_value_to_vk(instance))

    var inscope surface <- create_surface(instance, glfwGetNativeWindow(window), glfwGetNativeDisplay())

    let phys = select_physical_device(instance)
    if (!rt_supported(phys)) {
        panic("this device does not support VK_KHR_ray_tracing_pipeline")
    }
    let gfx = select_graphics_queue_family(phys)
    if (!queue_family_supports_present(phys, gfx, surface)) {
        panic("graphics queue family does not support presentation")
    }
    var inscope device <- create_device_ray_tracing(phys, gfx, ["VK_KHR_swapchain"])
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, gfx, 0u)

    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    // the resident RT stack: BLAS/TLAS + pipeline + SBT + the storage image the raygen writes
    var inscope rv <- build_rt_view(device, phys, pool, queue, RT_DIM, RT_DIM)

    var inscope swap <- create_swapchain(device, phys, surface, RT_DIM, RT_DIM)
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
        let angle = float(glfwGetTime()) * 0.4
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
            var rt_stage : VkPipelineStageFlags
            rt_stage.ray_tracing_shader_khr = true
            var xfer : VkPipelineStageFlags
            xfer.transfer = true
            var bottom : VkPipelineStageFlags
            bottom.bottom_of_pipe = true
            // trace this frame's orbit angle into the storage image (GENERAL)
            transition_image(cmd, rv.image, VkImageLayout.UNDEFINED, VkImageLayout.GENERAL,
                none, shader_write, top, rt_stage)
            record_trace(cmd, rv, angle)
            // make the raygen writes available to the blit (GENERAL is a legal blit source)
            transition_image(cmd, rv.image, VkImageLayout.GENERAL, VkImageLayout.GENERAL,
                shader_write, transfer_read, rt_stage, xfer)
            let dst = vk_value_to_boost(target)
            transition_image(cmd, dst, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
                none, transfer_write, top, xfer)
            var region : VkImageBlit
            region.srcSubresource.aspectMask.color = true
            region.srcSubresource.layerCount = 1u
            region.srcOffsets[1].x = rv.width
            region.srcOffsets[1].y = rv.height
            region.srcOffsets[1].z = 1
            region.dstSubresource.aspectMask.color = true
            region.dstSubresource.layerCount = 1u
            region.dstOffsets[1].x = swap.width
            region.dstOffsets[1].y = swap.height
            region.dstOffsets[1].z = 1
            unsafe {
                vkCmdBlitImage(boost_value_to_vk(cmd), boost_value_to_vk(rv.image), VkImageLayout.GENERAL,
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
