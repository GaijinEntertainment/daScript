options gen2
options indenting = 4

// GPU gate for the multi-UBO contract: a shader reaching TWO @uniform blocks compiles, and the
// generated <shader>_bind_uniform writes the FIRST declared block into the allocation it is handed.
// The second block is the host's own upload (one bind function maps ONE allocation), so the fragment
// reads one channel through each path: R + B from the generated bind, G from the host write. Decoys
// sit in the other lanes, so binding the wrong block -- or writing the wrong offsets -- lands a decoy
// in a channel and the pixel check fails.

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require vulkan/vulkan_commands public
require vulkan/spirv_vulkan_shader public
require spirv/spirv_shader public
require spirv/spirv_builtins public
require spirv/spirv_reflect public
require scene_helpers
require math

struct FrameUBO {
    tint : float4       // offset 0, size 16
}

struct LookupUBO {
    tint : float4       // offset 0, size 16
}

var @uniform @set = 0 @binding = 0 mu_frame : FrameUBO
var @uniform @set = 0 @binding = 1 mu_lookup : LookupUBO
var @out @location = 0 mu_color : float4

[vulkan_vertex_shader(name="ubo_multi_vert_spv"), marker(no_coverage)]
def ubo_multi_vert {
    let i = gl_VertexIndex
    var xy = float2(0.0f, 0.0f)
    if (i == 0) { xy = float2(-1.0f, -1.0f) }
    if (i == 1) { xy = float2(3.0f, -1.0f) }
    if (i == 2) { xy = float2(-1.0f, 3.0f) }
    gl_Position = float4(xy, 0.0f, 1.0f)
}

[vulkan_fragment_shader(name="ubo_multi_frag_spv"), marker(no_coverage)]
def ubo_multi_frag {
    mu_color = float4(mu_frame.tint.x, mu_lookup.tint.y, mu_frame.tint.z, 1.0f)
}

let TEX_W = 256
let TEX_H = 256
let UBO_SIZE = 16ul

def private render_multi() : array<uint8> {   // nolint:STYLE038 - flat command-record run
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan test ubo_multi", make_api_version(1u, 2u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
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

    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true
    var inscope ubo_frame <- create_host_buffer(device, phys, UBO_SIZE, ubo_usage)
    var inscope ubo_lookup <- create_host_buffer(device, phys, UBO_SIZE, ubo_usage)

    // Block 0 through the GENERATED bind: reads land in R (0.2 -> 51) and B (0.6 -> 153). Binding the
    // second block instead would write mu_lookup's 0.9 decoy into R.
    mu_frame.tint = float4(0.2f, 0.9f, 0.6f, 1.0f)
    ubo_multi_frag_bind_uniform(device, ubo_frame.memory)
    // Block 1 the way the host must upload every further block: its own map + write_field. G = 0.4 -> 102.
    mu_lookup.tint = float4(0.9f, 0.4f, 0.9f, 1.0f)
    with_mapped_memory(device, ubo_lookup.memory) $(ptr : void?) {
        write_field(ptr, 0u, mu_lookup.tint)
    }

    var reflections <- [decode_reflection(ubo_multi_vert_spv_reflect), decode_reflection(ubo_multi_frag_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u
    var ps = DescriptorPoolSize(type_ = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 2u)
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
    var w0 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 0u,
        descriptorType = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)
    var binfo0 = DescriptorBufferInfo(buffer = weak_copy(ubo_frame.buffer), range_ = UBO_SIZE)
    w0.pBufferInfo |> push(binfo0)
    writes |> emplace(w0)
    var w1 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 1u,
        descriptorType = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)
    var binfo1 = DescriptorBufferInfo(buffer = weak_copy(ubo_lookup.buffer), range_ = UBO_SIZE)
    w1.pBufferInfo |> push(binfo1)
    writes |> emplace(w1)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    delete reflections

    var inscope vert <- create_shader_module(device, ubo_multi_vert_spv)
    var inscope frag <- create_shader_module(device, ubo_multi_frag_spv)
    var inscope pipeline <- create_graphics_pipeline_simple(device, render_pass, pipe_layout, vert, frag, TEX_W, TEX_H)

    let buf_size = uint64(TEX_W * TEX_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    run_cmd_sync(device, pool, queue) $(cmd) {
        record_render_pass(cmd, render_pass, framebuffer, full_area(TEX_W, TEX_H), clear_color(0.0f, 0.0f, 0.0f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipeline)
            var sets : array<DescriptorSet>
            sets |> push(vk_value_to_boost(raw_set))
            var no_dyn : array<uint>
            cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, pipe_layout, 0u, sets, no_dyn)
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
def test_ubo_multi_first_block(t : T?) {
    var pixels <- render_multi()
    let c = px(pixels, 128, 128)
    // R = mu_frame.tint.x = 0.2 (51) and B = mu_frame.tint.z = 0.6 (153) prove the generated bind wrote
    // the FIRST block; G = mu_lookup.tint.y = 0.4 (102) proves the host's own upload of the second one.
    t |> success(abs(c.x - 51) <= 8 && abs(c.y - 102) <= 8 && abs(c.z - 153) <= 8,
        "two @uniform blocks: generated bind wrote the first, host wrote the second (got R={c.x} G={c.y} B={c.z})")
    delete pixels
}
