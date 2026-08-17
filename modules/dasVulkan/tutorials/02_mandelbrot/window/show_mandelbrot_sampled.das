options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 02, present method TWO of two: SAMPLE-AS-TEXTURE. Same animated Mandelbrot zoom as the blit
// viewer -- the compute shader recomputes the fractal every frame from a `time` push constant -- but here
// a graphics pass draws a single full-screen triangle whose fragment shader SAMPLES that image as a
// texture (mandelbrot_view_shaders.das). The result reaches the swapchain through the ordinary render
// pass, so this reuses draw_frame, the framebuffers and the graphics pipeline wholesale -- it adds only a
// sampler, one combined-image-sampler descriptor, and the two view shaders. Because a compute dispatch
// cannot run inside a render pass, each frame runs the compute as its own submit first, then draw_frame
// samples the result. Compare show_mandelbrot_blit.das, which blits the image straight onto the swapchain.
//
// Needs a display + the glfw module, so it lives in this window/ subfolder the tutorials .das_test skips
// in CI (the lavapipe CI daslang build is -DDAS_GLFW_DISABLED=ON). Run from the daslang root:
//   daslang -load_module <path-to-dasVulkan> <path-to>/tutorials/02_mandelbrot/window/show_mandelbrot_sampled.das
// Close the window to exit.

require glfw/glfw_boost
require vulkan
require vulkan/vulkan_boost
require vulkan/vulkan_window
require mandelbrot_zoom_shaders
require mandelbrot_view_shaders
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
    var window = glfwCreateWindow(ZOOM_DIM, ZOOM_DIM, "dasVulkan tutorial 02 - Mandelbrot zoom (sampled)", null, null)
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
    var inscope instance <- create_instance("dasVulkan tutorial 02 (sampled)", make_api_version(1u, 3u, 0u), inst_exts)
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

    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    // a sampler to read the fractal with (linear so window scaling is smooth)
    let sci = SamplerCreateInfo(magFilter = VkFilter.LINEAR, minFilter = VkFilter.LINEAR, mipmapMode = VkSamplerMipmapMode.LINEAR)
    var inscope sampler <- create_sampler(device, sci)

    // graphics descriptor set: binding 0 = combined image sampler (the compute image view), fragment stage
    var dslci : DescriptorSetLayoutCreateInfo
    var b0 = DescriptorSetLayoutBinding(binding = 0u, descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    b0.stageFlags.fragment = true
    dslci.pBindings |> emplace(b0)
    var inscope set_layout <- create_descriptor_set_layout(device, dslci)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u
    var ps0 = DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    dpci.pPoolSizes |> push(ps0)
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(desc_pool)
    dsai.descriptorSetCount = 1u
    var raw_set_layout = boost_value_to_vk(set_layout)
    var raw_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_set_layout)
        vk_check(vkAllocateDescriptorSets(boost_value_to_vk(device), dsai, addr(raw_set)), null)
    }

    var write = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 0u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    var iinfo = DescriptorImageInfo(sampler = weak_copy(sampler), imageView = weak_copy(mc.view),
        imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
    write.pImageInfo |> push(iinfo)
    var writes : array<WriteDescriptorSet>
    writes |> emplace(write)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    // graphics pipeline: full-screen triangle vert + texture-sampling frag, the descriptor set in the layout
    var inscope swap <- create_swapchain(device, phys, surface, ZOOM_DIM, ZOOM_DIM)
    var inscope render_pass <- create_render_pass_single_color(device, swap.format, VkImageLayout.PRESENT_SRC_KHR)
    build_swapchain_framebuffers(device, swap, render_pass)
    var inscope vert <- create_shader_module(device, fsq_vert_spv)
    var inscope frag <- create_shader_module(device, fsq_frag_spv)
    var plci : PipelineLayoutCreateInfo
    plci.pSetLayouts <- [weak_copy(set_layout)]
    var inscope layout <- create_pipeline_layout(device, plci)
    var inscope pipeline <- create_graphics_pipeline_simple(device, render_pass, layout, vert, frag, swap.width, swap.height, true)

    var inscope sync <- create_frame_sync(device)
    let clear = clear_color(0.0f, 0.0f, 0.0f, 1.0f)

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
            recreate_swapchain(swap, device, phys, surface, render_pass, fbw, fbh)
        }
        let t = float(glfwGetTime())
        // recompute the fractal for this frame, then leave it ready to sample (its own submit -- a
        // compute dispatch cannot run inside the draw_frame render pass)
        run_cmd_sync(device, pool, queue) $(cmd) {
            let none : VkAccessFlags
            var shader_write : VkAccessFlags
            shader_write.shader_write = true
            var shader_read : VkAccessFlags
            shader_read.shader_read = true
            var top : VkPipelineStageFlags
            top.top_of_pipe = true
            var comp : VkPipelineStageFlags
            comp.compute_shader = true
            var frag_stage : VkPipelineStageFlags
            frag_stage.fragment_shader = true
            transition_image(cmd, mc.image, VkImageLayout.UNDEFINED, VkImageLayout.GENERAL,
                none, shader_write, top, comp)
            record_compute(cmd, mc, t)
            transition_image(cmd, mc.image, VkImageLayout.GENERAL, VkImageLayout.SHADER_READ_ONLY_OPTIMAL,
                shader_write, shader_read, comp, frag_stage)
        }
        let ok = draw_frame(device, queue, swap, render_pass, pool, sync, clear, true) $(cmd) {
            cmd_bind_pipeline(cmd, pipeline)
            var sets : array<DescriptorSet>
            sets |> push(vk_value_to_boost(raw_set))
            var no_dyn : array<uint>
            cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, layout, 0u, sets, no_dyn)
            cmd_draw(cmd, 3u)
        }
        if (!ok) {
            recreate_swapchain(swap, device, phys, surface, render_pass, fbw, fbh)
        }
    }
    vkDeviceWaitIdle(boost_value_to_vk(device))
}
