options gen2
options _comment_hygiene = true
options indenting = 4

// PR 8 (dasSpirv PR-B GPU regression gate): the nonuniformEXT cascade. Identical descriptor-array
// shape to test_bindless_descriptor_array, but the fragment indexes the sampler with
// `nonuniformEXT(pc.id)`. PR-B emits OpCopyObject + OpDecorate NonUniform on the marker, plus the
// NonUniform cascade onto the OpAccessChain pointer + the OpLoad'd sampler, and pulls in
// ShaderNonUniform + SampledImageArrayNonUniformIndexing capabilities. The push-constant index is
// in fact uniform across invocations here -- the driver doesn't need to scalarize -- but the
// emitted blob carries the cap/decoration, and the device's
// shaderSampledImageArrayNonUniformIndexing feature must be enabled for the driver to accept it.
// A clean render proves both the feature-enabled device path and that the NonUniform-decorated
// chain runs end to end.

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require vulkan/vulkan_commands public
require spirv/spirv_shader public
require spirv/spirv_builtins public
require spirv/spirv_reflect public
require scene_helpers

// ===== Shader fixture (mirrors test_bindless_descriptor_array; only the index gets nonuniformEXT) =====
struct NuPush {
    id : int
}
var @binding = 0 nu_samplers : sampler2D[4]
var @push_constant nupc : NuPush
// Vertex outputs the UV at @location 0; the fragment reads it via a separate @in at the same
// location (two-variable cross-stage varying pattern -- see phase4_shaders.das).
var @out @location = 0 v_nu_uv : float2

[vertex_shader(name="nu_arr_vert_spv"), marker(no_coverage)]
def nu_arr_vert {
    let i = gl_VertexIndex
    var xy = float2(0.0f, 0.0f)
    if (i == 0) { xy = float2(-1.0f, -1.0f) }
    if (i == 1) { xy = float2( 3.0f, -1.0f) }
    if (i == 2) { xy = float2(-1.0f,  3.0f) }
    v_nu_uv = xy * 0.5f + float2(0.5f, 0.5f)
    gl_Position = float4(xy, 0.0f, 1.0f)
}

var @in @location = 0 fi_nu_uv : float2
var @out @location = 0 nu_color : float4

[fragment_shader(name="nu_arr_frag_spv"), marker(no_coverage)]
def nu_arr_frag {
    nu_color = texture(nu_samplers[nonuniformEXT(nupc.id)], fi_nu_uv)
}

// ===== Host render =====
let TEX_W = 256
let TEX_H = 256
let N_DESC = 4

def private texel_color(idx : int) : array<uint8> {
    var b : array<uint8>
    b |> resize(4)
    if (idx == 0) { b[0] = 255u8; b[1] = 0u8;   b[2] = 0u8;   b[3] = 255u8 }
    if (idx == 1) { b[0] = 0u8;   b[1] = 255u8; b[2] = 0u8;   b[3] = 255u8 }
    if (idx == 2) { b[0] = 0u8;   b[1] = 0u8;   b[2] = 255u8; b[3] = 255u8 }
    if (idx == 3) { b[0] = 255u8; b[1] = 255u8; b[2] = 0u8;   b[3] = 255u8 }
    return <- b
}

struct private Tex {
    image  : Image
    memory : DeviceMemory
    view   : ImageView
}

def private finalize(var t : Tex) {
    // Teardown order: views (they reference the image), then image, then its memory -- reversing image/memory trips "memory still in use by image".
    delete t.view
    delete t.image
    delete t.memory
}

def private create_1x1_texture(device : Device; phys : VkPhysicalDevice; queue : VkQueue;
                                 cmd_pool : CommandPool; fmt : VkFormat; var texel : array<uint8>) : Tex {
    let dev = boost_value_to_vk(device)
    var staging_usage : VkBufferUsageFlags
    staging_usage.transfer_src = true
    var inscope staging <- create_host_buffer(device, phys, 4ul, staging_usage)
    upload_bytes(staging, device, texel)

    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = fmt,
        mipLevels = 1u, arrayLayers = 1u, tiling = VkImageTiling.OPTIMAL,
        initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = 1u
    ici.extent.height = 1u
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.sampled = true
    ici.usage.transfer_dst = true
    var img <- create_image(device, ici)
    var ireq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(img), ireq)
    var iwant : VkMemoryPropertyFlags
    iwant.device_local = true
    let imai = MemoryAllocateInfo(allocationSize = ireq.size,
        memoryTypeIndex = find_memory_type(phys, ireq.memoryTypeBits, iwant))
    var mem <- allocate_memory(device, imai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(img), boost_value_to_vk(mem), 0ul), null)

    run_cmd_sync(device, cmd_pool, queue) $(cmd) {
        let none : VkAccessFlags
        var to_dst : VkAccessFlags
        to_dst.transfer_write = true
        var to_read : VkAccessFlags
        to_read.shader_read = true
        var top : VkPipelineStageFlags
        top.top_of_pipe = true
        var xfer : VkPipelineStageFlags
        xfer.transfer = true
        var frag : VkPipelineStageFlags
        frag.fragment_shader = true
        transition_image(cmd, img, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
            none, to_dst, top, xfer)
        var region : BufferImageCopy
        region.imageSubresource.aspectMask.color = true
        region.imageSubresource.layerCount = 1u
        region.imageExtent.width = 1u
        region.imageExtent.height = 1u
        region.imageExtent.depth = 1u
        var regions : array<BufferImageCopy>
        regions |> emplace(region)
        cmd_copy_buffer_to_image(cmd, staging.buffer, img, VkImageLayout.TRANSFER_DST_OPTIMAL, regions)
        transition_image(cmd, img, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.SHADER_READ_ONLY_OPTIMAL,
            to_dst, to_read, xfer, frag)
    }

    var tvci = ImageViewCreateInfo(image = weak_copy(img), viewType = VkImageViewType._2D, format = fmt)
    tvci.subresourceRange.aspectMask.color = true
    tvci.subresourceRange.levelCount = 1u
    tvci.subresourceRange.layerCount = 1u
    var view <- create_image_view(device, tvci)

    return <- Tex(image <- img, memory <- mem, view <- view)
}

def private render_one_id(id : int) : array<uint8> {   // nolint:STYLE038 - flat command-record run
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan test nonuniform_ext", make_api_version(1u, 2u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device_descriptor_indexing(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, gfx, 0u)
    let fmt = VkFormat.R8G8B8A8_UNORM

    var inscope target <- build_offscreen_target(device, phys, TEX_W, TEX_H, fmt)
    var inscope render_pass <- create_render_pass_single_color(device, fmt)
    var fbci = FramebufferCreateInfo(renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(target.view)],
        width = uint(TEX_W), height = uint(TEX_H), layers = 1u)
    var inscope framebuffer <- create_framebuffer(device, fbci)

    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope cmd_pool <- create_command_pool(device, poolci)

    var inscope textures : array<Tex>
    textures |> reserve(N_DESC)
    for (k in range(N_DESC)) {
        var texel <- texel_color(k)
        textures |> emplace(create_1x1_texture(device, phys, queue, cmd_pool, fmt, texel))
        delete texel
    }

    let sci = SamplerCreateInfo(magFilter = VkFilter.NEAREST, minFilter = VkFilter.NEAREST,
        mipmapMode = VkSamplerMipmapMode.NEAREST,
        addressModeU = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeV = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeW = VkSamplerAddressMode.CLAMP_TO_EDGE, maxLod = 1.0f)
    var inscope sampler <- create_sampler(device, sci)

    var reflections <- [decode_reflection(nu_arr_vert_spv_reflect), decode_reflection(nu_arr_frag_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u
    var ps = DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER,
        descriptorCount = uint(N_DESC))
    dpci.pPoolSizes |> push(ps)
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
    var w = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 0u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = uint(N_DESC))
    w.pImageInfo |> reserve(N_DESC)
    for (k in range(N_DESC)) {
        var iinfo = DescriptorImageInfo(sampler = weak_copy(sampler),
            imageView = weak_copy(textures[k].view),
            imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
        w.pImageInfo |> push(iinfo)
    }
    writes |> emplace(w)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    delete reflections

    var inscope vert <- create_shader_module(device, nu_arr_vert_spv)
    var inscope frag <- create_shader_module(device, nu_arr_frag_spv)
    var inscope pipeline <- create_graphics_pipeline_simple(device, render_pass, pipe_layout, vert, frag, TEX_W, TEX_H)

    let buf_size = uint64(TEX_W * TEX_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    var pushed = fixed_array(id)
    run_cmd_sync(device, cmd_pool, queue) $(cmd) {
        let raw_cmd = boost_value_to_vk(cmd)
        record_render_pass(cmd, render_pass, framebuffer, full_area(TEX_W, TEX_H), clear_color(0.0f, 0.0f, 0.0f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipeline)
            var sets : array<DescriptorSet>
            sets |> push(vk_value_to_boost(raw_set))
            var no_dyn : array<uint>
            cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, pipe_layout, 0u, sets, no_dyn)
            var pcf : VkShaderStageFlags
            pcf.fragment = true
            unsafe {
                vkCmdPushConstants(raw_cmd, boost_value_to_vk(pipe_layout), pcf, 0u, 4u, addr<void?>(pushed[0]))
            }
            cmd_draw(cmd, 3u)
        }
        copy_image_to_buffer(cmd, target.image, readback, TEX_W, TEX_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

[test]
def test_nonuniform_ext_red(t : T?) {
    var pixels <- render_one_id(0)
    let c = px(pixels, 128, 128)
    t |> success(c.x > 200 && c.y < 60 && c.z < 60,
        "nonuniformEXT(0) -> red (got R={c.x} G={c.y} B={c.z})")
    delete pixels
}

[test]
def test_nonuniform_ext_blue(t : T?) {
    var pixels <- render_one_id(2)
    let c = px(pixels, 128, 128)
    t |> success(c.z > 200 && c.x < 60 && c.y < 60,
        "nonuniformEXT(2) -> blue (got R={c.x} G={c.y} B={c.z})")
    delete pixels
}
