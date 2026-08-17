options gen2
options _comment_hygiene = true
options indenting = 4

// A Dear ImGui renderer backend (the dasVulkan<->dasImgui bridge), written entirely in daslang -- no
// C++. ImGui is API-agnostic: each frame it produces an `ImDrawData` (vertex/index buffers + draw
// commands + clip rects + texture refs), and a "backend" is just a driver that replays that into a
// graphics API. dasImgui exposes the whole ImDrawData surface to daslang and dasVulkan exposes the GPU
// primitives, so the driver is pure das: upload the merged vertex/index buffers, walk the draw
// commands, set the per-command scissor, bind the command's texture, and issue an indexed draw.
//
// ImGui 1.92's dynamic-texture model is honored: each frame we drain the platform texture queue,
// creating GPU textures on demand and handing ImGui back an `ImTextureID` (the Vulkan descriptor-set
// handle) via SetTexID.
//
// The renderer's own shaders are authored in daslang and lowered to SPIR-V by dasSpirv -- the exact
// equivalent of the stock imgui_impl_vulkan glsl. They live in this same module (the `*_spv` globals
// below) so the bridge is a single self-contained file.

module vulkan_imgui_driver shared

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require imgui

// ===== shaders (daslang -> SPIR-V via dasSpirv) =====
// Vertex maps screen-space ImDrawVert positions into clip space via a push-constant (scale,
// translate); fragment modulates the per-vertex color by the bound texture (font atlas or user image).
// Vulkan NDC is +Y down, matching ImGui's screen space, so no Y flip is needed.

//! 16-byte push constant: `scale` = 2/DisplaySize, `translate` = -1 - DisplayPos*scale.
struct ImguiPC {
    scale     : float2
    translate : float2
}
var @push_constant ipc : ImguiPC

var @in @location = 0 i_pos : float2
var @in @location = 1 i_uv  : float2
var @in @location = 2 i_col : float4
var @out @location = 0 io_col : float4
var @out @location = 1 io_uv  : float2

[vulkan_vertex_shader(name="imgui_vert_spv")]
def imgui_vs {
    io_col = i_col
    io_uv = i_uv
    gl_Position = float4(i_pos * ipc.scale + ipc.translate, 0.0, 1.0)
}

var @set = 0 @binding = 0 i_tex : sampler2D
var @in @location = 0 if_col : float4
var @in @location = 1 if_uv  : float2
var @out @location = 0 if_out : float4

[vulkan_fragment_shader(name="imgui_frag_spv")]
def imgui_fs {
    if_out = if_col * texture(i_tex, if_uv)
}

// ===== renderer =====

// sizeof(ImDrawVert) = ImVec2 pos (8) + ImVec2 uv (8) + ImU32 col (4); ImDrawIdx = unsigned short.
let private VERT_SIZE = 20
let private IDX_SIZE  = 2

//! Map a host-visible range for WRITING and invoke `blk` with the raw destination pointer, unmapping
//! afterwards. (vulkan_boost's map_memory_to_array is read-only -- it copies out then unmaps.) The
//! memory is HOST_COHERENT, so writes are visible to the GPU without an explicit flush.
def private map_and_write(device : Device; memory : DeviceMemory; size : uint64; blk : block<(dst : void?) : void>) {
    var mapped : void? = null
    let flags : VkMemoryMapFlags
    unsafe {
        vk_check(vkMapMemory(boost_value_to_vk(device), boost_value_to_vk(memory), 0ul, size, flags, addr(mapped)), null)
    }
    invoke(blk, mapped)
    vkUnmapMemory(boost_value_to_vk(device), boost_value_to_vk(memory))
}

//! GPU image backing one ImTextureData. The descriptor set that points at it is owned by the
//! renderer's pool (freed wholesale at shutdown), so only the image/memory/view are tracked here.
struct private ImguiTexture {
    image  : Image
    memory : DeviceMemory
    view   : ImageView
    tid    : uint64     // the ImTextureID (descriptor-set handle) -- used to find this texture on WantUpdates
}

struct public ImguiVkRenderer {
    sampler     : Sampler
    set_layouts : array<DescriptorSetLayout>
    desc_pool   : DescriptorPool
    pipe_layout : PipelineLayout
    pipeline    : Pipeline
    vert_mod    : ShaderModule
    frag_mod    : ShaderModule
    vbuf        : HostBuffer
    ibuf        : HostBuffer
    vbuf_cap    : uint64
    ibuf_cap    : uint64
    textures    : array<ImguiTexture>
}

def public finalize(var r : ImguiVkRenderer) {
    unsafe {
        for (t in r.textures) {
            // Vulkan requires the image destroyed before its backing memory is freed.
            delete t.view
            delete t.image
            delete t.memory
        }
        delete r.textures
        if (r.vbuf_cap > 0ul) {
            delete r.vbuf
        }
        if (r.ibuf_cap > 0ul) {
            delete r.ibuf
        }
        delete r.pipeline
        delete r.frag_mod
        delete r.vert_mod
        delete r.pipe_layout
        delete r.desc_pool
        delete r.set_layouts
        delete r.sampler
    }
}

//! ImGui's pipeline: per-vertex (pos, uv, col-as-rgba8), alpha blend, no depth, no cull, dynamic
//! viewport+scissor (paired with vulkan_window's draw_frame). Built against `render_pass` so it can
//! draw into the same pass as the app's own geometry.
def private build_imgui_pipeline(device : Device; render_pass : RenderPass; layout : PipelineLayout;   // nolint:STYLE038 - flat one-call-per-item pipeline setup
                                 vert, frag : ShaderModule) : Pipeline {
    var stages : array<VkPipelineShaderStageCreateInfo>
    stages |> resize(2)
    stages[0] = VkPipelineShaderStageCreateInfo()
    stages[0].stage.vertex = true
    stages[0].module_ = boost_value_to_vk(vert)
    stages[0].pName = "main"
    stages[1] = VkPipelineShaderStageCreateInfo()
    stages[1].stage.fragment = true
    stages[1].module_ = boost_value_to_vk(frag)
    stages[1].pName = "main"

    var vbind : VkVertexInputBindingDescription
    vbind.binding = 0u
    vbind.stride = uint(VERT_SIZE)
    vbind.inputRate = VkVertexInputRate.VERTEX
    var vbindings : array<VkVertexInputBindingDescription>
    vbindings |> push(vbind)
    var a_pos : VkVertexInputAttributeDescription
    a_pos.location = 0u
    a_pos.binding = 0u
    a_pos.format = VkFormat.R32G32_SFLOAT
    a_pos.offset = 0u
    var a_uv : VkVertexInputAttributeDescription
    a_uv.location = 1u
    a_uv.binding = 0u
    a_uv.format = VkFormat.R32G32_SFLOAT
    a_uv.offset = 8u
    var a_col : VkVertexInputAttributeDescription
    a_col.location = 2u
    a_col.binding = 0u
    a_col.format = VkFormat.R8G8B8A8_UNORM
    a_col.offset = 16u
    var vattrs <- [a_pos, a_uv, a_col]

    var vinput = VkPipelineVertexInputStateCreateInfo()
    vinput.vertexBindingDescriptionCount = 1u
    vinput.vertexAttributeDescriptionCount = 3u
    var iasm = VkPipelineInputAssemblyStateCreateInfo()
    iasm.topology = VkPrimitiveTopology.TRIANGLE_LIST

    var vpstate = VkPipelineViewportStateCreateInfo()
    vpstate.viewportCount = 1u
    vpstate.scissorCount = 1u

    var raster = VkPipelineRasterizationStateCreateInfo()
    raster.polygonMode = VkPolygonMode.FILL
    raster.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    raster.lineWidth = 1.0f       // cullMode left at default (VK_CULL_MODE_NONE) -- UI is single-sided
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true

    var blend_att : VkPipelineColorBlendAttachmentState
    blend_att.blendEnable = VK_TRUE
    blend_att.srcColorBlendFactor = VkBlendFactor.SRC_ALPHA
    blend_att.dstColorBlendFactor = VkBlendFactor.ONE_MINUS_SRC_ALPHA
    blend_att.colorBlendOp = VkBlendOp.ADD
    blend_att.srcAlphaBlendFactor = VkBlendFactor.ONE
    blend_att.dstAlphaBlendFactor = VkBlendFactor.ONE_MINUS_SRC_ALPHA
    blend_att.alphaBlendOp = VkBlendOp.ADD
    blend_att.colorWriteMask.r = true
    blend_att.colorWriteMask.g = true
    blend_att.colorWriteMask.b = true
    blend_att.colorWriteMask.a = true
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 1u

    var dyn_states <- [VkDynamicState.VIEWPORT, VkDynamicState.SCISSOR]
    var dynstate = VkPipelineDynamicStateCreateInfo()
    dynstate.dynamicStateCount = 2u

    var gp = VkGraphicsPipelineCreateInfo()
    gp.stageCount = 2u
    gp.layout = boost_value_to_vk(layout)
    gp.renderPass = boost_value_to_vk(render_pass)
    var h : VkPipeline
    unsafe {
        dynstate.pDynamicStates = addr(dyn_states[0])
        gp.pDynamicState = addr(dynstate)
        blend.pAttachments = addr(blend_att)
        vinput.pVertexBindingDescriptions = addr(vbindings[0])
        vinput.pVertexAttributeDescriptions = addr(vattrs[0])
        gp.pStages = addr(stages[0])
        gp.pVertexInputState = addr(vinput)
        gp.pInputAssemblyState = addr(iasm)
        gp.pViewportState = addr(vpstate)
        gp.pRasterizationState = addr(raster)
        gp.pMultisampleState = addr(msaa)
        gp.pColorBlendState = addr(blend)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(gp), null, addr(h)), null)
    }
    delete dyn_states
    delete vbindings
    delete vattrs
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! Build the renderer: sampler, descriptor layout (one combined image sampler), a pool sized for a
//! handful of textures, the pipeline layout (push constant + sampler) reflected from the imgui
//! shaders, and the pipeline itself. `render_pass` is the pass the UI will be recorded into.
def public imgui_vk_create(device : Device; render_pass : RenderPass) : ImguiVkRenderer {
    let sci = SamplerCreateInfo(
        magFilter = VkFilter.LINEAR,
        minFilter = VkFilter.LINEAR,
        mipmapMode = VkSamplerMipmapMode.LINEAR,
        addressModeU = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeV = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeW = VkSamplerAddressMode.CLAMP_TO_EDGE,
        maxLod = 1.0f)
    var inscope sampler <- create_sampler(device, sci)

    var reflections <- [decode_reflection(imgui_vert_spv_reflect), decode_reflection(imgui_frag_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)

    var dpci = DescriptorPoolCreateInfo(
        maxSets = 16u,
        pPoolSizes <- [DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 16u)])
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    delete reflections

    var inscope vert_mod <- create_shader_module(device, imgui_vert_spv)
    var inscope frag_mod <- create_shader_module(device, imgui_frag_spv)
    var inscope pipeline <- build_imgui_pipeline(device, render_pass, pipe_layout, vert_mod, frag_mod)

    return <- ImguiVkRenderer(
        sampler <- sampler, set_layouts <- set_layouts, desc_pool <- desc_pool,
        pipe_layout <- pipe_layout, pipeline <- pipeline, vert_mod <- vert_mod, frag_mod <- frag_mod,
        vbuf_cap = 0ul, ibuf_cap = 0ul)
}

//! Allocate one descriptor set pointing at (sampler, view) and return its raw handle.
def private make_texture_set(r : ImguiVkRenderer; device : Device; view : ImageView) : VkDescriptorSet {
    let dev = boost_value_to_vk(device)
    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(r.desc_pool)
    dsai.descriptorSetCount = 1u
    var raw_layout = boost_value_to_vk(r.set_layouts[0])
    var raw_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_layout)
        vk_check(vkAllocateDescriptorSets(dev, dsai, addr(raw_set)), null)
    }
    var writes : array<WriteDescriptorSet>
    var w = WriteDescriptorSet(
        dstSet = vk_value_to_boost(raw_set),
        dstBinding = 0u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER,
        descriptorCount = 1u)
    w.pImageInfo |> push(DescriptorImageInfo(
        sampler = weak_copy(r.sampler),
        imageView = weak_copy(view),
        imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL))
    writes |> emplace(w)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)
    return raw_set
}

//! Stage `nbytes` of RGBA8 `pixels` into `image` (extent w x h), leaving it SHADER_READ_ONLY_OPTIMAL.
//! `from_layout` is its current layout: UNDEFINED when freshly created, SHADER_READ_ONLY_OPTIMAL when
//! re-uploading a sampled atlas. Own submit -- the caller already idled the queue, so no in-flight race.
def private upload_pixels_to_image(device : Device; phys : VkPhysicalDevice; queue : VkQueue; pool : CommandPool;
                                   image : Image; w, h : int; nbytes : uint64; pixels : void?; from_layout : VkImageLayout) {
    var staging_usage : VkBufferUsageFlags
    staging_usage.transfer_src = true
    var inscope staging <- create_host_buffer(device, phys, nbytes, staging_usage)
    map_and_write(device, staging.memory, nbytes) $(dst) {
        unsafe {
            memcpy(dst, pixels, nbytes)
        }
    }
    run_cmd_sync(device, pool, queue) $(cmd) {
        var to_dst : VkAccessFlags; to_dst.transfer_write = true
        var to_read : VkAccessFlags; to_read.shader_read = true
        let none : VkAccessFlags
        var xfer : VkPipelineStageFlags; xfer.transfer = true
        var frag_stage : VkPipelineStageFlags; frag_stage.fragment_shader = true
        var top : VkPipelineStageFlags; top.top_of_pipe = true
        // a fresh (UNDEFINED) image has nothing to wait on; a re-upload waits on prior fragment reads
        var src_access = none
        var src_stage = top
        if (from_layout != VkImageLayout.UNDEFINED) {
            src_access = to_read
            src_stage = frag_stage
        }
        transition_image(cmd, image, from_layout, VkImageLayout.TRANSFER_DST_OPTIMAL,
            src_access, to_dst, src_stage, xfer)
        var region : BufferImageCopy
        region.imageSubresource.aspectMask.color = true
        region.imageSubresource.layerCount = 1u
        region.imageExtent.width = uint(w)
        region.imageExtent.height = uint(h)
        region.imageExtent.depth = 1u
        var regions : array<BufferImageCopy>
        regions |> emplace(region)
        cmd_copy_buffer_to_image(cmd, staging.buffer, image, VkImageLayout.TRANSFER_DST_OPTIMAL, regions)
        transition_image(cmd, image, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.SHADER_READ_ONLY_OPTIMAL,
            to_dst, to_read, xfer, frag_stage)
    }
}

//! Create a GPU texture for one ImTextureData (RGBA8) and register it; returns the ImTextureID
//! (the descriptor-set handle as a u64).
def private create_imgui_texture(var r : ImguiVkRenderer; device : Device; phys : VkPhysicalDevice;
                                 queue : VkQueue; pool : CommandPool; tex : ImTextureData?) : uint64 {
    let dev = boost_value_to_vk(device)
    let w = tex.Width
    let h = tex.Height
    let nbytes = uint64(w) * uint64(h) * uint64(tex.BytesPerPixel)   // widen before multiply

    var ici = ImageCreateInfo(
        imageType = VkImageType._2D,
        format = VkFormat.R8G8B8A8_UNORM,
        mipLevels = 1u,
        arrayLayers = 1u,
        tiling = VkImageTiling.OPTIMAL,
        initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = uint(w)
    ici.extent.height = uint(h)
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.sampled = true
    ici.usage.transfer_dst = true
    var inscope image <- create_image(device, ici)
    var ireq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(image), ireq)
    var iwant : VkMemoryPropertyFlags
    iwant.device_local = true
    let imai = MemoryAllocateInfo(
        allocationSize = ireq.size,
        memoryTypeIndex = find_memory_type(phys, ireq.memoryTypeBits, iwant))
    var inscope memory <- allocate_memory(device, imai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(image), boost_value_to_vk(memory), 0ul), null)

    var tvci = ImageViewCreateInfo(
        image = weak_copy(image),
        viewType = VkImageViewType._2D,
        format = VkFormat.R8G8B8A8_UNORM)
    tvci.subresourceRange.aspectMask.color = true
    tvci.subresourceRange.levelCount = 1u
    tvci.subresourceRange.layerCount = 1u
    var inscope view <- create_image_view(device, tvci)

    upload_pixels_to_image(device, phys, queue, pool, image, w, h, nbytes,
        unsafe(reinterpret<void?>(tex.Pixels)), VkImageLayout.UNDEFINED)

    let raw_set = make_texture_set(r, device, view)
    let tid = unsafe(reinterpret<uint64>(raw_set))
    r.textures |> emplace(ImguiTexture(image <- image, memory <- memory, view <- view, tid = tid))
    return tid
}

//! Re-upload an already-created texture's pixels (ImGui 1.92 bakes glyphs on demand and flags the
//! atlas WantUpdates -- without this, on-demand glyphs render blank). Full re-upload of the current
//! pixels into the existing image; atlas updates are infrequent so the per-glyph cost is negligible.
def private update_imgui_texture(r : ImguiVkRenderer; device : Device; phys : VkPhysicalDevice;
                                 queue : VkQueue; pool : CommandPool; tex : ImTextureData?) {
    let th & = unsafe(*tex)        // GetTexID is a method -- needs a value, not the pointer
    let want = th.GetTexID()
    let w = tex.Width
    let h = tex.Height
    let nbytes = uint64(w) * uint64(h) * uint64(tex.BytesPerPixel)   // widen before multiply
    for (t in r.textures) {
        if (t.tid == want) {
            upload_pixels_to_image(device, phys, queue, pool, t.image, w, h, nbytes,
                unsafe(reinterpret<void?>(tex.Pixels)), VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
            return
        }
    }
}

//! Drain ImGui's per-frame texture queue: create GPU textures ImGui asked for (font atlas, user
//! images) and hand back their ids. Call once per frame after ImGui::Render, before recording.
def public imgui_vk_update_textures(var r : ImguiVkRenderer; device : Device; phys : VkPhysicalDevice;
                                    queue : VkQueue; pool : CommandPool) {
    // `ImDrawData.Textures` is a *pointer* to the shared texture list; the embedded list lives on
    // `GetPlatformIO().Textures` -- the canonical place a backend drains texture create/destroy
    // requests (imgui.h: "iterate platform_io->Textures[]").
    let pio & = unsafe(GetPlatformIO())
    for (i in range(length(pio.Textures))) {
        // const-strip: `pio` is const, so the element is a const-pointee pointer; SetTexID/SetStatus
        // below need a mutable ImTextureData.
        var tex = unsafe(reinterpret<ImTextureData?>(pio.Textures[i]))
        if (tex == null) {
            continue
        }
        if (tex.Status == ImTextureStatus.WantCreate) {
            let tid = create_imgui_texture(r, device, phys, queue, pool, tex)
            var th & = unsafe(*tex)
            th |> SetTexID(tid)
            th |> SetStatus(ImTextureStatus.OK)
        } elif (tex.Status == ImTextureStatus.WantUpdates) {
            // 1.92 on-demand glyph baking: re-upload so newly added glyphs reach the GPU atlas.
            update_imgui_texture(r, device, phys, queue, pool, tex)
            var th & = unsafe(*tex)
            th |> SetStatus(ImTextureStatus.OK)
        }
        // WantDestroy is left for renderer finalize (the font atlas isn't destroyed mid-run); a churny
        // user-texture workload would want explicit free + ack here.
    }
}

//! Record the UI into `cmd` (inside an already-begun render pass). Uploads the merged vertex/index
//! buffers, then replays each draw command with its own scissor + texture.
def public imgui_vk_render(var r : ImguiVkRenderer; device : Device; phys : VkPhysicalDevice; cmd : CommandBuffer) {   // nolint:STYLE038 - flat command-record run
    let dd = GetDrawData()
    // nothing to draw, or a zero DisplaySize (minimized/hidden) -- skip so ipc.scale never divides by 0
    if (dd == null || dd.TotalVtxCount == 0 || dd.DisplaySize.x <= 0.0f || dd.DisplaySize.y <= 0.0f) {
        return
    }
    let vtx_bytes = uint64(dd.TotalVtxCount) * uint64(VERT_SIZE)
    let idx_bytes = uint64(dd.TotalIdxCount) * uint64(IDX_SIZE)

    // grow host-visible vertex/index buffers if needed (with slack to avoid per-frame churn)
    if (r.vbuf_cap < vtx_bytes) {
        if (r.vbuf_cap > 0ul) {
            unsafe { delete r.vbuf }
        }
        var vusage : VkBufferUsageFlags
        vusage.vertex_buffer = true
        let cap = vtx_bytes + vtx_bytes / 2ul + 4096ul
        r.vbuf <- create_host_buffer(device, phys, cap, vusage)
        r.vbuf_cap = cap
    }
    if (r.ibuf_cap < idx_bytes) {
        if (r.ibuf_cap > 0ul) {
            unsafe { delete r.ibuf }
        }
        var iusage : VkBufferUsageFlags
        iusage.index_buffer = true
        let cap = idx_bytes + idx_bytes / 2ul + 4096ul
        r.ibuf <- create_host_buffer(device, phys, cap, iusage)
        r.ibuf_cap = cap
    }

    // nolint:STYLE014
    // upload the per-command-list vertex/index data back to back into the mapped GPU buffers.
    // NOTE the intptr arithmetic (mirrors write_field): `reinterpret<uint8?>(dst) + off` parses as
    // `reinterpret<uint8?>((dst) + off)` — cast syntax is juxtaposition, the parens are NOT call
    // parens — so the add runs on the void? at stride sizeof(void) = 0 and silently vanishes,
    // uploading every list at offset 0 (last list wins; single-window UIs never noticed).
    map_and_write(device, r.vbuf.memory, vtx_bytes) $(dst) {
        var off = 0ul
        for (i in range(length(dd.CmdLists))) {
            let cl = dd.CmdLists[i]
            let cnt = length(cl.VtxBuffer)
            if (cnt > 0) {
                unsafe {
                    let d = reinterpret<void?>(intptr(dst) + off)
                    memcpy(d, addr<void?>(cl.VtxBuffer[0]), cnt * VERT_SIZE)
                }
                off += uint64(cnt * VERT_SIZE)
            }
        }
    }
    map_and_write(device, r.ibuf.memory, idx_bytes) $(dst) {
        var off = 0ul
        for (i in range(length(dd.CmdLists))) {
            let cl = dd.CmdLists[i]
            let cnt = length(cl.IdxBuffer)
            if (cnt > 0) {
                unsafe {
                    let d = reinterpret<void?>(intptr(dst) + off)
                    memcpy(d, addr<void?>(cl.IdxBuffer[0]), cnt * IDX_SIZE)
                }
                off += uint64(cnt * IDX_SIZE)
            }
        }
    }

    cmd_bind_pipeline(cmd, r.pipeline)

    let ds = dd.DisplaySize
    let dp = dd.DisplayPos
    let fb = dd.FramebufferScale
    ipc.scale = float2(2.0f / ds.x, 2.0f / ds.y)
    ipc.translate = float2(-1.0f - dp.x * ipc.scale.x, -1.0f - dp.y * ipc.scale.y)
    imgui_vs_push_constants(cmd, r.pipe_layout)

    var vbufs <- [weak_copy(r.vbuf.buffer)]
    var voffs : array<uint64>
    voffs |> push(0ul)
    cmd_bind_vertex_buffers(cmd, 0u, vbufs, voffs)
    cmd_bind_index_buffer(cmd, weak_copy(r.ibuf.buffer), 0ul, VkIndexType.UINT16)
    delete voffs
    delete vbufs

    var global_idx = 0u
    var global_vtx = 0
    for (i in range(length(dd.CmdLists))) {
        let cl = dd.CmdLists[i]
        for (j in range(length(cl.CmdBuffer))) {
            let c & = unsafe(cl.CmdBuffer[j])
            if (c.ElemCount == 0u) {
                continue
            }
            let clip = c.ClipRect
            var minx = (clip.x - dp.x) * fb.x
            var miny = (clip.y - dp.y) * fb.y
            var maxx = (clip.z - dp.x) * fb.x
            var maxy = (clip.w - dp.y) * fb.y
            // clamp to the framebuffer on BOTH ends -- a partially off-screen / docked window can push
            // the clip rect past the target, and Vulkan requires the scissor within the framebuffer.
            let fbw = ds.x * fb.x
            let fbh = ds.y * fb.y
            if (minx < 0.0f) {
                minx = 0.0f
            }
            if (miny < 0.0f) {
                miny = 0.0f
            }
            if (maxx > fbw) {
                maxx = fbw
            }
            if (maxy > fbh) {
                maxy = fbh
            }
            if (maxx <= minx || maxy <= miny) {
                continue
            }
            var sc : Rect2D
            sc.offset.x = int(minx)
            sc.offset.y = int(miny)
            sc.extent.width = uint(maxx - minx)
            sc.extent.height = uint(maxy - miny)
            var scis : array<Rect2D>
            scis |> push(sc)
            cmd_set_scissor(cmd, 0u, scis)
            delete scis

            let tid = c.GetTexID()
            if (tid != 0ul) {
                var sets <- [vk_value_to_boost(unsafe(reinterpret<VkDescriptorSet>(tid)))]
                var no_dyn : array<uint>
                cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, r.pipe_layout, 0u, sets, no_dyn)
                delete sets
                delete no_dyn
            }
            cmd_draw_indexed(cmd, c.ElemCount, 1u, c.IdxOffset + global_idx, int(c.VtxOffset) + global_vtx, 0u)
        }
        global_idx += uint(length(cl.IdxBuffer))
        global_vtx += length(cl.VtxBuffer)
    }
}
