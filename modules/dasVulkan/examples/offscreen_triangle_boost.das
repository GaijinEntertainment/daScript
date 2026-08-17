options gen2
options _comment_hygiene = true

require vulkan
require vulkan/vulkan_boost
require triangle_example_shaders
require daslib/fio
require math

let WIDTH = 256
let HEIGHT = 256
let FORMAT = VkFormat.R8G8B8A8_UNORM

def sample(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * WIDTH + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}

def is_clear(c : int3) : bool {
    return abs(c.x - 26) <= 8 && abs(c.y - 26) <= 8 && abs(c.z - 38) <= 8
}

def vcheck(what : string; ok : bool) : int {
    print("  [{ok ? "PASS" : "FAIL"}] {what}\n")
    return ok ? 0 : 1
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
    if (volkInitialize() != 0) {
        to_log(LOG_ERROR, "no Vulkan loader\n")
        return 1
    }

    var inscope instance <- create_instance("dasVulkan triangle", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, gfx, 0u)

    var inscope target <- build_offscreen_target(device, phys, WIDTH, HEIGHT, FORMAT)
    var inscope render_pass <- create_render_pass_single_color(device, FORMAT)

    var fbci = FramebufferCreateInfo(renderPass = weak_copy(render_pass),
                                     pAttachments <- [weak_copy(target.view)],
                                     width = uint(WIDTH), height = uint(HEIGHT), layers = 1u)
    var inscope framebuffer <- create_framebuffer(device, fbci)

    // tri_vert_spv / tri_frag_spv: SPIR-V words emitted by dasSpirv from triangle_example_shaders.das
    var inscope vert <- create_shader_module(device, tri_vert_spv)
    var inscope frag <- create_shader_module(device, tri_frag_spv)
    var inscope layout <- create_pipeline_layout(device)
    var inscope pipeline <- create_graphics_pipeline_simple(device, render_pass, layout, vert, frag, WIDTH, HEIGHT)

    let buf_size = uint64(WIDTH * HEIGHT * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)
    var inscope pool <- create_command_pool(device, command_pool_info(gfx))

    run_cmd_sync(device, pool, queue) $(cmd) {
        record_render_pass(cmd, render_pass, framebuffer, full_area(WIDTH, HEIGHT), clear_color(0.1f, 0.1f, 0.15f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipeline)
            cmd_draw(cmd, 3u)
        }
        copy_image_to_buffer(cmd, target.image, readback, WIDTH, HEIGHT)
    }

    var pixels : array<uint8>
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }

    write_ppm("triangle_boost.ppm", pixels, WIDTH, HEIGHT)
    print("wrote triangle_boost.ppm\n")

    let top = sample(pixels, 128, 80)
    let bl = sample(pixels, 75, 190)
    let br = sample(pixels, 180, 190)
    let mid = sample(pixels, 128, 150)
    let k0 = sample(pixels, 5, 5)
    var fails = 0
    fails += vcheck("top is red", top.x > top.y + 40 && top.x > top.z + 40)
    fails += vcheck("bottom-left is blue", bl.z > bl.x + 40 && bl.z > bl.y + 40)
    fails += vcheck("bottom-right is green", br.y > br.x + 40 && br.y > br.z + 40)
    fails += vcheck("centroid is a blend", mid.x > 20 && mid.y > 20 && mid.z > 20)
    fails += vcheck("corner is clear color", is_clear(k0))
    if (fails != 0) {
        to_log(LOG_ERROR, "verification FAILED\n")
        return 1
    }
    print("verification OK: boost triangle rendered\n")
    return 0
}

//! a CommandPoolCreateInfo for a queue family (the generated create_command_pool takes the boost struct)
def command_pool_info(queue_family : uint) : CommandPoolCreateInfo {
    var ci : CommandPoolCreateInfo
    ci.queueFamilyIndex = queue_family
    return ci
}
