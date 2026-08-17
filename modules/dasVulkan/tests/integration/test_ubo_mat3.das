options gen2
options _comment_hygiene = true
options indenting = 4

// GPU regression gate for the SHARED std140 walk (daslang's daslib/shader_block_layout): a UBO with a
// float3x3, a float4[2] array, and a trailing vec4, uploaded through the GENERATED <shader>_bind_uniform
// (the write_field path) -- NOT a hand-laid raw byte image (that is test_ubo_nested). This is the first
// test that exercises the generated bind with composite / matrix fields: a flat write_field per
// top-level field mis-packed float3x3 (36 packed bytes into the 48-byte std140 slot) and arrays / nested
// structs (wrong stride). The shared walk decomposes float3x3 into per-column writes (column c at
// offset+c*16) and arrays per element, so the bytes line up with the std140 layout the SPIR-V shader
// reads. The fragment reads ONE value via each path -- rot column 2 (mat3), arr[1] (array element),
// tint (the vec4 after the composites) -- into a different channel; decoys sit elsewhere so a wrong
// offset/column/stride yields a clearly different pixel.

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
require daslib/math_boost

// std140 layout: rot @ 0 (3 cols * 16 = 48), arr @ 48 (float4 stride 16, 32 bytes), tint @ 80 (16).
struct Mat3UBO {
    rot  : float3x3     // offset 0, size 48
    arr  : float4[2]    // offset 48, ArrayStride 16
    tint : float4       // offset 80
}                       // total 96

var @uniform @set = 0 @binding = 0 m3 : Mat3UBO
var @out @location = 0 m3_color : float4

[vulkan_vertex_shader(name="ubo_mat3_vert_spv"), marker(no_coverage)]
def ubo_mat3_vert {
    let i = gl_VertexIndex
    var xy = float2(0.0f, 0.0f)
    if (i == 0) { xy = float2(-1.0f, -1.0f) }
    if (i == 1) { xy = float2(3.0f, -1.0f) }
    if (i == 2) { xy = float2(-1.0f, 3.0f) }
    gl_Position = float4(xy, 0.0f, 1.0f)
}

[vulkan_fragment_shader(name="ubo_mat3_frag_spv"), marker(no_coverage)]
def ubo_mat3_frag {
    let c2 = m3.rot * float3(0.0f, 0.0f, 1.0f)       // mat3 * basis = column 2 (per-column repack path)
    m3_color = float4(m3.arr[1].x, m3.tint.y, c2.z, 1.0f)
}

let TEX_W = 256
let TEX_H = 256

def private render_mat3() : array<uint8> {   // nolint:STYLE038 - flat command-record run
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan test ubo_mat3", make_api_version(1u, 2u, 0u))
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

    // Host-visible UBO uploaded std140-packed via the GENERATED bind (the path under test). Decoys = 0.9; read values arr[1].x=0.2, tint.y=0.4, rot column2.z=0.6 -> expected center (51,102,153).
    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true
    var inscope ubo <- create_host_buffer(device, phys, 96ul, ubo_usage)
    m3.rot[0] = float3(0.9f, 0.9f, 0.9f)
    m3.rot[1] = float3(0.9f, 0.9f, 0.9f)
    m3.rot[2] = float3(0.0f, 0.0f, 0.6f)
    m3.arr[0] = float4(0.9f, 0.9f, 0.9f, 0.9f)
    m3.arr[1] = float4(0.2f, 0.9f, 0.9f, 0.9f)
    m3.tint = float4(0.9f, 0.4f, 0.9f, 0.9f)
    ubo_mat3_frag_bind_uniform(device, ubo.memory)

    var reflections <- [decode_reflection(ubo_mat3_vert_spv_reflect), decode_reflection(ubo_mat3_frag_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u
    var ps = DescriptorPoolSize(type_ = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)
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
        descriptorType = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)
    var binfo = DescriptorBufferInfo(buffer = weak_copy(ubo.buffer), range_ = 96ul)
    w.pBufferInfo |> push(binfo)
    writes |> emplace(w)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    delete reflections

    var inscope vert <- create_shader_module(device, ubo_mat3_vert_spv)
    var inscope frag <- create_shader_module(device, ubo_mat3_frag_spv)
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
def test_ubo_mat3_shared_walk(t : T?) {
    var pixels <- render_mat3()
    let c = px(pixels, 128, 128)
    // arr[1].x=0.2 -> R~51, tint.y=0.4 -> G~102, rot col2.z=0.6 -> B~153. A wrong column / stride /
    // offset would read a 0.9 decoy (~230) or 0 in some channel.
    t |> success(abs(c.x - 51) <= 8 && abs(c.y - 102) <= 8 && abs(c.z - 153) <= 8,
        "shared std140 walk: mat3 column + array element + vec4 read at correct offsets (got R={c.x} G={c.y} B={c.z})")
    delete pixels
}
