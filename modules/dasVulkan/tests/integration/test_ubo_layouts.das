options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require vulkan/vulkan_commands public
require vulkan/spirv_vulkan_shader public
require spirv/spirv_shader public
require spirv/spirv_builtins public
require spirv/spirv_reflect public
require _scene_helpers
require math
require daslib/math_boost
require strings
require _flat_shaders

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

[test(description = "a UBO with a mat3, an array and a vec4 is read at the std140 offsets the shared walk wrote", timeout = 1200)]
def test_ubo_mat3_shared_walk(t : T?) {
    var pixels <- render_mat3()
    let c = px(pixels, 128, 128)
    // arr[1].x=0.2 -> R~51, tint.y=0.4 -> G~102, rot col2.z=0.6 -> B~153. A wrong column / stride /
    // offset would read a 0.9 decoy (~230) or 0 in some channel.
    t |> success(abs(c.x - 51) <= 8 && abs(c.y - 102) <= 8 && abs(c.z - 153) <= 8,
        "shared std140 walk: mat3 column + array element + vec4 read at correct offsets (got R={c.x} G={c.y} B={c.z})")
    delete pixels
}

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

    // block 0 through the GENERATED bind: R and B
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

[test(description = "with two uniform blocks the generated bind writes the first block while the host upload serves the second", timeout = 1200)]
def test_ubo_multi_first_block(t : T?) {
    var pixels <- render_multi()
    let c = px(pixels, 128, 128)
    // R = mu_frame.tint.x = 0.2 (51) and B = mu_frame.tint.z = 0.6 (153) prove the generated bind wrote
    // the FIRST block; G = mu_lookup.tint.y = 0.4 (102) proves the host's own upload of the second one.
    t |> success(abs(c.x - 51) <= 8 && abs(c.y - 102) <= 8 && abs(c.z - 153) <= 8,
        "two @uniform blocks: generated bind wrote the first, host wrote the second (got R={c.x} G={c.y} B={c.z})")
    delete pixels
}

struct NMaterial {
    base_color : float4     // inner offset 0
    roughness  : float      // inner offset 16
    metallic   : float      // inner offset 20
}                           // size 24, aligned to 32 (std140 struct rule)

struct NLight {
    pos   : float4          // inner offset 0
    color : float4          // inner offset 16
}                           // size 32

struct NestedUBO {
    palette  : float4[3]    // offset 0,  ArrayStride 16, 48 bytes
    material : NMaterial    // offset 48 (16-aligned), 32 bytes
    lights   : NLight[2]    // offset 80, ArrayStride 32, 64 bytes
}                           // total 144 bytes

struct UboPush {
    palette_id : int        // index into nubo.palette (0..2)
}

var @uniform @set = 0 @binding = 0 nubo : NestedUBO
var @push_constant pc : UboPush
var @out @location = 0 nub_color : float4

// Vertex shader: a 3-vertex oversized "fullscreen triangle" (verts at (-1,-1), (3,-1), (-1,3))
// covering the screen with NO vertex buffer -- driven by gl_VertexIndex. The fragment doesn't need
// a UV varying for this fixture; the per-pixel color is fully driven by the UBO + push constant.
[vertex_shader(name="ubo_nested_vert_spv"), marker(no_coverage)]
def ubo_nested_vert {
    let i = gl_VertexIndex
    var xy = float2(0.0f, 0.0f)
    if (i == 0) { xy = float2(-1.0f, -1.0f) }
    if (i == 1) { xy = float2(3.0f, -1.0f) }
    if (i == 2) { xy = float2(-1.0f,  3.0f) }
    gl_Position = float4(xy, 0.0f, 1.0f)
}

// Fragment shader: read one field from each composite shape, combine into a color the host can
// distinguish per palette_id.
[fragment_shader(name="ubo_nested_frag_spv"), marker(no_coverage)]
def ubo_nested_frag {
    let base = nubo.palette[pc.palette_id]                              // float4 from array element
    let tint = nubo.material.base_color * nubo.material.roughness       // nested-struct field reads
    let glow = nubo.lights[1].color                                     // array-of-struct field read
    nub_color = float4(base.rgb + tint.rgb + glow.rgb, 1.0f)
}

// ===== Host render =====

// Build the 144-byte std140 image of NestedUBO at the exact offsets published by
// compute_block_layout. `palette[k] = (k==id ? 1 : 0, ...)` lets the fragment for that id resolve
// to red/green/blue cleanly; material is small and the light contributes ~zero.
def private build_nested_ubo(id : int) : array<uint8> {
    var bytes : array<uint8>
    bytes |> resize(144)
    // Non-selected entries stay dark grey, so a wrong-index pick lands at a clearly different color.
    let dark : float4 = float4(0.05f, 0.05f, 0.05f, 1.0f)
    let red  : float4 = float4(1.0f, 0.2f, 0.2f, 1.0f)
    let green : float4 = float4(0.2f, 1.0f, 0.2f, 1.0f)
    let blue : float4 = float4(0.2f, 0.2f, 1.0f, 1.0f)
    let entries = fixed_array(
        id == 0 ? red : dark,
        id == 1 ? green : dark,
        id == 2 ? blue : dark)
    // palette @ 0, stride 16
    for (k in range(3)) {
        write_float4_at(bytes, k * 16, entries[k])
    }
    // material @ 48: base_color = (0.05, 0.05, 0.05, 1), roughness = 1, metallic = 0
    write_float4_at(bytes, 48, float4(0.05f, 0.05f, 0.05f, 1.0f))
    write_float_at(bytes, 48 + 16, 1.0f)
    write_float_at(bytes, 48 + 20, 0.0f)
    // lights[0] unused/zero; lights[1] @ 80+32=112 has tiny nonzero color, proving the array-of-struct chain reads the SECOND element.
    write_float4_at(bytes, 112, float4(0.0f, 0.0f, 0.0f, 1.0f))
    write_float4_at(bytes, 112 + 16, float4(0.02f, 0.02f, 0.02f, 1.0f))   // glow nudges the output
    return <- bytes
}

def private write_float_at(var bytes : array<uint8>; off : int; v : float) {
    var probe = fixed_array(v)
    unsafe {
        let src = addr<uint8 const?>(probe[0])
        for (b in range(4)) {
            bytes[off + b] = src[b]
        }
    }
}

def private write_float4_at(var bytes : array<uint8>; off : int; v : float4) {
    var probe = fixed_array(v.x, v.y, v.z, v.w)
    unsafe {
        let src = addr<uint8 const?>(probe[0])
        for (b in range(16)) {
            bytes[off + b] = src[b]
        }
    }
}

def private render_one_id(id : int) : array<uint8> {   // nolint:STYLE038 - flat command-record run
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan test ubo_nested", make_api_version(1u, 2u, 0u))
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

    // UBO buffer (144 bytes of std140-laid-out bytes uploaded directly)
    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true
    var inscope ubo <- create_host_buffer(device, phys, 144ul, ubo_usage)
    var ubo_bytes <- build_nested_ubo(id)
    upload_bytes(ubo, device, ubo_bytes)
    delete ubo_bytes

    var reflections <- [decode_reflection(ubo_nested_vert_spv_reflect), decode_reflection(ubo_nested_frag_spv_reflect)]
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
    var binfo = DescriptorBufferInfo(buffer = weak_copy(ubo.buffer), range_ = 144ul)
    w.pBufferInfo |> push(binfo)
    writes |> emplace(w)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    delete reflections

    var inscope vert <- create_shader_module(device, ubo_nested_vert_spv)
    var inscope frag <- create_shader_module(device, ubo_nested_frag_spv)
    var inscope pipeline <- create_graphics_pipeline_simple(device, render_pass, pipe_layout, vert, frag, TEX_W, TEX_H)

    let buf_size = uint64(TEX_W * TEX_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    var pushed = fixed_array(id)
    run_cmd_sync(device, pool, queue) $(cmd) {
        let raw_cmd = boost_value_to_vk(cmd)
        record_render_pass(cmd, render_pass, framebuffer, full_area(TEX_W, TEX_H), clear_color(0.0f, 0.0f, 0.0f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipeline)
            var sets : array<DescriptorSet>
            sets |> push(vk_value_to_boost(raw_set))
            var no_dyn : array<uint>
            cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, pipe_layout, 0u, sets, no_dyn)
            var pcf : VkShaderStageFlags
            pcf.fragment = true
            vkCmdPushConstants(raw_cmd, boost_value_to_vk(pipe_layout), pcf, 0u, 4u, unsafe(addr<void?>(pushed[0])))
            cmd_draw(cmd, 3u)
        }
        copy_image_to_buffer(cmd, target.image, readback, TEX_W, TEX_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

// ===== Pixel oracle =====

// color = palette[id] + material(0.05) + lights[1].color(0.02), so id 0/1/2 give approx
// (1.07,0.27,0.27) / (0.27,1.07,0.27) / (0.27,0.27,1.07) -- red / green / blue-dominant.
// A wrong index picks dark grey instead: a wash with no dominance.
[test(description = "palette index 0 in a nested-struct UBO selects red through the access chains", timeout = 1200)]
def test_ubo_nested_palette_red(t : T?) {
    var pixels <- render_one_id(0)
    let c = px(pixels, 128, 128)
    t |> success(c.x > 200 && c.y < 120 && c.z < 120,
        "palette[0]=red selected -> red-dominant center (got R={c.x} G={c.y} B={c.z})")
    delete pixels
}

[test(description = "palette index 1 in a nested-struct UBO selects green through the access chains", timeout = 1200)]
def test_ubo_nested_palette_green(t : T?) {
    var pixels <- render_one_id(1)
    let c = px(pixels, 128, 128)
    t |> success(c.y > 200 && c.x < 120 && c.z < 120,
        "palette[1]=green selected -> green-dominant center (got R={c.x} G={c.y} B={c.z})")
    delete pixels
}

[test(description = "palette index 2 in a nested-struct UBO selects blue through the access chains", timeout = 1200)]
def test_ubo_nested_palette_blue(t : T?) {
    var pixels <- render_one_id(2)
    let c = px(pixels, 128, 128)
    t |> success(c.z > 200 && c.x < 120 && c.y < 120,
        "palette[2]=blue selected -> blue-dominant center (got R={c.x} G={c.y} B={c.z})")
    delete pixels
}

// std140 layout: rot @ 0 (3 cols * 16 = 48), hue @ 48 (vec3, 12 bytes), gain @ 60 (scalar packs into
// the 4 bytes after the vec3 -- NOT at 64). Total 64.
struct PCStd140 {
    rot  : float3x3     // offset 0, size 48
    hue  : float3       // offset 48
    gain : float        // offset 60
}

var @push_constant std140_pc : PCStd140
var @out @location = 0 pc_color : float4

[vulkan_vertex_shader(name="pc_std140_vert_spv"), marker(no_coverage)]
def pc_std140_vert {
    let i = gl_VertexIndex
    var xy = float2(0.0f, 0.0f)
    if (i == 0) { xy = float2(-1.0f, -1.0f) }
    if (i == 1) { xy = float2(3.0f, -1.0f) }
    if (i == 2) { xy = float2(-1.0f, 3.0f) }
    gl_Position = float4(xy, 0.0f, 1.0f)
}

[vulkan_fragment_shader(name="pc_std140_frag_spv"), marker(no_coverage)]
def pc_std140_frag {
    // mat3 * basis = column 2 (per-column repack path)
    let c2 = std140_pc.rot * float3(0.0f, 0.0f, 1.0f)
    pc_color = float4(std140_pc.hue.x, std140_pc.gain, c2.z, 1.0f)
}


def private render_push() : array<uint8> {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan test push_std140", make_api_version(1u, 2u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, gfx, 0u)
    let fmt = VkFormat.R8G8B8A8_UNORM

    var inscope target <- build_offscreen_target(device, phys, TEX_W, TEX_H, fmt)
    var inscope render_pass <- create_render_pass_single_color(device, fmt)
    var fbci = FramebufferCreateInfo(renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(target.view)],
        width = uint(TEX_W), height = uint(TEX_H), layers = 1u)
    var inscope framebuffer <- create_framebuffer(device, fbci)

    var reflections <- [decode_reflection(pc_std140_vert_spv_reflect), decode_reflection(pc_std140_frag_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)
    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    delete reflections

    var inscope vert <- create_shader_module(device, pc_std140_vert_spv)
    var inscope frag <- create_shader_module(device, pc_std140_frag_spv)
    var inscope pipeline <- create_graphics_pipeline_simple(device, render_pass, pipe_layout, vert, frag, TEX_W, TEX_H)

    let buf_size = uint64(TEX_W * TEX_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    // Decoys = 0.9; read values hue.x=0.2, gain=0.4, rot column2.z=0.6 -> expected center (51,102,153), so a wrong column/offset reads a 0.9 decoy (~230) or stale memory.
    std140_pc.rot[0] = float3(0.9f, 0.9f, 0.9f)
    std140_pc.rot[1] = float3(0.9f, 0.9f, 0.9f)
    std140_pc.rot[2] = float3(0.0f, 0.0f, 0.6f)
    std140_pc.hue = float3(0.2f, 0.9f, 0.9f)
    std140_pc.gain = 0.4f

    run_cmd_sync(device, pool, queue) $(cmd) {
        record_render_pass(cmd, render_pass, framebuffer, full_area(TEX_W, TEX_H), clear_color(0.0f, 0.0f, 0.0f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipeline)
            pc_std140_frag_push_constants(cmd, pipe_layout)
            cmd_draw(cmd, 3u)
        }
        copy_image_to_buffer(cmd, target.image, readback, TEX_W, TEX_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

// True on Mesa llvmpipe (lavapipe) -- see the header note. A throwaway instance just to read the device
// name; the render below makes its own.
def private is_llvmpipe() : bool {
    if (volkInitialize() != 0) {
        return false
    }
    var inscope instance <- create_instance("push_std140 driver probe", make_api_version(1u, 2u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    var props : VkPhysicalDeviceProperties
    vkGetPhysicalDeviceProperties(phys, props)
    unsafe {
        return find("{reinterpret<string>(addr(props.deviceName[0]))}", "llvmpipe") >= 0
    }
}

[test(description = "a push-constant block holding a mat3 and a scalar after a vec3 is read at the std140 offsets", timeout = 1200)]
def test_push_std140(t : T?) {
    if (is_llvmpipe()) {
        t->skip("Mesa llvmpipe mis-reads std140 push constants (float-after-vec3 / post-mat3 read from the wrong offset in PushConstant storage). Driver bug, not ours: same block reads correctly as a UBO + on real hardware, SPIR-V is glslang-identical + spirv-val-clean. See the header note.")
        return
    }
    var pixels <- render_push()
    let c = px(pixels, 128, 128)
    // hue.x=0.2 -> R~51, gain=0.4 -> G~102, rot col2.z=0.6 -> B~153. The scalar `gain` reading right
    // proves the float3-before-scalar offset; the B channel proves the mat3 per-column packing.
    t |> success(abs(c.x - 51) <= 8 && abs(c.y - 102) <= 8 && abs(c.z - 153) <= 8,
        "std140 push: mat3 column + float3-before-scalar read at correct offsets (got R={c.x} G={c.y} B={c.z})")
    delete pixels
}

//! GPU-free: no device is created
[test(description = "the reflection descriptor kinds and stage flags map to the matching Vulkan enums")]
def test_reflect_mappers(t : T?) {
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.uniform_buffer) == VkDescriptorType.UNIFORM_BUFFER, "uniform_buffer -> UNIFORM_BUFFER")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.storage_buffer) == VkDescriptorType.STORAGE_BUFFER, "storage_buffer -> STORAGE_BUFFER")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.combined_image_sampler) == VkDescriptorType.COMBINED_IMAGE_SAMPLER, "combined_image_sampler -> COMBINED_IMAGE_SAMPLER")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.storage_image) == VkDescriptorType.STORAGE_IMAGE, "storage_image -> STORAGE_IMAGE")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.sampled_image) == VkDescriptorType.SAMPLED_IMAGE, "sampled_image -> SAMPLED_IMAGE")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.sampler) == VkDescriptorType.SAMPLER, "sampler -> SAMPLER")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.comparison_sampler) == VkDescriptorType.COMBINED_IMAGE_SAMPLER, "comparison_sampler -> COMBINED_IMAGE_SAMPLER: a sampler2DShadow is a combined image sampler with the depth flag")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.input_attachment) == VkDescriptorType.INPUT_ATTACHMENT, "input_attachment -> INPUT_ATTACHMENT")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.acceleration_structure) == VkDescriptorType.ACCELERATION_STRUCTURE_KHR, "acceleration_structure -> ACCELERATION_STRUCTURE_KHR")
    let vf = to_vk_stage_flags(SpirvStageFlags.vertex | SpirvStageFlags.compute)
    t |> success(vf.vertex && vf.compute && !vf.fragment, "stage flags map vertex|compute, not fragment")
    let rt = to_vk_stage_flags(SpirvStageFlags.raygen | SpirvStageFlags.miss | SpirvStageFlags.closest_hit)
    t |> success(rt.raygen_khr && rt.miss_khr && rt.closest_hit_khr && !rt.compute, "RT stage flags map raygen|miss|closest_hit, not compute")
}

[test(description = "one binding declared by two stages merges into a single binding carrying both stage flags")]
def test_reflect_merge_shared_binding(t : T?) {
    // two stages both declaring set 0 / binding 0 (a UBO read in vertex AND fragment)
    var rv : SpirvReflection
    rv.bindings |> push(SpirvDescriptorBinding(set = 0, binding = 0, kind = SpirvDescriptorKind.uniform_buffer, count = 1, stages = SpirvStageFlags.vertex))
    var rf : SpirvReflection
    rf.bindings |> push(SpirvDescriptorBinding(set = 0, binding = 0, kind = SpirvDescriptorKind.uniform_buffer, count = 1, stages = SpirvStageFlags.fragment))
    var refls <- [ <- rv, <- rf]
    var merged <- merge_set_bindings(refls, 0)
    t |> success(length(merged) == 1, "shared (set 0, binding 0) collapses to one binding")
    if (length(merged) == 1) {
        t |> success(merged[0].binding == 0u, "merged binding number is 0")
        t |> success(merged[0].descriptorType == VkDescriptorType.UNIFORM_BUFFER, "merged type is UNIFORM_BUFFER")
        t |> success(merged[0].stageFlags.vertex && merged[0].stageFlags.fragment, "merged stage flags are vertex|fragment")
    }
    unsafe { delete merged }    // DescriptorSetLayoutBinding holds a sampler-array field -> unsafe delete
    delete refls
}

[test(description = "two different bindings in one set stay separate and keep their insertion order")]
def test_reflect_merge_distinct_bindings(t : T?) {
    // the gate's actual shape: vertex UBO @binding 0, fragment sampler @binding 1, both in set 0
    var rv : SpirvReflection
    rv.bindings |> push(SpirvDescriptorBinding(set = 0, binding = 0, kind = SpirvDescriptorKind.uniform_buffer, count = 1, stages = SpirvStageFlags.vertex))
    var rf : SpirvReflection
    rf.bindings |> push(SpirvDescriptorBinding(set = 0, binding = 1, kind = SpirvDescriptorKind.combined_image_sampler, count = 1, stages = SpirvStageFlags.fragment))
    var refls <- [ <- rv, <- rf]
    var merged <- merge_set_bindings(refls, 0)
    t |> success(length(merged) == 2, "two distinct bindings stay separate")
    if (length(merged) == 2) {
        // merge preserves insertion order: vertex UBO @0 first, then fragment sampler @1
        t |> success(merged[0].binding == 0u && merged[0].descriptorType == VkDescriptorType.UNIFORM_BUFFER &&
            merged[0].stageFlags.vertex && !merged[0].stageFlags.fragment, "binding 0 is a vertex UBO")
        t |> success(merged[1].binding == 1u && merged[1].descriptorType == VkDescriptorType.COMBINED_IMAGE_SAMPLER &&
            merged[1].stageFlags.fragment && !merged[1].stageFlags.vertex, "binding 1 is a fragment sampler")
    }
    unsafe { delete merged }    // DescriptorSetLayoutBinding holds a sampler-array field -> unsafe delete
    delete refls
}

//! the CONSTANT_COLOR oracle is skipped on llvmpipe, which reads the
//! blend color as ARGB
[test(description = "a graphics pipeline built only from boost view structs renders exact pixels for both batch entries", timeout = 1200)]
def test_pipeline_boost_structs(t : T?) {   // nolint:STYLE038 - flat test cell
    if (volkInitialize() != 0) {
        t |> failure("no Vulkan loader")
        return
    }
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

    var inscope vert <- create_shader_module(device, flat_vert_spv)
    var inscope frag <- create_shader_module(device, flat_frag_spv)
    var inscope layout <- create_pipeline_layout(device)

    // shared fixed-function state, one local per boost view struct; the opt-ptr
    // fields below point at these locals (each view clone-copies its pointee)
    var st_vert = PipelineShaderStageCreateInfo(module_ = weak_copy(vert), pName = "main")
    st_vert.stage.vertex = true
    var st_frag = PipelineShaderStageCreateInfo(module_ = weak_copy(frag), pName = "main")
    st_frag.stage.fragment = true

    var vin_a, vin_b : PipelineVertexInputStateCreateInfo
    var ia = PipelineInputAssemblyStateCreateInfo(topology = VkPrimitiveTopology.TRIANGLE_LIST)
    var vp : PipelineViewportStateCreateInfo
    var vport = Viewport(width = float(TRI_W), height = float(TRI_H), maxDepth = 1.0f)
    vp.pViewports |> push(vport)
    var scissor : Rect2D
    scissor.extent.width = uint(TRI_W)
    scissor.extent.height = uint(TRI_H)
    vp.pScissors |> push(scissor)
    var rs = PipelineRasterizationStateCreateInfo(polygonMode = VkPolygonMode.FILL, lineWidth = 1.0f)
    var ms : PipelineMultisampleStateCreateInfo
    ms.rasterizationSamples._1 = true

    // exact-UNORM8 blend constants: 255 * (64/255, 128/255, 192/255) is integral,
    // so the blended pixel must be exactly (64, 128, 192) on any conformant driver
    var att_const = PipelineColorBlendAttachmentState(blendEnable = 1u,
        srcColorBlendFactor = VkBlendFactor.CONSTANT_COLOR,
        dstColorBlendFactor = VkBlendFactor.ZERO,
        colorBlendOp = VkBlendOp.ADD,
        srcAlphaBlendFactor = VkBlendFactor.ONE,
        dstAlphaBlendFactor = VkBlendFactor.ZERO,
        alphaBlendOp = VkBlendOp.ADD)
    att_const.colorWriteMask.r = true
    att_const.colorWriteMask.g = true
    att_const.colorWriteMask.b = true
    att_const.colorWriteMask.a = true
    var cb_const : PipelineColorBlendStateCreateInfo
    cb_const.pAttachments |> push(att_const)
    cb_const.blendConstants = fixed_array(64.0f / 255.0f, 128.0f / 255.0f, 192.0f / 255.0f, 1.0f)

    var att_plain : PipelineColorBlendAttachmentState
    att_plain.colorWriteMask.r = true
    att_plain.colorWriteMask.g = true
    att_plain.colorWriteMask.b = true
    att_plain.colorWriteMask.a = true
    var cb_plain : PipelineColorBlendStateCreateInfo
    cb_plain.pAttachments |> push(att_plain)

    var gp_const = GraphicsPipelineCreateInfo(layout = weak_copy(layout),
        renderPass = weak_copy(render_pass))
    gp_const.pStages <- [st_vert, st_frag]
    unsafe {
        gp_const.pVertexInputState = addr(vin_a)
        gp_const.pInputAssemblyState = addr(ia)
        gp_const.pViewportState = addr(vp)
        gp_const.pRasterizationState = addr(rs)
        gp_const.pMultisampleState = addr(ms)
        gp_const.pColorBlendState = addr(cb_const)
    }
    var gp_plain = GraphicsPipelineCreateInfo(layout = weak_copy(layout),
        renderPass = weak_copy(render_pass))
    gp_plain.pStages <- [st_vert, st_frag]
    unsafe {
        gp_plain.pVertexInputState = addr(vin_b)
        gp_plain.pInputAssemblyState = addr(ia)
        gp_plain.pViewportState = addr(vp)
        gp_plain.pRasterizationState = addr(rs)
        gp_plain.pMultisampleState = addr(ms)
        gp_plain.pColorBlendState = addr(cb_plain)
    }
    var gpcis <- [ <- gp_const, <- gp_plain]
    let no_cache : PipelineCache
    var inscope pipelines <- create_graphics_pipelines(device, no_cache, gpcis)
    t |> equal(length(pipelines), 2)
    t |> success(pipelines[0]._vk != 0ul && pipelines[1]._vk != 0ul, "both graphics pipelines are non-null")
    t |> success(pipelines[0]._vk != pipelines[1]._vk, "batch-created graphics pipelines are distinct")

    let buf_size = uint64(TRI_W * TRI_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    var pixels_const : array<uint8>
    run_cmd_sync(device, pool, queue) $(cmd) {
        record_render_pass(cmd, render_pass, framebuffer, full_area(TRI_W, TRI_H), clear_color(0.1f, 0.1f, 0.15f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipelines[0])
            cmd_draw(cmd, 3u)
        }
        copy_image_to_buffer(cmd, target.image, readback, TRI_W, TRI_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels_const := m
    }
    if (is_llvmpipe()) {
        t |> success(true, ("skipped the CONSTANT_COLOR pixel oracle on llvmpipe "
            + "(ARGB blend-color driver bug)"))
    } else {
        t |> assert_pixels_exact(pixels_const, TRI_W, TRI_H, "CONSTANT_COLOR blend (blendConstants float[4])") $(_x, _y) {
            return int4(64, 128, 192, 255)
        }
    }
    delete pixels_const

    var pixels_plain : array<uint8>
    run_cmd_sync(device, pool, queue) $(cmd) {
        record_render_pass(cmd, render_pass, framebuffer, full_area(TRI_W, TRI_H), clear_color(0.1f, 0.1f, 0.15f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipelines[1])
            cmd_draw(cmd, 3u)
        }
        copy_image_to_buffer(cmd, target.image, readback, TRI_W, TRI_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels_plain := m
    }
    t |> assert_pixels_exact(pixels_plain, TRI_W, TRI_H, "blend-disabled entry (batch order preserved)") $(_x, _y) {
        return int4(255, 255, 255, 255)
    }
    delete pixels_plain
}
