options gen2
options _comment_hygiene = true
options indenting = 4

// The triangle drawn with NO VkPipeline: VK_EXT_shader_object binds a shader object per stage and takes
// every piece of fixed-function state from the command buffer, so a program that changes blend, cull
// or depth state per draw sets it instead of keeping a pipeline per combination. An opt-in beside the
// pipeline path, not a replacement: the extension is absent on Android and partial on MoltenVK, and
// nothing else in the module needs it. Same pixels as offscreen_triangle_boost.das, which the test
// beside this file checks.

module shader_objects

require vulkan public
require vulkan/vulkan_boost public
require triangle_example_shaders
require daslib/fio

let public SO_W = 256
let public SO_H = 256
let SO_FORMAT = VkFormat.R8G8B8A8_UNORM

//! True when a device on this machine can draw with shader objects; the example and its test skip otherwise.
def public shader_objects_available() : bool {
    if (volkInitialize() != 0) {
        return false
    }
    var inscope instance <- create_instance("dasVulkan shader objects probe", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    var count = 0u
    vkEnumeratePhysicalDevices(boost_value_to_vk(instance), count, null)
    if (count == 0u) {
        return false
    }
    return shader_object_supported(select_physical_device(instance))
}

//! Render the RGB triangle through two linked shader objects and return SO_W*SO_H RGBA8 pixels.
def public render_shader_object_triangle() : array<uint8> {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan shader objects", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device_shader_objects(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, gfx, 0u)

    var inscope target <- build_offscreen_target(device, phys, SO_W, SO_H, SO_FORMAT)

    // two shader objects, linked: the vertex one names fragment as the stage after it, and both carry
    // the link flag, so the driver compiles them as one program - no layout, no pipeline
    var infos : array<ShaderCreateInfoEXT>
    var vert_info : ShaderCreateInfoEXT
    vert_info.flags.link_stage_ext = true
    vert_info.stage.vertex = true
    vert_info.nextStage.fragment = true
    vert_info.codeType = VkShaderCodeTypeEXT.SPIRV_EXT
    vert_info.pCode <- spirv_bytes(tri_vert_spv)
    vert_info.pName = "main"
    infos |> emplace(vert_info)
    var frag_info : ShaderCreateInfoEXT
    frag_info.flags.link_stage_ext = true
    frag_info.stage.fragment = true
    frag_info.codeType = VkShaderCodeTypeEXT.SPIRV_EXT
    frag_info.pCode <- spirv_bytes(tri_frag_spv)
    frag_info.pName = "main"
    infos |> emplace(frag_info)
    var inscope shaders <- create_shaders_e_x_t(device, infos)

    let buf_size = uint64(SO_W * SO_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    run_cmd_sync(device, pool, queue) $(cmd) {
        transition_image2(cmd, target.image, VkImageLayout.UNDEFINED, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL,
            PIPELINE_STAGE_2_NONE, ACCESS_2_NONE,
            VkPipelineStageFlags2.color_attachment_output, VkAccessFlags2.color_attachment_write)
        record_rendering(cmd, target.view, full_area(SO_W, SO_H), clear_color(0.1f, 0.1f, 0.15f, 1.0f)) {
            // what cmd_bind_pipeline did in one call is now the two stages plus every state a pipeline froze
            cmd_bind_graphics_shaders(cmd, shaders[0], shaders[1])
            cmd_set_shader_object_baseline(cmd, SO_W, SO_H, 1u)
            cmd_draw(cmd, 3u)
        }
        transition_image2(cmd, target.image, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
            VkPipelineStageFlags2.color_attachment_output, VkAccessFlags2.color_attachment_write,
            VkPipelineStageFlags2.copy, VkAccessFlags2.transfer_read)
        copy_image_to_buffer(cmd, target.image, readback, SO_W, SO_H)
    }
    var pixels : array<uint8>
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! Sample the RGB of pixel (x,y) from an SO_W*SO_H RGBA8 buffer.
def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * SO_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}

def write_ppm(path : string; pixels : array<uint8>; w, h : int) {
    fopen(path, "wb") $(f) {
        fprint(f, "P6\n{w} {h}\n255\n")
        var rgb : array<uint8>
        rgb |> resize(w * h * 3)
        for (i in range(w * h)) {
            rgb[i * 3] = pixels[i * 4]
            rgb[i * 3 + 1] = pixels[i * 4 + 1]
            rgb[i * 3 + 2] = pixels[i * 4 + 2]
        }
        fwrite(f, rgb)
    }
}

[export]
def main() : int {
    if (!shader_objects_available()) {
        print("VK_EXT_shader_object not supported here; nothing drawn\n")
        return 0
    }
    let pixels <- render_shader_object_triangle()
    write_ppm("triangle_shader_objects.ppm", pixels, SO_W, SO_H)
    print("wrote triangle_shader_objects.ppm\n")
    let top = px(pixels, 128, 80)
    let bl = px(pixels, 75, 190)
    let br = px(pixels, 180, 190)
    if (!(top.x > top.y + 40 && bl.z > bl.x + 40 && br.y > br.x + 40)) {
        to_log(LOG_ERROR, "verification FAILED: top={top} bottom-left={bl} bottom-right={br}\n")
        return 1
    }
    print("verification OK: triangle drawn with shader objects\n")
    return 0
}
