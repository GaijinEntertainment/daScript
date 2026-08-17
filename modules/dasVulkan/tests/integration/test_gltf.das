options gen2
options _comment_hygiene = true
options indenting = 4

// vulkan_gltf integration: load glTF fixtures from the daslang tree (tests/dasGLTF/models),
// upload + render offscreen, and property-assert the pixels — Box exercises the rigid pipeline
// (push constants + red PBR material), SimpleSkin the skinned pipeline (joint-palette UBO).

require dastest/testing_boost public
require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_gltf
require daslib/math_boost
require math

let GLTF_TEST_W = 256
let GLTF_TEST_H = 256
let GLTF_COLOR_FMT = VkFormat.R8G8B8A8_UNORM   // the shader sRGB-encodes; an _SRGB target would double-encode
let GLTF_DEPTH_FMT = VkFormat.D32_SFLOAT

def private model_path(name : string) : string {
    return "{get_das_root()}/tests/dasGLTF/models/{name}"
}

// perspective_rh_0_to_1 gives Vulkan's z in [0,1]; Vulkan NDC additionally has +Y down, so flip Y
def private perspective_vk_01(fovy, aspect, znear, zfar : float) : float4x4 {
    var m = perspective_rh_0_to_1(fovy, aspect, znear, zfar)
    m[1] = float4(m[1].x, -m[1].y, m[1].z, m[1].w)
    return m
}

def private px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * GLTF_TEST_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}

// render `scene` offscreen with an auto-framed camera; self-contained context per call
def private render_gltf_scene(var scene : GltfScene) : array<uint8> {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan gltf test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    var inscope color <- build_offscreen_target(device, phys, GLTF_TEST_W, GLTF_TEST_H, GLTF_COLOR_FMT)
    var inscope depth <- build_offscreen_depth(device, phys, GLTF_TEST_W, GLTF_TEST_H, GLTF_DEPTH_FMT)
    var inscope render_pass <- create_render_pass_color_depth(device, GLTF_COLOR_FMT, GLTF_DEPTH_FMT)
    var inscope framebuffer <- create_framebuffer(device, FramebufferCreateInfo(
        renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(color.view), weak_copy(depth.view)],
        width = uint(GLTF_TEST_W),
        height = uint(GLTF_TEST_H),
        layers = 1u))
    let buf_size = uint64(GLTF_TEST_W * GLTF_TEST_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    var inscope model <- gltf_vk_upload(device, phys, queue, pool, render_pass, scene)

    // frame the scene's world AABB from a 3/4 view
    let (bmin, bmax) = gltf_scene_bounds(scene)
    let center = (bmin + bmax) * 0.5
    let radius = max(length(bmax - bmin) * 0.5, 0.001)
    let eye = center + normalize(float3(0.6, 0.4, 1.0)) * (radius * 2.6)
    let view = look_at_rh(eye, center, float3(0.0, 1.0, 0.0))
    let proj = perspective_vk_01(45.0 * PI / 180.0, float(GLTF_TEST_W) / float(GLTF_TEST_H), radius * 0.05, radius * 10.0)
    gltf_vk_update(device, model, scene, identity4x4(), view, proj, eye, GltfPbrLight())

    var pixels : array<uint8>
    run_cmd_sync(device, pool, queue) $(cmd) {
        var clears <- [clear_color(0.05f, 0.07f, 0.1f, 1.0f), clear_depth(1.0f)]
        record_render_pass(cmd, render_pass, framebuffer, full_area(GLTF_TEST_W, GLTF_TEST_H), clears) {
            gltf_vk_render(cmd, model, scene, identity4x4(), GLTF_TEST_W, GLTF_TEST_H)
        }
        delete clears
        copy_image_to_buffer(cmd, color.image, readback, GLTF_TEST_W, GLTF_TEST_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

[test]
def test_gltf_box(t : T?) {
    t |> run("Box.glb renders through the rigid PBR path") @(t : T?) {
        var scene <- load_gltf(model_path("Box.glb"))
        t |> equal(length(scene.meshes), 1)
        var pixels <- render_gltf_scene(scene)
        t |> equal(length(pixels), GLTF_TEST_W * GLTF_TEST_H * 4)
        // corner = clear color (dark blue-grey), center = the lit red Box material (0.8, 0, 0)
        let corner = px(pixels, 2, 2)
        t |> success(corner.x < 90 && corner.z >= corner.x, "corner should be the clear color, got {corner}")
        let center = px(pixels, GLTF_TEST_W / 2, GLTF_TEST_H / 2)
        t |> success(center.x > 70, "center should be lit red, got {center}")
        t |> success(center.x > center.y * 2 && center.x > center.z * 2, "center should be dominantly red, got {center}")
        delete pixels
        delete scene
    }
}

[test]
def test_gltf_simple_skin(t : T?) {
    t |> run("SimpleSkin.gltf renders through the skinned path") @(t : T?) {
        var scene <- load_gltf(model_path("SimpleSkin.gltf"))
        t |> equal(length(scene.skins), 1)
        // pose mid-animation so the strip is bent by the joint palette, then render skinned
        evaluate_animation(scene, 0, 2.75)
        var pixels <- render_gltf_scene(scene)
        // the untextured default-material strip must cover the center with a non-background tone
        let center = px(pixels, GLTF_TEST_W / 2, GLTF_TEST_H / 2)
        t |> success(center.x > 40 && center.y > 40 && center.z > 40, "center should be the lit default material, got {center}")
        let corner = px(pixels, 2, 2)
        t |> success(center.x > corner.x + 20, "center should differ from the clear color, got center {center} corner {corner}")
        delete pixels
        delete scene
    }
}
