options gen2
options _comment_hygiene = true
options indenting = 4

// PR 8 (dasSpirv PR-A GPU regression gate): a vertex shader reading gl_DrawID (BuiltIn DrawIndex)
// to pick a per-draw color, dispatched via vkCmdDrawIndirect with 4 draws in one command. Each
// draw renders a quad covering a vertical strip; the strip's color is its gl_DrawID. Pixel oracle:
// at the center of each strip, the rendered pixel matches the expected color.
//
// Device features: multiDrawIndirect (so drawCount > 1 is valid) + shaderDrawParameters (so the
// SPIR-V can read DrawIndex). Both threaded via the new create_device_draw_parameters helper --
// the latter pulls in PR-A's DrawParameters capability emission requirement.

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require vulkan/vulkan_commands public
require spirv/spirv_shader public
require spirv/spirv_builtins public
require spirv/spirv_reflect public
require scene_helpers

// ===== Shader fixture =====
// Each draw renders a 6-vertex quad. gl_DrawID selects:
//   - the X strip: xmin = -1 + draw_id * 0.5, xmax = xmin + 0.5  (NDC, 4 strips of width 0.5)
//   - the output color: 0 -> red, 1 -> green, 2 -> blue, 3 -> yellow
// gl_VertexIndex picks corner: 0=(xmin,-1) 1=(xmax,-1) 2=(xmax,1) 3=(xmin,-1) 4=(xmax,1) 5=(xmin,1).
// dasSpirv auto-applies the Flat decoration to integer @in / @out (per tests/spirv/test_raster.das
// `test_vertex_flat`); marking the vertex side explicitly @flat here matches the house convention
// (see tutorials/10_deferred/deferred_tut_shaders.das) and makes the intent unambiguous to
// readers comparing the cross-stage interface.
var @flat @out @location = 0 di_draw : int

[vertex_shader(name="draw_id_vert_spv"), marker(no_coverage)]
def draw_id_vert {
    let did = gl_DrawID
    let xmin = float(did) * 0.5f - 1.0f
    let xmax = xmin + 0.5f
    let i = gl_VertexIndex
    var xy = float2(xmin, -1.0f)
    if (i == 1) { xy = float2(xmax, -1.0f) }
    if (i == 2) { xy = float2(xmax,  1.0f) }
    if (i == 3) { xy = float2(xmin, -1.0f) }
    if (i == 4) { xy = float2(xmax,  1.0f) }
    if (i == 5) { xy = float2(xmin,  1.0f) }
    di_draw = did
    gl_Position = float4(xy, 0.0f, 1.0f)
}

var @in @flat @location = 0 fi_draw : int
var @out @location = 0 di_color : float4

[fragment_shader(name="draw_id_frag_spv"), marker(no_coverage)]
def draw_id_frag {
    var c = float4(0.0f, 0.0f, 0.0f, 1.0f)
    if (fi_draw == 0) { c = float4(1.0f, 0.0f, 0.0f, 1.0f) }
    if (fi_draw == 1) { c = float4(0.0f, 1.0f, 0.0f, 1.0f) }
    if (fi_draw == 2) { c = float4(0.0f, 0.0f, 1.0f, 1.0f) }
    if (fi_draw == 3) { c = float4(1.0f, 1.0f, 0.0f, 1.0f) }
    di_color = c
}

// ===== Host render =====
let TEX_W = 256
let TEX_H = 256
let N_DRAW = 4

def private render_draw_id() : array<uint8> {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan test gl_DrawID", make_api_version(1u, 1u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    // multiDrawIndirect (base features) + shaderDrawParameters (1.1 features).
    var inscope device <- create_device_draw_parameters(phys, gfx)
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

    // Indirect draw buffer: N_DRAW VkDrawIndirectCommand entries (16 bytes each).
    let stride = 16u
    let buf_bytes = uint64(stride * uint(N_DRAW))
    var indirect_usage : VkBufferUsageFlags
    indirect_usage.indirect_buffer = true
    var inscope indirect_buf <- create_host_buffer(device, phys, buf_bytes, indirect_usage)
    var indirect_bytes : array<uint8>
    indirect_bytes |> resize(int64(buf_bytes))
    for (k in range(N_DRAW)) {
        // every draw: 6 vertices, 1 instance; the shader keys off gl_DrawID + gl_VertexIndex, so first{Vertex,Instance} stay 0.
        write_u32_at(indirect_bytes, k * int(stride) + 0,  6u)    // vertexCount
        write_u32_at(indirect_bytes, k * int(stride) + 4,  1u)    // instanceCount
        write_u32_at(indirect_bytes, k * int(stride) + 8,  0u)    // firstVertex
        write_u32_at(indirect_bytes, k * int(stride) + 12, 0u)    // firstInstance
    }
    upload_bytes(indirect_buf, device, indirect_bytes)
    delete indirect_bytes

    var reflections <- [decode_reflection(draw_id_vert_spv_reflect), decode_reflection(draw_id_frag_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)
    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    delete reflections

    var inscope vert <- create_shader_module(device, draw_id_vert_spv)
    var inscope frag <- create_shader_module(device, draw_id_frag_spv)
    var inscope pipeline <- create_graphics_pipeline_simple(device, render_pass, pipe_layout, vert, frag, TEX_W, TEX_H)

    let buf_size = uint64(TEX_W * TEX_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    run_cmd_sync(device, cmd_pool, queue) $(cmd) {
        record_render_pass(cmd, render_pass, framebuffer, full_area(TEX_W, TEX_H), clear_color(0.0f, 0.0f, 0.0f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipeline)
            cmd_draw_indirect(cmd, indirect_buf.buffer, 0ul, uint(N_DRAW), stride)
        }
        copy_image_to_buffer(cmd, target.image, readback, TEX_W, TEX_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

def private write_u32_at(var bytes : array<uint8>; off : int; v : uint) {
    var probe = fixed_array(v)
    unsafe {
        let src = addr<uint8 const?>(probe[0])
        for (b in range(4)) {
            bytes[off + b] = src[b]
        }
    }
}

// ===== Pixel oracle =====
// 4 vertical strips, each 64 px wide on a 256 px texture. Strip 0 center is x=32, strip 1 center is
// x=96, strip 2 center is x=160, strip 3 center is x=224. All at y=128.
[test]
def test_gl_draw_id_indirect(t : T?) {
    var pixels <- render_draw_id()
    let c0 = px(pixels, 32, 128)
    let c1 = px(pixels, 96, 128)
    let c2 = px(pixels, 160, 128)
    let c3 = px(pixels, 224, 128)
    t |> success(c0.x > 200 && c0.y < 60 && c0.z < 60,
        "strip 0 (gl_DrawID=0) is red (got R={c0.x} G={c0.y} B={c0.z})")
    t |> success(c1.y > 200 && c1.x < 60 && c1.z < 60,
        "strip 1 (gl_DrawID=1) is green (got R={c1.x} G={c1.y} B={c1.z})")
    t |> success(c2.z > 200 && c2.x < 60 && c2.y < 60,
        "strip 2 (gl_DrawID=2) is blue (got R={c2.x} G={c2.y} B={c2.z})")
    t |> success(c3.x > 200 && c3.y > 200 && c3.z < 60,
        "strip 3 (gl_DrawID=3) is yellow (got R={c3.x} G={c3.y} B={c3.z})")
    delete pixels
}
