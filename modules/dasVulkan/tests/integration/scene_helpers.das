options gen2
options _comment_hygiene = true
options indenting = 4

// Shared offscreen scenes for the integration tests: render the triangle and
// run the compute square, returning the result data for assertions. No [test]
// functions here -- required by the test files.

module scene_helpers

require vulkan public
require vulkan/vulkan_boost public
require compute_shaders
require triangle_shaders
require phase4_shaders
require array_shaders
require separate_shaders
require vulkan/vulkan_reflect
require dastest/testing_boost public

let TRI_W = 256
let TRI_H = 256

//! render the RGB triangle offscreen and return the RGBA pixels (TRI_W*TRI_H*4)
def public render_triangle() : array<uint8> {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, gfx, 0u)
    let fmt = VkFormat.R8G8B8A8_UNORM

    var inscope target <- build_offscreen_target(device, phys, TRI_W, TRI_H, fmt)
    var inscope render_pass <- create_render_pass_single_color(device, fmt)
    var fbci = FramebufferCreateInfo(renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(target.view)],
        width = uint(TRI_W), height = uint(TRI_H), layers = 1u)
    var inscope framebuffer <- create_framebuffer(device, fbci)

    // tri_vert_spv / tri_frag_spv are emitted by dasSpirv from triangle_shaders.das (array<uint> words)
    var inscope vert <- create_shader_module(device, tri_vert_spv)
    var inscope frag <- create_shader_module(device, tri_frag_spv)
    var inscope layout <- create_pipeline_layout(device)
    var inscope pipeline <- create_graphics_pipeline_simple(device, render_pass, layout, vert, frag, TRI_W, TRI_H)

    let buf_size = uint64(TRI_W * TRI_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    run_cmd_sync(device, pool, queue) $(cmd) {
        record_render_pass(cmd, render_pass, framebuffer, full_area(TRI_W, TRI_H), clear_color(0.1f, 0.1f, 0.15f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipeline)
            cmd_draw(cmd, 3u)
        }
        copy_image_to_buffer(cmd, target.image, readback, TRI_W, TRI_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! run the compute square (out[i] = i*i) over `n` elements and return the result
def public run_compute(n : int) : array<uint> {   // nolint:STYLE038 - flat command-record run
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var values : array<uint>
    values |> resize(n)
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, gfx, 0u)

    let buf_size = uint64(n * 4)
    var bci = VkBufferCreateInfo()
    bci.size = buf_size
    bci.usage.storage_buffer = true
    var buffer : VkBuffer
    vk_check(vkCreateBuffer(dev, bci, null, buffer), null)
    var req : VkMemoryRequirements
    vkGetBufferMemoryRequirements(dev, buffer, req)
    var want : VkMemoryPropertyFlags
    want.host_visible = true
    want.host_coherent = true
    var mai = VkMemoryAllocateInfo()
    mai.allocationSize = req.size
    mai.memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want)
    var memory : VkDeviceMemory
    vk_check(vkAllocateMemory(dev, mai, null, memory), null)
    vk_check(vkBindBufferMemory(dev, buffer, memory, 0ul), null)

    var binding : VkDescriptorSetLayoutBinding
    binding.descriptorType = VkDescriptorType.STORAGE_BUFFER
    binding.descriptorCount = 1u
    binding.stageFlags.compute = true
    var dslci = VkDescriptorSetLayoutCreateInfo()
    dslci.bindingCount = 1u
    var set_layout : VkDescriptorSetLayout
    unsafe {
        dslci.pBindings = addr(binding)
        vk_check(vkCreateDescriptorSetLayout(dev, dslci, null, set_layout), null)
    }
    var pool_size : VkDescriptorPoolSize
    pool_size.type_ = VkDescriptorType.STORAGE_BUFFER
    pool_size.descriptorCount = 1u
    var dpci = VkDescriptorPoolCreateInfo()
    dpci.maxSets = 1u
    dpci.poolSizeCount = 1u
    var desc_pool : VkDescriptorPool
    unsafe {
        dpci.pPoolSizes = addr(pool_size)
        vk_check(vkCreateDescriptorPool(dev, dpci, null, desc_pool), null)
    }
    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = desc_pool
    dsai.descriptorSetCount = 1u
    var desc_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(set_layout)
        vk_check(vkAllocateDescriptorSets(dev, dsai, addr(desc_set)), null)
    }
    var dbinfo : VkDescriptorBufferInfo
    dbinfo.buffer = buffer
    dbinfo.range_ = buf_size
    var write = VkWriteDescriptorSet()
    write.dstSet = desc_set
    write.descriptorCount = 1u
    write.descriptorType = VkDescriptorType.STORAGE_BUFFER
    unsafe {
        write.pBufferInfo = addr(dbinfo)
        vkUpdateDescriptorSets(dev, 1u, addr(write), 0u, null)
    }
    var inscope shader <- create_shader_module(device, square_spv)
    var plci = VkPipelineLayoutCreateInfo()
    plci.setLayoutCount = 1u
    var pipe_layout : VkPipelineLayout
    unsafe {
        plci.pSetLayouts = addr(set_layout)
        vk_check(vkCreatePipelineLayout(dev, plci, null, pipe_layout), null)
    }
    var stage = VkPipelineShaderStageCreateInfo()
    stage.stage.compute = true
    stage.module_ = boost_value_to_vk(shader)
    stage.pName = "main"
    var cpci = VkComputePipelineCreateInfo()
    cpci.stage = stage
    cpci.layout = pipe_layout
    var pipeline : VkPipeline
    unsafe {
        vk_check(vkCreateComputePipelines(dev, null, 1u, addr(cpci), null, addr(pipeline)), null)
    }
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    run_cmd_sync(device, pool, queue) $(cmd) {
        vkCmdBindPipeline(boost_value_to_vk(cmd), VkPipelineBindPoint.COMPUTE, pipeline)
        unsafe {
            vkCmdBindDescriptorSets(boost_value_to_vk(cmd), VkPipelineBindPoint.COMPUTE, pipe_layout, 0u, 1u, addr(desc_set), 0u, null)
        }
        cmd_dispatch(cmd, uint(n / 64), 1u, 1u)
    }
    var mapped : void? = null
    let mf : VkMemoryMapFlags
    unsafe {
        vk_check(vkMapMemory(dev, memory, 0ul, buf_size, mf, addr(mapped)), null)
        memcpy(addr<void?>(values[0]), mapped, n * 4)
    }
    vkUnmapMemory(dev, memory)
    vkDestroyPipeline(dev, pipeline, null)
    vkDestroyPipelineLayout(dev, pipe_layout, null)
    vkDestroyDescriptorPool(dev, desc_pool, null)
    vkDestroyDescriptorSetLayout(dev, set_layout, null)
    vkDestroyBuffer(dev, buffer, null)
    vkFreeMemory(dev, memory, null)
    return <- values
}

def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * TRI_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}

//! Exact-analytic image oracle: assert EVERY pixel of a w*h RGBA8 buffer equals `expected(x,y)`.
//! Reports the FIRST mismatch (coords + got/want) as one dastest success — stronger than the `px`
//! spot-checks, and the right fit for CPU-computable content (gradients, NEAREST-sampled texels).
def public assert_pixels_exact(t : T?; pixels : array<uint8>; w, h : int; lbl : string;
                               expected : block<(x, y : int) : int4>) {
    let need = uint64(w) * uint64(h) * 4ul   // uint64 so the w*h*4 byte count can't overflow int
    if (uint64(long_length(pixels)) < need) {     // guard before indexing pixels[p+3]: a wrong-sized buffer
        t |> success(false, "{lbl}: pixel buffer too small ({long_length(pixels)} < {need})")
        return                               // is a clean test failure, not an out-of-bounds crash
    }
    var ok = true
    var msg = "{lbl}: all {w}x{h} pixels match expected"
    for (y in range(h)) {
        for (x in range(w)) {
            let p = (y * w + x) * 4
            let got = int4(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]), int(pixels[p + 3]))
            let want = invoke(expected, x, y)
            if (got.x != want.x || got.y != want.y || got.z != want.z || got.w != want.w) {
                ok = false
                msg = "{lbl}: first mismatch at ({x},{y}): got {got}, want {want}"
                break
            }
        }
        if (!ok) break
    }
    t |> success(ok, msg)
}

//! the dasSpirv GPU gate: run an emitted SPIR-V compute blob (`words`) over `n` elements, descriptor
//! layout / pool / update all through the BOOST path. The storage-buffer binding is descriptorCount=1
//! with no immutable samplers -- the independent-count case erasure got wrong (0 => out[i] != i*i).
def public run_compute_spirv(var words : array<uint>; n : int) : array<uint> {   // nolint:STYLE038 - flat command-record run
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var values : array<uint>
    values |> resize(n)
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, gfx, 0u)

    let buf_size = uint64(n * 4)
    var bci : BufferCreateInfo
    bci.size = buf_size
    bci.usage.storage_buffer = true
    var inscope buffer <- create_buffer(device, bci)
    var req : VkMemoryRequirements
    vkGetBufferMemoryRequirements(dev, boost_value_to_vk(buffer), req)
    var want : VkMemoryPropertyFlags
    want.host_visible = true
    want.host_coherent = true
    let mai = MemoryAllocateInfo(allocationSize = req.size,
        memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    var inscope memory <- allocate_memory(device, mai)
    vk_check(vkBindBufferMemory(dev, boost_value_to_vk(buffer), boost_value_to_vk(memory), 0ul), null)

    // descriptor set layout + pool via BOOST (descriptorCount = 1, no samplers)
    var dslci : DescriptorSetLayoutCreateInfo
    var b0 = DescriptorSetLayoutBinding(binding = 0u,
        descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    b0.stageFlags.compute = true
    dslci.pBindings |> emplace(b0)
    var inscope set_layout <- create_descriptor_set_layout(device, dslci)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u
    var psize = DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    dpci.pPoolSizes |> push(psize)
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(desc_pool)
    dsai.descriptorSetCount = 1u
    var raw_set_layout = boost_value_to_vk(set_layout)
    var raw_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_set_layout)
        vk_check(vkAllocateDescriptorSets(dev, dsai, addr(raw_set)), null)
    }

    // bind the buffer through BOOST update_descriptor_sets (descriptorCount = 1, only pBufferInfo)
    var write = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 0u,
        descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    var binfo = DescriptorBufferInfo(buffer = weak_copy(buffer), range_ = buf_size)
    write.pBufferInfo |> push(binfo)
    var writes : array<WriteDescriptorSet>
    writes |> emplace(write)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    // compute pipeline (raw: compute pipelines are batch-created)
    var inscope shader <- create_shader_module(device, words)
    var plci : PipelineLayoutCreateInfo
    plci.pSetLayouts <- [weak_copy(set_layout)]
    var inscope pipe_layout <- create_pipeline_layout(device, plci)
    var stage = VkPipelineShaderStageCreateInfo()
    stage.stage.compute = true
    stage.module_ = boost_value_to_vk(shader)
    stage.pName = "main"
    var cpci = VkComputePipelineCreateInfo()
    cpci.stage = stage
    cpci.layout = boost_value_to_vk(pipe_layout)
    var pipeline : VkPipeline
    unsafe {
        vk_check(vkCreateComputePipelines(dev, null, 1u, addr(cpci), null, addr(pipeline)), null)
    }

    var inscope pool <- create_command_pool(device, pool_info(gfx))
    run_cmd_sync(device, pool, queue) $(cmd) {
        vkCmdBindPipeline(boost_value_to_vk(cmd), VkPipelineBindPoint.COMPUTE, pipeline)
        unsafe {
            vkCmdBindDescriptorSets(boost_value_to_vk(cmd), VkPipelineBindPoint.COMPUTE, boost_value_to_vk(pipe_layout), 0u, 1u, addr(raw_set), 0u, null)
        }
        cmd_dispatch(cmd, uint(n / 64), 1u, 1u)
    }

    var mapped : void? = null
    let mf : VkMemoryMapFlags
    unsafe {
        vk_check(vkMapMemory(dev, boost_value_to_vk(memory), 0ul, buf_size, mf, addr(mapped)), null)
        memcpy(addr<void?>(values[0]), mapped, n * 4)
    }
    vkUnmapMemory(dev, boost_value_to_vk(memory))
    vkDestroyPipeline(dev, pipeline, null)
    return <- values
}

//! the boost-descriptor compute square, now fed by the daslang-emitted `square_spv` blob.
def public run_compute_boost(n : int) : array<uint> {
    return <- run_compute_spirv(square_spv, n)
}

//! the dasSpirv storage-image GPU gate: run a compute blob (`words`) that writes a w*h RGBA8 storage
//! image at set=0/binding=0, then read it back as RGBA8 bytes. The image analog of run_compute_spirv.
//! STORAGE usage + GENERAL layout for the compute write, then TRANSFER_SRC for the readback copy.
def public compute_image_rgba8(var words : array<uint>; w, h : int) : array<uint8> {
    // delegates to the public boost rail (daslib/vulkan_boost.das); kept as a named test entry point so
    // the image-phase tests read cleanly. The storage-image + descriptor + dispatch + readback path now
    // lives once, in boost, where real users (and the Mandelbrot tutorial) reach it.
    return <- compute_to_storage_image(words, w, h)
}

//! the dasSpirv texelFetch GPU gate: upload `src` (w*h RGBA8) as a sampled texture at set=0/binding=0,
//! run a compute blob (`words`) that texelFetch-copies it into a storage image at binding=1, read back.
//! Exact-analytic: the caller asserts readback == `src`, proving texelFetch indexes the right texel.
def public texelfetch_copy_rgba8(var words : array<uint>; var src : array<uint8>; w, h : int) : array<uint8> {   // nolint:STYLE038 - flat command-record run
    // the dispatch is w/8 x h/8 (8x8 local size); require exact multiples so no texels are silently
    // skipped and extents are never 0/negative -- a misuse fails deterministically here.
    assert(w > 0 && h > 0 && (w % 8) == 0 && (h % 8) == 0,
        "texelfetch_copy_rgba8: w/h must be positive multiples of 8")
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, gfx, 0u)
    let fmt = VkFormat.R8G8B8A8_UNORM
    let byte_count = uint64(w) * uint64(h) * 4ul
    assert(uint64(long_length(src)) == byte_count, "texelfetch_copy_rgba8: src must be exactly w*h*4 bytes")

    // source sampled texture (SAMPLED + TRANSFER_DST), device-local + view + NEAREST sampler
    var sici = ImageCreateInfo(imageType = VkImageType._2D, format = fmt,
        mipLevels = 1u, arrayLayers = 1u, tiling = VkImageTiling.OPTIMAL,
        initialLayout = VkImageLayout.UNDEFINED)
    sici.extent.width = uint(w)
    sici.extent.height = uint(h)
    sici.extent.depth = 1u
    sici.samples._1 = true
    sici.usage.sampled = true
    sici.usage.transfer_dst = true
    var inscope src_image <- create_image(device, sici)
    var sreq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(src_image), sreq)
    var swant : VkMemoryPropertyFlags
    swant.device_local = true
    let smai = MemoryAllocateInfo(allocationSize = sreq.size,
        memoryTypeIndex = find_memory_type(phys, sreq.memoryTypeBits, swant))
    var inscope src_mem <- allocate_memory(device, smai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(src_image), boost_value_to_vk(src_mem), 0ul), null)
    var svci = ImageViewCreateInfo(image = weak_copy(src_image), viewType = VkImageViewType._2D, format = fmt)
    svci.subresourceRange.aspectMask.color = true
    svci.subresourceRange.levelCount = 1u
    svci.subresourceRange.layerCount = 1u
    var inscope src_view <- create_image_view(device, svci)
    let sci = SamplerCreateInfo(magFilter = VkFilter.NEAREST, minFilter = VkFilter.NEAREST,
        mipmapMode = VkSamplerMipmapMode.NEAREST)
    var inscope sampler <- create_sampler(device, sci)

    // staging buffer with the source bytes
    var staging_usage : VkBufferUsageFlags
    staging_usage.transfer_src = true
    var inscope staging <- make_host_buffer(device, phys, byte_count, staging_usage)
    upload(device, staging.memory, src)

    // destination storage image (STORAGE + TRANSFER_SRC), device-local + view
    var dici = ImageCreateInfo(imageType = VkImageType._2D, format = fmt,
        mipLevels = 1u, arrayLayers = 1u, tiling = VkImageTiling.OPTIMAL,
        initialLayout = VkImageLayout.UNDEFINED)
    dici.extent.width = uint(w)
    dici.extent.height = uint(h)
    dici.extent.depth = 1u
    dici.samples._1 = true
    dici.usage.storage = true
    dici.usage.transfer_src = true
    var inscope dst_image <- create_image(device, dici)
    var dreq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(dst_image), dreq)
    var dwant : VkMemoryPropertyFlags
    dwant.device_local = true
    let dmai = MemoryAllocateInfo(allocationSize = dreq.size,
        memoryTypeIndex = find_memory_type(phys, dreq.memoryTypeBits, dwant))
    var inscope dst_mem <- allocate_memory(device, dmai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(dst_image), boost_value_to_vk(dst_mem), 0ul), null)
    var dvci = ImageViewCreateInfo(image = weak_copy(dst_image), viewType = VkImageViewType._2D, format = fmt)
    dvci.subresourceRange.aspectMask.color = true
    dvci.subresourceRange.levelCount = 1u
    dvci.subresourceRange.layerCount = 1u
    var inscope dst_view <- create_image_view(device, dvci)

    // descriptor set layout: binding 0 = combined image sampler, binding 1 = storage image
    var dslci : DescriptorSetLayoutCreateInfo
    var b0 = DescriptorSetLayoutBinding(binding = 0u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    b0.stageFlags.compute = true
    dslci.pBindings |> emplace(b0)
    var b1 = DescriptorSetLayoutBinding(binding = 1u,
        descriptorType = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 1u)
    b1.stageFlags.compute = true
    dslci.pBindings |> emplace(b1)
    var inscope set_layout <- create_descriptor_set_layout(device, dslci)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u
    var ps0 = DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    dpci.pPoolSizes |> push(ps0)
    var ps1 = DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 1u)
    dpci.pPoolSizes |> push(ps1)
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(desc_pool)
    dsai.descriptorSetCount = 1u
    var raw_set_layout = boost_value_to_vk(set_layout)
    var raw_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_set_layout)
        vk_check(vkAllocateDescriptorSets(dev, dsai, addr(raw_set)), null)
    }

    var writes : array<WriteDescriptorSet>
    var w0 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 0u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    var srcinfo = DescriptorImageInfo(sampler = weak_copy(sampler), imageView = weak_copy(src_view),
        imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
    w0.pImageInfo |> push(srcinfo)
    writes |> emplace(w0)
    var w1 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 1u,
        descriptorType = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 1u)
    var dstinfo = DescriptorImageInfo(imageView = weak_copy(dst_view), imageLayout = VkImageLayout.GENERAL)
    w1.pImageInfo |> push(dstinfo)
    writes |> emplace(w1)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    var inscope shader <- create_shader_module(device, words)
    var plci : PipelineLayoutCreateInfo
    plci.pSetLayouts <- [weak_copy(set_layout)]
    var inscope pipe_layout <- create_pipeline_layout(device, plci)
    var inscope pipeline <- create_compute_pipeline(device, pipe_layout, shader)

    var inscope readback <- create_host_buffer(device, phys, byte_count)
    var inscope pool <- create_command_pool(device, pool_info(gfx))

    run_cmd_sync(device, pool, queue) $(cmd) {
        let none : VkAccessFlags
        var to_dst : VkAccessFlags
        to_dst.transfer_write = true
        var shader_read : VkAccessFlags
        shader_read.shader_read = true
        var shader_write : VkAccessFlags
        shader_write.shader_write = true
        var to_read : VkAccessFlags
        to_read.transfer_read = true
        var top : VkPipelineStageFlags
        top.top_of_pipe = true
        var xfer : VkPipelineStageFlags
        xfer.transfer = true
        var comp : VkPipelineStageFlags
        comp.compute_shader = true
        // upload the source: UNDEFINED -> TRANSFER_DST, copy, -> SHADER_READ_ONLY
        transition_image(cmd, src_image, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
            none, to_dst, top, xfer)
        var region : BufferImageCopy
        region.imageSubresource.aspectMask.color = true
        region.imageSubresource.layerCount = 1u
        region.imageExtent.width = uint(w)
        region.imageExtent.height = uint(h)
        region.imageExtent.depth = 1u
        var regions : array<BufferImageCopy>
        regions |> emplace(region)
        cmd_copy_buffer_to_image(cmd, staging.buffer, src_image, VkImageLayout.TRANSFER_DST_OPTIMAL, regions)
        transition_image(cmd, src_image, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.SHADER_READ_ONLY_OPTIMAL,
            to_dst, shader_read, xfer, comp)
        // destination storage image: UNDEFINED -> GENERAL
        transition_image(cmd, dst_image, VkImageLayout.UNDEFINED, VkImageLayout.GENERAL,
            none, shader_write, top, comp)
        cmd_bind_pipeline(cmd, pipeline, VkPipelineBindPoint.COMPUTE)
        var sets : array<DescriptorSet>
        sets |> push(vk_value_to_boost(raw_set))
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.COMPUTE, pipe_layout, 0u, sets, no_dyn)
        cmd_dispatch(cmd, uint(w / 8), uint(h / 8), 1u)
        // destination GENERAL -> TRANSFER_SRC for the readback copy
        transition_image(cmd, dst_image, VkImageLayout.GENERAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
            shader_write, to_read, comp, xfer)
        copy_image_to_buffer(cmd, dst_image, readback, w, h)
    }
    map_memory_to_array(device, readback.memory, byte_count) $(m) {
        pixels := m
    }
    return <- pixels
}

//! the dasSpirv sampler2DArray GPU gate: render a fullscreen quad sampling a 4-layer 2D array texture,
//! the layer chosen per screen quadrant -- proving the Arrayed image + layer (.z) coordinate select
//! the right layer. Layer colors are red/green/blue/yellow (0..3). Returns RGBA (TRI_W*TRI_H*4).
def public render_sampler2darray() : array<uint8> {   // nolint:STYLE038 - flat command-record run
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, gfx, 0u)
    let fmt = VkFormat.R8G8B8A8_UNORM

    var inscope target <- build_offscreen_target(device, phys, TRI_W, TRI_H, fmt)
    var inscope render_pass <- create_render_pass_single_color(device, fmt)
    var fbci = FramebufferCreateInfo(renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(target.view)],
        width = uint(TRI_W), height = uint(TRI_H), layers = 1u)
    var inscope framebuffer <- create_framebuffer(device, fbci)

    // a 2x2-per-layer, 4-layer array texture: layer 0 red, 1 green, 2 blue, 3 yellow (each solid)
    let lw = 2
    let lh = 2
    let layers = 4
    var texels <- [for (_i in range(lw * lh * layers * 4)); uint8(0)]
    let cols = fixed_array(
        fixed_array(255, 0, 0, 255),        // layer 0 red
        fixed_array(0, 255, 0, 255),        // layer 1 green
        fixed_array(0, 0, 255, 255),        // layer 2 blue
        fixed_array(255, 255, 0, 255))      // layer 3 yellow
    for (l in range(layers)) {
        for (t in range(lw * lh)) {
            let base = (l * lw * lh + t) * 4
            for (c in range(4)) {
                texels[base + c] = uint8(cols[l][c])
            }
        }
    }
    var staging_usage : VkBufferUsageFlags
    staging_usage.transfer_src = true
    var inscope staging <- make_host_buffer(device, phys, uint64(long_length(texels)), staging_usage)
    upload(device, staging.memory, texels)
    delete texels

    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = fmt,
        mipLevels = 1u, arrayLayers = uint(layers), tiling = VkImageTiling.OPTIMAL,
        initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = uint(lw)
    ici.extent.height = uint(lh)
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.sampled = true
    ici.usage.transfer_dst = true
    var inscope tex_image <- create_image(device, ici)
    var ireq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(tex_image), ireq)
    var iwant : VkMemoryPropertyFlags
    iwant.device_local = true
    let imai = MemoryAllocateInfo(allocationSize = ireq.size,
        memoryTypeIndex = find_memory_type(phys, ireq.memoryTypeBits, iwant))
    var inscope tex_memory <- allocate_memory(device, imai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(tex_image), boost_value_to_vk(tex_memory), 0ul), null)

    var tvci = ImageViewCreateInfo(image = weak_copy(tex_image), viewType = VkImageViewType._2D_ARRAY, format = fmt)
    tvci.subresourceRange.aspectMask.color = true
    tvci.subresourceRange.levelCount = 1u
    tvci.subresourceRange.layerCount = uint(layers)
    var inscope tex_view <- create_image_view(device, tvci)

    let sci = SamplerCreateInfo(magFilter = VkFilter.NEAREST, minFilter = VkFilter.NEAREST,
        mipmapMode = VkSamplerMipmapMode.NEAREST,
        addressModeU = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeV = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeW = VkSamplerAddressMode.CLAMP_TO_EDGE)
    var inscope sampler <- create_sampler(device, sci)

    var dslci : DescriptorSetLayoutCreateInfo
    var b0 = DescriptorSetLayoutBinding(binding = 0u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
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
        vk_check(vkAllocateDescriptorSets(dev, dsai, addr(raw_set)), null)
    }
    var write = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 0u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    var iinfo = DescriptorImageInfo(sampler = weak_copy(sampler), imageView = weak_copy(tex_view),
        imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
    write.pImageInfo |> push(iinfo)
    var writes : array<WriteDescriptorSet>
    writes |> emplace(write)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    var inscope pipe_layout <- create_pipeline_layout(device, PipelineLayoutCreateInfo(pSetLayouts <- [weak_copy(set_layout)]))
    var inscope vert <- create_shader_module(device, arr_vert_spv)
    var inscope frag <- create_shader_module(device, arr_frag_spv)
    var inscope pipeline <- create_graphics_pipeline_simple(device, render_pass, pipe_layout, vert, frag, TRI_W, TRI_H)

    let buf_size = uint64(TRI_W * TRI_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)
    var inscope pool <- create_command_pool(device, pool_info(gfx))

    run_cmd_sync(device, pool, queue) $(cmd) {
        let none : VkAccessFlags
        var to_dst : VkAccessFlags
        to_dst.transfer_write = true
        var to_read : VkAccessFlags
        to_read.shader_read = true
        var top : VkPipelineStageFlags
        top.top_of_pipe = true
        var xfer : VkPipelineStageFlags
        xfer.transfer = true
        var frag_stage : VkPipelineStageFlags
        frag_stage.fragment_shader = true
        transition_image(cmd, tex_image, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
            none, to_dst, top, xfer)
        var region : BufferImageCopy
        region.imageSubresource.aspectMask.color = true
        region.imageSubresource.layerCount = uint(layers)
        region.imageExtent.width = uint(lw)
        region.imageExtent.height = uint(lh)
        region.imageExtent.depth = 1u
        var regions : array<BufferImageCopy>
        regions |> emplace(region)
        cmd_copy_buffer_to_image(cmd, staging.buffer, tex_image, VkImageLayout.TRANSFER_DST_OPTIMAL, regions)
        transition_image(cmd, tex_image, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.SHADER_READ_ONLY_OPTIMAL,
            to_dst, to_read, xfer, frag_stage)
        record_render_pass(cmd, render_pass, framebuffer, full_area(TRI_W, TRI_H), clear_color(0.1f, 0.1f, 0.15f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipeline)
            var sets : array<DescriptorSet>
            sets |> push(vk_value_to_boost(raw_set))
            var no_dyn : array<uint>
            cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, pipe_layout, 0u, sets, no_dyn)
            cmd_draw(cmd, 6u)
        }
        copy_image_to_buffer(cmd, target.image, readback, TRI_W, TRI_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! the dasSpirv separate-image+sampler GPU gate: a fullscreen quad sampling a standalone texture2D
//! (binding 0, SAMPLED_IMAGE) + standalone sampler (binding 1, SAMPLER) via sampleTexture. The 2x2
//! checkerboard (red/green/blue/yellow) puts one texel per quadrant. Returns RGBA (TRI_W*TRI_H*4).
def public render_separate_image_sampler() : array<uint8> {   // nolint:STYLE038 - flat command-record run
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, gfx, 0u)
    let fmt = VkFormat.R8G8B8A8_UNORM

    var inscope target <- build_offscreen_target(device, phys, TRI_W, TRI_H, fmt)
    var inscope render_pass <- create_render_pass_single_color(device, fmt)
    var fbci = FramebufferCreateInfo(renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(target.view)],
        width = uint(TRI_W), height = uint(TRI_H), layers = 1u)
    var inscope framebuffer <- create_framebuffer(device, fbci)

    // 2x2 checkerboard: (0,0) red, (1,0) green, (0,1) blue, (1,1) yellow
    var texels <- [for (c in fixed_array(
        255, 0, 0, 255,
        0, 255, 0, 255,
        0, 0, 255, 255,
        255, 255, 0, 255)); uint8(c)]
    var staging_usage : VkBufferUsageFlags
    staging_usage.transfer_src = true
    var inscope staging <- make_host_buffer(device, phys, 16ul, staging_usage)
    upload(device, staging.memory, texels)
    delete texels

    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = fmt,
        mipLevels = 1u, arrayLayers = 1u, tiling = VkImageTiling.OPTIMAL,
        initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = 2u
    ici.extent.height = 2u
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.sampled = true
    ici.usage.transfer_dst = true
    var inscope tex_image <- create_image(device, ici)
    var ireq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(tex_image), ireq)
    var iwant : VkMemoryPropertyFlags
    iwant.device_local = true
    let imai = MemoryAllocateInfo(allocationSize = ireq.size,
        memoryTypeIndex = find_memory_type(phys, ireq.memoryTypeBits, iwant))
    var inscope tex_memory <- allocate_memory(device, imai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(tex_image), boost_value_to_vk(tex_memory), 0ul), null)
    var tvci = ImageViewCreateInfo(image = weak_copy(tex_image), viewType = VkImageViewType._2D, format = fmt)
    tvci.subresourceRange.aspectMask.color = true
    tvci.subresourceRange.levelCount = 1u
    tvci.subresourceRange.layerCount = 1u
    var inscope tex_view <- create_image_view(device, tvci)

    let sci = SamplerCreateInfo(magFilter = VkFilter.NEAREST, minFilter = VkFilter.NEAREST,
        mipmapMode = VkSamplerMipmapMode.NEAREST,
        addressModeU = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeV = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeW = VkSamplerAddressMode.CLAMP_TO_EDGE)
    var inscope sampler <- create_sampler(device, sci)

    // separate descriptors: binding 0 = SAMPLED_IMAGE, binding 1 = SAMPLER
    var dslci : DescriptorSetLayoutCreateInfo
    var bi = DescriptorSetLayoutBinding(binding = 0u,
        descriptorType = VkDescriptorType.SAMPLED_IMAGE, descriptorCount = 1u)
    bi.stageFlags.fragment = true
    dslci.pBindings |> emplace(bi)
    var bs = DescriptorSetLayoutBinding(binding = 1u,
        descriptorType = VkDescriptorType.SAMPLER, descriptorCount = 1u)
    bs.stageFlags.fragment = true
    dslci.pBindings |> emplace(bs)
    var inscope set_layout <- create_descriptor_set_layout(device, dslci)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u
    var psi = DescriptorPoolSize(type_ = VkDescriptorType.SAMPLED_IMAGE, descriptorCount = 1u)
    dpci.pPoolSizes |> push(psi)
    var pss = DescriptorPoolSize(type_ = VkDescriptorType.SAMPLER, descriptorCount = 1u)
    dpci.pPoolSizes |> push(pss)
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(desc_pool)
    dsai.descriptorSetCount = 1u
    var raw_set_layout = boost_value_to_vk(set_layout)
    var raw_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_set_layout)
        vk_check(vkAllocateDescriptorSets(dev, dsai, addr(raw_set)), null)
    }
    var writes : array<WriteDescriptorSet>
    var wi = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 0u,
        descriptorType = VkDescriptorType.SAMPLED_IMAGE, descriptorCount = 1u)
    var iinfo = DescriptorImageInfo(imageView = weak_copy(tex_view),
        imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
    wi.pImageInfo |> push(iinfo)
    writes |> emplace(wi)
    var ws = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 1u,
        descriptorType = VkDescriptorType.SAMPLER, descriptorCount = 1u)
    var sinfo : DescriptorImageInfo
    sinfo.sampler = weak_copy(sampler)
    ws.pImageInfo |> push(sinfo)
    writes |> emplace(ws)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    var inscope pipe_layout <- create_pipeline_layout(device, PipelineLayoutCreateInfo(pSetLayouts <- [weak_copy(set_layout)]))
    var inscope vert <- create_shader_module(device, arr_vert_spv)
    var inscope frag <- create_shader_module(device, sep_frag_spv)
    var inscope pipeline <- create_graphics_pipeline_simple(device, render_pass, pipe_layout, vert, frag, TRI_W, TRI_H)

    let buf_size = uint64(TRI_W * TRI_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)
    var inscope pool <- create_command_pool(device, pool_info(gfx))

    run_cmd_sync(device, pool, queue) $(cmd) {
        let none : VkAccessFlags
        var to_dst : VkAccessFlags
        to_dst.transfer_write = true
        var to_read : VkAccessFlags
        to_read.shader_read = true
        var top : VkPipelineStageFlags
        top.top_of_pipe = true
        var xfer : VkPipelineStageFlags
        xfer.transfer = true
        var frag_stage : VkPipelineStageFlags
        frag_stage.fragment_shader = true
        transition_image(cmd, tex_image, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
            none, to_dst, top, xfer)
        var region : BufferImageCopy
        region.imageSubresource.aspectMask.color = true
        region.imageSubresource.layerCount = 1u
        region.imageExtent.width = 2u
        region.imageExtent.height = 2u
        region.imageExtent.depth = 1u
        var regions : array<BufferImageCopy>
        regions |> emplace(region)
        cmd_copy_buffer_to_image(cmd, staging.buffer, tex_image, VkImageLayout.TRANSFER_DST_OPTIMAL, regions)
        transition_image(cmd, tex_image, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.SHADER_READ_ONLY_OPTIMAL,
            to_dst, to_read, xfer, frag_stage)
        record_render_pass(cmd, render_pass, framebuffer, full_area(TRI_W, TRI_H), clear_color(0.1f, 0.1f, 0.15f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipeline)
            var sets : array<DescriptorSet>
            sets |> push(vk_value_to_boost(raw_set))
            var no_dyn : array<uint>
            cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, pipe_layout, 0u, sets, no_dyn)
            cmd_draw(cmd, 6u)
        }
        copy_image_to_buffer(cmd, target.image, readback, TRI_W, TRI_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

def private pool_info(queue_family : uint) : CommandPoolCreateInfo {
    var ci : CommandPoolCreateInfo
    ci.queueFamilyIndex = queue_family
    return ci
}

// ===== Phase-4 GPU gate: UBO matrix + push constant + combined image sampler =====

def private make_host_buffer(device : Device; phys : VkPhysicalDevice; size : uint64; usage : VkBufferUsageFlags) : HostBuffer {
    var bci = BufferCreateInfo(size = size, usage = usage)
    var inscope buffer <- create_buffer(device, bci)
    var req : VkMemoryRequirements
    vkGetBufferMemoryRequirements(boost_value_to_vk(device), boost_value_to_vk(buffer), req)
    var want : VkMemoryPropertyFlags
    want.host_visible = true
    want.host_coherent = true
    let mai = MemoryAllocateInfo(allocationSize = req.size,
        memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    var inscope memory <- allocate_memory(device, mai)
    vk_check(vkBindBufferMemory(boost_value_to_vk(device), boost_value_to_vk(buffer), boost_value_to_vk(memory), 0ul), null)
    var hb = HostBuffer(size = size)
    hb.buffer <- buffer
    hb.memory <- memory
    return <- hb
}

def private upload(device : Device; memory : DeviceMemory; var data : array<auto(TT)>) {
    if (empty(data)) return            // vkMapMemory(size=0) is invalid; nothing to copy
    let nbytes = length(data) * typeinfo sizeof(type<TT>)
    var mapped : void? = null
    let mf : VkMemoryMapFlags
    unsafe {
        vk_check(vkMapMemory(boost_value_to_vk(device), boost_value_to_vk(memory), 0ul, uint64(nbytes), mf, addr(mapped)), null)
        memcpy(mapped, addr<void?>(data[0]), nbytes)
    }
    vkUnmapMemory(boost_value_to_vk(device), boost_value_to_vk(memory))
}

//! Phase-4 GPU gate: a textured quad transformed by a UBO MVP and tinted by a push constant; returns
//! RGBA (TRI_W*TRI_H*4). MVP scales 0.6 and translates x by +0.4 -- that translation lives in matrix
//! column 3, so a right-shifted quad proves ColMajor; tint=(1,1,0) zeroes blue (white texel -> yellow).
def public render_textured_quad() : array<uint8> {   // nolint:STYLE038 - flat command-record run
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, gfx, 0u)
    let fmt = VkFormat.R8G8B8A8_UNORM

    // offscreen color target + render pass + framebuffer (as render_triangle)
    var inscope target <- build_offscreen_target(device, phys, TRI_W, TRI_H, fmt)
    var inscope render_pass <- create_render_pass_single_color(device, fmt)
    var fbci = FramebufferCreateInfo(renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(target.view)],
        width = uint(TRI_W), height = uint(TRI_H), layers = 1u)
    var inscope framebuffer <- create_framebuffer(device, fbci)

    // UBO: a single column-major MVP. Columns are contiguous; the translation (0.4, 0) is column 3.
    var mvp <- [
        0.6f, 0.0f, 0.0f, 0.0f,     // column 0
        0.0f, 0.6f, 0.0f, 0.0f,     // column 1
        0.0f, 0.0f, 1.0f, 0.0f,     // column 2
        0.4f, 0.0f, 0.0f, 1.0f]     // column 3 (translation x = +0.4)
    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true
    var inscope ubo <- make_host_buffer(device, phys, 64ul, ubo_usage)
    upload(device, ubo.memory, mvp)
    delete mvp

    // texture: a 2x2 RGBA checkerboard, row 0 = red,green and row 1 = blue,white (NEAREST sampled)
    var texels <- [for (c in fixed_array(
        255, 0, 0, 255,         // texel (0,0) red
        0, 255, 0, 255,         // texel (1,0) green
        0, 0, 255, 255,         // texel (0,1) blue
        255, 255, 255, 255));   // texel (1,1) white
        uint8(c)]
    var staging_usage : VkBufferUsageFlags
    staging_usage.transfer_src = true
    var inscope staging <- make_host_buffer(device, phys, 16ul, staging_usage)
    upload(device, staging.memory, texels)
    delete texels

    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = fmt,
        mipLevels = 1u, arrayLayers = 1u, tiling = VkImageTiling.OPTIMAL,
        initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = 2u
    ici.extent.height = 2u
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.sampled = true
    ici.usage.transfer_dst = true
    var inscope tex_image <- create_image(device, ici)
    var ireq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(tex_image), ireq)
    var iwant : VkMemoryPropertyFlags
    iwant.device_local = true
    let imai = MemoryAllocateInfo(allocationSize = ireq.size,
        memoryTypeIndex = find_memory_type(phys, ireq.memoryTypeBits, iwant))
    var inscope tex_memory <- allocate_memory(device, imai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(tex_image), boost_value_to_vk(tex_memory), 0ul), null)

    var tvci = ImageViewCreateInfo(image = weak_copy(tex_image), viewType = VkImageViewType._2D, format = fmt)
    tvci.subresourceRange.aspectMask.color = true
    tvci.subresourceRange.levelCount = 1u
    tvci.subresourceRange.layerCount = 1u
    var inscope tex_view <- create_image_view(device, tvci)

    let sci = SamplerCreateInfo(magFilter = VkFilter.NEAREST, minFilter = VkFilter.NEAREST,
        mipmapMode = VkSamplerMipmapMode.NEAREST,
        addressModeU = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeV = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeW = VkSamplerAddressMode.CLAMP_TO_EDGE, maxLod = 1.0f)
    var inscope sampler <- create_sampler(device, sci)

    // Descriptor + pipeline layouts come from the shaders' own dasSpirv reflection (the single source
    // of truth), not hand-declared: UBO @set0/@binding0, sampler @set0/@binding1, fragment push range.
    var reflections <- [decode_reflection(quad_vert_spv_reflect), decode_reflection(quad_frag_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u
    var ps0 = DescriptorPoolSize(type_ = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)
    dpci.pPoolSizes |> push(ps0)
    var ps1 = DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    dpci.pPoolSizes |> push(ps1)
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(desc_pool)
    dsai.descriptorSetCount = 1u
    var raw_set_layout = boost_value_to_vk(set_layouts[0])
    var raw_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_set_layout)
        vk_check(vkAllocateDescriptorSets(dev, dsai, addr(raw_set)), null)
    }

    var writes : array<WriteDescriptorSet>
    var w0 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 0u,
        descriptorType = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)
    var binfo = DescriptorBufferInfo(buffer = weak_copy(ubo.buffer), range_ = 64ul)
    w0.pBufferInfo |> push(binfo)
    writes |> emplace(w0)
    var w1 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 1u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    var iinfo = DescriptorImageInfo(sampler = weak_copy(sampler), imageView = weak_copy(tex_view),
        imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
    w1.pImageInfo |> push(iinfo)
    writes |> emplace(w1)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    // pipeline layout (descriptor set + fragment push-constant range) is built from the same reflection.
    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    // The push-constant size is reflected too — Tint{float3} is 12 bytes (std140 vec3), so the command
    // below pushes exactly what the layout declares rather than the over-sized 16 the hand-build used.
    var push_size = 0u
    var have_push = false
    for (refl in reflections) {
        for (p in refl.push_constants) {
            // this gate has exactly one push range (fragment Tint at offset 0); assert that shape rather
            // than silently pushing 0 if reflection ever stops reporting it.
            assert(p.offset == 0, "reflected push-constant range must start at offset 0 for this gate")
            push_size = uint(p.size)
            have_push = true
        }
    }
    assert(have_push, "expected a reflected push-constant range (fragment Tint)")
    delete reflections

    var inscope vert <- create_shader_module(device, quad_vert_spv)
    var inscope frag <- create_shader_module(device, quad_frag_spv)
    var inscope pipeline <- create_graphics_pipeline_simple(device, render_pass, pipe_layout, vert, frag, TRI_W, TRI_H)

    let buf_size = uint64(TRI_W * TRI_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)
    var inscope pool <- create_command_pool(device, pool_info(gfx))

    var tint = fixed_array(1.0f, 1.0f, 0.0f, 0.0f)   // zero blue -> white texel renders yellow

    run_cmd_sync(device, pool, queue) $(cmd) {
        let raw_cmd = boost_value_to_vk(cmd)
        var to_dst : VkAccessFlags
        to_dst.transfer_write = true
        let none : VkAccessFlags
        var top : VkPipelineStageFlags
        top.top_of_pipe = true
        var xfer : VkPipelineStageFlags
        xfer.transfer = true
        var frag_stage : VkPipelineStageFlags
        frag_stage.fragment_shader = true
        var to_read : VkAccessFlags
        to_read.shader_read = true
        transition_image(cmd, tex_image, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
            none, to_dst, top, xfer)
        var region : BufferImageCopy
        region.imageSubresource.aspectMask.color = true
        region.imageSubresource.layerCount = 1u
        region.imageExtent.width = 2u
        region.imageExtent.height = 2u
        region.imageExtent.depth = 1u
        var regions : array<BufferImageCopy>
        regions |> emplace(region)
        cmd_copy_buffer_to_image(cmd, staging.buffer, tex_image, VkImageLayout.TRANSFER_DST_OPTIMAL, regions)
        transition_image(cmd, tex_image, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.SHADER_READ_ONLY_OPTIMAL,
            to_dst, to_read, xfer, frag_stage)
        record_render_pass(cmd, render_pass, framebuffer, full_area(TRI_W, TRI_H), clear_color(0.1f, 0.1f, 0.15f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipeline)
            var sets : array<DescriptorSet>
            sets |> push(vk_value_to_boost(raw_set))
            var no_dyn : array<uint>
            cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, pipe_layout, 0u, sets, no_dyn)
            var pcf : VkShaderStageFlags
            pcf.fragment = true
            unsafe {
                vkCmdPushConstants(raw_cmd, boost_value_to_vk(pipe_layout), pcf, 0u, push_size, addr<void?>(tint[0]))
            }
            cmd_draw(cmd, 6u)
        }
        copy_image_to_buffer(cmd, target.image, readback, TRI_W, TRI_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}
