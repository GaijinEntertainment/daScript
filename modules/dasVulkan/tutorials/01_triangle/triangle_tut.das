options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 01 render path: draw the rotating RGB triangle offscreen at a given angle and return the
// RGBA8 pixels. A pure parametric frame(angle) -> image: the CI gate asserts known frames, and the
// recording driver sweeps angle over a full turn. The only addition over the integration-test
// render_triangle is the vertex push-constant carrying the angle.

module triangle_tut

require vulkan public
require vulkan/vulkan_boost public
require triangle_tut_shaders

let public TRI_W = 256
let public TRI_H = 256

//! Render the rotating RGB triangle at `angle` radians; returns TRI_W*TRI_H RGBA8 pixels (clear
//! color is the dark blue-grey the integration scenes use).
def public render_spin_triangle(angle : float) : array<uint8> {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan tutorial", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, gfx, 0u)
    let fmt = VkFormat.R8G8B8A8_SRGB   // sRGB: gamma-encode the linear output on write (recording readback + window blit match)

    var inscope target <- build_offscreen_target(device, phys, TRI_W, TRI_H, fmt)

    var inscope vert <- create_shader_module(device, tri_spin_vert_spv)
    var inscope frag <- create_shader_module(device, tri_spin_frag_spv)

    // pipeline layout with one vertex push-constant range (the angle: 4 bytes at offset 0)
    var plci : PipelineLayoutCreateInfo
    var pcr : PushConstantRange
    pcr.stageFlags.vertex = true
    pcr.offset = 0u
    pcr.size = 4u
    plci.pPushConstantRanges |> emplace(pcr)
    var inscope layout <- create_pipeline_layout(device, plci)
    delete plci.pPushConstantRanges   // owned input array; the layout copied the ranges, so free it now
    // the pipeline carries the color format it renders into; there is no render pass to bind it to
    var inscope pipeline <- create_graphics_pipeline_simple(device, fmt, layout, vert, frag, TRI_W, TRI_H)

    let buf_size = uint64(TRI_W * TRI_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    pc.angle = angle
    run_cmd_sync(device, pool, queue) $(cmd) {
        // the image starts UNDEFINED; a barrier makes it a color attachment before the draw...
        transition_image2(cmd, target.image, VkImageLayout.UNDEFINED, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL,
            PIPELINE_STAGE_2_NONE, ACCESS_2_NONE,
            VkPipelineStageFlags2.color_attachment_output, VkAccessFlags2.color_attachment_write)
        record_rendering(cmd, target.view, full_area(TRI_W, TRI_H), clear_color(0.1f, 0.1f, 0.15f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipeline)
            tri_spin_vert_push_constants(cmd, layout)
            cmd_draw(cmd, 3u)
        }
        // ...and a second one hands the finished pixels to the copy
        transition_image2(cmd, target.image, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
            VkPipelineStageFlags2.color_attachment_output, VkAccessFlags2.color_attachment_write,
            VkPipelineStageFlags2.copy, VkAccessFlags2.transfer_read)
        copy_image_to_buffer(cmd, target.image, readback, TRI_W, TRI_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! Sample the RGB of pixel (x,y) from a TRI_W*TRI_H RGBA8 buffer.
def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * TRI_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}
