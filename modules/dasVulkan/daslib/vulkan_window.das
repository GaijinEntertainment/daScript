options gen2
options _comment_hygiene = true
options indenting = 4

// Windowing tier: bridge a native window handle to a VkSurfaceKHR, build a
// swapchain (format/present-mode/extent picked from surface caps, image views +
// framebuffers owned together), per-frame sync, and an acquire/record/submit/
// present helper. Pairs with daslib/vulkan_boost; the surface bridge takes raw
// void* handles (e.g. from glfwGetNativeWindow/Display) so there is no GLFW dep.

module vulkan_window shared

require vulkan public
require vulkan/vulkan_boost public
require daslib/defer
require math

let UINT64_MAX = 0xFFFFFFFFFFFFFFFFul

// ===== surface =====

//! wrap a native window/display (HWND / X11 Window+Display) as an owned VkSurfaceKHR.
//! the instance must enable the platform surface extension (glfwGetRequiredInstanceExtensions).
def public create_surface(instance : Instance; native_window : void?; native_display : void? = null) : SurfaceKHR {
    let raw = vk_surface_from_native(boost_value_to_vk(instance), native_window, native_display)
    if (raw == 0ul) {
        panic("failed to create vulkan surface from native window")
    }
    var s = vk_value_to_boost(unsafe(reinterpret<VkSurfaceKHR>(raw)))
    s._needs_delete = true
    s._instance = unsafe(reinterpret<uint64>(boost_value_to_vk(instance)))
    return <- s
}

//! does `queue_family` support presentation to `surface`?
def public queue_family_supports_present(phys : VkPhysicalDevice; queue_family : uint; surface : SurfaceKHR) : bool {
    var supported = 0u
    vk_check(vkGetPhysicalDeviceSurfaceSupportKHR(phys, queue_family, boost_value_to_vk(surface), supported), null)
    return supported != 0u
}

// ===== swapchain =====

//! swapchain + its per-image views and framebuffers, owned together
struct public Swapchain {
    handle : SwapchainKHR
    format : VkFormat
    color_space : VkColorSpaceKHR
    width : int
    height : int
    images : array<VkImage>            //!< owned by the swapchain object itself; not destroyed here
    views : array<ImageView>          //!< created + owned here
    framebuffers : array<Framebuffer> //!< created + owned here (after build_swapchain_framebuffers)
    readback_supported : bool         //!< true if the images carry TRANSFER_SRC (host readback is possible)
}

def public finalize(var sc : Swapchain) {
    for (i in range(length(sc.framebuffers))) {
        delete sc.framebuffers[i]
    }
    delete sc.framebuffers
    for (i in range(length(sc.views))) {
        delete sc.views[i]
    }
    delete sc.views
    unsafe {
        delete sc.images   // array of raw VkImage handles (owned by the swapchain); frees the array buffer only
    }
    delete sc.handle
}

def private choose_surface_format(phys : VkPhysicalDevice; surface : SurfaceKHR; var out_color_space : VkColorSpaceKHR&;
                                  preferred : VkFormat) : VkFormat {
    var count = 0u
    vk_check(vkGetPhysicalDeviceSurfaceFormatsKHR(phys, boost_value_to_vk(surface), count, null), null)
    var formats : array<VkSurfaceFormatKHR>
    formats |> resize(int(count))
    unsafe {
        vk_check(vkGetPhysicalDeviceSurfaceFormatsKHR(phys, boost_value_to_vk(surface), count, addr(formats[0])), null)
    }
    // 1. explicit preference: UNORM presents pass-through (UI/2D, no double sRGB encode), sRGB suits linear-lit 3D.
    if (preferred != VkFormat.UNDEFINED) {
        // sRGB-nonlinear colorspace first (predictable presentation), then the format with any colorspace.
        for (f in formats) {
            if (f.format == preferred && f.colorSpace == VkColorSpaceKHR.SRGB_NONLINEAR_KHR) {
                out_color_space = f.colorSpace
                return f.format
            }
        }
        for (f in formats) {
            if (f.format == preferred) {
                out_color_space = f.colorSpace
                return f.format
            }
        }
    }
    // 2. default: prefer B8G8R8A8_SRGB.
    for (f in formats) {
        if (f.format == VkFormat.B8G8R8A8_SRGB && f.colorSpace == VkColorSpaceKHR.SRGB_NONLINEAR_KHR) {
            out_color_space = f.colorSpace
            return f.format
        }
    }
    out_color_space = formats[0].colorSpace
    return formats[0].format
}

//! create a swapchain sized to the surface (FIFO present). Image views are made;
//! call build_swapchain_framebuffers afterwards to populate framebuffers.
def public create_swapchain(device : Device; phys : VkPhysicalDevice; surface : SurfaceKHR; width, height : int;
                            preferred_format : VkFormat = VkFormat.UNDEFINED) : Swapchain {
    let dev = boost_value_to_vk(device)
    var caps : VkSurfaceCapabilitiesKHR
    vk_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phys, boost_value_to_vk(surface), caps), null)

    var color_space = VkColorSpaceKHR.SRGB_NONLINEAR_KHR
    let format = choose_surface_format(phys, surface, color_space, preferred_format)
    var sc = Swapchain(color_space = color_space, format = format)

    var extent : VkExtent2D
    if (caps.currentExtent.width != 0xFFFFFFFFu) {
        extent = caps.currentExtent
    } else {
        extent.width = clamp(uint(width), caps.minImageExtent.width, caps.maxImageExtent.width)
        extent.height = clamp(uint(height), caps.minImageExtent.height, caps.maxImageExtent.height)
    }
    sc.width = int(extent.width)
    sc.height = int(extent.height)

    var image_count = caps.minImageCount + 1u
    if (caps.maxImageCount > 0u && image_count > caps.maxImageCount) {
        image_count = caps.maxImageCount
    }

    var ci = SwapchainCreateInfoKHR(surface = weak_copy(surface), minImageCount = image_count,
        imageFormat = sc.format, imageColorSpace = sc.color_space, imageExtent = extent, imageArrayLayers = 1u)
    ci.imageUsage.color_attachment = true
    ci.imageUsage.transfer_dst = true   // the blit-present path (present_frame + vkCmdBlitImage) writes the swapchain image as a transfer dst
    // readback path (vulkan_live screenshot/record) copies the presented image to a host buffer -- only
    // when the surface advertises it, since some WSI implementations don't and would reject the swapchain.
    sc.readback_supported = caps.supportedUsageFlags.transfer_src
    if (sc.readback_supported) {
        ci.imageUsage.transfer_src = true
    }
    ci.imageSharingMode = VkSharingMode.EXCLUSIVE
    ci.preTransform = caps.currentTransform
    ci.compositeAlpha.opaque_khr = true
    ci.presentMode = VkPresentModeKHR.FIFO_KHR
    ci.clipped = 1u
    sc.handle <- create_swapchain_k_h_r(device, ci)

    // fetch the swapchain images
    var got = 0u
    vk_check(vkGetSwapchainImagesKHR(dev, boost_value_to_vk(sc.handle), got, null), null)
    sc.images |> resize(int(got))
    unsafe {
        vk_check(vkGetSwapchainImagesKHR(dev, boost_value_to_vk(sc.handle), got, addr(sc.images[0])), null)
    }

    // one image view per image
    sc.views |> reserve(length(sc.images))
    for (img in sc.images) {
        var vci = ImageViewCreateInfo(image = vk_value_to_boost(img), viewType = VkImageViewType._2D, format = sc.format)
        vci.subresourceRange.aspectMask.color = true
        vci.subresourceRange.levelCount = 1u
        vci.subresourceRange.layerCount = 1u
        sc.views |> emplace(create_image_view(device, vci))
    }
    return <- sc
}

//! rebuild the swapchain + framebuffers at a new size (window resize / out-of-date
//! present). Waits for the device to idle, then replaces `sc` in place (the move-assign
//! finalizes the old swapchain's framebuffers, views and handle).
def public recreate_swapchain(var sc : Swapchain; device : Device; phys : VkPhysicalDevice;
                              surface : SurfaceKHR; render_pass : RenderPass; width, height : int) {
    vkDeviceWaitIdle(boost_value_to_vk(device))
    let keep_format = sc.format     // preserve the format across resize (the render pass was built for it)
    delete sc       // release the old swapchain before making a new one (the surface is app-owned)
    sc <- create_swapchain(device, phys, surface, width, height, keep_format)
    build_swapchain_framebuffers(device, sc, render_pass)
}

//! build one framebuffer per swapchain image view against `render_pass`
def public build_swapchain_framebuffers(device : Device; var sc : Swapchain; render_pass : RenderPass) {
    sc.framebuffers |> reserve(length(sc.views))
    for (i in range(length(sc.views))) {
        var fbci = FramebufferCreateInfo(renderPass = weak_copy(render_pass),
            pAttachments <- [weak_copy(sc.views[i])], width = uint(sc.width), height = uint(sc.height), layers = 1u)
        sc.framebuffers |> emplace(create_framebuffer(device, fbci))
    }
}

// ===== per-frame sync =====

struct public FrameSync {
    image_available : Semaphore
    render_finished : Semaphore
}

def public finalize(var f : FrameSync) {
    delete f.image_available
    delete f.render_finished
}

def public create_frame_sync(device : Device) : FrameSync {
    var f = FrameSync(image_available <- create_semaphore(device, SemaphoreCreateInfo()),
        render_finished <- create_semaphore(device, SemaphoreCreateInfo()))
    return <- f
}

// ===== draw =====

//! set a full-extent viewport + scissor on a pipeline built with dynamic viewport state
def private set_dynamic_viewport(cmd : CommandBuffer; width, height : int) {
    var vp = Viewport(width = float(width), height = float(height), maxDepth = 1.0f)
    var vps : array<Viewport>
    vps |> push(vp)
    cmd_set_viewport(cmd, 0u, vps)
    delete vps

    var sci : Rect2D
    sci.extent.width = uint(width)
    sci.extent.height = uint(height)
    var scis : array<Rect2D>
    scis |> push(sci)
    cmd_set_scissor(cmd, 0u, scis)
    delete scis
}

//! acquire -> record (render pass + user block) -> submit -> present, then wait idle. Returns false if
//! the swapchain is out of date (caller should recreate). `dynamic_viewport` sets a full-extent
//! viewport+scissor each frame — pairs with create_graphics_pipeline_simple(..., dynamic_viewport=true).
def public draw_frame(device : Device; queue : VkQueue; sc : Swapchain; render_pass : RenderPass;
                      pool : CommandPool; sync : FrameSync; clear : VkClearValue; dynamic_viewport : bool = false;
                      blk : block<(cmd : CommandBuffer) : void>) : bool {
    let dev = boost_value_to_vk(device)

    var image_index = 0u
    let acquired = vkAcquireNextImageKHR(dev, boost_value_to_vk(sc.handle), UINT64_MAX,
        boost_value_to_vk(sync.image_available), null, image_index)
    if (acquired == VkResult.ERROR_OUT_OF_DATE_KHR) {
        return false
    }

    var ai = VkCommandBufferAllocateInfo()
    ai.commandPool = boost_value_to_vk(pool)
    ai.level = VkCommandBufferLevel.PRIMARY
    ai.commandBufferCount = 1u
    var raw_cmd : VkCommandBuffer
    unsafe {
        vk_check(vkAllocateCommandBuffers(dev, ai, addr(raw_cmd)), null)
    }
    let cmd = vk_value_to_boost(raw_cmd)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw_cmd, begin), null)
    record_render_pass(cmd, render_pass, sc.framebuffers[int(image_index)],
            full_area(sc.width, sc.height), clear) {
        if (dynamic_viewport) {
            set_dynamic_viewport(cmd, sc.width, sc.height)
        }
        invoke(blk, cmd)
    }
    vk_check(vkEndCommandBuffer(raw_cmd), null)

    var wait_sem = boost_value_to_vk(sync.image_available)
    var signal_sem = boost_value_to_vk(sync.render_finished)
    var wait_stage : VkPipelineStageFlags
    wait_stage.color_attachment_output = true
    var submit = VkSubmitInfo()
    submit.waitSemaphoreCount = 1u
    submit.commandBufferCount = 1u
    submit.signalSemaphoreCount = 1u
    unsafe {
        submit.pWaitSemaphores = addr(wait_sem)
        submit.pWaitDstStageMask = addr<uint?>(wait_stage)
        submit.pCommandBuffers = addr(raw_cmd)
        submit.pSignalSemaphores = addr(signal_sem)
        vk_check(vkQueueSubmit(queue, 1u, addr(submit), null), null)
    }

    var sc_handle = boost_value_to_vk(sc.handle)
    var present = VkPresentInfoKHR()
    present.waitSemaphoreCount = 1u
    present.swapchainCount = 1u
    var presented : VkResult
    unsafe {
        present.pWaitSemaphores = addr(signal_sem)
        present.pSwapchains = addr(sc_handle)
        present.pImageIndices = addr(image_index)
        presented = vkQueuePresentKHR(queue, present)
    }
    vk_check(vkQueueWaitIdle(queue), null)
    unsafe {
        vkFreeCommandBuffers(dev, boost_value_to_vk(pool), 1u, addr(raw_cmd))
    }
    return presented != VkResult.ERROR_OUT_OF_DATE_KHR
}

//! acquire -> record a RAW command block (no render pass) -> submit -> present, then wait idle. Sibling
//! of draw_frame; the block gets (cmd, acquired VkImage, index) and MUST leave the image — UNDEFINED on
//! entry — in `PRESENT_SRC_KHR`. Waits at the transfer stage. Returns false if the swapchain is stale.
def public present_frame(device : Device; queue : VkQueue; sc : Swapchain; pool : CommandPool;
                         sync : FrameSync; blk : block<(cmd : CommandBuffer; target : VkImage; image_index : uint) : void>) : bool {
    let dev = boost_value_to_vk(device)

    var image_index = 0u
    let acquired = vkAcquireNextImageKHR(dev, boost_value_to_vk(sc.handle), UINT64_MAX,
        boost_value_to_vk(sync.image_available), null, image_index)
    if (acquired == VkResult.ERROR_OUT_OF_DATE_KHR) {
        return false
    }

    var ai = VkCommandBufferAllocateInfo()
    ai.commandPool = boost_value_to_vk(pool)
    ai.level = VkCommandBufferLevel.PRIMARY
    ai.commandBufferCount = 1u
    var raw_cmd : VkCommandBuffer
    unsafe {
        vk_check(vkAllocateCommandBuffers(dev, ai, addr(raw_cmd)), null)
    }
    let cmd = vk_value_to_boost(raw_cmd)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw_cmd, begin), null)
    invoke(blk, cmd, sc.images[int(image_index)], image_index)
    vk_check(vkEndCommandBuffer(raw_cmd), null)

    var wait_sem = boost_value_to_vk(sync.image_available)
    var signal_sem = boost_value_to_vk(sync.render_finished)
    var wait_stage : VkPipelineStageFlags
    wait_stage.transfer = true
    var submit = VkSubmitInfo()
    submit.waitSemaphoreCount = 1u
    submit.commandBufferCount = 1u
    submit.signalSemaphoreCount = 1u
    unsafe {
        submit.pWaitSemaphores = addr(wait_sem)
        submit.pWaitDstStageMask = addr<uint?>(wait_stage)
        submit.pCommandBuffers = addr(raw_cmd)
        submit.pSignalSemaphores = addr(signal_sem)
        vk_check(vkQueueSubmit(queue, 1u, addr(submit), null), null)
    }

    var sc_handle = boost_value_to_vk(sc.handle)
    var present = VkPresentInfoKHR()
    present.waitSemaphoreCount = 1u
    present.swapchainCount = 1u
    var presented : VkResult
    unsafe {
        present.pWaitSemaphores = addr(signal_sem)
        present.pSwapchains = addr(sc_handle)
        present.pImageIndices = addr(image_index)
        presented = vkQueuePresentKHR(queue, present)
    }
    vk_check(vkQueueWaitIdle(queue), null)
    unsafe {
        vkFreeCommandBuffers(dev, boost_value_to_vk(pool), 1u, addr(raw_cmd))
    }
    return presented != VkResult.ERROR_OUT_OF_DATE_KHR
}
