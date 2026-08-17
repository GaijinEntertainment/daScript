options gen2
options _comment_hygiene = true
options indenting = 4

// Shared compute resources for both animated Mandelbrot viewers. Unlike vulkan_boost's one-shot
// compute_to_storage_image (create -> dispatch -> read back -> tear down), this keeps the storage image,
// descriptor set and compute pipeline RESIDENT so the window can re-dispatch every frame with a new
// `time` push constant -- that per-frame recompute is what makes the zoom reveal new fractal detail
// rather than just scaling a static texture. Kept tutorial-local; promote to the library if a later
// compute tutorial needs it too.

module mandelbrot_compute

require vulkan public
require vulkan/vulkan_boost public
require mandelbrot_zoom_shaders public  // pc + mandelbrot_zoom_push_constants come from here

//! resident compute resources: an Rgba8 storage image + the descriptor/pipeline that fill it. Driven by
//! a single float push constant (the animation time). All handles owned here; record_compute re-runs it.
struct public MandelCompute {
    image       : Image
    memory      : DeviceMemory
    view        : ImageView
    set_layout  : DescriptorSetLayout
    desc_pool   : DescriptorPool
    pipe_layout : PipelineLayout
    pipeline    : Pipeline
    set         : VkDescriptorSet   //!< raw handle, owned by desc_pool (freed with it)
    width       : int
    height      : int
}

def public finalize(var mc : MandelCompute) {
    delete mc.pipeline
    delete mc.pipe_layout
    delete mc.desc_pool         // frees the descriptor set allocated from it
    delete mc.set_layout
    delete mc.view
    delete mc.image
    delete mc.memory
}

//! Build the resident compute resources for `words` (a dasSpirv compute blob binding a storage image2D at
//! set=0/binding=0 and a `float` push constant at offset 0). Rgba8, STORAGE (compute write) + TRANSFER_SRC
//! (blit) + SAMPLED (texture); starts UNDEFINED -- record_compute fills it. w/h: positive multiples of 8.
def public build_mandel_compute(device : Device; phys : VkPhysicalDevice; var words : array<uint>;   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
                                w, h : int) : MandelCompute {
    assert(w > 0 && h > 0 && (w % 8) == 0 && (h % 8) == 0,
        "build_mandel_compute: w/h must be positive multiples of 8")
    let dev = boost_value_to_vk(device)
    let fmt = VkFormat.R8G8B8A8_UNORM

    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = fmt, mipLevels = 1u,
        arrayLayers = 1u, tiling = VkImageTiling.OPTIMAL, initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = uint(w)
    ici.extent.height = uint(h)
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.storage = true
    ici.usage.transfer_src = true
    ici.usage.sampled = true
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

    // compute pipeline from the blob; the layout declares a 4-byte (one float) compute push constant
    var inscope shader <- create_shader_module(device, words)
    var plci : PipelineLayoutCreateInfo
    plci.pSetLayouts <- [weak_copy(set_layout)]
    var pcr : PushConstantRange
    pcr.stageFlags.compute = true
    pcr.offset = 0u
    pcr.size = 4u
    plci.pPushConstantRanges |> emplace(pcr)
    var inscope pipe_layout <- create_pipeline_layout(device, plci)
    delete plci.pPushConstantRanges   // owned input freed once the layout has copied the range
    unsafe { delete plci.pSetLayouts }   // the [weak_copy(set_layout)] literal we built above (handles non-owning)
    var inscope pipeline <- create_compute_pipeline(device, pipe_layout, shader)

    return <- MandelCompute(image <- image, memory <- imem, view <- view, set_layout <- set_layout,
        desc_pool <- desc_pool, pipe_layout <- pipe_layout, pipeline <- pipeline, set = raw_set,
        width = w, height = h)
}

//! Record a compute dispatch into `cmd`: push `time`, bind the pipeline + descriptor set, and dispatch
//! one invocation per pixel (w/8 x h/8). The caller is responsible for having the image in the GENERAL
//! layout and for the barriers around this call (compute write -> the consumer's read).
def public record_compute(cmd : CommandBuffer; mc : MandelCompute; time : float) {
    cmd_bind_pipeline(cmd, mc.pipeline, VkPipelineBindPoint.COMPUTE)
    var sets : array<DescriptorSet>
    sets |> push(vk_value_to_boost(mc.set))
    var no_dyn : array<uint>
    cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.COMPUTE, mc.pipe_layout, 0u, sets, no_dyn)
    pc.time = time
    mandelbrot_zoom_push_constants(cmd, mc.pipe_layout)
    cmd_dispatch(cmd, uint(mc.width / 8), uint(mc.height / 8), 1u)
}
