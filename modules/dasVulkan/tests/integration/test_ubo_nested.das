options gen2
options _comment_hygiene = true
options indenting = 4

// PR 8 (dasSpirv PR-G GPU regression gate): a UBO whose top-level fields cover the three composite
// shapes the pre-PR-G emitter rejected -- a fixed-array of vec4 (palette), a nested struct
// (Material), and an array of struct (lights). The fragment reads ONE field from each composite via
// the access chains landed in PR-G:
//   - nubo.palette[push_id].rgb              (fixed-array of vec4, indexed by push constant)
//   - nubo.material.base_color * material.roughness  (nested-struct member chain)
//   - nubo.lights[1].color.rgb               (array-of-struct member chain)
// The host uploads the UBO bytes directly (the std140 layout below mirrors the emitter's published
// offsets exactly; see daslang's tests/spirv/test_ubo_nested.das for the same fixture without GPU).
// A correct render on lavapipe (or any real driver) proves the std140 ArrayStride 16 + nested struct
// Offset cascade + array-of-struct ArrayStride 32 are GPU-honored end to end -- spirv-val + opcode
// asserts cannot verify the layout numerically lines up with what the driver expects.

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require vulkan/vulkan_commands public
require spirv/spirv_shader public
require spirv/spirv_reflect public
require scene_helpers

// ===== Shader fixture =====
// NestedUBO is the same struct as daslang's tests/spirv/test_ubo_nested.das fixture: published
// layout is palette@0 (3*16=48), material@48 (laid-out size 32; std140 align rounds 24 up to 32),
// lights@80 (2*32=64). Total 144 bytes. NMaterial offsets: base_color@0, roughness@16, metallic@20.
// NLight offsets: pos@0, color@16. We keep field names + ORDER aligned with the offsets in the
// host upload below.
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
let TEX_W = 256
let TEX_H = 256

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

// ===== Pixel oracle =====

// color = palette[id] + material(0.05) + lights[1].color(0.02), so id 0/1/2 give approx
// (1.07,0.27,0.27) / (0.27,1.07,0.27) / (0.27,0.27,1.07) -- red / green / blue-dominant.
// A wrong index picks dark grey instead: a wash with no dominance.
[test]
def test_ubo_nested_palette_red(t : T?) {
    var pixels <- render_one_id(0)
    let c = px(pixels, 128, 128)
    t |> success(c.x > 200 && c.y < 120 && c.z < 120,
        "palette[0]=red selected -> red-dominant center (got R={c.x} G={c.y} B={c.z})")
    delete pixels
}

[test]
def test_ubo_nested_palette_green(t : T?) {
    var pixels <- render_one_id(1)
    let c = px(pixels, 128, 128)
    t |> success(c.y > 200 && c.x < 120 && c.z < 120,
        "palette[1]=green selected -> green-dominant center (got R={c.x} G={c.y} B={c.z})")
    delete pixels
}

[test]
def test_ubo_nested_palette_blue(t : T?) {
    var pixels <- render_one_id(2)
    let c = px(pixels, 128, 128)
    t |> success(c.z > 200 && c.x < 120 && c.y < 120,
        "palette[2]=blue selected -> blue-dominant center (got R={c.x} G={c.y} B={c.z})")
    delete pixels
}
