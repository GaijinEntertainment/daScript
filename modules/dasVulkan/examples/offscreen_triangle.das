options gen2
options _comment_hygiene = true

require vulkan
require triangle_example_shaders
require daslib/fio
require math

let WIDTH = 256
let HEIGHT = 256
let FORMAT = VkFormat.R8G8B8A8_UNORM

def fail(msg : string) {
    to_log(LOG_ERROR, "FAIL: {msg}\n")
    panic(msg)
}

//! read the (r,g,b) of pixel (x,y) from a tightly-packed RGBA byte buffer
def sample(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * WIDTH + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}

//! the clear color is (0.1, 0.1, 0.15) -> roughly (26, 26, 38) in 8-bit
def is_clear(c : int3) : bool {
    return abs(c.x - 26) <= 8 && abs(c.y - 26) <= 8 && abs(c.z - 38) <= 8
}

def vcheck(what : string; ok : bool) : int {
    print("  [{ok ? "PASS" : "FAIL"}] {what}\n")
    return ok ? 0 : 1
}

def check(r : VkResult; msg : string) {
    if (r != VkResult.SUCCESS) {
        fail("{msg}: {r}")
    }
}

def find_memory_type(phys : VkPhysicalDevice; type_bits : uint; want : VkMemoryPropertyFlags) : uint {
    var mp : VkPhysicalDeviceMemoryProperties
    vkGetPhysicalDeviceMemoryProperties(phys, mp)
    for (i in range(int(mp.memoryTypeCount))) {
        let supported = (type_bits & (1u << uint(i))) != 0u
        let has = (uint(mp.memoryTypes[i].propertyFlags) & uint(want)) == uint(want)
        if (supported && has) {
            return uint(i)
        }
    }
    fail("no suitable memory type")
    return 0u
}

// SPIR-V words (array<uint>) straight from dasSpirv: codeSize is in BYTES (4 per word), pCode is the
// uint32 word data.
def create_shader_module(device : VkDevice; var code : array<uint>) : VkShaderModule {
    var sci : VkShaderModuleCreateInfo
    sci.sType = VkStructureType.SHADER_MODULE_CREATE_INFO
    sci.codeSize = uint64(length(code) * 4)
    unsafe {
        sci.pCode = addr(code[0])
    }
    var sm : VkShaderModule
    check(vkCreateShaderModule(device, sci, null, sm), "create shader module")
    return sm
}

[export]
def main() : int {   // nolint:STYLE038 - flat example scaffold
    if (volkInitialize() != 0) {
        to_log(LOG_ERROR, "no Vulkan loader\n")
        return 1
    }

    // ----- instance -----
    var ai : VkApplicationInfo
    ai.sType = VkStructureType.APPLICATION_INFO
    ai.apiVersion = (1u << 22u) | (3u << 12u)
    var ici : VkInstanceCreateInfo
    ici.sType = VkStructureType.INSTANCE_CREATE_INFO
    unsafe {
        ici.pApplicationInfo = addr(ai)
    }
    var instance : VkInstance
    check(vkCreateInstance(ici, null, instance), "create instance")
    volkLoadInstance(instance)

    // ----- physical device + graphics queue family -----
    var count = 0u
    vkEnumeratePhysicalDevices(instance, count, null)
    if (count == 0u) {
        fail("no physical devices")
    }
    var devices : array<VkPhysicalDevice>
    devices |> resize(int(count))
    unsafe {
        vkEnumeratePhysicalDevices(instance, count, addr(devices[0]))
    }
    let phys = devices[0]

    var qcount = 0u
    vkGetPhysicalDeviceQueueFamilyProperties(phys, qcount, null)
    var qprops : array<VkQueueFamilyProperties>
    qprops |> resize(int(qcount))
    unsafe {
        vkGetPhysicalDeviceQueueFamilyProperties(phys, qcount, addr(qprops[0]))
    }
    var gfx = -1
    for (i in range(int(qcount))) {
        if (qprops[i].queueFlags.graphics) {
            gfx = i
            break
        }
    }
    if (gfx < 0) {
        fail("no graphics queue")
    }

    // ----- logical device + queue -----
    var priority = 1.0f
    var qci : VkDeviceQueueCreateInfo
    qci.sType = VkStructureType.DEVICE_QUEUE_CREATE_INFO
    qci.queueFamilyIndex = uint(gfx)
    qci.queueCount = 1u
    unsafe {
        qci.pQueuePriorities = addr(priority)
    }
    var dci : VkDeviceCreateInfo
    dci.sType = VkStructureType.DEVICE_CREATE_INFO
    dci.queueCreateInfoCount = 1u
    unsafe {
        dci.pQueueCreateInfos = addr(qci)
    }
    var device : VkDevice
    check(vkCreateDevice(phys, dci, null, device), "create device")
    volkLoadDevice(device)
    var queue : VkQueue
    vkGetDeviceQueue(device, uint(gfx), 0u, queue)

    // ----- color image (render target) -----
    var img_ci : VkImageCreateInfo
    img_ci.sType = VkStructureType.IMAGE_CREATE_INFO
    img_ci.imageType = VkImageType._2D
    img_ci.format = FORMAT
    img_ci.extent.width = uint(WIDTH)
    img_ci.extent.height = uint(HEIGHT)
    img_ci.extent.depth = 1u
    img_ci.mipLevels = 1u
    img_ci.arrayLayers = 1u
    img_ci.samples._1 = true
    img_ci.tiling = VkImageTiling.OPTIMAL
    img_ci.usage.color_attachment = true
    img_ci.usage.transfer_src = true
    img_ci.initialLayout = VkImageLayout.UNDEFINED
    var color_image : VkImage
    check(vkCreateImage(device, img_ci, null, color_image), "create image")

    var img_req : VkMemoryRequirements
    vkGetImageMemoryRequirements(device, color_image, img_req)
    var img_alloc : VkMemoryAllocateInfo
    img_alloc.sType = VkStructureType.MEMORY_ALLOCATE_INFO
    img_alloc.allocationSize = img_req.size
    var want_local : VkMemoryPropertyFlags
    want_local.device_local = true
    img_alloc.memoryTypeIndex = find_memory_type(phys, img_req.memoryTypeBits, want_local)
    var img_mem : VkDeviceMemory
    check(vkAllocateMemory(device, img_alloc, null, img_mem), "alloc image memory")
    check(vkBindImageMemory(device, color_image, img_mem, 0ul), "bind image memory")

    // ----- image view -----
    var view_ci : VkImageViewCreateInfo
    view_ci.sType = VkStructureType.IMAGE_VIEW_CREATE_INFO
    view_ci.image = color_image
    view_ci.viewType = VkImageViewType._2D
    view_ci.format = FORMAT
    view_ci.subresourceRange.aspectMask.color = true
    view_ci.subresourceRange.levelCount = 1u
    view_ci.subresourceRange.layerCount = 1u
    var color_view : VkImageView
    check(vkCreateImageView(device, view_ci, null, color_view), "create image view")

    // ----- render pass -----
    var attach : VkAttachmentDescription
    attach.format = FORMAT
    attach.samples._1 = true
    attach.loadOp = VkAttachmentLoadOp.CLEAR
    attach.storeOp = VkAttachmentStoreOp.STORE
    attach.stencilLoadOp = VkAttachmentLoadOp.DONT_CARE
    attach.stencilStoreOp = VkAttachmentStoreOp.DONT_CARE
    attach.initialLayout = VkImageLayout.UNDEFINED
    attach.finalLayout = VkImageLayout.TRANSFER_SRC_OPTIMAL

    var color_ref : VkAttachmentReference
    color_ref.attachment = 0u
    color_ref.layout = VkImageLayout.COLOR_ATTACHMENT_OPTIMAL

    var subpass : VkSubpassDescription
    subpass.pipelineBindPoint = VkPipelineBindPoint.GRAPHICS
    subpass.colorAttachmentCount = 1u
    unsafe {
        subpass.pColorAttachments = addr(color_ref)
    }

    var rp_ci : VkRenderPassCreateInfo
    rp_ci.sType = VkStructureType.RENDER_PASS_CREATE_INFO
    rp_ci.attachmentCount = 1u
    unsafe {
        rp_ci.pAttachments = addr(attach)
    }
    rp_ci.subpassCount = 1u
    unsafe {
        rp_ci.pSubpasses = addr(subpass)
    }
    var render_pass : VkRenderPass
    check(vkCreateRenderPass(device, rp_ci, null, render_pass), "create render pass")

    // ----- framebuffer -----
    var fb_ci : VkFramebufferCreateInfo
    fb_ci.sType = VkStructureType.FRAMEBUFFER_CREATE_INFO
    fb_ci.renderPass = render_pass
    fb_ci.attachmentCount = 1u
    unsafe {
        fb_ci.pAttachments = addr(color_view)
    }
    fb_ci.width = uint(WIDTH)
    fb_ci.height = uint(HEIGHT)
    fb_ci.layers = 1u
    var framebuffer : VkFramebuffer
    check(vkCreateFramebuffer(device, fb_ci, null, framebuffer), "create framebuffer")

    // ----- shaders + pipeline -----
    // tri_vert_spv / tri_frag_spv: SPIR-V words emitted by dasSpirv from triangle_example_shaders.das
    var vert_mod = create_shader_module(device, tri_vert_spv)
    var frag_mod = create_shader_module(device, tri_frag_spv)

    var stages : array<VkPipelineShaderStageCreateInfo>
    stages |> resize(2)
    stages[0].sType = VkStructureType.PIPELINE_SHADER_STAGE_CREATE_INFO
    stages[0].stage.vertex = true
    stages[0].module_ = vert_mod
    stages[0].pName = "main"
    stages[1].sType = VkStructureType.PIPELINE_SHADER_STAGE_CREATE_INFO
    stages[1].stage.fragment = true
    stages[1].module_ = frag_mod
    stages[1].pName = "main"

    var vinput : VkPipelineVertexInputStateCreateInfo
    vinput.sType = VkStructureType.PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO

    var iasm : VkPipelineInputAssemblyStateCreateInfo
    iasm.sType = VkStructureType.PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO
    iasm.topology = VkPrimitiveTopology.TRIANGLE_LIST

    var viewport : VkViewport
    viewport.width = float(WIDTH)
    viewport.height = float(HEIGHT)
    viewport.maxDepth = 1.0f
    var scissor : VkRect2D
    scissor.extent.width = uint(WIDTH)
    scissor.extent.height = uint(HEIGHT)
    var vpstate : VkPipelineViewportStateCreateInfo
    vpstate.sType = VkStructureType.PIPELINE_VIEWPORT_STATE_CREATE_INFO
    vpstate.viewportCount = 1u
    unsafe {
        vpstate.pViewports = addr(viewport)
    }
    vpstate.scissorCount = 1u
    unsafe {
        vpstate.pScissors = addr(scissor)
    }

    var raster : VkPipelineRasterizationStateCreateInfo
    raster.sType = VkStructureType.PIPELINE_RASTERIZATION_STATE_CREATE_INFO
    raster.polygonMode = VkPolygonMode.FILL
    // cullMode left at zero == VK_CULL_MODE_NONE (no bit)
    raster.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    raster.lineWidth = 1.0f

    var msaa : VkPipelineMultisampleStateCreateInfo
    msaa.sType = VkStructureType.PIPELINE_MULTISAMPLE_STATE_CREATE_INFO
    msaa.rasterizationSamples._1 = true

    var blend_att : VkPipelineColorBlendAttachmentState
    blend_att.colorWriteMask.r = true
    blend_att.colorWriteMask.g = true
    blend_att.colorWriteMask.b = true
    blend_att.colorWriteMask.a = true
    var blend : VkPipelineColorBlendStateCreateInfo
    blend.sType = VkStructureType.PIPELINE_COLOR_BLEND_STATE_CREATE_INFO
    blend.attachmentCount = 1u
    unsafe {
        blend.pAttachments = addr(blend_att)
    }

    var pl_ci : VkPipelineLayoutCreateInfo
    pl_ci.sType = VkStructureType.PIPELINE_LAYOUT_CREATE_INFO
    var pipeline_layout : VkPipelineLayout
    check(vkCreatePipelineLayout(device, pl_ci, null, pipeline_layout), "create pipeline layout")

    var gp_ci : VkGraphicsPipelineCreateInfo
    gp_ci.sType = VkStructureType.GRAPHICS_PIPELINE_CREATE_INFO
    gp_ci.stageCount = 2u
    unsafe {
        gp_ci.pStages = addr(stages[0])
        gp_ci.pVertexInputState = addr(vinput)
        gp_ci.pInputAssemblyState = addr(iasm)
        gp_ci.pViewportState = addr(vpstate)
        gp_ci.pRasterizationState = addr(raster)
        gp_ci.pMultisampleState = addr(msaa)
        gp_ci.pColorBlendState = addr(blend)
    }
    gp_ci.layout = pipeline_layout
    gp_ci.renderPass = render_pass
    gp_ci.subpass = 0u
    var pipeline : VkPipeline
    unsafe {
        check(vkCreateGraphicsPipelines(device, null, 1u, addr(gp_ci), null, addr(pipeline)), "create graphics pipeline")
    }

    // ----- readback buffer (host visible) -----
    let buf_size = uint64(WIDTH * HEIGHT * 4)
    var buf_ci : VkBufferCreateInfo
    buf_ci.sType = VkStructureType.BUFFER_CREATE_INFO
    buf_ci.size = buf_size
    buf_ci.usage.transfer_dst = true
    var readback : VkBuffer
    check(vkCreateBuffer(device, buf_ci, null, readback), "create readback buffer")
    var buf_req : VkMemoryRequirements
    vkGetBufferMemoryRequirements(device, readback, buf_req)
    var buf_alloc : VkMemoryAllocateInfo
    buf_alloc.sType = VkStructureType.MEMORY_ALLOCATE_INFO
    buf_alloc.allocationSize = buf_req.size
    var want_host : VkMemoryPropertyFlags
    want_host.host_visible = true
    want_host.host_coherent = true
    buf_alloc.memoryTypeIndex = find_memory_type(phys, buf_req.memoryTypeBits, want_host)
    var buf_mem : VkDeviceMemory
    check(vkAllocateMemory(device, buf_alloc, null, buf_mem), "alloc buffer memory")
    check(vkBindBufferMemory(device, readback, buf_mem, 0ul), "bind buffer memory")

    // ----- command buffer -----
    var pool_ci : VkCommandPoolCreateInfo
    pool_ci.sType = VkStructureType.COMMAND_POOL_CREATE_INFO
    pool_ci.queueFamilyIndex = uint(gfx)
    var pool : VkCommandPool
    check(vkCreateCommandPool(device, pool_ci, null, pool), "create command pool")

    var cb_ai : VkCommandBufferAllocateInfo
    cb_ai.sType = VkStructureType.COMMAND_BUFFER_ALLOCATE_INFO
    cb_ai.commandPool = pool
    cb_ai.level = VkCommandBufferLevel.PRIMARY
    cb_ai.commandBufferCount = 1u
    var cmd : VkCommandBuffer
    unsafe {
        check(vkAllocateCommandBuffers(device, cb_ai, addr(cmd)), "alloc command buffer")
    }

    var begin : VkCommandBufferBeginInfo
    begin.sType = VkStructureType.COMMAND_BUFFER_BEGIN_INFO
    check(vkBeginCommandBuffer(cmd, begin), "begin command buffer")

    var clear : VkClearValue
    clear.color.float32[0] = 0.1f
    clear.color.float32[1] = 0.1f
    clear.color.float32[2] = 0.15f
    clear.color.float32[3] = 1.0f
    var rp_begin : VkRenderPassBeginInfo
    rp_begin.sType = VkStructureType.RENDER_PASS_BEGIN_INFO
    rp_begin.renderPass = render_pass
    rp_begin.framebuffer = framebuffer
    rp_begin.renderArea.extent.width = uint(WIDTH)
    rp_begin.renderArea.extent.height = uint(HEIGHT)
    rp_begin.clearValueCount = 1u
    unsafe {
        rp_begin.pClearValues = addr(clear)
    }
    vkCmdBeginRenderPass(cmd, rp_begin, VkSubpassContents.INLINE)
    vkCmdBindPipeline(cmd, VkPipelineBindPoint.GRAPHICS, pipeline)
    vkCmdDraw(cmd, 3u, 1u, 0u, 0u)
    vkCmdEndRenderPass(cmd)

    // copy image -> buffer (image is already in TRANSFER_SRC_OPTIMAL after the render pass)
    var region : VkBufferImageCopy
    region.imageSubresource.aspectMask.color = true
    region.imageSubresource.layerCount = 1u
    region.imageExtent.width = uint(WIDTH)
    region.imageExtent.height = uint(HEIGHT)
    region.imageExtent.depth = 1u
    unsafe {
        vkCmdCopyImageToBuffer(cmd, color_image, VkImageLayout.TRANSFER_SRC_OPTIMAL, readback, 1u, addr(region))
    }
    check(vkEndCommandBuffer(cmd), "end command buffer")

    // ----- submit + wait -----
    var submit : VkSubmitInfo
    submit.sType = VkStructureType.SUBMIT_INFO
    submit.commandBufferCount = 1u
    unsafe {
        submit.pCommandBuffers = addr(cmd)
    }
    unsafe {
        check(vkQueueSubmit(queue, 1u, addr(submit), null), "queue submit")
    }
    check(vkQueueWaitIdle(queue), "wait idle")

    // ----- read back pixels -----
    var mapped : void? = null
    let map_flags : VkMemoryMapFlags
    unsafe {
        check(vkMapMemory(device, buf_mem, 0ul, buf_size, map_flags, addr(mapped)), "map memory")
    }
    var pixels : array<uint8>
    pixels |> resize(WIDTH * HEIGHT * 4)
    unsafe {
        memcpy(addr<void?>(pixels[0]), mapped, WIDTH * HEIGHT * 4)
    }
    vkUnmapMemory(device, buf_mem)

    write_ppm("triangle.ppm", pixels, WIDTH, HEIGHT)
    print("wrote triangle.ppm\n")

    // Structural verification. NDC verts v0(0,-0.6) v1(0.6,0.6) v2(-0.6,0.6), colored red/green/blue,
    // land at (128,51) / (204,204) / (51,204) in a 256x256 framebuffer. Assert the matching channel
    // dominates inside each region, the centroid blends all three, and the corners keep the clear color.
    var rc = 0
    var fails = 0

    let top = sample(pixels, 128, 80)        // near the red vertex
    let bl = sample(pixels, 75, 190)         // near the blue vertex
    let br = sample(pixels, 180, 190)        // near the green vertex
    let mid = sample(pixels, 128, 150)       // centroid: the blend
    let k0 = sample(pixels, 5, 5)            // corner: clear color
    let k1 = sample(pixels, 250, 5)          // corner: clear color
    print("top   {top}\n")
    print("bl    {bl}\n")
    print("br    {br}\n")
    print("mid   {mid}\n")
    print("corner {k0} {k1}\n")

    fails += vcheck("top is red", top.x > top.y + 40 && top.x > top.z + 40)
    fails += vcheck("bottom-left is blue", bl.z > bl.x + 40 && bl.z > bl.y + 40)
    fails += vcheck("bottom-right is green", br.y > br.x + 40 && br.y > br.z + 40)
    fails += vcheck("centroid is a blend", mid.x > 20 && mid.y > 20 && mid.z > 20)
    fails += vcheck("corner 0 is clear color", is_clear(k0))
    fails += vcheck("corner 1 is clear color", is_clear(k1))

    if (fails != 0) {
        to_log(LOG_ERROR, "verification FAILED: {fails} check(s) wrong\n")
        rc = 1
    } else {
        print("verification OK: 6/6 structural checks passed\n")
    }

    // ----- cleanup -----
    vkDestroyCommandPool(device, pool, null)
    vkDestroyBuffer(device, readback, null)
    vkFreeMemory(device, buf_mem, null)
    vkDestroyPipeline(device, pipeline, null)
    vkDestroyPipelineLayout(device, pipeline_layout, null)
    vkDestroyShaderModule(device, vert_mod, null)
    vkDestroyShaderModule(device, frag_mod, null)
    vkDestroyFramebuffer(device, framebuffer, null)
    vkDestroyRenderPass(device, render_pass, null)
    vkDestroyImageView(device, color_view, null)
    vkDestroyImage(device, color_image, null)
    vkFreeMemory(device, img_mem, null)
    vkDestroyDevice(device, null)
    vkDestroyInstance(instance, null)
    return rc
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
