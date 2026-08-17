options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 03 render path: build the resident Vulkan resources for the SDF compute shader once, then
// drive a per-frame redispatch with a single float push constant (wall-clock time). The shader does
// all the rendering -- camera, raymarch, lighting, shadows, fog -- and the host just shuttles bytes
// back. The CI gate uses render_sdf at a fixed time, the recording driver advances time per frame.

module sdf_tut

require vulkan public
require vulkan/vulkan_boost public
require sdf_tut_shaders public

//! Resident compute resources for the SDF tutorial: an Rgba8 storage image + the descriptor and
//! pipeline that fill it. A single float push constant (animation time) re-drives the dispatch each
//! frame. Owns every handle it creates; render_sdf_frame re-runs the dispatch without rebuilding.
struct public SdfRenderer {
    instance    : Instance
    device      : Device
    queue       : VkQueue                //!< raw handle, owned by device
    pool        : CommandPool
    image       : Image
    memory      : DeviceMemory
    view        : ImageView
    srgb_image  : Image                  //!< R8G8B8A8_SRGB intermediate: linear storage image is blitted here so the readback gets gamma-encoded bytes
    srgb_memory : DeviceMemory
    set_layout  : DescriptorSetLayout
    desc_pool   : DescriptorPool
    pipe_layout : PipelineLayout
    pipeline    : Pipeline
    set         : VkDescriptorSet        //!< raw handle, owned by desc_pool (freed with it)
    readback    : HostBuffer
    buf_size    : uint64
}

def public finalize(var r : SdfRenderer) {
    unsafe {
        delete r.readback
        delete r.pipeline
        delete r.pipe_layout
        delete r.desc_pool       // frees the descriptor set allocated from it
        delete r.set_layout
        delete r.srgb_image
        delete r.srgb_memory
        delete r.view
        delete r.image
        delete r.memory
        delete r.pool
        delete r.device
        delete r.instance
    }
}

//! Initialize Vulkan, build the storage image + compute pipeline for sdf_spv, and allocate a host
//! readback buffer. Long-lived across frames -- call render_sdf_frame to redispatch with a new time.
def public build_sdf_renderer() : SdfRenderer {   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan tutorial 03", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx  = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)

    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    let dev = boost_value_to_vk(device)
    let fmt = VkFormat.R8G8B8A8_UNORM

    // storage image: STORAGE (compute writes) + TRANSFER_SRC (the readback path)
    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = fmt, mipLevels = 1u,
        arrayLayers = 1u, tiling = VkImageTiling.OPTIMAL, initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = uint(SDF_DIM)
    ici.extent.height = uint(SDF_DIM)
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.storage = true
    ici.usage.transfer_src = true
    var inscope image <- create_image(device, ici)
    var ireq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(image), ireq)
    var iwant : VkMemoryPropertyFlags
    iwant.device_local = true
    let imai = MemoryAllocateInfo(allocationSize = ireq.size,
        memoryTypeIndex = find_memory_type(phys, ireq.memoryTypeBits, iwant))
    var inscope imem <- allocate_memory(device, imai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(image), boost_value_to_vk(imem), 0ul), null)
    var vci = ImageViewCreateInfo(image = weak_copy(image), viewType = VkImageViewType._2D, format = fmt)
    vci.subresourceRange.aspectMask.color = true
    vci.subresourceRange.levelCount = 1u
    vci.subresourceRange.layerCount = 1u
    var inscope view <- create_image_view(device, vci)

    // sRGB readback intermediate: the compute shader writes LINEAR colors into the UNORM storage image;
    // we blit that into this R8G8B8A8_SRGB image so the hardware applies the linear->sRGB encode at the
    // blit (TRANSFER_DST + TRANSFER_SRC only, NO storage usage -- storage images don't sRGB-encode on write).
    let srgb_fmt = VkFormat.R8G8B8A8_SRGB
    var sici = ImageCreateInfo(imageType = VkImageType._2D, format = srgb_fmt, mipLevels = 1u,
        arrayLayers = 1u, tiling = VkImageTiling.OPTIMAL, initialLayout = VkImageLayout.UNDEFINED)
    sici.extent.width = uint(SDF_DIM)
    sici.extent.height = uint(SDF_DIM)
    sici.extent.depth = 1u
    sici.samples._1 = true
    sici.usage.transfer_dst = true
    sici.usage.transfer_src = true
    var inscope srgb_image <- create_image(device, sici)
    var sreq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(srgb_image), sreq)
    var swant : VkMemoryPropertyFlags
    swant.device_local = true
    let smai = MemoryAllocateInfo(allocationSize = sreq.size,
        memoryTypeIndex = find_memory_type(phys, sreq.memoryTypeBits, swant))
    var inscope smem <- allocate_memory(device, smai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(srgb_image), boost_value_to_vk(smem), 0ul), null)
    // no image view: this image is only a blit dst + readback src, never sampled / bound / attached
    // (an ImageView would require SAMPLED/STORAGE/attachment usage, which it deliberately lacks).

    // One-time post-create layout transition UNDEFINED -> TRANSFER_SRC_OPTIMAL so render_sdf_frame
    // can assume a stable starting layout across all frames (first frame and every subsequent one
    // transition TRANSFER_SRC_OPTIMAL -> GENERAL -> TRANSFER_SRC_OPTIMAL, never UNDEFINED).
    run_cmd_sync(device, pool, queue) $(cmd) {
        let none            : VkAccessFlags
        var transfer_read   : VkAccessFlags; transfer_read.transfer_read = true
        var top             : VkPipelineStageFlags; top.top_of_pipe       = true
        var xfer            : VkPipelineStageFlags; xfer.transfer         = true
        transition_image(cmd, image, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_SRC_OPTIMAL,
            none, transfer_read, top, xfer)
    }

    // descriptor set: binding 0 = STORAGE_IMAGE, compute stage
    var dslci : DescriptorSetLayoutCreateInfo
    var b0 = DescriptorSetLayoutBinding(binding = 0u, descriptorType = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 1u)
    b0.stageFlags.compute = true
    dslci.pBindings |> emplace(b0)
    var inscope set_layout <- create_descriptor_set_layout(device, dslci)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u
    var psize = DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 1u)
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

    var write = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 0u,
        descriptorType = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 1u)
    var iinfo = DescriptorImageInfo(imageView = weak_copy(view), imageLayout = VkImageLayout.GENERAL)
    write.pImageInfo |> push(iinfo)
    var writes : array<WriteDescriptorSet>
    writes |> emplace(write)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    // compute pipeline + a 4-byte (one float) compute push constant for time
    var inscope shader <- create_shader_module(device, sdf_spv)
    var plci : PipelineLayoutCreateInfo
    plci.pSetLayouts <- [weak_copy(set_layout)]
    var pcr : PushConstantRange
    pcr.stageFlags.compute = true
    pcr.offset = 0u
    pcr.size = 4u
    plci.pPushConstantRanges |> emplace(pcr)
    var inscope pipe_layout <- create_pipeline_layout(device, plci)
    delete plci.pPushConstantRanges
    unsafe { delete plci.pSetLayouts }   // the [weak_copy(set_layout)] literal we built above (handles are non-owning)
    var inscope pipeline <- create_compute_pipeline(device, pipe_layout, shader)

    let buf_size = uint64(SDF_DIM * SDF_DIM * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    return <- SdfRenderer(
        instance <- instance, device <- device, queue = queue, pool <- pool,
        image <- image, memory <- imem, view <- view,
        srgb_image <- srgb_image, srgb_memory <- smem,
        set_layout <- set_layout, desc_pool <- desc_pool,
        pipe_layout <- pipe_layout, pipeline <- pipeline, set = raw_set,
        readback <- readback, buf_size = buf_size)
}

//! Dispatch one frame at `time`, read back RGBA8 pixels. Deterministic: same time -> same pixels.
def public render_sdf_frame(var r : SdfRenderer; time : float) : array<uint8> {
    var pixels : array<uint8>
    run_cmd_sync(r.device, r.pool, r.queue) $(cmd) {
        let none          : VkAccessFlags
        var shader_write  : VkAccessFlags; shader_write.shader_write  = true
        var transfer_read : VkAccessFlags; transfer_read.transfer_read = true
        var comp          : VkPipelineStageFlags; comp.compute_shader   = true
        var xfer          : VkPipelineStageFlags; xfer.transfer         = true
        // image enters this command buffer in TRANSFER_SRC_OPTIMAL (set up at create-time on the
        // first frame, left there by the previous frame's final transition on every subsequent one)
        transition_image(cmd, r.image, VkImageLayout.TRANSFER_SRC_OPTIMAL, VkImageLayout.GENERAL,
            transfer_read, shader_write, xfer, comp)

        cmd_bind_pipeline(cmd, r.pipeline, VkPipelineBindPoint.COMPUTE)
        let sets <- [vk_value_to_boost(r.set)]
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.COMPUTE, r.pipe_layout, 0u, sets, no_dyn)
        pc.time = time
        sdf_main_push_constants(cmd, r.pipe_layout)
        cmd_dispatch(cmd, uint(SDF_DIM / 8), uint(SDF_DIM / 8), 1u)

        transition_image(cmd, r.image, VkImageLayout.GENERAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
            shader_write, transfer_read, comp, xfer)

        // r.image (linear UNORM) is now TRANSFER_SRC. Blit it into the R8G8B8A8_SRGB intermediate so the
        // hardware applies the linear->sRGB encode at the format conversion; the recording then reads
        // gamma-encoded bytes. Bring srgb_image UNDEFINED -> TRANSFER_DST first (we overwrite it whole).
        var transfer_write : VkAccessFlags; transfer_write.transfer_write = true
        var top            : VkPipelineStageFlags; top.top_of_pipe = true
        transition_image(cmd, r.srgb_image, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
            none, transfer_write, top, xfer)
        var region : VkImageBlit
        region.srcSubresource.aspectMask.color = true
        region.srcSubresource.layerCount = 1u
        region.srcOffsets[1].x = SDF_DIM
        region.srcOffsets[1].y = SDF_DIM
        region.srcOffsets[1].z = 1
        region.dstSubresource.aspectMask.color = true
        region.dstSubresource.layerCount = 1u
        region.dstOffsets[1].x = SDF_DIM
        region.dstOffsets[1].y = SDF_DIM
        region.dstOffsets[1].z = 1
        unsafe {
            vkCmdBlitImage(boost_value_to_vk(cmd),
                boost_value_to_vk(r.image),      VkImageLayout.TRANSFER_SRC_OPTIMAL,
                boost_value_to_vk(r.srgb_image), VkImageLayout.TRANSFER_DST_OPTIMAL,
                1u, addr(region), VkFilter.NEAREST)
        }
        // srgb_image -> TRANSFER_SRC so copy_image_to_buffer can read it
        transition_image(cmd, r.srgb_image, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
            transfer_write, transfer_read, xfer, xfer)
        copy_image_to_buffer(cmd, r.srgb_image, r.readback, SDF_DIM, SDF_DIM)
    }
    map_memory_to_array(r.device, r.readback.memory, r.buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! One-shot render at `time`. Builds Vulkan resources, dispatches once, tears down. Convenient for
//! the CI oracle; the recording driver builds an SdfRenderer once and calls render_sdf_frame N times.
def public render_sdf(time : float) : array<uint8> {
    var inscope r <- build_sdf_renderer()
    return <- render_sdf_frame(r, time)
}

//! Sample the RGB of pixel (x,y) from an SDF_DIM*SDF_DIM RGBA8 buffer.
def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * SDF_DIM + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}
