options gen2
options _comment_hygiene = true
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module vulkan_live shared public

//! Vulkan live utilities for daslang-live -- the mirror of opengl_live.
//!
//! Provides the same live commands for a windowed Vulkan app:
//!   screenshot     -- copy the last-presented swapchain image to a PNG
//!   vk_stats       -- per-frame render statistics
//!   record_start/stop/status -- streaming APNG capture
//!
//! Vulkan has no ambient "current framebuffer" the way GL does, so the app hands this module its
//! device + swapchain once via `vk_live_set_target` (the analog of glfw_live owning `live_window`).
//! This module OWNS NOTHING -- it never creates or destroys the window / device / swapchain; it only
//! reads them. Capture reads the swapchain image directly (it carries TRANSFER_SRC usage when the
//! surface advertises it), so the app keeps rendering straight to the swapchain.
//!
//! Usage in your script:
//!   require vulkan/vulkan_live
//!   // after creating device/queue/pool/swapchain:
//!   vk_live_set_target(device, phys, queue, pool, swap)
//!   // per frame, instead of draw_frame(...):
//!   vk_live_draw_frame(render_pass, sync, clear, true) $(cmd) { /* record draws */ }

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_window public
require stbimage
require daslib/json
require daslib/json_boost
require live/live_commands
require live_host
require math

// ============================================================================
// Registered target (option b -- the app hands us the objects; we own nothing)
// ============================================================================

struct private VkLiveTarget {
    device : Device                 // weak copy -- non-owning
    phys   : VkPhysicalDevice
    queue  : VkQueue
    pool   : CommandPool            // weak copy -- non-owning
    swap   : Swapchain?             // pointer to the app's swapchain (sees in-place recreate)
    valid  : bool
}

var private g : VkLiveTarget

// Index of the most recently presented swapchain image (set by vk_live_draw_frame); -1 = none yet.
var private g_last_index : int = -1
var private g_frames_rendered : int = 0

// Host-visible buffer the swapchain image is copied into for readback; grown on demand.
var private g_readback : HostBuffer
var private g_readback_cap : uint64 = 0ul

def public vk_live_set_target(device : Device; phys : VkPhysicalDevice; queue : VkQueue;
                              pool : CommandPool; var swap : Swapchain) {
    //! Register the objects screenshot / recording read from; call once after creating the swapchain.
    //! `swap` is stored BY POINTER — in-place `recreate_swapchain` is picked up free, but `swap` MUST
    //! outlive the app's run or the pointer dangles. Handles stored weak / by value; never destroyed here.
    g.device = weak_copy(device)
    g.pool   = weak_copy(pool)
    // phys / queue are non-owning raw handles; strip the caller's `let`-constness to store them.
    unsafe {
        g.phys  = reinterpret<VkPhysicalDevice>(phys)
        g.queue = reinterpret<VkQueue>(queue)
        g.swap  = addr(swap)
    }
    g.valid = true
    g_last_index = -1       // a freshly registered target has no presented frame yet
    g_frames_rendered = 0
    if (g_readback_cap > 0ul) {   // drop a readback buffer from a previous target (possibly a different device)
        delete g_readback
        g_readback_cap = 0ul
    }
}

def private is_bgra(fmt : VkFormat) : bool =>
    fmt == VkFormat.B8G8R8A8_SRGB || fmt == VkFormat.B8G8R8A8_UNORM

//! Readback assumes a tightly packed 4-byte 8888 surface (the formats choose_surface_format picks);
//! anything wider (R16G16B16A16, A2B10G10R10, ...) would mis-size the host buffer + swizzle.
def private is_rgba8(fmt : VkFormat) : bool =>
    is_bgra(fmt) || fmt == VkFormat.R8G8B8A8_SRGB || fmt == VkFormat.R8G8B8A8_UNORM

def private ensure_readback(nbytes : uint64) {
    if (g_readback_cap >= nbytes) {
        return
    }
    if (g_readback_cap > 0ul) {
        delete g_readback
    }
    g_readback <- create_host_buffer(g.device, g.phys, nbytes)   // 3-arg overload = transfer_dst usage
    g_readback_cap = nbytes
}

//! Copy swapchain image `image_index` (in PRESENT_SRC, GPU idle) into a host buffer and return it as
//! tightly packed top-row-first RGBA8 (BGRA surfaces are swizzled). The image is left back in
//! PRESENT_SRC so it stays presentable.
def private readback_swapchain_image(image_index : int) : array<uint8> {
    var pixels : array<uint8>
    let sc & = unsafe(*g.swap)
    // give up (empty) on a stale index, an unsupported non-8888 format, or a no-readback swapchain
    if (image_index < 0 || image_index >= length(sc.images) || !is_rgba8(sc.format) || !sc.readback_supported) {
        return <- pixels
    }
    let w = sc.width
    let h = sc.height
    let nbytes = uint64(w) * uint64(h) * 4ul   // widen before multiply
    ensure_readback(nbytes)
    let img = vk_value_to_boost(sc.images[image_index])   // non-owning wrapper around the raw VkImage
    run_cmd_sync(g.device, g.pool, g.queue) $(cmd) {
        let none : VkAccessFlags
        var to_read : VkAccessFlags; to_read.transfer_read = true
        var top : VkPipelineStageFlags; top.top_of_pipe = true
        var bottom : VkPipelineStageFlags; bottom.bottom_of_pipe = true
        var xfer : VkPipelineStageFlags; xfer.transfer = true
        transition_image(cmd, img, VkImageLayout.PRESENT_SRC_KHR, VkImageLayout.TRANSFER_SRC_OPTIMAL,
            none, to_read, top, xfer)
        copy_image_to_buffer(cmd, img, g_readback, w, h)
        transition_image(cmd, img, VkImageLayout.TRANSFER_SRC_OPTIMAL, VkImageLayout.PRESENT_SRC_KHR,
            to_read, none, xfer, bottom)
    }
    let swizzle = is_bgra(sc.format)
    let n = int(nbytes)
    map_memory_to_array(g.device, g_readback.memory, nbytes) $(m) {
        pixels |> resize(n)
        if (swizzle) {
            var i = 0
            while (i < n) {
                pixels[i]     = m[i + 2]   // R <- B
                pixels[i + 1] = m[i + 1]   // G
                pixels[i + 2] = m[i]       // B <- R
                pixels[i + 3] = m[i + 3]   // A
                i += 4
            }
        } else {
            unsafe { memcpy(addr(pixels[0]), addr(m[0]), n) }
        }
    }
    return <- pixels
}

// ============================================================================
// Frame (drop-in for vulkan_window's draw_frame, with integrated capture)
// ============================================================================

//! Acquire on the registered target; -1 when the swapchain is out of date or lost (the caller
//! recreates and keeps calling), so a garbage index never reaches the views or framebuffers.
def private live_acquire(sync : FrameSync) : int {
    if (!g.valid || g.swap == null) {
        panic("vk_live_draw_frame: no target -- call vk_live_set_target first")
    }
    let sc & = unsafe(*g.swap)
    var image_index = 0u
    if (!frame_acquired(acquire_frame_image(g.device, sc, sync, image_index))) {
        return -1
    }
    return int(image_index)
}

//! Records a present's outcome for the live session - last index, frame count, the APNG capture when the
//! recorder is on - and says whether the frame reached the screen: SUBOPTIMAL is still a usable present,
//! out-of-date or any other error is false so the caller recreates.
def private track_present(presented : VkResult; image_index : int) : bool {
    let ok = presented == VkResult.SUCCESS || presented == VkResult.SUBOPTIMAL_KHR
    if (ok) {
        g_last_index = image_index
        g_frames_rendered++
        if (recorder_active) {
            record_capture_frame(image_index)
        }
    }
    return ok
}

def public vk_live_draw_frame(sync : FrameSync; clear : VkClearValue;
                              dynamic_viewport : bool; blk : block<(cmd : CommandBuffer) : void>) : bool {
    //! vulkan_window's render-pass-free `draw_frame` on the `vk_live_set_target` target, which also
    //! tracks the presented image and feeds the APNG writer. False = swapchain out-of-date/lost
    //! (recreate, keep calling); other GPU errors vk_check-panic.
    let image_index = live_acquire(sync)
    if (image_index < 0) {
        return false
    }
    let sc & = unsafe(*g.swap)
    var raw_cmd = begin_one_time_commands(g.device, g.pool)
    record_swapchain_rendering(vk_value_to_boost(raw_cmd), sc, image_index, clear, dynamic_viewport, blk)
    let presented = submit2_and_present(g.device, g.queue, sc, g.pool, sync, raw_cmd, uint(image_index),
        VkPipelineStageFlags2.color_attachment_output)
    return track_present(presented, image_index)
}

def public vk_live_draw_frame(render_pass : RenderPass; sync : FrameSync; clear : VkClearValue;
                              dynamic_viewport : bool; blk : block<(cmd : CommandBuffer) : void>) : bool {
    //! `vk_live_draw_frame` over a render pass: `blk` records inside the pass on the image's framebuffer
    //! (the app ran `build_swapchain_framebuffers`); drop-in for vulkan_window's render-pass `draw_frame`.
    let image_index = live_acquire(sync)
    if (image_index < 0) {
        return false
    }
    let sc & = unsafe(*g.swap)
    var raw_cmd = begin_one_time_commands(g.device, g.pool)
    record_swapchain_render_pass(vk_value_to_boost(raw_cmd), sc, render_pass, image_index, clear, dynamic_viewport, blk)
    var wait_stage : VkPipelineStageFlags
    wait_stage.color_attachment_output = true
    let presented = submit_and_present(g.device, g.queue, sc, g.pool, sync, raw_cmd, uint(image_index), wait_stage)
    return track_present(presented, image_index)
}

// ============================================================================
// Render statistics (mirror of opengl_live's gl_stats)
// ============================================================================

//! Per-frame render statistics. Reset at the start of each frame via ``vk_stats_reset()``, accumulate
//! with ``vk_stats_draw()``, query with the ``vk_stats`` live command.
struct VkStats {
    draw_calls : int
    triangles  : int
    vertices   : int
}

var private vk_render_stats : VkStats

def public vk_stats_reset() {
    //! Reset render statistics. Call at the start of each frame.
    vk_render_stats = VkStats()
}

def public vk_stats_draw(index_count, instance_count : int) {
    //! Record an indexed draw. Call after each ``cmd_draw_indexed``.
    vk_render_stats.draw_calls++
    let verts = index_count * max(instance_count, 1)
    vk_render_stats.vertices += verts
    vk_render_stats.triangles += verts / 3
}

struct VkStatsResult {
    draw_calls : int
    triangles  : int
    vertices   : int
    fps        : float
}

[live_command(description="Vulkan render statistics for the last frame")]
def vk_stats(_input : JsonValue?) : JsonValue? {
    let dt = get_dt()
    return JV(VkStatsResult(
        draw_calls = vk_render_stats.draw_calls,
        triangles  = vk_render_stats.triangles,
        vertices   = vk_render_stats.vertices,
        fps        = dt > 0.0f ? 1.0f / dt : 0.0f
    ))
}

// ============================================================================
// Screenshot
// ============================================================================

struct ScreenshotArgs {
    file : string = "screenshot.png"
}

struct ScreenshotResult {
    saved  : string
    width  : int
    height : int
}

def public vk_live_save_screenshot(file : string) : bool {
    //! Write the last-presented swapchain image to `file` (PNG). Synchronous: reads the image that
    //! is currently on screen. Returns false if no frame has been rendered yet or no target is set.
    //! Callable directly from a plain `main` loop, or via the ``screenshot`` live command.
    if (!g.valid || g.swap == null || g_last_index < 0) {
        return false
    }
    let sc & = unsafe(*g.swap)
    let w = sc.width
    let h = sc.height
    if (w <= 0 || h <= 0 || g_last_index >= length(sc.images)) {   // stale index after an in-place recreate
        return false
    }
    var pixels <- readback_swapchain_image(g_last_index)
    if (empty(pixels)) {        // readback unavailable (stale index / unsupported format)
        return false
    }
    let ok = stbi_write_png(file, w, h, 4, unsafe(addr(pixels[0])), w * 4) != 0
    delete pixels
    return ok
}

[live_command(description="Save the last-presented swapchain image to PNG. Args: file")]
def screenshot(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<ScreenshotArgs>)
    var fname = args.file
    if (empty(fname)) {
        fname = "screenshot.png"
    }
    if (!g.valid || g.swap == null) {
        return JV((error = "no target -- call vk_live_set_target"))
    }
    if (g_last_index < 0) {
        return JV((error = "no frame rendered yet"))
    }
    if (!vk_live_save_screenshot(fname)) {
        return JV((error = "screenshot failed", file = fname))
    }
    let sc & = unsafe(*g.swap)
    return JV(ScreenshotResult(saved = fname, width = sc.width, height = sc.height))
}

// ===== APNG recording =====
// Streaming APNG via dasStbImage's stbi_apng_begin/frame/end. Vulkan readback here is already
// synchronous (vk_live_draw_frame waits idle), so record_capture_frame is driven straight off it.

struct private RecorderState {
    writer           : void?     // StbiApngWriter* opaque handle
    fps              : float
    frame_interval_s : float
    frame_ms         : int
    max_seconds      : float
    start_t          : float
    frames_written   : int
    width            : int
    height           : int
    file             : string
    prev_png_level   : int
}

var private recorder : RecorderState
var private recorder_active : bool = false

struct RecordStartArgs {
    @optional file        : string = "record.apng"
    @optional fps         : float  = 30.0f
    @optional max_seconds : float  = 60.0f   // 0 = no cap
}

struct RecordStartResult {
    file        : string
    fps         : float
    width       : int
    height      : int
    max_seconds : float
}

[live_command(description="Begin APNG recording. Args: file, fps, max_seconds.")]
def record_start(input : JsonValue?) : JsonValue? {
    if (recorder_active) {
        return JV((error = "already recording"))
    }
    if (!g.valid || g.swap == null) {
        return JV((error = "no target -- call vk_live_set_target"))
    }
    let args = from_JV(input, type<RecordStartArgs>)
    let sc & = unsafe(*g.swap)
    let w = sc.width
    let h = sc.height
    if (w <= 0 || h <= 0) {
        return JV((error = "no framebuffer"))
    }

    // stbi's default PNG level is high (slow). Level 4 is the tutorial sweet spot (mirrors
    // opengl_live); restored at record_stop so unrelated PNG writes aren't throttled.
    let prev_png_level = stbi_write_get_png_compression_level()
    stbi_write_set_png_compression_level(4)
    let writer_ptr = stbi_apng_begin(args.file, w, h, 4)
    if (writer_ptr == null) {
        stbi_write_set_png_compression_level(prev_png_level)
        return JV((error = "could not open file", file = args.file))
    }

    let effective_fps = max(args.fps, 1.0f)
    recorder.writer           = writer_ptr
    recorder.fps              = effective_fps
    recorder.frame_interval_s = 1.0f / effective_fps
    recorder.frame_ms         = int(1000.0f / effective_fps)
    recorder.max_seconds      = args.max_seconds
    recorder.start_t          = get_uptime()
    recorder.frames_written   = 0
    recorder.width            = w
    recorder.height           = h
    recorder.file             = args.file
    recorder.prev_png_level   = prev_png_level
    recorder_active           = true
    return JV(RecordStartResult(
        file        = args.file,
        fps         = effective_fps,
        width       = w,
        height      = h,
        max_seconds = args.max_seconds
    ))
}

// Capture one frame into the APNG writer. Called from vk_live_draw_frame while recording. A resize
// mid-recording (swapchain dims != writer dims) stops the recording cleanly rather than feeding a
// mismatched frame.
def private record_capture_frame(image_index : int) {
    let sc & = unsafe(*g.swap)
    if (sc.width != recorder.width || sc.height != recorder.height) {
        record_stop(null)
        return
    }
    if (recorder.max_seconds > 0.0f && (get_uptime() - recorder.start_t) >= recorder.max_seconds) {
        record_stop(null)
        return
    }
    var pixels <- readback_swapchain_image(image_index)
    if (empty(pixels)) {        // readback unavailable -- stop recording cleanly
        record_stop(null)
        return
    }
    let ok = stbi_apng_frame(recorder.writer, unsafe(addr(pixels[0])), recorder.width * 4, recorder.frame_ms)
    delete pixels
    if (ok == 0) {
        record_stop(null)
        return
    }
    recorder.frames_written++
}

struct RecordStopResult {
    saved      : string
    frames     : int
    dropped    : int
    duration_s : float
    ok         : bool
}

[live_command(description="Stop APNG recording. Returns saved path + frame count.")]
def record_stop(_input : JsonValue?) : JsonValue? {
    if (!recorder_active) {
        return JV((error = "not recording"))
    }
    let dropped = stbi_apng_dropped(recorder.writer)
    let ok      = stbi_apng_end(recorder.writer)
    stbi_write_set_png_compression_level(recorder.prev_png_level)
    let elapsed = get_uptime() - recorder.start_t
    let saved   = recorder.file
    let frames  = recorder.frames_written
    recorder.writer = null
    recorder_active = false
    return JV(RecordStopResult(
        saved      = saved,
        frames     = frames,
        dropped    = dropped,
        duration_s = elapsed,
        ok         = ok != 0
    ))
}

struct RecordStatusResult {
    active    : bool
    file      : string
    frames    : int
    elapsed_s : float
    fps       : float
    width     : int
    height    : int
    dropped   : int
}

[live_command(description="Recording status -- active state, frame count, elapsed.")]
def record_status(_input : JsonValue?) : JsonValue? {
    var dropped = 0
    if (recorder_active) {
        dropped = stbi_apng_dropped(recorder.writer)
    }
    return JV(RecordStatusResult(
        active    = recorder_active,
        file      = recorder.file,
        frames    = recorder.frames_written,
        elapsed_s = recorder_active ? get_uptime() - recorder.start_t : 0.0f,
        fps       = recorder.fps,
        width     = recorder.width,
        height    = recorder.height,
        dropped   = dropped
    ))
}
