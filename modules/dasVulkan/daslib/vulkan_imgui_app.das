options gen2
options _comment_hygiene = true
options indenting = 4

// A windowed Dear ImGui application harness on Vulkan -- the Vulkan-backed mirror of dasImgui's
// imgui_harness. It absorbs all the GLFW + Vulkan boilerplate (window, instance/device/swapchain,
// render pass, sync, the ImGui context + daslang theme + the vulkan_imgui_driver renderer) behind the
// same five lifecycle calls dasImgui apps already use:
//
//   harness_init / harness_begin_frame / harness_new_frame / harness_end_frame / harness_shutdown
//
// so a backend-agnostic dasImgui app ports to Vulkan by swapping `require imgui/imgui_harness` for
// `require vulkan/vulkan_imgui_app`. Apps that draw their own geometry behind the UI use the
// `harness_end_frame(scene)` overload to record into the same render pass as the UI. Apps that
// build their own Vulkan resources (pipelines, buffers, images) reach the harness-owned objects
// through the `harness_device()` / `harness_phys()` / `harness_queue()` / `harness_pool()` /
// `harness_render_pass()` accessors.
//
// Live-ready: the boost-v2 widget stack + imgui_live_core are re-exported and the boost synth tick
// runs in `harness_new_frame`, so a harness app started under daslang-live answers `imgui_snapshot`
// / `imgui_click` / `imgui_force_set` (and the vulkan_live `screenshot`) with no opt-in — the
// Vulkan mirror of what imgui_harness wires for GL. Windowed only (no headless backend); input is
// the GLFW backend (ImGui_ImplGlfw) -- mouse, keyboard, and scroll.

module vulkan_imgui_app shared public

options persistent_heap   // collectable heap so per-frame JsonValue churn (live commands) is reclaimed

// windowed backend first — mirrors imgui_harness's require ordering (live_api after the backend)
require imgui public
require imgui_app
require glfw/glfw_boost
require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_window public
require vulkan/vulkan_imgui_driver public
require vulkan/vulkan_live public
require imgui/imgui_theme_daslang

// live-host + boost-v2 widget stack — re-exported public so one `require vulkan/vulkan_imgui_app`
// gives a consumer the same surface a GL imgui_harness app gets. ?dashv: live_api is the HTTP
// live-command server (pulls dasHV); guarded so builds without dashv still compile.
require ?dashv live/live_api public
require live/live_commands public
require live/live_vars public
require live_host public
require imgui/imgui_live_core public
require imgui/imgui_boost_runtime public
require imgui/imgui_boost_v2 public
require imgui/imgui_widgets_builtin public
require imgui/imgui_containers_builtin public
require imgui/imgui_scope_builtin public
require imgui/imgui_id_builtin public
require imgui/imgui_style_builtin public
require imgui/imgui_layout_builtin public
require imgui/imgui_docking_builtin public
require imgui/imgui_drawlist_builtin public
require imgui/imgui_colors public

// ===== app state (one owning heap struct, held by a single pointer so teardown is explicit) =====

struct private VulkanApp {
    window        : GLFWwindow?
    instance      : Instance
    surface       : SurfaceKHR
    phys          : VkPhysicalDevice
    gfx           : uint
    device        : Device
    queue         : VkQueue
    pool          : CommandPool
    swap          : Swapchain
    render_pass   : RenderPass
    sync          : FrameSync
    igvk          : ImguiVkRenderer
    ctx           : ImGuiContext?
    shot_was_down : bool
    rt_enabled    : bool      // harness_init(ray_tracing=true) AND the device supports the RT pipeline
}

def private finalize(var a : VulkanApp) {
    unsafe {
        vkDeviceWaitIdle(boost_value_to_vk(a.device))
    }
    if (a.ctx != null) {
        ImGui_ImplGlfw_Shutdown()
        DestroyContext(a.ctx)
        a.ctx = null
    }
    unsafe {
        delete a.igvk           // GPU renderer (uses device)
        delete a.sync
        delete a.swap           // framebuffers + views + handle
        delete a.render_pass
        delete a.pool
        delete a.device
        delete a.surface
        delete a.instance
    }
    if (a.window != null) {
        glfwDestroyWindow(a.window)
        a.window = null
    }
}

var private g_app : VulkanApp?

// ===== lifecycle =====

def public harness_init(title : string; width : int = 800; height : int = 600; ray_tracing : bool = false) {
    //! Create window + Vulkan device/swapchain + ImGui context (daslang theme, JetBrains Mono) + the
    //! vulkan_imgui_driver renderer, and register the swapchain with vulkan_live. Idempotent; call from
    //! init(). `ray_tracing = true` requests the RT device but falls back SILENTLY — probe `harness_rt_supported()`.
    if (g_app != null) {
        return
    }
    if (volkInitialize() != 0) {
        panic("vulkan_imgui_app: no vulkan loader")
    }
    glfwInitVulkanLoader(vk_get_instance_proc_addr())
    if (glfwInit() == 0) {
        panic("vulkan_imgui_app: can't init glfw")
    }
    if (glfwVulkanSupported() == 0) {
        panic("vulkan_imgui_app: glfw reports no vulkan support")
    }
    glfwWindowHint(int(GLFW_CLIENT_API), GLFW_NO_API)
    glfwWindowHint(int(GLFW_RESIZABLE), GLFW_TRUE)
    var window = glfwCreateWindow(width, height, title, null, null)
    if (window == null) {
        panic("vulkan_imgui_app: can't create window")
    }

    var ext_count = 0u
    let glfw_exts = glfwGetRequiredInstanceExtensions(unsafe(addr(ext_count)))
    var inst_exts : array<string>
    inst_exts |> reserve(int(ext_count))
    for (i in range(int(ext_count))) {
        unsafe {
            inst_exts |> push(glfw_exts[i])
        }
    }
    var inscope instance <- create_instance(title, make_api_version(1u, 3u, 0u), inst_exts)
    volkLoadInstance(boost_value_to_vk(instance))
    var inscope surface <- create_surface(instance, glfwGetNativeWindow(window), glfwGetNativeDisplay())
    var phys = select_physical_device(instance)   // var: keep the handle pointee non-const for the struct field
    let gfx = select_graphics_queue_family(phys)
    if (!queue_family_supports_present(phys, gfx, surface)) {
        panic("vulkan_imgui_app: graphics queue family can't present")
    }
    let rt_enabled = ray_tracing && rt_supported(phys)
    var inscope device : Device
    if (rt_enabled) {
        device <- create_device_ray_tracing(phys, gfx, ["VK_KHR_swapchain"])
    } else {
        device <- create_device(phys, gfx, ["VK_KHR_swapchain"])
    }
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    // UNORM (pass-through) swapchain: a UI outputs already-sRGB colors, so the GPU must not sRGB-encode
    // them again (matches how the GL tools render).
    var inscope swap <- create_swapchain(device, phys, surface, width, height, VkFormat.B8G8R8A8_UNORM)
    var inscope render_pass <- create_render_pass_single_color(device, swap.format, VkImageLayout.PRESENT_SRC_KHR)
    build_swapchain_framebuffers(device, swap, render_pass)
    var inscope sync <- create_frame_sync(device)

    var ctx = CreateContext(null)
    load_daslang_font()
    apply_daslang_theme()
    var io & = unsafe(GetIO())
    io.BackendFlags |= ImGuiBackendFlags.RendererHasTextures
    io.BackendFlags |= ImGuiBackendFlags.RendererHasVtxOffset
    // GLFW platform backend: installs mouse/keyboard/scroll/char callbacks and feeds DisplaySize +
    // DisplayFramebufferScale each frame (replaces the hand-rolled mouse-only polling).
    ImGui_ImplGlfw_InitForVulkan(window, true)
    var inscope igvk <- imgui_vk_create(device, render_pass)

    g_app = new VulkanApp(window = window, instance <- instance, surface <- surface, phys = phys,
        gfx = gfx, device <- device, queue = queue, pool <- pool, swap <- swap, render_pass <- render_pass,
        sync <- sync, igvk <- igvk, ctx = ctx, shot_was_down = false, rt_enabled = rt_enabled)

    // register on the app's OWN swapchain field (not the moved-out local), so an in-place resize is seen
    vk_live_set_target(g_app.device, g_app.phys, g_app.queue, g_app.pool, g_app.swap)
}

def public harness_begin_frame() : bool {
    //! Poll events, handle minimize/resize, feed ImGui input. Returns false to skip this update
    //! (window closed -> request_exit; or minimized). Call from update().
    if (g_app == null) {
        return false
    }
    glfwPollEvents()
    if (glfwWindowShouldClose(g_app.window) != 0) {
        request_exit()
        return false
    }
    var fbw = 0
    var fbh = 0
    glfwGetFramebufferSize(g_app.window, fbw, fbh)
    if (fbw == 0 || fbh == 0) {
        glfwWaitEvents()
        return false
    }
    if (fbw != g_app.swap.width || fbh != g_app.swap.height) {
        recreate_swapchain(g_app.swap, g_app.device, g_app.phys, g_app.surface, g_app.render_pass, fbw, fbh)  // waits idle internally
    }
    // boost runtime frame open: advances the widget registry's frame clock (render-twice detection,
    // snapshot bookkeeping) — mirrors imgui_harness's begin_frame placement
    begin_frame()
    // Feeds DisplaySize/DisplayFramebufferScale, DeltaTime, and the polled mouse/keyboard/scroll events
    // (callbacks installed by InitForVulkan) into io.
    ImGui_ImplGlfw_NewFrame()
    // The GLFW backend advances its frame clock only inside NewFrame, which we skip while minimized,
    // so the first frame after a restore would otherwise see a huge DeltaTime. Clamp it (also absorbs
    // debugger-pause / long-hitch spikes).
    var io & = unsafe(GetIO())
    if (io.DeltaTime > 0.1f) {
        io.DeltaTime = 0.1f
    }
    // step the L1 synthetic-input coroutines (imgui_click et al) one frame — must run between the
    // GLFW backend's NewFrame (above) and NewFrame() in harness_new_frame
    advance_coroutines()
    return true
}

def public harness_new_frame() {
    //! Runs the boost synth tick — so synthetic input (`imgui_click`, `imgui_force_set`,
    //! `set_user_control`) works with no opt-in — then NewFrame(). The tick MUST sit between
    //! ImGui_ImplGlfw_NewFrame (in harness_begin_frame) and NewFrame(): the slot synth input drains in.
    imgui_synth_tick()
    NewFrame()
}

// recreate-on-out-of-date + the press-P screenshot, shared by both end_frame overloads
def private after_present(ok : bool) {
    if (!ok) {
        var fbw = 0
        var fbh = 0
        glfwGetFramebufferSize(g_app.window, fbw, fbh)
        if (fbw > 0 && fbh > 0) {
            recreate_swapchain(g_app.swap, g_app.device, g_app.phys, g_app.surface, g_app.render_pass, fbw, fbh)  // waits idle internally
        }
    }
    let down = glfwGetKey(g_app.window, GLFW_KEY_P) == GLFW_PRESS
    if (down && !g_app.shot_was_down) {
        vk_live_save_screenshot("screenshot.png")
    }
    g_app.shot_was_down = down
}

def public harness_end_frame() {
    //! Render the UI and present. Call after building the UI.
    harness_end_frame() $(_cmd) {}
}

def public harness_end_frame(scene : block<(cmd : CommandBuffer) : void>) {
    //! Render `scene` (custom geometry) then the UI on top, into one render pass, and present. Use this
    //! when the app draws its own Vulkan geometry behind the ImGui UI.
    end_of_frame()
    Render()
    imgui_vk_update_textures(g_app.igvk, g_app.device, g_app.phys, g_app.queue, g_app.pool)
    let ok = vk_live_draw_frame(g_app.render_pass, g_app.sync,
                                clear_color(0.10f, 0.11f, 0.14f, 1.0f), true) $(cmd) {
        invoke(scene, cmd)
        imgui_vk_render(g_app.igvk, g_app.device, g_app.phys, cmd)
    }
    after_present(ok)
}

def public harness_shutdown() {
    //! Tear down everything (waits for the device to idle first). Call from shutdown().
    if (g_app == null) {
        return
    }
    unsafe {
        delete g_app
    }
    g_app = null
    glfwTerminate()
}

// ===== harness-owned Vulkan objects — non-owning views for apps that build their own resources =====

def private app_or_panic : VulkanApp ? {
    if (g_app == null) {
        panic("vulkan_imgui_app: harness accessor called before harness_init")
    }
    return g_app
}

def public harness_device : Device {
    //! Non-owning view of the harness device (weak_copy — never delete it).
    return weak_copy(app_or_panic().device)
}

def public harness_phys : VkPhysicalDevice {
    return app_or_panic().phys
}

def public harness_queue : VkQueue {
    return app_or_panic().queue
}

def public harness_pool : CommandPool {
    //! Non-owning view of the harness command pool (weak_copy — never delete it).
    return weak_copy(app_or_panic().pool)
}

def public harness_render_pass : RenderPass {
    //! Non-owning view of the swapchain render pass (for pipelines drawn inside harness_end_frame's scene block).
    return weak_copy(app_or_panic().render_pass)
}

def public harness_rt_supported : bool {
    //! True when harness_init was asked for ray_tracing AND the device actually has the RT pipeline.
    return g_app != null && g_app.rt_enabled
}

def public harness_size : int2 {
    //! Current swapchain extent (the render-pass target size), e.g. for projection aspect.
    let a = app_or_panic()
    return int2(a.swap.width, a.swap.height)
}

// ===== compatibility shims (match imgui_harness so apps port unchanged) =====

def public harness_is_headless() : bool {
    //! Always false -- the Vulkan harness is windowed-only for now.
    return false
}

def public harness_frame_count() : int {
    //! ImGui's frame counter.
    return GetFrameCount()
}
