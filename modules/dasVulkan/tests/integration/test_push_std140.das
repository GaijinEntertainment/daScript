options gen2
options _comment_hygiene = true
options indenting = 4

// GPU regression gate for std140-packed PUSH CONSTANTS (the push analogue of test_ubo_mat3). The
// generated push_constants_<shader> now stages the block through the shared std140 walk
// (daslib/shader_block_layout) + write_field, then vkCmdPushConstants the staged bytes -- instead of
// raw-uploading reinterpret(addr(pc)). The old raw path mis-packed exactly the shapes here: a float3x3
// (daslang packs 9 tight floats = 36 bytes; std140 wants 3 columns at 0/16/32 = 48) and a float3
// immediately before a scalar (daslang pads the float3 to 16, so the scalar lands at the wrong offset).
// The fragment reads one value via each path -- rot column 2 (mat3 per-column), gain (the scalar right
// after the float3) -- with 0.9 decoys elsewhere, so a wrong offset / column / stride yields a clearly
// different pixel. Vertex is a plain fullscreen triangle; only the fragment reaches the push constant.
//
// SKIPPED on Mesa llvmpipe (lavapipe): llvmpipe mis-reads std140 PUSH constants where a member's offset
// is tighter than naive 16-byte rounding -- a float immediately after a vec3 (std140 offset 12), and any
// member after a mat3 -- reading them from the wrong offset. This is a llvmpipe driver bug, NOT a layout
// bug on our side: the SAME std140 block reads correctly as a UBO and on real hardware, our SPIR-V is
// byte-identical to glslang's + spirv-val-clean, and a fully hand-built push (explicit layout + manual
// std140 bytes) fails on llvmpipe too. So we keep the full GPU render+read on real hardware and skip only
// the read on llvmpipe; the shared std140 walk stays CI-gated on llvmpipe via the UBO path (test_ubo_mat3).

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
require strings

// std140 layout: rot @ 0 (3 cols * 16 = 48), hue @ 48 (vec3, 12 bytes), gain @ 60 (scalar packs into
// the 4 bytes after the vec3 -- NOT at 64). Total 64.
struct PCStd140 {
    rot  : float3x3     // offset 0, size 48
    hue  : float3       // offset 48
    gain : float        // offset 60
}

var @push_constant pc : PCStd140
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
    let c2 = pc.rot * float3(0.0f, 0.0f, 1.0f)       // mat3 * basis = column 2 (per-column repack path)
    pc_color = float4(pc.hue.x, pc.gain, c2.z, 1.0f)
}

let TEX_W = 256
let TEX_H = 256

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
    pc.rot[0] = float3(0.9f, 0.9f, 0.9f)
    pc.rot[1] = float3(0.9f, 0.9f, 0.9f)
    pc.rot[2] = float3(0.0f, 0.0f, 0.6f)
    pc.hue = float3(0.2f, 0.9f, 0.9f)
    pc.gain = 0.4f

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

[test]
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
