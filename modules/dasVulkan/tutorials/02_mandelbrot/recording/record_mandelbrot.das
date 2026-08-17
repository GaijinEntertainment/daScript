options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 02 recording driver (manual, local-only). Renders the animated Mandelbrot zoom headlessly
// at fixed dt = 1/fps (no glfw window, no swapchain), captures an APNG, then ffmpeg-muxes a strudel
// music bed into the committed ../mandelbrot.mp4. CI never runs this -- the tutorials .das_test skips
// the `recording/` folder when stbimage/audio aren't loaded. Sibling of record_triangle.das.
//
//   <daslang> -load_module <dasVulkan> tutorials/02_mandelbrot/recording/record_mandelbrot.das

require ../window/mandelbrot_compute.das
require ../window/mandelbrot_zoom_shaders.das
require ../../recording/tutorial_record_voiced.das    // re-exports tutorial_record's silent path
require vulkan
require vulkan/vulkan_boost
require daslib/fio
require daslib/module_path

// Long-lived Vulkan state shared across all recorded frames -- per-call rebuild would dominate the
// recording wall-clock (900 frames * full instance/device init = many minutes; the triangle driver
// can afford per-call rebuild because triangle is cheap, the Mandelbrot compute cannot).
struct private MandelRecorder {
    instance : Instance
    device   : Device
    queue    : VkQueue
    pool     : CommandPool
    mc       : MandelCompute
    readback : HostBuffer
    buf_size : uint64
}

// Custom finalize so `var inscope rec` works at the call site without leaking unsafe outward -- the
// raw VkQueue handle is non-owning (no destroy), the rest delete in reverse-of-creation order.
def private finalize(var rec : MandelRecorder) {
    unsafe {
        delete rec.readback
        delete rec.mc
        delete rec.pool
        delete rec.device
        delete rec.instance
    }
}

def private build_mandel_recorder() : MandelRecorder {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan tutorial 02 (record)", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx  = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)   // var (not let) so the pointee stays non-const

    var inscope mc <- build_mandel_compute(device, phys, mandelbrot_zoom_spv, ZOOM_DIM, ZOOM_DIM)

    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    let buf_size = uint64(ZOOM_DIM * ZOOM_DIM * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    return <- MandelRecorder(
        instance <- instance,
        device <- device,
        queue = queue,
        pool <- pool,
        mc <- mc,
        readback <- readback,
        buf_size = buf_size)
}

// One frame: dispatch the compute shader at push-constant time `t`, then read the storage image back
// into a host buffer and copy the bytes out as RGBA8. `capture_apng` invokes this once per integer
// frame; deterministic so the APNG can be re-rendered identically if needed.
def private render_mandel_frame(var rec : MandelRecorder; t : float) : array<uint8> {
    var pixels : array<uint8>
    run_cmd_sync(rec.device, rec.pool, rec.queue) $(cmd) {
        let none          : VkAccessFlags
        var shader_write  : VkAccessFlags; shader_write.shader_write  = true
        var transfer_read : VkAccessFlags; transfer_read.transfer_read = true
        var top           : VkPipelineStageFlags; top.top_of_pipe       = true
        var comp          : VkPipelineStageFlags; comp.compute_shader   = true
        var xfer          : VkPipelineStageFlags; xfer.transfer         = true
        // UNDEFINED -> GENERAL so the compute shader can write the storage image
        transition_image(cmd, rec.mc.image, VkImageLayout.UNDEFINED, VkImageLayout.GENERAL,
            none, shader_write, top, comp)
        record_compute(cmd, rec.mc, t)
        // GENERAL -> TRANSFER_SRC_OPTIMAL so vkCmdCopyImageToBuffer can read it
        transition_image(cmd, rec.mc.image, VkImageLayout.GENERAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
            shader_write, transfer_read, comp, xfer)
        copy_image_to_buffer(cmd, rec.mc.image, rec.readback, ZOOM_DIM, ZOOM_DIM)
    }
    // map + clone the bytes out (the mapped pointer is invalidated when the block ends)
    map_memory_to_array(rec.device, rec.readback.memory, rec.buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

[export]
def main {
    let here = get_this_module_dir()
    let apng = path_join(here, "mandelbrot.apng")
    let out_mp4 = path_join(dir_name(here), "mandelbrot.mp4")
    let das_root = get_das_root()
    let daslang_exe = find_daslang_exe(das_root)
    let music = path_join(das_root, "examples/daStrudel/features/sf2_integration_full_ambient.das")

    let n_frames = 30 * 30      // 30 fps * 30 s
    let fps = 30
    let frame_ms = 1000 / fps
    let dur_s = float(n_frames) / float(fps)

    // captions stay canonical; voice text is respelled phonetically per skills/recording.md
    // (dasVulkan -> "das Vulkan", SPIR-V -> "Spear V" per Khronos, digits -> words for the ear).
    say( 30, "dasVulkan tutorial 2 -- Mandelbrot zoom",
            "das Vulkan tutorial two. The Mandelbrot zoom.")
    say(360, "Time drives the animation",
            "One time push constant drives the whole animation. Zoom. Rotation. Color.")
    say(720, "daslang to SPIR-V",
            "Authored in daslang. Lowered to Spear V.")

    var inscope rec <- build_mandel_recorder()

    let ok = capture_apng(apng, ZOOM_DIM, ZOOM_DIM, n_frames, frame_ms) $(frame : int) {
        let t = float(frame) / float(fps)
        return <- render_mandel_frame(rec, t)
    }
    if (!ok) {
        panic("capture failed")
    }
    if (!prepare_voiceover(apng)) {
        panic("voiceover failed -- is Kokoro running at :8880?")
    }
    if (!convert_to_mp4_voiced(apng, music, daslang_exe, out_mp4, dur_s, fps, -13, 100)) {
        panic("convert failed")
    }
    to_log(LOG_INFO, "[record] tutorial 02 done\n")
}
