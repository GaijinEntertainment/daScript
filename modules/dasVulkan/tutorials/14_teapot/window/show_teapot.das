options gen2
options _comment_hygiene = true
options indenting = 4

// Windowed teapot: the Tutorial 14 mesh-shader pipeline (teapot_task/mesh/frag) renders the GPU-
// tessellated Utah teapot into the same offscreen color+depth target the [test] uses, then blits it
// onto the swapchain image -- so the only window-specific code is the surface + swapchain + per-frame
// blit (show_teapot reuses build_teapot_resources). The teapot spins; the task shader backface-culls
// patches every frame. Needs a display, the GLFW module, and a VK_EXT_mesh_shader-capable driver, so
// it lives in window/ which the tutorials .das_test skips in CI. Run from the daslang root:
//   daslang -load_module <path-to-dasVulkan> <path-to>/tutorials/14_teapot/window/show_teapot.das
// Close the window to exit.

require glfw/glfw_boost
require vulkan
require vulkan/vulkan_boost
require vulkan/vulkan_window
require ../teapot_tut.das
require daslib/defer

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
    var window = glfwCreateWindow(TEAPOT_W, TEAPOT_H, "dasVulkan tutorial 14 - Utah teapot (mesh shaders)", null, null)
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
    var inscope instance <- create_instance("dasVulkan tutorial 14 (window)", make_api_version(1u, 3u, 0u), inst_exts)
    volkLoadInstance(boost_value_to_vk(instance))

    var inscope surface <- create_surface(instance, glfwGetNativeWindow(window), glfwGetNativeDisplay())

    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    if (!queue_family_supports_present(phys, gfx, surface)) {
        panic("graphics queue family does not support presentation")
    }
    if (!mesh_shader_supported(phys)) {
        panic("this device does not support VK_EXT_mesh_shader (meshShader+taskShader); cannot run tutorial 14")
    }
    // swapchain + mesh-shader extension & features in one device
    var mf : VkPhysicalDeviceMeshShaderFeaturesEXT
    mf.meshShader = 1u
    mf.taskShader = 1u
    var inscope device <- create_device(phys, gfx, ["VK_KHR_swapchain", "VK_EXT_mesh_shader"], mf)
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, gfx, 0u)

    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    var inscope res <- build_teapot_resources(device, phys)
    var inscope swap <- create_swapchain(device, phys, surface, TEAPOT_W, TEAPOT_H)
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
        update_teapot_uniforms(res, device, t, false)

        let ok = present_frame(device, queue, swap, pool, sync) $(cmd; target; _idx) {
            // record_teapot_render_pass leaves res.color.image in TRANSFER_SRC_OPTIMAL (render-pass finalLayout)
            record_teapot_render_pass(res, cmd)

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
            region.srcOffsets[1].x = TEAPOT_W
            region.srcOffsets[1].y = TEAPOT_H
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
